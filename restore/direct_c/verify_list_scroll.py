"""Closed eight-function scroll graph, original PE versus real C; stdout only."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from native_test_pe import PE
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
pin='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
path=ROOT/'build/smoke-game/Pharaoh.exe';assert hashlib.sha256(path.read_bytes()).hexdigest()==pin
native=PE(path);candidate=PE(args.dll);owned={};entries={0x40d1b0,0x4483f0,0x55bb90,0x4cde40,0x4cde60,0x4cde80,0x5013a0,0x502780}
for batch in ['batch-0064.json','batch-0640.json','batch-1728.json','batch-2240.json','batch-2304.json','batch-2752.json']:
    cache=json.loads((ROOT/'metadata/native-function-audit-cache'/batch).read_text());assert cache['native_sha256']==pin
    for f in cache['entries']:
        if int(f['requested_address'],16) not in entries:continue
        assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more']
        for i in f['assembly']['items']:
            a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert native.data[a-native.base:a-native.base+len(b)]==b
            owned[a]=b
assert len(owned)==275
table=bytes(native.data[0x5013c4-native.base:0x5013df-native.base])
assert table==struct.pack('<II',0x5013b9,0x5013bf)+bytes.fromhex('00000000000000000101010100010001000100')
STACK=0x30000000;ENTRY=STACK+0x8000;STOP=0x31000000
indices=[0,1,3999,0xffffffff];pages={0xe38000,0xe3d000,0xead000,0xeaa000,0xf53000,0x785000,0x78a000}
scenes=[0,1,256,0xffffffff]
pages|={((0xea60a4+i*0x49d8)&0xffffffff)&~4095 for i in scenes}
pages|={((0x799ab8+i*264)&0xffffffff)&~4095 for i in indices};pages=sorted(pages)
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines=[];seen=set();trace=[];checked=0
for compiled in [False,True]:
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for p in pages:u.mem_map(p,4096)
    u.mem_map(STACK,0x10000);u.mem_map(STOP,4096)
    if compiled:u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
    else:
        for p in {a&~4095 for a in owned}:u.mem_map(p,4096)
        for a,b in owned.items():u.mem_write(a,b)
        u.mem_write(0x5013c4,table)
        def step(cpu,address,size,user):
            assert address in owned,hex(address)
            seen.add(address)
        u.hook_add(UC_HOOK_CODE,step)
    def write(cpu,access,address,size,value,user):
        if STACK<=address and address+size<=STACK+0x10000:return
        assert any(p<=address and address+size<=p+4096 for p in pages),(hex(address),size)
        trace.append((address,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write);machines.append(u)

def compare(function,arguments=(),state=None):
    global checked
    state=state or {};outputs=[]
    for compiled,u in enumerate(machines):
        for p in pages:u.mem_write(p,b'\xa5'*4096)
        for a,v in state.items():u.mem_write(a,struct.pack('<I',v&0xffffffff))
        u.mem_write(STACK,b'\xcc'*0x10000);stack=struct.pack('<'+'I'*(len(arguments)+2),STOP,*[a&0xffffffff for a in arguments],0x12345678)
        u.mem_write(ENTRY,stack);initial=[0x87654321,0x12345678,0xabcdef01,0x22222222,ENTRY,0x33333333,0x44444444,0x55555555]
        for r,v in zip(registers,initial):u.reg_write(r,v)
        u.reg_write(UC_X86_REG_EFLAGS,0xad7);trace.clear()
        u.emu_start(candidate.entry(f'FUN_{function:08x}') if compiled else function,STOP,count=4000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==ENTRY+4
        assert bytes(u.mem_read(ENTRY,len(stack)))==stack
        assert all(u.reg_read(registers[j])==initial[j] for j in [3,5,6,7])
        outputs.append((u.reg_read(UC_X86_REG_EAX),list(trace),[bytes(u.mem_read(p,4096)) for p in pages]))
    assert outputs[0]==outputs[1],(hex(function),arguments,state,outputs[0][:2],outputs[1][:2])
    checked+=1

values=[0,1,2,3,7,8,9,10,19,20,100,255,256,0x10001,0x7fffffff,0x80000000,0xffffffff]
for value in values:compare(0x502780,state={0xe38e68:value})
for value in list(range(256))+values:compare(0x5013a0,(value,))
for a,b in itertools.product(values,repeat=2):
    compare(0x4cde40,(a,b))
    if not (a==0x80000000 and b==0xffffffff):compare(0x4cde80,(a,b))
    if not ((a*100)&0xffffffff==0x80000000 and b==0xffffffff):compare(0x4cde60,(a,b))
rng=random.Random(0x40d1b0)
for j in range(512):
    a,b=rng.getrandbits(32),rng.getrandbits(32)
    for function in [0x4cde40,0x4cde60,0x4cde80]:compare(function,(a,b))
for mode in [0,1,2,3,0x100,0xffffffff]:
    rows={1:15,2:21,3:24}.get(mode,10);span=(((rows*4+2)//3)-6)*16
    for limit,click,origin in itertools.product([0,rows,rows+1,100,0x7fffffff,0xffffffff],[0,1,0x100],[0,0x80000000]):
        for dx,dy in [(413,106),(414,105),(414,106),(446,span+122),(447,106),(414,span+123),(414,span+106),(446,span+107)]:
            compare(0x40d1b0,state={0xe38e68:mode,0xf53e6c:limit,0xe38e00:click,0xeadd90:origin,0xe38e34:(origin+dx)&0xffffffff,0xe38e30:dy,0xeadd6c:indices[checked%4]})
for count,scroll,click,ox,oy in itertools.product([0,1,12,13,16,0x100,0x7fffffff,0x80000000,0xffffffff],[0,1,0x100],[0,1,0x100],[0,0x7fffffff,0x80000000],[0,0x80000000]):
    for dx,dy in [(507,80),(508,79),(508,80),(540,300),(541,80),(508,301),(508,276),(540,277)]:
        compare(0x4483f0,state={0x785d70:count,0x78a620:scroll,0xe38e00:click,0xe38da8:ox,0xe38da4:oy,0xe38e34:(ox+dx)&0xffffffff,0xe38e30:(oy+dy)&0xffffffff})
for count,click,ox,oy,scene in itertools.product([0,15,16,0x100,0x7fffffff,0x80000000,0xffffffff],[0,1,0x100],[0,0x7fffffff,0x80000000],[0,0x80000000],scenes):
    for dx,dy in [(591,78),(592,77),(592,78),(620,366),(621,78),(592,367),(592,334),(620,335)]:
        compare(0x55bb90,state={0xf53e6c:count,0xe38e00:click,0xf53f0c:ox,0xf53f08:oy,0xe38e34:(ox+dx)&0xffffffff,0xe38e30:(oy+dy)&0xffffffff,0xeaac28:scene})
assert seen==set(owned),sorted(hex(a) for a in set(owned)-seen)
print(f'PASS {checked} C/native cases;275/275 instructions;8 real functions;full-width rows/coords/scene,wrap/IDIV,guards/order/RET0;division-fault domain excluded')
