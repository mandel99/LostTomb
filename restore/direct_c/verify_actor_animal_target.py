"""Offline animal-target native/C comparison; clock is the only platform fixture.

Protocol child results and read-triggered RAM mutations are explicit lanes.
Byte APIs compare AL; periodic and search named/raw entries compare full EAX.
"""
from pathlib import Path
from functools import cache
import hashlib, itertools, re, struct, sys
ROOT = next(p for p in Path(__file__).resolve().parents if (p/'runtime/actor_civic.c').is_file())
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
def snapshot(u):return bytes(u.mem_read(0x560000,len(seed)))

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
    u.hook_add(UC_HOOK_CODE,intercept,state,begin=S+0x49000,end=S+0x49000)
    machines.append((c,u,state))

from unicorn import UC_HOOK_MEM_WRITE, UC_HOOK_MEM_READ, UC_HOOK_MEM_INVALID, UcError
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/actor-animal-target-native.json').read_text())
records=[]
for path in catalogue['evidence_files']:
    d=json.loads((ROOT/'metadata'/path).read_text());records+=d.get('records',[d])
evidence={e['evidence_id']:e for e in records}
owned={};branches={};hits=set();edges=set();last=[None]
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);e=evidence[f['bytes_evidence_id']]
    assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==catalogue['native_sha256']
    assert e['normalized_result']['complete'] and e['normalized_result']['bytes_hex']==f['bytes_hex']
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in code}
    assert len(code)==f['instruction_count']
    for i in code:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
owner={pc:a for a,pcs in owned.items() for pc in pcs};points=set(owner)
names={int(f['address'],16):f['name'] for f in catalogue['functions']}
arity={0x4eaec0:1,0x4eb000:1,0x4ebc90:10,0x4eabf0:6,0x56233e:1,
       0x4ea9c0:1,0x4ea9a0:1,0x4ec0f0:7,0x4eb800:9,0x4b3250:1,
       0x51cd80:4,0x4ec410:2,0x5181c0:4}
POOL,STRIDE,HISTORY,RNGA,RNGB=0xbcd5f8,388,0xd3b1c4,0xd3b384,0xd3b380
CLOCK,FRAME,FSP,FLAGS=S+0x49000,0x30000100,0x30006000,0x30000140
checked=invocations=actual_cases=protocol_cases=synthetic_cases=0

def restore_seed(u):
    current=bytes(u.mem_read(0x560000,len(seed)))
    for offset in range(0,len(seed),4096):
        page=seed[offset:offset+4096]
        if current[offset:offset+4096]!=page:u.mem_write(0x560000+offset,page)

for compiled,u,state in machines:
    state.update(compiled=compiled,collect=False,read_hook_handles=[])
    def write(uc,access,at,size,value,data):
        if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x48000):
            data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S,end=S+0x47fff)
    def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
    u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
    def read(uc,access,at,size,value,data):
        if not data['collect'] or at not in data['read_mutations']:return
        n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n
        for nth,changes in data['read_mutations'][at]:
            if n==nth:
                data['events'].append(('read_mutation',at,n,changes))
                for dst,value in changes:put(uc,dst,value)
    state['read_callback']=read
    if not compiled:
        def visit(uc,pc,size,data):
            if not data['collect'] or owner.get(pc) in data['controlled']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            last[0]=pc
        for pcs in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(pcs),end=max(pcs))
    def child(uc,pc,size,data):
        if not data['collect']:return
        token=data['entries'][pc]
        if token==data['fn']:return
        sp=uc.reg_read(UC_X86_REG_ESP);args=[get(uc,sp+4+i*4) for i in range(arity[token])]
        n=data['counts'].get(token,0)+1;data['counts'][token]=n
        meaningful=list(args)
        if token in [0x4ebc90,0x4eb800]:meaningful=[*args[:5],*args[7:]]
        if token==0x4ec0f0:meaningful=[*args[:2],*args[4:6],0x4eae20]
        if token==0x4eabf0:meaningful=args[:4]
        if token==0x4ec410:meaningful[1]&=255
        data['events'].append((token,tuple(meaningful)))
        for dst,value in data['cfg'].get('child_mutations',{}).get((token,n),[]):put(uc,dst,value)
        if token not in data['controlled']:return
        values=data['cfg'].get('results',{}).get(token,[0]);value=values[min(n-1,len(values)-1)]
        if token in [0x4ebc90,0x4eb800,0x4ec0f0,0x4eabf0]:
            first=5 if token in [0x4ebc90,0x4eb800] else 2 if token==0x4ec0f0 else 4
            for i,key in [(first,'child_x'),(first+1,'child_y')]:
                if key in data['cfg']:put(uc,args[i],data['cfg'][key])
        uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
    state['entries']={}
    for token in arity:
        name='restore_building_nearest_search' if token==0x4eb800 else 'FUN_%08x'%token
        pc=token if not compiled else sym(name)
        state['entries'][pc]=token;u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)

