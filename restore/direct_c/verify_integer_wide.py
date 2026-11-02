"""Real IDIV/one-operand IMUL versus emitted C; stdout only, no original process."""
from pathlib import Path
import argparse,hashlib,itertools,random,struct,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_ERR_EXCEPTION
from unicorn.x86_const import *
from native_test_pe import PE
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
p=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(p.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(p);candidate=PE(args.dll)
assert bytes(native.data[0x4f161c-native.base:0x4f161f-native.base])==bytes.fromhex('f77e44')
assert bytes(native.data[0x4f162a-native.base:0x4f162c-native.base])==bytes.fromhex('f7e9')
assert bytes(native.data[0x4541da-native.base:0x4541dd-native.base])==bytes.fromhex('0f9dc2')
assert bytes(native.data[0x45d5c6-native.base:0x45d5c9-native.base])==bytes.fromhex('0f9cc0')
HOST=0x34000000;VIRTUAL=0x32000000;FRAME=0x35000000;BUF=0x36000000;STOP=0x31000000
HSP=HOST+0x8000;VSP=VIRTUAL+0x8000;regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines=[]
class CReject(Exception):pass
for compiled in [False,True]:
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(HOST,0x10000),(VIRTUAL,0x10000),(FRAME,4096),(BUF,4096),(STOP,4096)]:u.mem_map(a,n)
    if compiled:
        u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
        abort=candidate.entry('abort')
        def code(cpu,address,size,user):
            if address==abort:raise CReject()
        u.hook_add(UC_HOOK_CODE,code)
    else:
        u.mem_map(0x4f1000,4096)
        u.mem_write(0x4f161c,bytes.fromhex('f77e44'));u.mem_write(0x4f162a,bytes.fromhex('f7e9'))
        u.mem_map(0x454000,4096);u.mem_map(0x45d000,4096)
        u.mem_write(0x4541da,bytes.fromhex('0f9dc2'));u.mem_write(0x45d5c6,bytes.fromhex('0f9cc0'))
    machines.append(u)

def compare(op,low,high,operand,flags=2,invalid=False):
    initial=[low&0xffffffff,operand&0xffffffff,high&0xffffffff,0x22222222,VSP,0x33333333,BUF+3,0x55555555]
    outputs=[];rejections=[]
    for compiled,u in enumerate(machines):
        # Unicorn's Windows backend can raise a host exception for native #DE.
        # Invalid inputs test only the C rejection contract, not fault emulation.
        if invalid and not compiled:
            outputs.append(None);rejections.append(True);continue
        u.mem_write(BUF,b'\xa5'*4096);u.mem_write(BUF+0x47,struct.pack('<I',operand&0xffffffff))
        u.mem_write(HOST,b'\xcc'*0x10000);u.mem_write(VIRTUAL,b'\xcc'*0x10000)
        rejected=False
        if compiled:
            frame=struct.pack('<10I',*initial,flags,0);u.mem_write(FRAME,frame+struct.pack('<I',0x89abcdef))
            u.mem_write(HSP,struct.pack('<III',STOP,FRAME,0x12345678))
            for r,v in zip(regs,[1,2,3,0x11223344,HSP,0x22334455,0x33445566,0x44556677]):u.reg_write(r,v)
            # The host C ABI has DF clear; the virtual/native DF is independent.
            u.reg_write(UC_X86_REG_EFLAGS,2)
            name={'idiv':'restore_test_idiv','imul':'restore_test_imul64','setge':'restore_test_setge','setl':'restore_test_setl'}[op]
            try:u.emu_start(candidate.entry(name),STOP,count=20000)
            except CReject:rejected=True
            if not rejected:
                assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==HSP+4
                assert [u.reg_read(regs[j]) for j in [3,5,6,7]]==[0x11223344,0x22334455,0x33445566,0x44556677]
                assert int.from_bytes(u.mem_read(HSP,4),'little')==STOP
                assert int.from_bytes(u.mem_read(HSP+8,4),'little')==0x12345678
            actual=list(struct.unpack('<8I',bytes(u.mem_read(FRAME,32))));out_flags=int.from_bytes(u.mem_read(FRAME+32,4),'little')
            assert int.from_bytes(u.mem_read(FRAME+40,4),'little')==0x89abcdef
        else:
            for r,v in zip(regs,initial):u.reg_write(r,v)
            u.reg_write(UC_X86_REG_EFLAGS,flags)
            start,end={'idiv':(0x4f161c,0x4f161f),'imul':(0x4f162a,0x4f162c),'setge':(0x4541da,0x4541dd),'setl':(0x45d5c6,0x45d5c9)}[op]
            try:u.emu_start(start,end,count=10)
            except UcError as error:
                assert error.errno==UC_ERR_EXCEPTION,(op,error)
                rejected=True
            if not rejected:assert u.reg_read(UC_X86_REG_EIP)==end
            actual=[u.reg_read(r) for r in regs];out_flags=u.reg_read(UC_X86_REG_EFLAGS)
        rejections.append(rejected)
        mask=0x400|0x200|(0x801 if op=='imul' else 0xcd5 if op.startswith('set') else 0) # Only defined arithmetic flags.
        outputs.append((actual,out_flags&mask,bytes(u.mem_read(BUF,4096))))
        if rejected:assert actual==initial
    assert rejections[0]==rejections[1],(op,low,high,operand,rejections)
    if not rejections[0]:assert outputs[0]==outputs[1],(op,low,high,operand,flags,outputs[0][:2],outputs[1][:2])
    return rejections[0]

