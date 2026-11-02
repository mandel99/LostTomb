"""Whole481AA0 table/boundary leaf against original235 instructions, four C variants."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args()
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
facts_path=HERE/'map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
f=facts['functions']['00481aa0'];instructions={int(i['address'],16):i for i in f['assembly']['items']};assert len(instructions)==235
for a,i in instructions.items():assert bytes(vm.mem_read(a,len(bytes.fromhex(i['bytes']))))==bytes.fromhex(i['bytes'])
tables=[[0x70bf7c,0x70bf5c,0x70bf2c,0x70beec,0x70be9c,0x70be3c],[0x5c9ba8,0x5c9bc0,0x5c9be8,0x5c9c20,0x5c9c68,0x5c9cc0],[0x5c9d28,0x5c9d48,0x5c9d78,0x5c9db8,0x5c9e08,0x5c9e68],[0x5c9ed8,0x5c9f00,0x5c9f38,0x5c9f80,0x5c9fd8,0x5ca040],[0x5ca0b8,0x5ca0e8,0x5ca128,0x5ca178,0x5ca1d8,0x5ca248],[0x5ca2c8,0x5ca300,0x5ca348,0x5ca3a0,0x5ca408,0x5ca480]]
regions=[(0x5c9b98,0xa00),(0x70be2c,0x180),(0xbca2c0,48)]
def select(p):
    row=(p[2]-1)&MASK
    if row<5:col=(p[3]-1)&MASK;col=col if col<5 else 5
    else:row=5;col=p[3] if p[3]<5 else 5
    return row,col,tables[row][col]+p[4]*2
hits=set();events=[];running=False
def code(cpu,a,n,user):
    if a==STOP:return
    assert a in instructions,hex(a)
    hits.add(a)
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        events.append([a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x100000,('unexpected leaf write',hex(a),n)
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def native(p):
    global events,running
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes(((p[7]+j*13+i*37)%17-8)&255 for j in range(n)))
    put(0xbca2d0,p[5]);put(0xbca2d4,p[6])
    if p[8]:a=select(p)[2];put(a,p[9],1);put(a+1,p[9]>>8,1)
    sp=STACK+0xf0000;put(sp,STOP)
    for i,v in enumerate(p[:5]):put(sp+4+i*4,v)
    put(sp+24,0xbaadf00d)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);events=[];running=True
    try:vm.emu_start(0x481aa0,STOP,count=300)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    assert [read(sp+4+i*4) for i in range(5)]==p[:5] and read(sp+24)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'result':vm.reg_read(UC_X86_REG_EAX),'memory':h.hexdigest(),'reads':events.copy()}
default=[0,0,1,1,0,228,228,17,0,0];cases=[]
def add(**kw):
    p=default.copy()
    for name,v in kw.items():p[int(name[1:])]=v&MASK
    cases.append(p)
# Every selector/early-exit instruction, genuine inclusive physical padding, all ring points.
for family in range(1,7):
    for column in range(6):
        ring=column+1 if family<6 else column
        count=8*(column+1)+4*(family-1)
        for point in range(count):
            for x,y in [(0,0),(-1,-1),(228,228),(-8,0),(0,-8),(236,0),(0,236)]:add(p0=x,p1=y,p2=family,p3=ring,p4=point,p7=(family*43+column*7+point)%256)
# Fallback selectors remain exact integer contracts, without inventing internal clamps.
for family,ring,x,y in itertools.product([0,7,MASK],[MASK,0,1,5,6,7],[0,-2,228],[0,-2,228]):add(p0=x,p1=y,p2=family,p3=ring)
# Offset boundaries with explicit raw signed bytes, coordinate wrap and zero/small dimensions.
for x,y,w,h,dx,dy in itertools.product([-1,0,1,228,0x7fffffff,0x80000000],[-1,0,228],[0,1,228],[0,228],[-128,-1,0,1,127],[-1,0,1]):add(p0=x,p1=y,p5=w,p6=h,p8=1,p9=(dx&255)|((dy&255)<<8))
complete=len(cases)
if args.limit:cases=cases[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four actual shared binaries required'
executables=[(p or HERE/'build'/cfg/f'map_refresh_leaf_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
if args.production:
    assert len(set(executables))==4 and [e.name.lower() for e in executables]==['restore_map_refresh_leaf_verify.exe','restore_map_refresh_leaf_plain_verify.exe']*2
    assert all('map_refresh_phase/build' not in e.as_posix() for e in executables),'private binaries are not production proof'
    for staged,target in [('unit_01198.c','src/unit_01198.c'),('map_refresh.h','include/map_refresh.h')]:assert sha((HERE/staged).read_bytes())==sha((ROOT/target).read_bytes()),target
files=[Path(__file__),facts_path,HERE/'analyze_map_refresh.py',HERE/'map_refresh_leaf_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md']+[ROOT/'src/unit_01198.c',ROOT/'include/map_refresh.h'] if args.production else [Path(__file__),facts_path,HERE/'analyze_map_refresh.py',HERE/'map_refresh.h',HERE/'unit_01198.c',HERE/'map_refresh_leaf_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',ROOT/'src/unit_01198.c']
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p.read_bytes()) for p in files};binary_before={rel(e):sha(e.read_bytes()) for e in executables}
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();outcomes={0:0,1:0};table_counts={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':10}
    for i,p in enumerate(cases):
        expected=native(p);outcomes[expected['result']]+=1;row,col,a=select(p);key=f'{row}:{col}';table_counts[key]=table_counts.get(key,0)+1
        for k,proc in enumerate(procs):
            proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit: '+proc.stderr.read();actual=json.loads(line)
            if k%2:actual['reads']=[];want=dict(expected,reads=[])
            else:want=expected
            assert actual==want,json.dumps({'case':i,'p':p,'variant':str(executables[k]),'actual':actual,'expected':want})
        if (i+1)%6000==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
if not args.limit:assert len(hits)==235 and len(table_counts)==36
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'native_algorithm_instructions':235,'coverage':[len(hits),235],'table_branches':table_counts,'branch_outcomes':outcomes,'corpus_sha256':sha(json.dumps(cases,separators=(',',':')).encode()),'compiled_C_sha256':binary_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':binary_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'native_exports':{rel(facts_path):sha(facts_path.read_bytes())},'source_drift':[rel(p) for p in files if sha(p.read_bytes())!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e.read_bytes())!=binary_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Only complete481AA0 leaf is reconstructed and compared.488E90/489030,481540/481690 and56133C/FPU remain unfinished.','All36 table branches and every canonical-size table index are covered with prepared signed-byte table data. Fallback selectors and coordinate wrap are separate valid-mapped ABI probes; malformed unmapped table addresses are excluded.','Native leaf has no callees, no floating point and no global writes. Ordered external reads, fullEAX, saved registers, stack, prepared tables/dimension guardbytes are compared.','Private O2 leaf/fixture uses the existing Debug static data library and static Debug C runtime. Shared recipe inherits actual root configuration runtime. No whole-map-refresh/UI/menu completion claim.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-leaf-emulation-partial.json' if args.limit else 'map-refresh-leaf-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','branch_outcomes','elapsed_seconds']}))
