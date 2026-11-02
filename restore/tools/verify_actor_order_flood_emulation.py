"""Complete closed flood with actual fill, native buffers and qualified CFG."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[3] if 'actor_order_flood_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/actor_order_flood_phase';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-order-flood/Debug/restore_actor_order_flood_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-actor-order-flood/Release/restore_actor_order_flood_verify.exe');ap.add_argument('--start',type=int,default=0);ap.add_argument('--limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();product=ROOT if args.production else PHASE
names=re.findall(r'\bF_([A-Z0-9_]+)\b',(product/'tests/actor_order_flood_case.h').read_text().split('enum {',1)[1].split('};',1)[0]);names.remove('FIELDS');FIELDS=len(names);globals().update({'F_'+n:i for i,n in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x30000000;STACK=0x31000000;vm.mem_map(STOP,0x10000);vm.mem_map(STACK,0x20000)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
native_file=ROOT/'metadata/actor-order-flood-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
core={};sets={};branches={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
for name,f in d['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];sets[int(name,16)]=set()
 for x in a['items']:
  ip=int(x['address'],16);code=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(ip,len(code)))==code;core[ip]=x;sets[int(name,16)].add(ip);i=next(cap.disasm(code,ip,count=1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[ip]={ip+i.size,int(i.op_str,16)}
full_regions=[(0xbca2c0,48),(0xeadff0,0xcb40),(0xed4130,0xcb30),(0xeed750,0x658a0)]
fill_regions=[(0xeae000,32),(0xeed750,96)]
p=[];regions=[];events=[];previous=None;executed=set();edges=set();block_cache={};wrap_sites=set();native_metrics=[]
wrap_stores={0x51cf60:'retry',0x51cfe8:'north',0x51d051:'east',0x51d0bd:'south',0x51d126:'west'}
def block(cpu,address,size,user):
 global previous
 if previous in branches:edges.add((previous,address))
 key=(address,size)
 if key not in block_cache:
  decoded=list(cap.disasm(bytes(cpu.mem_read(address,size)),address));assert decoded and sum(i.size for i in decoded)==size
  for i in decoded:assert i.address in core,f'Native escaped {i.address:08x}';executed.add(i.address)
  block_cache[key]=decoded[-1].address
  for i in decoded:
   if i.address in wrap_stores:wrap_sites.add(wrap_stores[i.address])
 previous=block_cache[key]
def fill_entry(cpu,address,size,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);events.append([get(sp+4),get(sp+8),get(sp+12)&255,get(0xeae004),get(0xeae008),get(0xeae00c)]);assert len(events)<=2
def written(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x20000:return
 if p[F_OP]:
  start=0xeed760+p[F_ALIGN];assert start<=a and a+n<=start+p[F_FILL_BYTES],f'Fill guard write{a:08x}/{n}'
 else:assert 0xeae004<=a and a+n<=0xebab20 or 0xeed760<=a and a+n<=0xf52fe0,f'Flood guard write{a:08x}/{n}'
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_CODE,fill_entry,begin=0x40f8f0,end=0x40f8f0);vm.hook_add(UC_HOOK_MEM_WRITE,written)
def terrain(case):
 t=bytearray([253])*51984;style=case[F_STYLE]
 def carve(x,y,depth):
  assert 0<x<227 and 0<y<227;t[y*228+x]=254;step=case[F_WEIGHT]>>(depth//2)
  if depth>=case[F_WIDTH] or not step:return
  for i in range(1,step+1):
   if depth&1:t[(y-i)*228+x]=t[(y+i)*228+x]=254
   else:t[y*228+x-i]=t[y*228+x+i]=254
  if depth&1:carve(x,y-step,depth+1);carve(x,y+step,depth+1)
  else:carve(x-step,y,depth+1);carve(x+step,y,depth+1)
 if style>=7:
  carve(case[F_X],case[F_Y],0)
  if style==8:
   for y in range(1,227):
    for x in range(1,227):
     h=(x*1103515245+y*12345+case[F_SEED])&0xffffffff
     if t[y*228+x]==254 and (h>>16)%100>=case[F_HEIGHT]:t[y*228+x]=0
 else:
  for y in range(1,case[F_HEIGHT]-1):
   for x in range(1,case[F_WIDTH]-1):
    h=(x*1103515245+y*12345+case[F_SEED])&0xffffffff
    value={0:case[F_CLASS]&255,1:0,2:254 if (x+y)&1 else 0,3:254 if x&1 else 0,4:254 if (h>>16)%100<case[F_WEIGHT] else 0,5:255 if (h>>16)%100<case[F_WEIGHT] else 0,6:253 if x%8==0 and y%8!=1 else 254 if (x+y)%3==0 else 0}[style]
    t[y*228+x]=value
 start=(case[F_BASE]+case[F_Y]*228+case[F_X])&0xffffffff;assert start<51984
 if style!=0:t[start]=case[F_CLASS]&255
 return t
def setup(case):
 global p,regions,events,previous
 p=case;regions=fill_regions if p[F_OP] else full_regions;events=[];previous=None
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[F_SEED]+j*37+i*11)&255 for j in range(n)))
 if not p[F_OP]:put(0xbca2d8,p[F_BASE]);vm.mem_write(0xed4140,bytes(terrain(p)))
def native(case,index):
 setup(case);sp=STACK+0x10000;put(sp,STOP)
 argv=[0xeed760+p[F_ALIGN],p[F_FILL_BYTES],p[F_FILL_VALUE]&255] if p[F_OP] else [p[F_X],p[F_Y]]
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 saved={UC_X86_REG_EBX:p[F_SEED]^0x12345678,UC_X86_REG_ESI:p[F_SEED]^0x789abcde,UC_X86_REG_EDI:p[F_SEED]^0xabcdef12,UC_X86_REG_EBP:p[F_SEED]^0x2468ace0}
 for r,v in saved.items():vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,p[F_SEED]);vm.reg_write(UC_X86_REG_ECX,p[F_SEED]^0x11223344);vm.reg_write(UC_X86_REG_EDX,p[F_SEED]^0x99aabbcc);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
 before_wraps=set(wrap_sites);begin=time.monotonic();vm.emu_start(0x40f8f0 if p[F_OP] else 0x51ce90,STOP,count=20000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,{'case':index,'ip':hex(vm.reg_read(UC_X86_REG_EIP))}
 for r,v in saved.items():assert vm.reg_read(r)==v
 assert not vm.reg_read(UC_X86_REG_EFLAGS)&0x400
 answer=0 if p[F_OP] else vm.reg_read(UC_X86_REG_EAX)
 if p[F_OP]:metrics=[0,0,0,0]
 else:
  distances=struct.unpack('<51984I',bytes(vm.mem_read(0xeed760,207936)));attempts=bytes(vm.mem_read(0xeae010,51984));metrics=[sum(v!=0 for v in distances),sum(min(v,4) for v in attempts),get(0xeae008),get(0xeae00c)]
 native_metrics.append({'case':index,'fields':dict(zip(names,p)),'result':answer,'metrics':metrics,'new_wrap_sites':sorted(wrap_sites-before_wraps),'seconds':round(time.monotonic()-begin,3)})
 if p[F_STYLE]>=7 and not p[F_OP]:print(json.dumps({'native_case':index,'profile':p[F_SEED],'metrics':metrics,'new_tail_wraps':sorted(wrap_sites-before_wraps)}),flush=True)
 return {'result':answer,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events),'metrics':metrics}
rng=random.Random(0x51ce90);cases=[]
def add(**kw):
 values={'OP':0,'SEED':rng.getrandbits(32),'STYLE':0,'CLASS':0,'WEIGHT':50,'X':2,'Y':2,'BASE':0,'WIDTH':5,'HEIGHT':5,'FILL_BYTES':0,'FILL_VALUE':0,'ALIGN':0};values.update({k.upper():v for k,v in kw.items()});cases.append([values[n]&0xffffffff for n in names])
for classification,width in itertools.product([0,1,2,3,127,128,129,250,251,252,253,254,255],[3,5,9]):
 add(**{'class':classification,'width':width,'height':width,'x':width//2,'y':width//2})
for style,width,weight in itertools.product([1,2,3,4,5,6],[9,17,31],[10,50,99]):add(style=style,width=width,height=width,x=width//2,y=width//2,weight=weight)
for x,y,base in [(202,0,256),(0x01000002,2,0xff000000),(0xffffff00,0,714)]:add(style=1,x=x,y=y,base=base)
add(style=1,width=228,height=228,x=114,y=114)
for profile in [41,59,313,2119,1885]:add(style=8,weight=48,width=12,height=80+profile%21,x=114,y=114,seed=(profile*1234567)&0xffffffff,**{'class':254})
for size,value,alignment in itertools.product([0,1,2,3,4,5,7,31],[0,0x5a,0xffffffff],[0,1,3]):add(op=1,fill_bytes=size,fill_value=value,align=alignment)
selected=cases[args.start:args.start+args.limit if args.limit else None];expected=[native(c,args.start+i) for i,c in enumerate(selected)]
owned=sets[0x51ce90];excluded={int(x,16) for x in d['counter_qualification']['excluded_instructions']};excluded_edges={(int(a,16),int(b,16)) for a,b in d['counter_qualification']['excluded_edges']};feasible=owned-excluded;all_edges={(a,b) for a,z in branches.items() if a in owned for b in z};feasible_edges=all_edges-excluded_edges;complete=feasible<=executed and feasible_edges<=edges and wrap_sites==set(wrap_stores.values())
source_paths=[product/x for x in ['src/unit_02335.c','include/actor_order_flood.h','tests/actor_order_flood.c','tests/actor_order_flood_actual.c','tests/actor_order_flood_case.h']]+[ROOT/x for x in ['src/unit_00119.c','include/restore_types.h','include/standalone_data.h']]+[PHASE/'build/CMakeLists.txt',PHASE/'tools/extract_actor_order_flood_native.py',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,'complete_case_set':not args.limit and not args.start,'checked':len(selected),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file)},'qualified_controller':{'complete':complete,'native_instructions':len(owned),'feasible_instructions':len(feasible),'executed':len(feasible&executed),'missing_instructions':[f'{a:08x}' for a in sorted(feasible-executed)],'conditional_edges':len(all_edges),'feasible_edges':len(feasible_edges),'edges_executed':len(feasible_edges&edges),'missing_edges':[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(feasible_edges-edges)],'excluded_instructions':d['counter_qualification']['excluded_instructions'],'excluded_edges':d['counter_qualification']['excluded_edges'],'tail_wrap_sites':sorted(wrap_sites)},'actual_fill_coverage':{'native_instructions':22,'executed':len(sets[0x40f8f0]&executed),'missing':[f'{a:08x}' for a in sorted(sets[0x40f8f0]-executed)]},'watched_memory':{'flood_regions':full_regions,'flood_bytes_per_case':sum(n for a,n in full_regions),'direct_fill_regions':fill_regions,'every_payload_and_guard_byte_compared':True,'native_writes_restricted_to_exact_payloads_and_stack':True},'native_ABI':{'cdecl_arguments':2,'RET_cleanup':0,'full_EAX_compared':True,'stack_and_saved_registers':['ESP','EBX','ESI','EDI','EBP'],'DFclear_preserved':True,'fill_arguments':'fullDWORDdestination/count,lowBYTEvalue;actualC/nativebody,no controlled replacement.'},'native_fixture_metrics':native_metrics,'controlled_deep_boundaries':[],'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'limitations':d['limitations']+['Two original head-wrap instructions/edges are qualified infeasible by the exact50000-pop/noalias invariant in native.json. Full wrap code is preserved in the candidate. All five tail-wrap sites must execute on real valid boxed fixtures.','No physical GUI, cursor or OS effect is performed. This closes the distance-field producer only, not the still-unreconstructed LOS/action algorithms that consume its workspace.']}
out=ROOT/'metadata/actor-order-flood-emulation.json' if args.production else PHASE/'metadata/actor-order-flood-emulation.json'
try:
 if report['complete_case_set']:assert complete,report['qualified_controller'];assert report['actual_fill_coverage']['executed']==22
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();smoke=subprocess.run([str(path)],capture_output=True,text=True,timeout=10);assert smoke.returncode==0,smoke.stderr
  proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   assert json.loads(proc.stdout.readline())=={'fields':FIELDS}
   for i,(case,want) in enumerate(zip(selected,expected)):
    proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read());actual=json.loads(line)
    if actual!=want:
     field=next(k for k in want if actual[k]!=want[k]);detail={'variant':label,'case':args.start+i,'input':dict(zip(names,case)),'field':field,'C':actual[field],'native':want[field]};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError({k:v for k,v in detail.items() if k not in ['C','native']})
   proc.stdin.close();proc.wait(timeout=10);assert proc.returncode==0,proc.stderr.read()
  finally:
   if proc.poll() is None:proc.kill();proc.wait()
  report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(selected),'smoke_exit':smoke.returncode}
 assert all(sha(ROOT/path)==value for path,value in report['source_hashes'].items()),'Concurrent source drift.'
 assert all(sha(ROOT/path)==value for path,value in report['native_exports'].items()),'Concurrent evidence drift.'
 report['comparisons']=len(selected)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':report['passed'],'checked':len(selected),'comparisons':report.get('comparisons'),'coverage':report['qualified_controller'],'actual_fill':report['actual_fill_coverage']}))
