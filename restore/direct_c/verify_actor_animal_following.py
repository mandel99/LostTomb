"""Offline main-PE oracle for crocodile/hyena, cue, weighted state and initializer.

Only scenario definitions are ported from isolated work. All comparisons are
fresh against current main C; ordinary children are never redirected to native.
"""
from pathlib import Path
from functools import cache
from collections import Counter
import argparse,hashlib,itertools,json,re,struct,sys
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'runtime/actor_civic.c').is_file())
sys.path[:0]=[str(HERE),str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from actor_animal_following_cases import enumerate_ported,enumerate_initializer,advance,SIBLING_ARITY,terrain_deltas

NATIVE_SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ENTRY_NAMES={0x49d0f0:'restore_actor_update_crocodile',0x49dc50:'restore_actor_update_hyena',
             0x4b5050:'restore_actor_play_kind_cue',0x4ecd40:'restore_actor_choose_crocodile_state',
             0x4eca70:'restore_actor_enter_animal_state'}
RAW_OWNED_ENTRIES=[0x49d0f0,0x49dc50,0x4ecd40,0x4eca70]
EXTRA_ROOTS={0x49e8b0:'restore_actor_update_hippo'}
DATA_START,DATA_END=0x560000,0x126d000
SP,STOP,S=0x30008000,0x31000000,0x32000000
POOL,STRIDE,HISTORY,RNGA,RNGB=0xbcd5f8,388,0xd3b1c4,0xd3b384,0xd3b380
CLOCK,FRAME,FSP,FLAGS=S+0x49000,0x30000100,0x30006000,0x30000140
SIBLING_CHILDREN=list(SIBLING_ARITY)
ARITY={**SIBLING_ARITY,0x4eca70:2,0x4eb800:9,0x562348:0,0x49d0f0:0,0x49dc50:0,0x49e8b0:0}
COMPILED_NAMES={a:ENTRY_NAMES.get(a,'FUN_%08x'%a) for a in ARITY}
COMPILED_NAMES.update({0x4eca70:'restore_actor_enter_animal_state',0x4eb800:'restore_building_nearest_search',0x4eaec0:'restore_actor_try_periodic_target',**EXTRA_ROOTS})
CASES=[]
PRESERVED_4206_REPR_SHA='f89e26be058c9a0cfab17bd66caeb3027c00dec1a56661e4dbbe6a5be872a06f'

def compiled_routes(case):
    # Global cue scratch ABI remains open; its original frame model is retained.
    return ['named','canonical'] if case['fn']==0x4b5050 else ['named','canonical','raw']

def add(profile,fn,cfg,origin):
    cfg=dict(cfg)
    if origin=='ported' and profile=='sibling':cfg.setdefault('controlled',SIBLING_CHILDREN)
    if profile=='weighted' and origin=='ported':cfg.setdefault('controlled',[0x4eca70])
    if profile=='initializer' and origin=='ported':cfg.setdefault('controlled',[0x4eb800])
    if profile=='cue':
        cfg.setdefault('controlled',([0x562348] if cfg.get('control_rng') else [])+([0x53b4a0] if cfg.get('control_sound') else []))
    CASES.append(dict(id='%s-%04d'%(profile,len(CASES)+1),profile=profile,fn=fn,cfg=cfg,origin=origin))

def ported_cases():
    def sibling(kind=82,**cfg):add('sibling',{82:0x49d0f0,83:0x49dc50}[kind],dict(cfg,kind=kind),'ported')
    enumerate_ported(sibling,lambda **cfg:add('cue',0x4b5050,cfg,'ported'),lambda **cfg:add('weighted',0x4ecd40,cfg,'ported'))
    assert Counter(c['profile'] for c in CASES)==dict(sibling=2162,cue=1205,weighted=798)
    for index,case in enumerate(CASES):
        case['definition_set']=next(label for end,label in [(2076,'sibling-baseline'),(2132,'sibling-additions'),(2162,'sibling-aliases'),(3367,'cue'),(4145,'weighted-baseline'),(4165,'weighted-aliases')] if index<end)

def percentage_seeds():
    seeds={};value=0x1a2b3c4d
    while len(seeds)<100:
        value=(value*1664525+1013904223)&0xffffffff
        seeds.setdefault((advance(value)&0x7fff)%100,value)
    return seeds

def composition_cases():
    def sibling(kind=82,**cfg):add('sibling',{82:0x49d0f0,83:0x49dc50,84:0x49e8b0}[kind],dict(cfg,kind=kind),'composition')
    for kind in [82,83,84]:sibling(kind,state=12,delay=0)
    for kind in [82,83]:sibling(kind,state=12,delay=1)
    # Lifecycle is an explicit protocol here; cue and sound are actual main C.
    sibling(84,state=4,delay=0,controlled=[0x4b1d50])
    for kind,delay in itertools.product([82,83],[2,1]):sibling(kind,state=24,delay=delay)
    for kind in [82,83]:sibling(kind,state=9,terrain=4,speed=2,direction=0,bytes=[(0xbcd758+388,0)])
    sibling(82,state=9,terrain=0,speed=2,direction=0,bytes=[(0xbcd758+388,0)])
    for state,history in itertools.product([8,9],[98,99]):
        sibling(83,state=state,direction=8,speed=1,history=history,terrain=0,bytes=[(0xbcd758+388,0)])
    for kind,counter in itertools.product([82,83],[0,11]):
        sibling(kind,state=18,delay=5,counter=counter,actual_target=True)
    seeds=percentage_seeds()
    vector_seed=next(value for value in seeds.values() if advance(value)&15==0)
    for kind,state in itertools.product([82,83],[18,23]):
        sibling(kind,state=state,delay=0,first=vector_seed,group_x=10,group_y=11,group_previous_x=10,group_previous_y=11)
    for state,samples in [(8,[49,50]),(19,[89,90]),(9,[24,25])]:
        for sample in samples:
            cfg=dict(state=state,a=seeds[sample],b=0x3456789a,actual_child=True,selected=19 if sample<({8:50,19:90}.get(state,25)) else 18)
            add('weighted',0x4ecd40,cfg,'composition')
    for state,samples,direction in [(8,[49,50],0),(19,[89,90],8),(9,[24,25],10)]:
        for sample in samples:
            sibling(82,state=state,first=seeds[sample],direction=direction,speed=1,terrain=0,bytes=[(0xbcd758+388,0)])
    for history,terrain in itertools.product([110,111],[0,4]):
        sibling(82,state=8,history=history,terrain=terrain,first=seeds[49],previous_sample=seeds[49],expect_child_state=19)

def initializer_cases():
    start=len(CASES)
    enumerate_initializer(lambda **cfg:add('initializer',0x4eca70,cfg,'ported'))
    assert len(CASES)-start==740
    for index,case in enumerate(CASES[start:]):case['definition_set']='initializer-baseline' if index<708 else 'initializer-aliases'
    seeds=percentage_seeds();sample=advance(seeds[40])&0x7fff
    for state in [18,0x10000013]:
        add('initializer',0x4eca70,dict(state=state,a=seeds[40],actual_search=True,expected_eax=sample//100,expected_calls=0,
            expected_memory=[(0xbcd62e+388,290,2),(0xbcd73c+388,12,4),(0xbcd630+388,state&255,1)]),'composition')
    add('initializer',0x4eca70,dict(state=19,a=0,terrain=[4]*8,actual_search=True,expected_eax=10,expected_calls=0),'composition')
    add('initializer',0x4eca70,dict(state=19,a=0,actual_search=True,center_flags=4,expected_eax=11,expected_calls=1),'composition')
    add('initializer',0x4eca70,dict(state=19,a=0,actual_search=True,center_flags=0,expected_eax=10,expected_calls=1),'composition')

def plan():
    CASES.clear()
    ported_cases();composition_cases()
    assert len(CASES)==4206 and hashlib.sha256(repr(CASES).encode()).hexdigest()==PRESERVED_4206_REPR_SHA,'Previous4206 input definitions changed'
    initializer_cases()
    counts=Counter(c['profile'] for c in CASES)
    route_counts=Counter(route for case in CASES for route in ['native',*compiled_routes(case)])
    return dict(scenarios=len(CASES),profiles=dict(counts),definition_sets=dict(Counter(c.get('definition_set','composition') for c in CASES)),ported=4905,compositions=len(CASES)-4905,
                invocations=sum(route_counts.values()),route_counts=dict(route_counts),raw_owned_entries=[hex(a) for a in RAW_OWNED_ENTRIES],cue_raw_retained=True,cue_raw_verified=False,
                host_game_launched=False,scope='Scenario enumeration only; no PE execution. Cue has native/named/canonical only; four newly owned functions have raw routes.')

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('target',nargs='?',type=Path,default=ROOT/'build/c-only/Release/PharaohRestoreC.exe')
    ap.add_argument('--plan',action='store_true');ap.add_argument('--quick',action='store_true')
    ap.add_argument('--case-id',action='append',default=[])
    ap.add_argument('--evidence-dir',type=Path,default=ROOT/'metadata')
    ap.add_argument('--output-dir',type=Path,default=HERE if HERE!=ROOT/'direct_c' else ROOT/'metadata')
    args=ap.parse_args()
    args.evidence_dir=args.evidence_dir.resolve();args.output_dir=args.output_dir.resolve()
    inventory=plan()
    if args.plan:print(json.dumps(inventory,indent=2));return
    run(args,inventory)

def run(args,inventory):
    from native_test_pe import PE
    from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
    from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EIP,UC_X86_REG_EFLAGS
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    original=ROOT/'build/smoke-game/Pharaoh.exe'
    assert hashlib.sha256(original.read_bytes()).hexdigest()==NATIVE_SHA
    target=args.target;images=[PE(original),PE(target)]
    target_sha=hashlib.sha256(target.read_bytes()).hexdigest();map_sha=hashlib.sha256(target.with_suffix('.map').read_bytes()).hexdigest()
    symbols=target.with_suffix('.map').read_text()
    @cache
    def sym(name):
        found=re.findall(r'\s_'+re.escape(name)+r'\s+([0-9a-fA-F]{8})\s',symbols)
        assert len(found)==1,(name,found)
        return int(found[0],16)
    def put(u,a,v):u.mem_write(a&0xffffffff,struct.pack('<I',v&0xffffffff))
    def word(u,a,v):u.mem_write(a&0xffffffff,struct.pack('<H',v&65535))
    def byte(u,a,v):u.mem_write(a&0xffffffff,bytes([v&255]))
    def get(u,a):return struct.unpack('<I',u.mem_read(a&0xffffffff,4))[0]
    def change(u,triples):
        for at,size,value in triples:u.mem_write(at&0xffffffff,(value&((1<<(size*8))-1)).to_bytes(size,'little'))
    registers=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
    saved=[0x12345678,0x23456789,0x3456789a,0x456789ab]
    seed=bytes(images[0].data[DATA_START-images[0].base:DATA_END-images[0].base])
    catalogue_path=args.evidence_dir/'actor-animal-following-native.json'
    audit_path=args.evidence_dir/'actor-animal-following-caller-audit.json'
    catalogue=json.loads(catalogue_path.read_text());audit=json.loads(audit_path.read_text())
    assert audit['passed'] and catalogue['native_sha256']==NATIVE_SHA
    assert audit.get('production_ready'), 'Wait for the final ordinary-C/current-batch cue and four-function raw scope audit.'
    assert audit['raw_scope']['entries']==[hex(a) for a in RAW_OWNED_ENTRIES]
    assert audit['cue_raw_retained'] is True and audit['cue_raw_verified'] is False
    records=[];evidence_paths=[]
    for name in catalogue['evidence_files']:
        path=args.evidence_dir/name;evidence_paths.append(path);d=json.loads(path.read_text());records+=d.get('records',[d])
    evidence={e['evidence_id']:e for e in records};owned={};branches={};hits=set();edges=set();switches=set()
    assert {int(f['address'],16):f['name'] for f in catalogue['functions']}==ENTRY_NAMES
    for f in catalogue['functions']:
        at=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
        assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==NATIVE_SHA
        z=ev['normalized_result'];assert z['complete'] and z['bytes_hex']==f['bytes_hex']
        assert raw==bytes(images[0].data[at-images[0].base:at-images[0].base+len(raw)])
        code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,at));owned[at]={i.address for i in code}
        assert len(code)==f['instruction_count'] and sum(i.size for i in code)==len(raw)
        for i in code:
            if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
    points=set.union(*owned.values());owner={pc:at for at,pcs in owned.items() for pc in pcs};assert len(points)==1922
    dead={int(a,16) for a in audit['analytically_unreachable_instructions']}
    dead_edges={(int(a,16),int(b,16)) for a,b in audit['analytically_unreachable_edges']}
    assert len(dead)==15 and len(dead_edges)==6 and dead<=points
    assert sum(map(len,branches.values()))==302
    assert tuple(struct.unpack_from('<8i',images[0].data,0x5e4e80-images[0].base))==terrain_deltas
    expected_switches={(int(s['site'],16),int(t,16)) for s in catalogue['switches'] for t in s['targets']}
    assert len(expected_switches)==53
    switch_sites={a for a,b in expected_switches}
    # Existing hippo compositions need its caller sites for observation only;
    # none of this already-owned body receives new coverage credit.
    previous_catalogue_path=ROOT/'metadata/actor-animal-parent-native.json'
    previous_catalogue=json.loads(previous_catalogue_path.read_text())
    assert previous_catalogue['native_sha256']==NATIVE_SHA
    hippo=next(f for f in previous_catalogue['functions'] if int(f['address'],16)==0x49e8b0)
    hippo_raw=bytes.fromhex(hippo['bytes_hex'])
    assert hippo_raw==bytes(images[0].data[0x49e8b0-images[0].base:0x49e8b0-images[0].base+len(hippo_raw)])
    hippo_points={i.address for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(hippo_raw,0x49e8b0)}
    function_starts=sorted({int(m.group(1),16) for line in symbols.splitlines() if (m:=re.match(r'\s*[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s+f\s',line))})
    def interval(name):
        at=sym(name);return at,next(x for x in function_starts if x>at)
    owned_c_intervals=[interval(n) for n in [*ENTRY_NAMES.values(),*EXTRA_ROOTS.values()]]
    alias_intervals={at:interval('FUN_%08x'%at) for at in ENTRY_NAMES}
    source_paths=[Path(__file__),HERE/'actor_animal_following_cases.py',ROOT/'CMakeLists.txt',ROOT/'direct_c/native_test_pe.py',catalogue_path,audit_path,previous_catalogue_path,*evidence_paths]
    source_paths += [ROOT/name for name in audit['source_hashes']]
    for directory in ['src','runtime','include','generated']:
        source_paths += [p for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
    source_hashes={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in set(source_paths)}
    for name,sha in audit['source_hashes'].items():assert source_hashes[name]==sha,('stale audit',name)
    def owned_c_return(pc):return any(lo<=pc<hi for lo,hi in owned_c_intervals)
    def inside_alias(pc,token):
        lo,hi=alias_intervals[token];return lo<=pc<hi
    machines=[]
    for compiled,pe in enumerate(images):
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
        u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
        state=dict(compiled=compiled,collect=False,entries={},handles=[],hooks={})
        def clock(uc,pc,size,d):
            if not d['collect']:return
            assert pc==CLOCK;sp=uc.reg_read(UC_X86_REG_ESP)
            d['events'].append(('clock',))
            for at,value in d['cfg'].get('clock_mutations',[]):put(uc,at,value)
            uc.reg_write(UC_X86_REG_EAX,d['cfg'].get('clock',0x12345678)&0xffffffff)
            uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
        u.hook_add(UC_HOOK_CODE,clock,state,begin=CLOCK,end=CLOCK)
        def mutations(uc,d,key,at,n):
            for nth,changes in d['cfg'].get(key,{}).get(at,[]):
                if n!=nth:continue
                # Weighted triples are (address,value,size), siblings use DWORD pairs.
                d['events'].append((key,at,n,tuple(tuple(x) for x in changes)))
                for item in changes:
                    dst,value,*width=item;size=width[0] if width else 4
                    if isinstance(dst,str):dst=d['output_ptrs'][dst]
                    change(uc,[(dst,size,value)])
        def write(uc,access,at,size,value,d):
            if not d['collect'] or not(DATA_START<=at<DATA_END or S<=at<S+0x48000):return
            d['writes'].append((at,size,value&((1<<(size*8))-1)))
            if d['profile'] in ['weighted','initializer']:
                n=d['write_counts'].get(at,0)+1;d['write_counts'][at]=n;mutations(uc,d,'write_mutations',at,n)
        u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=DATA_START,end=DATA_END-1)
        u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S,end=S+0x47fff)
        def read(uc,access,at,size,value,d):
            if not d['collect']:return
            if d['profile']=='weighted' and DATA_START<=at<DATA_END and not d['counts'].get(0x4eca70):d['reads'].append((at,size))
            if d['profile']=='initializer' and DATA_START<=at<DATA_END and not d['read_suppression']:d['reads'].append((at,size))
            if at in d['cfg'].get('read_mutations',{}):
                n=d['read_counts'].get(at,0)+1;d['read_counts'][at]=n;mutations(uc,d,'read_mutations',at,n)
            if d['profile']=='cue' and at==0x604950 and d['cfg'].get('seed_read_mutations'):
                d['seed_reads']+=1
                if d['seed_reads']==1:
                    changes=d['cfg']['seed_read_mutations'];d['events'].append(('seed_read_mutation',tuple(changes)));change(uc,changes)
        state['read_callback']=read
        def invalid(uc,access,at,size,value,d):d['faults'].append((access,at,size));return False
        u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
        if not compiled:
            def visit(uc,pc,size,d):
                if not d['collect'] or owner.get(pc) in d['controlled']:return
                hits.add(pc)
                if d['last'] in branches:edges.add((d['last'],pc))
                if d['last'] in switch_sites:switches.add((d['last'],pc))
                d['last']=pc
            for pcs in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(pcs),end=max(pcs))
        def child(uc,pc,size,d):
            if not d['collect']:return
            token,is_alias=d['entries'][pc]
            if token==d['fn']:return
            sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
            if d['compiled']:
                previous=d['handoff'].get(token)
                if not is_alias and previous and (previous==(sp,ret) or inside_alias(ret,token)):
                    del d['handoff'][token];return
                if not owned_c_return(ret):return
                if is_alias:d['handoff'][token]=(sp,ret)
            elif ret not in points and not(d['fn']==0x49e8b0 and ret in hippo_points):return
            arguments=tuple(get(uc,sp+4+i*4) for i in range(ARITY[token]))
            meaningful=arguments
            if token==0x4eca70:d['initializer_arguments']=arguments
            if token==0x4eb800:
                first,second=arguments[5:7]
                assert first!=second and not set(range(first,first+4))&set(range(second,second+4)),('search output alias',arguments)
                initial=(get(uc,first),get(uc,second))
                assert initial==d['initializer_arguments'],('search output seeds',initial,d['initializer_arguments'])
                d['output_ptrs']={'out_x':first,'out_y':second}
                meaningful=arguments[:5]+('out_x','out_y')+arguments[7:]+initial
                if d['profile']=='initializer':
                    # Observe owned reads after the provider returns, while
                    # excluding reads performed inside the unchanged provider.
                    d['read_suppression']=True
                    def resume_reads(machine,address,width,data,expected_sp=sp+4):
                        if data['collect'] and machine.reg_read(UC_X86_REG_ESP)==expected_sp:data['read_suppression']=False
                    d['handles'].append(uc.hook_add(UC_HOOK_CODE,resume_reads,d,begin=ret,end=ret))
            n=d['counts'].get(token,0)+1;d['counts'][token]=n;d['events'].append((token,meaningful))
            cfg=d['cfg']
            if d['profile']=='cue' and token in [0x562348,0x53b4a0]:
                changes=cfg.get('rng_mutations' if token==0x562348 else 'sound_mutations',[])
                if changes:d['events'].append(('cue_child_mutations',token,tuple(changes)));change(uc,changes)
            if d['profile']=='initializer' and token==0x4eb800:
                if token in d['controlled']:
                    for at,value,width in cfg.get('outputs',[('out_x',30,4),('out_y',31,4)]):
                        change(uc,[(d['output_ptrs'].get(at,at),width,value)])
                for at,value,width in cfg.get('child_mutations',[]):change(uc,[(d['output_ptrs'].get(at,at),width,value)])
            elif d['profile']=='weighted' and token==0x4eca70:
                for at,value,width in cfg.get('child_mutations',[]):change(uc,[(at,width,value)])
            else:
                for at,value in cfg.get('child_mutations',{}).get((token,n),[]):put(uc,at,value)
                for at,value in cfg.get('child_bytes',{}).get((token,n),[]):byte(uc,at,value)
            if token not in d['controlled']:return
            d['handoff'].pop(token,None)
            values=cfg.get('results',{}).get(token,[0]);value=values[min(n-1,len(values)-1)]
            if d['profile']=='weighted' and token==0x4eca70:value=cfg.get('result',0)
            if d['profile']=='initializer' and token==0x4eb800:value=cfg.get('result',0)
            if d['profile']=='cue':value=cfg.get('random_result',0) if token==0x562348 else cfg.get('sound_result',0x89abcdef)
            uc.reg_write(UC_X86_REG_EAX,value&0xffffffff)
            if cfg.get('clobber',True):
                uc.reg_write(UC_X86_REG_ECX,0x76543210 if d['profile']=='cue' else 0x6a6b6c6d)
                uc.reg_write(UC_X86_REG_EDX,0xfedcba98 if d['profile']=='cue' else 0x7a7b7c7d)
            if d['profile']=='cue':uc.reg_write(UC_X86_REG_EFLAGS,cfg.get('child_flags',0x246))
            uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
        for token in ARITY:
            entries=[(token,False)] if not compiled else [(sym(COMPILED_NAMES[token]),False)]
            if compiled and token in ENTRY_NAMES:
                alias=sym('FUN_%08x'%token)
                if alias!=entries[0][0]:entries.append((alias,True))
            for pc,is_alias in entries:
                assert pc not in state['entries'],(token,hex(pc));state['entries'][pc]=(token,is_alias)
                u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)
        machines.append((compiled,u,state))

    def restore_seed(u):
        current=bytes(u.mem_read(DATA_START,len(seed)))
        for off in range(0,len(seed),4096):
            page=seed[off:off+4096]
            if current[off:off+4096]!=page:u.mem_write(DATA_START+off,page)

    def setup(compiled,u,d,case):
        for handle in d['handles']:u.hook_del(handle)
        cfg=case['cfg'];d.update(collect=False,fn=case['fn'],profile=case['profile'],cfg=cfg,handles=[],writes=[],reads=[],events=[],counts={},read_counts={},write_counts={},seed_reads=0,faults=[],handoff={},last=None,controlled=set(cfg.get('controlled',[])),output_ptrs={},read_suppression=False,initializer_arguments=None)
        assert case['fn'] not in d['controlled']
        restore_seed(u);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
        put(u,0xf55b88,cfg.get('sound_gate',0));byte(u,0xe38e63,cfg.get('sound_option',1))
        actor=cfg.get('actor',1);offset=(actor*388)&0xffffffff
        if case['profile']=='cue':
            put(u,0x604950,cfg.get('seed',1));byte(u,0xbcd602+offset,cfg.get('kind',26));change(u,cfg.get('initial_changes',[]))
        elif case['profile']=='weighted':
            if cfg.get('actual_child'):
                u.mem_write(POOL,bytes(2000*388));u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',4))*51984)
                for at,value in [(0xbca2d8,1000),(0xbca2d0,228),(0xbca2d4,228),(0xbcd618+offset,1000+11*228+10)]:put(u,at,value)
                for at,value in [(0xbcd60c+offset,10),(0xbcd60e+offset,11)]:word(u,at,value)
            for at,value in [(HISTORY,cfg.get('history',0)),(0xd3b374,cfg.get('previous',0x13572468)),(RNGA,cfg.get('a',0x12345678)),(RNGB,cfg.get('b',0x3456789a)),(0xd056f8,0x99)]:put(u,at,value)
            for n,value in enumerate(cfg.get('table',[19,18,41])):put(u,0x5e4ee0+n*4,value)
            byte(u,0xbcd630+offset,cfg.get('state',8))
            for at,value,width in cfg.get('initial',[]):change(u,[(at,width,value)])
        elif case['profile']=='initializer':
            actual=cfg.get('actual_search',False)
            if actual:
                u.mem_write(POOL,bytes(2000*388));u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xb350dc,bytes(51984*2))
                for at,value in [(0xbca2d8,1000),(0xbca2d0,228),(0xbca2d4,228)]:put(u,at,value)
            for at,value in [(HISTORY,cfg.get('history',0)),(0xd3b374,cfg.get('previous',0x13572468)),(RNGA,cfg.get('a',0x12345678)),(RNGB,cfg.get('b',0x3456789a)),(0xd056f8,99)]:put(u,at,value)
            tile=cfg.get('tile',1000+11*228+10 if actual else 10000)
            for at,value,width in [(0xbcd630+offset,8,1),(0xbcd60c+offset,cfg.get('x',10),2),(0xbcd60e+offset,cfg.get('y',11),2),(0xbcd618+offset,tile,4),(0xbcd73c+offset,0x11223344,4),(0xbcd740+offset,0x55667788,4)]:change(u,[(at,width,value)])
            if actual:put(u,0xaf598c+tile*4,cfg.get('center_flags',0))
            for delta,value in zip(terrain_deltas,cfg.get('terrain',[0]*8)):put(u,0xaf598c+((tile+delta)&0xffffffff)*4,value)
            for at,value,width in cfg.get('initial',[]):change(u,[(at,width,value)])
            d['initializer_arguments']=(actor&0xffffffff,cfg.get('state',19)&0xffffffff)
        else:
            u.mem_write(POOL,bytes(2000*388));u.mem_write(0xbcb9c8,bytes(144*50))
            u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',4))*51984);u.mem_write(0xb350dc,bytes(51984*2));u.mem_write(0xac2d4c,bytes(51984))
            for at,value in [(0xd056f8,actor),(0xeaac28,1),(0x7027e1,0),(0xbca2e0,cfg.get('rotation',0)),(HISTORY,cfg.get('history',0)),(0xd3b374,cfg.get('previous_sample',0x13572468)),(RNGA,cfg.get('first',0x12345678)),(RNGB,cfg.get('second',0x3456789a)),(0xbca2d8,1000),(0xbca2d0,228),(0xbca2d4,228),(0x784cb0,0),(0x5e4cd0,0),(0x5e4cd4,0),(0x604950,0xabcdef01)]:put(u,at,value)
            word(u,0x784b94,0);byte(u,0x7027e1,cfg.get('special_graphics',0))
            for n,at in enumerate([0xf56e14,0xf55da4,0xf55dac,0xf55db4,0xf55dbc,0xf55dc4,0xf561f4,0xf561fc,0xf56204,0xf5620c,0xf56214]):put(u,at,10000+n*1000)
            kind=cfg.get('kind',82)
            for at,value in [(0xbcd602,kind),(0xbcd606,1),(0xbcd630,cfg.get('state',8)),(0xbcd5f9,cfg.get('animation',0)),(0xbcd609,cfg.get('direction',0)),(0xbcd60a,cfg.get('previous_direction',2)),(0xbcd65a,cfg.get('ordinal',3))]:byte(u,at+offset,value)
            for at,value in [(0xbcd740,cfg.get('counter',0)),(0xbcd73c,cfg.get('cycle',12)),(0xbcd618,1000+11*228+10)]:put(u,at+offset,value)
            put(u,0x7090ec+kind*56,cfg.get('speed',12))
            for at,value in [(0xbcd60c,10),(0xbcd60e,11),(0xbcd61c,cfg.get('target_x',30)),(0xbcd61e,cfg.get('target_y',31)),(0xbcd62e,cfg.get('delay',5)),(0xbcd632,0),(0xbcd656,cfg.get('linked',2)),(0xbcd658,cfg.get('group',1))]:word(u,at+offset,value)
            for i in [2,3]:
                off=i*388
                for at,value in [(0xbcd630,cfg.get('linked_state',8)),(0xbcd606,cfg.get('linked_status',1))]:byte(u,at+off,value)
                for at,value in [(0xbcd60c,cfg.get('linked_x',30)),(0xbcd60e,cfg.get('linked_y',31))]:word(u,at+off,value)
            for at,value in [(0xbcba02,cfg.get('group_x',40)),(0xbcba04,cfg.get('group_y',41)),(0xbcb9fe,cfg.get('group_previous_x',50)),(0xbcba00,cfg.get('group_previous_y',51))]:word(u,at+144*cfg.get('group',1),value)
            if cfg.get('actual_target'):
                linked_kind=83 if kind!=83 else 84
                byte(u,0xbcd602+2*388,linked_kind);put(u,0x7090cc+linked_kind*56,1)
                word(u,0xbcd60c+2*388,10);word(u,0xbcd60e+2*388,11)
                word(u,0xb350dc+(1000+11*228+10)*2,2)
            for at,value in cfg.get('bytes',[]):byte(u,at,value)
            for at,value in cfg.get('words',[]):word(u,at,value)
            for at,value in cfg.get('stores',[]):put(u,at,value)
        if case['profile'] in ['weighted','initializer']:d['handles'].append(u.hook_add(UC_HOOK_MEM_READ,d['read_callback'],d,begin=DATA_START,end=DATA_END-1))
        else:
            anchors=set(cfg.get('read_mutations',{}))
            if cfg.get('seed_read_mutations'):anchors.add(0x604950)
            for at in anchors:
                def anchored_read(uc,access,start,size,value,data,anchor=at):
                    if start==anchor:data['read_callback'](uc,access,start,size,value,data)
                d['handles'].append(u.hook_add(UC_HOOK_MEM_READ,anchored_read,d,begin=at,end=at))
        slot=sym('_imp__timeGetTime@0') if compiled else 0x56f22c
        d['clock_slot']=slot;d['clock_saved']=get(u,slot);put(u,slot,CLOCK)
        if case['fn']==0x4eca70:return [actor,cfg.get('state',19)]
        return [] if case['fn'] in [0x49d0f0,0x49dc50,0x49e8b0] else [actor]

    def invoke(compiled,u,d,case,arguments,route):
        for reg,value in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
        u.reg_write(UC_X86_REG_EFLAGS,case['cfg'].get('flags',2));put(u,SP,STOP)
        pc=case['fn'] if not compiled else sym({**ENTRY_NAMES,**EXTRA_ROOTS}[case['fn']] if route=='named' else 'FUN_%08x'%case['fn'])
        actual_args=arguments
        if route=='raw':
            assert case['fn']!=0x4b5050,'Original raw cue model is retained and outside this verifier scope.'
            for i,value in enumerate([0xa5b6c7d8,0x22222222,0x33333333,saved[0],FSP,*saved[1:]]):put(u,FRAME+i*4,value)
            put(u,FLAGS,0x202);put(u,FSP,STOP)
            for i,value in enumerate(arguments):put(u,FSP+4+i*4,value)
            actual_args=[case['fn'],FRAME,FLAGS];pc=sym('restore_actor_civic_raw')
        for i,value in enumerate(actual_args):put(u,SP+4+i*4,value)
        try:u.emu_start(pc,STOP,count=50000000)
        except UcError as exc:raise AssertionError((case['id'],route,'emulator',str(exc),d['faults'])) from exc
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4,(case['id'],route,'exit')
        assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved,(case['id'],route,'nonvolatile ABI')
        if route=='raw':
            assert [get(u,FRAME+i*4) for i in range(1,8)]==[0x22222222,0x33333333,saved[0],FSP+4,*saved[1:]]
            assert get(u,FLAGS)==0x202
            return get(u,FRAME)
        return u.reg_read(UC_X86_REG_EAX)

    checked=invocations=0;lanes=Counter();profiles=Counter();ram_features=Counter();route_counts=Counter()
    selected=CASES
    if args.case_id:selected=[c for c in CASES if c['id'] in args.case_id];assert len(selected)==len(set(args.case_id))
    elif args.quick:
        selected=[next(c for c in CASES if c['profile']==p and c['origin']=='ported') for p in ['sibling','cue','weighted','initializer']]
        selected += [next(c for c in CASES if c['origin']=='composition' and c['profile']=='sibling' and c['cfg'].get('kind')==83 and c['cfg'].get('state')==24),next(c for c in CASES if c['origin']=='composition' and c['profile']=='weighted')]
        selected += [next(c for c in CASES if c['origin']=='composition' and c['profile']=='sibling' and c['cfg'].get('kind')==82 and c['cfg'].get('state')==state) for state in [12,23]]
        selected += [c for c in CASES if c['origin']=='composition' and c['profile']=='initializer']
    for case in selected:
        expected=None;native_result=None;cfg=case['cfg']
        for compiled,u,d in machines:
            for route in ['native'] if not compiled else compiled_routes(case):
                arguments=setup(compiled,u,d,case);d['collect']=True
                result=invoke(compiled,u,d,case,arguments,route);d['collect']=False;invocations+=1;route_counts[route]+=1
                put(u,d['clock_slot'],d['clock_saved']);assert not d['faults'],(case['id'],route,d['faults'])
                if not compiled:native_result=result
                meaningful=case['fn']!=0x4b5050 and not(route=='canonical' and case['fn'] in [0x49d0f0,0x49dc50,0x49e8b0,0x4ecd40,0x4eca70])
                if meaningful:assert result==native_result,(case['id'],route,'EAX',hex(result),hex(native_result),cfg)
                if case['profile']=='weighted':
                    calls=[event for event in d['events'] if event[0]==0x4eca70];assert len(calls)==1 and calls[0][1][0]==(cfg.get('actor',1)&0xffffffff)
                    if cfg.get('selected') is not None:assert calls[0][1][1]==cfg['selected'],(case['id'],'selected state',calls)
                    for at,value in cfg.get('expected_memory',[]):assert get(u,at)==value,(case['id'],'RAM witness',hex(at),hex(get(u,at)),hex(value))
                    if 0x4eca70 in d['controlled'] and meaningful:assert result==(cfg.get('result',0)&0xffffffff)
                if case['profile']=='cue':assert d['counts'].get(0x53b4a0)==1
                if case['profile']=='initializer':
                    if cfg.get('expected_eax') is not None and meaningful:assert result==cfg['expected_eax'],(case['id'],'initializer EAX witness',hex(result),hex(cfg['expected_eax']))
                    if cfg.get('expected_calls') is not None:assert d['counts'].get(0x4eb800,0)==cfg['expected_calls'],(case['id'],'search count',d['counts'])
                    for at,value,width in cfg.get('expected_memory',[]):
                        observed=int.from_bytes(u.mem_read(at&0xffffffff,width),'little')
                        assert observed==value,(case['id'],'initializer RAM witness',hex(at),hex(observed),hex(value))
                if cfg.get('expect_child_state') is not None:
                    calls=[event for event in d['events'] if event[0]==0x4eca70]
                    assert len(calls)==1 and calls[0][1][1]==cfg['expect_child_state'],(case['id'],'nested child state',calls)
                if cfg.get('seed_read_mutations'):assert d['seed_reads']>=1
                current=(tuple(d['writes']),tuple(d['events']),tuple(d['reads']),bytes(u.mem_read(DATA_START,len(seed))),bytes(u.mem_read(S,0x48000)))
                if expected is None:expected=current
                elif current!=expected:
                    trace=[(part,len(expected[part]),len(current[part]),next(((i,a,b) for i,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)) for part in [0,1,2] if expected[part]!=current[part]]
                    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(DATA_START,expected[3],current[3]),(S,expected[4],current[4])]]
                    raise AssertionError((case['id'],route,'difference',trace,memory,cfg))
        checked+=1;profiles[case['profile']]+=1
        lane='protocol' if cfg.get('controlled') else 'RAM' if any(cfg.get(k) for k in ['read_mutations','write_mutations','child_mutations','child_bytes','clock_mutations','seed_read_mutations','rng_mutations','sound_mutations','synthetic']) else 'actual'
        lanes[lane]+=1
        if case.get('definition_set','').endswith('aliases'):ram_features[case['definition_set']]+=1
        elif any(cfg.get(k) for k in ['read_mutations','write_mutations','child_mutations','child_bytes','clock_mutations','seed_read_mutations','rng_mutations','sound_mutations','synthetic']):ram_features['other_listed_mutations']+=1
        if checked%25==0 or args.quick or args.case_id:print('progress',checked,case['id'],lane,flush=True)
    if args.quick or args.case_id:print('PASS partial animal following',checked,invocations);return
    assert checked==inventory['scenarios'] and invocations==inventory['invocations'] and dict(route_counts)==inventory['route_counts']
    missing=points-hits;missing_edges={(a,b) for a,values in branches.items() for b in values}-edges
    assert missing==dead and missing_edges==dead_edges and switches==expected_switches,(sorted(missing-dead),missing_edges-dead_edges,expected_switches-switches)
    for name,sha in source_hashes.items():assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==sha,('source changed during oracle',name)
    assert hashlib.sha256(target.read_bytes()).hexdigest()==target_sha and hashlib.sha256(target.with_suffix('.map').read_bytes()).hexdigest()==map_sha,'Main artifacts changed during oracle'
    proof=dict(passed=True,native_sha256=NATIVE_SHA,compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,map_sha256=map_sha,
        checked=checked,invocations=invocations,scenario_inventory=inventory,profiles=dict(profiles),route_counts=dict(route_counts),actual_scenarios=lanes['actual'],protocol_scenarios=lanes['protocol'],synthetic_memory_scenarios=lanes['RAM'],ram_features_including_protocol=dict(ram_features),
        coverage={hex(at):dict(instructions=len(pcs),executed=len(pcs&hits),analytically_unreachable=len(pcs&dead)) for at,pcs in owned.items()},conditional_edges=len(edges),switch_targets=len(switches),
        unexecuted_instructions=[],unexecuted_edges=[],unexecuted_switch_targets=[],analytically_unreachable_instructions=[hex(a) for a in sorted(dead)],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(dead_edges)],
        source_hashes=source_hashes,scenario_definitions_sha256=hashlib.sha256(repr(CASES).encode()).hexdigest(),host_game_launched=False,
        raw_scope=dict(entries=[hex(a) for a in RAW_OWNED_ENTRIES],qualification='Four newly converted raw entries only; existing hippo raw appears solely as a prior-family composition.'),cue_raw_retained=True,cue_raw_verified=False,
        return_contract=dict(crocodile_hyena_named_raw='full EAX, including hyena state24 literals20/8',weighted_named_raw='full forwarded initializer EAX',initializer_named_raw='full EAX: non19 quotient; neighbor WORD Y; failed search fresh WORD X; search success highWORD child EAX plus WORD outY',legacy_callbacks_weighted_initializer='void, discarded return',cue='void named/canonical; four current-batch plus one existing C caller at49C50C; original legacy raw model retained and not verified here'),
        controlled_algorithm_endpoints_by_lane=dict(actual=[],protocol='Per-case explicit controlled token lists; RAM/child/clock mutations are recorded separately'),controlled_platform_endpoints=['timeGetTime fixed zero-argument clock fixture'],
        limitations=['Five ordinary-C native bodies1922 instructions. Fifteen dead instructions and six impossible outcomes require the separate authenticated CFG/caller audit. Four newly converted functions have raw proof; cue has named/canonical proof only. Its complete original raw frame model remains because global scratch ABI is open through the CRT new-handler.','Fixed original data/private scratch bytes, ordered stores, meaningful direct-owned calls, weighted pre-child reads, initializer reads excluding the unchanged nearest provider, supported results and normal stack/nonvolatile ABI are compared. Search output pointers are normalized by role with initial DWORD contents and disjointness checked. Compiler-private stack bytes and physical OS/audio behavior are outside the comparison.','Ordinary lanes execute current main-C children; no candidate/native-child redirect exists. The720-byte201-instruction initializer is ordinary owned C, with genuine compositions using the existing9argument nearest-search provider. Full game equivalence remains unfinished.'])
    args.output_dir.mkdir(parents=True,exist_ok=True)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (args.output_dir/f'actor-animal-following-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS animal following',checked,'cases;',invocations,'invocations;',len(points&hits),'executed +',len(dead),'dead;',len(edges),'outcomes;',len(switches),'switches;',dict(lanes),flush=True)

if __name__=='__main__':main()
