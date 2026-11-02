"""Complete third map-refresh postpass and its real transitive C callees, no internal seams."""
from pathlib import Path
import re,argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true');ap.add_argument('--manifest-sha256')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=ROOT/'artifacts/map_refresh_phase/map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
geometry_path=ROOT/'artifacts/map_refresh_postpass1_phase/postpass1-geometry-native.json';geometry=json.loads(geometry_path.read_text());assert geometry['native_sha256']==PIN
for name,want in geometry['actual_C_sources'].items():assert sha(ROOT/name)==want,name
scope=['004777c0','00479e70','0046f930','0047a1c0','0047db00','00484f00','0047ddd0','00484be0','00484d70','0056133c'];instructions={};decoded={};decoder=Cs(CS_ARCH_X86,CS_MODE_32)
def authenticate(name,items):
    for item in items:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name;sub=list(decoder.disasm(b,a));assert b''.join(bytes(i.bytes) for i in sub)==b
        for i in sub:decoded[i.address]=a
for name in scope:authenticate(name,facts['functions'][name]['assembly']['items'])
authenticate('0046f7b0',geometry['function']['assembly']['items'])
regions=[(0xa509ac,0xcb30),(0xa5d4bc,0xcb30),(0xacf84c,0xcb30),(0xae8e6c,0xcb30),(0xaf557c,0x33460),(0xb285bc,0xcb30),(0xb67d0c,0xcb30),(0xb7481c,0x32c60),(0xbc9730,0xc00),(0xf56c24,0x630),(0x56f580,0x30),(0xd05710,0x32c60),(0x5cac48,0xc20),(0x6f4000,0x1000),(0x78d79c,0x30)]
record_input=bytes(vm.mem_read(0x5cac58,0xc00))
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();setup_hits=set();events=[];running=False;preparing=False;pending=None;bounds_args=None;water_args=None;bound_outcomes={0:0,1:0};water_outcomes={0:0,1:0};op_counts={};helper_returns=[];branches={}
fp_sites={0x47a049,0x47a04d,0x47a053};quiet={'0046f930','0047a1c0','0047db00','00484f00'}
def fp_event(a):
    top=(vm.reg_read(UC_X86_REG_FPSW)>>11)&7;sig,exponent=vm.reg_read(UC_X86_REG_FP0+top);events.append([2,a,sig&MASK,sig>>32,exponent]);op_counts[f'{a:08x}']=op_counts.get(f'{a:08x}',0)+1
def helper(a,phase,arg):events.append([3,a,phase,arg,read(0xbc9768),*[read(0xbc97c0+i*4) for i in range(8)]])
def code(cpu,a,n,user):
    global pending,bounds_args,water_args
    if pending:fp_event(pending);pending=None
    if a==STOP:return
    assert a in decoded,f'escaped native instruction{a:08x}'
    owner=instructions[decoded[a]]
    if preparing:assert owner=='0046f7b0';setup_hits.add(decoded[a]);return
    assert running and owner in scope;hits.add(decoded[a])
    if helper_returns and a==helper_returns[-1][0]:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    if a in [0x47db00,0x484f00]:
        sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4) if a==0x47db00 else 0;helper(a,0,arg);helper_returns.append((read(sp),a,arg))
    if a in fp_sites:pending=a
    elif a==0x56133c:fp_event(a)
    elif a==0x46f930:
        sp=cpu.reg_read(UC_X86_REG_ESP);bounds_args=[read(sp+4),read(sp+8)]
    elif a==0x479e95:
        result=cpu.reg_read(UC_X86_REG_EAX);events.append([4,*bounds_args,result]);bound_outcomes[result&255]+=1
    elif a==0x47a1c0:
        sp=cpu.reg_read(UC_X86_REG_ESP);water_args=[read(sp+4),read(sp+8)]
    elif a==0x479f39:
        result=cpu.reg_read(UC_X86_REG_EAX);result=result if signed(read(0x78d7b0))>0 else result&255;events.append([5,*water_args,result]);water_outcomes[result&255]+=1
    if a in [0x479fcf,0x479fe3,0x47a099,0x47a0bf]:branches[f'{a:08x}']=branches.get(f'{a:08x}',0)+1

