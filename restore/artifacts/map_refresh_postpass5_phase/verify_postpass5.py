"""Complete road rectangle/worker and real transitive native dependencies."""
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
facts_path=HERE/'postpass5-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and sha(ROOT/facts['parent_dossier']['path'])==facts['parent_dossier']['sha256']
instructions={}
for name,want in facts['unchanged_actual_C'].items():assert sha(ROOT/name)==want,name
for name,f in facts['functions'].items():
    assert not f['assembly']['has_more']
    for item in f['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
regions=[(11283772, 52016), (11490684, 260976), (11855084, 364432), (12359472, 3072), (13653776, 207968), (16083828, 368), (9932132, 48), (6079064, 1328), (7290864, 288), (7969240, 296), (7970296, 296), (7971352, 296), (7972408, 296)];record_input=bytes(vm.mem_read(0x5cc268,0x510));STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();events=[];running=False;branches={};routes={i:0 for i in range(7)};helper_returns=[];quiet={'0047db00','00484f20'}
def helper(a,phase,arg):events.append([3,a,phase,arg,read(0xbc9768),*[read(0xbc97c0+i*4) for i in range(8)]])
def code(cpu,a,n,user):
    if helper_returns and a==helper_returns[-1][0]:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    if a==STOP:return
    assert a in instructions,hex(a);hits.add(a)
    if a in [0x47db00,0x484f20]:
        sp=cpu.reg_read(UC_X86_REG_ESP);arg=read(sp+4) if a==0x47db00 else 0;helper(a,0,arg);helper_returns.append((read(sp),a,arg))
    if a in [0x47b69d,0x47b709,0x47b764,0x47b787,0x47b7af,0x47b7e8,0x47b816,0x47b84e,0x47b875,0x47b8a4]:branches[f'{a:08x}']=branches.get(f'{a:08x}',0)+1

def memread(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([0,a,n,read(a,n),0])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
        if instructions[cpu.reg_read(UC_X86_REG_EIP)] not in quiet:events.append([1,a,n,value&((1<<(n*8))-1),0])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
offsets=[-228,-227,1,229,228,227,-1,-229]
def prepare(p):
    for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[14]+i*37)&255])*n)
    vm.mem_write(0xac2d4c,bytes([p[10]&255])*0xcb10);vm.mem_write(0xaf558c,b'\0'*0x33040);vm.mem_write(0xd05720,b'\0'*0x32c40);vm.mem_write(0xb4e4fc,b'\0'*0x19820);vm.mem_write(0xb285cc,bytes([p[19]&255])*0xcb10)
    offset=((228-p[2])//2)*228+(228-p[1])//2
    for a,value in [(0xbca2d0,p[1]),(0xbca2d4,p[2]),(0xbca2d8,offset),(0xbca2dc,228-p[1]),(0xbca2e0,p[18])]:put(a,value)
    vm.mem_write(0xb7482c,struct.pack('<I',(p[14]+p[21])&MASK)*51984);put(0x6f4018,0 if p[12]==1 else 128 if p[12]==3 else 1,1)
    for dy in range(-2,3):
        for dx in range(-2,3):
            nx=signed(p[16])+dx;ny=signed(p[17])+dy
            if nx<0 or ny<0 or nx>=p[1] or ny>=p[2]:continue
            index=offset+ny*228+nx;put(0xaf598c+index*4,p[9] if p[15]==1 else 0);put(0xd05720+index*4,0x6f4000 if p[12] else 0)
    if p[16]<p[1] and p[17]<p[2]:
        index=offset+p[17]*228+p[16]
        if p[15] in [2,3]:
            for i,d in enumerate(offsets):put(0xaf598c+(index+d)*4,p[26+i] if p[15]==2 else p[9] if p[24]>>i&1 else 0)
        for i in range(4):
            cell=index+offsets[i*2]
            if 0<=cell<51984:put(0xb4e6fc+cell*2,i*4,2)
        put(0xaf598c+index*4,p[8]);put(0xb4e6fc+index*2,p[20],2)
    put(0xbc9768,offset+p[17]*228+p[16])
    for i in range(8):put(0xbc97c0+i*4,((p[24]>>i)&1)|(p[38]&~1))
    for i in range(4):
        a=0x7999e8+i*4*264;put(a,p[34],2);put(a+2,p[35] if p[35] else 2 if i&1 else 1,2);put(a+154,p[36]>>(i*8),1)
    vm.mem_write(0x5cc268,record_input)
    for i in range(81):
        a=0x5cc268+i*16
        if p[22]:put(a+13,p[22],1)
        put(a+14,p[23],1)
        if p[25]:put(a+12,p[25]-1,1)
    for a,value in [(0xf56b84,p[14]),(0xf56c34,p[14]),(0xf56c44,p[14]+0x10000),(0xf56c94,p[14]+0x20000),(0xf56cd4,p[14]+0x30000),(0x978d74,p[11])]:put(a,value)

def native(p):
    global events,running,helper_returns
    prepare(p);sp=STACK+0xf0000;put(sp,STOP)
    argv=[p[3],p[4],p[5],p[6]] if p[0]==1 else [p[7]] if p[0]==3 else [p[37],p[7]] if p[0]==4 else [p[39]] if p[0]==6 else []
    for i,value in enumerate(argv):put(sp+4+i*4,value)
    put(sp+4+len(argv)*4,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[13]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];helper_returns=[];running=True
    try:vm.emu_start([0x47b670,0x47b590,0x47b920,0x51c620,0x51c5f0,0x484f20,0x47db00][p[0]],STOP,count=2000000)
    finally:running=False
    if helper_returns and helper_returns[-1][0]==STOP and vm.reg_read(UC_X86_REG_EIP)==STOP:
        _,entry,arg=helper_returns.pop();helper(entry,1,arg)
    assert not helper_returns and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
    assert [read(sp+4+i*4) for i in range(len(argv))]==argv and read(sp+4+len(argv)*4)==0xbaadf00d
    assert vm.reg_read(UC_X86_REG_FPCW)==p[13] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
    assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
    digest=hashlib.sha256()
    for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
    return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0] in [2,3,4] else 0,'cw':vm.reg_read(UC_X86_REG_FPCW),'memory':digest.hexdigest(),'events':events.copy()}
