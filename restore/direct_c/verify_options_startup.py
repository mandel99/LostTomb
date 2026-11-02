"""Offline native/C regression for Options startup, dialog layout and road queries.

REA 4147F0/414A90, original SHA pinned below. Bootstrap controls only INI,
hardware/campaign setup and persisted settings endpoints. Parser controls CRT
file/allocation endpoints; its actual search helpers execute. Menu actions run
without substituted providers. This does not certify live rendering or gameplay.
"""
from pathlib import Path
from functools import cache
import hashlib, itertools, re, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'build/offline-verification-deps'), str(ROOT/'direct_c')]
from native_test_pe import PE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

original = ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(original.read_bytes()).hexdigest() == 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
targets=[a for a in sys.argv[1:] if not a.startswith('--')]
target = Path(targets[0]) if targets else ROOT/'build/c-only/Release/PharaohRestoreC.exe'
images = [PE(original), PE(target)]
target_sha = hashlib.sha256(target.read_bytes()).hexdigest()
symbols = target.with_suffix('.map').read_text()
@cache
def sym(name):
    hits = re.findall(r'\s_'+re.escape(name)+r'\s+([0-9a-fA-F]{8})\s', symbols)
    assert len(hits)==1, (name, hits)
    return int(hits[0],16)
def address(c, token): return sym('FUN_%08x'%token) if c else token
def put(u,a,v): u.mem_write(a,struct.pack('<I',v&0xffffffff))
def word(u,a,v): u.mem_write(a,struct.pack('<H',v&65535))
def byte(u,a,v): u.mem_write(a,bytes([v&255]))
def get(u,a): return struct.unpack('<I',u.mem_read(a,4))[0]
SP,STOP,S = 0x30008000,0x31000000,0x32000000
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
saved=[0x12345678,0x23456789,0x3456789a,0x456789ab]
seed=bytes(images[0].data[0x560000-images[0].base:0x126d000-images[0].base])
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state={'hooks':{},'calls':[]}
    def intercept(uc,pc,size,data):
        handler=data['hooks'].get(pc)
        if handler is None:return
        count,cleanup,action=handler
        sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(get(uc,sp+4+i*4) for i in range(count))
        value=action(uc,args)
        uc.reg_write(UC_X86_REG_EAX,value&0xffffffff)
        uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
    u.hook_add(UC_HOOK_CODE,intercept,state)
    machines.append((c,u,state))

def reset(u,state):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000))
    u.mem_write(0xe38e58,b'\xa5'*0x238)
    state['hooks'].clear();state['calls'].clear()
def hook(c,state,token,count,action,cleanup=0):
    state['hooks'][address(c,token)]=(count,cleanup,action)
