"""Check whole 418FE0 call protocol; production uses actual reviewed C callees."""
from pathlib import Path
import argparse,hashlib,itertools,struct,sys
from native_test_pe import PE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
original=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(original.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(original);candidate=PE(args.dll)
START=0x418fe0;END=0x41908a;STACK=0x30000000;SP=STACK+0x8000;STOP=0x31000000
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,
           UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
names={0x425bc0:'FUN_00425bc0',0x425990:'FUN_00425990',0x4c7db0:'FUN_004c7db0'}
regions=[(0x6fdc90,32),(0x7056b0,32)]
machines=[];trace=[];state={};hits=set()
def word(u,a):return int.from_bytes(u.mem_read(a,4),'little')
def put(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
for is_c in [False,True]:
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(STACK,0x10000),(STOP,4096),(0x6fd000,4096),(0x705000,4096)]:u.mem_map(a,n)
    if is_c:
        u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095)
        u.mem_write(candidate.base,bytes(candidate.data))
    else:
        u.mem_map(START&~4095,8192)
        u.mem_write(START,bytes(native.data[START-native.base:END-native.base]))
        for a in {v&~4095 for v in names}:u.mem_map(a,4096)
        for a in names:u.mem_write(a,b'\xc3')
    boundaries={candidate.entry(name) if is_c else address:address for address,name in names.items()}
    def code(cpu,address,size,context):
        endpoints,compiled=context
        if address not in endpoints:
            if not compiled:assert START<=address<END,f'native escaped {address:08x}';hits.add(address)
            return
        target=endpoints[address];sp=cpu.reg_read(UC_X86_REG_ESP)
        count={0x425bc0:5,0x425990:4,0x4c7db0:0}[target]
        argv=[word(cpu,sp+4+4*j) for j in range(count)]
        trace.append((target,argv,word(cpu,0x6fdc9c),word(cpu,0x6fdca0)))
        if state['mutate']:
            put(cpu,0x6fdc9c,word(cpu,0x6fdc9c)+0x7fffff41)
            put(cpu,0x6fdca0,word(cpu,0x6fdca0)+0x80000231)
        put(cpu,0x7056b8,len(trace))
        result=0xfeedbeef
        if target==0x425990:
            result=state['results'][state['query']];state['query']+=1
        cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff)
        cpu.reg_write(UC_X86_REG_ECX,0x13579bdf);cpu.reg_write(UC_X86_REG_EDX,0x2468ace0)
        cpu.reg_write(UC_X86_REG_EFLAGS,0xad7)
        ret=word(cpu,sp);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,code,user_data=(boundaries,is_c));machines.append(u)

checked=0
for x,y,results,mutate in itertools.product([0,1000,0x7fffffff,0x80000000],
        [0,0xffffffff,0x7fffffff],[(0,0),(0,1),(0,-1),(1,0),(-1,0)], [False,True]):
    outcomes=[]
    for is_c,u in enumerate(machines):
        for a,n in regions:u.mem_write(a,b'\xa5'*n)
        put(u,0x6fdc9c,y);put(u,0x6fdca0,x)
        u.mem_write(STACK,b'\xcc'*0x10000);u.mem_write(SP,struct.pack('<II',STOP,0x12345678))
        initial=[0x1234,0x5678,0x9abc,0x11111111,SP,0x22222222,0x33333333,0x44444444]
        for reg,value in zip(registers,initial):u.reg_write(reg,value)
        u.reg_write(UC_X86_REG_EFLAGS,2);trace.clear();state.update(results=results,query=0,mutate=mutate)
        u.emu_start(candidate.entry('FUN_00418fe0') if is_c else START,STOP,count=10000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
        assert bytes(u.mem_read(SP,8))==struct.pack('<II',STOP,0x12345678)
        for j in [3,5,6,7]:assert u.reg_read(registers[j])==initial[j]
        outcomes.append((list(trace),[bytes(u.mem_read(a,n)) for a,n in regions]))
    assert outcomes[0]==outcomes[1],(x,y,results,mutate,outcomes)
    checked+=1
expected={i.address for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(native.data[START-native.base:END-native.base]),START)}
assert hits==expected,(expected-hits,hits-expected)
print(f'PASS {checked} C/native briefing protocols; {len(hits)} instructions, full argument/result widths, anchor reloads, tail return, stack/data guards; callee algorithms qualified separately')
