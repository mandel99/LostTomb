"""Bounded native checks for new lowering patterns and early large-function paths.
The 1600-instruction parents remain unbound: their other paths/callees are open.
No manifests, execution dumps, or original process are created.
"""
from pathlib import Path
import argparse,hashlib,random,struct,sys
from native_test_pe import PE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
source=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(source.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(source);candidate=PE(args.dll)
VS=0x30000000;HS=0x34000000;FRAME=0x35000000;BUF=0x33000000
STOP=0x31000000;BOUND=STOP+0x100;SP=VS+0x8000;HSP=HS+0x8000
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,
      UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
regions=[(0x605a30,32),(0xe38dc0,64),(0xe950d0,32),(0xeadd40,256),
         (0xbcb9c0,64),(0x7056b0,48),(0xd3b190,32),(BUF,4096)]
machines=[];events=[];calls=[];hits=set()
def read(u,a,n=4):return int.from_bytes(u.mem_read(a,n),'little')
def put(u,a,v,n=4):u.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
for is_c in [False,True]:
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(VS,0x10000),(HS,0x10000),(FRAME,4096),(BUF,0x10000),
        (STOP,4096),(0x600000,0x10000),(0xe38000,4096),(0xe95000,4096),
        (0xead000,8192),(0xbcb000,4096),(0x705000,4096),(0xd3b000,4096),
        (0x36000000,4096)]:u.mem_map(a,n)
    if is_c:
        u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095)
        u.mem_write(candidate.base,bytes(candidate.data));u.mem_write(BOUND,b'\xc3')
    else:
        u.mem_map(0x4f7000,0x4000)
        u.mem_write(0x4f7000,bytes(native.data[0x4f7000-native.base:0x4fb000-native.base]))
        u.mem_map(0x406000,4096);u.mem_write(0x406720,b'\xc3')
    def code(cpu,address,size,compiled):
        if address!=(BOUND if compiled else 0x406720):
            if not compiled:hits.add(address)
            return
        physical_sp=cpu.reg_read(UC_X86_REG_ESP)
        if compiled:
            site,target,frame=struct.unpack('<III',bytes(cpu.mem_read(physical_sp+4,12)))
            assert target==0x406720 and frame==FRAME,(site,target,frame)
            values=list(struct.unpack('<8I',bytes(cpu.mem_read(FRAME,32))));flags=read(cpu,FRAME+32)
            virtual_sp=values[4]
        else:
            target=0x406720;virtual_sp=physical_sp;site=read(cpu,virtual_sp)-5
            values=[cpu.reg_read(r) for r in regs];flags=cpu.reg_read(UC_X86_REG_EFLAGS)
        calls.append((site,target,read(cpu,0xd3b198),read(cpu,virtual_sp)))
        put(cpu,0x7056b8,0x12345678)
        values[0:3]=[0x12345678,0x87654321,0xabcdef01]
        flags=(flags&0x400)|0xad7
        if compiled:
            values[4]=virtual_sp+4;cpu.mem_write(FRAME,struct.pack('<8I',*values));put(cpu,FRAME+32,flags)
        else:
            for r,v in zip(regs,values):cpu.reg_write(r,v)
            cpu.reg_write(UC_X86_REG_EFLAGS,flags)
        ret=read(cpu,physical_sp)
        cpu.reg_write(UC_X86_REG_ESP,physical_sp+4);cpu.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,code,user_data=is_c);machines.append(u)

