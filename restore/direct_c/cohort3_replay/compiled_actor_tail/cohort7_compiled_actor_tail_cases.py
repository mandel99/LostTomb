"""Bounded actor/city/monument recipes; no image execution at import."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase

TARGETS=(0x4e7300,0x4e81d0,0x4e8650,0x4e8720,0x4ec4a0,0x4f5050,0x4f53d0,0x542ee0)
BUILDINGS,BUILDING_BYTES=0x7999d8,4000*264
DELTAS=(-228,-227,1,229,228,227,-1,-229)
XY=((0,-1),(1,-1),(1,0),(1,1),(0,1),(-1,1),(-1,0),(-1,-1))
ROLE_X=['stack-output','frog-x'];ROLE_Y=['stack-output','frog-y']


class Case(BaseCase):
    def __init__(self,entry,label,args=(),result=None):
        super().__init__(entry,label,0x89abcdef,args,named=args,result=result)
        self.c['id']=f'c7-compiled-actor-tail/{entry:08x}/'+label
        self.c.update(compare_x87_values=True,compare_x87_control=True,
            initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'])

    def event(self,token,args,value=0,mutations=(),**payload):
        c=self.c;spec=self.eps[token]
        c['authored_endpoint_order'].append(token)
        c['authored_endpoint_observations'].append([[a,n,self.get(a,n)]for a,n in self.observe])
        c['expected_endpoint_counts'][token]+=1
        c['expected_endpoint_arguments'][token].append([a&0xffffffff if isinstance(a,int)else a for a in args])
        response=dict(value=value,**payload)
        if mutations:response['mutations']=list(mutations)
        spec['responses'].append(response)
        for at,width,new in mutations:self.put(at,new,width)


def add_buildings(c):c.block(BUILDINGS,BUILDING_BYTES,0)
def building(c,index):return BUILDINGS+index*264
def age_base(player):return 0xea3e34+player*0x49d8


def add_random(c,index,left,right):
    c.block(0xd3b1c4,404,0xb6);c.put(0xd3b1c4,index)
    c.block(0xd3b364,36,0xc7)
    c.put(0xd3b374,0x91234567);c.put(0xd3b380,right);c.put(0xd3b384,left)


def random_advance(c,movement=False):
    index=c.get(0xd3b1c4);sample=c.get(0xd3b374)
    c.put((0xd3b1c8+index*4)&0xffffffff,sample)
    next_=(index+1)&0xffffffff
    c.put(0xd3b1c4,0 if next_<0x80000000 and next_>=100 else next_)
    left,right=c.get(0xd3b384),c.get(0xd3b380)
    for _ in range(31):
        left=(left>>1)|(((left^(left>>4))&1)<<30)
        right=(right>>1)|(((right^(right>>4))&1)<<30)
    for at,value in [(0xd3b384,left),(0xd3b378,left&7),(0xd3b380,right),
        (0xd3b364,right&32767),(0xd3b370,left&32767),(0xd3b368,right&127),
        (0xd3b36c,right&7),(0xd3b374,left&127)]:c.put(at,value)
    return left,right


def frog_cases():
    out=[]
    for label,index,left,right,search in [
        ('gate-reject-rollover',99,0x13579bdf,0x2468ace0,None),
        ('gate-reject-index-alias',0xffffffff,0x12345678,0xf2345678,None),
        ('search-declared-failure',0,0,1,0),
        ('search-output-alias-then-spawn',5,1,0xffffffff,1)]:
        c=Case(0x4e7300,label,[73]);add_random(c,index,left,right)
        c.start([(0xd3b1c4,4),(0xd3b374,4),(0xd3b380,4),(0xd3b384,4)])
        c.endpoint('frog-target',0x4e75a0,3,'full32',compiled_symbol='restore_god_effect_004e75a0')
        c.eps['frog-target']['compiled_symbols']=['restore_god_effect_004e75a0','FUN_004e75a0']
        c.eps['frog-target']['pointer_args']={1:'frog-x',2:'frog-y'}
        c.endpoint('frog-spawn',0x4e7410,3,compiled_symbol='restore_god_effect_004e7410')
        c.eps['frog-spawn']['compiled_symbols']=['restore_god_effect_004e7410','FUN_004e7410']
        advanced,_=random_advance(c)
        assert ((advanced&32767)%100<50)==(search is not None)
        if search is not None:
            writes=[(1,4,0xfffffff7),(2,4,257)]if search else[]
            c.event('frog-target',[73,ROLE_X,ROLE_Y],search,writes_args=writes)
            if search:c.event('frog-spawn',[73,0xfffffff7,257])
        out.append(c.end())
    return out


def health_cases():
    out=[]
    for label in ('signed-BYTE-skip-quarantine-fixed-last-id','skip-BYTE-one',
                  'DWORD-pending-full-empty-scan','lower-clamp-empty-scan',
                  'upper-clamp-empty-scan','message-reloads-live-player'):
        c=Case(0x4e81d0,label);add_buildings(c)
        c.block(0xeaac28,8,0xa5);c.put(0xeaac28,0)
        c.block(0xe38ea8,24,0xa5);c.put(0xe38ea8,0xff if label.startswith('signed')else(1 if label.startswith('skip')else 2),1)
        c.put(0xe38ebc,0x100 if label=='DWORD-pending-full-empty-scan'else 0)
        if label=='DWORD-pending-full-empty-scan':c.put(0xe38ea8,0,1)
        c.block(0x78d790,4,0)
        cities=(0,2)if label=='message-reloads-live-player'else(0,)
        for city in cities:
            off=city*0x49d8
            for at,width,value in [(0xea5fc0+off,4,0x91234567),(0xea1884+off,4,0x81234567),
                (0xea1888+off,4,150 if city else (0xfffffff6 if label.startswith('lower')else(200 if label.startswith('upper')else(11 if label.startswith('message')else 0)))),
                (0xea1894+off,4,0x7fffffff),(0xea5ce8+off,2,0)]:
                c.block(at,width);c.put(at,value,width)
        skip=label.startswith('signed')or label.startswith('skip')
        if label.startswith('signed'):
            for id_,state,house,time in [(1,1,1,2),(3999,1,1,1),(2,1,0,1),(3,0,1,1)]:
                b=building(c,id_);c.put(b,state,1);c.put(b+5,house,1);c.put(b+135,1,1);c.put(b+240,time)
        c.start([(0xeaac28,4),(0xea1884,4),(0xea1888,4),(0xea5fc0,4)])
        c.c['budget']=500000
        c.endpoint('health-event-gate',0x42fbb0,0,'al8')
        c.endpoint('health-message',0x4e2970,1)
        c.endpoint('disease-latency',0x430250,0)
        c.endpoint('disease-trigger',0x4302b0,0)
        c.put(0xea5fc0,0)
        if skip:
            c.put(0xea1884,75);c.put(0xea1888,75)
        else:
            c.put(0xeaac2c,0,1);c.put(0xea1884,0)
            c.event('health-event-gate',[],0)
            old=c.get(0xea1888);signed=old if old<0x80000000 else old-0x100000000
            value=signed+2 if signed<0 else signed-2 if signed>0 else 0
            if signed<0 and value>0:value=0
            c.put(0xea1888,value)
            if label=='message-reloads-live-player':
                c.event('health-message',[0x5f],mutations=[(0xeaac28,4,2)])
                c.put(0xea1888+2*0x49d8,100)
            else:c.put(0xea1888,min(100,max(0,value)))
        if label.startswith('signed'):
            c.put(building(c,1)+240,1)
            c.put(building(c,3999)+240,0);c.put(building(c,3999)+135,0,1)
        if not skip:
            c.event('disease-latency',[]);c.event('disease-trigger',[])
        out.append(c.end())
    return out


def demographic_cases(entry):
    out=[];death=entry==0x4e8650
    for label in ('zero-bins-original-rates','signed-bins-original-rates','one-real-house'):
        player=2 if label=='signed-bins-original-rates'else 0;off=player*0x49d8;age=age_base(player)
        c=Case(entry,label);add_buildings(c)
        c.block(0xeaac28,4);c.put(0xeaac28,player)
        c.block(age,0x160,0xa5)
        for i in range(100):c.put(age+i*2,0,2)
        c.block(0xea1888+off,4,0)
        rate_base=0x5e49d4 if death else 0x5e49ac
        total=0xea3f50+off if death else 0xea3f4c+off
        c.put(total,0x99887766)
        cursor=0xea3f8c+off if death else 0xea3f88+off
        c.put(cursor,0xa5a50001 if death else 1)
        if label=='signed-bins-original-rates':
            c.put(age,0x8000,2);c.put(age+198,0xffff,2)
        if label=='one-real-house':
            c.put(age+(89 if death else 20)*2,100,2)
            h=building(c,2)
            c.put(h,1,1);c.put(h+5,1,1);c.put(h+4,0,1)
            c.put(h+28,5,2);c.put(h+30,5,2);c.put(h+32,0 if death else 1,2)
            c.put(h+18,1,2)
            if not death:c.block(0x70ce14+88,4);c.put(0x70ce14+88,10)
        c.start([(0xeaac28,4),(total,4),(cursor,4),(age,4),(age+180,4)])
        c.c['budget']=1000000
        if death:
            old=bytes(c.mem[age][:200])
            for i in range(99,0,-1):c.write(age+i*2,old[(i-1)*2:i*2])
            c.put(age,0,2)
        c.put(total,0)
        if label=='one-real-house':
            h=building(c,2)
            if death:
                c.put(h+28,0,2);c.put(cursor,2,2);c.put(age+180,95,2);c.put(total,5)
            else:
                c.put(h+28,6,2);c.put(h+30,4,2);c.put(cursor,2);c.put(age,1,2);c.put(total,1)
        out.append(c.end())
    return out


def command_cases():
    out=[];actor=7;row=0xbcd5f8+actor*388
    modes=[('state26',26,0,0,0,None),('state18-random',18,99,0x13579bdf,0x2468ace0,None),
           ('state18-index-alias',18,0xffffffff,0x12345678,0xf2345678,None),
           ('state19-first-direction',19,0,0,1,0),('state19-last-direction',19,5,1,0xffffffff,7),
           ('full-DWORD-state-default',0x100001a,0,0,0,None)]
    for label,state,index,left,right,last in modes:
        c=Case(0x4ec4a0,label,[actor,state],0xffffffff);c.block(row,388,0xa5)
        c.put(row+20,0x7fff,2);c.put(row+22,0x8000,2);c.put(row+32,1000)
        if state in(18,19):add_random(c,index,left,right)
        if state==19:
            c.block(0xaf598c+(1000-512)*4,4096,0)
            future=left
            for _ in range(31):future=(future>>1)|(((future^(future>>4))&1)<<30)
            direction=((future&7)+last)%8
            for d,delta in enumerate(DELTAS):c.put(0xaf598c+(1000+delta)*4,4 if d==direction else 0x80004)
        c.start([(row+56,1),(row+54,2),(row+324,4),(row+328,4)])
        c.endpoint('seek-other-kind',0x4ec990,1,'full32',compiled_symbol='restore_actor_seek_other_kind')
        c.eps['seek-other-kind']['compiled_symbols']=['restore_actor_seek_other_kind','FUN_004ec990']
        c.put(row+56,state,1)
        if state==26:
            c.put(row+324,12);c.put(row+54,20,2);result=0
        elif state==18:
            advanced,_=random_advance(c)
            c.put(row+54,(advanced&32767)%100+250,2);c.put(row+324,12)
            result=(advanced&32767)//100
        elif state==19:
            advanced,_=random_advance(c,True);c.put(row+324,12)
            direction=((advanced&7)+last)%8;dx,dy=XY[direction]
            c.put(row+36,(0x7fff+dx)&65535,2);c.put(row+38,(0x8000+dy)&65535,2)
            result=(0x8000+dy)&65535
        else:
            result=0x81234567;c.event('seek-other-kind',[actor],result)
            c.put(row+324,7);c.put(row+54,200,2)
        c.put(row+328,0);out.append(c.end(result))
    return out


def material_cases():
    out=[]
    modes=[('no-match',77,None,0,None),('signed-WORD-match',0xffffffff,6,0,None),
        ('first-material',77,6,0,None),('second-material',77,8,0,None),('third-material',77,10,0,None),
        ('active-incomplete',77,6,0,('cf',99)),('active-complete',77,6,0,('cf',100)),
        ('active-force',77,6,1,('cf',100)),('force-low-BYTE-only',77,6,0x100,('cf',100)),
        ('D2-negative-WORD',77,6,0,('d2',0xffff)),('D2-complete',77,6,0,('d2',16)),
        ('active-without-building',77,6,1,('none',0))]
    for label,resource,field,force,active in modes:
        c=Case(0x4f5050,label,[resource,force],255)
        c.block(0xeaac28,4,0);c.block(0xea5fa0,24,0);c.block(0x784de0,6,0)
        c.block(0x5d19c8,5*16,0)
        for i in range(3):c.put(0x784de0+i*2,i+1,2)
        for i in range(1,5):
            for f in(6,8,10):c.put(0x5d19c8+i*16+f,1,2)
        record=0x5d19c8+(4 if active else 1)*16
        if field:c.put(record+field,resource,2)
        expected=bool(field)
        if active:
            tag,value=active;c.put(0xea5fa0,4);c.put(0xea5fac,0 if tag=='none'else 2)
            b=BUILDINGS+2*264;c.block(b,264,0)
            c.put(b+16,0xd2 if tag=='d2'else 0xcf,2)
            c.put(b+0x55,value,1)
            if tag=='d2':c.block(0xea5baa,2);c.put(0xea5baa,value,2)
            incomplete=(value if value<32768 else value-65536)<16 if tag=='d2'else value!=100
            expected=tag!='none'and(incomplete or bool(force&255))
        c.start();out.append(c.end(int(expected)))
    return out


def pyramid_gate_cases():
    out=[];id_=2;b=BUILDINGS+id_*264
    for label,progress,classify,blocked in [('BYTE-progress-early-reject',2,0,0),
        ('real-16-row-skip-query-classifier-false',1,0,0),
        ('real-query-signed-coordinates-clear',1,1,0),('real-query-DWORD-blocked',1,1,0x100)]:
        c=Case(0x4f53d0,label,[id_],255);c.block(b,16*264,0)
        for row in range(16):
            c.put(b+row*264+0x55,1,1);c.put(b+row*264+0x56,1,1)
        c.put(b+0x55,progress,1);c.put(b+0x54,0,1);c.put(b+18,0,2);c.put(b+12,1000)
        for at,width,value in [(0xeaaf58,4,0x91234567),(0xeaae90,4,0),(0x5e4ef4,4,0),
            (0xbca2d8,4,1229)]:
            c.block(at,width);c.put(at,value,width)
        c.start([(0xeaaf58,4),(0xeaae90,4),(b+0x54,1),(b+0x55,1)])
        c.endpoint('classify-category',0x4ecea0,1,'al8')
        c.endpoint('validate-pyramid-cells',0x4ed360,4)
        if progress<=1:
            c.put(0xeaaf58,0)
            c.event('classify-category',[0],classify)
            if classify:c.event('validate-pyramid-cells',[0xffffffff,0xffffffff,3,0],mutations=[(0xeaae90,4,blocked)])
        out.append(c.end(int(progress<=1 and not blocked)))
    return out


def ready_cases():
    out=[]
    for label,incoming,candidates,responses in [
        ('no-candidate-preserves-live-high24',0xa5b6c7d8,[],[]),
        ('no-candidate-distinct-live-EAX',0xf1234567,[],[]),
        ('first-complete',0x81234567,[1],[100]),
        ('skip99-after-incomplete',0xfedcba98,[1,2,101],[99,100]),
        ('full-DWORD-progress-not-AL100',0x12345678,[3999],[0x12340064])]:
        c=Case(0x542ee0,label,result=0xffffffff);add_buildings(c)
        c.c.update(initial_eax=incoming,named_arguments=[incoming],canonical_arguments=[],
            routes_reason='Named helper explicitly receives actual incoming EAX; logical native consumer observes AL. Full physical EAX is compared only with that real live input. No canonical FUN survives current MAPs.')
        for id_ in candidates:
            b=building(c,id_);c.put(b,1,1);c.put(b+16,213,2)
        c.start([(BUILDINGS+264,1),(BUILDINGS+264+16,2)])
        c.endpoint('pyramid-progress',0x542ad0,2,'full32')
        eax=incoming
        visited=[]if not candidates else([1,101]if label.startswith('skip99')else[candidates[0]])
        for id_,value in zip(visited,responses):
            c.event('pyramid-progress',[id_,0],value);eax=value
            if value==100:break
        expected=(eax&0xffffff00)|(1 if responses and responses[-1]==100 else 0)
        out.append(c.end(expected))
    return out


def cases(registry=None):
    out=(frog_cases()+health_cases()+demographic_cases(0x4e8650)+demographic_cases(0x4e8720)+
         command_cases()+material_cases()+pyramid_gate_cases()+ready_cases())
    if registry is None:return out
    present={int(m['address'],16):m for m in registry['members']}
    selected=[c for c in out if c['entry']in present]
    for c in selected:
        providers=present[c['entry']].get('providers')
        if providers:c['expected_invocations']=1+sum(bool(v.get('named'))+bool(v.get('canonical'))for v in providers.values())
    return selected


definitions=cases
