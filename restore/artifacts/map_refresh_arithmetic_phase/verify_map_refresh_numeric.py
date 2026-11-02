"""Required finite raw80/binary64 arithmetic against exact original x87 instruction sites."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
def original(va,n):
    for k in range(u16(pe+6)):
        q=table+k*40;vs,rva,rawsz,raw=struct.unpack_from('<IIII',image,q+8)
        if base+rva<=va and va+n<=base+rva+rawsz:return image[raw+va-base-rva:raw+va-base-rva+n]
    raise AssertionError((hex(va),n))
facts_path=ROOT/'artifacts/map_refresh_phase/map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(0x481000,4096);vm.mem_map(0x56f000,4096);STACK=0x34000000;vm.mem_map(STACK,4096)
sites={int(i['address'],16):i for i in facts['functions']['00481540']['assembly']['items']}
operations=[0x481602,0x481628,0x4815c9,0x481674,0x48159f];store=0x4815cf
for address in operations+[store]:
    b=bytes.fromhex(sites[address]['bytes']);assert original(address,len(b))==b;vm.mem_write(address,b)
def native(case):
    op,sig,exponent,bits,cw=case;sp=STACK+0x800;vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_FPCW,cw);vm.reg_write(UC_X86_REG_FPSW,0);vm.reg_write(UC_X86_REG_FPTAG,0xfffc);vm.reg_write(UC_X86_REG_FP0,(sig,exponent))
    location=[sp+0x20,sp+0x28,0x56f470,sp+0x20,0x56f520][op];vm.mem_write(location,struct.pack('<Q',bits))
    address=operations[op];vm.emu_start(address,address+len(bytes.fromhex(sites[address]['bytes'])),count=1)
    sig_out,exp_out=vm.reg_read(UC_X86_REG_FP0);raw=struct.pack('<QH',sig_out,exp_out).hex()
    vm.emu_start(store,store+len(bytes.fromhex(sites[store]['bytes'])),count=1)
    assert vm.reg_read(UC_X86_REG_FPCW)==cw and vm.reg_read(UC_X86_REG_FPTAG)==0xffff and (vm.reg_read(UC_X86_REG_FPSW)&0x3800)==0x800
    return {'raw80':raw,'spill':list(struct.unpack('<II',vm.mem_read(sp+0x28,8))),'cw':cw}
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]
cases=[]
def raw80(sig,e,sign=0):return sig,((e+16383) if sig else 0)|(sign<<15)
def binary64(mantissa,e,sign=0):return (sign<<63)|(((e+1023)<<52)|mantissa if mantissa is not None else 0)
vectors=[]
for e,sign in itertools.product([-80,-47,-3,0,1,6,12,80],[0,1]):
    for sig,frac in [(0x8000000000000000,0),(0x8000000000000001,1),(0xbfffffffffffffff,0x7ffffffffffff),(0xc000000000000000,0x8000000000000),(0xffffffffffffffff,0xfffffffffffff)]:vectors.append((*raw80(sig,e,sign),binary64(frac,e,sign)))
for signa,signb,e in itertools.product([0,1],[0,1],[-80,-1,0,6,80]):
    vectors.append((*raw80(0x8000000000000001,e,signa),binary64(0,e,signb)))
    vectors.append((*raw80(0,e,signa),binary64(None,e,signb)))
    vectors.append((*raw80(0,e,signa),binary64(0,e,signb)))
    vectors.append((*raw80(0x8000000000000000,e,signa),binary64(None,e,signb)))
for cw,(sig,exp,bits),op in itertools.product(cws,vectors,range(5)):cases.append([op,sig,exp,bits,cw])
rng=random.Random(0x481540)
for i in range(1024):
    sig=rng.getrandbits(63)|0x8000000000000000;exp=rng.randrange(-80,81)+16383|(rng.randrange(2)<<15);bits=binary64(rng.getrandbits(52),rng.randrange(-80,81),rng.randrange(2));cases.append([rng.randrange(5),sig,exp,bits,rng.choice(cws)])
complete=len(cases)
if args.limit:cases=cases[:args.limit]
executables=[HERE/'build'/cfg/'map_refresh_numeric.exe' for cfg in ['Debug','Release']]
files=[Path(__file__),HERE/'map_refresh_fp.c',HERE/'map_refresh_arithmetic.h',HERE/'numeric_fixture.c',HERE/'CMakeLists.txt',facts_path]
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};binary_before={rel(p):sha(p) for p in executables}
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':7}
    for i,case in enumerate(cases):
        expected=native(case);op,sig,exponent,bits,cw=case;counts[op]=counts.get(op,0)+1;line=f'{op} {sig&0xffffffff} {sig>>32} {exponent} {bits&0xffffffff} {bits>>32} {cw}\n'
        for k,proc in enumerate(procs):
            proc.stdin.write(line);proc.stdin.flush();text=proc.stdout.readline();assert text,'C exit: '+proc.stderr.read();actual=json.loads(text)
            assert actual==expected,json.dumps({'case':i,'p':case,'variant':str(executables[k]),'actual':actual,'expected':expected})
        if (i+1)%10000==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
report={'format':1,'passed':True,'production':False,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*2,'native_sha256':PIN,'original_game_executed':False,'operation_sites':operations,'store_site':store,'operation_counts':counts,'control_words':cws,'compiled_C_sha256':binary_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':binary_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug','O2'],executables)},'source_hashes':before,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in executables if sha(p)!=binary_before[rel(p)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Exact numeric raw80 result and binary64 spill only for required finite operand domain; inputexponents[-80,80], addition exponent separation<=192, normal binary64 stores; signedzero and derived0/0 NaN included.','All x87 exception masks set. No precision/invalid/overflow status or trap equivalence claim. Native instruction operand memory is supplied by this isolated numeric oracle.','Complete481540/481690 composition must be proved separately, including actual481AA0 and memory/ABI boundaries.']}
assert not report['source_drift'] and not report['binary_drift']
(HERE/('map-refresh-numeric-partial.json' if args.limit else 'map-refresh-numeric-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','operation_counts','elapsed_seconds']}))
