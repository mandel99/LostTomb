"""Raw80 numeric/CW component compared with complete original56133C, not FPSW equivalence."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
def image_read(va,n):
    for k in range(u16(pe+6)):
        q=table+k*40;vs,rva,rawsz,raw=struct.unpack_from('<IIII',image,q+8)
        if base+rva<=va and va+n<=base+rva+rawsz:return image[raw+va-base-rva:raw+va-base-rva+n]
    raise AssertionError((hex(va),n))
facts_path=HERE.parent/'map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
items=facts['functions']['0056133c']['assembly']['items'];assert len(items)==15
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(0x561000,4096);STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x10000);vm.mem_map(STOP,4096)
instructions={int(i['address'],16) for i in items};decoded={};decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for item in items:
    a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert image_read(a,len(b))==b;vm.mem_write(a,b)
    sub=list(decoder.disasm(b,a));assert b''.join(bytes(i.bytes) for i in sub)==b
    for i in sub:decoded[i.address]=a
hits=set();decoded_hits=set()
def code(cpu,a,n,user):
    if a!=STOP:assert a in decoded,hex(a);hits.add(decoded[a]);decoded_hits.add(a)
def write(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x10000,('outside native stack',hex(a),n)
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def native(sig,exponent,cw):
    sp=STACK+0x8000;vm.mem_write(sp,struct.pack('<III',STOP,0xbaadf00d,0x55aa55aa))
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_FPCW,cw);vm.reg_write(UC_X86_REG_FPSW,0);vm.reg_write(UC_X86_REG_FPTAG,0xfffc);vm.reg_write(UC_X86_REG_FP0,(sig,exponent))
    vm.emu_start(0x56133c,STOP,count=20)
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert bytes(vm.mem_read(sp+4,8))==struct.pack('<II',0xbaadf00d,0x55aa55aa)
    assert vm.reg_read(UC_X86_REG_FPCW)==cw and vm.reg_read(UC_X86_REG_FPTAG)==0xffff and (vm.reg_read(UC_X86_REG_FPSW)&0x3800)==0x800
    return {'low':vm.reg_read(UC_X86_REG_EAX),'high':vm.reg_read(UC_X86_REG_EDX),'cw':vm.reg_read(UC_X86_REG_FPCW)}
cases=[]
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]
exponents=[0,1,2,0x3fc0,0x3ffd,0x3ffe,0x3fff,0x4000,0x401e,0x401f,0x4020,0x403c,0x403d,0x403e,0x403f,0x7ffe,0x7fff]
significands=[0,1,0x7fffffffffffffff,0x8000000000000000,0x8000000000000001,0x8000000000000002,0xbfffffffffffffff,0xc000000000000000,0xc000000000000001,0xfffffffffffffffe,0xffffffffffffffff]
for exponent,sig,cw,sign in itertools.product(exponents,significands,cws,[0,0x8000]):cases.append([sig,exponent|sign,cw])
# Every15-bit exponent, both signs over adjacent exponents, exact raw significand.
for exponent in range(0x8000):cases.append([0xc000000000000001,exponent|((exponent&1)<<15),0x37f])
rng=random.Random(0x56133c)
for i in range(1024):cases.append([rng.getrandbits(64),rng.randrange(0x10000),rng.choice(cws)])
complete=len(cases)
if args.limit:cases=cases[:args.limit]
executables=[HERE/'build'/cfg/'map_refresh_x87_conversion.exe' for cfg in ['Debug','Release']]
files=[Path(__file__),HERE/'map_x87_conversion.c',HERE/'map_x87_conversion.h',HERE/'x87_conversion_fixture.c',HERE/'CMakeLists.txt',HERE/'CONTRACT.md',facts_path]
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables}
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();kinds={}
def classify(sig,exp):
    e=exp&0x7fff
    if e==0:return 'zero' if sig==0 else ('pseudo_denormal' if sig>>63 else 'subnormal')
    if e==0x7fff:return 'infinity_or_nan'
    if sig>>63==0:return 'unsupported_unnormal'
    return 'normal'
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':4}
    for i,(sig,exponent,cw) in enumerate(cases):
        expected=native(sig,exponent,cw);kind=classify(sig,exponent);kinds[kind]=kinds.get(kind,0)+1
        line=f'{sig&0xffffffff} {sig>>32} {exponent} {cw}\n'
        for k,proc in enumerate(procs):
            proc.stdin.write(line);proc.stdin.flush();out=proc.stdout.readline();assert out,'C exited: '+proc.stderr.read();actual=json.loads(out)
            assert actual==expected,json.dumps({'case':i,'raw':f'{sig:016x}/{exponent:04x}','cw':f'{cw:04x}','variant':str(executables[k]),'actual':actual,'expected':expected})
        if (i+1)%10000==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
assert len(hits)==15 and len(decoded_hits)==len(decoded)
report={'format':1,'passed':True,'production':False,'canonical_56133c_replaced':False,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*2,'native_sha256':PIN,'original_game_executed':False,'coverage':[len(hits),15],'decoded_coverage':[len(decoded_hits),len(decoded)],'encoding_classes':kinds,'control_words':cws,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug','O2'],executables)},'source_hashes':before,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Numeric result and unchangedCW only under all exceptions masked. Unicorn omits native FISTP precision/invalid FPSW flag updates; no status/trap equivalence claim.','NativeST0 register input/pop is normalized to explicit10-byte C input; nativeTOP/tag pop checked independently, C input/guards and EDX:EAX ABI checked. No canonical legacy ST0 callers are redirected.','PC/RC arithmetic, binary64 spills,481540/481690,488E90/489030 and postpasses remain unfinished.']}
assert not report['source_drift'] and not report['binary_drift']
(HERE/('x87-conversion-partial.json' if args.limit else 'x87-conversion-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','encoding_classes','elapsed_seconds']}))
