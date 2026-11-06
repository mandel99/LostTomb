"""Connected player/record contracts, authored from pinned native instructions."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase
PLAYER=0xeaac28
STRIDE=0x49d8
S=0x32000000
TARGETS=(0x416990,0x41d410,0x420550,0x420600,0x4269b0,0x426b00,0x446260,0x446650,0x4474e0,0x44d520)


def signed(value,bits=32):
    value&=(1<<bits)-1
    return value-(1<<bits)if value&(1<<(bits-1))else value


def bank(address,player):return(address+(player*STRIDE&0xffffffff))&0xffffffff


class Case(BaseCase):
    def __init__(self,entry,label,args=(),mask=None):
        super().__init__(entry,label,0x89abcdef,arguments=args,named=list(args),result=mask)
        self.c['id']=f'c8-player10/{entry:08x}/'+label
        self.c.pop('expected_invocations')
        self.c.update(required_retained_route='canonical',budget=2000000,
            initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'],
            compare_x87_values=True,compare_x87_control=True)

    def cell(self,address,value,width=4):self.block(address,width);self.put(address,value,width)

    def banks(self,address,size=4,numbers=(0,1,2,4)):
        for number in numbers:self.block(address+number*STRIDE-4,size+8)

    def begin(self):
        self.start([(address+offset,4)for address,data in self.mem.items()for offset in range(0,min(len(data),32)//4*4,4)])


def mode_cases():
    out=[]
    for label,state,selected,value,player in[
        ('same-high-DWORD-clears-one-bank-BYTE',7,0x12340100,0x12340100,1),
        ('different-DWORD-stores-full-mode',7,0x76543210,0x12345678,1),
        ('mode1-full-parameter',1,0,0x80000001,0),
        ('mode30-normalizes',30,0,0x100,0),
        ('unhandled-mode-preserves-all',5,0,0x12345678,1),
        ('wrapped-DWORD-player',7,0x11223344,0x11223344,0x20000001)]:
        c=Case(0x416990,label,[value]);c.cell(PLAYER,player)
        c.cell(0xe38dda,state,1);c.cell(0xe38dca,0x6a,1)
        c.cell(0x605a44,selected);c.cell(0xd3b198,0xabcdef12);c.banks(0xea623c)
        c.begin();c.endpoint('action',0x40a0e0,0)
        if state==7 and selected==value:
            c.put(0xe38dda,1,1);c.put(0xe38dca,1,1);c.put(bank(0xea623c,player),0,1)
        elif state in(1,30)or state==7:
            if state==30:c.put(0xe38dda,1,1)
            c.put(0xd3b198,value);c.event('action',[])
        out.append(c.end())
    return out


def statistic_cases():
    out=[]
    for label,kind,blocked,fallback,code in[
        ('active-BYTE-and-WORD-type',31,0,1,3),('blocked-player-bank',31,1,1,3),
        ('signed-fallback-negative',30,0,-1,3),('high-WORD-type',226,0,1,3),
        ('ordinary-type',5,0,0,2)]:
        id_=2;player=1;arg=3
        c=Case(0x41d410,label,[id_,arg]);c.cell(PLAYER,player)
        c.block(0x7999d8+id_*264-4,84);c.put(0x7999d8+id_*264,1,1)
        c.put(0x7999e8+id_*264,kind,2);c.put(0x799a18+id_*264,fallback,2)
        c.cell(0x57c780+kind*4,code);c.banks(0xea5ea4);c.put(bank(0xea5ea4,player),blocked)
        c.block(0x702900+code*128-4,128+8)
        c.put(0x702904+code*128,0xffffffff);c.put(0x70290c+code*128+arg*4,0x7fffffff)
        c.begin()
        if kind not in(30,31,32,33,226)or(blocked<=0 and fallback>0):
            c.put(0x702900+code*128,1);c.put(0x702904+code*128,0)
            c.put(0x70290c+code*128+arg*4,0x80000000)
        out.append(c.end())
    return out


def seed_table(c,player,threshold):
    c.cell(PLAYER,player);c.cell(bank(0xea5baa,player),threshold,2)
    c.block(0x5c2748,22*16)
    c.block(S+0x1000,22*32)
    for i in range(22):
        c.put(0x5c2748+i*16,i-2);c.put(0x5c274c+i*16,79 if i%2==0 else 80)
        c.put(0x5c2750+i*16,S+0x1000+i*32)
        c.put(S+0x1000+i*32+18,1,2);c.put(S+0x1000+i*32+12,0)


def table_cases():
    out=[]
    for label,index,flag,quota,threshold,wanted in[
        ('first-match',0,0,1,-2,1),('last-match',21,0,0x12345678,19,1),
        ('nonzero-high-WORD-rejects',0,0x100,1,-2,0),
        ('nonpositive-DWORD-quota',0,0,0x80000000,-2,0),
        ('signed-country-mismatch',0,0,1,2,0)]:
        c=Case(0x420550,label,[79 if index%2==0 else 80],0xff)
        seed_table(c,1,threshold);c.put(S+0x1000+index*32+18,flag,2)
        c.put(S+0x1000+index*32+12,quota);c.begin();out.append(c.end(wanted))
    for label,threshold,alias in [('normal-counts',5,False),('signed-country',-1,False),('real-output-alias',5,True)]:
        outputs=[S+0x3000+i*4 for i in range(6)]
        if alias:outputs[0]=outputs[4]
        c=Case(0x420600,label,outputs);seed_table(c,0x20000001,threshold)
        c.block(S+0x3000-4,32);c.begin()
        first=sum(i-2<threshold for i in range(22)if i%2==0)
        second=sum(i-2<threshold for i in range(22)if i%2)
        values=[first*100//11,second*100//11,11,first,11,second]
        if alias:values[1]=second*100//values[0];values[4]=values[0]
        for address,value in zip(outputs,values):c.put(address,value)
        out.append(c.end())
    return out


def rating_cases():
    out=[]
    for label,a,b,counter,mutate in[
        ('full-values-clamp100',300,5,0x89abcdef,False),
        ('signed-negative-clamp0',-300,10,0x12345678,False),
        ('wrapped-addition-clamp0',0x7fffffff,1,0xfedcba98,False),
        ('normal50',20,30,700,False),('callbacks-reload-player-and-values',1,2,3,True)]:
        c=Case(0x4269b0,label);c.cell(PLAYER,1)
        for at,value in[(0x5c9348,a),(0x705734,b),(0x705738,counter)]:c.cell(at,value)
        c.banks(0xea5b3c);c.banks(0xea5ca4);c.begin()
        c.endpoint('first',0x426a00,0);c.endpoint('second',0x426a80,0)
        c.event('first',[],mutations=[(0x5c9348,4,60)]if mutate else[])
        c.event('second',[],mutations=[(PLAYER,4,2),(0x705734,4,3),(0x705738,4,0x87654321)]if mutate else[])
        value=signed(c.get(0x5c9348)+c.get(0x705734))
        player=c.get(PLAYER);c.put(bank(0xea5b3c,player),min(100,max(0,value)))
        c.put(bank(0xea5ca4,player),c.get(0x705738));out.append(c.end())
    for player in(1,0x20000001):
        c=Case(0x426b00,f'full-counter-player-{player:08x}');c.cell(PLAYER,player)
        c.cell(0x705738,0xfedcba98);c.banks(0xea5ca4);c.begin()
        c.put(bank(0xea5ca4,player),0xfedcba98);out.append(c.end())
    return out


def fee_cases():
    out=[]
    for label,cost,available,flag,charge,result in[
        ('row256-and-DWORD-count',50,30,0,100,0),
        ('forced-flag-clears-correct-bank',100,10,0x80000000,0,1),
        ('percentage5',95,100,0,70,1),('percentage10',90,100,0,50,1),
        ('percentage25',75,100,0,25,1),('percentage50',50,100,0,15,1),
        ('percentage75',25,100,0,10,1),('percentage100',0,100,0,5,1),
        ('percentage149',-99,200,0,5,1),('percentage150',-100,200,0,0,1)]:
        row=256;player=1;c=Case(0x446260,label,[row],0xffffffff)
        c.cell(PLAYER,player);c.cell(0x785d70,257)
        c.block(0x785d74+row*124-4,124+8);c.put(0x785d74+row*124+12,cost,2)
        c.banks(0xea61fc);c.put(bank(0xea61fc,player),flag);c.begin()
        c.endpoint('available',0x4b8360,3,'full32');c.endpoint('charge',0x4b8890,2)
        c.event('available',[row,0,0],available)
        if flag:c.put(bank(0xea61fc,player),0)
        c.event('charge',[charge,row]);c.c['actual_children']=['004CDE60 actual signed percentage']if not flag and result else[]
        out.append(c.end(result))
    return out


def formation_cases():
    out=[]
    for id_,kind,player,value in[(0,5,1,0xffffffff),(1,7,1,0x7fffffff),(2,2,0x20000001,17)]:
        c=Case(0x446650,f'formation-{id_}-descriptor-{kind}',[id_]);c.cell(PLAYER,player)
        c.block(0xbcd5f8,4096);offset=id_*388;c.put(0xbcd602+offset,kind,1)
        c.block(0x7090d0+kind*56,52)
        fields=[(0,0x12345678),(4,789),(28,0x89abcdef),(36,0x13572468),(44,0x24681357),(48,0xcafebabe)]
        for at,v in fields:c.put(0x7090d0+kind*56+at,v)
        c.banks(0xea6210);c.put(bank(0xea6210,player),value);c.begin()
        for at,v,width in[(0xbcd71c,0,4),(0xbcd720,fields[0][1],4),(0xbcd724,fields[0][1],4),
            (0xbcd728,0,1),(0xbcd730,0,4),(0xbcd72c,0,4),(0xbcd734,fields[2][1],4),
            (0xbcd738,fields[3][1],4),(0xbcd73c,fields[4][1],4),(0xbcd740,fields[5][1],4),
            (0xbcd748,789,4),(0xbcd74c,0,1),(0xbcd750,157,4)]:c.put(at+offset,v,width)
        c.put(bank(0xea6210,player),value+1);c.c['actual_children']=['004CDE40 actual wrapped signed product/100']
        out.append(c.end())
    for player in(1,0x20000001):
        c=Case(0x4474e0,f'rollover-player-{player:08x}');c.cell(PLAYER,player)
        c.cell(0x78d668,0x89abcdef);c.cell(0x78d66c,0x12345678);c.banks(0xea5fdc,24)
        for off,value in[(8,0xfedcba98),(20,0x76543210)]:c.put(bank(0xea5fdc+off,player),value)
        c.begin()
        for off,value in[(0,0x12345678),(4,0xfedcba98),(8,0),(12,0x89abcdef),(16,0x76543210),(20,0)]:c.put(bank(0xea5fdc+off,player),value)
        out.append(c.end())
    return out


def town_cases():
    out=[];record=S+0x6000;output=S+0x6100
    scenarios=[('money-insufficient',36,500,500,0,{},-1),('money-sufficient',36,501,500,0,{},258),
        ('troop-needs-first-pair',37,0,1,2,{},-6),('troop-needs-second-pair',37,0,1,2,{0xea5ed8:0x80},-5),
        ('troop-active-pairs',37,0,1,2,{0xea5ed8:0x80,0xea5ed9:1},-2),
        ('troop-no-town-permission',37,0,1,3,{},-4),('troop-no-transport',37,0,1,3,{0xea5edb:1},-3),
        ('troop-transport-present',37,0,1,3,{0xea5edb:1,0xea5ed6:0xfe},-2),
        ('resource-insufficient',5,9999,100,0,{},-1),('resource-exact-threshold',5,10000,100,0,{},258)]
    for label,kind,wealth,cost,status,flags,result in scenarios:
        player=1;c=Case(0x44d520,label,[0,output],0xffffffff);c.cell(PLAYER,player)
        c.block(record,124);c.put(record,257,2);c.put(record+4,kind,2);c.put(record+12,cost,2);c.put(record+28,1,2)
        c.cell(output,0xabcdef12);c.banks(0xea187c,numbers=(0,1,4));c.banks(0xea5ed0,0x340);c.block(0x7770f0,0x600)
        c.put(0x7770f0+324,status,1);c.put(bank(0xea187c,player),wealth)
        c.put(bank(0xea6160+kind*4,player),wealth)
        for at in(0xea5ed5,0xea5ed6,0xea5ed8,0xea5ed9,0xea5eda,0xea5edb):c.put(bank(at,player),flags.get(at,0),1)
        c.begin();c.endpoint('cursor',0x44baf0,1,'full32',native_cleanup=4,native_receiver=0x785d70)
        c.endpoint('eligible',0x44d4c0,0,'al8',native_receiver=record)
        c.eps['eligible'].update(native_arity=0,compiled_arity=1,compiled_receiver_arg=0)
        c.event('cursor',[0],record);c.event('eligible',[],1);c.put(output,257)
        out.append(c.end(result))
    return out


def cases(registry=None):
    out=mode_cases()+statistic_cases()+table_cases()+rating_cases()+fee_cases()+formation_cases()+town_cases()
    if registry is None:return out
    members={int(m['address'],16):m for m in registry['members']}
    out=[c for c in out if c['entry']in members]
    for c in out:c['expected_invocations']=1+sum(bool(p['canonical'])for p in members[c['entry']]['providers'].values())
    return out
