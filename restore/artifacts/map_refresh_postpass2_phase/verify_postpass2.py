"""Whole record-selector leaves and unchanged DWORD inversion against complete native bodies."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes();u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=HERE/'postpass2-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and sha(HERE/'selector_native_data.h')==facts['header_sha256']
for name,want in facts['unchanged_actual_C_source'].items():assert sha(ROOT/name)==want,name
instructions={}
for name,f in facts['functions'].items():
    assert not f['assembly']['has_more']
    for item in f['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
TABLE=0x6f2000;regions=[(TABLE-16,0x1820),(0xbc9730,0xc0),(0xbca2d0,0x40)];STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();events=[];running=False;match_groups={};modulo_divisors=set();counter_stores={'increment':0,'reset':0}
def code(cpu,a,n,user):
    if a==STOP:return
    assert a in instructions,hex(a);hits.add(a)
    if a==0x484e24:
        group=cpu.reg_read(UC_X86_REG_EAX);match_groups[group]=match_groups.get(group,0)+1
    if a in [0x484c7d,0x484d2a]:modulo_divisors.add(cpu.reg_read(UC_X86_REG_ECX))
    if a in [0x484c96,0x484d43,0x484eab]:counter_stores['increment']+=1
    if a in [0x484c9d,0x484d4a,0x484eb2]:counter_stores['reset']+=1
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)]!='00484f00':events.append([0,a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)]!='00484f00':events.append([1,a,n,value&((1<<(n*8))-1)])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def context(p,i):return ((p[12]>>i)&1)|(((p[13]>>i)&1)<<1)|(p[14]&0xfffffffc)
def truth(p,i):return context(p,i)&1 if p[0]==1 else int(context(p,i)!=0)
def prepare(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[15]+i*37)&255])*n)
    for i in range(8):put(0xbc97c0+i*4,context(p,i))
    put(0xbca2e0,p[5])
    if p[1]:
        data=facts['record_inputs']['post2_main_records' if p[1]==1 else 'post2_secondary_records'];blob=bytearray.fromhex(data['bytes'])
        for i in range(len(blob)//16):blob[i*16+14]=p[9]&255
        vm.mem_write(TABLE,bytes(blob))
    else:
        count=max(0,signed(p[2]));assert count<=48
        for group in range(8):
            for row in range(count):
                record=bytearray(16)
                for i in range(8):
                    if p[7]==0 or (p[7]==2 and i%3==0):record[i]=2
                    else:record[i]=(128 if p[7]==3 else 1) if truth(p,i) else 0
                if row!=p[6]:i=row%8;record[i]=0 if truth(p,i) else 1
                for j in range(4):record[8+j]=(p[10]+group*17+row*3+j*37)&255
                record[12]=(p[11]+group*11+row)&255;record[13]=p[8]&255;record[14]=p[9]&255;record[15]=(0xa5^group^row)&255;vm.mem_write(TABLE+(group*count+row)*16,bytes(record))
def native(p):
    global events,running
    prepare(p);sp=STACK+0xf0000;put(sp,STOP)
    argv=[TABLE,p[2],p[3],p[4]] if p[0]==0 else ([TABLE,p[2]] if p[0]==1 else [])
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];running=True
    try:vm.emu_start([0x484be0,0x484d70,0x484f00][p[0]],STOP,count=10000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert [read(sp+4+i*4) for i in range(len(argv))]==argv and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==0x37f and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'result':0 if p[0]==2 else vm.reg_read(UC_X86_REG_EAX),'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
default=[0,0,4,0xffffffff,0,0,0,1,6,0,17,128,0x55,0,0,17];cases=[];categories=[]
def add(category='synthetic',**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(p);categories.append(category)
# Actual native479E70 main records, observed orientation domain and both scan contracts.
for mask,orientation,reverse,animation in itertools.product(range(256),[0,2,4,6],[0,1],[-1,0,1]):add('real_main',p1=1,p2=24,p3=animation,p4=reverse,p5=orientation,p12=mask)
for mask,reverse,animation in itertools.product(range(256),[0,1],[-1,0,1]):add('real_secondary',p1=2,p2=26,p3=animation,p4=reverse,p12=mask)
# Lowbit matching and independent group flags exhaust their256 masks separately.
for mask,orientation in itertools.product(range(256),[0,2,4,6]):
    add('rotation_truth_sweep',p0=1,p1=1,p2=24,p5=orientation,p12=mask,p13=0x55)
    add('rotation_flag_sweep',p0=1,p1=1,p2=24,p5=orientation,p12=0x55,p13=mask)
# FullDWORD zero truth, including words whose lowBYTE is0.
for mask,high in itertools.product(range(256),[0,0x100,0x80000000,0xfffffffc]):add('unchanged_invert',p0=2,p12=mask,p13=0,p14=high)
for mask in range(256):add('unchanged_invert_flags',p0=2,p12=0,p13=mask)
for reverse,animation,frames,oldframe in itertools.product([0,1],[-0x80000000,-255,-7,-1,0,1,7,255,0x7fffffff],[1,2,6,255],[0,1,254,255]):add(p3=animation,p4=reverse,p8=frames,p9=oldframe,p6=2)
# Zero-frame cycle is legal; selected modulo with zero frames is excluded(IDIV fault).
for reverse,oldframe in itertools.product([0,1],[0,1,254,255]):add(p3=0 if reverse else -1,p4=reverse,p8=0,p9=oldframe)
for route,count,orientation in itertools.product([0,1],[0,-1,-2],[0,1,2,3,4,5,6,7,-1,-2,-3,8]):add('empty_or_mapped_orientation',p0=route,p2=count,p5=orientation)
for route,match,pattern,orientation,high in itertools.product([0,1],[0,1,7,8],[0,1,2,3],[0,2,4,6],[0,0x100,0x80000000]):add(p0=route,p2=8,p6=match,p7=pattern,p5=orientation,p14=high,p13=0x41)
for reverse in [0,1,2,128,255,256,257,MASK]:add(p4=reverse,p3=0,p9=254)
for flagmask in range(256):add(p0=1,p2=48,p6=47,p13=flagmask,p9=255,p8=3)
for orientation in [-3,-2,-1,1,3,5,7,8]:add('mapped_orientation',p5=orientation,p7=0)
rng=random.Random(0x484be0)
for i in range(256):
    route=rng.randrange(2);reverse=rng.randrange(2);animation=rng.choice([-1,0,1,7]);frames=rng.choice([1,2,3,6,255]);count=rng.randrange(1,9)
    add('mixed',p0=route,p2=count,p3=animation,p4=reverse,p5=rng.choice([0,2,4,6]),p6=rng.randrange(count+1),p7=rng.randrange(4),p8=frames,p9=rng.randrange(256),p10=rng.randrange(256),p11=rng.randrange(256),p12=rng.randrange(256),p13=rng.randrange(256),p14=rng.choice([0,0x100,0x80000000]),p15=rng.getrandbits(32))
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post2_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
mappings=[(HERE/'unit_01233.c',ROOT/'src/unit_01233.c'),(HERE/'unit_01234.c',ROOT/'src/unit_01234.c'),(HERE/'map_refresh_postpass2.h',ROOT/'include/map_refresh_postpass2.h')]
files=[HERE/'postpass2_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),facts_path,HERE/'selector_native_data.h',ROOT/'src/unit_01236.c']+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass2_verify.exe','restore_map_refresh_postpass2_plain_verify.exe']*2
    assert all('map_refresh_postpass2_phase/build' not in p.as_posix() for p in executables)
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected={HERE/'postpass2_fixture.c',ROOT/'src/unit_01233.c',ROOT/'src/unit_01234.c',ROOT/'src/unit_01236.c'}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed') and ('MAP_REFRESH_POSTPASS2_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={'_FUN_00484be0':'unit_01233.obj','_FUN_00484d70':'unit_01234.obj','_FUN_00484f00':'unit_01236.obj'}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={};returns={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':16}
    for i,p in enumerate(cases):
        expected=native(p);counts[categories[i]]=counts.get(categories[i],0)+1;returns[expected['result']]=returns.get(expected['result'],0)+1
        for k,proc in enumerate(procs):
            proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
            if k%2:actual['events']=[];want=dict(expected,events=[])
            else:want=expected
            if actual!=want:
                ae=actual['events'];ee=want['events'];mismatch=next((j for j in range(min(len(ae),len(ee))) if ae[j]!=ee[j]),min(len(ae),len(ee)))
                raise AssertionError(json.dumps({'case':i,'p':p,'variant':str(executables[k]),'event':mismatch,'actual_events':ae[max(0,mismatch-2):mismatch+3],'expected_events':ee[max(0,mismatch-2):mismatch+3],'actual_other':{a:v for a,v in actual.items() if a!='events'},'expected_other':{a:v for a,v in want.items() if a!='events'}}))
        if (i+1)%2048==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(instructions[a]==name for a in hits),sum(v==name for v in instructions.values())] for name in facts['functions']}
if not args.limit:assert all(a==b for a,b in coverage.values()) and sum(b for a,b in coverage.values())==289
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'whole_4777c0_479e70_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'categories':counts,'return_histogram':returns,'rotation_groups':match_groups,'modulo_divisors':sorted(modulo_divisors),'counter_store_counts':counter_stores,'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Two complete handwritten selectors plus actual unchanged root484F00; no callees, imports, fake internal algorithms or mapneighbor prerequisites.484F00 is already correct address-only BYTEbase cast to DWORDpointer; its C4133 warning is retained rather than inventing a width repair.','New selector external reads/transient writes are ordered. Unchanged inversion internal order is not C-traced; all8DWORD effects/fullguardstate are compared. Its voidEAX is ignored; result0 on that route is a fixture placeholder, not a native return claim.','Real PE main192records/count24 and secondary26records plus valid prepared16byte records. Ordinary orientations0/2/4/6; small negative/odd/8 orientations and extra signed/count/frame states are explicitly mapped ABI probes.','Selected modulo requires frameBYTE1..255; zero-frame cycle and empty/no-match are legal. Native IDIV0 fault behavior and C undefined divisionby0 are outside this proof, with no invented fallback/clamp.','No floating point arithmetic. Old x87 values/TOP/CW and C savedGPRs/SP checked; no generalized FPSW/trap claim. Private O2 links Debugdata/staticDebugCRT; shared recipe inherits rootDebugMTd/ReleaseMT.4777C0/479E70 and whole488E90/489030 remain unfinished.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass2-partial.json' if args.limit else 'map-refresh-postpass2-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','elapsed_seconds']}))