def run(c,u,token,args=(),cleanup=0,receiver=None,budget=2000000):
    for r,v in zip(registers,[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    if receiver is not None:u.reg_write(UC_X86_REG_ECX,receiver)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(address(c,token),STOP,count=budget)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(token),c,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    return u.reg_read(UC_X86_REG_EAX)
def snapshot(u):return bytes(u.mem_read(0x560000,len(seed)))
def equal(states,label):
    assert states[0]==states[1],(label,[(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(states[0][-1],states[1][-1])) if a!=b][:12])
def bootstrap(c,u,state,width=1024,ram=15,variant=0):
    def ini(uc,args):put(uc,0xe38ed8,ram);return 1
    def campaign(uc,args):assert args==(0,);return 0
    def persisted(uc,args):
        assert args==(0x579274,0xe38e58)
        for a,v in [(0xe38eac,variant%3),(0xe38eec,variant&1),(0xe38ec1,(variant>>1)&1),(0xe38ec3,(variant>>2)&1),(0xe39088,(variant>>3)&1)]:
            (byte if a in (0xe38ec1,0xe38ec3,0xe39088) else put)(uc,a,v)
        return 1
    for token,n,fn in [(0x414a90,0,ini),(0x417dd0,1,campaign),(0x4ceb10,2,persisted)]:hook(c,state,token,n,fn)
    slot=sym('_imp__GetSystemMetrics@4') if c else 0x56f1a0
    put(u,slot,S+0x49000)
    state['hooks'][S+0x49000]=(1,4,lambda uc,args:width if args==(0,) else 1/0)
    result=run(c,u,0x4147f0)
    state['hooks'].clear()
    if not c:put(u,slot,struct.unpack_from('<I',seed,slot-0x560000)[0])
    return result

def verify_startup():
    count=0
    for width,ram in itertools.product([640,799,800,1023,1024,1920],[15,23,24,256,0xffffffff]):
        states=[]
        for c,u,state in machines:
            reset(u,state);value=bootstrap(c,u,state,width,ram)
            assert get(u,0x5c8fd4)==0x40c7d0
            states.append((value,snapshot(u)))
        equal(states,('bootstrap',width,ram));count+=1
    for variant in range(16):
        states=[]
        for c,u,state in machines:
            reset(u,state);value=bootstrap(c,u,state,1024,256,variant)
            states.append((value,snapshot(u)))
        equal(states,('persisted labels',variant));count+=1
    print('PASS bootstrap: %d native/C settings, signed RAM, width and label cases'%count,flush=True)
    
    for variant,row in itertools.product(range(16),range(1,9)):
        states=[]
        for c,u,state in machines:
            reset(u,state);bootstrap(c,u,state,1024,256,variant)
            for a,v in [(0x705724,0x5c9200),(0x705728,5),(0x70572c,row),(0x705730,2)]:put(u,a,v)
            byte(u,0xe38dda,3);byte(u,0xe38e03,1)
            value=run(c,u,0x425f20,[0x5c91f0,4])
            states.append((value,snapshot(u)))
        equal(states,('Options row',variant,row))
    print('PASS all eight real Options actions through dropdown dispatch: 128 native/C cases',flush=True)
    
    for ram,cpu in [(15,66),(256,1024),(-1,2810),(-1,-2147483648),(2147483647,2147483647)]:
        states=[]
        content=('; regression\r\nRAM = %d\r\nCheckCD = 0\r\nCPU = %d\r\n'%(ram,cpu)).encode()
        for c,u,state in machines:
            reset(u,state)
            def alloc(uc,args):assert args==(20000,);return S+0x1000
            def fopen(uc,args):assert args==(0x5764d8,0x5763bc);return 0x1234
            def fread(uc,args):
                assert args==(S+0x1000,20000,1,0x1234)
                uc.mem_write(args[0],content);return 0
            for token,n,fn in [(0x561a1d,1,alloc),(0x561a0a,2,fopen),(0x561ca1,4,fread),(0x561994,1,lambda uc,args:0),(0x561a2b,1,lambda uc,args:0)]:hook(c,state,token,n,fn)
            value=run(c,u,0x414a90)
            assert get(u,0xe38ed8)==ram&0xffffffff
            assert get(u,0xe38edc)==cpu&0xffffffff
            states.append((value,snapshot(u)))
        equal(states,('actual ini parser',ram,cpu))
    print('PASS actual INI search/parser and numeric conversion, full DWORD RAM/CPU: 5 native/C cases',flush=True)

def verify_render():
    # Observe the complete dialog's calls, not substituted pixels. The same
    # low-level raster/text endpoints are controlled on both sides.
    endpoints={0x536820:4,0x4d6480:8,0x4d6250:9,0x4d62f0:8,
               0x4d6860:9,0x5370d0:0,0x4256a0:4}
    checked=0
    for token,(x,y),hover in itertools.product(
        [0x531190,0x531480,0x531be0,0x531cc0,0x531ef0],
        [(0,0),(80,60),(192,144),(320,240),(640,300)],
        list(range(15))+[257]):
        states=[]
        for c,u,state in machines:
            reset(u,state)
            for a,v in [(0xe38da8,x),(0xe38da4,y),(0x7056c8,hover),
                        (0xf56f8c,0x10203),(0xf56fa4,0x40506),(0xf56fb4,0x70809),
                        (0x6de510,0xaaaa if hover%2 else 0x1554),
                        (0xe38e5c,hover%2),(0xe39080,2),(0xe38e6c,90),
                        (0xe38e70,70),(0xe38ec4,100),(0xe38ec8,80),
                        (0xe38ecc,100),(0xe38ed0,100)]:put(u,a,v)
            for a in [0xe38e63,0xe38e64,0xe38e65,0xe38ec0]:byte(u,a,hover%2)
            for endpoint,n in endpoints.items():
                def observe(uc,args,endpoint=endpoint,state=state):
                    state['calls'].append((endpoint,args));return 0
                hook(c,state,endpoint,n,observe)
            run(c,u,token)
            states.append((tuple(state['calls']),snapshot(u)))
        assert states[0][0]==states[1][0],('dialog render calls',hex(token),x,y,hover,
            next(((i,a,b) for i,(a,b) in enumerate(zip(states[0][0],states[1][0])) if a!=b),
                 (len(states[0][0]),len(states[1][0]))))
        equal(states,('dialog render state',hex(token),x,y,hover));checked+=1
    print('PASS five complete Options dialog layouts, fonts, highlights and timers: %d native/C cases'%checked,flush=True)

def verify_neighbors():
    # Exercise both real dispatchers that compose the retained register frame.
    # No game providers or operating-system endpoints are substituted here.
    offsets=struct.unpack_from('<24i',images[0].data,0x5cc9e0+4*96-images[0].base)
    offset=next(v for v in offsets if v)
    checked=0
    for token,player,variant in itertools.product([0x480180,0x4802c0,0x480460,0x4806a0],[0,3],range(6)):
        expected=None
        expected_args=None
        for mode in ['native','public','resource','transport']:
            if mode=='transport' and token not in (0x480180,0x4806a0):continue
            c=mode!='native';_,u,state=machines[int(c)];reset(u,state)
            put(u,0xbca2d8,0x1200);put(u,0xeaac28,player)
            for i in range(10):put(u,0xea5f1c+player*0x49d8+i*8,i+1)
            cell=0x1200+100+100*228+offset
            terrain=[0,0x40,0x44,0x48,0x48,0x40][variant]
            put(u,0xaf598c+cell*4,terrain);byte(u,0xa76aec+cell,3 if variant!=5 else 250)
            word(u,0xb4e6fc+cell*2,400)
            word(u,0x7999e8+400*264,136 if variant==3 else 138 if variant==4 else 70)
            args=(100,100,4) if token in (0x480180,0x480460) else (100,100)
            if mode in ('native','public'):
                value=run(c,u,token,args)
                if token==0x4806a0 and variant in (0,1):
                    assert value==variant,(mode,'expected empty/connected road query',player,variant,value)
                if mode=='native':expected_args=bytes(u.mem_read(SP+4,len(args)*4))
            else:
                vsp=S+0x40000;frame=S+0x48000
                regs=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
                put(u,vsp,STOP)
                for i,v in enumerate(args):put(u,vsp+4+i*4,v)
                boundary='restore_resource_neighbors_dispatch' if mode=='resource' else 'restore_actor_transport_dispatch'
                u.mem_write(frame,struct.pack('<10I',*regs,2,sym(boundary)))
                put(u,SP,STOP)
                for i,v in enumerate([0x45db7c,token,frame]):put(u,SP+4+i*4,v)
                u.reg_write(UC_X86_REG_ESP,SP)
                u.emu_start(sym(boundary),STOP,count=2000000)
                assert u.reg_read(UC_X86_REG_EIP)==STOP,(mode,hex(token))
                assert u.reg_read(UC_X86_REG_ESP)==SP+4
                out=struct.unpack('<8I',u.mem_read(frame,32))
                assert out[4]==vsp+4 and [out[i] for i in [3,5,6,7]]==saved
                # 480460 intentionally rewrites its native by-value x/y slots.
                assert bytes(u.mem_read(vsp+4,len(args)*4))==expected_args,(mode,hex(token),'native argument slots')
                value=out[0]
            observed=(value,snapshot(u))
            if expected is None:expected=observed
            else:equal([expected,observed],('neighbor composition',hex(token),player,variant,mode));checked+=1
    print('PASS four real neighboring-cell functions through public and shared dispatchers: %d native/C comparisons'%checked,flush=True)

if __name__=='__main__':
    if '--neighbors-only' not in sys.argv:
        if '--render-only' not in sys.argv:verify_startup()
        verify_render()
    if '--render-only' not in sys.argv:verify_neighbors()
    print('Verified target SHA256',target_sha)
