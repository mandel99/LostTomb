"""Exact-pixel leftward RLE decoder plus actual row-skip helper versus original x86."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--plain-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ
from unicorn.x86_const import *
MASK=0xffffffff;SRC=0x31000000;DST=0x31100000;CAP=0x80000;STOP=0x33000000;STACK=0x34000000
sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768;s32=lambda v:(v^0x80000000)-0x80000000
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(SRC,CAP),(DST,CAP),(STOP,4096),(STACK,0x10000)]:vm.mem_map(a,n)
vm.mem_protect(SRC,CAP,UC_PROT_READ)
main=json.loads((ROOT/'metadata/flat-sprite-raster-next-native.json').read_text())['functions']['0040fc40']['assembly']['items']
helper=json.loads((ROOT/'metadata/upper-raster-native.json').read_text())['functions']['00410f20']['assembly']['items']
allowed=sorted(int(i['address'],16) for i in main+helper);allowed_set=set(allowed);main_set={int(i['address'],16) for i in main};helper_set={int(i['address'],16) for i in helper}
for i in main+helper:
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
regions=[(SRC,CAP),(DST,CAP),(0xd3b520,224),(0xe38d88,32),(0xe92e10,32)]
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
hits=set();block_cache={};events=[];params=None;write_count=0;write_hash=0;mutated=set()
def block(cpu,a,n,user):
    assert a in allowed_set,'Unexpected original block '+hex(a)
    if(a,n) not in block_cache:block_cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
    hits.update(block_cache[a,n])
def observe(phase,source):
    events.append([phase,source,read(0xd3b5be,2),read(0xd3b538),read(0xd3b5d4),read(0xd3b5e0),read(0xd3b5dc),read(0xe38d94),read(0xe92e18),read(0xd3b53c)])
    if params[11]&(1<<phase):
        mutated.add(phase)
        for a,v,n in [(0xd3b5be,params[12],2),(0xd3b5d4,params[13],4),(0xd3b5e0,params[14],4),(0xd3b5dc,params[15],4),(0xe38d94,params[16],4),(0xd3b53c,params[17],4),(0xd3b538,params[18],4)]:write(a,v,n)
def helper_entry(cpu,a,n,user):observe(0,read(cpu.reg_read(UC_X86_REG_ESP)+4))
def helper_exit(cpu,a,n,user):observe(1,cpu.reg_read(UC_X86_REG_EAX))
def memory_write(cpu,access,a,n,value,user):
    global write_count,write_hash
    if STACK<=a and a+n<=STACK+0x10000:return
    assert DST<=a and a+n<=DST+CAP and n==2,'Native write outside frame '+hex(a)
    write_hash=((write_hash^a)*1099511628211)&0xffffffffffffffff;write_hash=((write_hash^(value&65535))*1099511628211)&0xffffffffffffffff;write_count+=1
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_CODE,helper_entry,begin=0x410f20,end=0x410f20);vm.hook_add(UC_HOOK_CODE,helper_exit,begin=0x410f6b,end=0x410f6b);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
initial_frame=bytes(((i*13+(i>>7)*7)^0xc7)&255 for i in range(CAP))
def source_data(p):
    width=s16(p[12] if p[11]&1 else p[0]);rows=max(s32(p[1]),0)+max(s32(p[5]),0)+max(s32(p[13]),0)+max(s32(p[18]),0)+2
    pattern=p[7];data=bytearray(bytes([1 if pattern in [6,8] else 0xa5])*CAP);cursor=p[9]
    for row in range(rows if width>0 else 0):
        col=0
        while col<width:
            if pattern in [4,8] or pattern==11 and col>0:data[cursor]=0;cursor+=1
            if pattern==5:data[cursor:cursor+2]=b'\xff\0';cursor+=2
            run=1 if pattern==0 else 254 if pattern==1 else 1+(row+col+p[8])%16
            if pattern in [6,8]:run=min(254,width+7)
            if pattern in [2,7]:run=255
            if pattern==7:run=min(255,width+7)
            if pattern in [10,11]:run=(130 if pattern==10 else 128) if col==0 else 17
            if pattern not in [6,7,8]:run=min(run,width-col)
            transparent=pattern in [2,7] or pattern==3 and(row+col+p[8])%3==0 or pattern==10 and col>0
            assert cursor+run*2+4<CAP,'Source fixture capacity'
            data[cursor]=255 if transparent else run;cursor+=1
            if transparent:data[cursor]=run;cursor+=1
            else:
                for i in range(run):struct.pack_into('<H',data,cursor,0x0101 if pattern in [6,8] else((row*width+col+i)*37+p[8])&65535);cursor+=2
            col+=run
    return bytes(data)
def native(p):
    global params,write_count,write_hash
    params=p;events.clear();write_count=0;write_hash=14695981039346656037
    vm.mem_write(SRC,source_data(p));vm.mem_write(DST,initial_frame)
    for i,(a,n) in enumerate(regions[2:],2):vm.mem_write(a,bytes([0x5a+i])*n)
    for a,v,n in [(0xd3b5be,p[0],2),(0xd3b5d4,p[1],4),(0xd3b5e0,p[2],4),(0xd3b5dc,p[3],4),(0xe38d94,p[4],4),(0xd3b538,p[5],4),(0xd3b53c,p[6],4),(0xe92e18,DST+p[10],4)]:write(a,v,n)
    for repeat in range(p[19]):
        sp=STACK+0x8000;vm.mem_write(sp-0x100,b'\xcc'*0x300);write(sp,STOP);write(sp+4,SRC+p[9])
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start(0x40fc40,STOP,count=6000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget';assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack cleanup';assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
        assert bytes(vm.mem_read(sp+8,0x100))==b'\xcc'*0x100,'Native caller stack guard'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return{'memory':h.hexdigest(),'source':vm.reg_read(UC_X86_REG_EAX),'write_count':write_count,'write_hash':format(write_hash,'016x'),'events':events.copy()}
default=[7,2,300,8,2048,1,2034,3,123,17,0x10000,0,5,1,301,9,2048,2038,0,1];cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    if('p0' in kw or 'p4' in kw) and 'p6' not in kw:p[6]=(p[4]-s16(p[0])*2)&MASK
    cases.append(tuple(p))
case()
for width,height,skip,pattern in itertools.product([1,2,7,31,58,118,257,358],[0,1,3],[0,1,4],range(10)):case(p0=width,p1=height,p5=skip,p7=pattern)
for width,height,skip in itertools.product([0,65535,32768,65536,0x10000007],[-1,0,1,4,256],[-1,0,1,4]):case(p0=width,p1=height,p5=skip,p4=64 if height==256 else 2048,p3=0 if height==256 else 8)
for width,height,skip,pattern in itertools.product([32767],[0,1],[0,1],[1,2]):case(p0=width,p1=height,p5=skip,p7=pattern,p2=0,p3=0)
for x,y,pitch in itertools.product([-1,0,300,1000,0x80000000,0x80000001],[-9,0,8,0x80000000],[1024,2048,4096]):case(p2=x,p3=y,p4=pitch)
for pitch,y in itertools.product([-1024,-2048],[0,8,-1]):case(p4=pitch,p3=y)
for width,delta in itertools.product([0,1,31,257],[-256,-1,0,1,256]):case(p0=width,p6=2048-width*2+delta)
for frame,source in itertools.product([0x10000,0x20000,0x30000],[0,1,17,255,256,4096]):case(p10=frame,p9=source)
for phase,width,pattern in itertools.product([1,2,3],[0,1,5,65535,358],[1,3,6,7]):case(p11=phase,p12=width,p17=2048-s16(width)*2,p7=pattern)
for height in [0x80000000,0xffffffff,0,1,256]:case(p1=height,p4=64,p3=0)
for repeat,phase in itertools.product([1,2],[0,1,2,3]):case(p19=repeat,p11=phase)
cases=list(dict.fromkeys(cases))
for width,skip,height,pattern in itertools.product([129,257,358],[0,1,2],[0,1,3],[10,11]):case(p0=width,p5=skip,p1=height,p7=pattern)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-mirrored-raster/Release/mirrored_raster_verify.exe';process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
plain_exe=args.plain_exe or exe;plain=subprocess.Popen([str(plain_exe),'--oracle-batch-plain'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'start':args.start,'limit':args.limit,'single_case':bool(args.case)},
 'method':'Actual64-instruction mirrored RLE decoder plus actual32-instruction row skip against compiled traced and plain C, no controlled algorithms. Complete source/frame/state+canary SHA, ordered WORD pixel writes/count, final source cursor, real helper entry/exit mutations, stack/saved registers compared.',
 'limitations':['Valid mapped source/frame fixtures; no arbitrary malformed-stream termination guarantee or entire-city pixel fidelity claim.','Public decoder remainsvoid. Test-only trace exposes source cursor and ordered pixels; native incidental EAX is compared as an internal consumption observation. Clear direction flag is required by the native ABI.']}
checked=0;p=None;started=time.monotonic()
try:
    assert json.loads(process.stdout.readline())=={'fields':20};assert json.loads(plain.stdout.readline())=={'fields':20}
    for p in cases:
        expected=native(p);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read();actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/mirrored_raster_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected},indent=2)+'\n')
        assert actual==expected,repr({'C':actual,'native':expected})
        plain.stdin.write(' '.join(map(str,p))+'\n');plain.stdin.flush();line=plain.stdout.readline();assert line,'Plain C exited: '+plain.stderr.read();untraced=json.loads(line)
        assert untraced['source']==0 and untraced['write_count']==0 and untraced['write_hash']=='cbf29ce484222325','Plain body unexpectedly traced'
        assert {k:untraced[k] for k in ['memory','events']}=={k:expected[k] for k in ['memory','events']},'Plain C pixels/state/helper observations differ'
        checked+=1
        if checked%100==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):assert hits==allowed_set,'Uncovered '+repr([hex(a) for a in sorted(allowed_set-hits)]);assert mutated=={0,1},'Unreached helper mutations'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10);plain.stdin.close();plain.wait(timeout=10)
prefix='src' if args.production else 'artifacts/mirrored_raster_phase'
sources=[prefix+'/unit_00131.c',prefix+'/unit_00149.c','include/mirrored_raster.h','include/standalone_data.h','tests/mirrored_raster.c','tests/mirrored_raster_real_skip.c','tests/mirrored_raster_plain.c','tools/mirrored_raster_project/CMakeLists.txt','tools/verify_mirrored_raster_emulation.py','metadata/flat-sprite-raster-next-native.json','metadata/upper-raster-native.json']
report.update(plain_checked=checked,plain_compiled_C_sha256=sha(plain_exe.read_bytes()),elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits&main_set),len(main_set)],helper_instruction_coverage=[len(hits&helper_set),len(helper_set)],native_uncovered_instructions=[hex(a) for a in sorted(allowed_set-hits)],helper_mutation_phases=sorted(mutated),controlled_algorithm_endpoints=[],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/mirrored-raster-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
