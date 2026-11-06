"""Bounded region/flood/irrigation recipes. No executable is run here."""
import struct

S = 0x32000000
REGION, ITEM, NODE = S+0x1000, S+0x3000, S+0x5000
WORLD, GLOBAL = S+0x10000, 0xd05720
TARGETS = (0x4bd6a0,0x4bd740,0x4bd950,0x4be0f0,
           0x4be640,0x4beb90,0x4becc0,0x4becf0)


def p(value, width=4):
    return (value & ((1 << (width*8))-1)).to_bytes(width, 'little')


class Case:
    def __init__(self, entry, label, receiver, arguments=(), named=None,
                 cleanup=0, result=None):
        self.mem = {}
        self.c = dict(id=f'c7-compiled-acceptance/{entry:08x}/'+label,
            entry=entry, receiver=receiver, arguments=list(arguments),
            named_arguments=[receiver,*arguments] if named is None else list(named),
            canonical_arguments=[receiver,*arguments] if named is None else list(named),
            eflags=0xad7, initial_eax=0xf1e2d3c4, native_cleanup=cleanup,
            named_cleanup=0, canonical_cleanup=0, result_mask=result,
            canonical_registers_and_flags=False, expected_invocations=7,
            endpoints=[], expected_endpoint_counts={}, expected_endpoint_arguments={},
            authored_endpoint_order=[], authored_endpoint_observations=[])
        self.eps = {}

    def block(self, at, size, fill=0xa5):
        assert all(at+size<=old or old+len(data)<=at for old,data in self.mem.items())
        self.mem[at] = bytearray(bytes([fill])*size)

    def locate(self, at, width):
        for base,data in self.mem.items():
            if base<=at and at+width<=base+len(data):return data,at-base
        raise AssertionError(('fixture range',hex(at),width))

    def write(self, at, data):
        block,index=self.locate(at,len(data));block[index:index+len(data)]=data

    def put(self, at, value, width=4):self.write(at,p(value,width))
    def get(self, at, width=4):
        data,index=self.locate(at,width);return int.from_bytes(data[index:index+width],'little')

    def start(self, observe=None):
        self.c['stores']=[(at,bytes(data))for at,data in self.mem.items()]
        self.c['watched']=[(at,len(data))for at,data in self.mem.items()]
        self.c['write_ranges']=list(self.c['watched'])
        self.observe=observe if observe is not None else [
            (at+i,4)for at,data in self.mem.items()for i in range(0,min(len(data),28)//4*4,4)]

    def endpoint(self, token, address, arity, result='void', native_cleanup=0,
                 compiled_symbol=None, native_receiver=None):
        assert token not in self.eps
        spec=dict(token=token,native=address,
            compiled_symbols=[compiled_symbol or f'FUN_{address:08x}'],arity=arity,
            native_cleanup=native_cleanup,compiled_cleanup=0,result_kind=result,
            value=0,responses=[],observe=self.observe,
            clobber_ecx=0xdead4321,clobber_edx=0xbeef3456)
        if native_receiver is not None:spec['native_receiver']=native_receiver
        self.eps[token]=spec
        self.c['expected_endpoint_counts'][token]=0
        self.c['expected_endpoint_arguments'][token]=[]

    def event(self, token, args, value=0, mutations=()):
        c=self.c;spec=self.eps[token]
        c['authored_endpoint_order'].append(token)
        c['authored_endpoint_observations'].append([
            [at,width,self.get(at,width)]for at,width in self.observe])
        c['expected_endpoint_counts'][token]+=1
        c['expected_endpoint_arguments'][token].append([x&0xffffffff for x in args])
        response=dict(value=value)
        if mutations:response['mutations']=list(mutations)
        spec['responses'].append(response)
        for at,width,new in mutations:self.put(at,new,width)

    def end(self, expected=None):
        if expected is not None:
            self.c['expected_masked_result']=expected&self.c['result_mask']
        self.c['endpoints']=list(self.eps.values())
        self.c['authored_final_memory']=[(at,bytes(data).hex())for at,data in self.mem.items()]
        return self.c


def add_chain(c, region, count, first=NODE, item=ITEM, priorities=None):
    c.put(region,count);c.put(region+16,first if count else 0)
    c.put(region+20,first+(count-1)*32 if count else 0)
    c.put(region+24,0x88776655)
    for i in range(count):
        n,o=first+i*32,item+i*32
        c.block(n,16);c.block(o,28)
        c.put(n,first+(i+1)*32 if i+1<count else 0)
        c.put(n+4,first+(i-1)*32 if i else 0)
        c.put(n+8,(priorities or list(range(count)))[i]);c.put(n+12,o)
        c.put(o+4,n)


def teardown(c, region):
    if not c.get(region):return
    node=c.get(region+16)
    current=c.get(node+12)if node else 0
    node=c.get(node)if node else 0
    c.put(region+24,current)
    while current:
        following=c.get(node+12)if node else 0
        node=c.get(node)if node else 0
        link=c.get(current+4);previous=c.get(link+4);next_=c.get(link)
        if previous:c.put(previous,next_)
        else:c.put(region+16,next_)
        if next_:c.put(next_+4,previous)
        else:c.put(region+20,previous)
        c.put(link,0);c.put(link+4,0)
        c.event('delete',[link])
        c.put(current+4,0)
        # Real51F780 is the existing empty RET child; it is not replaced.
        c.event('delete',[current])
        c.put(region+24,following);current=following
    c.put(region,0)


def teardown_cases():
    out=[]
    for label,count in [('zero-count-preserves-stale-links',0),
                        ('positive-count-null-head',-1),('one-item',1),('two-items',2)]:
        c=Case(0x4bd6a0,label,REGION);c.block(REGION,28)
        if count>0:add_chain(c,REGION,count)
        else:
            c.put(REGION,0 if count==0 else 1)
            c.put(REGION+16,0xdeadbeef if count==0 else 0)
        c.start();c.endpoint('delete',0x561a2b,1)
        teardown(c,REGION);out.append(c.end())
    return out


def insertion_cases():
    out=[]
    modes=[('empty',[],70,None),('front-equal',[70,100],70,None),
           ('middle-unsigned-BYTE',[20,240],229,None),
           ('tail-equal',[20,70],70,None),
           ('signed-existing-priority',[-1,240],70,None),
           ('tile-allocation-zero',[],70,'item'),
           ('node-allocation-zero',[],70,'node')]
    for label,keys,priority,failure in modes:
        index=123;new_item,new_node=S+0x7000,S+0x7100
        args=[index,17,23,0x81234567,0x123401a6]
        c=Case(0x4bd740,label,REGION,args,cleanup=20,result=0xffffffff)
        c.block(REGION,28);add_chain(c,REGION,len(keys),priorities=keys)
        c.block(new_item,28,0xb6);c.block(new_node,16,0xc7)
        table=0xacf85c+index;c.block(table,1);c.put(table,priority,1)
        if failure:c.block(0,16,0x9a)
        c.start();c.endpoint('allocate',0x561a1d,1,'full32')
        item=0 if failure=='item'else new_item
        node=0 if failure=='node'else new_node
        c.event('allocate',[28],item)
        if item:
            for off,value in [(8,index),(12,17),(16,23),(20,0x81234567),(4,0),(0,REGION)]:c.put(item+off,value)
            c.put(item+24,0xa6,1)
        c.event('allocate',[16],node)
        if node:
            for off,value in [(4,0),(0,0),(12,item),(8,priority)]:c.put(node+off,value)
        head,tail=c.get(REGION+16),c.get(REGION+20)
        signed=lambda v:v if v<0x80000000 else v-0x100000000
        if not head or signed(c.get(head+8))>=priority:
            c.put(node,head)
            if head:c.put(head+4,node)
            c.put(node+4,0);c.put(REGION+16,node)
            if not tail:c.put(REGION+20,node)
        elif signed(c.get(tail+8))<=priority:
            c.put(node+4,tail);c.put(tail,node);c.put(node,0)
            c.put(REGION+20,node)
        else:
            current=head
            while signed(c.get(current+8))<priority:current=c.get(current)
            previous=c.get(current+4)
            c.put(node,current);c.put(node+4,previous)
            c.put(previous,node);c.put(current+4,node)
        c.put(item+4,node);c.put(REGION,c.get(REGION)+1)
        if failure:c.c['known_current_C_gap']='Active4BD740 calls abort() on this allocation failure; native fallthrough is authored without an abort endpoint.'
        out.append(c.end(item))
    return out


def world_reset_case():
    c=Case(0x4be0f0,'full-cell-clear-and-last-region-teardown',WORLD)
    c.block(WORLD,0x33000)
    for i in range(30):c.put(WORLD+0x32c64+i*28,0)
    last=WORLD+0x32c64+29*28
    add_chain(c,last,1)
    observe=[(WORLD,4),(WORLD+0x32c3c,4)]+[
        (r+i,4)for r in (WORLD+0x32c64,last)for i in range(0,28,4)]+[
        (NODE+i,4)for i in range(0,16,4)]+[(ITEM+4,4)]
    c.start(observe);c.c['budget']=500000;c.endpoint('delete',0x561a2b,1)
    c.write(WORLD,b'\0'*0x32c40)
    for i in range(30):
        r=WORLD+0x32c64+i*28;teardown(c,r)
        for off,value in [(4,0),(8,i+1),(12,WORLD),(24,0),(0,0)]:c.put(r+off,value)
    c.put(WORLD+0x32fac,30);c.put(WORLD+0x32fb0,0);c.put(WORLD+0x32fbc,0,1)
    return c.end()


DRAW=[('water-images',0x517690,4),('water-overlay',0x516f90,5),
      ('water-edges',0x5173a0,4),('terrain',0x4777c0,4),
      ('road',0x475cc0,5),('buildings',0x47b590,4),('actors',0x47bae0,5)]


def flood_cell_cases():
    out=[]
    for label,flooding,flags in [('land',0,0),('land-live-coordinates',0,0),
                                ('flood-road',1,0x1040),('receding-road',0,0x08000004)]:
        cell=333;terrain=0xaf598c+cell*4;building=0xb4e6fc+cell*2
        c=Case(0x4bd950,label,ITEM,[flooding],cleanup=4)
        c.block(ITEM,28);c.put(ITEM+8,cell);c.put(ITEM+12,17);c.put(ITEM+16,23);c.put(ITEM+24,0,1)
        for at,width,value in [(terrain,4,flags),(building,2,0),
                (0xb350dc+cell*2,2,0),(0xadc36c+cell,1,0xd4),(0xa5d4cc+cell,1,0xc5)]:
            c.block(at,width);c.put(at,value,width)
        c.start([(ITEM+12,4),(ITEM+16,4),(terrain,4),
                 (0xadc36c+cell,1),(0xa5d4cc+cell,1)])
        for token,address,arity in DRAW:c.endpoint(token,address,arity)
        c.endpoint('receding-area',0x47bd50,4)
        c.endpoint('monument-predicate',0x488030,1,'al8',4,native_receiver=0xbca350)
        if flooding:
            c.put(0xadc36c+cell,0,1);c.put(terrain,0x08000004)
            c.put(0xa5d4cc+cell,0,1)
        if flags&4:
            c.put(terrain,0x40)
            c.event('receding-area',[14,20,20,26])
        water=bool(flooding or flags&4)
        if water:
            c.event('water-images',[16,22,18,24])
            c.event('water-overlay',[16,22,18,24,0])
            c.event('water-edges',[16,22,18,24])
        inner=1 if flooding else (2 if water else 0)
        terrain_args=[17-inner,23-inner,17+inner+(not water),23+inner+(not water)]
        mutations=[(ITEM+12,4,100),(ITEM+16,4,200)]if label=='land-live-coordinates'else[]
        c.event('terrain',terrain_args,mutations=mutations)
        x,y=(100,200)if mutations else(17,23)
        middle=2 if water else 1;radius=3 if water and not flooding else 2
        c.event('road',[x-middle,y-middle,x+middle,y+middle,0])
        c.event('buildings',[x-radius,y-radius,x+radius,y+radius])
        c.event('actors',[x-radius,y-radius,x+radius,y+radius,0])
        if not flooding:c.event('monument-predicate',[0],0)
        if mutations:c.c['known_current_C_gap']='refresh() caches x/y across endpoints; native4BDF45/48,4BDF5E/61,4BDF79/7C reload the actual receiver after prior draw calls.'
        out.append(c.end())
    return out


def announcement_cases():
    out=[]
    specs=[('mode-not-five',0,1,20,0,None),('zero-BYTE-flag',5,0,20,0,None),
           ('quality-negative',5,1,-40,0,None),('category0',5,1,0,0,0x158),
           ('category1',5,1,1,0,0x157),('category1-WORD-nonzero',5,1,1,256,0x159),
           ('category2',5,1,21,0,0x156),('category2-WORD-nonzero',5,1,21,256,0x15a),
           ('category3',5,1,41,0,0x15b),('category4',5,1,61,0,0x15c),
           ('category5',5,1,81,0,0x15d),('quality-clamps100',5,1,101,0,0x15d)]
    for label,mode,flag,quality,special,message in specs:
        c=Case(0x4be640,label,GLOBAL,named=[])
        c.block(GLOBAL+0x32c40,36);c.put(GLOBAL+0x32c54,quality)
        c.block(GLOBAL+0x32fac,20);c.put(GLOBAL+0x32fb4,mode);c.put(GLOBAL+0x32fbc,flag,1)
        for at,width,value in [(0xeaac28,4,0),(0xea5ce8,2,0),(0x7848e2,2,special),(0xe950dc,4,0x778899aa)]:
            c.block(at,width);c.put(at,value,width)
        c.start([(GLOBAL+0x32c54,4),(0xe950dc,4),(0x7848e2,2)])
        c.endpoint('flood-message',0x4e0cf0,5)
        if mode==5 and flag:c.put(0xe950dc,1)
        if message is not None:c.event('flood-message',[message,0,0,0,0])
        out.append(c.end())
    return out


def irrigation_cases():
    out=[]
    for label,phase in [('empty',0),('overflow-bucket',30),('boundary-null-and-visited',28),
                        ('negative-signed-phase',0xffffffff),('live-phase-and-cached-next',0)]:
        c=Case(0x4beb90,label,WORLD)
        c.block(WORLD+0x32c00,0x400,0);c.put(WORLD+0x32fac,phase)
        nodes=[NODE+i*32 for i in range(4)];items=[ITEM+i*32 for i in range(4)];cells=[101,102,103,104]
        for n,o,cell in zip(nodes,items,cells):
            c.block(n,16,0);c.block(o,28,0);c.put(n+12,o);c.put(o+8,cell)
            c.block(0xaf598c+cell*4,4);c.put(0xaf598c+cell*4,0x100)
            c.block(0xae8e7c+cell,1,0)
        def bucket(index,node,count=1):
            at=WORLD+0x32c64+index*28;c.put(at,count);c.put(at+16,node)
        visits=[]
        if label=='overflow-bucket':bucket(29,nodes[0]);visits=[(101,())]
        elif label=='boundary-null-and-visited':
            bucket(28,nodes[0],0xffffffff);c.put(nodes[0],nodes[1]);c.put(nodes[1],nodes[2]);c.put(nodes[1]+12,0)
            c.put(0xae8e7c+101,1,1);bucket(29,nodes[3]);visits=[(104,())]
        elif label=='negative-signed-phase':
            for index,node in zip((-1,0,1),nodes):bucket(index,node)
            visits=[(101,()),(102,()),(103,())]
        elif label=='live-phase-and-cached-next':
            bucket(0,nodes[0]);c.put(nodes[0],nodes[1]);bucket(29,nodes[3])
            visits=[(101,[(WORLD+0x32fac,4,28),(nodes[0],4,0)]),(102,()),(104,())]
        c.start([(WORLD+0x32fac,4)]+[(n,4)for n in nodes]+[(n+12,4)for n in nodes])
        c.endpoint('propagate',0x468590,1)
        for cell,mutations in visits:c.event('propagate',[cell],mutations=mutations)
        out.append(c.end())
    return out


def irrigation_leaf_cases():
    out=[]
    for label,terrain,visited in [('eligible-mutates-live-tile',0x100,0),
                                  ('visited-BYTE',0x100,1),('other-terrain-bit',0x10000,0)]:
        cell=515;c=Case(0x4becc0,label,ITEM)
        c.block(ITEM,28);c.put(ITEM+8,cell)
        c.block(0xaf598c+cell*4,4);c.put(0xaf598c+cell*4,terrain)
        c.block(0xae8e7c+cell,1,visited)
        c.start([(ITEM+8,4),(0xaf598c+cell*4,4),(0xae8e7c+cell,1)])
        c.endpoint('propagate',0x468590,1)
        if terrain==0x100 and not visited:c.event('propagate',[cell],mutations=[(ITEM+8,4,516)])
        out.append(c.end())
    return out


def month_cases():
    out=[]
    for month in (0,1,12,13,0x80000000,0xffffffff,0x80000001):
        c=Case(0x4becf0,f'month-{month:08x}',GLOBAL,named=[],result=0xffffffff)
        c.block(GLOBAL+0x32fac,20);c.put(GLOBAL+0x32fb0,month)
        c.start();answer=(month-1)&0xffffffff
        if answer&0x80000000:answer=(answer+12)&0xffffffff
        out.append(c.end(answer))
    return out


def cases(registry=None):
    out=(teardown_cases()+insertion_cases()+flood_cell_cases()+[world_reset_case()]+
        announcement_cases()+irrigation_cases()+irrigation_leaf_cases()+month_cases())
    for c in out:
        if c['entry']==0x4be0f0:c['budget']=1000000
    if registry is None:return out
    present={int(m['address'],16):m for m in registry['members']}
    selected=[c for c in out if c['entry']in present]
    for c in selected:
        providers=present[c['entry']].get('providers')
        if providers:
            c['expected_invocations']=1+sum(bool(v.get('named'))+bool(v.get('canonical'))for v in providers.values())
    return selected


definitions=cases
