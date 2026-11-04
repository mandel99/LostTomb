"""Offline native/main-C comparison for route helpers and the hippo callback.

Ordinary lanes execute current C children. Protocol results, platform clock and
RAM mutations are listed separately. No candidate DLL/native-child redirection
is used here, and neither host game is launched.
"""
from pathlib import Path
from functools import cache
import hashlib,itertools,re,struct,sys,json
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

original=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(original.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
targets=[a for a in sys.argv[1:] if not a.startswith('--')]
target=Path(targets[0]) if targets else ROOT/'build/c-only/Release/PharaohRestoreC.exe'
images=[PE(original),PE(target)];target_sha=hashlib.sha256(target.read_bytes()).hexdigest();symbols=target.with_suffix('.map').read_text()
@cache
def sym(name):
    found=re.findall(r'\s_'+re.escape(name)+r'\s+([0-9a-fA-F]{8})\s',symbols)
    assert len(found)==1,(name,found)
    return int(found[0],16)
def put(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def word(u,a,v):u.mem_write(a,struct.pack('<H',v&65535))
def byte(u,a,v):u.mem_write(a,bytes([v&255]))
def get(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
SP,STOP,S=0x30008000,0x31000000,0x32000000
registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
saved=[0x12345678,0x23456789,0x3456789a,0x456789ab]
seed=bytes(images[0].data[0x560000-images[0].base:0x126d000-images[0].base])
def snapshot(u):return bytes(u.mem_read(0x560000,len(seed)))
# The shared startup fixture intentionally owns an instruction-global endpoint
# interceptor. This family has one platform endpoint, so only that exact clock
# address is hooked; nonowned C fill/path bodies run without Python code hooks.
machines=[]
for compiled,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(hooks={},calls=[],read_hook_handles=[])
    def intercept(uc,pc,size,data):
        handler=data['hooks'].get(pc)
        if handler is None:return
        count,cleanup,action=handler;sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(get(uc,sp+4+i*4) for i in range(count))
        value=action(uc,args);uc.reg_write(UC_X86_REG_EAX,value&0xffffffff)
        uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
    u.hook_add(UC_HOOK_CODE,intercept,state,begin=S+0x49000,end=S+0x49000)
    machines.append((compiled,u,state))

catalogue=json.loads((ROOT/'metadata/actor-animal-parent-native.json').read_text())
records=[];evidence_paths=[]
for path in catalogue['evidence_files']:
    rel=path if path.startswith('metadata/') else 'metadata/'+path
    evidence_paths.append(rel)
    d=json.loads((ROOT/rel).read_text());records+=d.get('records',[d])
evidence={e['evidence_id']:e for e in records}
owned={};branches={};hits=set();edges=set();switches=set();last=[None]
names={int(f['address'],16):f['name'] for f in catalogue['functions']}
assert names=={0x4eaac0:'restore_actor_route_line_mode',0x4eab10:'restore_actor_find_clear_vector',0x49e8b0:'restore_actor_update_hippo'}
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);e=evidence[f['bytes_evidence_id']]
    assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==catalogue['native_sha256']
    z=e['normalized_result'];assert z['complete'] and z['bytes_hex']==f['bytes_hex']
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in code}
    assert len(code)==f['instruction_count'] and sum(i.size for i in code)==len(raw)
    for i in code:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());owner={pc:a for a,pcs in owned.items() for pc in pcs};assert len(points)==1036
expected_switches=set()
for site,table,count in [(0x49e9f0,0x49f688,15),(0x49f4f6,0x49f6dc,6)]:
    for a in struct.unpack_from('<'+'I'*count,images[0].data,table-images[0].base):
        assert a in owned[0x49e8b0];expected_switches.add((site,a))
assert len(expected_switches)==21

arity={0x4eaac0:1,0x4eab10:6,0x49e8b0:0,0x519be0:4,0x517f20:4,0x5181c0:4,0x4ec410:2,
       0x53b4a0:3,0x4b17d0:1,0x4b5050:1,0x4b1d50:1,0x4ec7c0:1,0x4a5da0:2,0x4b3250:1,0x4eaec0:1,0x4ec990:1}
