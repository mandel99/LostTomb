"""Complete staged world pass versus pinned original instructions in Unicorn."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--source',type=Path,default=ROOT/'artifacts/world_render_phase/unit_00547.c');parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--case',type=Path);parser.add_argument('--start',type=int,default=0)
args=parser.parse_args()
if args.production:args.source=ROOT/'src/unit_00547.c'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ
from unicorn.x86_const import *
MASK=0xffffffff;ALT=0x30000000;PIXELS=0x31000000;FRAME=0x31100000;STOP=0x33000000;STACK=0x34000000
sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(image,pe+6)):
    q=table+k*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
for a,n in [(ALT,0x400000),(PIXELS,0x10000),(FRAME,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
vm.mem_write(PIXELS,b'\x6b'*0x10000);vm.mem_write(FRAME,b'\xc7'*0x10000);vm.mem_protect(PIXELS,0x10000,UC_PROT_READ);vm.mem_protect(FRAME,0x10000,UC_PROT_READ)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
write(ALT+0x30f5cc,16)
for i in range(1000):write(ALT+i*8+4,500+i*3)
vm.mem_protect(ALT,0x400000,UC_PROT_READ)
snapshot=json.loads((ROOT/'metadata/world-render-native.json').read_text());main_instructions={int(i['address'],16) for i in snapshot['assembly']['items']}
callee_data=json.loads((ROOT/'metadata/world-render-callee-contracts-native.json').read_text())['functions']
real_helpers={int(i['address'],16) for a in ['00438180','004381a0'] for i in callee_data[a]['assembly']['items']}|{0x489500}
for i in snapshot['assembly']['items']+[i for a in ['00438180','004381a0'] for i in callee_data[a]['assembly']['items']]:
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
assert bytes(vm.mem_read(0x489500,3))==bytes.fromhex('c21000')
assert bytes(vm.mem_read(0x437889,8))==bytes.fromhex('83e70383ff037757')
assert [read(0x438170+i*4) for i in range(4)]==[0x437898,0x4378ac,0x4378c0,0x4378d4]
unreachable={0x4378e8,0x4378ec}
watched=[0x70e47c,0x70e480,0x70e488,0x70e458,0x70e464,0x70e460,0xd3b5c0,0xe94e0c,0x126517c,0xd3b5e0,0xd3b5dc]
arities={0x4ef550:4,0x401f70:3,0x420a30:2,0x41fff0:2,0x4e5e90:3,0x542e10:3,0x420050:2,0x4206f0:2,0x420d60:2,0x421920:3,0x421060:2,0x438e50:4,0x442280:4,0x40f2c0:2,0x488030:1,0x488050:1,0x488c20:4,0x489500:4,0x4381b0:5,0x438210:5,0x438350:5,0x438470:4,0x4384d0:4,0x438590:4,0x438650:4,0x4386b0:4,0x438710:4,0x438770:4,0x438180:1,0x4381a0:1,0x53e590:3,0x53f250:3,0x4c0160:3,0x436310:3,0x4cdad0:0,0x4cdb80:0,0x410230:1,0x4105c0:1,0x410660:1,0x410700:2,0x4109d0:2,0x410a70:2}
selfs={0x401f70:0x6de650,0x488030:0xbca350,0x488050:0xbca350,0x488c20:0xbca350,0x489500:0xbca350,0x438180:0xf55bb0,0x4381a0:0xf55bb0,0x53e590:0xf55bb0,0x53f250:0xf55bb0}
real_endpoints={0x438180,0x4381a0,0x489500,0x53f250,0x4cdad0,0x4cdb80}
ranges=[(0x53f250,0x53f49f),(0x4cdad0,0x4cdb79),(0x4cdb80,0x4cdc2d)]
params=None;events=[];mutation_seen=0;hits=set();helper_hits=set();composed_hits=set();regions=[];write_sites=set();upper_trace=[];descriptor_cache={}
def endpoint(cpu,a,n,user):
    global mutation_seen
    sp=cpu.reg_read(UC_X86_REG_ESP);count=arities[a];arguments=[read(sp+4+k*4) for k in range(count)];self=selfs.get(a,0)
    if a in [0x410700,0x4109d0,0x410a70]:arguments[1]&=65535
    if self:assert cpu.reg_read(UC_X86_REG_ECX)==(ALT if params[64]>=3 and a in [0x438180,0x4381a0] else self),'Native receiver '+hex(a)
    if params[64]==0:
        b=s16(params[15])*264&MASK
        e=[a,self,count]+arguments+[0]*(5-count)+[read(v) for v in watched]+[read(0x70e120,2),read(0x775688,2),read(0x7999e8+b,2),read(0x7999ea+b,2),read(0x7999dd+b,1),read(0x799a5f+b,1),read(0x799a97+b,1),read(0xd3b5bc,2),read(0xd3b5be,2),read(0xd3b530,1),read(0xd3b531,1),read(0xd3b5b8),read(0xbca2e0)]
        assert len(e)==32;events.append(e)
        if a==params[42]:
            mutation_seen+=1
            if mutation_seen==params[43]:
                m=[]
                if params[44]&1:m.append((0x70e488,params[45],4))
                if params[44]&2:m.extend([(0x70e47c,params[46],4),(0x70e480,params[47],4)])
                if params[44]&4:m.append((0x70e458,params[48],4))
                if params[44]&8:m.extend([(0x7999e8+b,params[49],2),(0x7999ea+b,params[50],2)])
                if params[44]&16:m.append((0x70e120,params[51],2))
                if params[44]&32:m.extend([(0xd3b5c0,params[52],4),(0xb7482c+4,params[52],4)])
                if params[44]&64:m.extend([(0x10d6a16+params[14]*64,params[53],2),(0x126517c,1,4)])
                if params[44]&128:m.extend([(0x7999dd+b,1,1),(0x799a5f+b,2,1)])
                for v,value,width in m:write(v,value,width)
    if a in real_endpoints:return
    result=params[35] if a==0x40f2c0 else params[36] if a==0x488030 else params[37] if a==0x488050 else 1 if a==0x53e590 else 0x12345678
    cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
    cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(count*4 if self else 0))
def code(cpu,a,n,user):
    if 0x53f250<=a<=0x53f29c:upper_trace.append([hex(a),cpu.reg_read(UC_X86_REG_EAX),cpu.reg_read(UC_X86_REG_ECX),cpu.reg_read(UC_X86_REG_EDX),cpu.reg_read(UC_X86_REG_EFLAGS)])
    if a in main_instructions:hits.add(a);return
    if a in real_helpers:helper_hits.add(a);return
    if any(lo<=a<=hi for lo,hi in ranges):composed_hits.add(a);return
    if a not in arities:raise RuntimeError('Unexpected guest instruction '+hex(a))
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a<STACK+0x100000:return
    assert any(start<=a and a+n<=start+size for start,size in regions),'Unwatched native data write '+hex(a)
    write_sites.add((a,n))
vm.hook_add(UC_HOOK_CODE,code)
for a in arities:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)

def native(p):
    global params,mutation_seen,regions
    params=p;mutation_seen=0;events.clear();upper_trace.clear();regions=[]
    def region(a,n):
        a&=MASK
        if any(start==a for start,size in regions):return
        vm.mem_write(a,bytes([(p[0]+len(regions))&255])*n);regions.append((a,n))
    region(0x6de510,16);region(0x70e118,16);region(0x70e0f0,0x775620-0x70e0f0);region(0x775680,24)
    for a,w in [(0xadc36c,1),(0xaf598c,4),(0xb285cc,1),(0xb350dc,2),(0xb4e6fc,2),(0xb67d1c,1),(0xb7482c,4)]:region(a-8,8*w+16)
    for a,n in [(0xbca2d8,24),(0xd3b520,224),(0xe38d90,24),(0xe92e10,16),(0xe94e04,16),(0xf55bb0,8000),(0x10d69b0,4097*64+96),(0x1265178,16)]:region(a,n)
    b=s16(p[15])&MASK
    for v in [b,9,11]:region(0x7999d0+v*264,280)
    first=p[27] or 1;ids=[first,1000 if first==257 else 257,1999 if first==1000 else 1000]
    for v in ids+[1]:region(0xbcd5f8+v*388,404)
    if p[24]:region(0xbcd5f8+p[24]*388,404)
    if p[26]:region(0xbcd5f8+s16(p[26])*388,404)
    tags=bytearray(8000)
    for i in range(1000):struct.pack_into('<II',tags,i*8,0,100+i*4)
    vm.mem_write(0xf55bb0,bytes(tags));key=(p[39],p[40],p[34],p[41])
    if key not in descriptor_cache:
        record=bytearray(64);struct.pack_into('<III',record,0,PIXELS+0x100,0,4)
        for off,val in [(0x14,p[39]),(0x16,p[40]),(0x1e,p[34]),(0x22,0xfffb),(0x24,7)]:struct.pack_into('<H',record,off,val&65535)
        record[0x35]=p[41]&255;block=bytearray(bytes(record)*4097)
        struct.pack_into('<I',block,0,PIXELS+0x200)
        for off,val in [(0x14,118),(0x16,70),(0x1e,(p[34]+1)&65535 if p[34] else 0),(0x22,0xfff5),(0x24,13)]:struct.pack_into('<H',block,off,val)
        descriptor_cache[key]=bytes(block)
    vm.mem_write(0x10d69f8,descriptor_cache[key]);write(0x126517c,p[38]);vm.mem_write(0x70e494,struct.pack('<I',0x0fff0000)*(231*457))
    for a,value,n in [(0x70e440,p[7]*4,4),(0x70e444,p[7]*3,4),(0x70e448,p[7]*2,4),(0x70e44c,p[7],4),(0x70e450,p[8]*3,4),(0x70e454,p[8]*2,4),(0x70e458,p[8],4),(0x70e45c,p[9],4),(0x70e460,p[1],4),(0x70e464,p[2],4),(0x70e478,p[3],4),(0x70e474,p[4],4),(0x70e490,p[5],4),(0x70e48c,p[6],4),(0x775688,p[62],2),(0xe94e0c,p[55],4),(0x6de515,p[56],1),(0xbca2e0,p[57],4)]:write(a,value,n)
    row=(p[4]-8)&MASK;col=(p[3]-4)&MASK
    if row<457 and col<231:
        index=row*231+col
        if p[58]!=2:write(0x70e494+index*4,1)
        if p[58]==1:
            write(0x70e494+(index+1)*4,2)
            if row<456:write(0x70e494+(index+231)*4,2)
        if p[58]==3:vm.mem_write(0x70e494,struct.pack('<I',1)*(231*457))
        write(0x70e0f4+index*4,p[59]);write(0x70e0f8+index*4,p[59]);write(0x70e0fc+index*4,p[60])
    for i in range(8):
        for a,value,n in [(0xadc36c+i,p[13],1),(0xaf598c+i*4,p[10],4),(0xb285cc+i,p[12],1),(0xb350dc+i*2,0,2),(0xb4e6fc+i*2,p[15],2),(0xb67d1c+i,p[11],1),(0xb7482c+i*4,p[14],4)]:write(a,value,n)
    write(0xb4e6fc+6,9,2);write(0xb4e6fc+8,11,2);a=(0x7999d8+b*264)&MASK;vm.mem_write(a,bytes(264))
    for off,value,n in [(0x10,p[16],2),(0x12,p[17],2),(5,p[19],1),(0x87,p[20],1),(0xbf,p[18],1),(0x56,p[21],1),(0x68,p[22],4),(0x59,p[23],1),(0x5c,p[24],4),(0x64,p[25],4),(0xb4,p[26],2)]:write(a+off,value,n)
    for i,v in enumerate([9,11]):
        a=0x7999d8+v*264;vm.mem_write(a,bytes(264));mode=(p[61]>>(8*i))&255
        if mode in [1,3,4,5,6]:write(a+16,182 if mode==6 else 183,2);write(a+0x56,0 if mode==3 else 1,1);write(a+0x68,0 if mode==4 else 1);write(a+0x59,4 if mode==5 else 3,1)
        if mode==2:write(0xb350dc+(4 if i else 3)*2,1,2)
    if p[66]:write(0xb4e6fc+4,9,2);write(0x7999e8+9*264,p[66],2)
    vm.mem_write(0xbcd600+388,bytes(388));write(0xbcd602+388,p[28],1)
    for i,v in enumerate(ids):
        a=0xbcd600+v*388;vm.mem_write(a,bytes(388))
        for off,value,n in [(0,ids[i+1] if i<p[33] else 0,2),(2,p[28],1),(0x5d,p[29],1),(0x118,p[30],2),(9,p[31],1),(0x30,p[32],1)]:write(a+off,value,n)
    if p[27]:write(0xb350dc+2,p[27],2)
    if p[24]:write(0xbcd609+p[24]*388,p[31],1)
    if p[26]:write(0xbcd630+s16(p[26])*388,p[32],1)
    for a,value in [(0xd3b55c,0),(0xd3b558,1024),(0xd3b554,0),(0xd3b550,768),(0xe38d9c,1024),(0xe38d94,2048),(0xe92e18,FRAME+0x100)]:write(a,value)
    result=0
    for repeat in range(1 if p[64] else p[63]):
        sp=STACK+0xf0000;write(sp,STOP);write(sp+4,p[65] if p[64] else p[54])
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ECX,ALT if p[64]>=3 else 0xf55bb0);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        entry=0x438180 if p[64] in [1,3] else 0x4381a0 if p[64] in [2,4] else 0x436b60
        vm.emu_start(entry,STOP,count=2000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(4 if p[64] else 0),'Native stack'
        assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
        if p[64]:result=vm.reg_read(UC_X86_REG_EAX)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'result':result,'memory':h.hexdigest(),'events':events.copy()}

default=[0]*67
for k,v in {1:-17,2:-6,3:4,4:8,5:320,6:144,7:15,8:58,9:29,10:1,11:64,14:2000,15:7,28:13,31:8,32:10,35:1,38:4096,39:58,40:60,41:1,43:1,45:2,46:0,47:0,48:60,49:71,52:2001,53:2,59:0x0fff0000,60:0x0fff0000,63:1}.items():default[k]=v&MASK
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
case()
for kind,frames,footprint in itertools.product(range(237),[0,5],range(6)):case(p16=kind,p34=frames,p12=footprint)
for kind,state,direction in itertools.product([210,225,213,227,228,230],range(15),[0,2,4,6,1,0x100]):case(p16=kind,p17=state,p57=direction)
for kind in [230,0]:
    for first,second,mode,actor_type,hidden in itertools.product([3,0x0fff0000],[4,0x0fff0000],[0,1,2,0x100,0x201],range(11,60),[0,1]):case(p16=kind,p10=0,p59=first,p60=second,p61=mode,p28=actor_type,p29=hidden)
for row,col,rows,columns in itertools.product([0,1,456,457,-1],[-1,0,230,231],[-19,-18,-17,-16,0],[-8,-7,-6,-5]):case(p3=col+4,p4=row+8,p1=rows,p2=columns,p58=2)
for kind,frames,returned,worker,action in itertools.product([0,75,192,90,74,76],[0,1,5,20,21,32767,32768,65535],[-1,0,1,20,21,40],[0,1,257],[0,10]):case(p16=kind,p34=frames,p35=returned,p26=worker,p32=action)
for actor,links,hidden,extra,terrain in itertools.product([1,257,1999],[0,1,2],[0,1],[0,1,65535],[0,1,0xeefed77f]):case(p27=actor,p33=links,p29=hidden,p30=extra,p10=terrain)
for op,value,count in itertools.product([1,3],[-1,0,1,15,16,4095,4096,0x7fffffff,0x80000000],[0,16,4096,0xffffffff]):case(p64=op,p65=value,p38=count)
for op,group in itertools.product([2,4],[0,1,351,546,547,624,628,632,633,999]):case(p64=op,p65=group)
for kind in [65535,32768,255,256]:
    for frames,footprint in itertools.product([0,5],[0,2,5,6,15]):case(p16=kind,p34=frames,p12=footprint)
for footprint in range(16):case(p12=footprint,p34=5)
for image,kind,worker,frames in itertools.product([2000,2001,2002,2003],[75,192,90],[0,1],[1,20,21]):case(p14=image,p16=kind,p26=worker,p34=frames,p35=30)
for image,footprint in itertools.product([3119,3120,3143,3144],range(6)):case(p14=image,p12=footprint)
for extra,flags,edge,mode,special in itertools.product([0,1,255],[0,0x44,0x40,0x20000002,0x20000003,0xffffffff],[0,64,255],[0,51],[0,1]):case(p13=extra,p10=flags,p11=edge,p55=mode,p56=special)
for active,warning,highlight in itertools.product([0,1,128],[0,1,2,3],[0,1,255]):case(p19=active,p20=warning,p18=highlight)
for state,edge in itertools.product([0,11,12,13,32768,65535],[0,64]):case(p16=57,p17=state,p11=edge)
for kind,related,state in itertools.product([229,234,235,236],[0,1,257],[0,1,8,255]):case(p16=kind,p24=related,p25=1,p31=state)
for first,second,mode in itertools.product([3,0x0fff0000],[4,0x0fff0000],[1,3,4,5,6,0x100,0x300,0x400,0x500,0x600]):case(p10=0,p59=first,p60=second,p61=mode)
for first,second in [(183,230),(230,183),(0,230),(230,0)]:case(p16=first,p66=second,p58=1,p1=-16,p21=1,p22=1,p23=3)
for actor_type in [13,45,50,56,57,255]:case(p16=230,p27=1,p28=actor_type,p58=1,p1=-16,p10=0)
for result,footprint,frames in itertools.product([0,1,2,0x100,0xffffff00,0xdeadbeef],[0,2],[0,5]):case(p36=result,p37=result,p12=footprint,p34=frames)
for field,value in itertools.product([5,6,7,8,9],[0,1,255,256,0x7fffffff,0x80000000,0xffffffff]):case(**{'p'+str(field):value})
for field,value in itertools.product([15,62],[0,1,257,3999,65535]):case(**{'p'+str(field):value})
for parameter in [0,9999,0x80000000,0xffffffff]:case(p27=1,p54=parameter,p33=2)
for mode,special,footprint in itertools.product([0,1,51,0x100,0x1000033,0x80000000,0xffffffff],[0,1,255],[0,5,0x80,0xf5,255]):case(p55=mode,p56=special,p12=footprint,p10=0x20000002)
for image,count,frames in itertools.product([1,2000,4095,4096],[0,1,2000,2001,4096,0xffffffff],[0,5]):case(p14=image,p38=count,p34=frames)
for kind,rows,footprint in itertools.product([0,183,230],[-17,-16],[0,2]):case(p16=kind,p1=rows,p12=footprint,p58=3)
mutation_fixtures={
 0x53f250:{},0x4cdad0:{},0x4cdb80:{},0x410230:{},0x410700:{},
 0x53e590:{34:5},0x40f2c0:{34:5},0x488030:{12:2},0x488050:{34:5},
 0x488c20:{12:2,36:1},0x489500:{34:5,37:1},0x438180:{34:5},0x4381a0:{34:5,16:75},
 0x4381b0:{34:5,16:75},0x438210:{34:5,16:74},0x438350:{34:5,16:76},
 0x438470:{34:5,16:161},0x4384d0:{34:5,16:106},0x438590:{34:5,16:30},
 0x438650:{34:5,16:34},0x4386b0:{34:5,16:31},0x438710:{34:5,16:33},0x438770:{34:5,16:35},
 0x401f70:{16:230},0x420a30:{16:230},0x4ef550:{16:183},0x442280:{16:71},
 0x4e5e90:{16:207},0x542e10:{16:213},0x420050:{16:213},0x41fff0:{16:210,57:4},
 0x4206f0:{16:225,17:6},0x420d60:{16:227},0x421920:{16:229,24:1,25:1},0x421060:{16:228},
 0x438e50:{27:1},0x436310:{},0x4c0160:{16:202}}
for target,fixture in mutation_fixtures.items():
    for when,bits in itertools.product([1,2],[1,2,4,8,16,32,64,128,255]):
        kw={'p'+str(k):v for k,v in fixture.items()};kw.update(p42=target,p43=when,p44=bits,p46=1,p47=1,p51=0x80ff,p58=1,p1=-16);case(**kw)
for repeat in [1,2]:case(p63=repeat,p27=1,p33=2,p34=5,p62=65535,p58=1,p1=-16)
case(p16=0,p10=0,p58=1,p1=-16)
case(p18=1,p34=5)
case(p38=1,p34=5,p35=40)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-world-render/Release/world_render_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'checked':0,'passed':False,
 'case_selection':{'start':args.start,'limit':args.limit,'single_case':bool(args.case)},
 'method':'Complete staged four-pass world renderer versus original instructions; real manager record/tag helpers, upper sprite algorithm and both clipping algorithms compose inside the pass. Ordered deep endpoint arguments/receiver/state, all watched fixture bytes+guards and native/C stack+saved registers are compared.',
 'limitations':['Other large object drawing/classifier/animation algorithms and six final raster consumers are controlled endpoints; this proof establishes world-controller behavior and composed upper-sprite/clipping state, not city pixel fidelity.',
 'Valid mapped tile IDs0..7/sentinel, finite mapped building/actor records and acyclic actor links; arbitrary invalid pointers or cyclic lists are outside finite fixtures.',
 'Native thiscall cleanup and explicit receiver are checked separately from source C facades. World renderer return is void; helper full EAX results are compared.',
 'Candidate remains staged until coordinator releases application; --source binds the exact staged or applied source in source_hashes.']}
checked=0;p=None;started=time.monotonic()
try:
    assert json.loads(process.stdout.readline())=={'fields':67}
    for p in cases:
        expected=native(p);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read();actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/world_render_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected,'upper_trace':upper_trace},indent=2)+'\n')
        assert actual==expected,repr({'result':[actual['result'],expected['result']],'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.start or args.limit or args.case):
        assert hits==main_instructions-unreachable,'Unexpected world coverage '+repr([hex(v) for v in sorted(main_instructions-unreachable-hits)])
        assert helper_hits==real_helpers,'Incomplete real helper coverage'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
helper_root='src' if args.source.parent==ROOT/'src' else 'artifacts/world_render_phase'
sources=[args.source.relative_to(ROOT).as_posix(),helper_root+'/unit_00548.c',helper_root+'/unit_00549.c','include/world_render.h','tests/world_render.c','tests/world_render_real_upper.c','tests/world_render_real_clips.c','tests/world_render_real_members.c','tools/world_render_project/CMakeLists.txt','tools/verify_world_render_emulation.py','src/unit_02525.c','src/unit_01725.c','src/unit_01726.c','src/unit_01299.c','metadata/world-render-native.json','metadata/world-render-callee-contracts-native.json','metadata/world-render-city-members-native.json','metadata/world_render_member_binding_audit.json']
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(main_instructions)],reachable_native_instruction_coverage=[len(hits-unreachable),len(main_instructions-unreachable)],native_uncovered_instructions=[hex(v) for v in sorted(main_instructions-hits)],unreachable_native_instructions={'addresses':[hex(v) for v in sorted(unreachable)],'reason':'AND EDI,3 constrains variant to0..3; CMP EDI,3 / JA4378e8 cannot take default. The four pointer-table targets are authenticated as437898/4378ac/4378c0/4378d4.'},helper_instruction_coverage=[len(helper_hits),len(real_helpers)],composed_upper_clip_executed_instruction_count=len(composed_hits),native_data_write_sites=[[hex(a),n] for a,n in sorted(write_sites)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/world-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','native_data_write_sites']},indent=2));sys.exit(0 if report['passed'] else 1)
