"""Coherent normal/overlay ground passes and preview wrapper versus original x86."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--start',type=int,default=0);parser.add_argument('--limit',type=int);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ
from unicorn.x86_const import *
MASK=0xffffffff;PIXELS=0x31000000;FRAME=0x31100000;STOP=0x33000000;STACK=0x34000000
sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(image,pe+6)):
    q=table+k*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
for a,n in [(PIXELS,0x10000),(FRAME,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
vm.mem_write(PIXELS,b'\x6b'*0x10000);vm.mem_write(FRAME,b'\xc7'*0x10000);vm.mem_protect(PIXELS,0x10000,UC_PROT_READ);vm.mem_protect(FRAME,0x10000,UC_PROT_READ)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
ground=json.loads((ROOT/'metadata/ground-render-next-native.json').read_text())['functions']
helper=json.loads((ROOT/'metadata/ground-render-helpers-native.json').read_text())['functions']
boundary=json.loads((ROOT/'metadata/water-boundary-native.json').read_text())['functions']['0x0047a1c0']['items']
sprite=json.loads((ROOT/'metadata/world-render-sprite-core-native.json').read_text())['functions']['0053e590']['assembly']['items']
main_instructions={int(i['address'],16) for f in ground.values() for i in f['assembly']['items']}
helper_instructions={int(i['address'],16) for a in ['0053b670','0053b830'] for i in helper[a]['assembly']['items']}|{int(i['address'],16) for i in boundary}|{0x51f780}
sprite_instructions={int(i['address'],16) for i in sprite}
for i in [i for f in ground.values() for i in f['assembly']['items']]+[i for a in ['0053b670','0053b830'] for i in helper[a]['assembly']['items']]+boundary+sprite:
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
assert bytes(vm.mem_read(0x51f780,1))==b'\xc3'
arities={0x53b670:0,0x53b830:1,0x47a1c0:2,0x51f780:2,0x436b60:1,0x439740:1,0x41d410:2,0x431ce0:2,0x432620:2,0x432d30:4,0x433760:3,0x434aa0:4,0x434e10:2,0x53e590:3,0x53f250:3,0x4cdad0:0,0x4cdb80:0,0x40fb60:1,0x40fc40:1,0x40fcf0:1,0x40fdd0:1,0x40fed0:1,0x40ff50:1,0x40ffd0:1,0x410090:1,0x4100f0:2,0x4101c0:2,0x410230:1,0x4105c0:1,0x410660:1,0x410700:2,0x4109d0:2,0x410a70:2,0x410b10:1,0x410de0:1,0x410e80:1,0x410f70:1,0x410fb0:1,0x4114a0:1,0x411fd0:1,0x427a20:2,0x42a160:2,0x42b440:2,0x42c740:0,0x42c9a0:0,0x42cee0:0,0x42d810:0,0x42e680:0,0x4cdf20:2,0x4cdeb0:2}
real_endpoints={0x53b670,0x53b830,0x47a1c0,0x51f780,0x53e590,0x53f250,0x4cdad0,0x4cdb80}
colors={0x4100f0,0x4101c0,0x410700,0x4109d0,0x410a70}
watched=[0x70e47c,0x70e480,0x70e488,0x70e458,0x70e44c,0x70e464,0x70e460,0xd3b5c0,0x70e3e8,0x70e3ec,0xe94e0c,0x6059d0,0xd3b55c,0xd3b558,0xd3b554,0xd3b550]
params=None;events=[];mutation_seen=0;regions=[];hits=set();helper_hits=set();sprite_hits=set();composed_hits=set();writes=set();descriptor_cache={};mutated=set()
def endpoint(cpu,a,n,user):
    global mutation_seen
    sp=cpu.reg_read(UC_X86_REG_ESP);count=arities[a];v=[read(sp+4+k*4) for k in range(count)]
    if a in colors:v[1]&=65535
    self=0xf55bb0 if a in [0x53e590,0x53f250] else 0
    if self:assert cpu.reg_read(UC_X86_REG_ECX)==self,'Native receiver'
    b=s16(params[17])*264&MASK;e=[a,self,count]+v+[0]*(5-count)+[read(x) for x in watched]+[read(0x70e120,2),read(0x7999e8+b,2),read(0x7999e8,2),read(0xf55b84)]+[read(0xf55b50+i*4) for i in range(12)]
    assert len(e)==40;events.append(e)
    if a==params[43]:
        mutation_seen+=1
        if mutation_seen==params[44]:
            m=[];bits=params[45];mutated.add(a)
            if bits&1:m.append((0x70e488,params[46],4))
            if bits&2:m.extend([(0x70e47c,params[47],4),(0x70e480,params[48],4)])
            if bits&4:m.extend([(0x70e458,params[49],4),(0x70e44c,params[50],4),(0x70e464,params[51],4),(0x70e460,params[52],4)])
            if bits&8:m.extend([(0xd3b5c0,params[53],4),(0xb7482c+4,params[53],4)])
            if bits&16:m.append((0x7999e8+b,params[54],2))
            if bits&32:m.extend([(0xaf598c+4,params[55],4),(0xb285cc+1,params[56],1),(0xb67d1c+1,params[57],1)])
            if bits&64:m.append((0x6059d0,params[58],4))
            if bits&128:m.extend([(0xe38db0,params[59],4),(0xe38dac,params[60],4)])
            if bits&256:m.extend((0xf55b50+i*4,params[61],4) for i in range(12));m.append((0xf55b84,params[61],4)) if bits&256 else None
            if bits&512:m.append((0x70e120,0x80ff,2))
            for address,value,width in m:write(address,value,width)
    if a in real_endpoints:return
    cpu.reg_write(UC_X86_REG_EAX,77);cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
    cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def code(cpu,a,n,user):
    if a in main_instructions:hits.add(a);return
    if a in helper_instructions:helper_hits.add(a);return
    if a in sprite_instructions:sprite_hits.add(a);return
    if 0x53f250<=a<=0x53f49f or 0x4cdad0<=a<=0x4cdb79 or 0x4cdb80<=a<=0x4cdc2d:composed_hits.add(a);return
    if a not in arities:raise RuntimeError('Unexpected guest instruction '+hex(a))
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a<STACK+0x100000:return
    assert any(start<=a and a+n<=start+size for start,size in regions),'Unwatched native data write '+hex(a)
    writes.add((a,n))
vm.hook_add(UC_HOOK_CODE,code)
for a in arities:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
def native(p):
    global params,mutation_seen,regions
    params=p;mutation_seen=0;events.clear();regions=[]
    def region(a,n):
        a&=MASK
        if any(x==a for x,y in regions):return
        vm.mem_write(a,bytes([(p[1]+len(regions))&255])*n);regions.append((a,n))
    for a,n in [(0x6059c8,16),(0x6de510,16),(0x70e118,16),(0x70e0f0,0x775620-0x70e0f0),(0x78d7a8,16)]:region(a,n)
    for a,w in [(0xaf598c,4),(0xb285cc,1),(0xb350dc,2),(0xb4e6fc,2),(0xb67d1c,1),(0xb7482c,4)]:region(a-8,8*w+16)
    for a,n in [(0xd3b520,224),(0xe38aa0,0x150),(0xe38d90,0x150),(0xe92e10,24),(0xe94e04,16),(0xf55b48,0x68),(0xf55bb0,8000),(0x10d69b0,4097*64+96),(0x1265178,16)]:region(a,n)
    b=s16(p[17])&MASK
    for v in [0,b,9,11]:region(0x7999d0+v*264,280)
    ids=[1,257,1000]
    for v in ids:region(0xbcd5f8+v*388,404)
    tags=bytearray(8000)
    for i in range(1000):struct.pack_into('<II',tags,i*8,0,100+i*4)
    vm.mem_write(0xf55bb0,bytes(tags));current_image=p[16]
    if p[38]:current_image=(current_image+read([0,0xf56bd4,0xf56d24,0xf57224,0xf572ec,0xf56bdc][p[38]]))&MASK
    key=(p[33],p[34],p[35],p[36],p[63],p[66],p[67])
    if key not in descriptor_cache:
        record=bytearray(64);struct.pack_into('<III',record,0,p[66],0x100,p[67]);struct.pack_into('<I',record,16,1);struct.pack_into('<HH',record,20,p[33]&65535,p[34]&65535);record[0x32]=p[35]&255;record[0x33]=p[63]&255;record[0x35]=p[36]&255;descriptor_cache[key]=bytes(record)*4097
    vm.mem_write(0x10d69f8,descriptor_cache[key]);write(0x126517c,p[37]);vm.mem_write(0x70e494,struct.pack('<I',0x0fff0000)*(231*457))
    for a,value,n in [(0x70e440,p[8]*4,4),(0x70e444,p[8]*3,4),(0x70e448,p[8]*2,4),(0x70e44c,p[8],4),(0x70e450,p[9]*3,4),(0x70e454,p[9]*2,4),(0x70e458,p[9],4),(0x70e45c,p[10],4),(0x70e460,p[2],4),(0x70e464,p[3],4),(0x70e478,p[4],4),(0x70e474,p[5],4),(0x70e490,p[6],4),(0x70e48c,p[7],4),(0x70e3c4,p[20],4),(0xe94e0c,p[22],4),(0x6de515,p[21],1),(0x6059d0,p[23],4),(0xe38e54,p[24],4),(0x78d7b0,p[25],4),(0x78d7ac,p[26],4)]:write(a,value,n)
    for a,value in [(0x70e430,768),(0x70e434,0),(0x70e438,1024),(0x70e43c,0)]:write(a,value)
    row=(p[5]-8)&MASK;col=(p[4]-4)&MASK
    if row<457 and col<231:
        index=row*231+col
        if p[11]!=2:write(0x70e494+index*4,p[12])
        if p[11]==1:
            write(0x70e494+(index+1)*4,2)
            if row<456:write(0x70e494+(index+231)*4,2)
        if p[11]==3:vm.mem_write(0x70e494,struct.pack('<I',p[12])*(231*457))
        write(0x70e0f4+index*4,p[27]);write(0x70e0f8+index*4,p[28]);write(0x70e0fc+index*4,p[28])
    for i in range(8):
        varied=not(i==1 or p[65])
        for a,value,n in [(0xaf598c+i*4,p[13]^(0x100 if varied else 0),4),(0xb285cc+i,p[15]^(7 if varied else 0),1),(0xb350dc+i*2,0,2),(0xb4e6fc+i*2,p[17],2),(0xb67d1c+i,p[14]^(0x40 if varied else 0),1),(0xb7482c+i*4,current_image+(i*31 if varied else 0),4)]:write(a,value,n)
    write(0xb4e6fc+6,9,2);write(0xb4e6fc+8,11,2);a=(0x7999d8+b*264)&MASK;vm.mem_write(a,bytes(264));write(a+16,p[18],2);write(a+0xbf,p[19],1)
    for i,v in enumerate([9,11]):
        a=0x7999d8+v*264;vm.mem_write(a,bytes(264));mode=(p[29]>>(8*i))&255
        if mode in [1,3,4,5,6]:write(a+16,182 if mode==6 else 183,2);write(a+0x56,0 if mode==3 else 1,1);write(a+0x68,0 if mode==4 else 1);write(a+0x59,4 if mode==5 else 3,1)
        if mode==2:write(0xb350dc+(4 if i else 3)*2,1,2)
    for i,v in enumerate(ids):
        a=0xbcd600+v*388;vm.mem_write(a,bytes(388));write(a,ids[i+1] if i<p[32] else 0,2);write(a+2,p[30],1);write(a+0x5d,p[31],1)
    for a,value,n in [(0x70e120,p[64],2),(0xd3b55c,0,4),(0xd3b558,1024,4),(0xd3b554,0,4),(0xd3b550,768,4),(0xe38d9c,1024,4),(0xe38d98,768,4),(0xe38db0,1024,4),(0xe38dac,768,4),(0xe38d94,2048,4),(0xe92e18,FRAME+0x100,4),(0xe92e1c,FRAME+0x200,4),(0xe38dda,0,1)]:write(a,value,n)
    for repeat in range(p[62]):
        sp=STACK+0xf0000
        for i,v in enumerate([STOP,p[39],p[40],p[41],p[42]]):write(sp+i*4,v)
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ECX,0x0badc0de);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start([0x430ef0,0x4316f0,0x430e60][p[0]],STOP,count=3000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
        assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'events':events.copy()}
default=[0]*68
for k,v in {2:-13,3:-6,4:4,5:8,6:320,7:144,8:15,9:58,10:29,12:1,14:64,16:2000,23:1,26:232,27:0xfff0000,28:0xfff0000,30:13,33:58,34:60,35:1,36:1,37:4096,39:4,40:8,41:0x12345678,42:9999,44:1,46:2,47:1,48:1,49:60,50:17,51:-6,52:-13,53:2001,54:71,57:64,58:2,59:1920,60:1080,61:MASK,62:1,66:PIXELS+0x100,67:4}.items():default[k]=v&MASK
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
case()
for op,mode,kind,foot in itertools.product([0,1],[0,27,28,30,33,34,35,36,37,38,39,40,41,42,45,49,51,0x1000033],[0,229,230,234,235,236,71],range(6)):case(p0=op,p22=mode,p18=kind,p17=1,p15=foot)
for kind in list(range(237))+[255,256,32767,32768,65535]:case(p17=1,p18=kind)
flags=[0,1,2,4,8,16,32,64,128,256,0x800,0x4000,0x10000,0x40000,0x50000,0x80000,0xc04000,0x40000000,0x20000002,0xffffffff]
for op,terrain,foot in itertools.product([0,1],flags,range(16)):case(p0=op,p13=terrain,p15=foot)
for op,row,col,h,w in itertools.product([0,1],[-1,0,1,456,457],[-1,0,230,231],[-15,-14,-13,-12,0],[-8,-7,-6,-5]):case(p0=op,p4=col+4,p5=row+8,p2=h,p3=w,p11=2)
for mode,offset,water,animate,enabled,total in itertools.product([1,2,3],[-1,0,1,5,6,14,15,74,75,89,90],[4,0x4000004],[0,1],[0,1],[232,1000]):case(p38=mode,p16=offset,p13=water,p20=animate,p25=enabled,p26=total)
for op,tile,blank in itertools.product([0,1],[0xfff0000,0xfff0001,0xfff07d0,0x7fffffff],[0,1]):case(p0=op,p12=tile,p24=blank)
for first,second,mode,actor,hidden in itertools.product([3,0xfff0000],[4,0xfff0000],[0,1,2,3,4,5,6,0x100,0x200],[11,13,45,50,56,57,255],[0,1]):case(p27=first,p28=second,p29=mode,p30=actor,p31=hidden)
for op in [0,1,2]:case(p0=op,p11=1,p2=-12,p17=257,p18=71,p19=1,p62=2)
for foot in [0x10,0x11,0x12,0x15,0x1f,0xff]:case(p15=foot)
for offset in [47,48,71,72]:case(p38=4,p16=offset)
case(p27=0xfff0000,p28=4,p29=0x200,p30=255,p32=2)
for op,edge,kind in itertools.product([0,1],[0,1,64,128,255],[0,229]):case(p0=op,p14=edge,p18=kind,p17=1)
for bld,kind,terrain in itertools.product([0,1],[0,6,71,101,183],flags):case(p17=bld,p18=kind,p13=terrain,p2=0,p3=0,p11=3,p5=9)
case(p2=8,p3=10,p11=3,p5=9)
for op,tile,selected in itertools.product([0,1],[1,0xfff07d0],[1,0xfff07d0,0x1000001]):case(p0=op,p12=tile,p23=selected)
for op in [0,1,2]:case(p0=op,p11=1,p2=-12,p65=1)
for op,field,value in itertools.product([0,1],[6,7,8,9,10],[0,1,255,256,0x7fffffff,0x80000000,0xffffffff]):case(**{'p0':op,'p'+str(field):value})
for op,bld,damage in itertools.product([0,1],[0,1,257,3999,65535],[0,1,255]):case(p0=op,p17=bld,p19=damage)
for fmt,transparent,tint,width,height in itertools.product([0,1,2,30],[0,1],[0,0x80ff],[58,118],[60,70]):case(p35=fmt,p63=transparent,p64=tint,p33=width,p34=height)
for count,pointer,upper in itertools.product([0,1,4096],[0,PIXELS+0x100,0xffffffff],[0,1]):case(p37=count,p66=pointer,p36=upper,p27=3,p29=1)
mutation_fixtures={
 0x53b670:{},0x47a1c0:{},0x53b830:{17:1,18:101},0x41d410:{13:0x20},
 0x53e590:{},0x53f250:{27:3,29:1},0x4cdad0:{},0x4cdb80:{},
 0x40fb60:{},0x410230:{27:3,29:1},
 0x431ce0:{0:1,22:45},0x432620:{0:1,22:51},0x432d30:{0:1,13:8},
 0x433760:{0:1,22:27},0x434aa0:{0:1,22:33},0x434e10:{0:1,22:28},
 0x51f780:{0:1,22:30},0x436b60:{0:2},0x439740:{0:2}}
for target,fixture in mutation_fixtures.items():
    for when,bits in itertools.product([1,2],[1,2,4,8,16,32,64,128,256,512,1023]):
        kw={'p'+str(k):v for k,v in fixture.items()};kw.update(p43=target,p44=when,p45=bits,p11=1,p2=-12,p65=1);case(**kw)
for bits in [8,16,32,256,1023]:case(p17=1,p18=101,p43=0x53b830,p45=bits,p54=71)
for repeat in [1,2]:case(p62=repeat,p13=4,p20=1,p38=1,p16=5,p11=1,p2=-12,p65=1)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-ground-render/Release/ground_render_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'start':args.start,'limit':args.limit,'single_case':bool(args.case)},
 'method':'Actual normal477/overlay435/preview28 instruction ground module versus C, composed with real sprite dispatcher/upper draw/both clip algorithms, real stats clear/classifier/boundary and legitimate no-op. Ordered endpoint arguments/receivers/state, watched fixture bytes+guards and native/C stack+saved registers compared.',
 'limitations':['Final bitmap/RLE/terrain consumers and deeper overlay/world/actor/profile-publication algorithms are controlled. This comparison does not establish city pixel fidelity.','Fixtures contain loaded inline images; filesystem-loading branches in the composed sprite dispatcher are explicitly excluded, with failing C guards rather than fabricated I/O success.','Boundary callsite pushes an extra row while the real callee consumes only column; no-op callsites evaluate two inputs while the real callee consumes none. Explicit C adapters preserve observation of the callsite inputs; native/C stack conventions are checked independently.']}
checked=0;p=None;started=time.monotonic()
try:
    assert json.loads(process.stdout.readline())=={'fields':68}
    for p in cases:
        expected=native(p);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read();actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/ground_render_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected},indent=2)+'\n')
        assert actual==expected,repr({'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.start or args.limit or args.case):
        assert hits==main_instructions,'Uncovered ground instructions '+repr([hex(v) for v in sorted(main_instructions-hits)])
        assert helper_hits==helper_instructions,'Incomplete real helper coverage'
        assert mutated==set(mutation_fixtures),'Unreached mutation targets '+repr(set(mutation_fixtures)-mutated)
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
prefix='src' if args.production else 'artifacts/ground_render_phase'
sources=[prefix+'/unit_'+n+'.c' for n in ['00469','00470','00472','02493']]+['include/ground_render.h','tests/ground_render.c','tests/ground_render_real_graphics.c','tests/ground_render_real_upper.c','tests/ground_render_real_clips.c','tests/ground_render_real_helpers.c','tools/ground_render_project/CMakeLists.txt','tools/verify_ground_render_emulation.py','src/unit_02524.c','src/unit_02525.c','src/unit_01725.c','src/unit_01726.c','src/unit_02497.c','src/unit_01097.c','src/unit_02351.c','metadata/ground-render-next-native.json','metadata/ground-render-helpers-native.json','metadata/water-boundary-native.json','metadata/world-render-sprite-core-native.json']
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(main_instructions)],native_uncovered_instructions=[hex(v) for v in sorted(main_instructions-hits)],helper_instruction_coverage=[len(helper_hits),len(helper_instructions)],sprite_dispatch_instruction_coverage=[len(sprite_hits),len(sprite_instructions)],upper_clip_executed_instruction_count=len(composed_hits),mutated_endpoint_addresses=[hex(a) for a in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/ground-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