compiled_names={t:names.get(t,'FUN_%08x'%t) for t in arity}
compiled_names.update({0x4eaec0:'restore_actor_try_periodic_target',0x4ec990:'restore_actor_seek_other_kind'})
# Observe game calls issued by the owned bodies, including wrapper handoffs.
# Nested calls inside nonowned existing providers are outside this trace tier.
function_symbols=[]
for line in symbols.splitlines():
    m=re.match(r'\s*[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+(\S+)\s+([0-9a-fA-F]{8})\s+f\s',line)
    if m:function_symbols.append((int(m.group(2),16),m.group(1)))
function_starts=sorted({a for a,n in function_symbols})
def function_interval(name):
    at=sym(name);following=next((a for a in function_starts if a>at),None)
    assert following is not None,(name,hex(at))
    return at,following
owned_C_intervals=[function_interval(n) for n in names.values()]
alias_intervals={a:function_interval('FUN_%08x'%a) for a in owned}
def owned_c_return(pc):return any(lo<=pc<hi for lo,hi in owned_C_intervals)
def inside_alias(pc,token):
    lo,hi=alias_intervals[token];return lo<=pc<hi

POOL,STRIDE,HISTORY,RNGA,RNGB=0xbcd5f8,388,0xd3b1c4,0xd3b384,0xd3b380
CLOCK,FRAME,FSP,FLAGS=S+0x49000,0x30000100,0x30006000,0x30000140
PARENT_CHILDREN=[0x53b4a0,0x4b17d0,0x4b5050,0x4b1d50,0x4ec7c0,0x4a5da0,0x4b3250,0x4eab10,0x4eaec0,0x4ec990]
checked=invocations=actual_cases=protocol_cases=synthetic_cases=0

def restore_seed(u):
    current=bytes(u.mem_read(0x560000,len(seed)))
    for offset in range(0,len(seed),4096):
        page=seed[offset:offset+4096]
        if current[offset:offset+4096]!=page:u.mem_write(0x560000+offset,page)

for compiled,u,state in machines:
    state.update(compiled=compiled,collect=False)
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
    # Mutation hooks are installed only at the addresses requested by a case.
    # The callback still checks exact read starts, preserving the prior filter.
    state['read_callback']=read
    if not compiled:
        def visit(uc,pc,size,data):
            if not data['collect'] or owner.get(pc) in data['controlled']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0] in [0x49e9f0,0x49f4f6]:switches.add((last[0],pc))
            last[0]=pc
        for pcs in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(pcs),end=max(pcs))
    def child(uc,pc,size,data):
        if not data['collect']:return
        token,is_alias=data['entries'][pc]
        if token==data['fn']:return
        sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        if data['compiled']:
            previous=data['handoff'].get(token)
            if not is_alias and previous and (previous==(sp,ret) or inside_alias(ret,token)):
                del data['handoff'][token];return
            if not owned_c_return(ret):return
            if is_alias:data['handoff'][token]=(sp,ret)
        elif ret not in points:return
        args=[get(uc,sp+4+i*4) for i in range(arity[token])]
        meaningful=args.copy()
        if token==0x4ec410:meaningful[1]&=255
        n=data['counts'].get(token,0)+1;data['counts'][token]=n
        data['events'].append((token,tuple(meaningful)))
        for dst,value in data['cfg'].get('child_mutations',{}).get((token,n),[]):put(uc,dst,value)
        for dst,value in data['cfg'].get('child_bytes',{}).get((token,n),[]):byte(uc,dst,value)
        if token not in data['controlled']:return
        data['handoff'].pop(token,None)
        values=data['cfg'].get('results',{}).get(token,[0]);value=values[min(n-1,len(values)-1)]
        uc.reg_write(UC_X86_REG_EAX,value&0xffffffff)
        if data['cfg'].get('clobber',True):
            uc.reg_write(UC_X86_REG_ECX,0x6a6b6c6d);uc.reg_write(UC_X86_REG_EDX,0x7a7b7c7d)
        uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
    state['entries']={}
    for token in arity:
        entries=[(token,False)] if not compiled else [(sym(compiled_names[token]),False)]
        if compiled and token in owned:
            alias=sym('FUN_%08x'%token)
            if alias!=entries[0][0]:entries.append((alias,True))
        for pc,is_alias in entries:
            assert pc not in state['entries'],(token,hex(pc))
            state['entries'][pc]=(token,is_alias);u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)

