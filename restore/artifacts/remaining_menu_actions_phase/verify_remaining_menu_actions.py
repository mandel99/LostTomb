"""Two remaining menu controllers, explicit ABI facades, real close, opaque deep callees."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--limit',type=int);args=parser.parse_args()
MASK=0xffffffff;PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(size,0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
facts=json.loads((HERE/'remaining_menu_native.json').read_text());assert facts['native_sha256']==PIN
scope={0x406090,0x40a0e0,0x489030,0x4c7f30};core={};traced=set();abi_only=0
for name,f in facts['functions'].items():
    assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
    for i in f['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        if int(name,16) in scope:
            core[a]=name
            if name!='004c7f30':traced.add(a)
        else:abi_only+=1
boundaries={0x478420:(0,False,0),0x486e70:(0,False,0),0x4dea50:(0,False,0),0x4be160:(4,True,16),0x488e90:(5,False,20),0x4e2970:(1,False,0),0x40a160:(0,False,0),0x539170:(0,False,0)}
state=[0xe38dda,0xe38dca,0x605a3c,0xbca2d0,0xbca2d4,0xd3b198,0xeaac28,0x605a44,0xe38ea4,0xe38ebc,0xf53f48,0xd3b17c,0xd3b180]
events=[];hits=set();params=None;calls=0;running=False
def snapshot():return [read(a,1 if i<2 else 4) for i,a in enumerate(state)]
def mutate():
    if not params[10]&(1<<calls):return
    bits=params[11]
    if bits&1:put(0xbca2d0,params[12]);put(0xbca2d4,params[13])
    if bits&2:put(0xd3b198,params[14])
    if bits&4:put(0xeaac28,params[15])
    if bits&8:put(0xe38dda,params[16],1)
    if bits&16:put(0xe38ebc,params[17])
    if bits&32:put(0xd3b17c,params[18]);put(0xd3b180,params[19])
    if bits&64:put(0x605a3c,params[20])
    if bits&128:put(0xe38dca,0xa6,1);put(0xf53f48,0xa5a55a5a)
def code(cpu,a,n,user):
    global calls
    if a==STOP:return
    if a in boundaries:
        count,receiver,cleanup=boundaries[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(count)]
        events.append([2,a,cpu.reg_read(UC_X86_REG_ECX) if receiver else 0,count]+argv+snapshot())
        mutate();calls+=1
        cpu.reg_write(UC_X86_REG_EAX,0xdeadbeef);cpu.reg_write(UC_X86_REG_ECX,0xc0dec0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe)
        cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);return
    assert a in core,f'escaped native instruction {a:08x}'
    hits.add(a)
    if a==0x489030:events.append([3,a,cpu.reg_read(UC_X86_REG_ECX)])
def memread(cpu,access,a,n,value,user):
    if running and cpu.reg_read(UC_X86_REG_EIP) in traced and not STACK<=a<STACK+0x100000:events.append([0,a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions(params)),f'unwatched native write {a:08x}/{n} at{cpu.reg_read(UC_X86_REG_EIP):08x}'
        if cpu.reg_read(UC_X86_REG_EIP) in traced:events.append([1,a,n,value&((1<<(8*n))-1)])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def regions(p):
    fixed=[(0x605a2c,48),(0xbca2c0,80),(0xd3a6d8,0xae0),(0xe38dc0,0x110),(0xeaac18,32),(0xf53f38,32),(0xd05710,48),(0xbca340,48)]
    return fixed+[((0x6fdc88+p[4]-16)&MASK,33),((0x6fdc88+p[14]-16)&MASK,33),((0xea623c+p[5]*0x49d8-16)&MASK,33),((0xea623c+p[15]*0x49d8-16)&MASK,33)]
def native(p):
    global params,events,calls,running
    params=p;events=[];calls=0;rs=regions(p)
    for i,(a,n) in enumerate(rs):vm.mem_write(a,bytes([(p[9]+i*37)&255])*n)
    for a,v,n in [(0xe38dda,p[1],1),(0x605a3c,p[2],4),(0xe38ebc,p[3],4),(0xd3b198,p[4],4),(0xeaac28,p[5],4),(0x6fdc88+p[4],p[6],1),(0x6fdc88+p[14],p[28],1),(0xbca2d0,p[7],4),(0xbca2d4,p[8],4),(0xd3b17c,p[21],4),(0xd3b180,p[22],4),(0xe38dca,p[23],1),(0xf53f48,p[24],4),(0xea623c+p[5]*0x49d8,p[26],1),(0xea623c+p[15]*0x49d8,p[27],1)]:put(a,v,n)
    sp=STACK+0xf0000;put(sp,STOP);put(sp+4,0xbaadf00d)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,p[25]);vm.reg_write(UC_X86_REG_ECX,p[25]^MASK);vm.reg_write(UC_X86_REG_EDX,p[25]^0x12345678);vm.reg_write(UC_X86_REG_EFLAGS,2)
    running=True
    try:vm.emu_start(0x406090 if p[0]==0 else 0x40a0e0,STOP,count=3000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(sp+4)==0xbaadf00d
    assert all(vm.reg_read(r)==v for r,v in saved)
    h=hashlib.sha256()
    for a,n in rs:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'events':events.copy()}
default=[0,1,1,1,7,2,0,228,228,17,0,0,123,124,311,3,0,0,0x12345678,1,0,3,0,0,0xffffffff,0xdeadbeef,255,128,0,0]
cases=[]
def add(**kw):
    p=default.copy()
    for name,v in kw.items():p[int(name[1:])]=v&MASK
    cases.append(p)
for mode,guard,w,h in itertools.product([0,1,2,5,255,256],[0,1,0x100,0x10001,0x80000001,MASK],[0,1,228,256,0x80000000,MASK],[0,1,228,257]):add(p1=mode,p2=guard,p7=w,p8=h)
for gate,index,flag,player,mode in itertools.product([0,1,256,0x80000000],[-1,0,1,255,256,511],[0,1,128,255],[-1,0,1,255,256,0x40000001],[1,5]):add(p0=1,p3=gate,p4=index,p6=flag,p5=player,p1=mode)
for which,when,bits in itertools.product([0,1],[1,2,4,8,16,31],[1,2,4,8,16,32,64,128,255]):
    add(p0=which,p10=when,p11=bits,p1=5 if which else 1,p3=0 if which else 1,p22=1)
    if which:add(p0=1,p10=when,p11=bits,p3=1,p5=256)
for saved,media,bits in itertools.product([0,1,5,255,256,0x12345678,0xffffffff],[0,1,0x100,0x80000000],[0,32,255]):add(p0=1,p1=5,p3=0,p21=saved,p22=media,p10=1,p11=bits)
for newmode in range(256):add(p0=1,p1=5,p3=0,p22=1,p10=1,p11=32,p18=0x12340000|newmode)
rng=random.Random(0x406090)
for i in range(400):
    add(p0=rng.randrange(2),p1=rng.choice([1,5,0,255]),p2=rng.choice([1,0x101]),p3=rng.choice([0,1,0x100]),p4=rng.choice([-1,0,255,256,511]),p5=rng.choice([-1,0,1,255,256,0x40000001]),p6=rng.randrange(256),p9=rng.randrange(256),p10=rng.randrange(32),p11=rng.randrange(256),p12=rng.getrandbits(32),p13=rng.getrandbits(32),p16=rng.getrandbits(32),p17=rng.getrandbits(32),p18=rng.getrandbits(32),p19=rng.getrandbits(32),p20=rng.getrandbits(32),p21=rng.getrandbits(32),p22=rng.getrandbits(32),p24=rng.getrandbits(32))
for p in cases[::17]:q=p.copy();q[29]=1;cases.append(q)
complete=len(cases)
if args.limit:cases=cases[:args.limit]
executables=[HERE/'build'/cfg/f'remaining_menu_{kind}.exe' for cfg in ['Debug','Release'] for kind in ['trace','plain']]
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
started=time.monotonic();counts={'00406090':0,'0040a0e0':0};endpoint_counts={f'{a:08x}':0 for a in boundaries}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':30}
    for i,p in enumerate(cases):
        expected=native(p);counts['00406090' if p[0]==0 else '0040a0e0']+=1
        for e in expected['events']:
            if e[0]==2:endpoint_counts[f'{e[1]:08x}']+=1
        line=' '.join(map(str,p))+'\n'
        for k,proc in enumerate(procs):
            proc.stdin.write(line);proc.stdin.flush();out=proc.stdout.readline();assert out,'C exited: '+proc.stderr.read();actual=json.loads(out)
            if k in [1,3]:
                actual['events']=[e for e in actual['events'] if e[0]==2]
                want={'memory':expected['memory'],'events':[e for e in expected['events'] if e[0]==2]}
            else:want=expected
            assert actual==want,json.dumps({'case':i,'p':p,'variant':str(executables[k]),'actual':actual,'expected':want})
        if (i+1)%3000==0:print(f'checked {i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(a in hits for a,f in core.items() if f==name),sum(f==name for f in core.values())] for name in facts['algorithm_scope']}
if not args.limit:assert all(a==b for a,b in coverage.values()) and sum(b for a,b in coverage.values())==70
files=[HERE/n for n in ['unit_00049.c','unit_00077.c','remaining_menu_route.c','remaining_menu_actions.h','remaining_menu_fixture.c','remaining_menu_native.json','CMakeLists.txt']]+[Path(__file__),ROOT/'src/unit_01628.c']
relative=lambda p:str(p.relative_to(ROOT)).replace('\\','/')
report={'format':1,'passed':True,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'product_applied':False,'method':'Complete49/77 controller, explicit489030 facade and real4C7F30 close in Unicorn versus separate staged C units, Debug/O2 trace/plain. Ordered callback reads/writes, semantic endpoint arguments/context and post-call mutations, full guards/state, cdecl/saved GPRs and real C close. Deep algorithms explicitly controlled in this local proof; production facade calls actual existing C functions.','per_controller':counts,'coverage':coverage,'native_algorithm_instructions':70,'abi_only_instruction_bytes_authenticated':abi_only,'controlled_boundaries':endpoint_counts,'compiled_C_sha256':sha(executables[0].read_bytes()),'variants':{label:{'path':relative(e),'sha256':sha(e.read_bytes()),'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':{relative(p):sha(p.read_bytes()) for p in files},'native_exports':{relative(HERE/'remaining_menu_native.json'):sha((HERE/'remaining_menu_native.json').read_bytes())},'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['This is a local controller/ABI composition proof, not completion of the eight explicitly controlled deep algorithms. The real existing C implementations remain the production callees.','488E90 remains mandatory unfinished map-refresh work: known generated scalar/index-width gaps and missing x87/FPU conversion semantics are not fixed or proved by this phase.','The incoming ECX context of489030 is explicit and compared at facade entry; all132 native488E90 instructions show no incoming receiver field use. Its actual C definition is five cdecl arguments.','4E2970 has one consumed incoming stack argument; complete85 native instructions show no use of arguments2/3. The facade preserves a three-argument source interface and forwards only the real callee argument.','Internal events in the unchanged close helper are compared through state snapshots/final guards and boundary539170; callback read/write traces exclude its internal stores.','Only direct and local staged-resolver paths are tested. Actual central/dropdown/click integration and the existing menu/map proofs must be replayed by root after application.','No UI, renderer, text, route-building or whole-game completion is established.']}
report['source_drift']=[relative(p) for p in files if sha(p.read_bytes())!=report['source_hashes'][relative(p)]]
out=HERE/('remaining-menu-actions-emulation-partial.json' if args.limit else 'remaining-menu-actions-emulation.json');out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','elapsed_seconds']}))
