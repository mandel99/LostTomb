"""Complete lookup-grid initialization/rebuild versus the pinned original instructions."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path);parser.add_argument('--production',action='store_true')
parser.add_argument('--limit',type=int);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,UC_PROT_READ,UC_PROT_EXEC
from unicorn.x86_const import *
MASK=0xffffffff;GRID=0x70e494;CELLS=231*457;IMAGES=0xb7482c;IMAGE_COUNT=228*228
STACK=0x34000000;STOP=0x33000000;sha=lambda b:hashlib.sha256(b).hexdigest()
regions=[(0x70e3fc,0x54),(GRID-16,CELLS*4+32),(IMAGES-16,IMAGE_COUNT*4+32),(0xf56b34,36)]
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
# Permit native stores only in the grid/setup pages and stack. Every byte in
# the image source and sentinel source pages remains read-only to CPU writes.
vm.mem_protect(base,(size+4095)&~4095,UC_PROT_READ|UC_PROT_EXEC)
vm.mem_protect(0x70e000,0x68000,7)
native=json.loads((ROOT/'metadata/lookup-sentinel-endpass-next-native.json').read_text())['functions']
instructions={}
for fn in ['00430480','00430530']:
    for i in native[fn]['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        instructions[a]=len(b)
hits=set();events=[];params=None;blocks=set()
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def write(a,v):vm.mem_write(a,(v&MASK).to_bytes(4,'little'))
def observe(phase):
    events.append([phase,read(0xf56b44)]+[read(0x70e40c+i*4) for i in range(9)]+[read(GRID),read(GRID+(CELLS-1)*4)])
    if phase==0 and params[5]&1:write(0xf56b44,params[6])
    if phase==1:
        if params[5]&2:write(0xf56b44,params[7])
        if params[5]&4:write(IMAGES+params[8]%IMAGE_COUNT*4,params[9])
        if params[5]&8:
            for i in range(9):write(0x70e40c+i*4,0x80010000+i)
def entry(cpu,a,n,user):observe(0)
def leave(cpu,a,n,user):observe(1)
def block(cpu,a,n,user):blocks.add((a,n))
vm.hook_add(UC_HOOK_CODE,entry,begin=0x430480,end=0x430480)
vm.hook_add(UC_HOOK_CODE,leave,begin=0x4304b8,end=0x4304b8)
cycle=[0,1,5,6,7,255,256,65535,0x80000000,0xfffffffe,0x12345678,0x80000001]
def source_values(p):
    result=[];random=p[4] or 1
    for index in range(IMAGE_COUNT):
        row,col=divmod(index,228);random^=(random<<13)&MASK;random^=random>>17;random^=(random<<5)&MASK;random&=MASK
        kind=p[3]
        if kind<6:v=[0,5,6,0x7fffffff,0x80000000,0xffffffff][kind]
        elif kind==6:v=cycle[index%12]
        elif kind==7:v=random
        elif kind==8:v=6 if index==p[4]%IMAGE_COUNT else 0
        elif kind==9:v=0 if index==p[4]%IMAGE_COUNT else 6
        elif kind==10:v=0x12345678 if row>p[4]%80 and row<148 and col>p[4]%60 and col<168 else 1
        elif kind==11:v=6 if row<col else 5
        elif kind==12:v=0x80000000 if row%3==0 else 5 if col%5==0 else 65536
        elif kind==13:v=index
        else:v=p[4]
        result.append(v)
    return struct.pack('<%dI'%IMAGE_COUNT,*result)
def run_native(p,cover):
    global params,events
    params=p;events=[]
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[4]+i*29)&255])*n)
    write(0xf56b44,p[2])
    for i in range(9):write(0x70e40c+i*4,p[4]*0x01010101+0x80000100+i)
    vm.mem_write(IMAGES,source_values(p))
    token=vm.hook_add(UC_HOOK_BLOCK,block) if cover else None
    for repeat in range(p[10]):
        sp=STACK+0xf0000
        write(sp,STOP);write(sp+4,p[1])
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ECX,0xbadf00d);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start(0x430480 if p[0]==0 else 0x430530,STOP,count=6000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
        assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
    if token:vm.hook_del(token)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest(),'events':events.copy()}
cases=[]
def case(op=1,mode=0,asset=0x1234,pattern=6,seed=1,mutation=0,before=0x123456,after=0xabcdef,index=0,value=6,repeats=1):
    cases.append(tuple(v&MASK for v in [op,mode,asset,pattern,seed,mutation,before,after,index,value,repeats]))
# Coverage fixtures come first, retaining original instruction addresses for
# both sides of eligibility and all four native orientation branches.
for mode,pattern in itertools.product([0,2,4,6],[0,2,6]):case(mode=mode,pattern=pattern)
for asset in [0,1,255,256,65535,65536,0x12345678,0x7fffffff,0x80000000,0xffffffff,0xf0010000]:case(op=0,asset=asset,seed=asset)
for mode,pattern,asset in itertools.product([0,2,4,6,1,0x80000000,0xffffffff],range(15),[0x1234,0xffffffff]):case(mode=mode,pattern=pattern,asset=asset,seed=0x51983)
for mode,selected,pattern in itertools.product([0,2,4,6],[0,1,227,228,229,25991,51755,51982,51983],[8,9]):case(mode=mode,pattern=pattern,seed=selected)
for mode,seed in itertools.product([0,2,4,6],[0,1,0x12345678,0x80000000,0xffffffff]):case(mode=mode,pattern=7,seed=seed)
for op,mode,bits,value in itertools.product([0,1],[0,2,4,6],[1,2,3,4,8,15],[5,6,0x80000000]):case(op=op,mode=mode,mutation=bits,pattern=6,index=51983,value=value)
for mode in [0,2,4,6]:case(mode=mode,pattern=6,mutation=15,repeats=2)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-lookup-sentinel/Release/lookup_sentinel_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,
 'case_selection':{'limit':args.limit,'single_case':bool(args.case)},
 'method':'Complete178 original instructions of430480/430530 versus actual compiled C. Real clear helper composition; every105567 grid DWORD, all51984 image-source DWORDs, full setup/bounds/sentinel and canaries compared. Real clear entry/exit observations and mutations, cdecl stack and all saved registers checked.',
 'limitations':['No image descriptors, rasterization, camera clamps or later viewport passes are executed by this pair. Their correctness is not claimed.','Functions have no consumed semantic return; incidental EAX and caller-saved scratch registers are not asserted. Native caller ABI requires a clear direction flag.']}
checked=0;p=None;started=time.monotonic()
try:
    assert json.loads(process.stdout.readline())=={'fields':11}
    for p in cases:
        expected=run_native(p,checked<12);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush()
        line=process.stdout.readline();assert line,'C exited: '+process.stderr.read();actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/lookup_sentinel_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected},indent=2)+'\n')
        assert actual==expected,repr({'C':actual,'native':expected})
        checked+=1
        if checked%50==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    for a,n in blocks:hits.update(i for i in instructions if a<=i<a+n)
    if not(args.limit or args.case):assert hits==set(instructions),'Uncovered original instructions '+repr([hex(i) for i in sorted(set(instructions)-hits)])
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
for a,n in blocks:hits.update(i for i in instructions if a<=i<a+n)
prefix='src' if args.production else 'artifacts/lookup_sentinel_phase'
sources=[prefix+'/unit_00457.c',prefix+'/unit_00459.c','include/lookup_sentinel.h','tests/lookup_sentinel.c','tests/lookup_sentinel_real_clear.c','tools/lookup_sentinel_project/CMakeLists.txt','tools/verify_lookup_sentinel_emulation.py','metadata/lookup-sentinel-endpass-next-native.json']
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],native_uncovered_instructions=[hex(i) for i in sorted(set(instructions)-hits)],full_grid_cells=CELLS,source_image_cells=IMAGE_COUNT,controlled_algorithm_endpoints=[],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/lookup-sentinel-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes']},indent=2));sys.exit(0 if report['passed'] else 1)