def setup(compiled,u,state,fn,cfg):
    for handle in state['read_hook_handles']:u.hook_del(handle)
    state['read_hook_handles']=[]
    state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],faults=[],counts={},handoff={},controlled=set(cfg.get('controlled',[])),read_mutations=cfg.get('read_mutations',{}),read_counts={},extra_restore=[])
    assert fn not in state['controlled']
    state['hooks'].clear();state['calls'].clear();last[0]=None
    restore_seed(u);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
    for at,value in [(0x784cb0,cfg.get('scene',0)),(0xbca2d8,cfg.get('base',1000)),(0xbca2d0,228),(0xbca2d4,228),
                     (0xd056f8,cfg.get('actor',1)),(0xeaac28,1),(0x7027e1,0),(0xbca2e0,cfg.get('rotation',0)),
                     (HISTORY,cfg.get('history',0)),(0xd3b374,0x13572468),(RNGA,cfg.get('first',0x12345678)),(RNGB,cfg.get('second',0x3456789a)),(0x604950,0xabcdef01)]:put(u,at,value)
    word(u,0x784b94,cfg.get('variant',0))
    mode=cfg.get('mode',0)
    table,offset=(0x5e4c3c,(cfg.get('scene',0)&255)*48) if mode&255 else (0x5e4ccc,((cfg.get('variant',0)+(cfg.get('scene',0)&255)*2)*48)&0xffffffff)
    at=(table+offset)&0xffffffff
    if 'selector_byte' in cfg:
        # Deliberately synthetic signed-variant extremes can address original
        # text rather than the fixed data arena. Seed that input byte explicitly
        # and restore the per-image original byte before another case can run.
        if not 0x560000<=at<0x126d000:state['extra_restore'].append((at,bytes(u.mem_read(at,1))))
        byte(u,at,cfg['selector_byte'])
    if fn!=0x4eaac0:
        u.mem_write(0xb350dc,struct.pack('<H',cfg.get('head',0))*51984)
        u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',4))*51984);u.mem_write(0xac2d4c,bytes(51984))
    if fn==0x49e8b0:
        u.mem_write(POOL,bytes(2000*388));u.mem_write(0xbcb9c8,bytes(144*50))
        actor=cfg.get('actor',1);o=(actor*388)&0xffffffff;kind=cfg.get('kind',84)
        byte(u,0x7027e1,cfg.get('special_graphics',0))
        for n,a in enumerate([0xf56e14,0xf56e6c,0xf56e74,0xf56e7c,0xf56e84,0xf56e8c,0xf56e94,0xf56e9c,0xf56ea4]):put(u,a,10000+n*1000)
        for a,v in [(0xbcd602,kind),(0xbcd606,1),(0xbcd630,cfg.get('state',8)),(0xbcd5f9,cfg.get('animation',0)),(0xbcd609,cfg.get('direction',0)),(0xbcd60a,cfg.get('previous_direction',2)),(0xbcd65a,cfg.get('ordinal',3))]:byte(u,(a+o)&0xffffffff,v)
        for a,v in [(0xbcd740,cfg.get('counter',0)),(0xbcd73c,cfg.get('cycle',12)),(0xbcd618,1000+11*228+10)]:put(u,(a+o)&0xffffffff,v)
        put(u,0x7090ec+kind*56,cfg.get('speed',12))
        for a,v in [(0xbcd60c,10),(0xbcd60e,11),(0xbcd61c,cfg.get('target_x',30)),(0xbcd61e,cfg.get('target_y',31)),(0xbcd62e,cfg.get('delay',5)),(0xbcd632,0),(0xbcd656,cfg.get('linked',2)),(0xbcd658,cfg.get('group',1))]:word(u,(a+o)&0xffffffff,v)
        for i in [2,3]:
            off=i*388
            for a,v in [(0xbcd602,cfg.get('linked_kind',84)),(0xbcd630,cfg.get('linked_state',8)),(0xbcd606,cfg.get('linked_status',1))]:byte(u,a+off,v)
            for a,v in [(0xbcd60c,cfg.get('linked_x',30)),(0xbcd60e,cfg.get('linked_y',31))]:word(u,a+off,v)
            put(u,0x7090cc+cfg.get('linked_kind',84)*56,1)
        for a,v in [(0xbcba02,cfg.get('group_x',40)),(0xbcba04,cfg.get('group_y',41)),(0xbcb9fe,cfg.get('group_previous_x',50)),(0xbcba00,cfg.get('group_previous_y',51))]:word(u,(a+144*cfg.get('group',1))&0xffffffff,v)
        put(u,0x5e4cd0,0);put(u,0x5e4cd4,0)
        if cfg.get('actual_target'):
            byte(u,0xbcd602+2*388,83);byte(u,0xbcd630+2*388,8);put(u,0x7090cc+83*56,1)
            word(u,0xb350dc+(1000+11*228+10)*2,2)
    for a,v in cfg.get('bytes',[]):byte(u,a,v)
    for a,v in cfg.get('words',[]):word(u,a,v)
    for a,v in cfg.get('stores',[]):put(u,a,v)
    slot=sym('_imp__timeGetTime@0') if compiled else 0x56f22c
    state['clock_slot']=slot;state['clock_saved']=get(u,slot);put(u,slot,CLOCK)
    def clock(uc,args):
        state['events'].append(('clock',))
        for a,v in cfg.get('clock_mutations',[]):put(uc,a,v)
        return cfg.get('clock',0x12345678)
    state['hooks'][CLOCK]=(0,0,clock)
    for at in state['read_mutations']:
        def anchored_read(uc,access,start,size,value,data,anchor=at):
            if start==anchor:data['read_callback'](uc,access,start,size,value,data)
        state['read_hook_handles'].append(u.hook_add(UC_HOOK_MEM_READ,anchored_read,state,begin=at,end=at))
    if fn==0x4eaac0:return [mode]
    if fn==0x4eab10:return [cfg.get('x',10),cfg.get('y',11),cfg.get('vector',0),cfg.get('target_x',10),cfg.get('target_y',11),mode]
    return []

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

