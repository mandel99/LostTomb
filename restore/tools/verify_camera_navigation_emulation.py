"""Real camera navigation, lookup, reset and clamp chain versus pinned x86 instructions."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
MASK=0xffffffff;CELLS=231*457;STACK=0x34000000;STOP=0x33000000
sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
functions={}
packets=[('camera-navigation-next-native.json',['00430cc0','00430cd0','004304c0','004de140']),('camera-sidebar-coordinate-native.json',['004de080']),('camera-bounds-helpers-native.json',['00430ca0','00430cb0','004ddea0','004ddeb0'])]
for name,entries in packets:
    packet=json.loads((ROOT/'metadata'/name).read_text())['functions']
    for fn in entries:
        body=packet[fn]['assembly'];assert not body['has_more'];functions[int(fn,16)]=body['items']
allowed=sorted(int(i['address'],16) for body in functions.values() for i in body);allowed_set=set(allowed)
for body in functions.values():
    for i in body:
        b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
regions=[(0x70e440,80),(0x70e484,CELLS*4+32),(0xbca2b0,96),(0xd3b4d0,48),(0xe94db0,128),(0x6fdc40,32),(0x605a30,32),(0xe38dc0,32)]
observed=[0x70e458,0x70e460,0x70e464,0xd3b4e0,0xd3b4e4,0xbca2bc,0xbca2c0,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x6fdc50,0x605a3c,0xe38dcc,0xe94ddc,0xe94de0,0xe94df4,0xe94df8,0xe94dfc,0x70e494,0x77560c,0xe94e00,0xbca2c4]
returning={0x430cc0,0x430ca0,0x430cb0,0x4ddea0,0x4ddeb0}
with_argument={0x430cd0,0x4304c0}
hits=set();block_cache={};params=None;events=[];callstack=[];mutated=set();mutation_seen=0
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def observe(fn,phase,arg,result):
    global mutation_seen
    events.append([fn,phase,arg,result]+[read(a) for a in observed]+[read(0xe38dca,1)])
    p=params
    if fn==p[19] and phase==p[20]:
        mutation_seen+=1
        if mutation_seen==p[21]:
            bits=p[22];mutated.add((fn,phase))
            if bits&1:write(0x70e464,p[23]);write(0x70e460,p[24]);write(0x70e458,p[33])
            if bits&2:write(0xd3b4e0,p[34]);write(0xd3b4e4,p[35])
            if bits&4:write(0xbca2bc,p[25]);write(0xbca2c0,p[26])
            if bits&8:write(0xe94dd0,p[27]);write(0xe94dc8,p[28]);write(0xe94dcc,p[29]);write(0xe94dc4,p[30])
            if bits&16:write(0x6fdc50,p[31]);write(0x605a3c,p[32])
            if bits&32:write(0xe38dca,0x81,1);write(0xe38dcc,0x10203040)
            if bits&64:write(0x70e494,p[3]);write(0x77560c,p[3])
            if bits&128:write(0xe94ddc,0x12345678);write(0xe94df8,0x87654321)
def entry(cpu,address,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4) if address in with_argument else 0
    callstack.append((address,sp,arg));observe(address,0,arg,0)
def leave(cpu,address,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);assert callstack and callstack[-1][1]==sp,'Native return stack'
    while callstack and callstack[-1][1]==sp:
        fn,_,arg=callstack.pop();observe(fn,1,arg,cpu.reg_read(UC_X86_REG_EAX) if fn in returning else 0)
def block(cpu,address,n,user):
    assert address in allowed_set,'Unexpected native block '+hex(address)
    if (address,n) not in block_cache:block_cache[address,n]=allowed[bisect.bisect_left(allowed,address):bisect.bisect_left(allowed,address+n)]
    hits.update(block_cache[address,n])
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a and a+n<=STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
for fn,body in functions.items():
    vm.hook_add(UC_HOOK_CODE,entry,begin=fn,end=fn)
    for i in body:
        if i['instruction'].startswith('RET'):
            a=int(i['address'],16);vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
grid_cache={}
def grid(p):
    key=tuple(p[2:6])
    if key in grid_cache:return grid_cache[key]
    values=list(range(CELLS)) if p[2]==1 else [p[3] if p[2]==2 else 0xfff0000]*CELLS
    for i in [p[4],p[5]]:
        if i<CELLS:values[i]=p[3]
    b=struct.pack('<%dI'%CELLS,*values)
    if len(grid_cache)<100:grid_cache[key]=b
    return b
def native(p):
    global params,mutation_seen
    params=p;events.clear();callstack.clear();mutation_seen=0
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[1]+i*29)&255])*n)
    vm.mem_write(0x70e494,grid(p))
    for a,v in zip([0x70e464,0x70e460,0x70e458,0xbca2bc,0xbca2c0,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x6fdc50,0x605a3c],p[6:17]):write(a,v)
    write(0xe38dca,p[17],1);write(0xe38dcc,p[18])
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    result=0
    sequence=([0x4de140] if p[37] else [])+[[0x430cc0,0x430cd0,0x4304c0,0x4de140,0x4de080][p[0]]]*p[36]
    for fn in sequence:
        sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
        if fn in with_argument:write(sp+4,p[3])
        for reg,v in saved:vm.reg_write(reg,v)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start(fn,STOP,count=2000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP and not callstack,'Native execution budget/callstack'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
        assert all(vm.reg_read(reg)==v for reg,v in saved),'Native saved register'
        guard=sp+8 if fn in with_argument else sp+4
        assert bytes(vm.mem_read(guard,0x100))==b'\xcc'*0x100,'Native caller guard'
        if fn!=0x4de140 or not p[37]:result=vm.reg_read(UC_X86_REG_EAX) if p[0]==0 else 0
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'return':result,'events':events.copy()}
default=[0,1,0,12345,0,MASK,14,30,20,100,100,0,0,500,500,0,1,0x80,0x10203040,0,0,1,0,0x10003,0xfffffffd,123,125,5,7,400,450,1,1,0x12345678,456,230,1,0]
cases=[]
def case(**kw):
    p=default.copy()
    for key,value in kw.items():p[int(key[1:])]=value&MASK
    cases.append(tuple(p))
for op in range(5):case(p0=op,p37=1)
for rows,width in itertools.product([0,1,2,255,256,65535,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff],[0,1,2,255,256,65535,0x10000,0x7fffffff,0x80000000,0xffffffff]):case(p0=0,p8=rows,p6=width)
for width,height,editor,mode in itertools.product([-3,-2,-1,0,1,2,3,255,256,65535,65536,0x7fffffff,0x80000000],[1,3,30,256,-3,0x80000000],[0,1,0x100],[0,1,0x100]):case(p6=width,p7=height,p15=editor,p16=mode)
for position in [0,1,230,231,232,456,457,51983,51984,105104,105335,CELLS-2,CELLS-1,MASK]:
    case(p0=2,p4=position)
    case(p0=1,p4=position,p37=1)
for target in [0,1,51983,51984,0xffeffff,0xfff0000,0x7fffffff,0x80000000,MASK]:
    case(p0=2,p3=target,p4=0,p5=1)
    case(p0=1,p3=target,p4=0,p5=231)
for fn in functions:
    for phase,bits in itertools.product([0,1],[1,2,4,8,16,32,64,128,255]):
        op=0 if fn==0x430cc0 else 3 if fn==0x4de140 else 1
        case(p0=op,p19=fn,p20=phase,p22=bits,p37=1)
for fn in [0x430ca0,0x430cb0,0x4ddea0,0x4ddeb0]:
    for phase,occurrence in itertools.product([0,1],[1,2]):case(p19=fn,p20=phase,p21=occurrence,p22=255,p6=1000,p7=1000,p13=1,p14=1)
for target in [0,51983,CELLS-1,MASK]:case(p0=2,p2=1,p3=target,p4=MASK)
case(p0=2,p2=2,p3=12345,p4=MASK);case(p0=1,p4=0,p5=1,p36=2,p37=1)
rng=random.Random(0x430cd0)
for i in range(200):
    case(p6=rng.getrandbits(32),p7=rng.getrandbits(32),p11=rng.getrandbits(32),p12=rng.getrandbits(32),p13=rng.getrandbits(32),p14=rng.getrandbits(32),p15=i%2,p16=i%3,p17=i)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-camera-navigation/Debug/camera_navigation_verify.exe'
optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-camera-navigation/Release/camera_navigation_verify.exe')
oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in [exe]+([optimized] if optimized else [])]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'limit':args.limit,'start':args.start,'single_case':bool(args.case)},'method':'Complete native area/navigation and actual lookup/reset/clamp/dimension chain versus compiled C. Ordered real helper entry/exit arguments, consumed raw returns, full lookup/state memory with canaries, injected state mutations and native/C cdecl stack/saved registers compared.','limitations':['Pixel-free fixture proof; no scrolling input dispatcher, map initialization or city pixel fidelity is claimed.','Reset is composed as an explicit fixture sequence where requested; navigation itself calls lookup then tail-clamp and does not call reset.','All native lookup cells are present and scanned by the missing-target cases; individual target positions are bounded fixtures rather than an exhaustive proof of every target value. Void incidental EAX is not asserted.']}
checked=0;p=None;started=time.monotonic()
try:
    assert all(json.loads(o.stdout.readline())=={'fields':38} for o in oracles)
    for p in cases:
        expected=native(p)
        for number,o in enumerate(oracles):
            o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C oracle exited: '+o.stderr.read();actual=json.loads(line)
            if actual!=expected:(ROOT/'artifacts/camera_navigation_last_difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
            assert actual==expected,repr({'oracle':number,'memory':[actual['memory'],expected['memory']],'return':[actual['return'],expected['return']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):
        assert hits==allowed_set,'Uncovered '+repr([hex(a) for a in sorted(allowed_set-hits)])
        assert mutated==set(itertools.product(functions,[0,1])),'Unreached mutation phases '+repr(set(itertools.product(functions,[0,1]))-mutated)
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='src' if args.production else 'artifacts/camera_navigation_phase'
sources=[prefix+'/unit_'+n+'.c' for n in ['00466','00467','01886']]+['src/unit_'+n+'.c' for n in ['00458','01887','00464','00465','01880','01881']]+['include/camera_navigation.h','include/camera_bounds.h','include/restore_types.h','include/standalone_data.h','tests/camera_navigation.c','tests/camera_navigation_real.c','tools/camera_navigation_project/CMakeLists.txt','tools/verify_camera_navigation_emulation.py']+['metadata/'+name for name,_ in packets]
owned={int(i['address'],16) for fn in [0x430cc0,0x430cd0] for i in functions[fn]}
report.update(comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(allowed_set)],owned_instruction_coverage=[len(hits&owned),len(owned)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_uncovered_instructions=[hex(a) for a in sorted(allowed_set-hits)],controlled_algorithm_endpoints=[],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],full_grid_cells=CELLS,source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/camera-navigation-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