def c_call(u,name,argv):
    u.mem_write(HSP,struct.pack('<'+'I'*(len(argv)+2),STOP,*argv,0xa5a5a5a5))
    u.reg_write(UC_X86_REG_ESP,HSP);u.reg_write(UC_X86_REG_EFLAGS,2)
    u.emu_start(candidate.entry(name),STOP,count=200000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==HSP+4
    return u.reg_read(UC_X86_REG_EAX)

checks=0
def compare(name,start,end,values,flags,changes=(),flag_mask=0xcd5):
    global checks
    results=[]
    for compiled,u in enumerate(machines):
        for a,n in regions:u.mem_write(a,b'\xa5'*n)
        for a,v,n in changes:put(u,a,v,n)
        u.mem_write(VS,b'\xcc'*0x10000);put(u,SP,STOP);put(u,SP+4,0x12345678)
        calls.clear();values=list(values);values[4]=SP
        if compiled:
            u.mem_write(FRAME,struct.pack('<10I',*values,flags,BOUND))
            c_call(u,name,[FRAME]);result=list(struct.unpack('<8I',bytes(u.mem_read(FRAME,32))))
            out_flags=read(u,FRAME+32)
        else:
            for r,v in zip(regs,values):u.reg_write(r,v&0xffffffff)
            u.reg_write(UC_X86_REG_EFLAGS,flags)
            u.emu_start(start,end,count=200000)
            assert u.reg_read(UC_X86_REG_EIP)==end
            result=[u.reg_read(r) for r in regs];out_flags=u.reg_read(UC_X86_REG_EFLAGS)
        assert read(u,SP)==STOP and read(u,SP+4)==0x12345678
        results.append((result,out_flags&flag_mask,[bytes(u.mem_read(a,n)) for a,n in regions],list(calls)))
    assert results[0]==results[1],(name,values,flags,changes)
    checks+=1

initial=[0x87654321,0x12345678,0xabcdef01,0x11111111,SP,0x22222222,0x33333333,BUF+1024]
rng=random.Random(0x4f8000)
for value in [0,1,2,3,0x7fffffff,0x80000000,0xffffffff]+[rng.getrandbits(32) for _ in range(1017)]:
    for flags in [2,3,0x8d7,0xcd7]:
        v=initial.copy();v[1]=value
        compare('restore_test_shr',0x4f80ff,0x4f8102,v,flags,flag_mask=0x4c5) # OF/AF undefined for SHR2.
shift_checks=checks
for value in [0,1,0x12345678,0x80000000,0xffffffff]:
    for count in [0,1,2,7,31,64,255]:
        for flags in [2,3,0x8d7,0xcd7]:
            v=initial.copy();v[0]=value;v[1]=count
            compare('restore_test_rep',0x4f866f,0x4f8671,v,flags)
rep_checks=checks-shift_checks

# These are parent paths with explicit finite 406720 protocol, not a whole-parent proof.
for flags in [2,3,0x8d7,0xcd7]:
    compare('restore_C_004f7e50',0x4f7e50,STOP,initial,flags,[(0x605a3c,1,4)])
    for panel in [0,4]:
        for active in [0,1]:
            for state in [-32768,-1,*range(17),32767]:
                for kind in [0,13]:
                    compare('restore_C_004f96e0',0x4f96e0,STOP,initial,flags,
                        [(0xe38df8,1,1),(0xeadd50,panel,4),(0xeadccc,0,4),
                         (0xbcb9f0,active,1),(0xbcb9cc,kind,2),(0xbcb9f2,state,2)])
parent_checks=checks-shift_checks-rep_checks
rom_checks=0;u=machines[1]
for name,ranges in [('restore_test_rom_004f7e50',[(0x4f8900,16),(0x4f8910,79)]),
                    ('restore_test_rom_004f96e0',[(0x4fa310,16),(0x4fa320,15)])]:
    for a,n in ranges:
        for width in [1,2,4]:
            for offset in range(n-width+1):
                expected=int.from_bytes(native.data[a-native.base+offset:a-native.base+offset+width],'little')
                assert c_call(u,name,[a+offset,width])==expected;rom_checks+=1
    for width in [1,2,4]:
        put(u,0x36000001,0x87654321)
        assert c_call(u,name,[0x36000001,width])==(0x87654321&((1<<(8*width))-1));rom_checks+=1
print(f'PASS SHR:{shift_checks},REP:{rep_checks},immutable/fallback loads:{rom_checks},parent pilot:{parent_checks}; parents not fully covered/bound')
