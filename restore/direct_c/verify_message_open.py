"""Three whole message-open functions versus original instructions; stdout only."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_MEM_WRITE
from unicorn.x86_const import *
from native_test_pe import PE
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);args=ap.parse_args()
p=ROOT/'build/smoke-game/Pharaoh.exe';pin='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert hashlib.sha256(p.read_bytes()).hexdigest()==pin
native=PE(p);candidate=PE(args.dll);owned={};entries={0x4072c0,0x4c5bf0,0x4c8040}
for batch in ['batch-0064.json','batch-1600.json']:
    cache=json.loads((ROOT/'metadata/native-function-audit-cache'/batch).read_text());assert cache['native_sha256']==pin
    for f in cache['entries']:
        if int(f['requested_address'],16) not in entries:continue
        assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more']
        for i in f['assembly']['items']:
            a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert native.data[a-native.base:a-native.base+len(b)]==b;owned[a]=b
assert len(owned)==93
def alias_index(base,target,stride):
    assert stride%16==0 and (target-base)%16==0
    return (((target-base)&0xffffffff)//16*pow(stride//16,-1,1<<28))% (1<<28)
messages=[0,15,16,17,255,256,0xffffffff,0x80000000,alias_index(0xe209f4,0xd3b124,80),alias_index(0xe209f4,0xd3aae4,80),alias_index(0xe209f4,0xd3b184,80)]
events=[0,1,255,256,0xffffffff,alias_index(0xe9524e,0xd3aaee,48)]
pages={0xd3a000,0xd3b000,0xe38000,0x705000,0xe95000}
pages|={((0xe209f4+m*80)&0xffffffff)&~4095 for m in messages}
pages|={((0xe9524e+e*48)&0xffffffff)&~4095 for e in events};pages=sorted(pages)
STACK=0x30000000;SP=STACK+0x8000;STOP=0x31000000
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
machines=[];hits=set();accesses=[];calls=[];checked=0
def put(u,a,v,n=4):u.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
for compiled in [False,True]:
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a in pages:u.mem_map(a,4096)
    u.mem_map(STACK,0x10000);u.mem_map(STOP,4096)
    if compiled:u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
    else:
        for page in {a&~4095 for a in owned}:u.mem_map(page,4096)
        for a,b in owned.items():u.mem_write(a,b)
    actual_entries={candidate.entry(f'FUN_{a:08x}'):a for a in entries} if compiled else {a:a for a in entries}
    def step(cpu,address,size,user,entry_map=actual_entries):
        if not user:
            assert address in owned,hex(address);hits.add(address)
        if address in entry_map:
            target=entry_map[address];sp=cpu.reg_read(UC_X86_REG_ESP)
            args=tuple(struct.unpack('<3I',bytes(cpu.mem_read(sp+4,12)))) if target==0x4c5bf0 else ()
            calls.append((target,args))
    u.hook_add(UC_HOOK_CODE,step,user_data=compiled)
    def access(cpu,kind,address,size,value,user):
        if STACK<=address and address+size<=STACK+0x10000:return
        assert any(a<=address and address+size<=a+4096 for a in pages),(hex(address),size)
        if kind!=UC_MEM_WRITE:value=int.from_bytes(cpu.mem_read(address,size),'little')
        accesses.append((int(kind==UC_MEM_WRITE),address,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_READ|UC_HOOK_MEM_WRITE,access);machines.append(u)

def compare(function,message=0,background=0,template=0,event=0,media=0,enabled=1,mode=1,selector=0):
    global checked
    results=[]
    for compiled,u in enumerate(machines):
        for p in pages:u.mem_write(p,b'\xa5'*4096)
        put(u,0xe951e4,event);put(u,(0xe209f4+message*80)&0xffffffff,media)
        put(u,(0xe9524e+event*48)&0xffffffff,0x81fe,2)
        for m in [15,16,17]:put(u,0xe209f4+m*80,media)
        put(u,0xe950dc,enabled);put(u,0x70569c,selector);put(u,0xe38dda,mode,1)
        arguments=(message,background,template) if function==0x4c5bf0 else ()
        u.mem_write(STACK,b'\xcc'*0x10000);frame=struct.pack('<'+'I'*(len(arguments)+2),STOP,*[a&0xffffffff for a in arguments],0x12345678);u.mem_write(SP,frame)
        initial=[0x87654321,0x12345678,0xabcdef01,0x22222222,SP,0x33333333,0x44444444,0x55555555]
        for r,v in zip(regs,initial):u.reg_write(r,v)
        u.reg_write(UC_X86_REG_EFLAGS,0xad7);accesses.clear();calls.clear()
        u.emu_start(candidate.entry(f'FUN_{function:08x}') if compiled else function,STOP,count=20000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
        assert int.from_bytes(u.mem_read(SP,4),'little')==STOP and int.from_bytes(u.mem_read(SP+len(frame)-4,4),'little')==0x12345678
        assert all(u.reg_read(regs[j])==initial[j] for j in [3,5,6,7])
        results.append(([bytes(u.mem_read(p,4096)) for p in pages],list(accesses),list(calls)))
    if results[0]!=results[1]:
        differing=next(((i,a,b) for i,(a,b) in enumerate(itertools.zip_longest(results[0][1],results[1][1])) if a!=b),None)
        raise AssertionError((hex(function),message,template,event,media,enabled,mode,selector,'same memory',results[0][0]==results[1][0],'first access',differing,'calls',results[0][2],results[1][2]))
    checked+=1

for m,t,e,enabled,media in itertools.product(messages,[0,1,128,255,256],events,[0,1,0x100],[0,0x100]):
    compare(0x4c5bf0,m,0x89abcdef,t,e,media,enabled,(m+t)&255)
for selector in [*range(256),0x100,0x101,0x10001,0x80000001,0xffffffff]:
    compare(0x4072c0,selector=selector,media=0x100,mode=selector&255)
for mode in range(256):compare(0x4c8040,mode=mode)
assert hits==set(owned),sorted(hex(a) for a in set(owned)-hits)
print(f'PASS {checked} whole C/native cases;93/93 instructions;3 real functions,ordered widths/reads/writes/calls,template aliases,wrap/DWORD selector,reset/return-mode order,stack/GPR guards;DF0 and mapped data domain')
