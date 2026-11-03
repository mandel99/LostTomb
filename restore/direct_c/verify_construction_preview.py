"""Native/C drag-input and40-route preview controllers; actual child graph.

Full original data, ordered owned stores, immediate helper inputs, ABI and
native branch coverage. No algorithm or platform helper return is replaced.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/construction-preview-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x405d90:'restore_construction_drag_update',0x46fef0:'restore_construction_preview'}
owned={};branches={};hits=set();edges=set();jumps=set();last=[None];checked=invocations=0
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
points=set.union(*owned.values());machines=[]
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in names.values()]
calls={0x46fef0:5,0x478440:2,0x40f920:3,0x440700:5,0x440850:5,0x440b10:5,0x440ed0:5,0x4743f0:5,0x47eda0:5,0x47bae0:5,0x475220:6,0x474f50:5,0x4754a0:5,0x4759e0:5,0x475090:5,0x474130:5,0x4bfa50:3,0x47d530:5,0x47d060:3,0x47cc70:4,0x486cf0:1,0x486a30:9,0x486020:0,0x4829b0:6,0x46fde0:1,0x4828e0:5,0x482650:5,0x482720:5,0x47d360:6}
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
            if last[0]==0x46ff7e:jumps.add(pc)
            last[0]=pc
        for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
    def returned(uc,pc,size,data):
        if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
    def entry(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in ranges)
        tail=data['compiled'] and data['top']==0x405d90 and ret==STOP
        if not immediate and not tail:return
        if t==0x46fef0:
            if data['top']!=0x405d90 or data['preview_seen']:return
            data['preview_seen']=True
        args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
        if t==0x482720:args=args[:4]+(data['predicates'].get(args[4],args[4]),)
        if t==0x486a30:args=args[:8]+('count-local',)
        if t==0x47d360:args=args[:5]+(args[5]&255,)
        data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1
        if t!=0x46fef0:
            data['own']=False;data['pending'].add(ret)
            if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
        if data['mutation']=='discount' and t==0x40f920:put(uc,0xbc9718,7)
    state['entries']={(sym('restore_building_rect_00474130' if t==0x474130 else 'FUN_%08x'%t) if c else t):t for t in calls}
    if c:state['entries'][sym('restore_construction_preview')]=0x46fef0
    for a in state['entries']:u.hook_add(UC_HOOK_CODE,entry,state,begin=a,end=a)
    state['predicates']={}
    if c:
        for token in [0x405b40,0x405b60,0x405b80]:state['predicates'][run(0,u,sym('restore_placement_rule'),[token])]=token

def store_value(u,a,v,n=4):u.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def dw(v):return struct.pack('<I',v&0xffffffff)
DATA,BASELINE=0x560000,seed
def fixture(u,fn,tool,cfg):
    width=20;height=20;x=8;y=8;origin=912;cell=origin+y*228+x
    coords=cfg.get('coords',(x,y,x+1,y+1))
    u.mem_write(DATA,BASELINE)
    for a,n in [(0x7999d8,4000*264),(0xaf598c,0x32c40),(0xae8e7c,0xcb10),(0xb4e6fc,0x19620),(0xb350dc,0x19620),(0xd05720,0x32c40),(0xb7482c,0x32c40),(0xb285cc,0xcb10),(0xb67d1c,0xcb10),(0xacf85c,0xcb10),(0xa509bc,0xcb10),(0x705c6c,0x28),(0xba745c,0x22200),(0xbc95e0,0x6c),(0xbc9730,0xd20),(0xd3b1b0,0x1e0),(0x804000,0x10000),(0xeaac30,0x340)]:u.mem_write(a,bytes(n))
    for a,v in [(0xbca2d0,width),(0xbca2d4,height),(0xbca2d8,origin),(0xbca2dc,228-width),(0xbca2e0,cfg.get('orientation',0)),(0xbca2e8,0),(0xbca2ec,0),(0xbca2f0,20),(0xbca2f4,20),(0xbca2f8,10),(0xbca2fc,10),(0xbca300,cfg.get('cell',cell)),(0xbca304,x),(0xbca308,y),(0xbca310,cfg.get('link_mode',0)),(0xbca314,0xa5a5a5a5),(0xeaac28,0),(0xeaaf64,0),(0xea187c,cfg.get('cash',10000)),(0xbc96f8,cfg.get('brush',1)),(0xbc971c,cfg.get('count',1)),(0xbc9718,0),(0xbc9768,cell),(0x605994,1),(0x6059e4,cfg.get('drag',1)),(0x6059e0,coords[0]),(0x6059d8,coords[1]),(0x6059dc,coords[2]),(0x6059d4,coords[3]),(0x6059e8,tool),(0x605a3c,cfg.get('editor',0)),(0x126517c,0),(0x10d69f8,0)]:store_value(u,a,v)
    u.mem_write(0x6059b4,bytes(28));store_value(u,0x605a48,cfg.get('predicate',0))
    if 'gate' in cfg:store_value(u,cfg['gate'],cfg.get('gate_value',1))
    store_value(u,0x605a5c,cfg.get('held',0),1);store_value(u,0xe38ec1,0,1)
    for j,v in enumerate([0,0,width-1,height-1]):store_value(u,0x784c64+j*2,v,2)
    for row in range(228):store_value(u,0xbc9f24+row*4,0);store_value(u,0xbc9b94+row*4,width if row<height else 0);store_value(u,0xbc9804+row*4,origin+row*228)
    for j in range(4):store_value(u,0x784b64+j*2,1000,2);store_value(u,0x784b6c+j*2,1000,2);store_value(u,0x784cc0+j*4,1000);store_value(u,0x784cd0+j*4,1000)
    u.mem_write(0xaf598c,dw(cfg.get('flags',0))*0xcb10);u.mem_write(0xb285cc,bytes([0x80])*0xcb10);u.mem_write(0xb67d1c,bytes([0x9f])*0xcb10)
    # Genuine terrain2 blocks the flood outside a finite open physical patch.
    u.mem_write(0xee0c50,bytes([2])*0xcb10)
    for yy in range(7,12):
     for xx in range(7,16):store_value(u,0xee0c50+origin+yy*228+xx,0,1)
    matrix=cfg.get('matrix',[int(j//5==2 or j%5==2) for j in range(25)])
    if tool in [30,31,33,209]:
     u.mem_write(0xaf598c,dw(0x40)*0xcb10)
     for yy in range(5):
      for xx in range(5):store_value(u,0xaf598c+(cell+xx+yy*228)*4,0x40 if matrix[xx*5+yy] else 0)
    if cfg.get('existing',False):
     store_value(u,0xb4e6fc+cell*2,1,2);store_value(u,0x7999d8+264,1,1);store_value(u,0x7999e8+264,90,2);store_value(u,0x7999de+264,x,2);store_value(u,0x7999e0+264,y,2);store_value(u,0x7999db+264,1,1);store_value(u,0xbc9800,1)
    if cfg.get('dock',False):
     for id,rx,ry in [(1,coords[0],coords[1]),(2,coords[2],coords[3])]:
      store_value(u,0x7999d8+id*264,1,1);store_value(u,0x7999e8+id*264,136,2);store_value(u,0x7999de+id*264,rx,2);store_value(u,0x7999e0+id*264,ry,2);store_value(u,0x7999db+id*264,1,1);store_value(u,0x799a9c+id*264,cfg.get('linked',0xffffffff));store_value(u,0x799a82+id*264,cfg.get('source_orientation' if id==1 else 'target_orientation',0),1)
      for yy in range(2):
       for xx in range(2):store_value(u,0xb4e6fc+(origin+(ry+yy)*228+rx+xx)*2,id,2)
     store_value(u,0xbc9800,1)
    for group in range(0x240):store_value(u,0xf55bb4+group*8,1000)
    for a,v in [(0xf567ac,200),(0xf566fc,300),(0xf5659c,400),(0xf565f4,500)]:store_value(u,a,v)
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,0x32c40)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,0xcb10)));u.mem_write(0x9b84fc,bytes(u.mem_read(0xb285cc,0xcb10)));u.mem_write(0x9ab9ec,bytes(u.mem_read(0xb67d1c,0xcb10)))
    if cfg.get('shore'):
        base=origin+coords[3]*228+coords[2]
        size=cfg['shore'];deltas=0x5cccc8 if size==2 else 0x5cccec;pattern=0x5dbda8 if size==2 else 0x5dbe48
        for j in range(size*size):put(u,(0xaf598c+(base+get(u,deltas+j*4))*4)&0xffffffff,4 if get(u,pattern+j*4) else 0)
        borders=[0x5dbde8,0x5dbe08,0x5dbe28] if size==2 else [0x5dbed8]
        for table in borders:
            for j in range(2):put(u,(0xaf598c+(base+get(u,table+j*4))*4)&0xffffffff,4)
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)
    return [] if fn==0x405d90 else list(coords)+[tool]

def exercise(tool=31,fn=0x46fef0,**cfg):
    global checked,invocations
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False;args=fixture(u,fn,tool,cfg);state.update(writes=[],events=[],pending=set(),own=True,collect=True,counts={},mutation=cfg.get('mutation',''),top=fn,preview_seen=False);last[0]=None
            invocations+=1;pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
            try:run(0,u,pc,args,budget=25000000)
            except Exception as e:raise AssertionError(('execute',hex(fn),tool,cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
            if expected is None:expected=answer
            elif answer!=expected:
                diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
                raise AssertionError((hex(fn),tool,cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory))
    checked+=1
    if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
    quick='--quick' in sys.argv
    for tool in [0,7,9,10,31]:exercise(tool)
    for token,flags in [(0x405b40,0x100002),(0x405b60,2),(0x405b80,2)]:exercise(11,predicate=token,flags=flags)
    exercise(136,dock=True,coords=(8,8,12,8),link_mode=1,target_orientation=2)
    exercise(82,brush=2)
    for tool,editor in [(31,0),(5,1),(88,1),(31,1)]:exercise(tool,fn=0x405d90,editor=editor,drag=256)
    if not quick:
        for tool in range(5,237):exercise(tool)
        for tool in [-1,1,4,237,255]:exercise(tool)
        for cash in [-5001,-5000,-4999]:exercise(11,cash=cash)
        for count in [-1,0,3]:exercise(31,count=count,mutation='discount')
        for tool in [40,182]:exercise(tool,gate=0xbcb9b4 if tool==40 else 0xbcb9b0,gate_value=6,mutation='discount')
        for tool,gate in [(50,0xea5e70),(77,0xea620c),(84,0xea5cf6),(94,0xea6240),(95,0x705c6c)]:exercise(tool,gate=gate,gate_value=256)
        for flag in [0x6059cc,0x6059c8,0x6059c4,0x6059bc,0x6059b8,0x6059b4,0x6059c0]:
            for flags in [0,0xffffffff]:exercise(11,gate=flag,flags=flags,mutation='discount')
        for mode,linked in itertools.product([0,1,2,3],[-1,0]):exercise(136,dock=True,coords=(8,8,12,8),link_mode=mode,linked=linked)
        for drag,cell in [(0,100),(1,0)]:exercise(11,fn=0x405d90,drag=drag,cell=cell)
        for held,tool in itertools.product([0,1,255],[5,88,89,93,31]):exercise(tool,fn=0x405d90,editor=1,held=held)
        for tool in [74,75,182]:exercise(tool,shore=3)
        for tool in [76,90,136,181]:exercise(tool,shore=2)
        exercise(182,shore=3,gate=0xbcb9b0,gate_value=6,mutation='discount')
        for flags in [0,0x80]:exercise(77,gate=0x6059b4,flags=flags)
        exercise(11,gate=0x6059cc,flags=0x800)
        exercise(11,gate=0x6059b8,flags=0x40)
        exercise(11,predicate=0x405b60,flags=0)
        exercise(136,dock=True,coords=(8,8,12,8),link_mode=1,source_orientation=1,target_orientation=3,flags=4,gate=0xbc96e4,gate_value=2)
    missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
    if not quick:
        assert missing=={0x470442,0x470443,0x470446},('unexpected instruction coverage',missing)
        assert missing_edges=={(0x470440,0x470442)},('unexpected edge coverage',missing_edges)
        assert jumps==set(struct.unpack('<40I',bytes.fromhex(catalogue['tables'][0]['bytes_hex'])))
    print('PASS preview:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges,',len(jumps),'switch targets',flush=True)
    print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    if quick:sys.exit(0)
    sources=['CMakeLists.txt','include/preview_parent_model.h','runtime/menu_drag_preview_parent.c','direct_c/verify_construction_preview.py','metadata/construction-preview-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
        coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jumps),unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        limitations=['Actual child graph; owned stores, normalized immediate helper inputs, all final original data, stack/saved registers. Input tail calls observed at their genuine callee entries.',
            'Pointer normalization only for actual callback addresses and the route count output; sixth footprint argument consumes AL. Native private argument scratch is excluded.',
            'Three signed-remainder repair instructions470442/443/446 are unreachable after an unsigned BYTE forms4..514 and AND7 yields0..7. No forced returns, host game launch or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/construction-preview-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
