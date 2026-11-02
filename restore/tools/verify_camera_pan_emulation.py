"""Native camera cadence/direction/movement with real cache decay and clamp composition."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
MASK=0xffffffff;STACK=0x34000000;STOP=0x33000000;CLOCK=0x35000000;sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
    q=table+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(STACK,0x100000),(STOP,4096),(CLOCK,4096)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def string(a):
    b=bytearray()
    while read(a,1):b.append(read(a,1));a+=1
    return b.decode('ascii')
imports={};descriptor=base+u32(opt+104)
while read(descriptor+12):
    dll=string(base+read(descriptor+12));lookup=read(descriptor) or read(descriptor+16);iat=read(descriptor+16);j=0
    while read(base+lookup+j*4):
        name=read(base+lookup+j*4)
        imports[base+iat+j*4]=(dll,str(name&65535) if name&0x80000000 else string(base+name+2));j+=1
    descriptor+=20
assert imports[0x56f22c][1]=='timeGetTime';write(0x56f22c,CLOCK)
packets=[('camera-pan-chain-native.json',None),('camera-sidebar-coordinate-native.json',['004de080']),('camera-bounds-helpers-native.json',['00430ca0','00430cb0','004ddea0','004ddeb0'])]
functions={}
for name,entries in packets:
    packet=json.loads((ROOT/'metadata'/name).read_text())['functions']
    for fn in entries or packet:
        body=packet[fn]['assembly'];assert not body['has_more'];functions[int(fn,16)]=body['items']
instructions={int(i['address'],16):i for body in functions.values() for i in body};allowed=sorted(instructions)
for a,i in instructions.items():
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
owned={int(i['address'],16) for fn in [0x4cf660,0x4ddd60,0x4ddde0] for i in functions[fn]}
cap=Cs(CS_ARCH_X86,CS_MODE_32);branches={}
for a in owned:
    i=next(cap.disasm(bytes.fromhex(instructions[a]['bytes']),a))
    if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]=(a+i.size,int(i.op_str,16))
regions=[(0xe38da0,0x100),(0xe94da0,128),(0xe92e10,64),(0x5e2a50,32),(0xbca2b0,96),(0x70e450,64),(0x6fdc40,32),(0x605a30,32),(0x702970,0x22b0)]
watched=[0xe94db4,0xe38e70,0xe38db8,0xe38db4,0xe38dc4,0xe38dc0,0xe92e2c,0xe92e30,0xe92e34,0xe92e38,0xe38dcc,0x5e2a60,0xbca2bc,0xbca2c0,0x70e464,0x70e460,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x605a3c,0x6fdc50,0x702984,0x704b84,0x70298c,0x704b8c]
returning={0x4cf660,0x4ddd60,0x4ddea0,0x4ddeb0,0x430ca0,0x430cb0}
params=None;events=[];frames=[];hits=set();edges=set();mutated=set();seen=0;clock_now=0;last_block=None;block_cache={}
def observe(fn,phase,arg,result=0):
    global seen,clock_now
    events.append([fn,phase,arg,result]+[read(a) for a in watched]+[read(0xe38dca,1)])
    p=params
    if fn==p[29] and phase==p[30]:
        seen+=1
        if seen==p[31]:
            bits=p[32];mutated.add((fn,phase))
            if bits&1:clock_now=p[33]
            if bits&2:write(0xe94db4,p[34]);write(0xe38e70,p[35])
            if bits&4:write(0xbca2bc,p[36]);write(0xbca2c0,p[37])
            if bits&8:write(0x70e464,p[38]);write(0x70e460,p[39])
            if bits&16:write(0x605a3c,p[40]);write(0x6fdc50,1)
            if bits&32:
                for a,bit in [(0xe92e2c,1),(0xe92e30,2),(0xe92e34,4),(0xe92e38,8)]:write(a,256 if p[41]&bit else 0)
            if bits&64:write(0xe38dc4,p[36]);write(0xe38dc0,p[37]);write(0xe38db8,p[38]);write(0xe38db4,p[39])
            if bits&128:write(0x5e2a60,p[42])
            if bits&256:write(0xe38dcc,0x87654321);write(0xe38dca,0x80,1)
            if bits&512:write(0x702984,0xfffffffd);write(0x70298c,0x11110000);write(0x704b84,0x80000001);write(0x704b8c,0x22220000)
def entry(cpu,a,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4) if a==0x4ddde0 else 0;frames.append((a,sp,arg));observe(a,0,arg)
def leave(cpu,a,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native return frame'
    while frames and frames[-1][1]==sp:
        fn,_,arg=frames.pop();observe(fn,1,arg,cpu.reg_read(UC_X86_REG_EAX) if fn in returning else 0)
def host_clock(cpu,a,n,user):
    global clock_now
    sp=cpu.reg_read(UC_X86_REG_ESP);observe(CLOCK,0,0);r=clock_now;clock_now=(clock_now+params[27])&MASK;observe(CLOCK,1,0,r);cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def block(cpu,a,n,user):
    global last_block
    if a==CLOCK:return
    assert a in instructions,'Native escaped '+hex(a)
    if last_block in branches:edges.add((last_block,a))
    if(a,n) not in block_cache:block_cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
    block_instructions=block_cache[a,n];hits.update(block_instructions);last_block=block_instructions[-1]
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a and a+n<=STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write);vm.hook_add(UC_HOOK_CODE,host_clock,begin=CLOCK,end=CLOCK)
for fn,body in functions.items():
    vm.hook_add(UC_HOOK_CODE,entry,begin=fn,end=fn)
    for i in body:
        if i['instruction'].startswith('RET'):
            a=int(i['address'],16);vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
def native(p):
    global params,seen,clock_now,last_block
    params=p;seen=0;clock_now=p[2];events.clear();frames.clear()
    for j,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[1]+j*29)&255])*n)
    for j in range(69):
        for k in range(32):write(0x702984+j*128+k*4,(j*0x1020304+k*0x1234567+p[1])^(0x80000001 if j&1 else MASK))
    for a,v in zip([0xe94db4,0xe38e70,0xe38db8,0xe38db4,0xe38dc4,0xe38dc0,0xe92e2c,0xe92e30,0xe92e34,0xe92e38,0xe38dcc,0x5e2a60,0xbca2bc,0xbca2c0,0x70e464,0x70e460,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x6fdc50,0x605a3c],p[3:25]):write(a,v)
    write(0xe38dca,p[25],1)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    result=0
    def execute(fn,arg=0):
        global last_block
        sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
        if fn==0x4ddde0:write(sp+4,arg)
        for reg,v in saved:vm.reg_write(reg,v)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);last_block=None;vm.emu_start(fn,STOP,count=100000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native execution budget/frames'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack';assert all(vm.reg_read(reg)==v for reg,v in saved),'Native saved registers'
        guard=sp+8 if fn==0x4ddde0 else sp+4;assert bytes(vm.mem_read(guard,128))==b'\xcc'*128,'Native caller guard';return vm.reg_read(UC_X86_REG_EAX)
    for repeat in range(p[28]):
        if p[0] in [0,3]:result=execute(0x4cf660)
        elif p[0]==1:result=execute(0x4ddd60)
        elif p[0]==2:execute(0x4ddde0,p[26]);result=0
        elif p[0]==4:execute(0x41d6f0);result=0
        else:execute(0x4de080);result=0
        if p[0]==3:execute(0x4ddde0,result)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'return':result,'memory':h.hexdigest(),'events':events.copy()}
default=[0,1,1000,0,100,1024,768,512,384,0,0,0,0,0xaabbccdd,1,100,100,14,30,0,0,500,500,0,1,0x80,0,1,1,0,0,1,0,2000,1990,50,1000,1001,256,257,1,15,256]
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
case()
for op,flags,magnitude in itertools.product([0,3],range(16),[1,256,0x80000000]):
    for x,y in [(512,384),(-1,384),(0,384),(1023,384),(1024,384),(512,0),(512,767),(512,768),(0,0),(1023,767)]:case(p0=op,p7=x,p8=y,p9=magnitude if flags&1 else 0,p10=magnitude if flags&2 else 0,p11=magnitude if flags&4 else 0,p12=magnitude if flags&8 else 0)
for speed in [-0x80000000,-1000,-1,0,1,9,10,11,49,50,51,89,90,91,99,100,101,109,110,111,255,256,65535,0x7fffffff]:
    for now,last in [(0,0),(1,0),(2,0),(109,0),(110,0),(10000,0),(0,1),(0,0xffffffff),(1,0xffffffff),(0x7fffffff,0),(0x80000000,0),(0xffffffff,0),(0,0x7fffffff),(0,0x80000000),(0,0x80000001)]:case(p0=1,p4=speed,p2=now,p3=last)
for speed in range(101):
    threshold=((100-speed)//10)*12+2
    for elapsed in [threshold-1,threshold,threshold+1]:case(p0=1,p4=speed,p2=elapsed)
for direction,step,x,y in itertools.product([-1,0,1,2,3,4,5,6,7,8,9,0x100,0x7fffffff],[0,1,256],[0,100,255,256,0x7fffffff,0x80000000,0xffffffff],[0,100,255,256,0x7fffffff,0x80000000,0xffffffff]):case(p0=2,p26=direction,p14=step,p15=x,p16=y)
for width,height,cursor in itertools.product([0,1,256,0x7fffffff,0x80000000,0xffffffff],[0,1,256,0x7fffffff,0x80000000,0xffffffff],[-1,0,1,255,256,0x7fffffff,0x80000000]):case(p5=width,p6=height,p7=cursor,p8=cursor)
for fn in list(functions)+[CLOCK]:
    for phase,bits in itertools.product([0,1],[1,2,4,8,16,32,64,128,256,512,1023]):
        op=1 if fn in [CLOCK,0x4ddd60] else 0 if fn==0x4cf660 else 2
        case(p0=op,p29=fn,p30=phase,p32=bits,p26=3,p15=1000,p16=1000,p21=500,p22=500)
for fn in [0x4ddea0,0x4ddeb0,0x430ca0,0x430cb0]:
    for phase,occurrence in itertools.product([0,1],[1,2]):case(p0=2,p29=fn,p30=phase,p31=occurrence,p32=1023,p26=3,p15=1000,p16=1000)
for repeats,delta in itertools.product([1,2,3],[0,1,2,110,0x80000000]):case(p0=3,p28=repeats,p27=delta,p10=256,p12=256)
rng=random.Random(0x4cf660)
for j in range(200):case(p0=j%4,p2=rng.getrandbits(32),p3=rng.getrandbits(32),p4=rng.getrandbits(32),p5=rng.getrandbits(32),p6=rng.getrandbits(32),p7=rng.getrandbits(32),p8=rng.getrandbits(32),p9=rng.getrandbits(32),p10=rng.getrandbits(32),p11=rng.getrandbits(32),p12=rng.getrandbits(32),p15=rng.getrandbits(32),p16=rng.getrandbits(32),p26=j%10-1)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-camera-pan/Debug/camera_pan_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-camera-pan/Release/camera_pan_verify.exe')
oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in [exe]+([optimized] if optimized else [])]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'limit':args.limit,'start':args.start,'single_case':bool(args.case)},'method':'Complete camera sampler/cadence/movement with actual69-record cache decay, camera clamp and dimension getters versus compiled C. Ordered real calls, raw consumed returns, full watched memory/canaries, callback mutations, native/C stack and saved registers compared.','limitations':['Only the Windows timeGetTime host clock is controlled, through its authenticated IAT slot and typed WINAPI C test adapter. No gameplay algorithm endpoint is substituted.','Fixture pipeline invokes sampler then movement with its full returned direction, matching the direct native caller pattern. No full window event loop, actual mouse sampling, wall-clock timing or city pixel fidelity is claimed.','Supported fixed mapped state is used, including full signed/wrapped DWORD probes; arbitrary invalid addresses or asynchronous data races are outside the proof.']}
checked=0;p=None;started=time.monotonic()
try:
    assert all(json.loads(o.stdout.readline())=={'fields':43} for o in oracles)
    for p in cases:
        expected=native(p)
        for number,o in enumerate(oracles):
            o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
            if actual!=expected:(ROOT/'artifacts/camera_pan_last_difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
            assert actual==expected,repr({'oracle':number,'return':[actual['return'],expected['return']],'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):
        assert hits==set(instructions),'Uncovered native instructions '+repr([hex(a) for a in sorted(set(instructions)-hits)])
        assert mutated==set(itertools.product(list(functions)+[CLOCK],[0,1])),'Unreached mutation phases '+repr(set(itertools.product(list(functions)+[CLOCK],[0,1]))-mutated)
        assert all((a,target) in edges for a,targets in branches.items() for target in targets),'Unreached owned conditional outcome'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='src' if args.production else 'artifacts/camera_pan_phase'
sources=[prefix+'/unit_'+n+'.c' for n in ['01771','01877','01879']]+['src/unit_'+n+'.c' for n in ['00281','01886','01880','01881','00464','00465']]+['include/'+n+'.h' for n in ['camera_pan','camera_bounds','restore_types','standalone_data']]+['tests/camera_pan.c','tests/camera_pan_real.c','tools/camera_pan_project/CMakeLists.txt','tools/verify_camera_pan_emulation.py']+['metadata/'+name for name,_ in packets]
missing=[(a,target) for a,targets in branches.items() for target in targets if(a,target) not in edges]
report.update(comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],owned_instruction_coverage=[len(hits&owned),len(owned)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],owned_conditional_outcomes=[len(branches)*2-len(missing),len(branches)*2],missing_owned_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing],controlled_algorithm_endpoints=[],host_clock_boundary={'IAT':'0x56f22c','DLL':imports[0x56f22c][0],'name':imports[0x56f22c][1],'ABI':'DWORD WINAPI timeGetTime(void)','oracle_token':hex(CLOCK)},mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],cache_records=69,cache_stride=128,source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/camera-pan-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
