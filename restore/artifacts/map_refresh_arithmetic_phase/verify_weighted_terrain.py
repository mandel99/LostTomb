"""Complete481540/481690, actual481AA0 and explicit raw80 conversion, ordered arithmetic/state."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args()
MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=ROOT/'artifacts/map_refresh_phase/map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
scope=['00481540','00481690','00481aa0','0056133c'];instructions={};decoded={};decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for name in scope:
    f=facts['functions'][name]
    for item in f['assembly']['items']:
        address=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(address,len(b)))==b;instructions[address]=name
        sub=list(decoder.disasm(b,address));assert b''.join(bytes(i.bytes) for i in sub)==b
        for i in sub:decoded[i.address]=address
fixture_facts_path=HERE/'native-ring-fixtures.json';fixture_facts=json.loads(fixture_facts_path.read_text());assert fixture_facts['native_sha256']==PIN and sha(HERE/'map_ring_fixture_data.h')==fixture_facts['fixture_header_sha256']
for f in fixture_facts['functions'].values():
    for item in f['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
regions=[(0xaf558c,0x33440),(0x70ba9c,0x500),(0x70c8dc,0x500),(0x5c9af0,0x90),(0x56f460,0x150),(0xbc9758,0x30),(0xbca2c0,0x30)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();decoded_hits=set();events=[];running=False;pending=None;branch_counts={'point_rejected':0,'point_accepted':0,'clamp_score':0,'keep_score':0};operation_counts={}
fp_sites={0x481595,0x48159f,0x4815c9,0x4815fb,0x481602,0x48161a,0x48161e,0x481624,0x481628,0x481657,0x481670,0x481674,0x481678}
spill_sites={0x4815cf,0x481612,0x48162c}
def fp_event(a):
    top=(vm.reg_read(UC_X86_REG_FPSW)>>11)&7;sig,exponent=vm.reg_read(UC_X86_REG_FP0+top);events.append([2,a,sig&MASK,sig>>32,exponent]);operation_counts[f'{a:08x}']=operation_counts.get(f'{a:08x}',0)+1
def code(cpu,a,n,user):
    global pending
    if pending:
        kind,address=pending
        if kind=='fp':fp_event(address)
        else:events.append([4,address,(cpu.reg_read(UC_X86_REG_FPSW)>>8)&1])
        pending=None
    if a==STOP:return
    assert a in decoded,f'escaped original instruction{a:08x}';hits.add(decoded[a]);decoded_hits.add(a)
    if a in fp_sites:pending=('fp',a)
    elif a==0x48165b:pending=('compare',a)
    elif a==0x56133c:fp_event(a)
    if a==0x4815ef:branch_counts['point_rejected' if cpu.reg_read(UC_X86_REG_EAX) else 'point_accepted']+=1
    if a==0x481666:branch_counts['clamp_score' if cpu.reg_read(UC_X86_REG_FPSW)&0x100 else 'keep_score']+=1
def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        v=read(a,n);events.append([0,a,n,v&MASK,v>>32])
def memwrite(cpu,access,a,n,value,user):
    if not running:return
    ip=cpu.reg_read(UC_X86_REG_EIP);v=value&((1<<(n*8))-1)
    if STACK<=a and a+n<=STACK+0x100000:
        if ip in spill_sites:assert n==8;events.append([3,ip,v&MASK,v>>32])
    else:
        assert a==0xbc9768 and n==4,('unexpected original write',hex(ip),hex(a),n)
        events.append([1,a,n,v&MASK,v>>32])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def prepare(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[9]+i*37)&255])*n)
    for data in fixture_facts['ranges'].values():vm.mem_write(data['address'],bytes.fromhex(data['bytes']))
    put(0x56f470,0x4059000000000000,8);put(0x56f520,0x3ff0000000000000,8);put(0x56f5a0,0,8)
    put(0xbca2d0,p[5]);put(0xbca2d4,p[6]);put(0xbca2d8,p[7])
    for dy in range(-4,5):
        for dx in range(-4,5):
            nx=signed(p[1])+dx;ny=signed(p[2])+dy
            if nx< -1 or ny< -1 or nx>p[5] or ny>p[6]:continue
            index=(p[7]+nx+ny*228)&MASK;h=((index^((p[9]*0x9e3779b9)&MASK))*1664525+1013904223)&MASK;flags=h&~0x800
            enabled=[False,True,(nx+ny)&1,nx&1,h&0x10000,(h>>16)%100<p[11]][p[8]]
            if enabled:flags|=0x800
            a=(0xaf598c+index*4)&MASK;assert regions[0][0]<=a and a+4<=sum(regions[0]);put(a,flags)
def native(p):
    global events,running,pending
    prepare(p);sp=STACK+0xf0000;put(sp,STOP);argv=[p[1],p[2],p[3]]+([] if p[0] else [p[4]])
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[10]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0x0fff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];pending=None;running=True
    try:vm.emu_start(0x481690 if p[0] else 0x481540,STOP,count=20000)
    finally:running=False
    assert pending is None and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    actual_args=[read(sp+4+i*4) for i in range(len(argv))];want_args=argv.copy()
    if not p[0] and p[3]:want_args[2]=0 # Native callee mutates its consumed by-value argument slot; caller discards it.
    assert actual_args==want_args and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[10] and (vm.reg_read(UC_X86_REG_FPSW)&0x3800)==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'result':vm.reg_read(UC_X86_REG_EAX),'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])];cases=[]
def add(route=1,x=8,y=8,rings=4,mask=0x800,w=16,h=16,pattern=4,seed=17,cw=0x37f,threshold=50):cases.append([route,x,y,rings,mask,w,h,((228-h)//2)*228+(228-w)//2,pattern,seed,cw,threshold])
geometry=[(16,16,0,0),(16,16,15,15),(16,16,8,8),(228,228,0,0),(228,228,227,227),(1,1,0,0),(0,0,0,0),(7,19,3,9),(224,220,100,100)]
for cw,(w,h,x,y),pattern in itertools.product(cws,geometry,range(6)):add(x=x,y=y,w=w,h=h,pattern=pattern,cw=cw)
for cw,rings,mask in itertools.product(cws,range(5),[0,1,0x800,0x80000000,MASK]):add(route=0,rings=rings,mask=mask,cw=cw)
for cw,threshold in itertools.product(cws,[0,1,49,50,51,75,99,100]):add(pattern=5,cw=cw,threshold=threshold)
rng=random.Random(0x481690)
for i in range(128):
    w=rng.randrange(1,229);h=rng.randrange(1,229);add(route=rng.randrange(2),x=rng.randrange(w),y=rng.randrange(h),rings=rng.randrange(5),mask=rng.choice([0x800,0,1,MASK,0x80000000]),w=w,h=h,pattern=rng.randrange(6),seed=rng.randrange(256),cw=rng.choice(cws),threshold=rng.randrange(101))
complete=len(cases)
if args.limit:cases=cases[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four actual shared binaries required'
executables=[(p or HERE/'build'/cfg/f'map_refresh_weighted_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
mappings=[(HERE/'unit_01192.c',ROOT/'src/unit_01192.c'),(HERE/'unit_01193.c',ROOT/'src/unit_01193.c'),(HERE/'map_refresh_fp.c',ROOT/'runtime/map_refresh_fp.c'),(HERE/'map_refresh_arithmetic.h',ROOT/'include/map_refresh_arithmetic.h'),(ROOT/'artifacts/map_refresh_phase/unit_01198.c',ROOT/'src/unit_01198.c'),(ROOT/'artifacts/map_refresh_phase/map_refresh.h',ROOT/'include/map_refresh.h'),(ROOT/'artifacts/map_refresh_phase/x87_conversion/map_x87_conversion.c',ROOT/'runtime/map_x87_conversion.c'),(ROOT/'artifacts/map_refresh_phase/x87_conversion/map_x87_conversion.h',ROOT/'include/map_x87_conversion.h')]
files=[HERE/name for name in ['weighted_terrain_fixture.c','map_ring_fixture_data.h','prepare_native_ring_fixtures.py','native-ring-fixtures.json','CMakeLists.txt','shared_targets.cmake','CONTRACT.md','verify_map_refresh_numeric.py','map-refresh-numeric-emulation.json']]+[Path(__file__),facts_path]+[b if args.production else a for a,b in mappings]
project_evidence={}
if args.production:
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_weighted_verify.exe','restore_map_refresh_weighted_plain_verify.exe']*2
    assert all('map_refresh_arithmetic_phase/build' not in p.as_posix() for p in executables),'private binaries are not production evidence'
    for staged,target in mappings:assert sha(staged)==sha(target),('applied source drift',target)
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
    expected={HERE/'weighted_terrain_fixture.c',ROOT/'src/unit_01192.c',ROOT/'src/unit_01193.c',ROOT/'src/unit_01198.c',ROOT/'runtime/map_refresh_fp.c',ROOT/'runtime/map_x87_conversion.c'}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg
        project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve()
        compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
        cc=groups[0].find('m:ClCompile',ns);runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed'),(project,runtime,optimization)
        assert ('MAP_REFRESH_ARITHMETIC_TRACE' in definitions)==(k%2==0) and ('MAP_REFRESH_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale executable',exe,source)
        project_evidence[str(project.relative_to(ROOT))]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[str(p.relative_to(ROOT)) for p in sorted(compiled)]};files.append(project)
link_bindings={}
owners={'_FUN_00481540':'unit_01192.obj','_FUN_00481690':'unit_01193.obj','_FUN_00481aa0':'unit_01198.obj','_restore_map_trunc80':'map_x87_conversion.obj','_mr_fp_add':'map_refresh_fp.obj','_mr_fp_div':'map_refresh_fp.obj','_mr_fp_store64':'map_refresh_fp.obj'}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        found=[line for line in lines if symbol in line.split()];assert len(found)==1 and found[0].split()[-1]==owner,(mp,symbol,found);bindings[symbol]=owner
    link_bindings[str(mp.relative_to(ROOT))]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};binary_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();returns={};per_route={0:0,1:0}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':12}
    for i,p in enumerate(cases):
        expected=native(p);returns[expected['result']]=returns.get(expected['result'],0)+1;per_route[p[0]]+=1
        for k,proc in enumerate(procs):
            proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();out=proc.stdout.readline();assert out,'C exit: '+proc.stderr.read();actual=json.loads(out)
            if k%2:actual['events']=[];want=dict(expected,events=[])
            else:want=expected
            if actual!=want:
                ae=actual['events'];ee=want['events'];mismatch=next((j for j in range(min(len(ae),len(ee))) if ae[j]!=ee[j]),min(len(ae),len(ee)))
                raise AssertionError(json.dumps({'case':i,'p':p,'variant':str(executables[k]),'event_mismatch':mismatch,'actual_events':ae[max(0,mismatch-3):mismatch+4],'expected_events':ee[max(0,mismatch-3):mismatch+4],'actual_other':{a:v for a,v in actual.items() if a!='events'},'expected_other':{a:v for a,v in want.items() if a!='events'}}))
        if (i+1)%128==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(instructions[a]==name for a in hits),sum(v==name for v in instructions.values())] for name in scope}
if not args.limit:assert coverage['00481540']==[96,96] and coverage['00481690']==[10,10] and coverage['0056133c']==[15,15]
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'decoded_coverage':[len(decoded_hits),len(decoded)],'per_route':per_route,'branch_outcomes':branch_counts,'return_histogram':returns,'arithmetic_event_counts':operation_counts,'control_words':cws,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':binary_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':binary_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(p):sha(p) for p in [facts_path,fixture_facts_path]},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=binary_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete481540/481690 numeric/memory algorithm in actual menu radius4 domain plus valid mapped rings0..4/direct mask extensions. Actual frozen481AA0 and explicit integer raw80 conversion are compiled; no controlled internal algorithms.','All24 all-exception-masked CW settings; exact raw80 intermediate results, binary64 spills and consumedFCOMP.C0 compared. Precision/invalid status flags and unmasked traps remain unproved.','Physical native x87 input stack/TOP and C caller old x87 values are preserved. The native worker modifies its discarded by-value rings argument slot to0; C uses a local copy. The only actual native caller481690 discards that outgoing slot and consumesEAX only.','Native table producer481F20/46F8D0 provides consistent XY/linear tables; no C equivalence claim for those producers. Numeric helpers support finite exponent separation<=192 and normal binary64 stores, which covers these map operands.','Private O2 worker/fixture links static Debug data library/CRT; shared recipe inherits root DebugMTd/ReleaseMT.488E90/489030 and their remaining postpasses are not reconstructed or promoted by this proof.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-weighted-emulation-partial.json' if args.limit else ('map-refresh-weighted-emulation.json' if args.production else 'weighted-terrain-emulation.json'))).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','branch_outcomes','elapsed_seconds']}))
