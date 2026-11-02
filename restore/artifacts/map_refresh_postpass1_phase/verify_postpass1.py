"""Complete first map-refresh postpass and its real transitive C callees, no internal seams."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=ROOT/'artifacts/map_refresh_phase/map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
geometry_path=HERE/'postpass1-geometry-native.json';geometry=json.loads(geometry_path.read_text());assert geometry['native_sha256']==PIN
for name,want in geometry['actual_C_sources'].items():assert sha(ROOT/name)==want,name
scope=['00477500','00479b70','0046f930','0056133c'];instructions={};decoded={};decoder=Cs(CS_ARCH_X86,CS_MODE_32)
def authenticate(name,items):
    for item in items:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name;sub=list(decoder.disasm(b,a));assert b''.join(bytes(i.bytes) for i in sub)==b
        for i in sub:decoded[i.address]=a
for name in scope:authenticate(name,facts['functions'][name]['assembly']['items'])
authenticate('0046f7b0',geometry['function']['assembly']['items'])
regions=[(0xa509ac,0xcb30),(0xa5d4bc,0xcb30),(0xacf84c,0xcb30),(0xae8e6c,0xcb30),(0xaf597c,0x32c60),(0xb285bc,0xcb30),(0xb67d0c,0xcb30),(0xb7481c,0x32c60),(0xbc9750,0xbe0),(0xf56c50,0x120),(0x56f560,0x40)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();setup_hits=set();events=[];running=False;preparing=False;pending=None;bounds_args=None;bound_outcomes={0:0,1:0};families={};op_counts={}
fp_sites={0x479d1f,0x479d23,0x479d29,0x479d55,0x479d59,0x479d5f}
dead_modulo={0x479d7f,0x479d80,0x479d83,0x479da3,0x479da4,0x479da7,0x479dc5,0x479dc6,0x479dc9}
def fp_event(a):
    top=(vm.reg_read(UC_X86_REG_FPSW)>>11)&7;sig,exponent=vm.reg_read(UC_X86_REG_FP0+top);events.append([2,a,sig&MASK,sig>>32,exponent]);op_counts[f'{a:08x}']=op_counts.get(f'{a:08x}',0)+1
def code(cpu,a,n,user):
    global pending,bounds_args
    if pending:fp_event(pending);pending=None
    if a==STOP:return
    assert a in decoded,f'escaped native instruction{a:08x}'
    owner=instructions[decoded[a]]
    if preparing:assert owner=='0046f7b0';setup_hits.add(decoded[a]);return
    assert running and owner in scope;hits.add(decoded[a])
    if a in fp_sites:pending=a
    elif a==0x56133c:fp_event(a)
    elif a==0x46f930:
        sp=cpu.reg_read(UC_X86_REG_ESP);bounds_args=[read(sp+4),read(sp+8)]
    elif a==0x479b94:
        result=cpu.reg_read(UC_X86_REG_EAX);events.append([5,*bounds_args,result]);bound_outcomes[result&255]+=1
    elif a==0x479dcf:
        record=cpu.reg_read(UC_X86_REG_EBP);families[f'{record:03x}']=families.get(f'{record:03x}',0)+1
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        if instructions[decoded[cpu.reg_read(UC_X86_REG_EIP)]]=='0046f930':return
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        v=read(a,n);events.append([0,a,n,v&MASK,v>>32])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        assert n in [1,4],(hex(a),n);v=value&((1<<(n*8))-1);events.append([1,a,n,v&MASK,v>>32])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def prepare(p):
    global preparing
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[14]+i*37)&255])*n)
    for a,value in [(0xa509bc,p[11]),(0xa5d4cc,p[10]),(0xacf85c,p[12])]:vm.mem_write(a,bytes([value&255])*0xcb10)
    vm.mem_write(0xaf598c,b'\0'*0x32c40);offset=((228-p[2])//2)*228+(228-p[1])//2
    for a,value in [(0xbca2d0,p[1]),(0xbca2d4,p[2]),(0xbca2d8,offset),(0xbca2dc,228-p[1])]:put(a,value)
    sp=STACK+0x80000;put(sp,STOP);vm.reg_write(UC_X86_REG_ESP,sp);preparing=True
    try:vm.emu_start(0x46f7b0,STOP,count=20000)
    finally:preparing=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    for dy in range(-2,3):
        for dx in range(-2,3):
            nx=signed(p[16])+dx;ny=signed(p[17])+dy
            if nx<0 or ny<0 or nx>=p[1] or ny>=p[2]:continue
            index=offset+ny*228+nx;put(0xaf598c+index*4,p[9] if p[15] else 0)
    if p[16]<p[1] and p[17]<p[2]:put(0xaf598c+(offset+p[17]*228+p[16])*4,p[8])
    for a,value in [(0x56f588,0x3f84afd6a052bf5b),(0x56f580,0x4008000000000000),(0x56f570,0x4058000000000000),(0x56f578,0x4028000000000000)]:put(a,value,8)
    for a,value in [(0xf56c64,p[14]),(0xf56d4c,p[14]+0x10000),(0xf56cf4,p[14]+0x20000),(0xf56cec,p[14]+0x30000)]:put(a,value)
def native(p):
    global events,running,pending,bounds_args
    prepare(p);sp=STACK+0xf0000;put(sp,STOP);argv=[p[3],p[4],p[5],p[6]] if p[0] else [p[3],p[4],p[7]]
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[13]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];pending=None;bounds_args=None;running=True
    try:vm.emu_start(0x477500 if p[0] else 0x479b70,STOP,count=2000000)
    finally:running=False
    assert pending is None and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved) and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[13] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':h.hexdigest(),'events':events.copy()}
default=[0,16,16,8,8,9,9,1,0x800,0x800,99,1,5,0x27f,1000,0,8,8];cases=[]
def add(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(p)
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]
for cw,first,second in itertools.product(cws,[0,1,33,34,66,67,99,100,133,166,199,232,255],[0,1,89,96,97,100,255]):add(p10=first,p11=second,p13=cw)
for first in range(256):add(p10=first,p11=[0,1,96,97][first%4],p12=first)
for second in range(256):add(p10=99,p11=second,p12=second)
flags=[0,0x800,0x900,0xa00,0xc00,0x1800,0x840,0x808,0x10000808,0x820,0x880,0x80000800,0x10000800]
for route,flag,neighbors,setvalue in itertools.product([0,1],flags,[0,0x800],[0,1,2]):add(p0=route,p8=flag,p9=neighbors,p15=1,p7=setvalue,p5=8,p6=8)
for w,h in [(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)]:
    for x,y in [(0,0),(max(w-1,0),max(h-1,0)),(w//2,h//2)]:
        for route,cw in itertools.product([0,1],cws[::4]):add(p0=route,p1=w,p2=h,p3=x,p4=y,p5=x+1,p6=y+1,p16=x,p17=y,p13=cw,p15=1)
# Valid physical-mapping extensions expose native lower clipping/empty-row/empty-column contracts.
for w,h,x,y in [(228,1,113,-2),(1,228,-2,200),(16,16,-1,8),(16,16,16,8)]:add(p1=w,p2=h,p3=x,p4=y,p16=w//2,p17=h//2)
for rectangle in [(-1,-1,1,1),(9,9,8,8),(0,0,228,228),(0,0,-1,-1)]:add(p0=1,p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3],p15=1)
for cw in cws[::4]:add(p0=1,p1=228,p2=228,p3=0,p4=0,p5=227,p6=227,p16=114,p17=114,p13=cw,p15=1)
for seed in [0,64,255,256,0x80000000,MASK]:add(p14=seed,p10=255,p11=100,p15=1)
complete=len(cases)
if args.limit:cases=cases[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post1_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
FP=ROOT/'artifacts/map_refresh_arithmetic_phase';RAW=ROOT/'artifacts/map_refresh_phase/x87_conversion'
mappings=[(HERE/'unit_01048.c',ROOT/'src/unit_01048.c'),(HERE/'unit_01094.c',ROOT/'src/unit_01094.c'),(HERE/'map_refresh_postpass1.h',ROOT/'include/map_refresh_postpass1.h'),(FP/'map_refresh_fp.c',ROOT/'runtime/map_refresh_fp.c'),(FP/'map_refresh_arithmetic.h',ROOT/'include/map_refresh_arithmetic.h'),(RAW/'map_x87_conversion.c',ROOT/'runtime/map_x87_conversion.c'),(RAW/'map_x87_conversion.h',ROOT/'include/map_x87_conversion.h')]
files=[HERE/'postpass1_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),geometry_path,facts_path,ROOT/'src/unit_01009.c',ROOT/'src/unit_01011.c',ROOT/'include/city_prepare.h']+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass1_verify.exe','restore_map_refresh_postpass1_plain_verify.exe']*2
    assert all('map_refresh_postpass1_phase/build' not in p.as_posix() for p in executables),'private binaries are not production evidence'
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
    expected={HERE/'postpass1_fixture.c',ROOT/'src/unit_01048.c',ROOT/'src/unit_01094.c',ROOT/'src/unit_01009.c',ROOT/'src/unit_01011.c',ROOT/'runtime/map_refresh_fp.c',ROOT/'runtime/map_x87_conversion.c'}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve()
        compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed')
        assert ('MAP_REFRESH_POSTPASS1_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={'_FUN_00477500':'unit_01048.obj','_FUN_00479b70':'unit_01094.obj','_FUN_0046f930':'unit_01011.obj','_FUN_0046f7b0':'unit_01009.obj','_restore_map_trunc80':'map_x87_conversion.obj','_mr_fp_mul':'map_refresh_fp.obj'}
for e in executables:
    mp=e.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};binary_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();routes={0:0,1:0}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':18}
    for i,p in enumerate(cases):
        expected=native(p);routes[p[0]]+=1
        for k,proc in enumerate(procs):
            proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();out=proc.stdout.readline();assert out,'C exited: '+proc.stderr.read();actual=json.loads(out)
            if k%2:actual['events']=[];want=dict(expected,events=[])
            else:want=expected
            if actual!=want:
                ae=actual['events'];ee=want['events'];mismatch=next((j for j in range(min(len(ae),len(ee))) if ae[j]!=ee[j]),min(len(ae),len(ee)))
                raise AssertionError(json.dumps({'case':i,'p':p,'variant':str(executables[k]),'event_mismatch':mismatch,'actual_events':ae[max(0,mismatch-3):mismatch+4],'expected_events':ee[max(0,mismatch-3):mismatch+4],'actual_other':{a:v for a,v in actual.items() if a!='events'},'expected_other':{a:v for a,v in want.items() if a!='events'}}))
        if (i+1)%256==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(instructions[a]==name for a in hits),sum(v==name for v in instructions.values())] for name in scope};missing={a for a,name in instructions.items() if name=='00479b70' and a not in hits}
if not args.limit:assert coverage['00477500']==[83,83] and missing==dead_modulo and coverage['0046f930']==[40,40] and coverage['0056133c']==[15,15]
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'unreachable_signed_modulo_in_byte_domain':[f'{a:08x}' for a in sorted(dead_modulo)],'setup_geometry_coverage':[len(setup_hits),90],'bounds_outcomes':bound_outcomes,'sprite_families':families,'fp_event_counts':op_counts,'per_route':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':binary_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':binary_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(p):sha(p) for p in [facts_path,geometry_path]},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=binary_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete477500 and479B70 plus actual accepted46F930 and exact frozen numeric/raw80 C helpers; no internal algorithm seams. Native void EAX is unconsumed and is not compared as a fabricated return value.','Nine signed-remainder correction instructions are unreachable for BYTE input domain: each MOV AL follows EAX==0 or XOR EAX,EAX, guaranteeing signbit0. All other213-body instructions execute in full corpus.','Internal reads of unchanged46F930 are excluded from ordered trace; actual full native/C return value and arguments are compared and caller consumesAL. All new-body reads/writes/raw80 intermediates are ordered; whole11guardregions and old x87 values/CW/GPRs/SP compared.','Native consumed by-value stack argument slots mutate inside these void functions; actual callers discard outgoing slots. Meaningful ABI/state contract is compared, not deallocated stack bytes.','All24 all-exception-masked CW; FP exception/status/traps remain unproved. Private O2 uses Debugdata/staticDebugCRT; shared recipe uses rootDebugMTd/ReleaseMT. No full488E90/canonical489030/otherpostpasses completion claim.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass1-partial.json' if args.limit else 'map-refresh-postpass1-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','sprite_families','elapsed_seconds']}))