def setup(compiled,u,state,fn,cfg):
    for handle in state['read_hook_handles']:u.hook_del(handle)
    state['read_hook_handles']=[]
    state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],faults=[],counts={},controlled=set(cfg.get('controlled',[])),read_mutations=cfg.get('read_mutations',{}),read_counts={})
    state['hooks'].clear();state['calls'].clear();last[0]=None
    restore_seed(u);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
    u.mem_write(POOL,bytes(2000*STRIDE));u.mem_write(0xb350dc,bytes(51984*2))
    u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',4))*51984);u.mem_write(0xac2d4c,bytes([cfg.get('occupied',0)])*51984)
    for at,value in [(0xbca2d0,cfg.get('width',228)),(0xbca2d4,cfg.get('height',228)),(0xbca2d8,cfg.get('base',1000)),(0xbc9768,0x12345678),(0xd3b4e0,0xabcdef01),(RNGA,cfg.get('first',0x12345678)),(RNGB,cfg.get('second',0x3456789a)),(HISTORY,cfg.get('history',0)),(0xd3b374,0x13572468),(0x784cb0,cfg.get('scene',0)),(0x604950,0xabcdef01)]:put(u,at,value)
    word(u,0x784b94,cfg.get('variant',0))
    off=((cfg.get('variant',0)+(cfg.get('scene',0)&255)*2)*48)&0xffffffff
    put(u,(0x5e4cd0+off)&0xffffffff,cfg.get('threshold',0));put(u,(0x5e4cd4+off)&0xffffffff,cfg.get('radius',0))
    actor=cfg.get('actor',1);offset=(actor*STRIDE)&0xffffffff
    for at,value in [(0xbcd602,cfg.get('kind',70)),(0xbcd606,cfg.get('status',1)),(0xbcd630,cfg.get('state',8)),(0xbcd663,cfg.get('counter',0)),(0xbcd68a,cfg.get('progress',0))]:byte(u,(at+offset)&0xffffffff,value)
    for at,value in [(0xbcd60c,cfg.get('actor_x',10)),(0xbcd60e,cfg.get('actor_y',11)),(0xbcd61c,71),(0xbcd61e,72),(0xbcd62e,1234),(0xbcd632,0)]:word(u,(at+offset)&0xffffffff,value)
    for i,spec in cfg.get('actors',{}).items():
        off=(i*STRIDE)&0xffffffff
        for at,key,default in [(0xbcd602,'kind',70),(0xbcd606,'status',1),(0xbcd630,'state',16)]:byte(u,(at+off)&0xffffffff,spec.get(key,default))
        for at,key,default in [(0xbcd600,'next',0),(0xbcd60c,'x',20),(0xbcd60e,'y',21),(0xbcd61c,'target_x',30),(0xbcd61e,'target_y',31)]:word(u,(at+off)&0xffffffff,spec.get(key,default))
        put(u,0x7090cc+spec.get('kind',70)*56,spec.get('enabled',1))
    types=cfg.get('types',[70,68,69]);ptr=cfg.get('types_ptr',S+256)
    if ptr:
        for i,value in enumerate(types):put(u,ptr+i*4,value)
    for at,value in cfg.get('bytes',[]):byte(u,at,value)
    for at,value in cfg.get('words',[]):word(u,at,value)
    for at,value in cfg.get('stores',[]):put(u,at,value)
    slot=sym('_imp__timeGetTime@0') if compiled else 0x56f22c
    state['clock_slot']=slot;state['clock_saved']=get(u,slot);put(u,slot,CLOCK)
    def clock(uc,args):
        state['events'].append(('clock',))
        for at,value in cfg.get('clock_mutations',[]):put(uc,at,value)
        return cfg.get('clock',0x12345678)
    state['hooks'][CLOCK]=(0,0,clock)
    for at in state['read_mutations']:
        def anchored_read(uc,access,start,size,value,data,anchor=at):
            if start==anchor:data['read_callback'](uc,access,start,size,value,data)
        state['read_hook_handles'].append(u.hook_add(UC_HOOK_MEM_READ,anchored_read,state,begin=at,end=at))
    if fn==0x56233e:return [cfg.get('seed',0x76543210)]
    if fn==0x4ebc90:return [cfg.get('x',10),cfg.get('y',11),cfg.get('count',len(types)),ptr,cfg.get('mask',0),cfg.get('out_x',S+128),cfg.get('out_y',S+132),cfg.get('rings',0),cfg.get('excluded',1),cfg.get('wanted_state',16)]
    if fn==0x4eabf0:return [cfg.get('x',10),cfg.get('y',11),cfg.get('from_x',20),cfg.get('from_y',21),cfg.get('out_x',S+128),cfg.get('out_y',S+132)]
    return [actor]

