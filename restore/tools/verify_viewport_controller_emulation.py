"""Actual viewport controller+facade versus original x86 executed only in Unicorn."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,help='Traced C verifier')
parser.add_argument('--plain-exe',type=Path,help='Optional plain C verifier')
args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_PROT_ALL,UC_PROT_NONE
from unicorn.x86_const import *
MASK=0xffffffff
sha=lambda b:hashlib.sha256(b).hexdigest()
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);image_size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(image_size+4095)&~4095)
for k in range(u16(image,pe+6)):
    s=table+k*40;vm.mem_write(base+u32(image,s+12),image[u32(image,s+20):u32(image,s+20)+u32(image,s+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
snapshot=json.loads((ROOT/'metadata/city-ui-next-native.json').read_text())
instructions={int(i['address'],16) for a in ['00430d30','004dddc0'] for i in snapshot['functions'][a]['items']}
for a in ['00430d30','004dddc0']:
    for i in snapshot['functions'][a]['items']:
        b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b,i
assert bytes(vm.mem_read(0x51f780,1))==b'\xc3'
regions=[(0x6de510,12),(0x70e3b8,24),(0x70e428,0x68),(0x775608,16),(0xbca2b4,0x2c),(0xd3b548,0x20),(0xe38da4,0x18),(0xe92e04,16),(0xe94e04,16)]
watched=[0x70e478,0x70e474,0x70e484,0xd3b55c,0xd3b558,0xd3b554,0xd3b550,0x70e3c0,0x70e3c4,0x775610,0xe92e0c,0xe94e0c,0x6de515,0xe38db0,0xe38dac,0x70e43c,0x70e438,0x70e434,0x70e430,0xbca2bc,0xbca2c0,0xbca2d8]
endpoints={0x430ef0,0x4316f0,0x436b60,0x439f10,0x43b630,0x439740,0x43e300,0x51f780}
params=None;events=[];ops=[];hits=set();empty_hit=False;native_entries=[]
def endpoint(cpu,a,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP);count=int(a in [0x436b60,0x439740]);arg=read(sp+4) if count else 0
    state=[MASK if params[36] else read(v,1) if i==12 else read(v) for i,v in enumerate(watched)]
    # Guard affects only the special byte, not the other state fields.
    if params[36]:state=[MASK if i==12 else read(v) for i,v in enumerate(watched)]
    events.append([a,count,arg]+state)
    if params[19]&(1<<(len(events)-1)):
        mutations=[]
        if params[20]&1:mutations.extend([(0xe94e0c,params[21],4),(0x6de515,params[22],1)])
        if params[20]&2:mutations.append((0x775610,params[23],4))
        if params[20]&4:mutations.extend([(0xe38db0,params[24],4),(0xe38dac,params[25],4)])
        if params[20]&8:mutations.extend((v,params[26+i],4) for i,v in enumerate([0xd3b55c,0xd3b558,0xd3b554,0xd3b550]))
        if params[20]&16:mutations.extend((v,params[30+i],4) for i,v in enumerate([0x70e478,0x70e474,0x70e484]))
        if params[20]&32:mutations.extend((v,params[33+i],4) for i,v in enumerate([0xe92e0c,0x70e3c0,0x70e3c4]))
        if params[20]&64:mutations.extend((v,params[26+i],4) for i,v in enumerate([0x70e43c,0x70e438,0x70e434,0x70e430]))
        if params[20]&128:mutations.extend((v,params[30+i],4) for i,v in enumerate([0xbca2bc,0xbca2c0,0xbca2d8]))
        for v,value,width in mutations:write(v,value,width)
    if a!=0x51f780:
        # Seven controlled cdecl void draws; caller owns all stack arguments.
        cpu.reg_write(UC_X86_REG_EAX,0x12345678);cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
        cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def code(cpu,a,n,user):
    global empty_hit
    if a in instructions:hits.add(a)
    if a==0x51f780:empty_hit=True
    if a==0x430d30:
        sp=cpu.reg_read(UC_X86_REG_ESP);native_entries.append([read(sp+4),read(sp+8),read(sp+12)])
def in_body(a):return 0x430d30<=a<=0x430e56 or 0x4dddc0<=a<=0x4ddddc
def memory_read(cpu,access,a,n,value,user):
    if in_body(cpu.reg_read(UC_X86_REG_EIP)) and 0x600000<=a<0x30000000:ops.append([0,a,n,read(a,n)])
def memory_write(cpu,access,a,n,value,user):
    if in_body(cpu.reg_read(UC_X86_REG_EIP)) and a<STACK:ops.append([1,a,n,value&((1<<(8*n))-1)])
vm.hook_add(UC_HOOK_CODE,code)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
vm.hook_add(UC_HOOK_MEM_READ,memory_read);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)

def native(p):
    global params
    params=p;events.clear();ops.clear();native_entries.clear()
    for k,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[1]+k)&255])*n)
    for a,value,n in [(0x70e43c,p[5],4),(0x70e438,p[6],4),(0x70e434,p[7],4),(0x70e430,p[8],4),(0xe92e0c,p[9],4),(0x70e3c0,p[10],4),(0xe94e0c,p[11],4),(0x6de515,p[12],1),(0x775610,p[13],4),(0xe38db0,p[14],4),(0xe38dac,p[15],4),(0xbca2bc,p[16],4),(0xbca2c0,p[17],4),(0xbca2d8,p[18],4)]:write(a,value,n)
    if p[36]:vm.mem_protect(0x6de000,4096,UC_PROT_NONE)
    try:
        for repeat in range(p[37]):
            sp=STACK+0xf0000
            for k,v in enumerate([STOP,p[2],p[3],p[4]]):write(sp+k*4,v)
            saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
            for r,v in saved:vm.reg_write(r,v)
            vm.reg_write(UC_X86_REG_ECX,0x0badc0de);vm.reg_write(UC_X86_REG_EDX,0x9abcdef0);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
            vm.emu_start(0x4dddc0 if p[0] else 0x430d30,STOP,count=10000)
            assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
            assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
            assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
    finally:
        if p[36]:vm.mem_protect(0x6de000,4096,UC_PROT_ALL)
    # Actual facade must pass all three dwords; subsequent fixture mutations
    # can change its inputs for a second invocation.
    expected_first=[p[16],p[17],p[18]] if p[0] else [p[2],p[3],p[4]]
    assert native_entries[0]==expected_first,'Native facade dword arguments'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'ops':ops.copy(),'events':events.copy()}

default=[0,1,0x123,0x234,0x345,1,1023,2,767,61,0,0,0,1,1024,768,0x12345,0x23456,0x34567,0,0,51,1,0x80000000,1920,1080,10,1100,20,900,0x87654321,0x76543210,0x65432109,MASK,0x80000000,0x12345678,0,1]
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
for op,mode,special in itertools.product([0,1],[0,1,50,51,52,0x100,0x1000000,0x80000000,MASK],range(256)):case(p0=op,p11=mode,p12=special,p1=mode^special)
edges=[0,1,59,60,61,127,255,256,65535,65536,0x7fffffff,0x80000000,0xfffffffe,MASK]
for op,mode,special,now,last in itertools.product([0,1],[0,51,1],[0,1],edges,edges):case(p0=op,p11=mode,p12=special,p9=now,p10=last)
for op,mode,last,delta in itertools.product([0,1],[0,51,1],[0,1,0x7fffffff,0x80000000,MASK],[0,1,59,60,61,62,127,255,256,0x7fffffff,0x80000000,0xfffffffe,MASK]):case(p0=op,p11=mode,p9=(last+delta)&MASK,p10=last)
for field,value,op in itertools.product(range(2,19),edges,[0,1]):case(**{'p'+str(field):value,'p0':op})
for op,mode,special,when,bits,counter,repeat in itertools.product([0,1],[0,51,1],[0,1],[1,2,4,8,16,32,63],[1,2,4,8,16,32,64,128,255],[0,1,255,256,0x7fffffff,0x80000000,MASK],[1,2]):case(p0=op,p11=mode,p12=special,p19=when,p20=bits,p23=counter,p37=repeat)
for op,when,bits in itertools.product([0,1],[0,1,2,4,8,16,32,63],[0,2,4,8,16,32,64,128,254]):case(p0=op,p11=51,p19=when,p20=bits,p36=1)
cases=list(dict.fromkeys(cases))
executables=[args.exe or ROOT/'build/agent-viewport-controller/Release/viewport_controller_trace.exe']
if args.plain_exe:executables.append(args.plain_exe)
elif not args.exe:executables.append(ROOT/'build/agent-viewport-controller/Release/viewport_controller_plain.exe')
processes=[subprocess.Popen([str(e),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(executables[0].read_bytes()),'compiled_C_variants_sha256':{e.name:sha(e.read_bytes()) for e in executables},'checked':0,
 'method':'Actual67-instruction430d30 controller and9-instruction4dddc0 facade plus real51f780 RET in Unicorn versus traced/plain handwritten C. Ordered fixed-memory reads/writes, endpoint arities/arguments/intermediate state, full guarded fixture bytes, DWORD wrap, callback mutations, repeated invocation and native/C cdecl stack+saved registers.',
 'limitations':['The seven deeper drawing algorithms430ef0,4316f0,436b60,439f10,43b630,439740,43e300 are controlled cdecl void endpoints. This comparison does not establish city pixel fidelity or completed gameplay.',
 '51f780 executes its authenticated native RET and unchanged C empty body; endpoint instrumentation observes its order and can inject declared synthetic test mutations.',
 'Controller and facade are used for side effects. Incidental native EAX forwarding is outside their C void contract; memory, calls, stack and saved registers are compared.',
 'Finite fixed-address fixtures, arbitrary32-bit scalar field values and protected special-byte page in mode51; no original executable process, original .text fallback or UI used.']}
checked=0;started=time.monotonic();p=None
try:
    for process in processes:assert json.loads(process.stdout.readline())=={'fields':38}
    for p in cases:
        expected=native(p)
        for k,process in enumerate(processes):
            process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
            actual=json.loads(line)
            if k==1:expected['ops']=[]
            assert actual==expected,repr({'build':['trace','plain'][k],'actual':actual,'expected':expected})
        checked+=1
        if checked%2000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    assert hits==instructions,'Uncovered native instructions '+repr([hex(a) for a in instructions-hits])
    assert empty_hit,'Real native RET not executed'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for process in processes:process.stdin.close();process.wait(timeout=10)
sources=['src/unit_00468.c','src/unit_01878.c','src/unit_02351.c','include/viewport_controller.h','tests/viewport_controller.c','tests/viewport_controller_empty_callback.c','tools/viewport_controller_project/CMakeLists.txt','tools/verify_viewport_controller_emulation.py','metadata/city-ui-next-native.json','metadata/actor-empty-callback-native.json']
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],real_empty_callback_covered=empty_hit,source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/viewport-controller-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