def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        if instructions[decoded[cpu.reg_read(UC_X86_REG_EIP)]] in quiet:return
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        v=read(a,n);events.append([0,a,n,v&MASK,v>>32])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        if instructions[decoded[cpu.reg_read(UC_X86_REG_EIP)]] in quiet:return
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        assert n in [1,4],(hex(a),n);v=value&((1<<(n*8))-1);events.append([1,a,n,v&MASK,v>>32])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def prepare(p):
    global preparing
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[14]+i*37)&255])*n)
    for a,value in [(0xa5d4cc,p[10]),(0xacf85c,p[11])]:vm.mem_write(a,bytes([value&255])*0xcb10)
    vm.mem_write(0xaf598c,b'\0'*0x32c40);vm.mem_write(0xd05720,b'\0'*0x32c40);offset=((228-p[2])//2)*228+(228-p[1])//2
    for a,value in [(0xbca2d0,p[1]),(0xbca2d4,p[2]),(0xbca2d8,offset),(0xbca2dc,228-p[1])]:put(a,value)
    sp=STACK+0x80000;put(sp,STOP);vm.reg_write(UC_X86_REG_ESP,sp);preparing=True
    try:vm.emu_start(0x46f7b0,STOP,count=20000)
    finally:preparing=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    put(0xbca2e0,p[18]);put(0x6f4000,0x6f4800);put(0x6f4008,(offset+p[25])%51984);put(0x6f4018,0 if p[12]==1 else 128 if p[12]==3 else 1,1);put(0x6f4804,p[21])
    for dy in range(-2,3):
        for dx in range(-2,3):
            nx=signed(p[16])+dx;ny=signed(p[17])+dy
            if nx<0 or ny<0 or nx>=p[1] or ny>=p[2]:continue
            index=offset+ny*228+nx;put(0xaf598c+index*4,p[9] if p[15]==1 else 0);put(0xd05720+index*4,0x6f4000 if p[12] else 0)
    if p[16]<p[1] and p[17]<p[2]:
        index=offset+p[17]*228+p[16]
        if p[15]==2:
            for i,d in enumerate([-228,-227,1,229,228,227,-1,-229]):put(0xaf598c+(index+d)*4,p[9] if p[24]>>i&1 else 0)
        put(0xaf598c+index*4,p[8])
    vm.mem_write(0x5cac58,record_input)
    for i in range(192):put(0x5cac58+i*16+13,p[22],1);put(0x5cac58+i*16+14,p[23],1)
    for a,value in [(0x56f598,0x3f847ae147ae147b),(0x56f590,0x4020000000000000)]:put(a,value,8)
    for a,value in [(0xf56c34,p[14]),(0xf57234,p[14]+0x10000),(0x78d7ac,p[19]),(0x78d7b0,p[20])]:put(a,value)

def native(p):
    global events,running,pending,bounds_args,water_args,helper_returns
    prepare(p);sp=STACK+0xf0000;put(sp,STOP);argv=[p[3],p[4],p[5],p[6]] if p[0] else [p[3],p[4],p[7]]
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[13]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];pending=None;bounds_args=None;water_args=None;helper_returns=[];running=True
    try:vm.emu_start(0x4777c0 if p[0] else 0x479e70,STOP,count=2000000)
    finally:running=False
    assert not helper_returns and pending is None and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved) and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[13] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':h.hexdigest(),'events':events.copy()}
