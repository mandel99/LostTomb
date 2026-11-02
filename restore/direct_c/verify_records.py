"""Compare six actual compiled menu leaves to pinned native bytes; stdout only."""
from pathlib import Path
import argparse,hashlib,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_MEM_WRITE
from unicorn.x86_const import *

from native_test_pe import PE

ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
original=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(original.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(original);candidate=PE(args.dll)
bounds={0x442500:0x442536,0x444920:0x44494c,0x448cf0:0x448d02,
        0x406190:0x4061d3,0x406250:0x4062c9,0x4e2ab0:0x4e2b10}
STOP=0x31000000;STACK=0x30000000;ENTRY=STACK+0x8000
regions=[(0x775910,16),(0x77de00,0x6600),(0x785d60,32),(0xe38dc0,48),
         (0x705690,32),(0x70e3c0,32),(0xe38e20,32),(0xf53db0,32),
         (0xe38da0,32),(0x5e44a0,64),(0xe94e20,0x200),(0xf53f20,48)]
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,
           UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines=[];writes=[];hits=set();total=0
for index in range(2):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x770000,0x20000);u.mem_map(0xe38000,0x1000)
    u.mem_map(0x700000,0x10000);u.mem_map(0xf53000,0x1000)
    u.mem_map(0x5e4000,0x1000);u.mem_map(0xe94000,0x2000)
    u.mem_map(STACK,0x10000);u.mem_map(STOP,4096)
    if index:
        u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095)
        u.mem_write(candidate.base,bytes(candidate.data))
    else:
        mapped=set()
        for start,end in bounds.items():
            page=start&~4095
            if page not in mapped:u.mem_map(page,4096);mapped.add(page)
            u.mem_write(start,bytes(native.data[start-native.base:end-native.base]))
        def code(cpu,address,size,user):
            assert any(a<=address<b for a,b in bounds.items()),f'native escaped {address:08x}'
            hits.add(address)
        u.hook_add(UC_HOOK_CODE,code)
    def written(cpu,access,address,size,value,user):
        if any(a<=address<a+n for a,n in regions):
            kind=int(access==UC_MEM_WRITE)
            value=value if kind else int.from_bytes(cpu.mem_read(address,size),'little')
            writes.append((kind,address,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE|UC_HOOK_MEM_READ,written);machines.append(u)

def compare(function,changes,argument=None):
    global total
    outcomes=[]
    for index,u in enumerate(machines):
        for a,n in regions:u.mem_write(a,b'\xa5'*n)
        for a,data in changes:u.mem_write(a,data)
        u.mem_write(STACK,b'\xcc'*0x10000)
        stack=struct.pack('<III',STOP,0xa5a5a5a5 if argument is None else argument&0xffffffff,0x12345678)
        u.mem_write(ENTRY,stack)
        initial=[0x87654321,0x12345678,0xabcdef01,0x22222222,ENTRY,0x33333333,0x44444444,0x55555555]
        for r,v in zip(registers,initial):u.reg_write(r,v)
        u.reg_write(UC_X86_REG_EFLAGS,0xad7);writes.clear()
        entry=candidate.entry(f'FUN_{function:08x}') if index else function
        u.emu_start(entry,STOP,count=20000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP
        assert u.reg_read(UC_X86_REG_ESP)==ENTRY+4
        assert bytes(u.mem_read(ENTRY,len(stack)))==stack
        for j in [3,5,6,7]:assert u.reg_read(registers[j])==initial[j]
        outcomes.append(([bytes(u.mem_read(a,n)) for a,n in regions],list(writes),
                         u.reg_read(UC_X86_REG_EAX) if function==0x444920 else None))
    assert outcomes[0]==outcomes[1],(hex(function),argument,changes[:2])
    total+=1

# First available slot, both byte values that stop the native loop, every slot.
for occupied in range(201):
    for invalid in [0,1]:
        data=bytearray(b'\xa5'*0x6600)
        for j in range(200):data[0x77f749+j*0x62-0x77de00]=255 if j<occupied else invalid
        compare(0x442500,[(0x77de00,bytes(data))])

# Every record and signed WORD key; mismatches retain full DWORD argument width.
for selected in range(61):
    for value in [-32768,-1,0,1,32767]:
        data=bytearray(0x6600)
        data[selected*0x6a]=1;struct.pack_into('<h',data,selected*0x6a+0x56,value)
        compare(0x444920,[(0x77de00,bytes(data))],value)
for active in range(256):
    data=bytearray(0x6600);data[30*0x6a]=active
    struct.pack_into('<h',data,30*0x6a+0x56,-1)
    compare(0x444920,[(0x77de00,bytes(data))],-1)
for value in [-2147483648,-32769,32768,65535,2147483647]:
    data=bytearray(b'\xff'*0x6600)
    compare(0x444920,[(0x77de00,bytes(data))],value)
data=bytearray(b'\xff'*0x6600)
compare(0x444920,[(0x77de00,bytes(data))],-1) # First duplicate wins.
for mode in range(256):compare(0x448cf0,[(0x785d6c,bytes([mode]))])

# Signed, half-open cursor rectangles and strict top-bar endpoints; full DWORDs.
def dword(value):return struct.pack('<I',value&0xffffffff)
for left,top,right,bottom in [(10,20,100,200),(-1000,-1000,-1,-1),
                              (-2147483648,-2147483648,2147483647,2147483647),
                              (100,200,10,20),(0,0,0,0)]:
    coords=[-2147483648,-1001,-1000,-1,0,9,10,11,19,20,21,99,100,101,199,200,201,2147483647]
    for x in coords:
        for y in coords:
            compare(0x406190,[(0xe38e34,dword(x)),(0xe38e30,dword(y)),
                    (0x70e3c8,struct.pack('<iiii',bottom,right,top,left))])
for left in [-2147483648,-1000,-129,-128,-1,0,2147483520,2147483647]:
    for overlap in [False,True]:
        anchors=[left,left if overlap else left+500,left if overlap else left+1000]
        for delta in [-1,0,1,127,128,129,501,1001]:
            for y in [-2147483648,3,4,17,18,2147483647]:
                compare(0x406250,[(0xe38e34,dword(left+delta)),(0xe38e30,dword(y)),
                        *((0xf53dc4-j*4,dword(a)) for j,a in enumerate(anchors))])

# DWORD-width arithmetic wraps before signed division; rows copy only WORD y.
widths=list(range(257))+[640,800,1024,1920,0x7fffffff,0x80000000,
                              0x80000001,0xffffff4b,0xffffff4c,0xffffff4d,
                              0xffffffff,0x100b4,0xffff00b4]
for width in widths:
    for seed in [0,0xffff,0x8000,0x12340001,0xdeadbeef]:
        compare(0x4e2ab0,[(0xe38db0,dword(width)),
                *((0x5e44ac+j*8,dword(seed+j*0x10001)) for j in range(5))])

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
expected={i.address for a,b in bounds.items() for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(native.data[a-native.base:b-native.base]),a)}
assert hits==expected,(sorted(expected-hits),sorted(hits-expected))
print(f'PASS {total} compiled C/native cases; {len(hits)} native instructions, ordered reads/writes, signed keys, DWORD count/layout, stack/data guards')
