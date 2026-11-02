"""Camera lookup bounds and signed clamps versus the original x86, without launching it."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path)
parser.add_argument('--production',action='store_true')
parser.add_argument('--limit',type=int)
args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK
from unicorn.x86_const import *
MASK=0xffffffff;STACK=0x34000000;STOP=0x33000000;CELLS=231*457
regions=[(0x70e450,64),(0x70e484,CELLS*4+32),(0xbca2b0,96),(0xe94db0,48),(0x6fdc40,32),(0x605a30,32),(0xe38dc0,32)]
sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert sha(image)=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
instructions={}
for name,functions in [('camera-sidebar-coordinate-native.json',['004ddf90','004de080','0047e830']),('camera-bounds-helpers-native.json',['004ddea0','004ddeb0','00430ca0','00430cb0','0046f930'])]:
    dossier=json.loads((ROOT/'metadata'/name).read_text())['functions']
    for fn in functions:
        asm=dossier[fn]['assembly'];assert not asm['has_more']
        for i in asm['items']:
            a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
            instructions[a]=len(b)
blocks=set()
def block(cpu,a,n,user):blocks.add((a,n))
vm.hook_add(UC_HOOK_BLOCK,block)
def write(a,v):vm.mem_write(a,(v&MASK).to_bytes(4,'little'))
grid_cache={}
def grid(p):
    key=p[1],p[2],p[20]
    if key in grid_cache:return grid_cache[key]
    r=p[1] or 1;values=[]
    for index in range(CELLS):
        r^=(r<<13)&MASK;r^=r>>17;r^=(r<<5)&MASK;r&=MASK
        if p[2]==0:v=114+114*228
        elif p[2]==1:v=(-100000)&MASK
        elif p[2]==2:v=r
        elif p[2]==3:v=index
        elif p[2]==4:v=index%231+(index//231//2)*228
        elif p[2]==5:v=p[20] if index%2 else 114+114*228
        else:v=p[20]
        values.append(v)
    result=struct.pack('<%dI'%CELLS,*values)
    if len(grid_cache)<60:grid_cache[key]=result
    return result
def run_native(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[1]+i*29)&255])*n)
    vm.mem_write(0x70e494,grid(p))
    for a,v in zip([0xbca2e8,0xbca2f0,0xbca2f4,0xbca2f8,0xbca2fc,0x70e464,0x70e460,0xbca2bc,0xbca2c0,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x6fdc50,0x605a3c],p[3:18]):write(a,v)
    vm.mem_write(0xe38dca,bytes([p[18]&255]))
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for entry in ([0x4ddf90] if p[0]==0 else [0x4de080] if p[0]==1 else [0x4ddf90,0x4de080]):
        sp=STACK+0xf0000;write(sp,STOP)
        for reg,v in saved:vm.reg_write(reg,v)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
        vm.emu_start(entry,STOP,count=1000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack'
        assert all(vm.reg_read(reg)==v for reg,v in saved),'Native saved registers'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'memory':h.hexdigest()}
cases=[]
def case(**kw):
    keys=['op','seed','pattern','geomLeft','geomRight','geomBottom','geomCenterX','geomCenterY','width','height','x','y','left','top','right','bottom','editor','mode','dirty','unused','tile']
    p=[1,1,0,20,200,200,110,110,14,30,100,100,10,12,300,400,0,1,0,0,0]
    for k,v in kw.items():p[keys.index(k)]=v&MASK
    cases.append(tuple(p))
for op,pattern in itertools.product([0,2],range(7)):
    for tile in [0,25992,51983,0x80000000,0xffffffff]:
        case(op=op,pattern=pattern,tile=tile,seed=1)
for op,geometry in itertools.product([0,2],[(0,227,227,114,114),(100,128,128,114,114),(-1000,1000,1000,0,0),(0x7fffffff,0x80000000,0xffffffff,0x80000000,0x7fffffff)]):
    for pattern in [0,1,2,3,4,5]:
        case(op=op,pattern=pattern,geomLeft=geometry[0],geomRight=geometry[1],geomBottom=geometry[2],geomCenterX=geometry[3],geomCenterY=geometry[4])
for editor,mode,x,y in itertools.product([0,1,0x100],[0,1,0x100,0x80000001],[-1000,9,10,100,286,287,1000],[-1000,11,12,100,370,371,1001]):
    case(editor=editor,mode=mode,x=x,y=y,dirty=0x80)
for width,height,x,y in itertools.product([0,14,16,30,255,256,0x1234,0xffffffff],[0,30,255,256,0x10002],[-1,0x80000000,0x7fffffff],[0x80000000,0x7fffffff,65537]):
    case(width=width,height=height,x=x,y=y,editor=0x100,mode=1)
rng=random.Random(0x4ddf90)
for i in range(200):
    case(left=rng.getrandbits(32),top=rng.getrandbits(32),right=rng.getrandbits(32),bottom=rng.getrandbits(32),x=rng.getrandbits(32),y=rng.getrandbits(32),width=rng.getrandbits(32),height=rng.getrandbits(32),mode=1,editor=i%2)
cases=list(dict.fromkeys(cases))
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-camera-bounds/Release/camera_bounds_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':sha(image),'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,
'case_selection':{'limit':args.limit},'method':'Complete camera scan/clamp with actual DWORD width/height accessors, facade thunks, tile exclusion helper and signed diamond geometry predicate. Every grid cell, state field and adjacent guard byte compared. Native and C cdecl stack and saved registers checked.',
'limitations':['No screen drawing, camera scrolling input, map initialization or later frame dispatcher is proved here.','The native 16-bit parity mask is intentionally preserved; incidental scratch-register/EAX values of void functions are not semantic returns.']}
started=time.monotonic();checked=0;p=None
try:
    assert json.loads(process.stdout.readline())=={'fields':21}
    for p in cases:
        expected=run_native(p);process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush()
        line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
        actual=json.loads(line)
        if actual!=expected:(ROOT/'artifacts/camera_bounds_last_difference.json').write_text(json.dumps({'case':p,'C':actual,'native':expected},indent=2)+'\n')
        assert actual==expected,repr({'case':p,'C':actual,'native':expected})
        checked+=1
        if checked%200==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    hits={i for a,n in blocks for i in instructions if a<=i<a+n}
    if not args.limit:assert hits==set(instructions),'Uncovered native instructions '+repr([hex(i) for i in sorted(set(instructions)-hits)])
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    process.stdin.close();process.wait(timeout=10)
hits={i for a,n in blocks for i in instructions if a<=i<a+n}
sources=['src/unit_%05d.c'%n for n in [464,465,1011,1151,1880,1881,1885,1886]]+['include/camera_bounds.h','include/city_prepare.h','tests/camera_bounds.c','tools/camera_bounds_project/CMakeLists.txt','tools/verify_camera_bounds_emulation.py','metadata/camera-sidebar-coordinate-native.json','metadata/camera-bounds-helpers-native.json','metadata/CAMERA_BOUNDS_RECONSTRUCTION.md']
report.update(elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],native_uncovered_instructions=[hex(i) for i in sorted(set(instructions)-hits)],full_grid_cells=CELLS,controlled_algorithm_endpoints=[],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/camera-bounds-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2))
sys.exit(0 if report['passed'] else 1)
