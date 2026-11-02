"""Whole47A220 and actual dependency closure; static original instructions only."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import *
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--start',type=int,default=0);ap.add_argument('--production',action='store_true');ap.add_argument('--manifest-sha256')
for option in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+option,type=Path)
args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes();u16=lambda a:struct.unpack_from('<H',image,a)[0];u32=lambda a:struct.unpack_from('<I',image,a)[0];pe=u32(60);opt=pe+24;tab=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
 q=tab+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=HERE/'postpass8-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN;instructions={}
for name,f in facts['functions'].items():
 assert not f['assembly']['has_more']
 for it in f['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
# Exact prep geometry from authenticated full-function cache, never gap disassembly.
cache_geometry=None
for p in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 d=json.loads(p.read_text());assert d['native_sha256']==PIN
 for e in d['entries']:
  if e['requested_address'].lower().lstrip('0x')=='46f7b0':cache_geometry=(p,e);break
 if cache_geometry:break
assert cache_geometry
for it in cache_geometry[1]['assembly']['items']:
 a=int(it['address'],16);raw=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw
for obj in facts['real_record_inputs'].values():assert bytes(vm.mem_read(int(obj['address'],16),len(bytes.fromhex(obj['bytes']))))==bytes.fromhex(obj['bytes'])
regions=[(0xacf640,0xd8700),(0xbc9730,0xc00),(0xd04f10,0x33c60),(0x6f4000,0x400),(0x78d79c,0x30),(0xf56b40,0x810),(0x5ca648,0x320),(0x5cbfa8,0x1c0)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096);read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
hits=set();events=[];running=False;helper_returns=[];helper_entries={0x46f930:2,0x47a1c0:2,0x47a1f0:2,0x47db00:1,0x484ee0:2,0x484f00:0,0x484f40:0};trace_owners={'0047a220','00484be0','0047dc80'};switch_targets=[{},{}];overlay_codes=set();primary_ordinals=set();secondary_ordinals=set();initial_road_variants=set()
def helper(a,phase,x,y,result=0):events.append([2,a,phase,x,y,result,read(0xbc9768),*[read(0xbc97c0+i*4) for i in range(8)]])
def code(cpu,a,n,user):
 if not running:return
 if helper_returns and a==helper_returns[-1][0]:
  _,entry,x,y=helper_returns.pop();r=cpu.reg_read(UC_X86_REG_EAX) if entry in [0x46f930,0x47a1c0,0x47a1f0,0x484f40] else 0
  if entry in [0x47a1c0,0x47a1f0] and signed(read(0x78d7b0))<=0:r&=255
  helper(entry,1,x,y,r)
 if a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
 if a in helper_entries:
  sp=cpu.reg_read(UC_X86_REG_ESP);cnt=helper_entries[a];x=read(sp+4) if cnt else 0;y=read(sp+8) if cnt>1 else 0;helper(a,0,x,y);helper_returns.append((read(sp),a,x,y))
 if a in [0x47a4e9,0x47a6ef]:
  i=0 if a==0x47a4e9 else 1;target=read((0x47aaa4 if i==0 else 0x47aae8)+cpu.reg_read(UC_X86_REG_EDX)*4);switch_targets[i][hex(target)]=switch_targets[i].get(hex(target),0)+1
 if a==0x47a8b9:overlay_codes.add(cpu.reg_read(UC_X86_REG_ECX))
 if a==0x47a37f:initial_road_variants.add(read(0xbc9748));primary_ordinals.add(cpu.reg_read(UC_X86_REG_EAX))
 if a==0x47a935:secondary_ordinals.add(cpu.reg_read(UC_X86_REG_EAX))
def memory(cpu,access,a,n,v,user):
 if not running or STACK<=a<STACK+0x100000:return
 if 0x47aaa4<=a and a+n<=0x47ab09:return # authenticated immutable jump vectors/mappers, not C-state data
 assert any(lo<=a and a+n<=lo+sz for lo,sz in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 if instructions[cpu.reg_read(UC_X86_REG_EIP)] in trace_owners:events.append([int(access==UC_MEM_WRITE),a,n,(v if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1),0])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def prepare(p):
 global running
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[16]+37*i)&255])*n)
 vm.mem_write(0xaf518c,b'\0'*(53008*4));vm.mem_write(0xd04f20,b'\0'*(53008*4))
 for obj in facts['real_record_inputs'].values():vm.mem_write(int(obj['address'],16),bytes.fromhex(obj['bytes']))
 for i in range(26):put(0x5cbfb8+i*16+14,p[16]+i,1)
 put(0xbca2d0,p[0]);put(0xbca2d4,p[1]);sp=STACK+0xf0000;put(sp,STOP);vm.reg_write(UC_X86_REG_ESP,sp);vm.emu_start(0x46f7b0,STOP,count=100000)
 b=((228-p[1])//2)*228+(228-p[0])//2
 for a,v in [(0xbca2d8,b),(0xbca2dc,228-p[0]),(0xbca2e0,p[5]),(0xbc9768,p[17])]:put(a,v)
 for i in range(8):put(0xbc97c0+i*4,p[16]^(i*0x11335577&MASK))
 tile=(b+p[3]*228+p[2])&MASK
 for i,offset in enumerate([-228,-227,1,229,228,227,-1,-229]):
  value=p[18]|(4 if p[6]>>i&1 else 0)|(0x10000 if p[7]>>i&1 else 0)|(0x100 if p[8]>>i&1 else 0)|(0x4000000 if p[9]>>i&1 else 0);put(0xaf598c+((tile+offset)&MASK)*4,value);address=0x6f4000+i*64;put(address+24,0 if p[11]>>i&1 else 0x7f,1);put(0xd05720+((tile+offset)&MASK)*4,0 if p[12]>>i&1 else address)
 put(0xaf598c+tile*4,p[10]);vm.mem_write(0xacf85c,bytes([p[13]])*51984);left=max(signed((p[2]-1)&MASK),0)
 put(0x78d7b0,1 if p[14] else 0);put(0x78d7ac,1001 if p[14]==1 else 1+2*(228-left) if p[14]==2 else 1)
 for i,a in enumerate([0xf5722c,0xf56bd4,0xf56c34,0xf57234,0xf57264,0xf56c9c,0xf57224,0xf57324,0xf56d24,0xf5721c,0xf56b64]):
  value=10000+i*1000
  if p[15]==1 or p[15]==2 and i in [2,3] or p[15]==3 and i in [0,1,4,5]:value=0x80000000
  elif p[15]==4:value=0xfffffff0
  put(a,value)
def native(p):
 global events,running,helper_returns
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);argv=p[2:5]
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 put(sp+16,0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0xfefefefe);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[19]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff));events=[];helper_returns=[];running=True
 try:vm.emu_start(0x47a220,STOP,count=1000000)
 finally:running=False
 assert not helper_returns and vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
 assert read(0xbc9768)==p[17]
 assert read(sp+16)==0xbaadf00d # Native consumes by-value argument slots as locals; caller discards them.
 assert vm.reg_read(UC_X86_REG_FPCW)==p[19] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
 assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'result':0,'cw':p[19],'memory':h.hexdigest(),'events':events.copy()}
# width,height,x,y,set,rotation,road8,building8,overlay8,secondary8,centerflags,inactive8,null8,animation,waterprofile,graphicsmode,seed,savedtile,neighborOR,CW
DEFAULT=[1,1,0,0,1,0,255,1,0,0,4,255,0,0,0,0,17,0xdeadbeef,0,0x37f];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
# Preserve every actual primary neighborhood/rotation, then branch-focused real input products.
for mask,rot in itertools.product(range(256),[0,2,4,6]):add('all_primary_neighborhoods',p6=mask,p5=rot,p13=(mask+rot)&255,p7=1)
catalog=json.loads((HERE/'primary-record-reachability.json').read_text());representatives={}
for mask,rot,ordinal,variant,frames in catalog['rows']:
 representatives.setdefault((variant,rot),(mask,ordinal))
for (variant,rot),(mask,_) in representatives.items():
 if variant not in [0,8,12,16,20,24,28,32,36,40,79]:continue
 for inactive,building,water in itertools.product([0,1,4,16,64,0x55,0xaa,255],[1,2,4,16,64,0x55,255],[0,1]):add('first_switch_inactive_instances',p6=mask,p5=rot,p11=inactive,p7=building,p14=water)
 for overlay,water in itertools.product(range(16),[0,1]):add('second_switch_cardinal_overlays',p6=mask,p5=rot,p11=0,p7=1,p8=sum((overlay>>i&1)<<(i*2) for i in range(4)),p14=water)
for anim,water,mask,graphics in itertools.product([0,1,2,3,5,6,127,255],[0,1,2,3],[0,0x55,255],[0,1,3,4]):add('secondary_fallback_reset_and_animation',p6=255,p7=0,p10=0x4000004,p9=mask,p13=anim,p14=water,p15=graphics)
for graphics,mask,rot in itertools.product([1,2,3,4],[0,0x15,0x55,0xaa,0xff],[0,2,4,6]):add('signed_sprite_fallback',p6=mask,p5=rot,p15=graphics)
for null in range(256):add('all_null_vs_BYTE_inactive_instances',p6=0x15,p7=255,p11=0x55,p12=null)
for w,h in [(0,0),(1,1),(2,2),(3,7),(16,16),(227,228),(228,228)]:
 for x,y in [(0,0),(w//2,h//2),(max(0,w-1),max(0,h-1)),(-1,0),(0,-1),(w,h)]:
  for state in [0,1,2,0x10001,-1]:add('diamond_physical_edges_clipping',p0=w,p1=h,p2=x,p3=y,p4=state,p10=0x4000004,p6=0x55,p7=0x11,p8=0xaa,p9=0x55)
for center,other in itertools.product([0,4,8,12,0x10004,0x4000004],[0,8,0x10000,0x4000000]):add('skip_and_bit_preservation',p0=16,p1=16,p2=8,p3=8,p10=center,p18=other,p6=0xaa,p7=0x55)
for animation in range(256):add('all_BYTE_animation',p10=0x4000004,p7=0,p13=animation,p14=2,p9=0x55)
for seed,saved in itertools.product([0,1,127,255],[0,1,0x7fffffff,0x80000000,MASK]):add('whole_guard_seed_saved_tile',p16=seed,p17=saved,p0=16,p1=16,p2=8,p3=8,p6=0xaa,p7=0x55,p8=0x55)
for cw in [0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]:add('CW_preservation',p19=cw,p10=0x4000004,p7=0,p14=2)
for (variant,rot),(mask,_) in representatives.items():
 for water,graphics in itertools.product([0,1],[0,1,4]):add('cell_10000_direct_or_fallback',p10=0x10004,p6=mask,p5=rot,p14=water,p15=graphics)
for (x,y),center,water in itertools.product([(113,0),(114,0),(113,227),(114,227),(0,113),(0,114),(227,113),(227,114)],[4,0x10004,0x4000004,12],[0,1,2]):add('actual_physical_diamond_vertices',p0=228,p1=228,p2=x,p3=y,p10=center,p14=water,p6=0x55,p7=0x11,p8=0xaa,p9=0x55,p18=0x80000000)
for height,y in [(0,1),(0,2),(0,4),(1,2),(2,3),(3,4)]:add('admitted_empty_row_crop_metadata_extension',p0=16,p1=height,p2=8,p3=y,p6=255,p10=4)
complete=len(cases)
if args.start:
 assert args.limit and not args.production
 cases=cases[args.start:];categories=categories[args.start:]
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four actual shared executables required'
executables=[(p or HERE/'build'/cfg/f'map_post8_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(c,k) for c in ['Debug','Release'] for k in ['trace','plain']])]
root_c=[ROOT/'src'/f'unit_{n:05d}.c' for n in [1009,1011,1097,1098,1137,1235,1236,1238]];frozen_c=[ROOT/'artifacts/map_refresh_postpass2_phase/unit_01233.c',ROOT/'artifacts/map_neighbors_phase/unit_01139.c'];files=[Path(__file__),facts_path,HERE/'unit_01099.c',HERE/'map_refresh_postpass8.h',HERE/'postpass8_fixture.c',HERE/'postpass8_records.h',HERE/'CMakeLists.txt',HERE/'primary-record-reachability.json',HERE/'probe_primary_records.py',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',*root_c,*frozen_c,ROOT/'artifacts/map_refresh_postpass2_phase/map_refresh_postpass2.h',ROOT/'artifacts/map_neighbors_phase/map_neighbors_phase.h',HERE/'leaf_preflight/leaf-freeze.json',cache_geometry[0],*[ROOT/'include'/n for n in ['restore_types.h','standalone_data.h','city_prepare.h','terrain_redraw.h']]]
project_evidence={}
if args.production:
 manifest=HERE/'postpass8_apply_map.json'
 if args.manifest_sha256:assert sha(manifest)==args.manifest_sha256
 frozen=json.loads(manifest.read_text());assert frozen['frozen'] and frozen['native_sha256']==PIN
 for name,want in frozen['proof_hashes'].items():assert sha(ROOT/name)==want,name
 for mapping in frozen['mappings']+frozen['required_prerequisite_mappings']:
  assert sha(ROOT/mapping['staged'])==mapping['staged_sha256'] and sha(ROOT/mapping['target'])==mapping['staged_sha256'],mapping
 for name,want in frozen['unchanged_actual_root_sources'].items():assert sha(ROOT/name)==want,name
 frozen_c=[ROOT/'src/unit_01233.c',ROOT/'src/unit_01139.c']
 remap={HERE/'unit_01099.c':ROOT/'src/unit_01099.c',HERE/'map_refresh_postpass8.h':ROOT/'include/map_refresh_postpass8.h',ROOT/'artifacts/map_refresh_postpass2_phase/unit_01233.c':frozen_c[0],ROOT/'artifacts/map_neighbors_phase/unit_01139.c':frozen_c[1],ROOT/'artifacts/map_refresh_postpass2_phase/map_refresh_postpass2.h':ROOT/'include/map_refresh_postpass2.h',ROOT/'artifacts/map_neighbors_phase/map_neighbors_phase.h':ROOT/'include/map_neighbors_phase.h'}
 files=[remap.get(p,p) for p in files]+[manifest]
 assert len(set(executables))==4 and [e.name for e in executables]==['restore_map_refresh_postpass8_verify.exe','restore_map_refresh_postpass8_plain_verify.exe']*2
 expected={HERE/'postpass8_fixture.c',ROOT/'src/unit_01099.c',*root_c,*frozen_c};ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
 for k,exe in enumerate(executables):
  cfg='Debug' if k<2 else 'Release';assert exe.parent.name==cfg;project=exe.parent.parent/(exe.stem+'.vcxproj');tree=ET.parse(project);resolve=lambda v:(project.parent/Path(v)).resolve();compiled={resolve(n.get('Include')) for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;cc=groups[0].find('m:ClCompile',ns);runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns);defines=cc.findtext('m:PreprocessorDefinitions',default='',namespaces=ns)
  assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed')
  for define in ['MAP_REFRESH_POSTPASS8_TRACE','MAP_REFRESH_POSTPASS2_TRACE','MAP_NEIGHBORS_TRACE']:assert (define in defines)==(k%2==0)
  for source in expected|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale',exe,source)
  project_evidence[project.relative_to(ROOT).as_posix()]={'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[p.relative_to(ROOT).as_posix() for p in sorted(compiled)]};files.append(project)

files+=list({e.parent/'restore_data.lib' for e in executables}) if args.production else [ROOT/'build/c-only-debug/Debug/restore_data.lib']
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in executables};owners={f'_FUN_{a}':f'unit_{n:05d}.obj' for a,n in [('0047a220',1099),('00484be0',1233),('0047dc80',1139),('0046f7b0',1009),('0046f930',1011),('0047a1c0',1097),('0047a1f0',1098),('0047db00',1137),('00484ee0',1235),('00484f00',1236),('00484f40',1238)]};maps={}
for exe in executables:
 mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();binding={}
 for symbol,owner in owners.items():
  found=[line for line in lines if symbol in line.split()];assert len(found)==1 and found[0].split()[-1]==owner,(exe,symbol,found);binding[symbol]=owner
 maps[rel(mp)]={'sha256':sha(mp),'bindings':binding}
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables];start=time.monotonic();counts={}
try:
 for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':20}
 for i,p in enumerate(cases):
  expected=native(p);counts[categories[i]]=counts.get(categories[i],0)+1
  for k,proc in enumerate(procs):
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'Cexit:'+proc.stderr.read();actual=json.loads(line);want=expected
   if k%2:actual['events']=[];want=dict(expected,events=[])
   if actual!=want:
    ae=actual['events'];ee=want['events'];j=next((j for j in range(min(len(ae),len(ee))) if ae[j]!=ee[j]),min(len(ae),len(ee)))
    raise AssertionError(json.dumps({'case':i,'p':p,'variant':str(executables[k]),'event':j,'actual_events':ae[max(0,j-3):j+4],'expected_events':ee[max(0,j-3):j+4],'actual_state':{x:v for x,v in actual.items() if x!='events'},'expected_state':{x:v for x,v in want.items() if x!='events'}}))
  if (i+1)%512==0:print(f'checked{i+1}/{len(cases)}',flush=True)
finally:
 for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={n:[sum(instructions[a]==n for a in hits),len(f['assembly']['items'])] for n,f in facts['functions'].items()};missing=[hex(a) for a,n in instructions.items() if n=='0047a220' and a not in hits]
if not args.limit:assert set(missing)=={'0x47a998','0x47a999','0x47a99c'},json.dumps({'missing':missing,'coverage':coverage})
report={'format':1,'passed':True,'production':args.production,'product_applied':args.production,'whole_map_refresh_complete':False,'whole_47a220_complete_within_domain':not bool(args.limit),'unreachable_main_instructions':{'addresses':['0047a998','0047a999','0047a99c'],'reason':'XOR EAX,EAX; MOV AL,BYTE; signed remainder fixup branch cannot have a negative input'},'checked':len(cases),'complete_cases':complete,'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'coverage':coverage,'missing_main_instructions':missing,'switch_target_counts':switch_targets,'overlay_codes':sorted(overlay_codes),'primary_ordinals':sorted(primary_ordinals),'primary_variants':sorted(initial_road_variants),'secondary_ordinals':sorted(secondary_ordinals),'categories':counts,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bins[rel(executables[0])],'variants':{label:{'path':rel(e),'sha256':bins[rel(e)],'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':before,'project_evidence':project_evidence,'link_map_bindings':maps,'native_exports':{rel(facts_path):sha(facts_path)},'native_only_record_range_cases':3072,'separate_C_leaf_cases':1465,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in executables if sha(p)!=bins[rel(p)]],'elapsed_seconds':round(time.monotonic()-start,2),'limitations':['Actual complete599-instruction main plus ten-function1019-instruction algorithm closure, no internal stubs. Prep geometry actualC/native90 instructions is separate fixture preparation.','Ordered main/frozenBE0/frozenDC80 reads/stores compare; immutable switch table reads authenticated separately. Unchanged root helper entry/return full8DWORD context snapshots compare; reset/count/equality have separate wholeleaf proof. Full8regions including record counters/map arrays/pointer grid/instance BYTE fields/guards compare.','Native voidEAX and consumed by-value slots ignored, callee-saved GPR/SP/sentinel/two oldx87/CW compare. DisabledwaterAL only, enabled fullEAXcarry; boundsfullEAXcarry.','No x87 algorithm here and no generalFPSW/trap equivalence claim. Frozen helpers remain staged, not presumed applied. PrivateO2 staticDebugCRT/data; rootRelease runtime would beMT.']}
assert not report['source_drift'] and not report['binary_drift'];((ROOT/'metadata' if args.production else HERE)/('postpass8-partial.json' if args.limit else 'map-refresh-postpass8-emulation.json')).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','missing_main_instructions','switch_target_counts','overlay_codes','elapsed_seconds']}))