def invoke(compiled,u,state,fn,args,route):
    for reg,value in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    actual_args=args;pc=fn if not compiled else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
    if route=='raw':
        for i,value in enumerate([0xa5b6c7d8,0x22222222,0x33333333,saved[0],FSP,*saved[1:]]):put(u,FRAME+i*4,value)
        put(u,FLAGS,0x202);put(u,FSP,STOP)
        for i,value in enumerate(args):put(u,FSP+4+i*4,value)
        actual_args=[fn,FRAME,FLAGS];pc=sym('restore_actor_civic_raw')
    for i,value in enumerate(actual_args):put(u,SP+4+i*4,value)
    try:u.emu_start(pc,STOP,count=50000000)
    except UcError:return 'fault'
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if route=='raw':
        assert [get(u,FRAME+i*4) for i in range(1,8)]==[0x22222222,0x33333333,saved[0],FSP+4,*saved[1:]]
        assert get(u,FLAGS)==0x202
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(fn,**cfg):
    global checked,invocations,actual_cases,protocol_cases,synthetic_cases
    expected=None;native_result=None
    if '--quick' in sys.argv:print('start',hex(fn),cfg,flush=True)
    for compiled,u,state in machines:
        for route in ['native'] if not compiled else ['named','canonical','raw']:
            args=setup(compiled,u,state,fn,cfg);state['collect']=True
            result=invoke(compiled,u,state,fn,args,route);state['collect']=False;invocations+=1
            put(u,state['clock_slot'],state['clock_saved'])
            assert not state['faults'] and result!='fault',(hex(fn),cfg,route,state['faults'])
            if not compiled:native_result=result
            if fn in [0x4eb000,0x4eabf0] or (fn==0x4eaec0 and route=='canonical'):assert (result&255)==(native_result&255),(hex(fn),cfg,route,result,native_result)
            elif fn!=0x56233e or route!='canonical':assert result==native_result,(hex(fn),cfg,route,hex(result),hex(native_result))
            current=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x48000)))
            if expected is None:expected=current
            elif current!=expected:
                trace=[(part,len(expected[part]),len(current[part]),next(((i,a,b) for i,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)) for part in [0,1] if expected[part]!=current[part]]
                memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[2],current[2]),(S,expected[3],current[3])]]
                raise AssertionError((hex(fn),cfg,route,'trace',trace,'memory',memory))
    checked+=1
    if cfg.get('controlled'):protocol_cases+=1
    elif cfg.get('read_mutations') or cfg.get('child_mutations') or cfg.get('clock_mutations'):synthetic_cases+=1
    else:actual_cases+=1
    if checked%20==0:print('progress',checked,hex(fn),flush=True)