def exercise(fn=0x49e8b0,**cfg):
    global checked,invocations,actual_cases,protocol_cases,synthetic_cases
    expected=None;native_result=None
    if '--quick' in sys.argv or '--trace' in sys.argv:print('start',hex(fn),cfg,flush=True)
    for compiled,u,state in machines:
        for route in ['native'] if not compiled else ['named','canonical','raw']:
            args=setup(compiled,u,state,fn,cfg);state['collect']=True
            result=invoke(compiled,u,state,fn,args,route);state['collect']=False;invocations+=1
            put(u,state['clock_slot'],state['clock_saved'])
            for a,raw in state['extra_restore']:u.mem_write(a,raw)
            assert not state['faults'] and result!='fault',(hex(fn),cfg,route,state['faults'])
            if not compiled:
                native_result=result
                if 'expect_result' in cfg:assert result==(cfg['expect_result']&0xffffffff),(hex(fn),cfg,'unexpected native outcome',hex(result))
            if route=='canonical' and fn==0x4eaac0:assert result&255==native_result&255,(cfg,route,result,native_result)
            elif not(route=='canonical' and fn==0x49e8b0):assert result==native_result,(hex(fn),cfg,route,hex(result),hex(native_result))
            current=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x48000)))
            if expected is None:expected=current
            elif current!=expected:
                trace=[(part,len(expected[part]),len(current[part]),next(((i,a,b) for i,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)) for part in [0,1] if expected[part]!=current[part]]
                memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[2],current[2]),(S,expected[3],current[3])]]
                raise AssertionError((hex(fn),cfg,route,'trace',trace,'memory',memory))
    checked+=1
    if cfg.get('controlled'):protocol_cases+=1
    elif cfg.get('read_mutations') or cfg.get('child_mutations') or cfg.get('child_bytes') or cfg.get('clock_mutations') or cfg.get('synthetic_table'):synthetic_cases+=1
    else:actual_cases+=1
    if checked%25==0:print('progress',checked,hex(fn),flush=True)

