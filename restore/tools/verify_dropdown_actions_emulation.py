"""Compare ten complete menu callbacks and real click dispatch with pinned x86.

No original process or code fallback is used. The original instructions run only
inside Unicorn; the four child processes contain compiled restored C.
"""
from pathlib import Path
from datetime import datetime, timezone
import argparse, hashlib, itertools, json, random, struct, subprocess, sys

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
ap=argparse.ArgumentParser()
ap.add_argument('--exe',type=Path,default=ROOT/'build/c-only-debug/Debug/restore_dropdown_actions_verify.exe')
ap.add_argument('--plain-exe',type=Path,default=ROOT/'build/c-only-debug/Debug/restore_dropdown_actions_plain_verify.exe')
ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/c-only/Release/restore_dropdown_actions_verify.exe')
ap.add_argument('--optimized-plain-exe',type=Path,default=ROOT/'build/c-only/Release/restore_dropdown_actions_plain_verify.exe')
ap.add_argument('--production',action='store_true')
args=ap.parse_args()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
digest=hashlib.sha256(image).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
    q=table+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
regions=[(0x605780,128),(0x6de500,32),(0x784b50,0x2a0),(0xe38dc0,0x160),(0xf53f00,0x90),(0x705700,64),(0xd3b180,48),(0x6f2000,64)]
tokens=[0x402bc0,0x402be0,0x402e30,0x402ee0,0x4032c0,0x406070,0x4060e0,0x406100,0x406140,0x406170]
facts=json.loads((ROOT/'metadata/dropdown-action-native-cfg.json').read_text())
assert facts['native_sha256']==digest
core={};sets={};successors={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for token in tokens:
    f=facts['functions'][f'{token:08x}'];a=f['assembly'];assert not f['unresolved_indirect_jumps'] and not a['has_more'] and len(a['items'])==a['total']
    sets[token]=set()
    for x in a['items']:
        address=int(x['address'],16);raw=bytes.fromhex(x['bytes'])
        assert bytes(vm.mem_read(address,len(raw)))==raw
        ins=next(cs.disasm(raw,address));assert ins.size==len(raw) and ins.mnemonic!='call'
        sets[token].add(address);core[address]=x
        if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':successors[address]={address+ins.size,int(ins.op_str,16)}
    # Close every CFG from the real table entry, not a contiguous byte window.
    todo=[token];seen=set()
    while todo:
        address=todo.pop()
        if address in seen:continue
        assert address in sets[token];seen.add(address)
        ins=next(cs.disasm(bytes.fromhex(core[address]['bytes']),address))
        if ins.mnemonic.startswith('ret'):assert ins.op_str=='';continue
        if ins.mnemonic=='jmp':todo.append(int(ins.op_str,16))
        elif ins.mnemonic.startswith('j'):todo.extend(successors[address])
        else:todo.append(address+ins.size)
    assert seen==sets[token]
ghidra=json.loads((ROOT/'metadata/dropdown-actions-ghidra-windows.json').read_text())
assert ghidra['native_sha256']==digest
covered_by_ghidra=set()
for window in ghidra['windows']:
    assert not window['gaps']
    a=int(window['address'],16);raw=bytes.fromhex(window['hex'])
    assert len(raw)==window['length'] and bytes(vm.mem_read(a,len(raw)))==raw
    for entry,x in core.items():
        if a<=entry and entry+len(bytes.fromhex(x['bytes']))<=a+len(raw):covered_by_ghidra.add(entry)
assert covered_by_ghidra==set(core)
context={}
native_context=json.loads((ROOT/'metadata/dropdown-menu-native.json').read_text())
assert native_context['native_sha256']==digest
for entry in ['00425f20','004260d0']:
    a=native_context['functions'][entry]['assembly'];assert not a['has_more']
    for x in a['items']:
        address=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;context[address]=x

def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
executed=set();context_executed=set();edges=set();last=None;writes=[]
def code(cpu,a,n,user):
    global last
    if a==STOP:cpu.emu_stop();return
    assert a in core or a in context,f'unexpected instruction {a:08x}'
    if last in successors:edges.add((last,a))
    if a in core:executed.add(a)
    else:context_executed.add(a)
    last=a
def write(cpu,access,a,n,value,user):
    if STACK<=a<STACK+0x100000:return
    assert any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatched native write {a:08x}/{n}'
    if last in core:writes.append([a,n,value&((1<<(n*8))-1)])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def native(p):
    global last,writes
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[2]+j*37+i*11)&255 for j in range(n)))
    for a,k,n in [(0xe38dda,3,1),(0xe38dca,4,1),(0xe38e64,5,4),(0xe38e63,6,1),(0xe38ec8,7,4),(0xe38ecc,8,4),(0xe38ec0,9,4),(0xe38ec4,10,4),(0xe38ed0,11,4),(0xe38ef0,12,4),(0xe38e6c,13,4),(0xe38e70,14,4),(0xf53f18,15,4),(0xf53f1c,16,4),(0xf53f20,17,4),(0xf53f4c,18,4),(0x705728,19,4)]:put(a,p[k],n)
    for a,v,n in [(0x705724,0x6f2000,4),(0x70572c,1,4),(0x705730,1,4),(0x6f2008,0x6f2020,4),(0x6f200c,1,2),(0x6f2024,tokens[p[0]],4),(0x6f202c,p[20],4),(0xe38e03,1,1)]:put(a,v,n)
    saved={UC_X86_REG_EBX:(0x12345678^p[21]),UC_X86_REG_ESI:(0x789abcde^p[21]),UC_X86_REG_EDI:(0xabcdef12^p[21]),UC_X86_REG_EBP:(0x2468ace0^p[21])}
    for r,v in saved.items():vm.reg_write(r,v)
    for r,v in [(UC_X86_REG_EAX,p[21]),(UC_X86_REG_ECX,p[21]^0xffffffff),(UC_X86_REG_EDX,p[21]^0x5a5a5a5a),(UC_X86_REG_EFLAGS,0x202)]:vm.reg_write(r,v)
    sp=STACK+0x80000;vm.reg_write(UC_X86_REG_ESP,sp);put(sp,STOP);put(sp+4,0x6f2000);put(sp+8,1)
    last=None;writes=[];vm.emu_start(0x425f20 if p[1]==4 else tokens[p[0]],STOP,count=2000)
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    for r,v in saved.items():assert vm.reg_read(r)==v
    return {'result':vm.reg_read(UC_X86_REG_EAX) if p[1]==4 else 0,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'writes':list(writes)}