if __name__=='__main__':
    exercise(0x56233e);exercise(0x4eaec0,kind=82);exercise(0x4ebc90);exercise(0x4eabf0);exercise(0x4eb000)
    if '--quick' in sys.argv:print('PASS quick animal target',checked,invocations);sys.exit(0)
    for value in [0,1,0x7fffffff,0x80000000,0xffffffff,0x100,0x12345678]:exercise(0x56233e,seed=value)
    for kind in [0,1,25,68,69,70,73,77,81,82,83,84,85,92,100,101,102,103,104,105,255]:
        for fn in [0x4eaec0,0x4eb000]:exercise(fn,kind=kind)
    for fn,states in [(0x4eaec0,[0,4,5,8,12,21,255]),(0x4eb000,[0,4,8,12,14,15,16,17,255])]:
        for status,state in itertools.product([0,1,255],states):exercise(fn,kind=82 if fn==0x4eaec0 else 70,status=status,state=state)
    for counter,threshold in itertools.product([0,1,127,254,255],[0,1,2,128,255,256,0x7fffffff,0x80000000,0xffffffff]):
        exercise(0x4eaec0,kind=82,counter=counter,threshold=threshold)
    for variant,scene in itertools.product([-2,-1,0,1,9],[0,1,0x100,0x102]):
        exercise(0x4eaec0,kind=82,variant=variant,scene=scene,threshold=256)
    tile=1000+11*228+10
    for radius in [0,1,2,0x10000,0xffffffff]:
        exercise(0x4eaec0,kind=82,radius=radius,actors={2:dict(kind=83,state=8)},words=[(0xb350dc+tile*2,2)])
    for result in [0,1,0x100,0x101,0xffffffff,0x80000000]:
        exercise(0x4eaec0,controlled=[0x4ea9c0],results={0x4ea9c0:[result]},threshold=256)
    for found in [0,2,0x40000002]:
        exercise(0x4eaec0,kind=82,controlled=[0x4ec0f0],results={0x4ec0f0:[found]},actors={2:dict(x=-1,y=-32768)},radius=0x12345678)
    exercise(0x4eaec0,kind=82,clock_mutations=[(0xbcd60c+388,0x00170016)])
    exercise(0x4eaec0,kind=82,read_mutations={0x784cb0:[(1,[(0x784b94,1),(0x5e4cd0+48,256)])]})

    # State-filtered spiral: full-width requested values, signed links, the
    # cached head/type, east-only terrain, and the native south-global quirk.
    for count,mask in itertools.product([-2,-1,0,1,2,3],[0,4,0x80000,0x80004]):
        exercise(0x4ebc90,count=count,mask=mask)
    for terrain in [0,4,0x80000,0x80004,0xffffffff]:exercise(0x4ebc90,count=0,mask=4,terrain=terrain)
    for kinds,state,excluded in itertools.product([[70],[0x146],[68,70],[68,69,70]], [16,0x110,0xffffffff],[1,2]):
        exercise(0x4ebc90,types=kinds,wanted_state=state,excluded=excluded,actors={2:{}},words=[(0xb350dc+tile*2,2)])
    exercise(0x4ebc90,actors={2:dict(next=3,kind=68,state=0),3:dict(next=4,kind=70,state=0),4:dict(kind=70,state=16)},words=[(0xb350dc+tile*2,2)])
    exercise(0x4ebc90,actors={-1:dict(kind=70,state=16)},words=[(0xb350dc+tile*2,0xffff)])
    exercise(0x4ebc90,types_ptr=0,count=3)
    for x,y,width,height in [(-1,11,228,228),(10,-1,228,228),(228,11,228,228),(10,228,228,228),(10,11,0,228),(10,11,228,0),(10,11,10,11)]:
        exercise(0x4ebc90,x=x,y=y,width=width,height=height)
    for rings in [1,2,5]:exercise(0x4ebc90,rings=rings)
    exercise(0x4ebc90,rings=-1,width=0,count=0)
    for out_x,out_y in [(S+128,S+128),(0xbc9768,S+132),(S+128,0xbc9768),(0xd3b4e0,S+132),(S+128,0xd3b4e0),(0xbca2d0,S+132),(S+128,0xbca2d4),(S+256,S+132),(S+128,S+256)]:
        exercise(0x4ebc90,out_x=out_x,out_y=out_y)
    for nth in [2,3,4]:
        exercise(0x4ebc90,actors={2:dict(state=0)},words=[(0xb350dc+tile*2,2)],read_mutations={0xbcd630+2*388:[(nth,[(0xbcd630+2*388,16)])]})
    exercise(0x4ebc90,types=[68,70],actors={2:dict(kind=70)},words=[(0xb350dc+tile*2,2)],read_mutations={S+256:[(1,[(0xb350dc+tile*2,0)])]})
    exercise(0x4ebc90,types=[70],actors={2:dict(kind=68,next=3),3:dict(kind=70)},words=[(0xb350dc+tile*2,2)],read_mutations={0xbcd602+2*388:[(1,[(S+256,68)])]})

    # Actual direction, terrain and line providers execute in ordinary lanes.
    for from_x,from_y,terrain in itertools.product([0,10,20],[0,11,21],[0,1,2,4,0x80000,0xffffffff]):
        exercise(0x4eabf0,from_x=from_x,from_y=from_y,terrain=terrain)
    exercise(0x4eabf0,occupied=1)
    probe=[-228,-227,1,229,228,227,-1,-229]
    for direction,nth in itertools.product(range(8),range(7)):
        blocked=[(0xac2d4c+tile+probe[(direction+i)%8],1) for i in range(nth)]
        exercise(0x4eabf0,bytes=blocked,controlled=[0x51cd80,0x4ec410,0x5181c0],results={0x51cd80:[direction],0x4ec410:[1],0x5181c0:[0x100]})
    for direction,terrain_result,line_result in itertools.product(range(8),[0,0x100,1,0x101],[0,1,0x100]):
        exercise(0x4eabf0,controlled=[0x51cd80,0x4ec410,0x5181c0],results={0x51cd80:[direction],0x4ec410:[terrain_result],0x5181c0:[line_result]})
    for out_x,out_y in [(S+128,S+128),(0xbca2d8,S+132),(S+128,0xbca2d8)]:
        exercise(0x4eabf0,out_x=out_x,out_y=out_y,controlled=[0x5181c0],results={0x5181c0:[0]})
    exercise(0x4eabf0,child_mutations={(0x51cd80,1):[(0xbca2d8,2000)]})

    # Real group peers and explicit child protocols exercise both paired RNG
    # phases and the progress timeout. No CRT random endpoint is substituted.
    for state in [0,8,14,15]:
        exercise(0x4eb000,state=state,actors={2:dict(kind=70,state=16)},words=[(0xb350dc+tile*2,2)])
        exercise(0x4eb000,state=state,actors={2:dict(kind=70,state=14)},words=[(0xb350dc+tile*2,2)])
        exercise(0x4eb000,state=state,actors={2:dict(kind=73,state=8)},words=[(0xb350dc+tile*2,2)])
    control=[0x4ea9a0,0x4ebc90,0x4eb800]
    for state,first,second,history in itertools.product([8,14,15],[0,1,0xffffffff,0x12345678],[0,0xffffffff],[0,98,99,100,110,111,0xffffffff]):
        exercise(0x4eb000,state=state,first=first,second=second,history=history,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[2]},actors={2:dict(target_x=-32768,target_y=32767)})
    for search_results,nearest in [([0,0],0),([0,2],0),([0,0],2)]:
        for state in [8,14,15]:exercise(0x4eb000,state=state,controlled=control,results={0x4ea9a0:[1],0x4ebc90:search_results,0x4eb800:[nearest]})
    for first,progress,step in itertools.product([0,1,2,0x12345678],[0,19,20,21,254,255],[0,1,0x100,0x101]):
        exercise(0x4eb000,state=14,first=first,progress=progress,controlled=[*control,0x4eabf0],results={0x4ea9a0:[1],0x4ebc90:[0],0x4eb800:[2],0x4eabf0:[step]},actors={2:dict(x=20,y=21)},child_x=0x1234ffff,child_y=0x56788000)
    for progress in [0,19,20,21,254,255]:
        exercise(0x4eb000,state=14,progress=progress,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[0],0x4eb800:[0]})
    for result in [0,1,0x100,0x101,0xffffffff,0x80000000]:exercise(0x4eb000,controlled=[0x4ea9a0],results={0x4ea9a0:[result]},state=16)
    inverse=pow(97,-1,1<<30)
    for field,destination in [(0xbcd630,RNGA),(0xbcd61c,RNGA),(0xbcd61c,HISTORY),(0xbcd61c,0xd3b374),(0xbcd61e,RNGB+2)]:
        assert (destination-field)%4==0
        actor=(((destination-field)//4)*inverse)%(1<<30)
        exercise(0x4eb000,actor=actor,state=14,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[2]},actors={2:{}},stores=[(HISTORY,0)] if destination==HISTORY else [])
    exercise(0x4eb000,state=14,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[2]},actors={2:{}},read_mutations={0xbcd61e+2*388:[(1,[(0xd3b374,0xabcdef01)])]})
    exercise(0x4eb000,clock_mutations=[(0xbcd60c+388,0x00170016)])
    for state in [8,15,255]:
        exercise(0x4eb000,state=14,progress=20,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[0],0x4eb800:[0]},child_mutations={(0x4eb800,1):[(0xbcd630+388,state)]})
    for state in [8,14,15]:
        exercise(0x4eb000,state=8,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[2]},actors={2:{}},child_mutations={(0x4b3250,1):[(0xbcd630+388,state)]})
    exercise(0x4eb000,state=14,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[0x40000002]},actors={2:{}})
    exercise(0x4eb000,state=14,first=0,history=100,controlled=control,results={0x4ea9a0:[1],0x4ebc90:[0],0x4eb800:[2]},actors={2:{}})
    exercise(0x4ebc90,actors={2:dict(state=0,next=3),3:dict(state=0)},words=[(0xb350dc+tile*2,2)])
    exercise(0x4ebc90,y=-2)
    missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets}-edges
    print('Compared animal target:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM',flush=True)
    print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    audit=json.loads((ROOT/'metadata/actor-animal-target-caller-audit.json').read_text());assert audit['passed']
    for p,sha in audit['source_hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha,('stale caller audit',p)
    dead_edges={(int(a,16),int(b,16)) for a,b in audit['analytically_unreachable_edges']}
    assert not missing and missing_edges==dead_edges,(sorted(hex(a) for a in missing),[(hex(a),hex(b)) for a,b in sorted(missing_edges-dead_edges)])
    sources=['CMakeLists.txt','direct_c/verify_actor_animal_target.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','metadata/actor-animal-target-native.json','metadata/actor-animal-target-caller-audit.json',*audit['source_hashes']]
    for directory in ['src','runtime','include','generated']:
        sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
    proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=checked,invocations=invocations,
        coverage={hex(a):dict(instructions=len(pcs),executed=len(pcs&hits)) for a,pcs in owned.items()},conditional_edges=len(edges),
        unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_instructions=[],analytically_unreachable_edges=audit['analytically_unreachable_edges'],
        actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,
        controlled_algorithm_endpoints_by_lane=dict(actual=[],protocol=['Explicit classifier, search, direction, terrain and line child results; each scenario lists its controlled entries']),
        controlled_platform_endpoints=['timeGetTime: controlled zero-argument clock result and explicitly listed RAM mutation cases'],
        return_contract=dict(periodic_named_and_raw='full EAX',periodic_legacy='AL',group_and_step='AL',state_search='full EAX',seed='full EAX named/raw; legacy void is checked by its store'),
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        limitations=['Five owned native bodies, 905 instructions. The clock is controlled; ordinary lanes execute the game seed, classifiers, searches, direction, terrain, route clearing and line algorithms. No game is launched.','Original data/private bytes, ordered stores, meaningful child arguments, byte/full return contracts and normal-return nonvolatile ABI are compared. Compatibility frames are checked separately from incidental physical scratch registers.','Four impossible positive-span JLE outcomes and the initial direction==previous-direction outcome have authenticated byte-level qualifications. No unexecuted instruction is accepted.','Explicit child contracts and RAM mutations are separated from ordinary algorithms. Arbitrary private/caller-stack corruption, OS exception dispatch, invalid direction outputs from substituted providers and complete gameplay remain unknown.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/actor-animal-target-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