def helper_corpus():
    for mode,scene,variant,value in itertools.product([0,1,0x100,0x101,0xffffffff],[0,1,9,255,0x109],[-32768,-2,-1,0,1,32767],[0,1,0x80,0xff]):
        exercise(0x4eaac0,mode=mode,scene=scene,variant=variant,selector_byte=value,synthetic_table=variant==-32768 and not(mode&255))
    # Real providers clear a full51984-cell workspace per call. Four successful
    # zero/one-tile compositions and one terrain-mode rejection establish the
    # integration; the exhaustive vector schedule below controls child results.
    for vector,selector in itertools.product([0,4],[0,1]):
        exercise(0x4eab10,vector=vector,mode=0,selector_byte=selector,terrain=4,expect_result=vector)
    exercise(0x4eab10,mode=1,selector_byte=0,terrain=4,expect_result=0xffffffff)
    for head in [1,0xffff]:exercise(0x4eab10,head=head,selector_byte=0,controlled=[0x5181c0],results={0x5181c0:[1]})
    # This explicit first-line contract reaches the real main-C fallback body.
    # It is a mixed protocol lane, not an ordinary all-child composition.
    exercise(0x4eab10,vector=4,mode=0,selector_byte=1,terrain=4,controlled=[0x519be0],results={0x519be0:[0]})
    for vector in [0,1,15,16,17,-1,0x40000000,0x7fffffff,0x80000000,0xffffffff]:
        exercise(0x4eab10,vector=vector,selector_byte=0,controlled=[0x5181c0],results={0x5181c0:[0]})
    control=[0x519be0,0x517f20,0x5181c0,0x4ec410]
    for selector,first_line,second_line,terrain in itertools.product([0,1],[0,1,0x100],[0,1,0x100],[0,1,0x100,0x101]):
        exercise(0x4eab10,selector_byte=selector,controlled=control,results={0x519be0:[first_line],0x517f20:[second_line],0x5181c0:[first_line],0x4ec410:[terrain]})
    for vector,nth in itertools.product([0,7,15],range(16)):
        exercise(0x4eab10,vector=vector,selector_byte=0,controlled=control,results={0x5181c0:[0]*nth+[1],0x4ec410:[1]})
    for mode in [0xff,0x100,0x101,0x123456ff]:exercise(0x4eab10,mode=mode,selector_byte=0,controlled=control,results={0x5181c0:[1],0x4ec410:[1]})
    exercise(0x4eab10,selector_byte=0,controlled=control,results={0x5181c0:[1],0x4ec410:[1]},child_mutations={(0x4eaac0,1):[(0xbca2d8,2000)]})
    exercise(0x4eab10,selector_byte=1,controlled=control,results={0x519be0:[0],0x517f20:[1],0x4ec410:[1]},child_mutations={(0x519be0,1):[(0xbca2d8,2000)]})
    exercise(0x4eab10,selector_byte=0,controlled=control,results={0x5181c0:[1],0x4ec410:[1]},read_mutations={0x5cdb7c:[(1,[(0x5cdb78,2),(0xbca2d8,2000)])]})
    exercise(0x4eaac0,mode=0,scene=1,variant=0,read_mutations={0x784cb0:[(1,[(0x784b94,2)])]})

def parent_protocol(**cfg):
    cfg.setdefault('controlled',PARENT_CHILDREN)
    exercise(0x49e8b0,**cfg)