def split(value):return value&0xffffffff,(value>>32)&0xffffffff
def quotient(a,b):return (abs(a)//abs(b))*(-1 if (a<0)^(b<0) else 1)
dividends=[-(1<<63),(1<<63)-1,-(1<<32),-2147483648,-2147483647,-7,-1,0,1,7,2147483647,1<<32]
divisors=[-2147483648,-2147483647,-65536,-7,-3,-2,-1,1,2,3,7,65536,2147483647]
good=[(a,b) for a,b in itertools.product(dividends,divisors) if -(1<<31)<=quotient(a,b)<(1<<31)]
rng=random.Random(0x4f161c)
while len(good)<2048:
    a=rng.randrange(-(1<<63),1<<63);b=rng.randrange(-(1<<31),1<<31)
    if b and -(1<<31)<=quotient(a,b)<(1<<31):good.append((a,b))
for a,b in good:
    for flags in [2,0xcd7]:assert not compare('idiv',*split(a),b,flags)
bad=[(0,0),(1,0),(-(1<<63),-1),(1<<32,1),(-(1<<32),1),((1<<63)-1,1),(-(1<<63),1)]
for a,b in bad:assert compare('idiv',*split(a),b,invalid=True)
values=[-2147483648,-65536,-2,-1,0,1,2,65536,2147483647]
products=list(itertools.product(values,repeat=2))+[(rng.randrange(-(1<<31),1<<31),rng.randrange(-(1<<31),1<<31)) for _ in range(2048)]
for a,b in products:
    for flags in [2,3,0xad7,0xcd7]:assert not compare('imul',a,rng.getrandbits(32),b,flags)
set_checks=0
for combination in range(64):
    flags=2|sum(bit for j,bit in enumerate([1,4,16,64,128,2048]) if combination&(1<<j))
    for control,low,high in itertools.product([0,0x200,0x400,0x600],[0,0xffffffff,0x87654321],[0,0xffffffff,0x12345678]):
        for op in ['setge','setl']:
            assert not compare(op,low,high,0x89abcdef,flags|control);set_checks+=1
print(f'PASS IDIV:{len(good)*2} defined cases +{len(bad)} C-only rejects; IMUL64:{len(products)*4}; SETGE/SETL:{set_checks}; full GPR/defined flags,stack/data guards; native fault domain unexecuted')
