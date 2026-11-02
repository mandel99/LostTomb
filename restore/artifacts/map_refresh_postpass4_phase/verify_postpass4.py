"""Whole building/bridge cardinal neighbor algorithms against complete native closure."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true');ap.add_argument('--manifest-sha256')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes();u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=HERE/'postpass4-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and sha(ROOT/facts['parent_dossier']['path'])==facts['parent_dossier']['sha256']
instructions={}
for name,f in facts['functions'].items():
    assert not f['assembly']['has_more']
    for item in f['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
regions=[(0xaf557c,0x33460),(0xb4e4ec,0x19a40),(0xbc9730,0xc0),(0x6f5ff0,0x30)];STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();events=[];running=False;branches={};routes={0:0,1:0,2:0};counter_changes={'increment':0,'wrap':0}
def code(cpu,a,n,user):
    if a==STOP:return
    assert a in instructions,hex(a);hits.add(a)
    if a in [0x47e628,0x47e683,0x47e6dd,0x47e738,0x47e95b,0x47e9a8,0x47ea03,0x47ea70,0x47eabb,0x47eb04,0x47eb52,0x47ec6b]:branches[f'{a:08x}']=branches.get(f'{a:08x}',0)+1

def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if a!=0x6f6000:events.append([0,a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if a!=0x6f6000:events.append([1,a,n,value&((1<<(n*8))-1)])
        else:counter_changes['increment']+=1;counter_changes['wrap']+=int((value&MASK)==0)
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
offsets=[-228,-227,1,229,228,227,-1,-229]
def prepare(p):
    global regions
    base_id=signed(p[22]);assert -32768<=base_id and base_id+12<=32767
    regions=regions[:4]+[(0x7999d8+(base_id+i*4)*264,0x128) for i in range(4)]
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[23]+i*37)&255])*n)
    put(0xbc9768,p[1]);put(0x6f6000,p[3])
    for i in range(8):put(0xbc97c0+i*4,p[24]^(i*0x11335577&MASK));put(0xaf598c+(p[1]+offsets[i])*4,p[5+i])
    put(0xaf598c+p[1]*4,p[4])
    for i in range(4):
        record_id=base_id+i*4;a=0x7999e8+record_id*264;put(0xb4e6fc+(p[1]+offsets[i*2])*2,record_id,2);put(a,p[13+i],2);put(a+2,p[17+i],2);put(a+154,p[21]>>(i*8),1)

def native(p):
    global events,running
    prepare(p);sp=STACK+0xf0000;put(sp,STOP)
    argv=[p[2],0x6f6000] if p[0]==1 else []
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[25]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];running=True
    try:vm.emu_start([0x47e5c0,0x47eb60,0x47e870][p[0]],STOP,count=10000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert [read(sp+4+i*4) for i in range(len(argv))]==argv and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[25] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
default=[0,26000,6,0,0x40,*([0]*8),0x3a,0xaa,0xab,0xc8,1,2,1,2,0,0,17,0x80000000,0x37f];cases=[];categories=[]
def add(category='ordinary',**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    assert len(p)==26
    cases.append(p);categories.append(category)
# Whole gate/type/WORDorientation across all four cardinal neighbors.
for gate,kind,orientation in itertools.product(range(16),[0,0x3a,0xaa,0xab,0xc8,0xc9,0xca,0x5a,0x13a,0x8000,0xffff],[0,1,2,0x101,0x102,0xffff]):
    kw={f'p{5+i*2}':0x8000 if gate>>i&1 else 0 for i in range(4)};kw.update({f'p{13+i}':kind for i in range(4)});kw.update({f'p{17+i}':orientation for i in range(4)});add('building_WORDs',**kw)
# Direction helper: exact dependency cases, odd-direction aliases separately qualified.
for direction,current,nflag,pairs,counter in itertools.product(range(8),[0,0x40],[0,0x100,0x140,0x8000100],range(4),[0,0x7fffffff,MASK]):
    row=[[-228,-1,1,-229,-227],[1,-228,228,-227,229],[228,-1,1,227,229],[-1,-228,228,-229,227]][direction//2]
    flags={d:0 for d in offsets};flags[row[0]]=nflag;test=row[1:3] if current else row[3:5]
    for j,d in enumerate(test):flags[d]=0x8000000 if pairs>>j&1 else 0
    add('direction_helper',p0=1,p2=direction,p3=counter,p4=current,**{f'p{5+i}':flags[d] for i,d in enumerate(offsets)})
# Full composed topology, all cardinal eligibility/road masks and representative diagonal masks.
for road,gates,roads,diags in itertools.product([0,0x40],range(16),range(16),[0,5,10,15]):
    fs=[0]*8
    for i in range(4):fs[i*2]=(0x100 if gates>>i&1 else 0)|(0x8000000 if roads>>i&1 else 0);fs[i*2+1]=0x8000000 if diags>>i&1 else 0
    add('composed_topology',p0=2,p4=road|(0x10000 if (gates^roads)&1 else 0),**{f'p{5+i}':v for i,v in enumerate(fs)})
# RecordBYTE154 and opposite-road suppression on all four building bridge directions.
for current,mask,recordflags,kind in itertools.product([0,0x40,0x10000,0x10040],range(16),[0,0x01010101,0x02020202,0x80808080,0xffffffff],[0x5a,0x15a]):
    fs=[0]*8
    for i in range(4):fs[i*2]=8|(4 if mask>>i&1 else 0)|(0x40 if mask>>(3-i)&1 else 0)
    add('composed_buildings',p0=2,p4=current,p21=recordflags,**{f'p{13+i}':kind for i in range(4)},**{f'p{5+i}':v for i,v in enumerate(fs)})
# Explicit valid mapped API extensions: signed WORD index, high DWORD tile and physical edge padding.
for route,tile,recordid in itertools.product([0,1,2],[0,1,227,228,51983],[-128,-1,128,256,1024,32755]):add('mapped_index_padding',p0=route,p1=tile,p22=recordid,p4=0x10040,**{f'p{5+i}':0x818c for i in range(8)})
# These integer algorithms must preserve all24 exception-masked PC/RC/IC control-word combinations.
for route,cw in itertools.product([0,1,2],[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]):add('CW_preservation',p0=route,p25=cw,**{f'p{5+i}':0x814c for i in range(8)})
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post4_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
mappings=[(HERE/f'unit_{n:05d}.c',ROOT/'src'/f'unit_{n:05d}.c') for n in [1149,1152,1153]]+[(HERE/'map_refresh_postpass4.h',ROOT/'include/map_refresh_postpass4.h')]
files=[HERE/'postpass4_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),facts_path]+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    freeze_path=HERE/'postpass4_apply_map.json'
    if args.manifest_sha256:assert sha(freeze_path)==args.manifest_sha256
    freeze=json.loads(freeze_path.read_text());assert freeze['frozen'] and freeze['native_sha256']==PIN
    for path,want in freeze['proof_hashes'].items():assert sha(ROOT/path)==want,path
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass4_verify.exe','restore_map_refresh_postpass4_plain_verify.exe']*2
    assert all('map_refresh_postpass4_phase/build' not in p.as_posix() for p in executables)
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected={HERE/'postpass4_fixture.c',ROOT/'src/unit_01149.c',ROOT/'src/unit_01152.c',ROOT/'src/unit_01153.c'}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed') and ('MAP_REFRESH_POSTPASS4_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={'_FUN_0047e5c0':'unit_01149.obj','_FUN_0047e870':'unit_01152.obj','_FUN_0047eb60':'unit_01153.obj'}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={};returns={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':26}
    for i,p in enumerate(cases):
        expected=native(p);counts[categories[i]]=counts.get(categories[i],0)+1;routes[p[0]]+=1
        for k,proc in enumerate(procs):
            proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
            if k%2:actual['events']=[];want=dict(expected,events=[])
            else:want=expected
            if actual!=want:
                ae=actual['events'];ee=want['events'];mismatch=next((j for j in range(min(len(ae),len(ee))) if ae[j]!=ee[j]),min(len(ae),len(ee)))
                raise AssertionError(json.dumps({'case':i,'p':p,'variant':str(executables[k]),'event':mismatch,'actual_events':ae[max(0,mismatch-2):mismatch+3],'expected_events':ee[max(0,mismatch-2):mismatch+3],'actual_other':{a:v for a,v in actual.items() if a!='events'},'expected_other':{a:v for a,v in want.items() if a!='events'}}))
        if (i+1)%512==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(instructions[a]==name for a in hits),len(f['assembly']['items'])] for name,f in facts['functions'].items()}
if not args.limit:assert coverage=={'0047e5c0':[101,101],'0047eb60':[85,85],'0047e870':[198,198]},coverage
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'branches':branches,'counter_changes':counter_changes,'categories':counts,'routes':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete3function closure with real47E870 calling real47EB60; no internal algorithms controlled or overridden. Does not close47B590/47B670/475CC0 or full488E90.','All new-body nonstack reads/stores compare in order, with full8guardregions. Counter pointer is actual caller localstack or disjoint guardedDWORD; counter memory operation excluded from ordered trace and final rawDWORD/guards compare, including wrap. Counter aliasing with other inputs/context is outside domain.','WORD record ids sign-extend then multiply264; WORD type/orientation and BYTE record+154 compare. Negative/high ids are explicitly mapped API extensions, not claims of ordinary game object validity. Direction0/2/4/6 ordinary; odd1/3/5/7 shares truncated half-row and writes its actual odd slot as mapped API extension.','Void EAX is not a fabricated result. GPRs/SP/stack sentinel/older2x87 values/tagdepth/CW preserved. No algorithm FPU instructions; general floating exception status/traps are not established.','Private O2 uses actualDebug restore_data/staticDebugCRT. Shared recipe inherits rootDebugMTd/ReleaseMT, /Od and/O2 respectively.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass4-partial.json' if args.limit else 'map-refresh-postpass4-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','counter_changes','elapsed_seconds']}))
