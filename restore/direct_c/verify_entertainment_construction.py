"""Whole-PE native/C entertainment constructors and bandstand image selection.

Actual preview restore, site/slot queries, allocator, map stamps, undo capture
and footprint commit execute. No algorithm/platform return is substituted.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/entertainment-construction-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x4405c0:'restore_bandstand_images',0x440700:'restore_build_booth',0x440850:'restore_build_bandstand',0x440b10:'restore_build_pavilion',0x440ed0:'restore_build_festival_square'}
owned={};branches={};hits=set();edges=set();jumps=set();last=[None];checked=invocations=0;OUT=S+0x2000
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for table in catalogue['tables']:
    a=int(table['address'],16);raw=bytes.fromhex(table['bytes_hex'])
    assert table['complete'] and raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values());machines=[]
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in list(names.values())+['restore_entertainment_preview_restore']]
calls={0x40f860:3,0x485830:0,0x43f410:1,0x4413b0:3,0x441130:6,0x4405c0:3,0x46a400:6,0x47bf50:7,0x485100:1,0x440060:3}
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(collect=False,own=True,writes=[],events=[],returns=set(),pending={},compiled=c);machines.append((c,u,state))
    def write(uc,access,at,size,value,data):
        if data['collect'] and data['own']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=OUT,end=OUT+63)
    if not c:
        def code(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0]==0x4405ce:jumps.add(pc)
            last[0]=pc
        for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
    def returned(uc,pc,size,data):
        if not data['collect'] or pc not in data['pending']:return
        token,ptrs=data['pending'].pop(pc);data['own']=True
        if token in [0x441130,0x4405c0]:data['events'].append(('outputs',token,tuple(get(uc,p) for p in ptrs)))
    def child(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in ranges)
        if not immediate:return
        args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]));ptrs=()
        if t==0x441130:ptrs=args[2:4];normalized=args[:2]+args[4:]
        elif t==0x4405c0:ptrs=args[1:];normalized=args[:1]
        else:normalized=args
        data['events'].append((t,normalized));data['own']=False;data['pending'][ret]=(t,ptrs)
        if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
        data['counts'][t]=data['counts'].get(t,0)+1;n=data['counts'][t];mode=data['mutation']
        if t==0x441130 and ((mode=='fail2' and n==2) or (mode=='fail3' and n==3)):uc.mem_write(0x775614,b'\x01\0\0\0'*25)
        if t==0x441130 and mode=='rotate' and n==2:put(uc,0x775614+(0*5+2)*4,1)
        if t==0x485830 and mode=='restore':
            put(uc,0xbca2d0,93);put(uc,0xbca2d4,83);put(uc,0xbca2d8,2000);put(uc,0xeaac28,3)
        if t==0x46a400 and mode=='images' and n==1:
            put(uc,0xf567ac,333333);put(uc,0xf5659c,444444);put(uc,0xf566fc,555555);put(uc,0xeaac28,3)
        if t==0x440060 and mode=='player':put(uc,0xeaac28,3)
    state['entries']={(sym('restore_building_allocate_0046a400' if t==0x46a400 else 'FUN_%08x'%t) if c else t):t for t in calls}
    for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def fixture(u,fn,v):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(OUT,b'\x6b'*64)
    for reg,val in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,val)
    put(u,0xbca2e0,v.get('view',0));put(u,0xf566fc,v.get('bank',1000))
    if fn==0x4405c0:return [v.get('direction',0),v.get('first',OUT),v.get('second',OUT+4)]
    kind={0x440700:31,0x440850:30,0x440b10:33,0x440ed0:209}[fn];size={31:2,30:3,33:4,209:5}[kind]
    x,y=v.get('position',(90,80));width,height=v.get('dimensions',(200,200));player=v.get('player',0)
    ox=min(x,width-1) if kind!=30 else x;oy=min(y,height-1) if kind!=30 else y
    for at,val in [(0xbca2d8,1000),(0xbca2d0,width),(0xbca2d4,height),(0xbca2dc,228-width),(0xbca304,ox),(0xbca308,oy),
                   (0xbca2e8,0),(0xbca2ec,0),(0xbca2f0,200),(0xbca2f4,200),(0xbca2f8,100),(0xbca2fc,100),
                   (0xeaac28,player),(0xbc9694,1),(0x78d7b4,100),(0xbc9714,0)]:put(u,at,val)
    u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xba746c,bytes(0x22000));u.mem_write(0x78d580,bytes(0x320))
    put(u,0x78d7b4,100)
    for row in range(200):
        put(u,0xbc9f24+row*4,0);put(u,0xbc9b94+row*4,0);put(u,0xbc9804+row*4,1000+row*228)
    for row in range(max(0,oy),min(height,oy+size)):
        put(u,0xbc9f24+row*4,ox);put(u,0xbc9b94+row*4,ox+size);put(u,0xbc9804+row*4,1000+row*228+ox)
    for city in [0,1,3]:
        a,b=v.get('existing',(-1,-1));put(u,0xea6010+city*0x49d8,a);put(u,0xea6014+city*0x49d8,b);word(u,0xea6084+city*0x49d8,7)
    for i in range(1000):put(u,0xf55bb4+i*8,100000+i*16)
    for i in range(36):put(u,0x5cc8c0+i*8,(i//6)*228+i%6)
    for at,scale in [(0xaf598c,4),(0xb4e6fc,2),(0xb7482c,4),(0xb285cc,1),(0xb67d1c,1),(0xae8e7c,1),(0xacf85c,1),(0xd05720,4)]:u.mem_write(at,bytes(228*228*scale))
    rx,ry=v.get('cross',((2,2) if size==5 else (1,size-1 if size==3 else 1)));roads=v.get('roads','cross')
    for dy in range(-5,size+5):
        for dx in range(-5,size+5):
            tile=1000+(oy+dy)*228+ox+dx;road=roads=='all' or (roads=='cross' and (dx==rx or dy==ry))
            put(u,0xaf598c+tile*4,0x40 if road else 0);put(u,0xa1126c+tile*4,0x123456)
    for src,dst,length in [(0xaf598c,0x9de62c,0x32c40),(0xae8e7c,0x9d1b1c,0xcb10),(0xb285cc,0x9b84fc,0xcb10),(0xb67d1c,0x9ab9ec,0xcb10)]:u.mem_write(dst,bytes(u.mem_read(src,length)))
    if 'free_records' in v:
        free=v['free_records']
        for i in range(1,4000):byte(u,0x7999d8+i*264,0 if i<=free else 1)
    return [v.get('preview',0),0x12345678,0x87654321,x,y]

def invoke(c,u,fn,args,route):
    global invocations
    invocations+=1;entry=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
    if c and fn!=0x4405c0 and route=='named':args=[args[0],args[3],args[4]]
    value=run(0,u,entry,args,budget=18000000)
    return None if fn==0x4405c0 else value

def exercise(fn,**v):
    global checked
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False;args=fixture(u,fn,v);state.update(writes=[],events=[],pending={},own=True,collect=True,counts={},mutation=v.get('mutation',''));last[0]=None
            try:value=invoke(c,u,fn,args,route)
            except Exception as e:raise AssertionError(('execution',hex(fn),v,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            if 'expected' in v:assert value==v['expected'],('fixture intent',hex(fn),v,route,value)
            state['collect']=False;answer=(value,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(OUT,64)))
            if expected is None:expected=answer
            elif expected!=answer:
                diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[1:3],answer[1:3])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[3],answer[3])) if a!=b),None)
                raise AssertionError((hex(fn),v,route,'EAX',expected[0],value,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[1:3],answer[1:3])],'memory',memory,'outputs',expected[4]==answer[4]))
    checked+=1

if __name__=='__main__':
    quick='--quick' in sys.argv
    for view,direction in itertools.product([0,1,2,3,4,5,6,7,-1,8],[0,1,2,-1]):exercise(0x4405c0,view=view,direction=direction)
    for view,direction in itertools.product([0,2,4,6],[0,1]):
        for first,second in [(OUT,OUT),(OUT,0xf566fc),(0xf566fc,OUT)]:exercise(0x4405c0,view=view,direction=direction,first=first,second=second,bank=0xffffffff)
    for fn in [0x440700,0x440850,0x440b10,0x440ed0]:
        for roads in ['none','all','cross']:exercise(fn,roads=roads,preview=1)
        for view in ([0] if quick else [0,2,4,6]):exercise(fn,view=view,expected=1)
    exercise(0x440850,cross=(0,1))
    exercise(0x440b10,mutation='rotate')
    for fn in [0x440850,0x440b10]:exercise(fn,mutation='fail2',preview=1)
    exercise(0x440b10,mutation='fail3',preview=1)
    for fn in [0x440700,0x440b10,0x440ed0]:exercise(fn,position=(96,86),dimensions=(93,83))
    for existing in [(0,0),(0,-1),(-1,0)]:exercise(0x440ed0,existing=existing,preview=1)
    if not quick:
        for fn in [0x440700,0x440850,0x440b10,0x440ed0]:
            exercise(fn,free_records=0)
            exercise(fn,mutation='images',player=1)
        for free in [1,2]:exercise(0x440850,free_records=free)
        for free in [1,2,3]:exercise(0x440b10,free_records=free)
        for fn in [0x440700,0x440b10,0x440ed0]:exercise(fn,mutation='restore',position=(96,86))
        exercise(0x440ed0,mutation='player')
    missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
    assert not missing,('instruction coverage',[hex(a) for a in sorted(missing)])
    assert not missing_edges,('edge coverage',missing_edges)
    assert jumps==set(struct.unpack('<7I',bytes.fromhex(catalogue['tables'][0]['bytes_hex'])))
    print('PASS entertainment construction:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges,',len(jumps),'switch targets',flush=True)
    print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edge outcomes:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    if quick:sys.exit(0)
    sources=['CMakeLists.txt','include/small_constructor_model.h','include/world_offset_model.h','runtime/menu_drag_construction_family.c','runtime/menu_drag_world_offset.c','direct_c/verify_entertainment_construction.py','metadata/entertainment-construction-native.json','runtime/menu_drag_construction_parent.c','runtime/menu_drag_construction_scan.c','runtime/building_allocator.c','runtime/construction_tiles.c','runtime/map_commit_state.c']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
        coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jumps),unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        limitations=['Real preview restore, site/slot queries, allocator including full/partially full tables, map stamps, undo capture and tile commit execute. Full EAX, owned ordered stores, actual child arguments/selected outputs, all final original data and ABI compared.',
            'Genuine child-entry input mutations cover failed later reservations, rotation retry, changed dimensions/origin/player and sprite banks without replacing helper returns.',
            'Constructor camera views0/2/4/6; direct image helper also checks invalid view/direction and output/base aliases. Native caller argument scratch and arbitrary private-stack aliases excluded. No host game launch or whole-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/entertainment-construction-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
