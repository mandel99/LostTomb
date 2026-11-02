"""Complete DWORD cursor selector versus pinned x86; finite SetCursor boundary."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if 'menu_cursor_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/menu_cursor_phase';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-menu-cursor/Debug/restore_menu_cursor_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-menu-cursor/Release/restore_menu_cursor_verify.exe');ap.add_argument('--limit',type=int);ap.add_argument('--start',type=int,default=0);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();product=ROOT if args.production else PHASE
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
    q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x30000000;TRAP=STOP+0x100;STACK=0x31000000
vm.mem_map(STOP,0x10000);vm.mem_map(STACK,0x10000)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def string(a):
    result=bytearray()
    while get(a,1):result.append(get(a,1));a+=1
    return result.decode('ascii')
imports={};desc=base+u32(opt+104)
while get(desc+12):
    source=get(desc) or get(desc+16);iat=get(desc+16);j=0
    while get(base+source+j*4):
        token=get(base+source+j*4)
        if not token&0x80000000:imports[base+iat+j*4]=string(base+token+2)
        j+=1
    desc+=20
assert imports[0x56f1bc]=='SetCursor' and imports[0x56f1b0]=='LoadCursorA'
native_file=ROOT/'metadata/menu-cursor-native.json' if args.production else PHASE/'metadata/native.json'
d=json.loads(native_file.read_text());assert d['native_sha256']==digest
f=d['functions']['004cf460'];assert not f['assembly']['has_more'] and f['assembly']['total']==99
core={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for x in f['assembly']['items']:
    a=int(x['address'],16);code=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(a,len(code)))==code
    core[a]=x;i=next(cs.disasm(code,a,count=1))
    if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
assert bytes(vm.mem_read(0x4cf5cc,76)).hex()==d['jump_table']['bytes']
slots=[int(a,16) for a in d['jump_table']['DWORD_handle_slots']]
assert len(slots)==19 and sorted(slots)==list(range(0xe39090,0xe390dc,4))
put(0x56f1bc,TRAP)
executed=set();edges=set();indices=set();previous=None;p=[];events=[]
def hook(cpu,address,size,user):
    global previous
    if previous in branches:edges.add((previous,address))
    previous=address
    if address==STOP:cpu.emu_stop();return
    if address==TRAP:
        sp=cpu.reg_read(UC_X86_REG_ESP);handle=get(sp+4)
        events.append([0x56f1bc,1,handle,get(0xe390dc)]+[get(0xe39090+i*4) for i in range(19)])
        assert len(events)<=2 and get(0xe390dc)==handle
        if p[5]&1:put(0xe390dc,p[6])
        if p[5]&2:
            for i in range(19):put(0xe39090+i*4,p[6]+i*0x1020304)
        cpu.reg_write(UC_X86_REG_EAX,p[4]);cpu.reg_write(UC_X86_REG_ECX,0xdecafbad);cpu.reg_write(UC_X86_REG_EDX,0xdead0123)
        cpu.reg_write(UC_X86_REG_ESP,sp+8);cpu.reg_write(UC_X86_REG_EIP,get(sp));return
    assert address in core,f'Unqualified native instruction {address:08x}'
    executed.add(address)
    if address==0x4cf46d:indices.add(cpu.reg_read(UC_X86_REG_EAX))
def written(cpu,access,address,size,value,user):
    assert STACK<=address and address+size<=STACK+0x10000 or 0xe39070<=address and address+size<=0xe39100,f'Unwatched native write {address:08x}/{size}'
vm.hook_add(UC_HOOK_CODE,hook);vm.hook_add(UC_HOOK_MEM_WRITE,written)
def call(selector):
    global previous
    previous=None;sp=STACK+0x8000;put(sp,STOP);put(sp+4,selector)
    saved={UC_X86_REG_EBX:p[3]^0x12345678,UC_X86_REG_ESI:p[3]^0x2468ace0,UC_X86_REG_EDI:p[3]^0x13579bdf,UC_X86_REG_EBP:p[3]^0xaabbccdd}
    for register,value in saved.items():vm.reg_write(register,value)
    vm.reg_write(UC_X86_REG_EAX,p[3]);vm.reg_write(UC_X86_REG_ECX,p[3]^0x55555555);vm.reg_write(UC_X86_REG_EDX,p[3]^0xaaaaaaaa)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,0x202)
    vm.emu_start(0x4cf460,STOP,count=200)
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    for register,value in saved.items():assert vm.reg_read(register)==value
    return vm.reg_read(UC_X86_REG_EAX)
def native(case):
    global p,events
    p=case;events=[];vm.mem_write(0xe39070,bytes((p[3]+i*37)&255 for i in range(144)))
    for i,value in enumerate(p[7:]):put(0xe39090+i*4,value)
    result=call(p[1])
    if p[0]==0:result=0
    if p[0]==2:result=call(p[2])
    return {'result':result,'state':bytes(vm.mem_read(0xe39070,144)).hex(),'events':list(events)}
rng=random.Random(0x4cf460);cases=[]
selectors=list(range(21))+[255,256,0x10000001,0x80000000,0xffffffff]
for op,selector,returned,mutation,profile in itertools.product([0,1],selectors,[0,1,255,0x80000000,0xffffffff],range(4),range(4)):
    seed=rng.getrandbits(32)
    handles=[[0]*19,[(i+1)*0x100+0x5a for i in range(19)],[0x80000000+i*0x10001 for i in range(19)],[rng.getrandbits(32) for i in range(19)]][profile]
    cases.append([op,selector,0,seed,returned,mutation,rng.getrandbits(32)]+handles)
for i in range(200):
    cases.append([2,rng.choice(selectors),rng.choice(selectors),rng.getrandbits(32),rng.getrandbits(32),i%4,rng.getrandbits(32)]+[rng.getrandbits(32) for j in range(19)])
selected=cases[args.start:args.start+args.limit if args.limit else None];expected=[native(c) for c in selected]
owned_edges={(a,b) for a,z in branches.items() for b in z};complete=set(core)<=executed and owned_edges<=edges and indices==set(range(19))
source_paths=[product/p for p in ['src/unit_01769.c','include/menu_cursor.h','runtime/menu_cursor_platform.c','tests/menu_cursor.c','tests/menu_cursor_case.h']]
source_paths += [ROOT/p for p in ['include/standalone_data.h','src/unit_01768.c','src/unit_01774.c','src/unit_00085.c']]
source_paths += [PHASE/'build/CMakeLists.txt',PHASE/'tools/extract_menu_cursor_native.py',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,
 'complete_case_set':not args.limit and not args.start,'checked':len(selected),'native_sha256':digest,
 'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),
 'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},
 'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file)},
 'qualified_controller':{'complete':complete,'instructions':len(core),'executed':len(executed),'missing_instructions':[f'{a:08x}' for a in sorted(set(core)-executed)],'conditional_edges':len(owned_edges),'edges_executed':len(owned_edges&edges),'dispatch_indices':sorted(indices),'excluded_instructions':[],'excluded_edges':[]},
 'watched_memory':{'address':'00e39070','bytes':144,'native_handle_payload_bytes':80,'prefix_guard_bytes':32,'suffix_guard_bytes':32,'all_native_writes_guarded':True,'all_bytes_compared':True},
 'native_ABI':{'cdecl_arguments':1,'RET_cleanup':0,'SetCursor_stdcall_arguments':1,'SetCursor_cleanup':4,'full_EAX_return_compared_in_named_helper':True,'legacy_void_facade_return_unconsumed':True,'stack_and_saved_registers_checked':['ESP','EBX','ESI','EDI','EBP'],'endpoint_clobbers':['EAX','ECX','EDX']},
 'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,
 'limitations':[
  'SetCursor is the only deep boundary. Finite API returns, argument/state snapshots and mutations are compared. Production provider forwards the real WINAPI SetCursor, compiled separately but never linked or executed in this oracle. No PC cursor or live GUI action occurs.',
  'The named HCURSOR helper preserves the original full EAX SetCursor return. Existing void FUN004CF460 facade preserves the four qualified native callers that discard it. Handle zero is passed to SetCursor exactly; no success fallback or BOOL conversion is invented.',
  'Existing4CF960 loader remains fact-only: its19 HCURSOR stores are already typed in root1774 and establish the cursor-table prerequisite. This phase does not prove physical resources, loader failures or the whole startup graph.',
  'Open4CF210 failure-domain difference remains unchanged: native ignores GetCursorPos BOOL and consumes unspecified POINT output; root1768 returns early on FALSE. No native FALSE equivalence or invented safe POINT is claimed. Existing source and tests are untouched.',
  'Large0040BA40 consumer root85 still uses BYTE cursor-handle comparisons. It is explicitly outside this99-instruction selector scope and is not repaired or proved here.']}
if sha(ROOT/'src/unit_00085.c')!='9c78635615701cf1328a6fff55580e158404aeb1219fca5d2a6f45a0fc16059a':
    # This old85 hash was a fact-only gap snapshot, never an executed selector
    # dependency. Retire its text only after the whole real85 production proof.
    from menu_cursor_consumer_proof import verified_consumer_proof
    supersession=verified_consumer_proof(ROOT)
    report['source_hashes'].update(supersession.pop('verified_source_and_evidence_hashes'))
    report['consumer_gap_supersession']=supersession
    report['limitations'][-1]='The historical root85 BYTE handle-comparison gap is superseded by the fresh whole238-instruction40BA40/4B5520 production proof. This99-instruction selector algorithm is unchanged. Three larger movement/action functions remain qualified boundaries; complete actor-order gameplay is not claimed.'
out=ROOT/'metadata/menu-cursor-emulation.json' if args.production else PHASE/'metadata/menu-cursor-emulation.json'
try:
    if report['complete_case_set']:assert complete,report['qualified_controller']
    for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
        path=path.resolve();smoke=subprocess.run([str(path)],capture_output=True,text=True,timeout=10);assert smoke.returncode==0,smoke.stderr
        proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
        try:
            assert json.loads(proc.stdout.readline())=={'fields':26,'handle_bytes':4}
            for i,(case,want) in enumerate(zip(selected,expected)):
                proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read())
                actual=json.loads(line)
                if actual!=want:
                    detail={'variant':label,'case':args.start+i,'input':case,'C':actual,'native':want};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError((label,args.start+i))
            proc.stdin.close();proc.wait(timeout=10);assert proc.returncode==0,proc.stderr.read()
        finally:
            if proc.poll() is None:proc.kill();proc.wait()
        report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(selected),'smoke_exit':smoke.returncode}
    assert all(sha(ROOT/path)==value for path,value in report['source_hashes'].items()),'Concurrent source drift.'
    assert all(sha(ROOT/path)==value for path,value in report['native_exports'].items()),'Concurrent evidence drift.'
    report['comparisons']=len(selected)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':report['passed'],'checked':len(selected),'comparisons':report.get('comparisons'),'coverage':report['qualified_controller']}))
