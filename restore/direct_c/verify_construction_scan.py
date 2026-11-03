"""Offline comparison of four construction leaves with the shipped x86 code.

Actual native instructions versus compiled C, full logical-return EAX, ordered
game/output writes, all original data, explicit output buffers and saved ABI.
Native argument slots used as loop scratch and volatile registers/flags are
outside the existing typed facade's contract. No game/platform calls occur.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import json, random

catalogue=json.loads((ROOT/'metadata/construction-scan-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x43f250:'restore_construction_road_access',0x43f3a0:'restore_construction_footprint_mask',
       0x441040:'restore_construction_empty_slots',0x4413b0:'restore_construction_occupancy'}
owned={};branches={};hits=set();edges=set();jumps=set();last=[None]
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    ins=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in ins}==owned[a]
    for i in ins:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for table in catalogue['tables']:
    a=int(table['address'],16);raw=bytes.fromhex(table['bytes_hex'])
    assert table['complete'] and raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values());machines=[];checked=invocations=compositions=0
COUNT,OUT=S+0x1000,S+0x2000
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(writes=[],collect=False);machines.append((c,u,state))
    def write(uc,access,at,size,value,data):
        if data['collect']:data['writes'].append((at,size,value&((1<<(8*size))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=COUNT,end=OUT+2047)
    if not c:
        def code(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0]==0x43f3ac:jumps.add(pc)
            last[0]=pc
        for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))

def fixture(u,fn,v):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000))
    u.mem_write(0xaf598c,bytes(228*228*4));u.mem_write(0x775614,b'\xa5'*100)
    put(u,COUNT,0xdeadbeef);u.mem_write(OUT,b'\x6b'*2048)
    if fn==0x43f3a0:return list(v['args'])
    if fn==0x43f250:
        x,y,ox,oy,n=v['args'];tile=v.get('tile',80*228+90);put(u,0xbca2d8,tile-y*228-x)
        put(u,0xaf598c+tile*4,0xa5000040 if v.get('center',True) else 0xa5000020)
        distances=[y-oy,ox-x+n-1,oy-y+n-1,x-ox]
        for d,(step,distance) in enumerate(zip([-228,1,228,-1],distances)):
            for i in range(1,max(1,distance)+1):
                road=bool(v.get('mask',15)&(1<<d))
                if v.get('hole')==d and i==max(1,distance)-1:road=False
                put(u,0xaf598c+(tile+step*i)*4,0xa5000040 if road else 0x40000020)
        return list(v['args'])
    if fn==0x4413b0:
        x,y,kind=v['args'];tile=10000;put(u,0xbca2d8,tile-y*228-x)
        rng=random.Random(v.get('pattern',0))
        for row in range(5):
            for col in range(5):put(u,0xaf598c+(tile+row*228+col)*4,rng.choice([0,0x40,0x40000000,0xffffffbf,0xffffffff]))
        return [x,y,kind]
    rng=random.Random(v.get('pattern',0))
    for i in range(25):put(u,0x775614+i*4,0 if v.get('empty') else rng.choice([0,0,0,1,0x100,0x80000000]))
    return list(v['args'])+[v.get('count',COUNT),v.get('out',OUT)]

def invoke(c,u,fn,args,route):
    global invocations
    invocations+=1;entry=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
    return run(0,u,entry,args,budget=1000000) if fn!=0x4413b0 else (run(0,u,entry,args,budget=1000000),None)[1]

def exercise(fn,**v):
    global checked
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False;args=fixture(u,fn,v);state.update(writes=[],collect=True);last[0]=None
            try:value=invoke(c,u,fn,args,route)
            except Exception as e:raise AssertionError(('execute',hex(fn),v,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;result=(value,tuple(state['writes']),snapshot(u),bytes(u.mem_read(COUNT,OUT+2048-COUNT)))
            if expected is None:expected=result
            elif result!=expected:
                change=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                stores=next(((i,a,b) for i,(a,b) in enumerate(zip(expected[1],result[1])) if a!=b),None)
                raise AssertionError((hex(fn),v,route,'EAX',expected[0],value,'memory',change,'store',stores,'store counts',len(expected[1]),len(result[1]),'outputs',expected[3]==result[3]))
    checked+=1

def composition(kind,rows,columns):
    global compositions
    expected=None
    for c,u,state in machines:
        state['collect']=False;args=fixture(u,0x4413b0,dict(args=(90,80,kind),pattern=kind));state.update(writes=[],collect=True);last[0]=None
        invoke(c,u,0x4413b0,args,'legacy')
        extent={31:2,30:3,33:4,209:5}[kind]
        value=invoke(c,u,0x441040,[extent,rows,columns,COUNT,OUT],'legacy');state['collect']=False
        result=(value,tuple(state['writes']),snapshot(u),bytes(u.mem_read(COUNT,OUT+2048-COUNT)))
        if expected is None:expected=result
        else:assert result==expected,('occupancy/slots composition',kind,rows,columns)
    compositions+=1

if __name__=='__main__':
    quick='--quick' in sys.argv
    for n in range(2,6):
        for x,y in itertools.product(range(n),repeat=2):exercise(0x43f3a0,args=(x,y,n))
    for n in [-2147483648,-1,0,1,6,257,2147483647]:exercise(0x43f3a0,args=(1,1,n))
    for n,x,y in [(2,256,-256),(3,256,0),(4,0,513),(5,256,0)]:exercise(0x43f3a0,args=(x,y,n))
    for n in range(2,6):
        for rx,ry,mask in itertools.product(range(n),range(n),range(1 if quick else 16)):
            exercise(0x43f250,args=(90,80,90-rx,80-ry,n),mask=mask)
    for n,rx,ry in itertools.product([1,2,3,5],[-1,0,1,5],[-1,0,1,5]):
        exercise(0x43f250,args=(90,80,90-rx,80-ry,n),mask=15)
    for d in range(4):exercise(0x43f250,args=(90,80,87,77,7),mask=15,hole=d)
    exercise(0x43f250,args=(0x1234,0,0x1234,0,2),center=False)
    for kind,pattern in itertools.product([31,30,33,209,0,-1,10,210],range(1 if quick else 5)):
        exercise(0x4413b0,args=(90,80,kind),pattern=pattern)
    for x,y in [(2147483647,80),(90,2147483647),(-10,-20)]:exercise(0x4413b0,args=(x,y,209))
    for n,rows,columns in itertools.product(range(1,6),range(1,6),range(1,6)):
        for pattern in range(1 if quick else 3):exercise(0x441040,args=(n,rows,columns),pattern=pattern)
    for args in [(0,1,1),(-1,1,1),(1,0,0),(2,-1,2),(2,2,-1),(2,0,3),(3,1,1),(1,257,1),(1,1,257)]:
        exercise(0x441040,args=args,empty=True)
    exercise(0x441040,args=(5,1,1),out=0)
    for count,out in [(COUNT,0x775614),(0x775614,OUT),(OUT,OUT)]:exercise(0x441040,args=(3,2,1),count=count,out=out,empty=True)
    for kind in [31,30,33,209]:
        for rows,columns in [(1,1),(2,1),(1,2)]:composition(kind,rows,columns)
    missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
    print('PASS construction:',checked,'scenarios +',compositions,'compositions;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges,',len(jumps),'switch targets',flush=True)
    print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edge outcomes:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    if quick:sys.exit(0)
    sources=['CMakeLists.txt','include/road_scan_model.h','runtime/menu_drag_construction_scan.c','direct_c/verify_construction_scan.py','metadata/construction-scan-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,
        checked=checked,compositions=compositions,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},
        conditional_edges=len(edges),switch_targets=len(jumps),unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        limitations=['Full EAX for three nonvoid entries, ordered game/output stores, all original data and explicit output buffers, stack balance and saved registers.',
            'Native private argument-slot scratch and volatile registers/flags are not part of the typed facade contract; no concurrency, arbitrary pointer faults or live gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/construction-scan-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