default=[0,16,16,8,8,8,8,0,0x40,0x40,1,0,0,0x37f,1000,0,8,8,0,0,0,0,0,0,0x55,0,*([0]*8),0x3a,0,0,0x12345678,0,0x8000040];cases=[];categories=[]
def add(category='ordinary',**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    assert len(p)==40
    cases.append(p);categories.append(category)
for height,flag in itertools.product(range(256),[0,0x2000,0x10000,0x12000]):add('height_predicate',p0=2,p10=height,p8=flag)
for value in [0,1,21,42,47,48,50,51,52,53,54,55,83,84,85,86,87,88,0x7fffffea,0x7fffffeb,0x7fffffff,0x80000000,MASK]+[random.Random(i).getrandbits(32) for i in range(128)]:add('wrap_add',p0=3,p7=value)
for value in list(range(256))+[0x7fffffff,0x80000000,MASK]:add('exact_mapping',p0=4,p7=value)
for mask,high in itertools.product(range(256),[0,0x100,0x80000000]):add('normalize',p0=5,p24=mask,p38=high)
for mask,bits in itertools.product(range(256),[0x40,0x8000000,0x8000040]):add('gather',p0=6,p15=3,p9=bits,p24=mask)
for mode,flag,height,instance in itertools.product([0,1,2],[0,0x40,0x100,0x140,0x10000,0x10100,0x10140],[0,1,4,5,127,128,255],[0,1,2]):add('worker_profiles',p11=mode,p8=flag,p10=height,p12=instance)
for delta,flag,attribute,height in itertools.product([-0x80000000,-1,0,47,48,49,50,51,52,53,54,55,83,84,85,86,87,88,0x7fffffff],[0x100,0x10000],[0,1,2],[0,5]):add('sprite_boundaries',p21=delta,p8=flag,p25=attribute,p10=height)
for road,gates,roads,diags in itertools.product([0,0x40],range(16),range(16),[0,5,10,15]):
    fs=[0]*8
    for i in range(4):fs[i*2]=(0x100 if gates>>i&1 else 0)|(0x8000000 if roads>>i&1 else 0);fs[i*2+1]=0x8000000 if diags>>i&1 else 0
    add('composed_topology',p8=0x100|road|(0x10000 if (gates^roads)&1 else 0),p15=2,**{f'p{26+i}':v for i,v in enumerate(fs)})
for flag,mode,kind in itertools.product([0,0x40,0x44,0x48,0x4c,0x140],[0,1,2],[0x3a,0xaa,0xab,0xc8,0xc9,0xca,0x5a,0x13a,0xffff]):add('wrapper_leaf_types',p0=1,p8=flag,p11=mode,p9=0x8040,p15=1,p34=kind)
for word,secondary in itertools.product([0,1,0x100,0x8000,0xffff],[0,0x7f,0x80,0xff]):add('early_word_byte',p20=word,p19=secondary)
for orientation,frames,counter in itertools.product([0,2,4,6],[0,1,2,3,255],[0,254,255]):add('real_records',p18=orientation,p22=frames,p23=counter,p15=3,p9=0x40)
for w,h in [(0,0),(1,1),(2,2),(7,19),(19,7),(228,228)]:
    for x,y in [(0,0),(max(w-1,0),max(h-1,0)),(w//2,h//2)]:
        for route in [0,1]:add('mapped_geometry',p0=route,p1=w,p2=h,p3=x,p4=y,p5=x+1,p6=y+1,p16=x,p17=y,p15=0)
for rectangle in [(-1,-1,1,1),(9,9,8,8),(0,0,228,228),(0,0,-1,-1)]:add('rectangle_clipping',p0=1,p3=rectangle[0],p4=rectangle[1],p5=rectangle[2],p6=rectangle[3])
add('full_rectangle',p0=1,p1=228,p2=228,p3=0,p4=0,p5=227,p6=227,p16=114,p17=114)
for seed in [0,255,256,0x7fffffff,0x80000000,MASK]:add('DWORD_sprite_bases',p14=seed,p8=0x10100,p25=1,p21=48)
for route,cw in itertools.product(range(7),[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]):add('CW_preservation',p0=route,p13=cw,p8=0x140,p15=3,p9=0x140)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post5_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(cfg,kind) for cfg in ['Debug','Release'] for kind in ['trace','plain']])]
P2=ROOT/'artifacts/map_refresh_postpass2_phase';P4=ROOT/'artifacts/map_refresh_postpass4_phase'
mappings=[(HERE/f'unit_{n:05d}.c',ROOT/'src'/f'unit_{n:05d}.c') for n in [1107,1108,1110,2327]]+[(HERE/'map_refresh_postpass5.h',ROOT/'include/map_refresh_postpass5.h')]+[(P2/'unit_01233.c',ROOT/'src/unit_01233.c'),(P2/'map_refresh_postpass2.h',ROOT/'include/map_refresh_postpass2.h')]+[(P4/f'unit_{n:05d}.c',ROOT/'src'/f'unit_{n:05d}.c') for n in [1149,1152,1153]]+[(P4/'map_refresh_postpass4.h',ROOT/'include/map_refresh_postpass4.h')]
prerequisite_paths=[P2/'postpass2_apply_map.json',P4/'postpass4_apply_map.json'];pins=['7b0869757729657686f31467bb5f7cb1c2b20d72023c3cdf4733b41258bdff3d','2f2def7be2230355a51d941905c82a141aba2d26a9e276a24d686c63bd4de207'];frozen={}
for path,want in zip(prerequisite_paths,pins):
    assert sha(path)==want,path
    for m in json.loads(path.read_text())['mappings']:frozen[m['staged']]=m['staged_sha256']
