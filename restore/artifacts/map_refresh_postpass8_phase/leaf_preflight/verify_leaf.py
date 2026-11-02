"""Whole actual record-reset/count/equality-water helpers, with no Cbody edits."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);args=ap.parse_args();MASK=0xffffffff;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes();u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;tab=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
 q=tab+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=HERE.parent/'postpass8-native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN
scope=['00484ee0','00484f40','0047a1f0'];instructions={}
for name in scope:
 for item in facts['functions'][name]['assembly']['items']:
  a=int(item['address'],16);raw=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw;instructions[a]=name
regions=[(0x6f1ff0,0x1020),(0xbc9730,0xc00),(0x78d79c,0x30)];STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096);hits=set();writes=0
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v if v<0x80000000 else v-0x100000000
def code(cpu,a,n,u):
 if a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
def memory(cpu,access,a,n,v,u):
 global writes
 if not STACK<=a<STACK+0x100000:
  assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(a),n)
  if access==17:writes+=1;assert n==1 and instructions[cpu.reg_read(UC_X86_REG_EIP)]=='00484ee0'
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def native(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[8]+37*i)&255])*n)
 vm.mem_write(0x6f1ff0,bytes((p[8]+i*17)&255 for i in range(0x1020)))
 for i in range(8):put(0xbc97c0+i*4,((p[3]>>i)&1)|(p[4]&~1))
 put(0x78d7ac,p[6]);put(0x78d7b0,p[7]);sp=STACK+0xf0000;put(sp,STOP);argv=[0 if p[1]==MASK else 0x6f2000+p[1],p[2]] if p[0]==0 else [] if p[0]==1 else [p[5]]
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 put(sp+4+4*len(argv),0xbaadf00d);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,v in saved:vm.reg_write(reg,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,0xdace1234^p[8]);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,p[9]);vm.reg_write(UC_X86_REG_FPSW,0x3000);vm.reg_write(UC_X86_REG_FPTAG,0xfff);vm.reg_write(UC_X86_REG_FP6,(0x8000000000000000,0xc000));vm.reg_write(UC_X86_REG_FP7,(0xc000000000000000,0x3fff))
 vm.emu_start([0x484ee0,0x484f40,0x47a1f0][p[0]],STOP,count=10000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(reg)==v for reg,v in saved)
 assert [read(sp+4+i*4) for i in range(len(argv))]==argv and read(sp+4+len(argv)*4)==0xbaadf00d
 assert vm.reg_read(UC_X86_REG_FPCW)==p[9] and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0x3000 and vm.reg_read(UC_X86_REG_FPTAG)==0xfff
 assert vm.reg_read(UC_X86_REG_FP6)==(0x8000000000000000,0xc000) and vm.reg_read(UC_X86_REG_FP7)==(0xc000000000000000,0x3fff)
 result=vm.reg_read(UC_X86_REG_EAX)
 if p[0]==0:result=0
 elif p[0]==2 and signed(p[7])<=0:result&=255
 digest=hashlib.sha256()
 for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
 return {'result':result,'cw':p[9],'memory':digest.hexdigest()}
default=[0,0,26,0x55,0,7,443,1,17,0x37f];cases=[];categories=[]
def add(category,**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for offset,count,seed in itertools.product([0,1,2,3,15,16,31],[-2,-1,0,1,2,26,48,64],[1,17,255]):add('record_reset',p1=offset,p2=count,p8=seed)
for count in [-2,-1,0]:add('empty_null_record_reset',p1=MASK,p2=count)
for mask,high in itertools.product(range(256),[0,0x100,0x80000000,0xfffffffe]):add('DWORD_count',p0=1,p3=mask,p4=high)
for active,diff in itertools.product([1,2,228,0x7fffffff],[-0x80000000,-513,-2,-1,0,1,2,455,456,457,0x7ffffffe,0x7fffffff]):
 half=(abs(diff)//2)*(-1 if diff<0 else 1)
 for delta in [-1,0,1]:add('enabled_water',p0=2,p5=228-half+delta,p6=active+diff,p7=active)
for active,x,total in itertools.product([0,-1,-0x80000000],[-0x80000000,-1,0,227,228,0x7fffffff],[0,1000,MASK]):add('disabled_AL_water',p0=2,p5=x,p6=total,p7=active)
for route,cw in itertools.product(range(3),[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])]):add('CW_preservation',p0=route,p9=cw)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
executables=[HERE/'build'/cfg/'map8_leaf.exe' for cfg in ['Debug','Release']];root_c=[ROOT/'src'/f'unit_{n:05d}.c' for n in [1235,1238,1098]];files=[Path(__file__),HERE/'fixture.c',HERE/'CMakeLists.txt',facts_path,ROOT/'include/restore_types.h',ROOT/'include/standalone_data.h',*root_c];rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in executables};owners={'_FUN_00484ee0':'unit_01235.obj','_FUN_00484f40':'unit_01238.obj','_FUN_0047a1f0':'unit_01098.obj'};maps={}
for exe in executables:
 mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
 for symbol,owner in owners.items():
  matched=[line for line in lines if symbol in line.split()];assert len(matched)==1 and matched[0].split()[-1]==owner,(symbol,matched);bindings[symbol]=owner
 maps[rel(mp)]={'sha256':sha(mp),'owners':bindings}
procs=[subprocess.Popen([str(exe),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for exe in executables];start=time.monotonic();counts={}
try:
 for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':10}
 for i,p in enumerate(cases):
  expected=native(p);counts[categories[i]]=counts.get(categories[i],0)+1
  for proc in procs:
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'Cexit:'+proc.stderr.read();actual=json.loads(line);assert actual==expected,json.dumps({'case':i,'parameters':p,'actual':actual,'expected':expected})
finally:
 for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={n:[sum(instructions[a]==n for a in hits),sum(v==n for v in instructions.values())] for n in scope}
if not args.limit:assert all(a==b for a,b in coverage.values()),coverage
report={'format':1,'passed':True,'production':False,'C_bodies_modified':False,'whole_47a220_verified':False,'checked':len(cases),'complete_cases':complete,'comparisons':2*len(cases),'coverage':coverage,'native_sha256':PIN,'original_game_executed':False,'categories':counts,'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'compiled_C_sha256':bins[rel(executables[0])],'variants':{label:{'path':rel(exe),'sha256':bins[rel(exe)],'checked':len(cases)} for label,exe in zip(['Debug','O2'],executables)},'source_hashes':before,'native_exports':{rel(facts_path):sha(facts_path)},'link_map_bindings':maps,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in executables if sha(p)!=bins[rel(p)]],'elapsed_seconds':round(time.monotonic()-start,2),'limitations':['Full actual unmodifiedC484EE0/484F40/47A1F0, no fake calls/results. Whole3guardregions, fullscalarEAX when defined, SP/GPR/argument/sentinel andold2x87/CW compared.','Reset voidEAX normalized only in fixture. Disabled water defines onlyAL; unused high24nativeincoming bits are ignored; enabled fullEAXcarry compared.','Internal memoryoperation order of these actual uninstrumentedhelpers is not separately compared; nonaliasrecordbuffers make independent reset stores observable at return, allguards compare.','Bounded validmapped16byterecords,count<=64, unalignedBYTEfield extensions, all256neighbor masks and signed32-bit water arithmetic. No generalFPstatus/trap claim, no whole47A220 or canonicalglobalST0promotion.','PrivateO2 uses actualDebugdata/staticDebugCRT; these cases remain distinct from laterwhole47A220cases.']}
assert not report['source_drift'] and not report['binary_drift'];(HERE/('leaf-partial.json' if args.limit else 'leaf-emulation.json')).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','elapsed_seconds']}))
