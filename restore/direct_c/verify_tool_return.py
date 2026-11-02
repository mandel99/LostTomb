"""Two real C functions versus authenticated native instructions; stdout only."""
from pathlib import Path
import argparse, hashlib, itertools, json, struct, sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from native_test_pe import PE

ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
original=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(original.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(original);candidate=PE(args.dll);owned={}
for batch in ['batch-0000.json','batch-1216.json']:
    cache=json.loads((ROOT/'metadata/native-function-audit-cache'/batch).read_text())
    assert cache['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
    for f in cache['entries']:
        if f['requested_address'] not in ['00405ba0','00486060']:continue
        assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more']
        for instruction in f['assembly']['items']:
            a=int(instruction['address'],16);data=bytes.fromhex(instruction['bytes'])
            assert native.data[a-native.base:a-native.base+len(data)]==data
            owned[a]=data
assert len(owned)==43
STACK=0x30000000;ENTRY=STACK+0x8000;STOP=0x31000000;seen=set()
indices=[0,1,3999,0xffffffff]
cells=[(0x7999d8+i*264)&0xffffffff for i in indices]
pages=sorted({0x605000,0xbc9000,0xbca000,0xe38000}|{a&~4095 for a in cells})
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,
           UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines=[];trace=[]
for compiled in [False,True]:
    cpu=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in pages:cpu.mem_map(page,4096)
    cpu.mem_map(STACK,0x10000);cpu.mem_map(STOP,4096)
    if compiled:
        cpu.mem_map(candidate.base,(len(candidate.data)+4095)&~4095)
        cpu.mem_write(candidate.base,bytes(candidate.data))
    else:
        for page in {a&~4095 for a in owned}:cpu.mem_map(page,4096)
        for a,data in owned.items():cpu.mem_write(a,data)
        def native_step(u,address,size,user):
            assert address in owned,hex(address)
            seen.add(address)
        cpu.hook_add(UC_HOOK_CODE,native_step)
    def written(u,access,address,size,value,user):
        if STACK<=address and address+size<=STACK+0x10000:return
        assert any(p<=address and address+size<=p+4096 for p in pages),(hex(address),size)
        trace.append((address,size,value&((1<<(8*size))-1)))
    cpu.hook_add(UC_HOOK_MEM_WRITE,written);machines.append(cpu)

def compare(mode,flag,action,index):
    outputs=[]
    for compiled,cpu in enumerate(machines):
        for p in pages:cpu.mem_write(p,b'\xa5'*4096)
        cpu.mem_write(0xe38dda,bytes([mode]));cpu.mem_write(0x6059e8,struct.pack('<I',flag))
        cpu.mem_write(0xbca310,struct.pack('<I',action));cpu.mem_write(0xbc9800,struct.pack('<I',index))
        cpu.mem_write(STACK,b'\xcc'*0x10000);frame=struct.pack('<III',STOP,0x89abcdef,0x12345678)
        cpu.mem_write(ENTRY,frame)
        initial=[0x87654321,0x12345678,0xabcdef01,0x22222222,ENTRY,0x33333333,0x44444444,0x55555555]
        for reg,value in zip(registers,initial):cpu.reg_write(reg,value)
        cpu.reg_write(UC_X86_REG_EFLAGS,0xad7);trace.clear()
        cpu.emu_start(candidate.entry('FUN_00405ba0') if compiled else 0x405ba0,STOP,count=1000)
        assert cpu.reg_read(UC_X86_REG_EIP)==STOP and cpu.reg_read(UC_X86_REG_ESP)==ENTRY+4
        assert bytes(cpu.mem_read(ENTRY,len(frame)))==frame
        assert all(cpu.reg_read(registers[j])==initial[j] for j in [3,5,6,7])
        outputs.append((cpu.reg_read(UC_X86_REG_EAX),list(trace),[bytes(cpu.mem_read(p,4096)) for p in pages]))
    assert outputs[0]==outputs[1],(mode,flag,action,index,outputs[0][:2],outputs[1][:2])

cases=[(mode,flag,0,0) for mode in range(256) for flag in [0,0x100]]
cases.extend(itertools.product([0,1,255],[0,1,0x80000000,0xffffffff],[0,1,2,0x100,0xffffffff],indices))
for case in cases:compare(*case)
assert seen==set(owned),sorted(hex(a) for a in set(owned)-seen)
print(f'PASS {len(cases)} C/native cases;43/43 instructions;real tool helper;DWORD flags/index/action;ordered writes,full EAX,guards,RET0')