for staged,target in mappings[5:]:assert sha(staged)==frozen[staged.relative_to(ROOT).as_posix()],staged
assert sha(HERE/'postpass5_record_data.h')==facts['record_input']['header_sha256']
import re
record_body=(HERE/'postpass5_record_data.h').read_text().split('post5_records[1296]={',1)[1].split('};',1)[0];assert bytes(int(v,16) for v in re.findall(r'0x([0-9a-fA-F]{2})',record_body))==record_input
root_c=[ROOT/name for name in facts['unchanged_actual_C']]
files=[HERE/'postpass5_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),facts_path,HERE/'postpass5_record_data.h',*root_c,*prerequisite_paths]+[target if args.production else staged for staged,target in mappings]
project_evidence={}
if args.production:
    freeze_path=HERE/'postpass5_apply_map.json'
    if args.manifest_sha256:assert sha(freeze_path)==args.manifest_sha256
    freeze=json.loads(freeze_path.read_text());assert freeze['frozen'] and freeze['native_sha256']==PIN
    for path,want in freeze['proof_hashes'].items():assert sha(ROOT/path)==want,path
    assert len(set(executables))==4 and [p.name.lower() for p in executables]==['restore_map_refresh_postpass5_verify.exe','restore_map_refresh_postpass5_plain_verify.exe']*2
    assert all('map_refresh_postpass5_phase/build' not in p.as_posix() for p in executables)
    for staged,target in mappings:assert sha(staged)==sha(target),target
    ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected={HERE/'postpass5_fixture.c',*root_c,*[target for staged,target in mappings if target.suffix=='.c']}
    for k,exe in enumerate(executables):
        cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project)
        resolve=lambda value:(project.parent/Path(value)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
        groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns)
        runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);definitions=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
        assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed') and ('MAP_REFRESH_POSTPASS5_TRACE' in definitions)==(k%2==0)
        for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared executable',exe,source)
        project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)
