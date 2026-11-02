"""Actor renderer and scanner, composed with real sprite/clip algorithms, versus original x86."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ
from unicorn.x86_const import *
MASK=0xffffffff;PIXELS=0x31000000;FRAME=0x31100000;STOP=0x33000000;STACK=0x34000000
sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(PIXELS,0x10000),(FRAME,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
vm.mem_write(PIXELS,b'\x6b'*0x10000);vm.mem_write(FRAME,b'\xc7'*0x10000);vm.mem_protect(PIXELS,0x10000,UC_PROT_READ);vm.mem_protect(FRAME,0x10000,UC_PROT_READ)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
renderer=json.loads((ROOT/'metadata/lookup-sentinel-endpass-next-native.json').read_text())['functions']['00438e50']['assembly']['items']
scanner=json.loads((ROOT/'metadata/viewport-endpasses-next-native.json').read_text())['functions']['00439740']['assembly']['items']
sprite=json.loads((ROOT/'metadata/world-render-sprite-core-native.json').read_text())['functions']['0053e590']['assembly']['items']
main_instructions={int(i['address'],16) for i in renderer+scanner};sprite_instructions={int(i['address'],16) for i in sprite}
for i in renderer+scanner+sprite:
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
assert bytes(vm.mem_read(0x51f780,1))==b'\xc3'
assert bytes(vm.mem_read(0x4396b4,8))==bytes.fromhex('958e4300a98e4300')
assert [i+1 for i,v in enumerate(vm.mem_read(0x4396bc,109)) if v==0]==[1,11,12,13,43,44,45,54,55,56,82,83,84,99,102,103,104,106,109]
assert bytes(vm.mem_read(0x43972c,16))==bytes.fromhex('0c9643005e9643000000000100000000')
arities={0x438e50:4,0x4387d0:1,0x4f0d30:3,0x4cce00:7,0x53e590:3,0x4cdad0:0,0x4cdb80:0,0x51f780:2,0x40fb60:1,0x40fc40:1,0x40fcf0:1,0x40fdd0:1,0x40fed0:1,0x40ff50:1,0x40ffd0:1,0x410090:1,0x4100f0:2,0x4101c0:2,0x410230:1,0x4105c0:1,0x410660:1,0x410700:2,0x4109d0:2,0x410a70:2,0x410b10:1,0x410de0:1,0x410e80:1,0x410f70:1,0x410fb0:1,0x4114a0:1,0x411fd0:1,0x427a20:2,0x42a160:2,0x42b440:2,0x42c740:0,0x42c9a0:0,0x42cee0:0,0x42d810:0,0x42e680:0,0x4cdf20:2,0x4cdeb0:2}
real={0x438e50,0x53e590,0x4cdad0,0x4cdb80,0x51f780};colors={0x4100f0,0x4101c0,0x410700,0x4109d0,0x410a70}
watched=[0x70e47c,0x70e480,0x70e488,0x70e458,0x70e44c,0x70e464,0x70e460,0xe94e00,0x70e3e4,0x70e3e0,0xbca2e0,0x78d5ec,0x126517c,0xd3b5c0,0xe94e0c,0x978d78]
params=None;events=[];regions=[];mutation_seen=0;hits=set();sprite_hits=set();clip_hits=set();writes=set();mutated=set();descriptor_cache={};table_cache={};endpoint_hits=set()
def endpoint(cpu,a,n,user):
    global mutation_seen
    sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+i*4) for i in range(arities[a])];count=len(v)
    if a==0x4f0d30:
        assert v[1]!=v[2],'Coordinate output pointers alias';v=[v[0],read(v[1]),read(v[2])]
    if a in colors:v[1]&=65535
    self=0xf55bb0 if a==0x53e590 else 0
    if self:assert cpu.reg_read(UC_X86_REG_ECX)==self,'Native sprite receiver'
    actor=params[2]*388&MASK;ship=s16(params[35])*144&MASK
    e=[a,self,count]+v+[0]*(7-count)+[read(x) for x in watched]
    e += [read(0x70e120,2),read(0xbcd602+actor,1),read(0xbcd664+actor,1),read(0xbcd665+actor,1),read(0xbcd600+actor,2),read(0xbcd778+actor),read(0xbcd5fc+actor),read(0xbcba4c+ship),read(0xbcb9ca+ship,1),read(0xbcd603+actor,1),read(0x10d6a0e+params[7]*64,2),read(0x978d78),read(0x605b3d,1),read(0xbcd66f+actor,1),read(0xbcd609+actor,1),read(0xbcd718+actor,2),read(0xf56b1c),read(0xf56814)]
    assert len(e)==44;events.append(e);endpoint_hits.add(a)
    if a==params[69]:
        mutation_seen+=1
        if mutation_seen==params[70]:
            bits=params[71];mutated.add(a)
            if bits&1:write(0xbcd602+actor,params[72],1)
            if bits&2:write(0xbcd664+actor,params[73],1);write(0xbcd665+actor,params[74],1)
            if bits&4:write(0x70e47c,params[75]);write(0x70e480,params[76]);write(0x70e488,params[77])
            if bits&8:write(0x70e458,params[79]);write(0x70e44c,params[79]+1);write(0x70e460,params[80]);write(0x70e464,params[81])
            if bits&16:write(0xbcd600+actor,params[82],2);write(0xbcd66f+actor,params[89],1)
            if bits&32:write(0x126517c,params[78]);write(0x10d6a0e+params[7]*64,params[86],2);write(0xf56b1c,params[85]-5);write(0xf56814,params[85])
            if bits&64:write(0xbcd778+actor,params[91]);write(0xbcd5fc+actor,params[91]+1)
            if bits&128:write(0xbcba4c+ship,params[83]);write(0xbcd603+actor,params[84],1);write(0xbcb9ca+ship,params[84],1);write(0xbcba42+ship,params[84],2)
            if bits&256:write(0xbcd609+actor,7,1);write(0xbca2e0,4);write(0xe94e00,0x12345678);write(0x978d78,1)
            if bits&512:write(0x70e3e4,0x12345678);write(0x70e3e0,0x87654321);write(0x70e120,0x80ff,2)
    if a in real:return
    result=77
    if a==0x4387d0:result=params[45]&1
    if a==0x4f0d30:write(read(sp+8),params[46]);write(read(sp+12),params[47])
    cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def code(cpu,a,n,user):
    if a in main_instructions:hits.add(a);return
    if a in sprite_instructions:sprite_hits.add(a);return
    if 0x4cdad0<=a<=0x4cdb79 or 0x4cdb80<=a<=0x4cdc2d:clip_hits.add(a);return
    if a not in arities:raise RuntimeError('Unexpected guest instruction '+hex(a))
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a<STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
    writes.add((a,n))
vm.hook_add(UC_HOOK_CODE,code)
for a in arities:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
def native(p):
    global params,regions,mutation_seen
    params=p;events.clear();regions=[];mutation_seen=0;ids=[p[2],p[2]+1,p[2]+257]
    def region(a,n):
        a&=MASK
        if any(v==a for v,w in regions):return
        vm.mem_write(a,bytes([(p[1]+len(regions))&255])*n);regions.append((a,n))
    for a,n in [(0x5c8500,0x2300),(0x605b30,32),(0x6de510,16),(0x70e118,16),(0x70e3d0,0x775630-0x70e3d0),(0x78d5e0,32),(0x978d70,16),(0xb350dc-8,32),(0xbca2d0,48),(0xd3b520,224),(0xe38aa0,0x150),(0xe38d90,0x150),(0xe92e10,24),(0xe94df8,32),(0xf5680c,24),(0xf56b14,24),(0xf55bb0,8000),(0x10d69b0,4097*64+96),(0x1265178,16)]:region(a,n)
    for v in ids:region(0xbcd5f0+v*388,412)
    region(0xbcd5f0+s16(p[18])*388,412);ship=s16(p[35])*144&MASK;region(0xbcb9b8+ship,168)
    if p[1] not in table_cache:table_cache[p[1]]=bytes((i*37+p[1]*13)%255 for i in range(0x2300))
    vm.mem_write(0x5c8500,table_cache[p[1]])
    tags=b''.join(struct.pack('<II',0,100+i*4) for i in range(1000));vm.mem_write(0xf55bb0,tags)
    key=tuple(p[i] for i in [64,65,66,67,88,90])
    if key not in descriptor_cache:
        data=bytearray(4097*64)
        for i in range(4097):
            o=i*64;struct.pack_into('<III',data,o,p[88],0x100,p[90]);struct.pack_into('<I',data,o+16,1);struct.pack_into('<HH',data,o+20,p[65]&65535,p[66]&65535);struct.pack_into('<HH',data,o+34,i%23,i%17);data[o+0x32]=p[64]&255;data[o+0x33]=p[67]&255;data[o+0x35]=1
        descriptor_cache[key]=bytes(data)
    vm.mem_write(0x10d69f8,descriptor_cache[key])
    for a,v in [(0x10d6a1a+p[8]*64,p[29]),(0x10d6a1c+p[8]*64,p[30]),(0x10d6a1a+p[7]*64,p[31]),(0x10d6a1c+p[7]*64,p[32]),(0x10d6a0e+p[7]*64,p[33])]:write(a,v,2)
    for i,v in enumerate(ids):
        o=v*388;vm.mem_write(0xbcd5f8+o,bytes(388))
        for a,value,n in [(0xbcd5fc,p[8]+i,4),(0xbcd600,ids[i+1] if i<p[60] else 0,2),(0xbcd602,p[6]+i,1),(0xbcd603,p[44],1),(0xbcd604,p[11],1),(0xbcd609,p[9],1),(0xbcd60a,p[10],1),(0xbcd60c,p[41],2),(0xbcd60e,p[42],2),(0xbcd614,p[17],1),(0xbcd631,p[13],1),(0xbcd639,p[12],1),(0xbcd642,p[15],2),(0xbcd644,p[16],2),(0xbcd652,p[18],2),(0xbcd658,p[35],2),(0xbcd664,p[21],1),(0xbcd665,p[22],1),(0xbcd66f,p[61],1),(0xbcd670,p[14],1),(0xbcd718,p[23],2),(0xbcd778,p[7],4),(0xbcd5f9,p[43],1)]:write(a+o,value,n)
    o=s16(p[18])*388&MASK;write(0xbcd602+o,p[19],1);write(0xbcd670+o,p[20],1)
    for a,v,n in [(0xbcb9ca,p[34],1),(0xbcba42,p[38],2),(0xbcba4a,p[36],2),(0xbcba4c,p[37],4)]:write(a+ship,v,n)
    vm.mem_write(0x70e494,struct.pack('<I',0xfff0000)*(231*457))
    for i in range(8):write(0xb350dc+i*2,p[59] if i==p[58] else 0,2)
    row=(p[52]-8)&MASK;col=(p[53]-4)&MASK
    if row<457 and col<231:
        index=row*231+col
        if p[57]:write(0x70e494+index*4,p[58])
        if p[57]>=2:
            if col<230:write(0x70e494+(index+1)*4,p[58])
            if row<456:write(0x70e494+(index+231)*4,p[58])
    for a,v,n in [(0x978d78,p[24],4),(0xbca2e0,p[25],4),(0x126517c,p[26],4),(0x70e45c,p[27],4),(0x70e44c,p[28],4),(0xf56b1c,p[39],4),(0xf56814,p[40],4),(0x78d5ec,p[48],4),(0x605b3d,p[49],1),(0x70e460,p[50],4),(0x70e464,p[51],4),(0x70e474,p[52],4),(0x70e478,p[53],4),(0x70e48c,p[54],4),(0x70e490,p[55],4),(0x70e458,p[56],4),(0xe94e0c,p[62],4),(0x6de515,p[63],1),(0x70e120,p[68],2),(0xd3b55c,0,4),(0xd3b558,1024,4),(0xd3b554,0,4),(0xd3b550,768,4),(0xe38d9c,1024,4),(0xe38d98,768,4),(0xe38db0,1024,4),(0xe38dac,768,4),(0xe38d94,2048,4),(0xe92e18,FRAME+0x100,4),(0xe92e1c,FRAME+0x200,4),(0xe38dda,0,1)]:write(a,v,n)
    for repeat in range(p[87]):
        sp=STACK+0xf0000
        for i,v in enumerate([STOP,p[5]] if p[0] else [STOP,p[2],p[3],p[4],p[5]]):write(sp+i*4,v)
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start(0x439740 if p[0] else 0x438e50,STOP,count=3000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget';assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack';assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'events':events.copy()}
default=[0]*92
for k,v in {2:1,3:320,4:144,6:1,7:2100,8:2000,10:1,18:2,19:62,20:3,21:2,22:-3,26:4096,27:29,28:15,29:14,30:18,31:20,32:24,33:60,34:5,35:1,36:-1,38:10,39:2600,40:2700,43:1,44:4,45:1,46:300,47:400,49:1,50:-13,51:-6,52:8,53:4,54:144,55:320,56:58,57:1,58:1,59:1,61:1,64:1,65:58,66:60,70:1,72:14,73:-4,74:6,75:1,76:1,77:2,78:4096,79:60,80:-13,81:-6,83:1,84:16,85:2800,86:64,87:1,88:PIXELS+0x100,89:1,90:4,91:2200}.items():default[k]=v&MASK
cases=[]
def case(**kw):
    p=default.copy()
    if 'p2' in kw and 'p59' not in kw:kw['p59']=kw['p2']
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
case()
for kind,upper,moving,rotation in itertools.product(range(256),[0,2100],[0,1],[0,2,4,6]):case(p6=kind,p7=upper,p11=moving,p25=rotation)
for kind,facing,dy,rotation in itertools.product([4,38,9,1,2,58,13,56,45],[-128,-1,0,1,2,3,4,5,6,7,8,127],[-128,-1,0,127],[0,6]):case(p6=kind,p9=facing,p22=dy,p25=rotation)
for x,y,rotation in itertools.product([-32768,-16,-15,-14,-1,0,1,14,15,16,32767],[-32768,-1,0,14,15,32767],[0,2,4,6,1]):case(p6=60,p11=1,p15=x,p16=y,p25=rotation)
for kind,selection,predicate,owner,blocked in itertools.product([1,4,14,36,60,77,78,106],[0,1,2,9999],[0,1],[0,1],[0,1]):case(p6=kind,p5=selection,p45=predicate,p23=owner,p24=blocked)
for state,flag,meter,cargo in itertools.product([-32768,-1,0,32767],[0,1,0x100],[-32768,-4,-3,0,32767],[0,255]):case(p6=14,p36=state,p37=flag,p38=meter,p34=cargo)
for quantity,flag in itertools.product(range(256),[0,1]):case(p6=36,p44=quantity,p43=flag)
for x,y in itertools.product([-32768,-1,0,32767],repeat=2):case(p6=36,p41=x,p42=y)
for kind,field,value in itertools.product([4,14,36,77],[27,28,29,30,31,32,33],[0,1,255,256,32767,32768,65535]):case(**{'p6':kind,'p'+str(field):value})
for kind,x,y in itertools.product([1,4,14,36,60,77,78],[0,0x7fffffff,0x80000000,0xffffffff],[0,0x7fffffff,0x80000000,0xffffffff]):case(p6=kind,p3=x,p4=y)
for actor,owner,attached,attached_type in itertools.product([1,257,1000],[-1,0,1],[0,2,257],[61,62]):case(p2=actor,p23=owner,p18=attached,p19=attached_type,p6=60,p11=1)
for row,col,h,w in itertools.product([-1,0,1,456,457],[-1,0,230,231],[-15,-14,-13,-12],[-8,-7,-6,-5]):case(p0=1,p52=row+8,p53=col+4,p50=h,p51=w,p57=0)
for counter,flag in itertools.product([0,4,5,255,256,0x7fffffff,0x80000000,0xffffffff],[0,1,255]):case(p0=1,p48=counter,p49=flag,p50=-14)
for links,visible,head,selection in itertools.product([0,1,2],[0,1,255],[0,1,65535],[0,9999]):case(p0=1,p60=links,p61=visible,p59=head,p5=selection,p57=2,p50=-12,p51=-5)
for mode,special in itertools.product([0,1,51,0x1000033],[0,1]):case(p0=1,p62=mode,p63=special,p57=2,p50=-12,p51=-5)
for fmt,tint,transparent,count in itertools.product([0,1,2,30],[0,0x80ff],[0,1],[0,1,2000,4096]):case(p64=fmt,p68=tint,p67=transparent,p26=count)
mutation_fixtures={0x438e50:{},0x4387d0:{5:9999},0x4f0d30:{23:1},0x53e590:{6:14},0x4cdad0:{},0x4cdb80:{},0x4cce00:{6:36}}
for target,fixture in mutation_fixtures.items():
    for occurrence,bits in itertools.product([1,2,3],[1,2,4,8,16,32,64,128,256,512,1023]):
        kw={'p'+str(k):v for k,v in fixture.items()};kw.update(p69=target,p70=occurrence,p71=bits);case(**kw)
for target in [0x438e50,0x53e590,0x4387d0,0x4f0d30]:
    for bits in [2,4,8,16,32,64,128,1023]:case(p0=1,p5=9999 if target==0x4387d0 else 0,p23=1 if target==0x4f0d30 else 0,p60=2,p57=2,p50=-12,p51=-5,p69=target,p71=bits)
for repeat in [1,2]:case(p0=1,p60=2,p57=2,p50=-12,p51=-5,p87=repeat)
cases=list(dict.fromkeys(cases))
for fallback in [8,127]:case(p9=8,p10=fallback)
for flag in [0,1]:case(p6=14,p26=1,p37=flag)
case(p6=36,p26=1)
pixel_mutations={0x40fb60:{},0x40ffd0:{64:0},0x410090:{3:80,64:0},0x4100f0:{64:0,68:0x80ff},0x410700:{64:0,67:1,68:0x80ff},0x410230:{64:0,67:1},0x51f780:{64:30,65:26,66:14,33:14}}
for target,fixture in pixel_mutations.items():
    for occurrence,bits in itertools.product([1,2],[2,4,8,16,32,64,128,256,1023]):
        kw={'p'+str(k):v for k,v in fixture.items()};kw.update(p69=target,p70=occurrence,p71=bits);case(**kw)
mutation_fixtures.update(pixel_mutations)
for kind,bits in itertools.product([1,4,58,77,78],[2,8,32,64,128,1023]):
    case(p6=kind,p9=3 if kind==4 else 0,p22=0,p69=0x53e590,p71=bits)
case(p6=14,p37=1,p5=9999,p69=0x4387d0,p71=32,p78=1)
case(p6=4,p5=9999,p69=0x4387d0,p71=256)
for occurrence in [1,2]:case(p6=36,p69=0x53e590,p70=occurrence,p71=128,p84=16)
for bits in [32,128,160]:case(p6=14,p69=0x40fb60,p71=bits)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-actor-render/Release/actor_render_verify.exe';process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'start':args.start,'limit':args.limit,'single_case':bool(args.case)},
 'method':'Actual833-instruction actor renderer/scanner versus actual composed C renderer/scanner, real sprite dispatcher/both clip algorithms. Ordered endpoint arguments/receivers/state, watched fixture bytes/canaries, caller reloads/mutations and native/C stack/saved registers compared.',
 'limitations':['Predicate4387d0 is controlled with its independently proven Boolean domain; coordinate4f0d30 is controlled through real writable DWORD output arguments. The current separate coordinate algorithm remains outside this repair/proof.','Numeric formatting and final bitmap/RLE/terrain consumers are controlled; no city pixel-fidelity claim.','Loaded inline image fixtures only. Filesystem branches fail explicitly. Void EAX is not asserted.']}
checked=0;p=None;started=time.monotonic()
try:
    assert json.loads(process.stdout.readline())=={'fields':92}
    for p in cases:
        expected=native(p);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read();actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/actor_render_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected},indent=2)+'\n')
        assert actual==expected,repr({'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    dead={0x439331,0x439332,0x439335}
    if not(args.limit or args.start or args.case):assert main_instructions-hits<=dead,'Uncovered '+repr([hex(a) for a in sorted(main_instructions-hits)]);assert mutated==set(mutation_fixtures),'Unreached mutation targets'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
prefix='src' if args.production else 'artifacts/actor_render_phase'
sources=[prefix+'/unit_00561.c',prefix+'/unit_00562.c','include/actor_render.h','include/actor_predicate.h','include/sprite_render.h','include/sprite_libraries.h','include/restore_types.h','include/standalone_data.h','tests/actor_render.c','tests/actor_render_real_renderer.c','tests/actor_render_real_graphics.c','tests/actor_render_real_clips.c','tools/actor_render_project/CMakeLists.txt','tools/verify_actor_render_emulation.py','src/unit_02524.c','src/unit_01725.c','src/unit_01726.c','metadata/lookup-sentinel-endpass-next-native.json','metadata/viewport-endpasses-next-native.json','metadata/world-render-sprite-core-native.json']
dead={0x439331,0x439332,0x439335}
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(main_instructions)],native_reachable_instruction_coverage=[len(hits-dead),len(main_instructions-dead)],native_uncovered_instructions=[hex(a) for a in sorted(main_instructions-hits)],native_unreachable_reason='439321 clearsECX;439323 loads an unsigned byte intoCL;439329 ANDs80000001. Sign is always clear, so43932f JNS always skips439331/332/335.',sprite_dispatch_instruction_coverage=[len(sprite_hits),len(sprite_instructions)],clip_executed_instruction_count=len(clip_hits),mutated_endpoint_addresses=[hex(a) for a in sorted(mutated)],ordered_endpoint_addresses=[hex(a) for a in sorted(endpoint_hits)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/actor-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
