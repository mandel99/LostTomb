"""Shared walking scheduler/physics/grid/queue chain versus pinned original instructions."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,random,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
MASK=0xffffffff;CELLS=51984;OUT=0x31000000;STOP=0x33000000;STACK=0x34000000;sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768
schema=(ROOT/'tests/actor_walking_case.h').read_text();field_names=[name.lower() for name in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum {',1)[1].split('};',1)[0]) if name!='FIELDS'];globals().update({'P_'+name.upper():i for i,name in enumerate(field_names)});FIELDS=len(field_names)
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
    q=table+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(OUT,4096),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
packets=[('actor-callback-human-queries-native.json',['004a5da0','004b3250']),('actor-walking-step-native.json',['004b27e0','004b33d0','004b3530','004b3c60','004b3f00']),('actor-walking-grid-native.json',None),('actor-walking-queue-native.json',None)]
functions={}
for name,entries in packets:
    packet=json.loads((ROOT/'metadata'/name).read_text());assert packet['native_sha256']==digest
    for fn in entries or packet['functions']:
        body=packet['functions'][fn]['assembly'];assert not body['has_more'];functions[int(fn,16)]=body['items']
empty=json.loads((ROOT/'metadata/actor-empty-callback-native.json').read_text());assert empty['bytes']['hex']=='c3';functions[0x51f780]=[{'address':'0051f780','bytes':'c3','instruction':'RET'}]
instructions={int(i['address'],16):i for body in functions.values() for i in body}
for a,i in instructions.items():
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
owned_entries=[0x4a5da0,0x4b27e0,0x4b33d0,0x4b3530,0x4b3c60,0x4b52f0,0x4b5420]
owned={int(i['address'],16) for fn in owned_entries for i in functions[fn]};dead={0x4b3e08,0x4b3e0a,0x4b3e10,0x4b3af4,0x4b3af5,0x4b3af6,0x4b3af7,0x4b3afe,0x4b3aff,0x4b3b02};reachable=set(instructions)-dead
dead_reason='Height BYTE is nonzero and<=6 before AND EAX,FF / DEC, so unsigned value0..5 cannot select JA5 zero-height arm (3 instructions). Gate AND EBX,EEFEC75F clears bit1000, so TEST BH,10 is always zero and its fallthrough cleanup/store arm is unreachable (7 instructions).'
cap=Cs(CS_ARCH_X86,CS_MODE_32);branches={}
for a in owned:
    i=next(cap.disasm(bytes.fromhex(instructions[a]['bytes']),a))
    if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]=(a+i.size,int(i.op_str,16))
excluded_edges={(0x4b3dcb,0x4b3e08),(0x4b3af2,0x4b3af4)}
dispatches=[(0x4a5dae,0x4a610c,18),(0x4b3890,0x4b3b64,3),(0x4b3a27,0x4b3bdc,5),(0x4b3ca8,0x4b3ea4,8),(0x4b3dcd,0x4b3ec4,6),(0x4b3e72,0x4b3edc,2)]
fixed=[(0xbcd5f0,776008),(0xc8ad38,500000),(0xd04e58,2016),(0xd056e8,32),(0xbcd5d8,32),(0xbca2c8,64),(0xaf598c,CELLS*4),(0xb350dc,CELLS*2),(0xb4e6fc,CELLS*2),(0xadc36c,CELLS),(0xec7630,CELLS),(0xebab20,CELLS),(0xee0c50,CELLS),(0xed4140,CELLS),(0x78d7c0,32),(0xeaaf60,256*224+16),(OUT,32)]
deltas=[-228,-227,1,229,228,227,-1,-229];assert [read(0x5f80fc+j*4) for j in range(8)]==[v&MASK for v in deltas]
argc={0x4a5da0:2,0x4b27e0:2,0x4b33d0:1,0x4b3530:1,0x4b3c60:1,0x4b52f0:1,0x4b5420:1,0x4b3f00:1,0x4b3250:1,0x51cba0:4,0x51f780:1}
queues={0x4f6630,0x4f6650,0x4f65b0};returning={0x51cba0}|queues
def semantic_result(fn,value):return value&255 if fn==0x51cba0 else value if fn in returning else 0
controlled={0x4baa10:2,0x4b2d20:1,0x4ad880:2,0x4b3470:2,0x4b1210:2,0x4690f0:2}
params=None;regions=[];events=[];frames=[];hits=set();edges=set();mutated=set();deep=set();heading_raw=set();seen=0;last_block=None;block_cache={};fill_cache={}
def actor(id):return(0xbcd600+id*388)&MASK
def queue(index):return(0xeaaf68+index*224)&MASK
def tag(pointer):return pointer if OUT<=pointer<OUT+32 else 0
def observe(fn,phase,v,result=0,point=0):
    global seen
    p=params;a=actor(p[P_ID]);other=actor(p[P_ALT]);b=(0x7999d8+p[P_BUILD]*264)&MASK;q=queue(p[P_QIDX])
    e=[fn,phase]+list(v)+[0]*(4-len(v))+[result,read(0xd056f8),read(0xbcd5e8),read(0xbca2d8)]
    e +=[read(a+o,n) for o,n in [(0x158,1),(0x31,1),(9,1),(0x32,2),(0x34,2),(0x36,2),(0x18,4),(0xc,2),(0xe,2),(0x30,1),(2,1),(6,1)]]
    e +=[read(other+0x31,1),read(other+9,1),read(other+0x18),read(b+0x10,2),read(b+0xca,1),read(q+4),read(q+8),read(q+12),read(a+0x6f,1),read(a+0x71,1)];assert len(e)==32;events.append(e)
    if fn==p[P_MFN] and phase==p[P_MPHASE]:
        seen+=1
        if seen==p[P_MOCC]:
            n=p[P_MBITS];mutated.add((fn,phase))
            if n&1:write(0xd056f8,p[P_NEWCURRENT])
            if n&2:write(a+0x158,p[P_NEWCOUNTER],1)
            if n&4:write(a+0x31,p[P_NEWPHASE],1)
            if n&8:write(a+9,p[P_NEWDIR],1)
            if n&16:write(a+2,p[P_NEWTYPE],1);write(a+0x6a,p[P_NEWUSAGE],1);write(a+0x6e,p[P_NEWWATER],1)
            if n&32:write(a+0x32,p[P_NEWROUTE],2);write(a+0x34,p[P_NEWPOS],2);write(a+0x36,p[P_NEWLEN],2)
            if n&64:write(a+0xc,p[P_NEWX],2);write(a+0xe,p[P_NEWY],2);write(a+0x18,p[P_NEWTILE])
            if n&128:
                t=(p[P_TILE]+deltas[p[P_DIR]&7])&MASK;write(0xaf598c+t*4,p[P_NEWTERRAIN]);write(0xb4e6fc+t*2,p[P_PAIR],2)
            if n&256:
                for off,value,width in [(0x10,p[P_NEWBKIND],2),(0xc4,p[P_NEWPAIR],4),(0x40,p[P_NEWOCC],2),(0xca,p[P_NEWQIDX],1),(0x2a,p[P_NEWOWNER],2)]:write(b+off,value,width)
                write(0x799a18+p[P_PAIR]*264,p[P_NEWPOCC],2)
            if n&512:
                if fn in queues:q=v[0]
                write(q+4,p[P_NEWQHEAD]);write(q+8,p[P_NEWQTAIL]);write(q+12,p[P_NEWQCAP])
            if n&1024 and point:write(point,p[P_NEWVALUE])
            if n&2048:write(a+0x30,p[P_NEWSTATE],1);write(a+6,p[P_NEWLIFE],1)
            if n&4096:write(0xbcd5e8,p[P_NEWGLOBAL])
def boundary_args(fn,sp,receiver,pointer):
    if fn==0x4f65b0:return[receiver,tag(pointer),read(pointer) if pointer else 0,int(pointer==0)]
    if fn in queues:return[receiver]
    return[read(sp+4+j*4) for j in range(argc[fn])]
def entry(cpu,a,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);receiver=cpu.reg_read(UC_X86_REG_ECX) if a in queues else 0;pointer=read(sp+4) if a==0x4f65b0 else 0
    v=boundary_args(a,sp,receiver,pointer);frames.append((a,sp,receiver,pointer,v));observe(a,0,v,point=pointer)
def leave(cpu,a,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native return frame'
    fn,saved_sp,receiver,pointer,v=frames.pop()
    if fn in queues:v=boundary_args(fn,saved_sp,receiver,pointer)
    if fn==0x51cba0:
        raw=cpu.reg_read(UC_X86_REG_EAX);assert raw&0xffffff00==v[1]&0xffffff00 and raw&255<=8
        heading_raw.add((v[1],raw))
    observe(fn,1,v,semantic_result(fn,cpu.reg_read(UC_X86_REG_EAX)),point=pointer)
def endpoint(cpu,a,n,user):
    p=params;sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+j*4) for j in range(controlled[a])];deep.add(a)
    if a==0x4ad880:
        assert v[0] and v[1] and v[0]!=v[1];shown=[tag(v[0]),tag(v[1]),read(v[0]),read(v[1])]
    else:shown=v
    observe(a,0,shown)
    r=p[P_RETVAL]
    if a==0x4b2d20:
        target=actor(v[0]);write(target+0x32,p[P_BUILD_ROUTE],2);write(target+0x34,p[P_BUILD_POS],2);write(target+0x36,p[P_BUILD_LEN],2)
        if s16(p[P_BUILD_ROUTE])>0:write(0xd04e58+p[P_BUILD_ROUTE]*2,v[0],2)
    elif a==0x4ad880:write(v[0],p[P_RELOCX]);write(v[1],p[P_RELOCY]);shown=[tag(v[0]),tag(v[1]),read(v[0]),read(v[1])]
    elif a==0x4b3470:r=p[P_LOOK]
    observe(a,1,shown,r if a in [0x4ad880,0x4b3470] else 0)
    cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def block(cpu,a,n,user):
    global last_block
    if a in controlled:return
    assert a in instructions,'Native escaped '+hex(a)
    if last_block is not None:edges.add((last_block,a))
    if(a,n) not in block_cache:
        block_cache[a,n]=[int(i['address'],16) for i in instructions.values() if a<=int(i['address'],16)<a+n]
        block_cache[a,n].sort()
    values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a and a+n<=STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
for fn,body in functions.items():
    vm.hook_add(UC_HOOK_CODE,entry,begin=fn,end=fn)
    for i in body:
        if i['instruction'].startswith('RET'):
            a=int(i['address'],16);vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
for a in controlled:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
    global params,regions,seen,last_block
    params=p;regions=[];events.clear();frames.clear();seen=0
    def region(a,n,fill):
        a&=MASK
        if(a,n) in regions:return
        key=(n,fill)
        if key not in fill_cache:fill_cache[key]=bytes([fill])*n
        vm.mem_write(a,fill_cache[key]);regions.append((a,n))
    for j,(a,n) in enumerate(fixed):region(a,n,0 if j==0 or 6<=j<=13 else(p[P_SEED]+j*29)&255)
    for j,value in enumerate([p[P_BUILD],p[P_PAIR],p[P_NEWPAIR]]):region(0x7999d0+value*264,280,(0x91+j*17)&255)
    def record(id,fill):
        a=actor(id);vm.mem_write(a-8,bytes([fill])*388)
        fields=[(0,0,2),(2,p[P_TYPE],1),(6,p[P_LIFE],1),(8,0x72,1),(9,p[P_DIR],1),(10,p[P_PREV],1),(0xc,p[P_X],2),(0xe,p[P_Y],2),(0x10,p[P_PREVX],2),(0x12,p[P_PREVY],2),(0x18,p[P_TILE],4),(0x1c,p[P_TX],2),(0x1e,p[P_TY],2),(0x2e,0x1234,2),(0x30,p[P_STATE],1),(0x31,p[P_PHASE],1),(0x32,p[P_ROUTE],2),(0x34,p[P_POS],2),(0x36,p[P_LENGTH],2),(0x39,1,1),(0x42,p[P_DRAWX],2),(0x44,p[P_DRAWY],2),(0x5d,p[P_HS],1),(0x6a,p[P_USAGE],1),(0x6e,p[P_WATER],1),(0x6f,p[P_HT],1),(0x70,p[P_HC],1),(0x71,p[P_HG],1),(0x82,0x55,1),(0x158,p[P_COUNTER],1)]
        for off,value,width in fields:write(a+off,value,width)
    record(p[P_ID],0xcc)
    if p[P_ALT]!=p[P_ID]:record(p[P_ALT],0xb9)
    if p[P_OWNER] not in [p[P_ID],p[P_ALT]]:record(p[P_OWNER],0xd7)
    vm.mem_write(0xc8ad38,bytes([p[P_PATH]&255])*500000);vm.mem_write(0xd04e58,bytes(2000))
    if s16(p[P_ROUTE])>0:write(0xd04e58+s16(p[P_ROUTE])*2,p[P_ID],2)
    b=(0x7999d8+p[P_BUILD]*264)&MASK
    for off,value,width in [(0x10,p[P_BKIND],2),(0xc4,p[P_PAIR],4),(0x40,p[P_OCC],2),(0xca,p[P_QIDX],1),(0x2a,p[P_OWNER],2)]:write(b+off,value,width)
    write(0x799a18+p[P_PAIR]*264,p[P_POCC],2);write(0x7999e8+p[P_PAIR]*264,0x38,2);write(0x799a18+p[P_NEWPAIR]*264,p[P_POCC],2)
    for j in range(256):
        a=queue(j);write(a+4,p[P_QHEAD]);write(a+8,p[P_QTAIL]);write(a+12,p[P_QCAP]);vm.mem_write(a+20,struct.pack('<51I',*[(j<<16)^k^0xabc00000 for k in range(51)]))
    for d in deltas:
        t=(p[P_TILE]+d)&MASK
        if t<CELLS:
            for a,value,width in [(0xaf598c+t*4,p[P_TERRAIN],4),(0xb4e6fc+t*2,p[P_BUILD],2),(0xadc36c+t,p[P_HEIGHT],1),(0xec7630+t,p[P_EC],1),(0xebab20+t,p[P_EB],1),(0xee0c50+t,p[P_EE],1),(0xed4140+t,p[P_ED],1)]:write(a,value,width)
    for a,value,width in [(0xd056f8,p[P_ID],4),(0xbcd5e8,p[P_GLOBAL],4),(0xbca2d8,p[P_MAPBASE],4),(0x78d7cc,p[P_MODE],1),(OUT,p[P_QVAL],4)]:write(a,value,width)
    a=actor(p[P_ID]);neighbor=(p[P_TILE]+deltas[p[P_DIR]&7])&MASK
    if p[P_TILE]<CELLS:write(0xb350dc+p[P_TILE]*2,0 if p[P_GRAPH]==4 else p[P_ID],2)
    write(a,0,2)
    if p[P_CHAIN]:
        assert p[P_CHAIN]<257 and p[P_ID]>p[P_CHAIN]
        for j in range(1,p[P_CHAIN]+1):write(actor(j),j+1 if j<p[P_CHAIN] else 0,2);write(actor(j)+0x18,neighbor if p[P_GRAPH]==3 else p[P_TILE])
        if p[P_GRAPH]==3:write(0xb350dc+neighbor*2,1,2)
        elif p[P_GRAPH]==6:write(a,1,2)
        else:
            write(0xb350dc+p[P_TILE]*2,1,2)
            if p[P_GRAPH]!=2:write(actor(p[P_CHAIN]),p[P_ID],2)
    entries=[0x4a5da0,0x4b27e0,0x4b33d0,0x4b3530,0x4b3c60,0x4b52f0,0x4b5420,0x4b3f00,0x4b3250,0x51cba0,0x4f6630,0x4f6650,0x4f65b0,0x51f780];fn=entries[p[P_OP]]
    top_args=[p[P_ID],p[P_SPEED]] if p[P_OP]==0 else [p[P_ID],p[P_STEPS]] if p[P_OP]==1 else [p[P_X],p[P_Y],p[P_TX],p[P_TY]] if p[P_OP]==9 else ([0 if p[P_NULLVALUE] else OUT] if p[P_OP]==12 else [] if p[P_OP] in [10,11] else [p[P_ID]])
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)];result=0
    for repeat in range(p[P_REPEATS]):
        sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
        for j,value in enumerate(top_args):write(sp+4+j*4,value)
        for reg,value in saved:vm.reg_write(reg,value)
        vm.reg_write(UC_X86_REG_ECX,queue(p[P_QIDX]) if fn in queues else 0xbadc0de);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);last_block=None
        vm.emu_start(fn,STOP,count=1000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native execution budget/frames'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+(8 if fn==0x4f65b0 else 4),'Native return/caller stack'
        assert all(vm.reg_read(reg)==value for reg,value in saved),'Native saved registers'
        guard=sp+4+len(top_args)*4;assert bytes(vm.mem_read(guard,128))==b'\xcc'*128,'Native caller guard'
        result=semantic_result(fn,vm.reg_read(UC_X86_REG_EAX))
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return{'return':result,'memory':h.hexdigest(),'events':events.copy()}
default=[0]*FIELDS
defaults={'seed':1,'id':257,'alt':1000,'speed':6,'steps':1,'phase':7,'dir':2,'type':4,'life':1,'x':100,'y':100,'tile':22900,'tx':110,'ty':100,'route':1,'pos':1,'length':16,'path':2,'drawx':1500,'drawy':1500,'build':257,'pair':1000,'occ':1,'pocc':1,'qidx':3,'owner':1999,'build_route':1,'build_pos':1,'build_len':16,'relocx':101,'relocy':100,'qcap':51,'qval':257,'mocc':1,'newcurrent':1000,'newcounter':255,'newphase':15,'newdir':9,'newtype':5,'newusage':1,'newroute':2,'newpos':2,'newlen':20,'newx':102,'newy':100,'newtile':22902,'newterrain':0x10000008,'newbkind':0x38,'newpair':1500,'newocc':256,'newpocc':257,'newqidx':7,'newowner':1000,'newqhead':1,'newqtail':50,'newqcap':51,'newstate':7,'newlife':2,'newglobal':256,'repeats':1,'newvalue':1000,'retval':0xa5a5a500}
for key,value in defaults.items():default[field_names.index(key)]=value&MASK
cases=[]
def case(**kw):
    p=default.copy()
    if 'dir' in kw and 'path' not in kw:kw['path']=kw['dir']
    for key,value in kw.items():p[field_names.index(key)]=value&MASK
    cases.append(tuple(p))
case(op=1)
case(op=3,terrain=8,bkind=0x88,look=1)
# Every selector value in the scheduler table is exercised with every BYTE
# accumulator. Multi-call cases separately prove the persisted cadence.
for speed,counter in itertools.product(range(18),range(256)):
    case(op=0,speed=speed,counter=counter,id=[1,257,1999][counter%3],phase=0)
for speed in [18,19,255,256,0x7fffffff,0x80000000,MASK]:
    for counter in [0,1,2,3,127,128,255]:case(op=0,speed=speed,counter=counter)
for speed in range(18):case(op=0,speed=speed,counter=0,repeats=39,phase=0)
for phase in range(256):
    case(op=1,phase=phase,**{'global':0x100})
for steps in [0,-1,-2147483648,1,2,3,16,33]:
    for phase in [0,7,14,15,127,128,254,255]:case(op=1,steps=steps,phase=phase)
for route,pos,length in itertools.product([0,1,257,999,65535],[0,1,15,16,499],[0,1,16,500,65535]):
    case(op=2,route=route,pos=pos,length=length)
for pos in [-32768,-1,32767]:
    for length in [-1,0,1,32767]:case(op=2,route=999,pos=pos,length=length)
for x,y,tx,ty in itertools.product([-1,0,1],repeat=4):case(op=9,x=x,y=y,tx=tx,ty=ty)
for x,y,tx,ty in [(0x80000000,0,0x7fffffff,0),(0x7fffffff,0,0x80000000,0),(0,0x80000000,0,0x7fffffff),(0,0x7fffffff,0,0x80000000)]:case(op=9,x=x,y=y,tx=tx,ty=ty)
for direction in range(8):
    for x,y in [(100,100),(32767,32767),(32768,32768),(65535,0),(0,65535)]:case(op=4,dir=direction,x=x,y=y)
for direction in [8,9,11,127,128,255]:case(op=4,dir=direction)
for height in range(256):case(op=4,terrain=0x44,height=height)
for terrain,type in itertools.product([0,4,0x40,0x40000000,0x40000004,0x40000044],[0,1,76,77,78,79,91,92,93,94,99,100,101,102,255]):case(op=4,terrain=terrain,type=type)
for direction,ht,hc,hg in itertools.product(range(10),[0,1,2,255],[0,1,127,255],[0,128,255]):case(op=7,dir=direction,ht=ht,hc=hc,hg=hg,drawx=65535,drawy=0)
for direction in [127,128,255]:case(op=7,dir=direction,ht=2,hc=255,hg=1)
for op in [5,6]:
    for tile in [-2147483648,-1,0,22900,51983]:case(op=op,tile=tile,graph=4)
    for chain in [0,1,2,19,20,21,254,255,256]:
        for graph in [0,2,6]:case(op=op,chain=chain,graph=graph)
for chain in [1,2,20,21,256]:case(op=4,chain=chain,graph=3)
for route in [0,1,257,999,65535]:case(op=8,route=route)
# Every BYTE actor kind is checked at both bridge dispatches; secondary state
# actions include non-selected actor types installed by an actual helper exit.
for type in range(256):
    case(op=3,type=type,terrain=8,bkind=0x88,look=1)
    case(op=3,type=type,terrain=8,bkind=0x88,look=1,occ=0)
for dir in [8,9,11,127]:case(op=3,dir=dir)
for usage in [0,1,2,3,4,5,6,7,11,255]:
    for water,terrain,ed in itertools.product([0,1],[0,4,0x400,0x10004],[-128,-1,0,127]):case(op=3,usage=usage,water=water,terrain=terrain,ed=ed,ee=255,eb=255,ec=255)
for ec,terrain,mode in itertools.product([0,1,2,3,4,5,6,127,128,255],[0,0x400],[0,1,2,3,4]):case(op=3,usage=2,ec=ec,terrain=terrain,mode=mode)
for eb in [0,1,127,128,255]:case(op=3,usage=4,eb=eb)
for ee,type,terrain in itertools.product([0,1,2,3,254,255],[1,2,3,4,94,95],[0,4,0x40,8,0x40000048]):case(op=3,ee=ee,type=type,terrain=terrain,bkind=0x38)
for terrain,flag in itertools.product([0,1,2,4,8,0x40,0x400,0x1000,0x4000,0x8000,0x40000000,0x40000448,0x40000048,0x80000000,0x10000008],[0,1,256]):
    for bkind in [0x3a,0xaa,0xab,0xc8,0xc9,0xca,0x8a,0x38,0x88,0x1234]:case(op=3,terrain=terrain,bkind=bkind,**{'global':flag})
for bkind,usage in itertools.product([0x38,0x36,0xb7,0xd1,0x88,0x89,0xffff],[0,1,7]):case(op=3,terrain=8,bkind=bkind,usage=usage,look=0)
for look,occ,pocc,state in itertools.product([0,1,2,3,MASK],[0,1,32767,32768],[0,1,65535],[0,7]):case(op=3,terrain=8,bkind=0x88,look=look,occ=occ,pocc=pocc,state=state)
for pos,length,path in [(0,1,2),(15,16,2),(16,16,2),(0,16,0),(0,16,7),(0,16,8),(0,16,9)]:case(op=3,terrain=8,bkind=0x88,look=2,pos=pos,length=length,path=path)
for cap in [1,2,3,16,51]:
    positions=range(cap) if cap<=16 else [0,1,25,49,50]
    for head,tail in itertools.product(positions,repeat=2):
        for op in [10,11,12]:case(op=op,qcap=cap,qhead=head,qtail=tail,qidx=(head*7+tail)&255,qval=0x80000101)
        if (tail+1)%cap==head:case(op=12,qcap=cap,qhead=head,qtail=tail,nullvalue=1)
for head,tail,cap in [(MASK,0,51),(0,MASK,51),(0x80000000,0x7fffffff,51),(0x7fffffff,0x80000000,MASK),(0,0,0)]:
    for op in [10,11]:case(op=op,qhead=head,qtail=tail,qcap=cap)
for qidx in [0,1,127,128,254,255]:case(op=3,terrain=8,bkind=0x88,look=1,qidx=qidx)
for qhead,qtail in [(1,0),(0,50),(50,49)]:case(op=3,terrain=8,bkind=0x88,look=1,qhead=qhead,qtail=qtail)
# Boundary interventions are declared once and performed at the same entry or
# exit in both implementations. They test captures and reloads, not path finding.
def mutate(fn,boundaryphase,bits=1,**kw):case(mfn=fn,mphase=boundaryphase,mbits=bits,**kw)
for phase in [0,1]:
    for speed in range(18):
        for counter in [0,1,2,3]:mutate(0x4b27e0,phase,bits=1|2,op=0,speed=speed,counter=counter,newcounter=254)
    for fn,op in [(0x4a5da0,0),(0x4b27e0,1),(0x4b33d0,2),(0x4b3530,3),(0x4b3c60,4),(0x4b52f0,5),(0x4b5420,6),(0x4b3f00,7),(0x4b3250,8),(0x51cba0,9),(0x51f780,13),(0x4f6630,10),(0x4f6650,11),(0x4f65b0,12)]:mutate(fn,phase,op=op)
    for fn in [0x4baa10,0x4b2d20]:mutate(fn,phase,bits=1|4|32,op=1,phase=14,route=0,newroute=2,newdir=2)
    for fn in [0x4ad880,0x4b3470,0x4b1210,0x4690f0]:
        kw={0x4ad880:dict(op=3,ee=1),0x4b3470:dict(op=3,terrain=8,bkind=0x88,look=1),0x4b1210:dict(op=4),0x4690f0:dict(op=3,usage=2,ec=1)}[fn]
        mutate(fn,phase,bits=1|2,**kw)
    for fn in [0x4f6630,0x4f6650,0x4f65b0]:mutate(fn,phase,bits=512,op=11 if fn==0x4f6630 else 12 if fn==0x4f65b0 else 3,terrain=8,bkind=0x88,look=1,newqtail=49,newqhead=1)
    mutate(0x4b5420,phase,bits=8,op=4,newdir=6)
    mutate(0x4b52f0,phase,bits=16,op=4,newtype=77)
    mutate(0x4b1210,phase,bits=64,op=4,newx=65535,newy=32768)
    mutate(0x4b3470,phase,bits=128|256,op=3,terrain=8,bkind=0x88,look=1)
    mutate(0x4b3470,phase,bits=2048,op=3,terrain=8,bkind=0x88,look=1,newstate=7)
    mutate(0x4f65b0,phase,bits=1024,op=3,terrain=8,bkind=0x88,look=1)
    mutate(0x4ad880,phase,bits=64,op=3,ee=1,newtile=22902)
    mutate(0x4b3250,phase,bits=16,op=3,terrain=8,bkind=0x88,look=1,occ=0,newtype=108)
for type in [0,5,11,39,73,108,255]:mutate(0x4b3250,1,bits=16,op=3,terrain=8,bkind=0x88,look=1,occ=0,newtype=type)
for phase in [0,1]:
    mutate(0x4ad880,phase,bits=128|256,op=3,ee=1,terrain=8,bkind=0x88,newterrain=0x10000008)
    mutate(0x51f780,phase,bits=1|64,op=6,chain=2,graph=0)
    mutate(0x4b3250,phase,bits=32,op=8)
mutate(0x4f6630,1,bits=512,op=11,newqhead=0,newqtail=0,newqcap=1)
mutate(0x4f6650,1,bits=512,op=3,terrain=8,bkind=0x88,look=1,newqhead=1,newqtail=0)
case(op=3,usage=6,ee=0)
case(op=3,ee=1,type=0)
case(op=2,route=0,tx=100,ty=100)
for path in [8,9,10,127]:case(op=1,phase=14,path=path)
for route,pos,length in itertools.product([0,1,999,65535],[0,499],[0,500,65535]):case(op=1,phase=14,route=0,build_route=route,build_pos=pos,build_len=length)
for steps in [0,-1,-2147483648]:case(op=1,steps=steps,**{'global':0x100})
case(op=1,phase=14,route=999,pos=65535,length=16)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-actor-walking/Debug/actor_walking_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-actor-walking/Release/actor_walking_verify.exe')
oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in [exe]+([optimized] if optimized else [])]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'limit':args.limit,'start':args.start,'single_case':bool(args.case)},'method':'Complete walking scheduler/step/gate/tile/grid core plus actual subpixel, route release, heading, shared RET and explicit-receiver queue algorithms against compiled C. Original instructions, ordered meaningful arguments/results and state, full watched memory/guards, boundary mutations and native/C stack/saved registers compared.','limitations':['Route builder4B2D20, broad service4BAA10, relocation search4AD880, bridge lookahead4B3470, collision4B1210 and terrain effect4690F0 remain explicitly controlled deeper endpoints. Their production algorithms and alternate movement4B3FF0 are outside this proof.','Prepared serialized routes, valid mapped records/queues and bounded initially acyclic grid lists form the fixture domain. No path-finding completeness, full actor-controller behavior, actual service success or city pixel fidelity is claimed.','Native member receivers are mapped to explicit C arguments. Queue push native RET4 and the shared RET ignored actor slot are checked and normalized; C adapter machine ABIs are not claimed identical.']}
checked=0;p=None;started=time.monotonic()
try:
    assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
    for p in cases:
        expected=native(p)
        for number,o in enumerate(oracles):
            o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
            if actual!=expected:(ROOT/'artifacts/actor_walking_last_difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
            assert actual==expected,repr({'oracle':number,'return':[actual['return'],expected['return']],'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):
        assert reachable<=hits,'Uncovered reachable instructions '+repr([hex(a) for a in sorted(reachable-hits)])
        missing=[(a,target) for a,targets in branches.items() for target in targets if(a,target) not in edges and(a,target) not in excluded_edges];assert not missing,'Uncovered feasible outcomes '+repr(missing)
        for instruction,address,count in dispatches:
            targets=set(struct.unpack('<%dI'%count,bytes(vm.mem_read(address,count*4))));seen_targets={b for a,b in edges if a==instruction};assert targets<=seen_targets,'Uncovered computed dispatch '+hex(instruction)
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='src' if args.production else 'artifacts/actor_walking_phase'
combat_prefix='src/unit_01435.c' if args.production else 'artifacts/actor_combat_phase/src/unit_01435.c';combat_header='include/actor_combat.h' if args.production else 'artifacts/actor_combat_phase/include/actor_combat.h'
sources=[prefix+'/unit_'+n+'.c' for n in ['01387','01423','01431','01433','01434','01451','01453']]+[combat_prefix,combat_header,'src/unit_01427.c','src/unit_02332.c','src/unit_02351.c']+['runtime/actor_walking_'+n+'.c' for n in ['facades','queue_count','queue_full','queue_push']]+['include/'+n+'.h' for n in ['actor_walking','actor_callbacks_human','restore_types','standalone_data']]+['tests/actor_walking.c','tests/actor_walking_real.c','tests/actor_walking_case.h','tools/actor_walking_project/CMakeLists.txt','tools/verify_actor_walking_emulation.py']+['metadata/'+name for name,_ in packets]+['metadata/actor-empty-callback-native.json']
missing_edges=[(a,target) for a,targets in branches.items() for target in targets if(a,target) not in edges and(a,target) not in excluded_edges]
dispatch_result=[]
for instruction,address,count in dispatches:
    raw=bytes(vm.mem_read(address,count*4));targets=set(struct.unpack('<%dI'%count,raw));seen_targets={b for a,b in edges if a==instruction};dispatch_result.append({'instruction':hex(instruction),'table':hex(address),'sha256':sha(raw),'covered':len(targets&seen_targets),'targets':len(targets),'uncovered':[hex(a) for a in sorted(targets-seen_targets)]})
report.update(comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],reachable_instruction_coverage=[len(hits&reachable),len(reachable)],owned_instruction_coverage=[len(hits&owned),len(owned)],owned_reachable_instruction_coverage=[len(hits&(owned-dead)),len(owned-dead)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},excluded_native_instructions=[hex(a) for a in sorted(dead)],exclusion_evidence=dead_reason,native_uncovered_instructions=[hex(a) for a in sorted(reachable-hits)],owned_feasible_conditional_outcomes=[len(branches)*2-len(excluded_edges)-len(missing_edges),len(branches)*2-len(excluded_edges)],missing_owned_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],computed_dispatch=dispatch_result,controlled_algorithm_endpoints=[hex(a) for a in controlled],reached_controlled_endpoints=[hex(a) for a in sorted(deep)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
report['heading_return_contract']={'consumed':'AL BYTE0..8; unchanged2332 widened C result is compared only in this consumed domain','native_raw':'(Y & FFFFFF00) | direction','raw_domain_asserted_samples':len(heading_raw),'raw_samples':[[hex(y),hex(raw)] for y,raw in sorted(heading_raw)]}
(ROOT/'metadata/actor-walking-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_owned_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
