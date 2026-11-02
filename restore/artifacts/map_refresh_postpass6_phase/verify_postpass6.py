"""Whole bounded radius/axis predicates and real recursive map cleanup."""
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
facts_path=HERE/'postpass6-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and sha(ROOT/facts['parent_dossier']['path'])==facts['parent_dossier']['sha256']
instructions={}
for name,want in facts['unchanged_actual_C'].items():assert sha(ROOT/name)==want,name
for name,f in facts['functions'].items():
    assert not f['assembly']['has_more']
    for item in f['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
regions=[(0xaf517c,0x33c60),(0xbc9730,0xc00)];STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();events=[];running=False;routes={i:0 for i in range(6)};helper_returns=[];frames=[];max_depth=0;recursion_calls=0;clears=0;quiet={'0047db00'}
def helper(a,phase,arg):events.append([3,a,phase,arg,read(0xbc9768),*[read(0xbc97c0+i*4) for i in range(8)]])
def code(cpu,a,n,user):
    global max_depth,recursion_calls
    if helper_returns and a==helper_returns[-1][0]:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    if a==STOP:return
    assert a in instructions,hex(a);hits.add(a)
    if a==0x47db00:
        sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4);helper(a,0,arg);helper_returns.append((read(sp),a,arg))
    elif a==0x487b70:
        sp=cpu.reg_read(UC_X86_REG_ESP);x,y=read(sp+4),read(sp+8);events.append([4,0,x,y,len(frames)]);frames.append((x,y));max_depth=max(max_depth,len(frames));recursion_calls+=1
    elif a==0x487cf6:
        x,y=frames.pop();events.append([4,1,x,y,len(frames)])
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([0,a,n,read(a,n),0])
def memwrite(cpu,access,a,n,value,user):
    global clears
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([1,a,n,value&((1<<(n*8))-1),0])
        if cpu.reg_read(UC_X86_REG_EIP)==0x487cad:assert read(a)&0x4000000 and value==read(a)&0xfbffffff;clears+=1
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def prepare(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[12]+i*37)&255])*n)
    vm.mem_write(0xaf518c,struct.pack('<I',p[7])*53008);base=((((228-p[2])&MASK)//2)*228+((228-p[1])&MASK)//2)&MASK
    for a,value in [(0xbca2d0,p[1]),(0xbca2d4,p[2]),(0xbca2d8,base),(0xbca2dc,228-p[1]),(0xbc9768,base+p[4]*228+p[3])]:put(a,value)
    for i in range(8):put(0xbc97c0+i*4,p[15]^(i*0x11335577&MASK))
    def field(x,y,value):
        tile=signed((base+y*228+x)&MASK)
        if -512<=tile<52496:put(0xaf598c+tile*4,value)
    x,y=signed(p[3]),signed(p[4]);w,h=p[10],p[11]
    if p[9]==0:
        for dy in range(-1,2):
            for dx in range(-1,2):field(x+dx,y+dy,p[7]|(p[6] if p[8]>>((dy+1)*3+dx+1)&1 else 0))
    else:
        for dy in range(h):
            for dx in range(w):field(x+dx-w//2,y+dy-h//2,p[7]|(p[6] if p[9]==1 or p[9]==2 and (dx+dy)&1 or p[9]==3 and (dx==w//2 or dy==h//2) else 0))

def native(p):
    global events,running,helper_returns,frames
    prepare(p);sp=STACK+0xf0000;put(sp,STOP)
    argv=[p[3],p[4],p[5],p[6]] if p[0]<2 else [p[3],p[4],p[6]] if p[0]<4 else [p[3],p[4]] if p[0]==4 else [p[6]]
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[13]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];helper_returns=[];frames=[];running=True
    try:vm.emu_start([0x4816b0,0x481770,0x487730,0x487790,0x487b70,0x47db00][p[0]],STOP,count=10000000)
    finally:running=False
    if helper_returns and helper_returns[-1][0]==STOP and vm.reg_read(UC_X86_REG_EIP)==STOP:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    assert not frames and not helper_returns and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert read(sp+4+len(argv)*4)==0xbaadf00d
    if p[0] in [2,3,5]:assert [read(sp+4+i*4) for i in range(len(argv))]==argv
    assert vm.reg_read(UC_X86_REG_FPCW)==p[13] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0]<4 else 0,'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
default=[0,16,16,8,8,2,4,0,0x155,0,3,3,17,0x37f,0,0x80000000];cases=[];categories=[]
def add(category='ordinary',**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    assert len(p)==16
    cases.append(p);categories.append(category)
for pattern in range(512):add('recursive_9cells',p0=4,p6=0x4000000,p8=pattern)
for route,pattern in itertools.product(range(4),range(512)):add('leaf_9cells',p0=route,p8=pattern)
# Bounded radius/coordinate extensions; no imposed coordinate clamp beyond original per-cell bounds.
for route,(w,h),radius in itertools.product([0,1],[(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)],[-2,-1,0,1,2,3,8]):
    for x,y in [(-1,-1),(0,0),(w//2,h//2),(max(w-1,0),max(h-1,0)),(w,h),(w+1,h+1)]:
        for pattern in [0,0x155,0x1ff]:add('radius_bounds',p0=route,p1=w,p2=h,p3=x,p4=y,p5=radius,p8=pattern)
for route,(w,h) in itertools.product([2,3,4],[(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)]):
    for x,y in [(-1,-1),(0,0),(w//2,h//2),(max(w-1,0),max(h-1,0)),(w,h)]:
        for pattern in [0,0x155,0x1ff]:add('axis_and_inclusive_crop',p0=route,p1=w,p2=h,p3=x,p4=y,p6=0x4000000,p8=pattern)
for kind,(w,h),bits in itertools.product([1,2,3],[(1,17),(17,1),(3,19),(19,3),(7,7),(16,16),(31,31)],[0,0x80000000,0xfbffffff]):add('recursive_shapes',p0=4,p1=32,p2=32,p3=16,p4=16,p6=0x4000000,p7=bits,p9=kind,p10=w,p11=h)
for route,mask,bits in itertools.product(range(6),[0,4,0x10000,0x80000000,MASK],[0,0x80000000,0xfbffffff]):add('mask_and_preserved_bits',p0=route,p6=0x4000000 if route==4 else mask,p7=bits,p8=0x1ff)
for route,cw in itertools.product(range(6),[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]):add('CW_preservation',p0=route,p13=cw,p6=0x4000000,p8=0x1ff)
for route,bound,coordinate in itertools.product(range(4),[0xffffffff,0xffffff80,0x80000000],[-1,0,1]):add('signed_DWORD_bound_extension',p0=route,p1=bound,p2=bound,p3=coordinate,p4=coordinate,p8=0x1ff)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post6_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
mappings=[(HERE/f'unit_{n:05d}.c',ROOT/'src'/f'unit_{n:05d}.c') for n in [1194,1195,1265,1266,1268]]+[(HERE/'map_refresh_postpass6.h',ROOT/'include/map_refresh_postpass6.h')]
root_c=[ROOT/name for name in facts['unchanged_actual_C']]
files=[HERE/'postpass6_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),facts_path,*root_c]+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    freeze_path=HERE/'postpass6_apply_map.json'
    if args.manifest_sha256:assert sha(freeze_path)==args.manifest_sha256
    freeze=json.loads(freeze_path.read_text());assert freeze['frozen'] and freeze['native_sha256']==PIN
    for path,want in freeze['proof_hashes'].items():assert sha(ROOT/path)==want,path
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass6_verify.exe','restore_map_refresh_postpass6_plain_verify.exe']*2
    assert all('map_refresh_postpass6_phase/build' not in p.as_posix() for p in executables)
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected={HERE/'postpass6_fixture.c',*root_c,*[target for staged,target in mappings if target.suffix=='.c']}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed') and ('MAP_REFRESH_POSTPASS6_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={f'_FUN_{name}':f'unit_{n:05d}.obj' for name,n in [('004816b0',1194),('00481770',1195),('00487730',1265),('00487790',1266),('00487b70',1268),('0047db00',1137)]}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={};returns={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':16}
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
if not args.limit:assert all(v[0]==v[1] for v in coverage.values()),coverage
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'whole_475cc0_or_47a220_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'recursion_calls':recursion_calls,'max_recursion_depth':max_depth,'monotonic_clears':clears,'categories':counts,'routes':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete5function closure including actual recursion487B70 and unchanged47DB00; no controlled internal algorithms, fake clear/postmutation or legacy helper substitution. 4877F0/47A220/475CC0/full488E90 are not closed.','Ordered all new-body reads/stores and original-argument recursion entry/exit depth compare; actual gather internals excluded with actualentry/exit tile+8DWORD snapshots. Both whole guardregions compare in trace/plain.','FullEAX0/1 compares for four scalar leaves. Axis helpers only validate their own axis; inclusive recursive crop can process width/height padding cells. Two actualint32 coordinate arguments replace legacy pointer-firstarg in1268.','Native radius functions overwrite consumed by-value argument slots and recursive487B70 reuses incomingY slot; actual caller discards them. Callee-saved GPRs/SP/sentinel and meaningful state compare, not discarded stackbytes.','No algorithmFPU; older2x87values/tagdepth/CW preserved for all24 exception-masked PC/RC/IC combinations. GeneralFPstatus/traps unproved. Bounded prepared shapes demonstrate recursion; no exhaustive proof over all51984cell active patterns or unbounded Cstack depth.','PrivateO2 uses actualDebugdata/staticDebugCRT; productionrecipe rootDebugMTd/ReleaseMT. Earlier frozen stages remain unchanged and are not presumed productapplied.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass6-partial.json' if args.limit else 'map-refresh-postpass6-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','recursion_calls','max_recursion_depth','monotonic_clears','elapsed_seconds']}))
