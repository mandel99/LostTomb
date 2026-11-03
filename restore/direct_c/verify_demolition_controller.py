"""Whole-PE native/C demolition request, mutation and refresh composition.

Actual children, normalized input contracts, owned ordered writes, all original
data and saved ABI. Synthetic resident text/font metadata, disabled audio and
the real no-image renderer guards; no host game/UI/pixel fidelity claim.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/demolition-controller-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x4743f0:'restore_demolition_request',0x474700:'restore_demolition_apply'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());machines=[]
calls={0x474700:5,0x40f860:3,0x4264d0:3,0x440060:3,0x440540:3,0x459020:4,0x474380:1,0x475720:5,0x475b50:1,0x475cc0:5,0x476530:0,0x4767d0:6,0x477500:4,0x4776f0:4,0x4777c0:4,0x477890:4,0x47afd0:3,0x47b480:3,0x47b590:4,0x47bae0:5,0x47bd50:4,0x47f810:2,0x47f9b0:1,0x485080:0,0x485100:1,0x485830:0,0x4e2970:1,0x4f4930:1,0x4f5e70:2,0x517370:0,0x517660:0,0x5178b0:0,0x5178e0:0,0x53b090:1}
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c);machines.append((c,u,state))
    def write(uc,access,at,size,value,data):
        if data['collect'] and data['own']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
    if not c:
        def code(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            last[0]=pc
        for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
    def returned(uc,pc,size,data):
        if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
    def child(uc,pc,size,data):
        if not data['collect'] or not data['own']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        if t==0x474700:
            if data['top']!=0x4743f0 or data['apply_seen']:return
            data['apply_seen']=True
        elif ret==STOP and data['top']==t:return
        if t==0x440540 and data['compiled']:
            ptr=get(uc,sp+4);args=tuple(get(uc,ptr+i*4) for i in range(3))+(get(uc,sp+8),)
        else:
            args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
            if t==0x440540:args=args+(uc.reg_read(UC_X86_REG_ECX),)
        if t==0x440060:args=args[:2]+(args[2]&65535,)
        if t==0x4f5e70:args=args[:1]+(args[1]&255,)
        data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1
        if data['mutation']=='reserved' and t==0x474380:B=0x7999d8+264;uc.mem_write(B,b'\x06')
        if data['mutation']=='pair_gate' and t==0x474380:uc.mem_write(0xbc96b4,b'\0'*4)
        if t!=0x474700:
            data['own']=False;data['pending'].add(ret)
            if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
    state['entries']={(sym('restore_bridge_00440540' if t==0x440540 else 'FUN_%08x'%t) if c else t):t for t in calls}
    if c:state['entries'][sym('restore_demolition_apply')]=0x474700
    for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

DATA,baseline,STACK,N=0x560000,seed,0x30000000,0xcb10
directions=[-228,-227,1,229,228,227,-1,-229]
def fixture(u,c):
    def put(a,v,n=4):u.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
    u.mem_write(DATA,baseline);u.mem_write(STACK,b'\xcc'*0x10000);u.mem_write(0xbc9730,bytes(0xd20))
    x,y=c.get('position',(8,8));width,height=c.get('dimensions',(16,16));origin=912;cell=origin+y*228+x
    for a,v in [(0xbca2d0,width),(0xbca2d4,height),(0xbca2d8,origin),(0xbca2dc,228-width),(0xbca2e0,c.get('orientation',0)),(0xbca2e8,c.get('offset_x',0)),(0xbca2ec,c.get('offset_y',0)),(0xbca2f0,500),(0xbca2f4,500),(0xbca2f8,100),(0xbca2fc,0),(0xeaac28,0),(0xeaaf64,0),(0x5e4ef4,5),(0xbc971c,c.get('count',0)),(0xbc9718,c.get('cost',0)),(0xbc9768,cell),(0x705f94,c.get('refund1',25)),(0x7078e4,c.get('refund2',41)),(0x126517c,0),(0x10d69f8,0)]:put(a,v)
    for a,n in [(0x7999d8,4000*264),(0xaf598c,N*4),(0xae8e7c,N),(0xb4e6fc,N*2),(0xb350dc,N*2),(0xd05720,N*4),(0xb7482c,N*4),(0xb285cc,N),(0xb67d1c,N),(0xacf85c,N),(0xa509bc,N),(0xf55bb4,0x2000)]:u.mem_write(a,bytes(n))
    u.mem_write(0xaf598c,struct.pack('<I',c.get('surround',0))*N);u.mem_write(0xa509bc,bytes([c.get('fertility',0)])*N)
    put(0xbc9768,cell)
    for row in range(228):put(0xbc9f24+row*4,0);put(0xbc9b94+row*4,width if row<height else 0);put(0xbc9804+row*4,origin+row*228)
    for j in range(4):put(0x784b64+j*2,1000,2);put(0x784b6c+j*2,1000,2);put(0x784cc0+j*4,1000);put(0x784cd0+j*4,1000)
    if c.get('point'):put(0x784b64,x,2);put(0x784b6c,y,2)
    u.mem_write(0xb285cc,bytes([c.get('access',0xff)])*N);u.mem_write(0xb67d1c,bytes([0x9f])*N)
    put(0xaf598c+cell*4,c.get('flags',0));put(0xb350dc+cell*2,c.get('building_word',0),2)
    pattern=c.get('pattern',0)
    for bit,d in enumerate(directions):
     if pattern&(1<<bit):put(0xaf598c+(cell+d)*4,0x4000)
    for bit,d in enumerate(directions[::2]):
     if c.get('road_pattern',0)&(1<<bit):
      at=0xaf598c+(cell+d)*4;put(at,int.from_bytes(u.mem_read(at,4),'little')|0x8000)
    if c.get('object'):put(0xd05720+cell*4,0x6f0000);put(0x6f0018,c.get('object_flag',0),1)
    # Real sparse map, empty sprite image0: renderer algorithms execute their
    # genuine no-image guards, never an injected rendering result.
    for group in range(0x240):put(0xf55bb4+group*8,1000+(group%200)*10)
    put(0x126517c,0);put(0x10d69f8,0)
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    u.mem_write(0xa1126c,struct.pack('<I',0x12345678)*N)
    u.mem_write(0xadc36c,bytes(N));u.mem_write(0xba746c,bytes(0x41a));u.mem_write(0xba7888,b'\xa5'*0x21d68)
    u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xeaaf68,bytes(0x2bc0));u.mem_write(0x9784d8,bytes(0x89c))
    for at,v,n in [(0x5e4ef4,0,4),(0xf55b88,0,4),(0xe38e63,1,1),(0xbc95f0,c.get('reservation_count',0),4),(0xbc96a8,c.get('allow_class',0),4),(0xbc96ac,c.get('allow_special',0),4),(0xbc96b0,c.get('allow_line',0),4),(0xbc96b4,c.get('allow_54',0),4),(0xbc9694,1,4),(0x604950,0,4),(0xd056fc,0,4),(0xbcd5f0,0,4),(0xd3b1c4,0,4)]:put(at,v,n)
    for j in range(100):put(0xd3b1c8+j*4,j);put(0xea3e34+j*2,6 if j in [0,70] else 0,2)
    for at,v in [(0xea3f68,0),(0xea1894,12),(0xea5f0c,12)]:put(at,v)
    kind=c.get('kind')
    if kind is not None:
     base=0x7999d8+264
     for at,v,n in [(base,c.get('status',1),1),(base+5,c.get('occupied',0),1),(base+6,x,2),(base+8,y,2),(base+12,cell,4),(base+16,kind,2),(base+28,c.get('people',0),2),(base+56,c.get('next',0),2),(base+58,0,2),(base+84,0,1),(0x5f5d08,1,4),(0x5cfcdc,0,4)]:put(at,v,n)
     put(0xb4e6fc+cell*2,1,2)
     if c.get('reservation_count'):put(0xba746c,1,2)
     if c.get('next'):
      put(0x7999d8+c['next']*264,1,1);put(0x7999e8+c['next']*264,20,2)
     if kind==183:
      cap=cell+5;put(0xaf598c+(cap+1)*4,4);put(0xaf598c+(cap+229)*4,4)
    if c.get('bridge'):
     put(0xaf598c+cell*4,4);put(0xadc36c+cell,1,1);put(0xb350dc+cell*2,c.get('line_block',0),2)
    if c.get('constructor'):
     base=0x7999d8+2*264;where=origin+5*228+5;typ=c['constructor']
     for at,v,n in [(base,1,1),(base+6,5,2),(base+8,5,2),(base+16,typ,2),(base+58,3,2),(base+208,where,4),(base+212,c.get('direction',0),4),(base+216,1,4),(0x7999d8+3*264,1,1),(0x7999e8+3*264,typ,2)]:put(at,v,n)
    for at,v,n in [(0xe38ec1,1,1),(0xe38dda,5,1),(0xe36c70,0x32001000,4),(0xe34c38+19*8,0x1000,4),(0xe34c38+17*8,0x1000,4),(0xf56f8c,10,4),(0xf56fb4,10,4),(0xf56fbc,20,4),(0xf56f84,30,4),(0x126517c,512,4),(0xe92e0c,0x11223344,4)]:put(at,v,n)
    u.mem_write(0xe94e28,bytes(5*120));u.mem_write(0x10d69f8,bytes(512*64))
    for j in range(512):put(0x10d6a0c+j*64,6,2)
    for ch in [65,66,67,48,49,50]:put(0x5e047c+ch,1,1)
    u.mem_write(0x32000000,bytes(0x10000));u.mem_write(0x32002000,b'A00\0'*300+b'TAIL\0')
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)
    if c.get('canal'):
        for at,val in [(0xae8d98,5),(0xae8e7d,6),(0xae8f60,5),(0xae8e7b,6)]:put(at+cell,val,1)
    if c.get('slots'):
        for at,val,n in [(0x799a99+264,2,1),(0x799aa3+264,3,1),(0x799aa2+264,4,1),
                         (0x7900c0+2*196,1,1),(0x9783a4+3*28,1,1),(0xeaaf68+4*224,99,4),(0x799a7e+264,1,1)]:put(at,val,n)
    if c.get('forward'):
        put(0x799a12+264,4,2);put(0x7999d8+4*264,1,1);put(0x7999e8+4*264,20,2)
    if c.get('cycle'):
        put(0x799a10+264,1,2);put(0x799a12+264,1,2)
    if c.get('long_chain'):
        for node in range(1,528):
            put(0x7999d8+node*264,1,1);put(0x7999e8+node*264,20,2);put(0x799a10+node*264,node+1 if node<527 else 0,2)
    if 'marker' in c:put(0xadc36c+cell,c['marker'],1)
    if c.get('mixed'):
        for node,kind in enumerate([65,54],2):
            put(0x7999d8+node*264,1,1);put(0x7999e8+node*264,kind,2);put(0xb4e6fc+(cell+node-1)*2,node,2);put(0xaf598c+(cell+node-1)*4,8)
    if c.get('child_parent'):put(0x799a10+3*264,2,2)
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    return [c.get('preview',0),*c.get('bounds',(x,y,x,y))]

def exercise(fn=0x474700,**cfg):
    global checked,invocations
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False;args=fixture(u,cfg);state.update(writes=[],events=[],pending=set(),own=True,collect=True,counts={},top=fn,apply_seen=False,mutation=cfg.get('mutation',''));last[0]=None
            invocations+=1;pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
            try:run(0,u,pc,args,budget=100000000)
            except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
            if expected is None:expected=answer
            elif answer!=expected:
                diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
                raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory))
    checked+=1
    if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
    quick='--quick' in sys.argv
    cases=[(0x4743f0,{}),(0x474700,dict(preview=1,flags=0x4000)),(0x4743f0,dict(preview=1,kind=20,flags=8)),(0x4743f0,dict(kind=183,flags=8)),(0x474700,dict(kind=183,flags=8,allow_special=1)),(0x474700,dict(kind=20,flags=8,occupied=1,people=258,next=4)),(0x474700,dict(preview=1,constructor=30,direction=2,orientation=7)),(0x474700,dict(bridge=True,line_block=1)),(0x474700,dict(kind=54,flags=8,allow_54=1)),(0x4743f0,dict(kind=65,flags=8)),(0x4743f0,dict(bridge=True)),(0x474700,dict(preview=1,constructor=209))]
    for fn,cfg in cases:exercise(fn,**cfg)
    if not quick:
        for fn,preview in itertools.product([0x4743f0,0x474700],[0,1]):
            for bounds in [(8,8,9,9),(9,9,8,8),(-1,-1,1,1),(14,14,999,999),(999,999,999,999)]:exercise(fn,preview=preview,bounds=bounds)
        for flag in [0,0x40,0x80,0x100,0x140,0x2040202,0x40000000,0x10000000]:
            for preview in [0,1]:exercise(preview=preview,flags=flag)
        for kind in [99,88,89,93,218,219,220,215,54,57,65,66,67,68,69,97,156,157,158,159,160,108,195,115,20,31,183,213,225]:
            exercise(preview=1,kind=kind,flags=8)
        for kind in [54,57,65,183,20,108,195,115]:
            for allow in [0,1]:exercise(kind=kind,flags=8,allow_special=allow,allow_class=allow,allow_54=allow)
            exercise(0x4743f0,kind=kind,flags=8)
        for constructor in [30,31,33,209]:
            for direction in [0,1]:exercise(preview=1,constructor=constructor,direction=direction)
        for kind in [66,67,68,69,97,156,157,158,159,160]:exercise(0x4743f0,kind=kind,flags=8)
        for kind in [20,54,65,183]:exercise(preview=1,kind=kind,flags=8,status=6)
        for kind in [20,108,195,115]:exercise(preview=1,kind=kind,flags=8,slots=True)
        exercise(flags=0x100,canal=True);exercise(preview=1,flags=0x100,canal=True)
        exercise(preview=1,kind=20,flags=8,forward=True)
        exercise(preview=1,kind=20,flags=8,cycle=True)
        exercise(preview=1,kind=20,flags=8,long_chain=True)
        exercise(preview=1,kind=20,flags=8,mutation='reserved')
        exercise(kind=20,flags=8,occupied=1,people=0)
        exercise(preview=1,kind=20,flags=8,occupied=1,people=10)
        exercise(0x4743f0,kind=183,flags=8,mixed=True,bounds=(8,8,10,8))
        exercise(0x4743f0,bridge=True,kind=20)
        exercise(0x4743f0,marker=1)
        for kind,allow in [(54,'allow_54'),(65,'allow_class'),(183,'allow_special')]:exercise(kind=kind,flags=8,**{allow:256})
        for kind in [20,213,225]:exercise(preview=1,kind=kind,flags=0x10000000)
        for fn in [0x4743f0,0x474700]:exercise(fn,preview=1,bounds=(999,8,999,8))
        exercise(0x4743f0,bounds=(999,8,999,8))
        exercise(preview=1,bounds=(8,8,11,8))
        for preview in [0,1]:exercise(preview=preview,bridge=True,allow_line=1)
        exercise(kind=54,flags=8,allow_54=1,mutation='pair_gate')
        for constructor in [30,31,33,209]:exercise(preview=1,constructor=constructor,child_parent=True)
    missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
    if not quick:
        assert not missing,('instruction coverage',missing)
        assert not missing_edges,('edge coverage',missing_edges)
    print('PASS demolition:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
    print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    if quick:sys.exit(0)
    sources=['CMakeLists.txt','include/construction_controller.h','runtime/construction_controller.c','direct_c/verify_demolition_controller.py','metadata/demolition-controller-native.json','include/bridge_place.h','runtime/bridge_place.c']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
        coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        limitations=['Actual confirmation scheduling, terrain edits, displacement, undo, redraw and refresh children. Owned ordered stores, helper inputs, all final original data and saved ABI compared.',
            'Bandstand redraw compares the genuine native incoming ECX against the explicit next-link-ID*33 C adapter input. Private caller scratch and void incidental registers/flags excluded.',
            'Resident synthetic text/font metadata, disabled audio device and genuine no-image renderer guards; no pixel/audio/UI fidelity, concurrent access or host game launch claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/demolition-controller-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
