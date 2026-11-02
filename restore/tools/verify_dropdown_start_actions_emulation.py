"""New-game jump table and map selection, with actual reset/stream/list bodies."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/c-only-debug/Debug/restore_dropdown_start_actions_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/c-only/Release/restore_dropdown_start_actions_verify.exe');args=ap.parse_args()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
    q=table+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def signed(v):return v if v<0x80000000 else v-0x100000000
def text(a):
    out=bytearray()
    for i in range(512):
        b=get(a+i,1)
        if not b:return bytes(out)
        out.append(b)
    raise AssertionError('unterminated string')
def hash_text(a):
    value=2166136261
    for b in text(a):value=((value^b)*16777619)&0xffffffff
    return value
core={};helpers={};context={};sets={};exports=['metadata/dropdown-start-actions-ghidra-window.json','metadata/dropdown-action-native-cfg.json','metadata/dropdown-menu-native.json']
w=json.loads((ROOT/exports[0]).read_text());assert w['native_sha256']==digest;w=w['window'];lo=int(w['address'],16);raw=bytes.fromhex(w['hex']);assert len(raw)==w['length'] and not w['gaps'] and bytes(vm.mem_read(lo,len(raw)))==raw
assert list(struct.unpack('<4I',bytes(vm.mem_read(0x40243c,16))))==[0x402413,0x402407,0x402413,0x402413]
assert bytes(vm.mem_read(0x4023f4,19)).hex()=='33c0a0b427700083f8037718ff24853c244000'
cs=Cs(CS_ARCH_X86,CS_MODE_32);todo=[0x4023a0];sets[0x4023a0]=set()
while todo:
    address=todo.pop()
    if address in core:continue
    assert 0x4023a0<=address<0x40243b
    ins=next(cs.disasm(bytes(vm.mem_read(address,16)),address,count=1));core[address]={'address':f'{address:08x}','bytes':ins.bytes.hex(),'instruction':ins.mnemonic+' '+ins.op_str};sets[0x4023a0].add(address)
    if ins.mnemonic.startswith('ret'):continue
    if ins.mnemonic=='jmp':
        if address==0x402400:todo.extend([get(0x40243c+i*4) for i in range(4)])
        else:todo.append(int(ins.op_str,16))
    elif ins.mnemonic.startswith('j'):todo.extend([address+ins.size,int(ins.op_str,16)])
    else:todo.append(address+ins.size)
def attach(entry,f,target):
    a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];sets[entry]=set()
    for x in a['items']:
        address=int(x['address'],16);b=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(b)))==b;target[address]=x;sets[entry].add(address)
d=json.loads((ROOT/exports[1]).read_text());assert d['native_sha256']==digest;attach(0x409580,d['functions']['00409580'],core)
selector_window='metadata/dropdown-start-selector-ghidra-window.json';exports.append(selector_window)
sw=json.loads((ROOT/selector_window).read_text());assert sw['native_sha256']==digest;sw=sw['window'];start=int(sw['address'],16);sb=bytes.fromhex(sw['hex'])
assert not sw['gaps'] and len(sb)==sw['length'] and bytes(vm.mem_read(start,len(sb)))==sb
assert all(start<=a and a+len(bytes.fromhex(core[a]['bytes']))<=start+len(sb) for a in sets[0x409580])
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
    d=json.loads(path.read_text());assert d['native_sha256']==digest;used=False
    for f in d['entries']:
        entry=int(f['requested_address'],16)
        if entry in [0x4d3c40,0x55f449,0x4cdff0,0x4ce3a0]:attach(entry,f,helpers);used=True
    if used:exports.append(path.relative_to(ROOT).as_posix())
assert len(core)==49 and len(helpers)==200
d=json.loads((ROOT/'metadata/dropdown-menu-native.json').read_text());assert d['native_sha256']==digest
for entry in [0x425f20,0x4260d0]:attach(entry,d['functions'][f'{entry:08x}'],context)
regions=[(0x6057b0,0x2b0),(0x6fdc90,0x90),(0x7027a0,48),(0x705700,64),(0x78d790,64),(0xbc9680,48),(0xd3b180,96),(0xe20980,48),(0xe36ee0,48),(0xe38dc0,256),(0xe94df0,48),(0xe92be0,0x240),(0xf53f20,0x70),(0xf55b70,48),(0x6f2000,64),(0x6f3000,0x400)]
names=[b'Alpha.map',b'Beta.extra.map',b'.hidden.map',b'no_extension',b'Gamma.MAP',b'zeta.map',b'  spaced.name.map',b'Last.'];profiles=[b'Restore',b'Alpha.Beta',b'',b'City']
sound=0xe92bf8;endpoints={0x516bb0:(0,0),0x516990:(0,0),0x4cb6a0:(0,0),0x5169c0:(0,0),0x41c740:(2,8),0x41ca10:(1,4)}
media={0x56f2d0:1,0x56f290:3,0x56f300:2,0x56f29c:2,0x56f304:1};imports={};descriptor=base+u32(opt+104)
while get(descriptor+12):
    original=get(descriptor) or get(descriptor+16);iat=get(descriptor+16);index=0
    while get(base+original+index*4):
        value=get(base+original+index*4)
        if not value&0x80000000:imports[base+iat+index*4]=text(base+value+2).decode('ascii')
        index+=1
    descriptor+=20
expected_imports={0x56f2d0:'_AIL_close_stream@4',0x56f290:'_AIL_open_stream@12',0x56f300:'_AIL_set_stream_loop_count@8',0x56f29c:'_AIL_set_stream_volume@8',0x56f304:'_AIL_start_stream@4'}
for i,(iat,count) in enumerate(media.items()):
    assert imports[iat]==expected_imports[iat];address=STOP+0x100+i*0x10;put(iat,address);endpoints[address]=(count,count*4)
media_traps={STOP+0x100+i*0x10:iat for i,iat in enumerate(media)}
executed=set();table_indices=set();events=[];p=[]
def event(address,argv):events.append([address,len(argv)]+[v&0xffffffff for v in argv]+[0]*(6-len(argv))+[get(0xe38dda,1),get(0xe38dca,1),get(0x605a3c),get(0xe38e64,1),get(0xf55b88),get(sound+0x1d8),get(0x6057d0),get(0xd3b198)])
def code(cpu,a,n,user):
    if a==STOP:cpu.emu_stop();return
    if a not in endpoints:
        assert a in core or a in helpers or a in context,f'native escaped {a:08x}';executed.add(a)
        if a==0x402400:table_indices.add(cpu.reg_read(UC_X86_REG_EAX))
        return
    count,cleanup=endpoints[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(sp+4+i*4) for i in range(count)];result=1;address=media_traps.get(a,a)
    if address in [0x516bb0,0x516990,0x4cb6a0]:
        event(address,[])
        if p[16]&1:put(0xe38dda,p[17],1)
        if p[16]&2:put(0xe38e64,p[18],1)
        if p[16]&4:put(0xf55b88,p[19])
    elif address==0x5169c0:event(address,[]);result=0x6f3340
    elif address==0x41c740:
        assert cpu.reg_read(UC_X86_REG_ECX)==0x6057c8;event(address,[0x6057c8,hash_text(argv[0]),argv[1]&255,0x576260 if argv[0]==0x576260 else 0]);count_signed=signed(p[14]);assert count_signed<=8
        for at,val in [(0x6057cc,0x6f3000),(0x6057d0,p[14]),(0x6057d4,8),(0x6057d8,0x6f3100),(0x6057dc,512),(0x6057e0,max(0,count_signed)*64)]:put(at,val)
        for i in range(max(0,count_signed)):put(0x6f3000+i*4,i*64);vm.mem_write(0x6f3100+i*64,names[i]+b'\0')
        if p[16]&32:put(0xe38dda,p[17],1);put(0x6059f4,0xabcdef01)
        result=p[28]
    elif address==0x41ca10:assert cpu.reg_read(UC_X86_REG_ECX)==0x6057c8 and argv==[0x562370];event(address,[0x6057c8,0x562370])
    else:
        event(address,argv)
        if address==0x56f2d0 and p[16]&8:put(sound+0x178,p[20])
        if address==0x56f290:
            result=p[13]
            if p[16]&16:
                for offset,v in [(0x14c,p[21]),(0x154,p[21]^0x8000),(0x158,p[21]^0xffff)]:put(sound+offset,v)
    cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,get(sp))
def write(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x100000 or any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatched native write {a:08x}/{n}'
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def native(case):
    global p,events
    p=case;events=[]
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[2]+j*37+i*11)&255 for j in range(n)))
    for a,v,n in [(0x605a3c,p[3],4),(0x7027b4,p[4],1),(0xe38dda,p[5],1),(0xe38dca,p[6],1),(0xe38e64,p[7],1),(0xf55b88,p[8],4),(sound+0x200,p[9],4),(sound+0x178,p[11],4),(0x705728,p[22],4),(0x705724,0x6f2000,4),(0x70572c,1,4),(0x705730,1,4),(0x6f2008,0x6f2020,4),(0x6f200c,1,2),(0x6f2024,0x409580 if p[0] else 0x4023a0,4),(0x6f202c,p[23],4),(0xe38e03,1,1)]:put(a,v,n)
    for i in range(3):put(sound+0x1d8+i*4,p[10])
    for i,xor in enumerate([0,1,0x8000,0xffff]):put(sound+0x14c+i*4,p[12]^xor)
    vm.mem_write(0x6f3340,profiles[p[15]]+b'\0');sp=STACK+0x80000;put(sp,STOP);cleanup=0
    if p[0]==2:entry=0x4d3c40
    elif p[0]==3:entry=0x4cdff0;put(sp+4,0x576260);put(sp+8,p[27])
    elif p[0]==4:entry=0x55f449;put(sp+4,0x5761ac);put(sp+8,p[25]);put(sp+12,p[26]);cleanup=12
    else:entry=0x425f20 if p[1]==4 else 0x409580 if p[0] else 0x4023a0;put(sp+4,0x6f2000);put(sp+8,1)
    saved={UC_X86_REG_EBX:p[30]^0x12345678,UC_X86_REG_ESI:p[30]^0x789abcde,UC_X86_REG_EDI:p[30]^0xabcdef12,UC_X86_REG_EBP:p[30]^0x2468ace0}
    for r,v in saved.items():vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_EAX,p[30]);vm.reg_write(UC_X86_REG_ECX,sound if p[0]==4 else p[24]);vm.reg_write(UC_X86_REG_EDX,p[31]);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
    vm.emu_start(entry,STOP,count=10000);assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup
    for r,v in saved.items():assert vm.reg_read(r)==v
    result=vm.reg_read(UC_X86_REG_EAX) if p[0]==4 or p[0]<2 and p[1]==4 else 0
    return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
rng=random.Random(0x4023a0);cases=[]
def add(op=0,route=4,editor=0,campaign=1,music=1,enabled=1,disabled=0,old=0,opened=0x12345678,count=2,profile=0,mask=0,fmt=1,channel=0,volume=100):
    q=[rng.getrandbits(32) for _ in range(32)]
    for index,v in {0:op,1:route,3:editor,4:campaign,7:music,8:enabled,9:disabled,10:old,12:volume,13:opened,14:count,15:profile,16:mask,22:1,25:channel,27:fmt}.items():q[index]=v&0xffffffff
    cases.append(q)
for route,editor,campaign in itertools.product(range(5),[0,1,256,0xffffffff],[0,1,2,3,4,255,256]):add(route=route,editor=editor,campaign=campaign)
for editor,campaign,music,enabled in itertools.product([0,1,256,0xffffffff],range(4),[0,1,256,255],[0,1,256,0xffffffff]):add(editor=editor,campaign=campaign,music=music,enabled=enabled)
for campaign in range(256):add(campaign=campaign,music=0)
for channel,disabled,old,opened,volume in itertools.product(range(4),[0,1,256],[0,1,0x12345678],[0,1,0x80000000],[0,100,0xffffffff]):add(op=4,channel=channel,disabled=disabled,old=old,opened=opened,volume=volume,mask=24)
for fmt,count,profile in itertools.product([0,1,256,0xffffffff],[0,1,2,4,8,0xffffffff],range(4)):add(op=3,fmt=fmt,count=count,profile=profile)
for route,count,mask in itertools.product(range(5),[0,1,4,8,0xffffffff],[0,32]):add(op=1,route=route,count=count,mask=mask)
for mask,music in itertools.product(range(32),[0,1,256]):
    add(campaign=[0,1,3][mask%3],mask=mask,old=1);cases[-1][18]=music
for _ in range(32):add(op=2)
expected=[native(case) for case in cases]
assert set(core)<=executed and sets[0x4d3c40]<=executed and sets[0x55f449]<=executed and sets[0x4ce3a0]<=executed and table_indices=={0,1,2,3}
sources=['runtime/dropdown_start_actions.c','include/dropdown_start_actions.h','runtime/menu_callbacks.c','runtime/dropdown_actions.c','runtime/dropdown_system_actions.c','runtime/dropdown_menu_boundary.c','runtime/dropdown_menu_options.c','src/unit_01809.c','src/unit_01739.c','src/unit_01745.c','src/unit_02707.c','src/unit_00410.c','src/unit_00413.c','tests/dropdown_start_actions.c','tests/dropdown_start_actions_real.c','tests/dropdown_start_map_real.c','include/sound_streams.h','include/sound_initialization.h','include/media_bindings.h','include/family_menu.h','include/file_list.h','include/dropdown_menu.h','include/standalone_data.h','tools/verify_dropdown_start_actions_emulation.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':len(cases),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe),'source_hashes':{s:sha(ROOT/s) for s in sources},'native_exports':{s:sha(ROOT/s) for s in exports},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'computer_UI_automation_used':False,'coverage':{f'{a:08x}':{'executed':len(sets[a]&executed),'total':len(sets[a]),'missing':[f'{i:08x}' for i in sorted(sets[a]-executed)]} for a in [0x4023a0,0x409580,0x4d3c40,0x55f449,0x4cdff0,0x4ce3a0]},'jump_table':{'address':'0040243c','entries':['00402413','00402407','00402413','00402413'],'indices_executed':sorted(table_indices),'guard':'XOR EAX,EAX; MOV AL,[7027B4]; CMP EAX,3; JA bypass'},'media_imports':{f'{a:08x}':imports[a] for a in media},'variants':{},'method':'Whole two native callbacks with the previously omitted bounded4entry jump table, actual23instruction reset, actual77instruction stream body and actual90instruction selector/10instruction first-dot trim. Real C resolver/central/dropdown/selected-row composition, all watched memory/guards, ordered family/file/media events, full DWORD versus BYTE guards, and native stack/thiscall cleanup/saved registers compared.','limitations':['Family/campaign controllers516BB0/516990/4CB6A0 are explicit deeper endpoints here; production calls existing actual C bodies with separate family/campaign proofs. Their full persistence/rendering is not claimed by this suite.','Profile-name getter5169C0, file enumeration41C740 and sorting41CA10 are explicit endpoints, with actual receiver, C/native-built pattern text, consumed file table and comparator identity compared. The profile fixture supplies the name only; pattern assembly and first-dot trimming execute actual bodies. Real filesystem enumeration/order has its own file-list proof.','Miles media imports are controlled; the actual stream algorithm runs. Audio devices/codecs and external side effects are not claimed. Mutations exercise digital/volume reads after close/open; no mutation of a stream handle by later volume/loop/start APIs is modeled.','The selector invalid-index path is unreachable with its stable valid enumeration table and real trim algorithm; its missing instruction is reported rather than manufactured through an impossible helper mutation. Negative bounded count guards are checked.','Native negative stream channels are unsafe; only0..3 are checked, including native rejection of3. All channel inputs from NewGame are the literal0.','Actual localized menus, hit-testing beyond prepared selected rows and remaining editor/replay/settings behavior are separate unfinished obligations.']}
out=ROOT/'metadata/dropdown-start-actions-emulation.json'
try:
    for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
        path=path.resolve();proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
        try:
            assert json.loads(proc.stdout.readline())['fields']==32
            for i,(case,want) in enumerate(zip(cases,expected)):
                proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll());actual=json.loads(line)
                if actual!=want:
                    field=next(k for k in want if actual[k]!=want[k]);raise AssertionError({'variant':label,'case':i,'input':case,'field':field,'C':actual[field],'native':want[field]})
            proc.stdin.close();proc.wait(timeout=30);assert proc.returncode==0,proc.stderr.read()
        finally:
            if proc.poll() is None:proc.kill();proc.wait()
        report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(cases),'process_exit_code':proc.returncode}
    report['comparisons']=len(cases)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'checked':len(cases),'comparisons':report['comparisons'],'coverage':report['coverage']}))