rng=random.Random(0x402ee0);cases=[]
def add(index,route,value):
    p=[rng.getrandbits(32) for _ in range(24)];p[0]=index;p[1]=route
    for j in range(3,22):p[j]=value if j%2 else (value^0xa5a55a5a)
    cases.append(p)
values=[0,1,2,3,127,128,255,256,257,65535,65536,0x12345678,0x7fffffff,0x80000000,0xffffff00,0xffffffff]
for index,route,value in itertools.product(range(10),range(5),values):add(index,route,value)
for value,route in itertools.product(range(256),[0,4]):
    add(3,route,value);cases[-1][5]=(value<<24)|((255-value)<<8)|value;cases[-1][6]=value;cases[-1][9]=(value<<24)|((value+1)&255)
for index in range(10):
    for _ in range(32):
        p=[rng.getrandbits(32) for _ in range(24)];p[0]=index;p[1]=rng.randrange(5);cases.append(p)
expected=[native(p) for p in cases]
assert executed==set(core),f'uncovered core instructions {set(core)-executed}'
assert edges=={(a,b) for a,s in successors.items() for b in s}
paths={'Debug':args.exe,'Debug_plain':args.plain_exe,'O2':args.optimized_exe,'O2_plain':args.optimized_plain_exe}
sources=['runtime/dropdown_actions.c','include/dropdown_actions.h','runtime/menu_callbacks.c','runtime/dropdown_menu_boundary.c','runtime/dropdown_menu_options.c','src/unit_00410.c','src/unit_00413.c','tests/dropdown_actions.c','include/dropdown_menu.h','tools/verify_dropdown_actions_emulation.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':len(cases),'native_sha256':digest,'original_game_process_executed':False,'original_machine_code_fallback_used':False,'computer_UI_automation_used':False,'source_hashes':{s:sha(ROOT/s) for s in sources},'native_exports':{s:sha(ROOT/s) for s in ['metadata/dropdown-action-native-cfg.json','metadata/dropdown-menu-native.json']},'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe),'variants':{},'coverage':{f'{k:08x}':{'executed':len(sets[k]&executed),'total':len(sets[k])} for k in tokens},'conditional_outcomes':{'checked':len(edges),'total':sum(map(len,successors.values()))},'composed_context_instructions':len(context_executed),'routes':['direct body','actual resolver','actual central menu router','actual dropdown router','actual clicked dropdown -> callback -> close'],'method':'Ten complete native no-call callback CFGs byte-authenticated and executed; complete core instruction/branch coverage, bounded global memory and canaries, exact callback store ordering for traced variants, native stack/callee-saved GPR preservation. Real C routing and clicked native dropdown path are composed without controlled algorithm endpoints.','limitations':['Native ignored EAX values are not promoted to C return values; the real dropdown ignores those callback results.','Settings entry/snapshot and cache invalidation callbacks are proved here. Settings drawing, sliders, confirmation, audio platform and every remaining menu action are separate requirements.','Native clicked-path context is covered for valid selected-row records and compared in this suite; other dropdown branches have their own complete controller proof.','Dedicated O2 variants compile the actual callback source with O2; the whole-program library retains its existing compiler settings.']}
report['native_exports']['metadata/dropdown-actions-ghidra-windows.json']=sha(ROOT/'metadata/dropdown-actions-ghidra-windows.json')
report['native_instructions_independently_authenticated_by_Ghidra']=len(covered_by_ghidra)
out=ROOT/'metadata/dropdown-actions-emulation.json'
try:
    for label,path in paths.items():
        path=path.resolve();proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
        try:
            assert json.loads(proc.stdout.readline())['fields']==24
            for i,(p,want) in enumerate(zip(cases,expected)):
                proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll())
                actual=json.loads(line)
                for key in ['result','state']+([] if label.endswith('_plain') else ['writes']):
                    if actual[key]!=want[key]:raise AssertionError({'variant':label,'case':i,'input':p,'field':key,'C':actual[key],'native':want[key]})
            proc.stdin.close();proc.wait(timeout=30);assert proc.returncode==0,proc.stderr.read()
        finally:
            if proc.poll() is None:proc.kill();proc.wait()
        report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(cases),'ordered_stores_compared':not label.endswith('_plain'),'process_exit_code':proc.returncode}
    report['comparisons']=len(cases)*len(paths);report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'checked':len(cases),'comparisons':report['comparisons'],'native_instructions':len(core),'context_instructions':len(context_executed),'conditional_outcomes':len(edges)}))
