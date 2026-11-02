"""Exact menu callbacks, real message setup/close and controlled Win32 edges."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/c-only-debug/Debug/restore_dropdown_system_actions_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/c-only/Release/restore_dropdown_system_actions_verify.exe');args=ap.parse_args()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
    q=table+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def cstr(a):
    b=bytearray()
    for i in range(512):
        v=get(a+i,1)
        if not v:return b.decode('ascii')
        b.append(v)
    raise ValueError('unterminated native string')
imports={};descriptor=base+u32(opt+104)
while get(descriptor+12):
    original=get(descriptor) or get(descriptor+16);iat=get(descriptor+16);dll=cstr(base+get(descriptor+12));index=0
    while get(base+original+index*4):
        value=get(base+original+index*4)
        if not value&0x80000000:imports[base+iat+index*4]={'dll':dll,'name':cstr(base+value+2)}
        index+=1
    descriptor+=20
for address,name in [(0x56f174,'PostMessageA'),(0x56f178,'ShowWindow'),(0x56f16c,'ShellExecuteA')]:assert imports[address]['name']==name
assert cstr(0x5761d8)=='open' and cstr(0x5761e0)=='mission editor guide.txt'
tokens=[0x402b90,0x4033f0,0x403400,0x4034d0];core={};helpers={};context={};sets={}
native_exports=['metadata/dropdown-action-native-cfg.json','metadata/dropdown-system-actions-ghidra-windows.json','metadata/dropdown-menu-native.json']
def attach(entry,f,target):
    a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];sets[entry]=set()
    for x in a['items']:
        address=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;target[address]=x;sets[entry].add(address)
facts=json.loads((ROOT/native_exports[0]).read_text());assert facts['native_sha256']==digest
for token in tokens:attach(token,facts['functions'][f'{token:08x}'],core)
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
    batch=json.loads(path.read_text());assert batch['native_sha256']==digest
    used=False
    for f in batch['entries']:
        entry=int(f['requested_address'],16)
        if entry in [0x4c5bf0,0x4c7f30,0x4c8040]:attach(entry,f,helpers);used=True
    if used:native_exports.append(path.relative_to(ROOT).as_posix())
assert len(core)==32 and len(helpers)==70
windows=json.loads((ROOT/native_exports[1]).read_text());assert windows['native_sha256']==digest;authenticated=set()
for w in windows['windows']:
    a=int(w['address'],16);raw=bytes.fromhex(w['hex']);assert not w['gaps'] and len(raw)==w['length'] and bytes(vm.mem_read(a,len(raw)))==raw
    authenticated.update(address for address,x in core.items() if a<=address and address+len(bytes.fromhex(x['bytes']))<=a+len(raw))
assert authenticated==set(core)
d=json.loads((ROOT/'metadata/dropdown-menu-native.json').read_text());assert d['native_sha256']==digest
for entry in [0x425f20,0x4260d0]:attach(entry,d['functions'][f'{entry:08x}'],context)
regions=[(0xd3a6c0,0xb40),(0xe38dc0,0x80),(0xe39120,64),(0xe209a0,0x410),(0xe950d0,32),(0x705700,64),(0xf53f60,32),(0x6f2000,64)]
platform={STOP+0x100:(0x56f178,2,21),STOP+0x200:(0x56f16c,6,22),STOP+0x300:(0x56f174,4,23),0x539170:(0x539170,0,None)}
for address,iat in [(STOP+0x100,0x56f178),(STOP+0x200,0x56f16c),(STOP+0x300,0x56f174)]:put(iat,address)
executed=set();events=[];p=[]
def event(address,argv):events.append([address,len(argv)]+argv+[0]*(6-len(argv))+[get(0xe38dda,1),get(0xe38dca,1),get(0xd3b17c),get(0xd3b180),get(0xe39140)])
def code(cpu,a,n,user):
    if a==STOP:cpu.emu_stop();return
    if a not in platform:
        assert a in core or a in helpers or a in context,f'escaped native instruction {a:08x}';executed.add(a);return
    address,count,ret=platform[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(sp+4+i*4) for i in range(count)];event(address,argv)
    if address==0x56f178:
        if p[16]&1:put(0xe39140,p[17])
        if p[16]&2:put(0xe38dda,p[18],1)
    if address==0x56f16c and p[16]&4:put(0xe38dca,p[19],1)
    if address==0x56f174 and p[16]&8:put(0xe38dda,p[18],1)
    if address==0x539170 and p[16]&16:put(0xd3b17c,p[20]);put(0xd3b180,0xabcdef01)
    cpu.reg_write(UC_X86_REG_EAX,p[ret] if ret else 0x98765432);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe)
    cpu.reg_write(UC_X86_REG_ESP,sp+4+(count*4 if ret else 0));cpu.reg_write(UC_X86_REG_EIP,get(sp))
def write(cpu,access,a,n,value,user):
    assert STACK<=a and a+n<=STACK+0x100000 or any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatched native write {a:08x}/{n}'
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def call(entry):
    saved={UC_X86_REG_EBX:p[11]^0x12345678,UC_X86_REG_ESI:p[11]^0x789abcde,UC_X86_REG_EDI:p[11]^0xabcdef12,UC_X86_REG_EBP:p[11]^0x2468ace0}
    for r,v in saved.items():vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_EAX,p[11]);vm.reg_write(UC_X86_REG_ECX,p[12]);vm.reg_write(UC_X86_REG_EDX,p[13]);vm.reg_write(UC_X86_REG_EFLAGS,0x202)
    sp=STACK+0x80000;put(sp,STOP);put(sp+4,0x6f2000);put(sp+8,1);vm.reg_write(UC_X86_REG_ESP,sp)
    vm.emu_start(entry,STOP,count=10000);assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    for r,v in saved.items():assert vm.reg_read(r)==v
    return vm.reg_read(UC_X86_REG_EAX)
def native(case):
    global p,events
    p=case;events=[]
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[2]+j*37+i*11)&255 for j in range(n)))
    for a,v,n in [(0xe38dda,p[3],1),(0xe38dca,p[4],1),(0xe39140,p[5],4),(0xe209f4,p[7],4),(0xe20d14,p[7],4),(0xe950dc,p[8],4),(0x705728,p[9],4),(0x705724,0x6f2000,4),(0x70572c,1,4),(0x705730,1,4),(0x6f2008,0x6f2020,4),(0x6f200c,1,2),(0x6f2024,tokens[p[0]],4),(0x6f202c,p[10],4),(0xe38e03,1,1)]:put(a,v,n)
    value=call(0x425f20 if p[1]>=4 else tokens[p[0]])
    result=value if p[1]>=4 else 0
    if p[1]>=5:call(0x4c7f30)
    return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
rng=random.Random(0x4033f0);cases=[]
def add(index,route,mode,mask=0,media=0,enabled=0):
    q=[rng.getrandbits(32) for _ in range(24)];q[0]=index;q[1]=route;q[3]=mode;q[7]=media;q[8]=enabled;q[9]=mode;q[16]=mask;cases.append(q)
for index,route,mode in itertools.product(range(4),range(5),[0,1,3,5,10,255,256,0x12345678,0xffffffff]):add(index,route,mode)
for index,route,media,enabled in itertools.product([1,2],[0,2,4,5,6],[0,1,0x12345678],[0,1,256,0xffffffff]):add(index,route,3,16,media,enabled)
for index,mask,mode in itertools.product([0,3],range(32),[1,3,255,0x12345678]):add(index,4,mode,mask)
for mode in range(256):add(1 if mode%2 else 2,6,mode,16,1,1)
for index,ret in itertools.product([0,3],[0,1,31,32,0x80000000,0xffffffff]):
    add(index,4,1,31);cases[-1][21]=cases[-1][22]=cases[-1][23]=ret
expected=[native(case) for case in cases]
assert set(core)<=executed
sources=['runtime/dropdown_system_actions.c','include/dropdown_system_actions.h','runtime/menu_callbacks.c','runtime/dropdown_actions.c','runtime/dropdown_menu_boundary.c','runtime/dropdown_menu_options.c','runtime/menu_platform_win32.c','include/menu_input.h','src/unit_01619.c','src/unit_01628.c','src/unit_01631.c','src/unit_00410.c','src/unit_00413.c','tests/dropdown_system_actions.c','tools/verify_dropdown_system_actions_emulation.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':len(cases),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe),'source_hashes':{s:sha(ROOT/s) for s in sources},'native_exports':{s:sha(ROOT/s) for s in native_exports},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'computer_UI_automation_used':False,'coverage':{f'{token:08x}':{'executed':len(sets[token]&executed),'total':len(sets[token])} for token in tokens},'helper_coverage':{f'{entry:08x}':{'executed':len(sets[entry]&executed),'total':len(sets[entry])} for entry in [0x4c5bf0,0x4c7f30,0x4c8040]},'context_instructions':len(set(context)&executed),'imports':{f'{a:08x}':imports[a] for a in [0x56f174,0x56f178,0x56f16c]},'shell_operation':cstr(0x5761d8),'shell_file':cstr(0x5761e0),'variants':{},'method':'Complete4 native callbacks and actual message setup/link-clear/close composed with the real C central/dropdown router and selected-row click; all watched state and guards, ordered OS/media-boundary arguments/state, saved modes, native stack/saved GPRs and explicit helper-exit mutations compared.','limitations':['PostMessageA/ShowWindow/ShellExecuteA are controlled at exact authenticated native imports and the existing C platform API. No actual OS delivery, window activation or opening an external editor is claimed. API failure values remain ignored exactly as native.','Media shutdown539170 is an explicit controlled endpoint for synthetic media-enabled message records; native/C selection and state reload after it are compared, not the shutdown algorithm.','Help10 and About0 always pass templated=0. The actual setup helper templated branch is therefore outside this new callback composition; its prior independent full-helper proof remains separate.','Clicked-path context uses valid prepared selected rows. Full pointer alias/timing behavior, hover/hit-testing and OS input delivery have separate obligations.','This proves help/about state initialization and close, not their localized text/pixels or every remaining menu/settings action.','Original4034D0 opens mission editor guide.txt. It is not relabeled as a website action.']}
for source in ['include/message_panel.h','include/message_panel_input.h','include/dropdown_menu.h','include/standalone_data.h']:
    report['source_hashes'][source]=sha(ROOT/source)
out=ROOT/'metadata/dropdown-system-actions-emulation.json'
try:
    for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
        path=path.resolve();proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
        try:
            assert json.loads(proc.stdout.readline())['fields']==24
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
print(json.dumps({'passed':True,'checked':len(cases),'comparisons':report['comparisons'],'core_instructions':len(core),'helper_coverage':report['helper_coverage']}))
