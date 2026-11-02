"""Complete4877F0 tagging/cleanup and its exact real recursive dependency closure."""
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
facts_path=HERE/'postpass7-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and sha(ROOT/facts['parent_dossier']['path'])==facts['parent_dossier']['sha256']
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
hits=set();events=[];running=False;routes={i:0 for i in range(6)};helper_returns=[];frames=[];max_depth=0;recursion_calls=0;clears=0;direct_clears=0;tag_stores={'set':0,'clear':0};quiet={'0047db00'}
def helper(a,phase,arg):events.append([3,a,phase,arg,read(0xbc9768),*[read(0xbc97c0+i*4) for i in range(8)]])
def phase(tag):
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    events.append([5,tag,*struct.unpack('<8I',h.digest())])
def code(cpu,a,n,user):
    global max_depth,recursion_calls
    if helper_returns and a==helper_returns[-1][0]:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    if a==STOP:return
    assert a in instructions,hex(a);hits.add(a)
    if a==0x47db00:
        sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4);helper(a,0,arg);helper_returns.append((read(sp),a,arg))
    elif a==0x487929:phase(1)
    elif a in [0x487b4b,0x487b5d]:phase(2)
    elif a==0x487b70:
        sp=cpu.reg_read(UC_X86_REG_ESP);x,y=read(sp+4),read(sp+8);events.append([4,0,x,y,len(frames)]);frames.append((x,y));max_depth=max(max_depth,len(frames));recursion_calls+=1
    elif a==0x487cf6:
        x,y=frames.pop();events.append([4,1,x,y,len(frames)])
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([0,a,n,read(a,n),0])
def memwrite(cpu,access,a,n,value,user):
    global clears,direct_clears
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([1,a,n,value&((1<<(n*8))-1),0])
        if cpu.reg_read(UC_X86_REG_EIP)==0x4878e2:tag_stores['set' if value&0x4000000 else 'clear']+=1
        elif cpu.reg_read(UC_X86_REG_EIP)==0x487aaa:assert read(a)&0x4000000 and value==read(a)&0xfbffffff;direct_clears+=1
        elif cpu.reg_read(UC_X86_REG_EIP)==0x487cad:assert read(a)&0x4000000 and value==read(a)&0xfbffffff;clears+=1
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def prepare(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[12]+i*37)&255])*n)
    vm.mem_write(0xaf518c,struct.pack('<I',p[7])*53008);base=((((228-p[2])&MASK)//2)*228+((228-p[1])&MASK)//2)&MASK
    for a,value in [(0xbca2d0,p[1]),(0xbca2d4,p[2]),(0xbca2d8,base),(0xbca2dc,228-p[1]),(0xbc9768,p[17])]:put(a,value)
    for i in range(8):put(0xbc97c0+i*4,p[18]^(i*0x11335577&MASK))
    def field(x,y,value):
        tile=signed((base+y*228+x)&MASK)
        if -512<=tile<52496:put(0xaf598c+tile*4,value)
    x,y=signed(p[14]),signed(p[15]);w,h=p[10],p[11];states=[0,4,0x80004,0x10004]
    if p[9]==0:
        for dy in range(-1,2):
            for dx in range(-1,2):
                bit=(dy+1)*3+dx+1;state=(p[19]>>(bit*2))&3;field(x+dx,y+dy,p[7]|(states[state] if p[19] else p[16] if p[8]>>bit&1 else 0))
    else:
        for dy in range(h):
            for dx in range(w):field(x+dx-w//2,y+dy-h//2,p[7]|(p[16] if p[9]==1 or p[9]==2 and (dx+dy)&1 or p[9]==3 and (dx==w//2 or dy==h//2) else 0))

def native(p):
    global events,running,helper_returns,frames
    prepare(p);sp=STACK+0xf0000;put(sp,STOP)
    argv=[p[3],p[4],p[5],p[6]]
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[13]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];helper_returns=[];frames=[];running=True
    try:vm.emu_start(0x4877f0,STOP,count=20000000)
    finally:running=False
    if helper_returns and helper_returns[-1][0]==STOP and vm.reg_read(UC_X86_REG_EIP)==STOP:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    assert not frames and not helper_returns and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[13] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'result':0,'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
default=[0,16,16,7,7,9,9,0,0x155,0,3,3,17,0x37f,8,8,4,0xdeadbeef,0x80000000,0];cases=[];categories=[]
def add(category='ordinary',**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    assert len(p)==20
    cases.append(p);categories.append(category)
for basebits,mask in [(0,4),(4,0x10000),(4,0x80000),(0,0x4000000)]:
    for pattern in range(512):add('independent_9cell_patterns',p7=basebits,p16=mask,p8=pattern)
rng=random.Random(0x4877f0)
for i in range(256):
    packed=rng.randrange(1<<18)
    for basebits in [0,0x4000000]:add('mixed_terrain_states',p7=basebits,p19=packed,p8=0)
for kind,(w,h),basebits in itertools.product([1,2,3],[(1,15),(15,1),(3,11),(11,3),(7,7),(15,15)],[0,4]):add('composed_shapes',p1=16,p2=16,p3=0,p4=0,p5=15,p6=15,p7=basebits,p9=kind,p10=w,p11=h,p16=0x4000000 if basebits==0 else 4)
for (w,h) in [(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)]:
    for x,y in [(0,0),(max(w-1,0),max(h-1,0)),(w//2,h//2)]:
        for rectangle in [(-1,-1,1,1),(x,y,x,y),(9,9,8,8),(w+1,h+1,w+3,h+3),(0,0,w,h)]:add('geometry_and_clipping',p1=w,p2=h,p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3],p14=x,p15=y,p8=0x1ff)
for marker,pattern in itertools.product([0,1,0x100,0x7fffffff,0x80000000,MASK],[0,0x155,0x1ff]):add('saved_tile_cache',p17=marker,p8=pattern,p7=4,p16=0x10000)
for raw,rectangle in itertools.product([MASK,0xffffff80],[(-1,-1,1,1),(0,0,0,0),(1,1,2,2)]):add('signed_DWORD_bound_extension',p1=raw,p2=raw,p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3])
for rectangle in [(0x7fffffff,0,0,0),(0,0x7fffffff,0,0),(0,0,-0x80000000,0),(0,0,0,-0x80000000)]:add('empty_raw_coordinate_extension',p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3])
for cw in [0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]:add('CW_preservation',p13=cw,p7=4,p16=0x10000,p8=0)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post7_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
P6=ROOT/'artifacts/map_refresh_postpass6_phase';predecessor=P6/'postpass6_apply_map.json';assert sha(predecessor)=='f4ad6b4aabc2cf0dbea9a7eca6faa5279e9760d459d697c7f05ec4d20d50a9cd'
predecessor_data=json.loads(predecessor.read_text());mappings=[(HERE/'unit_01267.c',ROOT/'src/unit_01267.c'),(HERE/'map_refresh_postpass7.h',ROOT/'include/map_refresh_postpass7.h')]+[(ROOT/m['staged'],ROOT/m['target']) for m in predecessor_data['mappings']]
for m in predecessor_data['mappings']:assert sha(ROOT/m['staged'])==m['staged_sha256'],m['staged']
root_c=[ROOT/name for name in facts['unchanged_actual_C']]
ledger_path=HERE/'map-refresh-dependency-ledger.json';ledger=json.loads(ledger_path.read_text());assert ledger['native_sha256']==PIN and len(ledger['entries'])==38
ledger_evidence=dict(ledger['proof_artifact_hashes'])
def ledger_selected(value):
    if not isinstance(value,dict):return
    if 'path' in value and 'sha256' in value:ledger_evidence[value['path']]=value['sha256']
    for child in value.values():ledger_selected(child)
for entry in ledger['entries'].values():ledger_selected(entry.get('selected_C'))
for path,want in ledger_evidence.items():assert sha(ROOT/path)==want,path

files=[HERE/'postpass7_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),facts_path,predecessor,ledger_path,HERE/'probe_rejected_intmin_bounds.py',HERE/'rejected-intmin-bound-probe.json',*root_c]+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    freeze_path=HERE/'postpass7_apply_map.json'
    if args.manifest_sha256:assert sha(freeze_path)==args.manifest_sha256
    freeze=json.loads(freeze_path.read_text());assert freeze['frozen'] and freeze['native_sha256']==PIN
    for path,want in freeze['proof_hashes'].items():assert sha(ROOT/path)==want,path
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass7_verify.exe','restore_map_refresh_postpass7_plain_verify.exe']*2
    assert all('map_refresh_postpass7_phase/build' not in p.as_posix() for p in executables)
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected={HERE/'postpass7_fixture.c',*root_c,*[target for staged,target in mappings if target.suffix=='.c']}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed') and ('MAP_REFRESH_POSTPASS7_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={f'_FUN_{name}':f'unit_{n:05d}.obj' for name,n in [('004877f0',1267),('004816b0',1194),('00481770',1195),('00487730',1265),('00487790',1266),('00487b70',1268),('0047db00',1137)]}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={};returns={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':20}
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
if not args.limit:assert coverage['004877f0']==[281,281],coverage
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'whole_4877f0_complete_within_domain':True,'whole_475cc0_or_47a220_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'separate_rejected_invalid_domain_probes':1,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'recursion_calls':recursion_calls,'max_recursion_depth':max_depth,'recursive_clears':clears,'direct_clears':direct_clears,'tagpass_stores':tag_stores,'categories':counts,'routes':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(facts_path):sha(facts_path)},'dependency_ledger':{'path':rel(ledger_path),'sha256':sha(ledger_path),'scope':"Scoping/provenance only; other ledger function sources are not additional compiled/proved functions in this target",'evidence_hashes':ledger_evidence,'evidence_drift':[path for path,want in ledger_evidence.items() if sha(ROOT/path)!=want]},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Whole4877F0 with exact frozen postpass6 algorithms and actual unchangedgather; no internal fake boundary or legacy root helper. Does not close47A220/475CC0/full488E90.','Ordered new/frozen reads/stores and actual recursive entry/exit depth compare. Actual gather internals excluded with full8DWORD+tile snapshots. Both complete guardregions compare. Tagpass and cleanup pre-restore SHA256 snapshots compare, then originalBC9768 restore and final wholememory compare.','Native caches width between defined reloads, scans inclusive upper width/height in cleanup crops and reuses consumed argument slots. Correct C locals and unsigned32 arithmetic preserve those contracts; discarded stackbytes and unconsumed voidEAX are not fabricated outputs.','All24 exception-maskedCW, old2x87 values/tagdepth/GPRs/SP/sentinel compare. No algorithmFPU; generalFPstatus/traps and arbitrary-world recursion-stack bound remain unproved. Numeric/globalST0 conversion prerequisites for488E90 retain their separate prior qualifications.','PrivateO2 uses actualDebugdata/staticDebugCRT; productionrecipe rootDebugMTd/ReleaseMT. Frozen map/numeric phases remain staged and are not presumed productapplied.']}
assert not report['source_drift'] and not report['binary_drift'] and not report['dependency_ledger']['evidence_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass7-partial.json' if args.limit else 'map-refresh-postpass7-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','recursion_calls','max_recursion_depth','direct_clears','elapsed_seconds']}))