link_bindings={};owners={f'_FUN_{name}':f'unit_{n:05d}.obj' for name,n in [('0047b590',1107),('0047b670',1108),('0047b920',1110),('0051c620',2327),('0051c5f0',2326),('00484f20',1237),('0047db00',1137),('00484be0',1233),('0047e5c0',1149),('0047e870',1152),('0047eb60',1153)]}
for exe in executables:
    mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for symbol,owner in owners.items():
        matches=[line for line in lines if symbol in line.split()];assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,symbol,matches);bindings[symbol]=owner
    link_bindings[mp.relative_to(ROOT).as_posix()]={'sha256':sha(mp),'symbols':bindings}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bin_before={rel(e):sha(e) for e in executables};procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];started=time.monotonic();counts={};returns={}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':40}
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
if not args.limit:assert all(coverage[name]==[n,n] for name,n in [('0047b590',69),('0047b670',151),('0047b920',18),('0051c620',3),('0051c5f0',17),('00484f20',10),('0047db00',51)]),coverage
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'whole_47b590_complete_within_domain':True,'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_only_range_probes':0,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'branches':branches,'categories':counts,'routes':routes,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bin_before[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bin_before[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':link_bindings,'native_exports':{rel(facts_path):sha(facts_path)},'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(e) for e in executables if sha(e)!=bin_before[rel(e)]],'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Complete47B590/47B670/47B920/51C620 plus actual frozen BE0/E5C0/E870/EB60 and actual unchanged47DB00/484F20/51C5F0; no internal controlled endpoint or legacy substitute. Does not close47A220/475CC0/full488E90.','Ordered new-body and frozen algorithm reads/stores compare. Unchanged gather/normalize internal order is excluded with actualentry/exit tile+8DWORD snapshots and full13region comparison. Record input1296PEbytes independently authenticated; frame/attribute/counter variants explicitly prepared.','Full32bit wrap-add implemented via unsigned addition then explicit MSVCsigned32 return conversion. SignedBYTEheight, WORDobjectgates, actual instanceBYTE+24 and record264stride remain native contracts.','Void EAX is normalized as fixture0 only for void routes; predicate/add/mapping fullEAX compares. GPR/SP/stack sentinel/old2x87values/tagdepth/CW compare. No algorithmFPU; generalFPstatus/traps unproved.','Terrain/secondary and WORDid/mark/sprite adjacent physical planes are watched as unions with outercanaries; no independent guard is claimed between native-adjacent planes. PrivateO2 uses Debugdata/staticDebugCRT, productionrecipe rootDebugMTd/ReleaseMT.']}
assert not report['source_drift'] and not report['binary_drift']
((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass5-partial.json' if args.limit else 'map-refresh-postpass5-emulation.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','branches','elapsed_seconds']}))