default=[0,16,16,8,8,8,8,1,0x10000,0x10004,99,1,1,0x27f,1000,0,8,8,0,1000,1,60,1,0,255,5];cases=[]
def add(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(p)
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]
for cw,height,kind,parity in itertools.product(cws,[0,1,12,13,24,25,99,100,254,255],[1,2],[0,1]):add(p10=height,p11=parity,p12=kind,p13=cw)
for height in range(256):add(p10=height,p11=height,p21=[0,54,59,60,65,77,83,84][height%8])
for pattern,kind in itertools.product(range(256),[0,1,2]):add(p12=kind,p15=2,p24=pattern,p22=[1,2,3][pattern%3],p23=pattern)
for route,flag,neighbors,setvalue in itertools.product([0,1],[0,0x10000,0x10008,0x10040,0x10100,0x10004,0x80010000],[0,0x10000,0x10004],[0,1,2]):add(p0=route,p8=flag,p9=neighbors,p15=1,p7=setvalue)
for delta,water,orientation,kind in itertools.product([0,53,54,59,60,65,77,78,83,84],[0,1],[0,2,4,6],[1,2]):add(p21=delta,p20=water,p18=orientation,p12=kind,p10=0)
for w,h in [(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)]:
    for x,y in [(0,0),(max(w-1,0),max(h-1,0)),(w//2,h//2)]:
        for route,kind in itertools.product([0,1],[0,1,3]):add(p0=route,p1=w,p2=h,p3=x,p4=y,p5=x+1,p6=y+1,p16=x,p17=y,p12=kind,p15=1)
for w,h,x,y in [(228,1,113,-2),(1,228,-2,200),(16,16,-1,8),(16,16,16,8)]:add(p1=w,p2=h,p3=x,p4=y,p16=w//2,p17=h//2)
for rectangle in [(-1,-1,1,1),(9,9,8,8),(0,0,228,228),(0,0,-1,-1)]:add(p0=1,p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3],p15=1)
for kind in [0,1,2]:add(p0=1,p1=228,p2=228,p3=0,p4=0,p5=227,p6=227,p16=114,p17=114,p12=kind)
for seed in [0,64,255,256,0x80000000,MASK]:add(p14=seed,p10=255,p15=1)
complete=len(cases)
if args.limit:cases=cases[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post3_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
FP=ROOT/'artifacts/map_refresh_arithmetic_phase';RAW=ROOT/'artifacts/map_refresh_phase/x87_conversion'
P2=ROOT/'artifacts/map_refresh_postpass2_phase';MN=ROOT/'artifacts/map_neighbors_phase'
mappings=[(HERE/'unit_01052.c',ROOT/'src/unit_01052.c'),(HERE/'unit_01095.c',ROOT/'src/unit_01095.c'),(HERE/'map_refresh_postpass3.h',ROOT/'include/map_refresh_postpass3.h'),(P2/'unit_01233.c',ROOT/'src/unit_01233.c'),(P2/'unit_01234.c',ROOT/'src/unit_01234.c'),(P2/'map_refresh_postpass2.h',ROOT/'include/map_refresh_postpass2.h'),(MN/'unit_01140.c',ROOT/'src/unit_01140.c'),(MN/'map_neighbors_phase.h',ROOT/'include/map_neighbors_phase.h'),(FP/'map_refresh_fp.c',ROOT/'runtime/map_refresh_fp.c'),(FP/'map_refresh_arithmetic.h',ROOT/'include/map_refresh_arithmetic.h'),(RAW/'map_x87_conversion.c',ROOT/'runtime/map_x87_conversion.c'),(RAW/'map_x87_conversion.h',ROOT/'include/map_x87_conversion.h')]
root_c=[ROOT/'src'/f'unit_{n:05d}.c' for n in [1009,1011,1097,1137,1236]]
prerequisite_manifests=[MN/'map_neighbors_phase_apply_map.json',P2/'postpass2_apply_map.json',FP/'map_refresh_arithmetic_apply_map.json']
# Explicit hashes freeze the actual C prerequisites; their earlier proofs remain immutable.
pins={
 MN/'map_neighbors_phase_apply_map.json':'e1141340a25f887ab123c85a8a34ac014ad7b64437c26bad73c369f5e6168561',
 P2/'postpass2_apply_map.json':'7b0869757729657686f31467bb5f7cb1c2b20d72023c3cdf4733b41258bdff3d',
 FP/'map_refresh_arithmetic_apply_map.json':'203457dbeb0bbdc67608826060d5c71816ddf6590a9a68f9bef047137830bf76'}
for path,want in pins.items():assert sha(path)==want,path
frozen_source_hashes={}
for path in prerequisite_manifests:
    manifest=json.loads(path.read_text())
    for entry in manifest['mappings']:frozen_source_hashes[entry['staged']]=entry['staged_sha256']
    frozen_source_hashes.update(manifest.get('proof_hashes',{}))
for staged,target in mappings[3:]:assert sha(staged)==frozen_source_hashes[staged.relative_to(ROOT).as_posix()],staged
for n,want in {1009:'c2ae6c562fee3614c2493d47ef14a0856244cd958e93c98da106a9978de042b1',1011:'30f894ef75ee0b2f4a657df39c22ca660a35650bae16dcd273ae774cc40e2d0f',1097:'5b8c240c7495e7199ece022c72e92e9e4f43f11060690e8c5f00a2594c61ba20',1137:'fb8e7ee47f65bd86d17672b738f85a3069a312c12baec01ac159a06e4edf9159',1236:'ec59c404ff88c9f5728617618970bedbd14bbc8cde75ca6d089d0ccfb4457777'}.items():assert sha(ROOT/'src'/f'unit_{n:05d}.c')==want,n
record_header=(P2/'selector_native_data.h').read_text();record_body=record_header.split('post2_main_records[3072]={',1)[1].split('};',1)[0];assert bytes(int(v,16) for v in re.findall(r'0x([0-9a-fA-F]{2})',record_body))==record_input
files=[HERE/'postpass3_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),geometry_path,facts_path,P2/'selector_native_data.h',*root_c,*prerequisite_manifests,ROOT/'include/city_prepare.h',ROOT/'include/terrain_redraw.h']+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    freeze_path=HERE/'postpass3_apply_map.json'
    if args.manifest_sha256:assert sha(freeze_path)==args.manifest_sha256
    freeze=json.loads(freeze_path.read_text());assert freeze['frozen'] and freeze['native_sha256']==PIN
    for path,want in freeze['proof_hashes'].items():assert sha(ROOT/path)==want,path
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass3_verify.exe','restore_map_refresh_postpass3_plain_verify.exe']*2
    assert all('map_refresh_postpass3_phase/build' not in p.as_posix() for p in executables),'private binaries are not production evidence'
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
    expected={HERE/'postpass3_fixture.c',*root_c,*[target for staged,target in mappings if target.suffix=='.c']}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve()
        compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed')
        assert ('MAP_REFRESH_POSTPASS3_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={'_FUN_004777c0':'unit_01052.obj','_FUN_00479e70':'unit_01095.obj','_FUN_0046f930':'unit_01011.obj','_FUN_0046f7b0':'unit_01009.obj','_FUN_0047a1c0':'unit_01097.obj','_FUN_0047db00':'unit_01137.obj','_FUN_0047ddd0':'unit_01140.obj','_FUN_00484be0':'unit_01233.obj','_FUN_00484d70':'unit_01234.obj','_FUN_00484f00':'unit_01236.obj','_restore_map_trunc80':'map_x87_conversion.obj','_mr_fp_mul':'map_refresh_fp.obj'}
for e in executables:
    mp=e.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};binary_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();routes={0:0,1:0}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':26}
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
coverage={name:[sum(instructions[a]==name for a in hits),sum(v==name for v in instructions.values())] for name in scope}
missing={name:[f'{a:08x}' for a,v in instructions.items() if v==name and a not in hits] for name in scope}
if not args.limit:assert coverage['004777c0']==[74,74] and coverage['00479e70']==[192,192] and coverage['0047ddd0']==[80,80] and coverage['0056133c']==[15,15]
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'missing_instructions':missing,'setup_geometry_coverage':[len(setup_hits),90],'bounds_outcomes':bound_outcomes,'water_outcomes':water_outcomes,'branches':branches,'fp_event_counts':op_counts,'per_route':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':binary_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':binary_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(p):sha(p) for p in [facts_path,geometry_path]},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=binary_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete4777C0/479E70 with actual root bounds/water/gather/invert, frozen complete47DDD0 and frozen selectors; all numeric arithmetic uses the frozen exact integer raw80 components. No internal controlled algorithm endpoints.','Ordered new-body/47DDD0/selector reads and stores/raw80 FP intermediates are compared. Unchanged bounds/water internal reads are excluded; arguments and meaningful returns compare. Unchanged gather/invert internal memory order is excluded; real entry/exit full8DWORD snapshots compare, with whole memory canaries.','Native void EAX is unconsumed and is not compared as a fabricated result. Water disabled path has only AL defined; enabled full native EAX compares. Native consumed by-value argument stack slots mutate; caller discards those slots.','All24 all-exception-masked CW, BYTE heights/parity, bounded disjoint mapped object/definition/record tables. Exception status/traps remain unproved. No arbitrary float state or canonical56133C/globalST0 replacement.','Private O2 target uses Debug data/staticDebugCRT; shared recipe inherits rootDebugMTd/ReleaseMT. No full488E90/canonical489030/otherpostpasses completion claim.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass3-partial.json' if args.limit else 'map-refresh-postpass3-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','branches','elapsed_seconds']}))