def parent_corpus():
    for state in range(256):parent_protocol(state=state)
    for state,terrain,special,counter,direction in itertools.product([4,5,8,9,10,12,13,18,19,20,21,23,24,25,26],[0,4],[0,1],[0,11],[0,8,9,10]):
        parent_protocol(state=state,terrain=terrain,special_graphics=special,counter=counter,direction=direction)
    for state,delay in itertools.product([4,12,18,21,24,25,26],[0,1,2,8,18,23,500,501,0x7fff,0x8000,0xffff]):parent_protocol(state=state,delay=delay,counter=5)
    for state,results in itertools.product([18,23],[[0],[15],[-1],[16],[-1,0],[-1,15],[-1,-1],[0x80000000],[0x7fffffff]]):parent_protocol(state=state,delay=0,results={0x4eab10:results})
    for state,linked,status,linked_state,same in itertools.product([21,25],[0,2],[0,1],[4,8],[False,True]):
        parent_protocol(state=state,counter=11,direction=8,linked=linked,linked_status=status,linked_state=linked_state,target_x=30 if same else 20)
    for history,a,b in itertools.product([0,98,99,100,110,111,0xffffffff],[0,1,0xffffffff,0x12345678],[0,0xffffffff]):parent_protocol(state=23,history=history,first=a,second=b,results={0x4eab10:[-1,-1]})
    for speed,rotation in itertools.product([0,1,0x7fffffff,0x80000000,0xffffffff],[0,7,255,0xffffffff]):parent_protocol(state=19,speed=speed,rotation=rotation)
    for state in [5,4,8,9,12,13,18,19,20,21,23,25,26]:
        for token in PARENT_CHILDREN:
            parent_protocol(state=state,delay=0,counter=11,direction=8,child_mutations={(token,1):[(0xd056f8,3)]},child_bytes={(token,1):[(0xbcd630+3*388,8),(0xbcd609+3*388,10)]},results={0x4eab10:[-1,0]})
    parent_protocol(animation=11);parent_protocol(state=18,delay=0,history=99)
    for state,tx,ty in itertools.product([21,25],[20,30],[20,31]):
        parent_protocol(state=state,counter=0,direction=8,target_x=tx,target_y=ty)
        parent_protocol(state=state,counter=11,direction=8,target_x=tx,target_y=ty)
    for linked,status,linked_state in itertools.product([0,2],[0,1],[4,8]):parent_protocol(state=25,counter=0,direction=8,linked=linked,linked_status=status,linked_state=linked_state,target_x=20,child_bytes={(0x4a5da0,1):[(0xbcd609+388,8)]})
    parent_protocol(state=4,special_graphics=1,delay=18);parent_protocol(state=4,terrain=0,delay=8)
    tile=1000+11*228+10
    for delta in [-228,1,228,-1,-227,-229,229,227]:parent_protocol(stores=[(0xaf598c+(tile+delta)*4,0)])
    parent_protocol(state=8,child_bytes={(0x4ec7c0,1):[(0xbcd630+388,255)]})
    for counter,cycle in itertools.product([0,5,11,12,0x7fffffff,0x80000000,0xffffffff],[0,1,2,12,0x80000000,0xffffffff]):parent_protocol(state=26,counter=counter,cycle=cycle)
    for group in [-1,0,1,49]:parent_protocol(state=23,group=group,results={0x4eab10:[-1,-1]})
    inverse=pow(97,-1,1<<30)
    for field,target in [(0xbcd740,RNGA),(0xbcd630,RNGA),(0xbcd61c,RNGA),(0xbcd61c,HISTORY),(0xbcd740,0xd056f8)]:
        actor=(((target-field)//4)*inverse)%(1<<30);stores=[(HISTORY,0)]
        if target==0xd056f8:stores.append((0xd056f8,actor))
        parent_protocol(actor=actor,state=18,delay=0,stores=stores,results={0x4eab10:[0]})
    for state in [21,25]:parent_protocol(state=state,counter=11,target_x=20,child_mutations={(0x4b3250,1):[(0xbcd60c+2*388,0x00160015)]})
    for state in [18,23]:parent_protocol(state=state,delay=0,child_mutations={(0x4eab10,1):[(0xd056f8,3),(0xbcba02+144,0x00340033)],(0x4b3250,1):[(0xd056f8,2)],(0x4eaec0,1):[(0xd056f8,3)]})
    parent_protocol(state=23,results={0x4eab10:[-1,0]},child_mutations={(0x4eab10,1):[(0xd056f8,3),(RNGA,1),(RNGB,0xffffffff)]},child_bytes={(0x4eab10,1):[(0xbcd658+388,2)]})
    parent_protocol(state=19,child_mutations={(0x4a5da0,1):[(0xd056f8,3)],(0x4b3250,1):[(0xd056f8,2)]},child_bytes={(0x4a5da0,1):[(0xbcd609+3*388,9)]})
    parent_protocol(state=13,child_mutations={(0x4a5da0,1):[(0xd056f8,3)],(0x4eaec0,1):[(0xd056f8,2)]},child_bytes={(0x4eaec0,1):[(0xbcd609+2*388,8)]})
    parent_protocol(state=4,delay=0,child_mutations={(0x4b5050,1):[(0xd056f8,3)],(0x4b1d50,1):[(0xd056f8,2)]})
    parent_protocol(state=5,child_mutations={(0x53b4a0,1):[(0xd056f8,3)],(0x4b17d0,1):[(0xd056f8,2)]})
    parent_protocol(state=26,counter=11,child_mutations={(0x4ec7c0,1):[(0xd056f8,3)]})
    parent_protocol(state=18,delay=0,read_mutations={0xbcd60e+388:[(1,[(0xd3b368,0xabcdef01)])],0xbcd60c+388:[(1,[(0xd3b370,0xabcdef02)])]})
    parent_protocol(state=23,results={0x4eab10:[-1,0]},read_mutations={0xbcd60e+388:[(2,[(0xd3b370,0xabcdef01)])]})
    parent_protocol(state=23,read_mutations={0x5cd978:[(1,[(0xbcba02+144,0x00360035)])]})

def actual_parent_corpus():
    # Fast real compositions cover timer/default/animation, actual command/RNG,
    # live periodic search and the two newly owned vector helper paths.
    for state in [0,12,24,26]:exercise(state=state,delay=5)
    exercise(state=8)
    exercise(state=20,actual_target=True)
    exercise(state=18,delay=5,actual_target=True)
    for state in [18,23]:
        exercise(state=state,delay=0,selector_byte=0,group_x=10,group_y=11,group_previous_x=10,group_previous_y=11)

if __name__=='__main__':
    exercise(0x4eaac0,mode=0,selector_byte=1)
    exercise(0x4eab10,selector_byte=0,controlled=[0x5181c0,0x4ec410],results={0x5181c0:[1],0x4ec410:[1]})
    parent_protocol(state=23,results={0x4eab10:[-1,0]})
    exercise(state=12,delay=5)
    if '--quick' in sys.argv:print('PASS quick animal parent',checked,invocations);sys.exit(0)
    helper_corpus();parent_corpus();actual_parent_corpus()
    missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets}-edges
    print('Compared animal parent:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(switches),'switch targets;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM',flush=True)
    print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],'switches:',[(hex(a),hex(b)) for a,b in sorted(expected_switches-switches)],flush=True)
    audit=json.loads((ROOT/'metadata/actor-animal-parent-caller-audit.json').read_text());assert audit['passed']
    for p,sha in audit['source_hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha,('stale caller audit',p)
    assert not missing and not missing_edges and switches==expected_switches,(missing,missing_edges,expected_switches-switches)
    sources=['CMakeLists.txt','direct_c/verify_actor_animal_parent.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','metadata/actor-animal-parent-native.json','metadata/actor-animal-parent-caller-audit.json',*evidence_paths,*audit['source_hashes']]
    for directory in ['src','runtime','include','generated']:
        sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
    proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=checked,invocations=invocations,
        coverage={hex(a):dict(instructions=len(pcs),executed=len(pcs&hits)) for a,pcs in owned.items()},conditional_edges=len(edges),switch_targets=len(switches),
        unexecuted_instructions=[],unexecuted_edges=[],unexecuted_switch_targets=[],analytically_unreachable_instructions=[],analytically_unreachable_edges=[],
        actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,
        controlled_algorithm_endpoints_by_lane=dict(actual=[],protocol=['Explicit listed direct-owned classifier, vector, line, terrain, lifecycle, movement, periodic and seek child results']),
        controlled_platform_endpoints=['timeGetTime: controlled zero-argument clock result; listed RAM/clock mutation scenarios'],
        return_contract=dict(mode_named_and_raw='full EAX',mode_legacy='AL',vector='full EAX signed index/-1',hippo_named_and_raw='full EAX',hippo_legacy='void, result discarded by observed callback invoker'),
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        limitations=['Three owned native bodies,1036 instructions, including both bounded parent switch tables. Ordinary lanes execute current C child bodies; there is no candidate-DLL or native-child redirection. No host game is launched.','Complete fixed native data/private bytes, ordered stores, meaningful game calls directly issued by owned bodies, supported byte/full returns and normal-return nonvolatile ABI are compared. Nested calls inside nonowned providers are outside this trace tier.','All owned instructions, conditional outcomes and switch targets must execute. No dead instruction/outcome qualification is accepted in this family.','The named hippo result preserves native EAX; its legacy void callback and unused civic-frame scratch/flags are qualified by the separate native invoker/profiler audit. Arbitrary private/caller-stack corruption and OS exception dispatch remain unknown.','Extreme negative selector variants use explicitly seeded input bytes outside the fixed data arena; those per-image bytes are restored before subsequent cases. This is synthetic RAM arithmetic coverage, not a valid gameplay-state claim. Complete gameplay remains unfinished.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/actor-animal-parent-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
