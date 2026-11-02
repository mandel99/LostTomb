"""Complete 004387d0 versus handwritten C; original instructions run only in Unicorn."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,help='Traced C verifier, e.g. build/c-only-debug/Debug/restore_actor_predicate_verify.exe')
parser.add_argument('--plain-exe',type=Path,help='Optional uninstrumented C verifier; private default is used only when --exe is absent')
args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_PROT_ALL,UC_PROT_NONE
from unicorn.x86_const import *
MASK=0xffffffff
sha=lambda b:hashlib.sha256(b).hexdigest()
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
signed16=lambda v:((v&65535)^32768)-32768
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);image_size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,max(0x1400000-base,(image_size+4095)&~4095))
for k in range(u16(image,pe+6)):
    s=table+k*40;vm.mem_write(base+u32(image,s+12),image[u32(image,s+20):u32(image,s+20)+u32(image,s+16)])
STACK=0x34000000;STOP=0x33000000
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')

# Authenticate every retained native instruction and both dispatch tables
# directly against the pinned PE. No .text is copied into a runnable C process.
snapshot=json.loads((ROOT/'metadata/minimap-helpers-native.json').read_text())['functions']['004387d0']['items']
helpers=json.loads((ROOT/'metadata/actor_predicate_helpers_native.json').read_text())['functions']
expected_instructions={int(i['address'],16) for i in snapshot}
helper_instructions={int(i['address'],16) for f in helpers.values() for i in f['assembly']['items']}
for i in snapshot+[i for f in helpers.values() for i in f['assembly']['items']]:
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b,i
overlay_table=[read(0x438d20+k*4) for k in range(48)]
actor_table=list(vm.mem_read(0x438dec,91));actor_targets=[read(0x438de0+k*4) for k in range(3)]
category_table=[read(0x4c5164+k*4) for k in range(10)]
contract={'native_sha256':digest,'original_game_executed':False,'instruction_counts':{'004387d0':len(snapshot),'004c5120':21,'004c52d0':14},
 'overlay_table':{str(k+4):hex(v) for k,v in enumerate(overlay_table)},
 'actor_table':{str(k+11):hex(actor_targets[v]) for k,v in enumerate(actor_table)},
 'category_table':{str(k+60):hex(v) for k,v in enumerate(category_table)},
 'contract':{'004387d0':'cdecl(int32 actor_id), full native EAX is exactly 0 or 1; compatible existing uint8 return interface',
 '004c5120':'cdecl(int32 building_id), full EAX category 0..4 or UINT32_MAX; no receiver',
 '004c52d0':'cdecl(int32 building_id,uint32 desired), Boolean AL only; high EAX can retain building ID when status==3; no receiver'},
 'fixed_accesses':{'overlay':'dword e94e0c','actor':'bcd600 + actor_id*0x184: type byte+2,subtype byte+3,action byte+0x30,building signed word+0x52,marker byte+0x5e',
 'building':'7999d8 + signed actor-building ID*0x108: type signed word+0x10,flags dword+0x100; helper52d0 status byte+0xa9',
 'profile':'eaac28 dword; selected profile stride0x49d8; ea5ce8 active word and ea5f9c selected building dword'},
 'reloads':['Mode10 captures signed actor-building ID and initial selected ID before calling helpers.',
 'Mode10 reads selected ID even when active word is zero.',
 'Mode10 calls5120(selected),5120(actor), then5120(initial selected) only when first categories match.',
 'Only third category1/3 continues; current profile index and selected ID reload after helpers; active is not rechecked.',
 'Mode49 type27 checks building signed type63/68, current active word, then calls52d0(current selected,2).'],
 'production_mutations':'No direct data writes in predicate or either real helper; controlled endpoint writes model only declared test callbacks.'}
(ROOT/'metadata/actor_predicate_native_contract.json').write_text(json.dumps(contract,indent=2)+'\n')

params=None;events=[];reads=[];writes=[];hits=set();helper_hits=set();category_calls=0
def mutate():
    if not(params[25]&(1<<(len(events)-1))):return
    profile=params[11]*0x49d8&MASK
    if params[26]&1:write(0xeaac28,params[16])
    if params[26]&2:write(0xea5f9c+profile,params[17])
    if params[26]&4:write(0xea5ce8+profile,0,2)
    if params[26]&8:write(0xbcd652+params[3]*0x184,params[17],2)
    if params[26]&16:write(0xe94e0c,0xdeadbeef)
    if params[26]&32:write(0x7999e8+params[13]*0x108,68,2)
    if params[26]&64:write(0x799a81+params[17]*0x108,3,1)
def endpoint(cpu,a,n,user):
    global category_calls
    sp=cpu.reg_read(UC_X86_REG_ESP);arg0=read(sp+4);arg1=read(sp+8) if a==0x4c52d0 else 0;profile=params[11]*0x49d8&MASK
    events.append([a,arg0,arg1,read(0xe94e0c),read(0xeaac28),read(0xea5ce8+profile,2),read(0xea5f9c+profile)])
    mutate()
    if params[20]:
        result=params[21+category_calls] if a==0x4c5120 else params[24]
        # cdecl: RET consumes only the return address. ECX/EDX are caller saved.
        cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
        cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
    if a==0x4c5120:category_calls+=1
def code(cpu,a,n,user):
    if a in expected_instructions:hits.add(a)
    if a in helper_instructions:helper_hits.add(a)
def memory_read(cpu,access,a,n,value,user):
    if 0x4387d0<=cpu.reg_read(UC_X86_REG_EIP)<=0x438d1c and 0x700000<=a<0x30000000:
        reads.append([a,n,read(a,n)])
def memory_write(cpu,access,a,n,value,user):
    if a<STACK or a>=STACK+0x100000:writes.append([cpu.reg_read(UC_X86_REG_EIP),a,n,value])
vm.hook_add(UC_HOOK_CODE,code)
vm.hook_add(UC_HOOK_CODE,endpoint,begin=0x4c5120,end=0x4c5120)
vm.hook_add(UC_HOOK_CODE,endpoint,begin=0x4c52d0,end=0x4c52d0)
vm.hook_add(UC_HOOK_MEM_READ,memory_read)
vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)

def native(p):
    global params,category_calls
    params=p;events.clear();reads.clear();writes.clear();category_calls=0;regions=[]
    def region(address,size):
        address&=MASK
        if any(a==address for a,n in regions):return
        regions.append((address,size));vm.mem_write(address,bytes([(p[1]+len(regions)-1)&255])*size)
    actor=p[3]*0x184&MASK;building=signed16(p[8])&MASK;profile=p[11]*0x49d8&MASK;next_profile=p[16]*0x49d8&MASK
    region(0xe94e08,12);region(0xeaac20,20);region(0xbcd600+actor-8,0x184+16)
    for b in [building,p[13],p[17]]:region(0x7999d8+b*0x108-8,0x108+16)
    region(0xea5ce0+profile,0x2c8);region(0xea5ce0+next_profile,0x2c8)
    for a,v,n in [(0xe94e0c,p[2],4),(0xeaac28,p[11],4),(0xbcd602+actor,p[4],1),(0xbcd603+actor,p[5],1),(0xbcd630+actor,p[6],1),(0xbcd65e+actor,p[7],1),(0xbcd652+actor,p[8],2),
                  (0x7999e8+building*0x108,p[9],2),(0x799ad8+building*0x108,p[10],4),(0xea5ce8+profile,p[12],2),(0xea5f9c+profile,p[13],4),(0xea5ce8+next_profile,1,2),(0xea5f9c+next_profile,p[17],4),
                  (0x7999e8+p[13]*0x108,p[14],2),(0x799a81+p[13]*0x108,p[15],1),(0x7999e8+p[17]*0x108,p[18],2),(0x799a81+p[17]*0x108,p[19],1)]:write(a,v,n)
    sp=STACK+0xf0000;arg=p[13] if p[0] else p[3]
    for k,v in enumerate([STOP,arg,p[27]]):write(sp+k*4,v)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ECX,0x0badc0de);vm.reg_write(UC_X86_REG_EDX,0x9abcdef0);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
    guard=((0xbcd600+actor)&MASK)&~4095
    if p[28]:vm.mem_protect(guard,4096,UC_PROT_NONE)
    try:vm.emu_start([0x4387d0,0x4c5120,0x4c52d0][p[0]],STOP,count=10000)
    finally:
        if p[28]:vm.mem_protect(guard,4096,UC_PROT_ALL)
    assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
    assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
    assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
    assert not writes,repr(writes)
    raw=vm.reg_read(UC_X86_REG_EAX)
    if p[0]==0:assert raw in [0,1],'Native full EAX Boolean domain'
    result=raw if p[0]!=2 else raw&255
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'result':result,'memory':h.hexdigest(),'reads':reads.copy(),'events':events.copy()},raw

default=[0,1,10,257,27,0,0,0,7,61,0,1,1,9,61,1,2,11,63,2,0,1,1,1,1,0,0,2,0]
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
# Every outer jump-table slot/default and every unsigned actor-type byte.
for mode,kind in itertools.product(list(range(56))+[-1,0x100,0x100000a,0x80000000,0xffffffff],range(256)):case(p2=mode,p4=kind,p1=(mode^kind)&MASK)
# Byte/word widths, signed IDs, every inner table dispatch class and its edges.
for subtype in range(256):case(p2=26,p4=4,p5=subtype)
for action,marker in itertools.product(range(256),[0,1,128,255]):case(p2=31,p4=4,p6=action,p7=marker)
for actor,building,flag in itertools.product([-1,0,1,257,1999,0x40000000,0x80000000],[-1,0,7,257,3999],[0]+[1<<k for k in range(32)]+[0xffffffff]):case(p2=31,p3=actor,p4=5,p8=building,p10=flag)
for actor,mode in itertools.product([-1,0,1,1999,0x40000000,0x80000000],[0,5,27,32,44,47,51,52,0x1000000a]):case(p2=mode,p3=actor,p28=1)
for mode,profile,active,building,kind in itertools.product([10,49],[-1,0,1,256,257],[0,1,255,256,32768,65535],[-1,7,257,3999],[0,61,63,68,0x803f,0xffff]):case(p2=mode,p11=profile,p12=active,p8=building,p9=kind,p14=kind)
# Actual native helpers run as instructions and actual existing C bodies.
for mode,actor_type,selected_type,status in itertools.product([10,49],range(59,71),range(59,71),[0,1,2,3,128,255]):case(p2=mode,p9=actor_type,p14=selected_type,p15=status)
for kind in list(range(71))+[127,128,255,256,32767,32768,65535]:case(p0=1,p14=kind)
for status,desired in itertools.product(range(256),[0,1,2,3,4,255,256,0xffffffff]):case(p0=2,p15=status,p27=desired)
for selected,mode in itertools.product([-1,0,1,257,3999,0x40000009,0x80000009],[10,49]):case(p13=selected,p2=mode,p9=63,p14=63,p15=3)
# Controlled helpers expose capture/reload behavior and AL-only final results.
categories=[0,1,2,3,4,0xffffffff,0x1000001]
for a,b,c,result in itertools.product(categories,categories,categories,[0,1,2,0x100,0xffffff00,0xdeadbeef]):case(p20=1,p21=a,p22=b,p23=c,p24=result)
for when,bits,category,result,profile in itertools.product([1,2,4,8,15],[1,2,4,8,16,32,64,127],[1,3],[0,1,0x100,0x800000ff],[0,1,256]):case(p20=1,p21=category,p22=category,p23=category,p24=result,p25=when,p26=bits,p16=profile)
for when,bits in itertools.product([1,2,4,8,15],[1,2,4,8,16,32,64,127]):
    case(p25=when,p26=bits,p19=1)
    case(p2=49,p9=63,p14=63,p25=when,p26=bits)
cases=list(dict.fromkeys(cases))
executables=[args.exe or ROOT/'build/agent-actors/Release/actor_predicate_trace.exe']
if args.plain_exe:executables.append(args.plain_exe)
elif not args.exe:executables.append(ROOT/'build/agent-actors/Release/actor_predicate_plain.exe')
processes=[subprocess.Popen([str(e),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(executables[0].read_bytes()),'compiled_C_variants_sha256':{e.name:sha(e.read_bytes()) for e in executables},'checked':0,
 'method':'Authenticated complete 436-instruction native predicate and real 21/14-instruction helpers in Unicorn against traced and uninstrumented handwritten C; all table cases, unsigned type/subtype/status bytes, ordered predicate reads and endpoint arguments/state, complete fixture bytes+guards, controlled helper mutations, native full EAX domain, C/native cdecl stack and all callee-saved registers.',
 'limitations':['Finite fixtures use mapped actor/building/profile addresses and guarded records; arbitrary invalid addresses retain native fault behavior rather than being treated as valid input.',
 'Both real helper algorithms execute in C and Unicorn. Additional controlled helpers deliberately return adversarial categories/AL and mutate declared fixture fields to expose predicate capture/reload semantics; those callback behaviors are tests rather than claims about real pure helpers.',
 'Existing C byte return interface is preserved. Native predicate full EAX is independently checked as0/1; C Boolean callers consume AL. Raw C EAX is recorded and its domain reported, because C does not guarantee upper return bits for uint8.',
 'No original executable process, original .text fallback, UI or original installation access. This bounded proof establishes this predicate and its two helpers, not a completed city frame or the100 other actor callbacks.']}
checked=0;raw_c=set();raw_native=set();started=time.monotonic();p=None
try:
    for process in processes:assert json.loads(process.stdout.readline())=={'fields':29}
    for p in cases:
        expected,native_raw=native(p)
        if p[0]==0:raw_native.add(native_raw)
        for k,process in enumerate(processes):
            process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
            actual=json.loads(line);raw=actual.pop('raw')
            if p[0]==0:raw_c.add(raw)
            if k==1:expected['reads']=[]
            assert actual==expected,repr({'build':['trace','plain'][k],'actual':actual,'expected':expected,'native_raw':native_raw,'c_raw':raw})
        checked+=1
        if checked%2000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    assert hits==expected_instructions,'Uncovered predicate instructions '+repr([hex(v) for v in expected_instructions-hits])
    assert helper_hits==helper_instructions,'Uncovered real helper instructions '+repr([hex(v) for v in helper_instructions-helper_hits])
    report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for process in processes:
        process.stdin.close();process.wait(timeout=10)
sources=['src/unit_00560.c','src/unit_01602.c','src/unit_01606.c','include/actor_predicate.h','tests/actor_predicate.c','tests/actor_predicate_helpers.c','tools/verify_actor_predicate_emulation.py','tools/actor_predicate_project/CMakeLists.txt','metadata/minimap-helpers-native.json','metadata/actor_predicate_helpers_native.json']
report.update(elapsed_seconds=round(time.monotonic()-started,2),predicate_instruction_coverage=[len(hits),len(expected_instructions)],helper_instruction_coverage=[len(helper_hits),len(helper_instructions)],native_predicate_full_EAX_domain=sorted(raw_native),compiled_C_raw_EAX_domain=sorted(raw_c),source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/actor-predicate-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2))
sys.exit(0 if report['passed'] else 1)
