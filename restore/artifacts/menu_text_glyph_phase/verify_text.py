"""Whole five-function text algorithm versus367 authenticated original instructions."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
ap=argparse.ArgumentParser();ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path);args=ap.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
schema=json.loads((HERE/'fixture-schema.json').read_text());names=schema['names'];slot={n:i for i,n in enumerate(names)};fields=schema['fields'];regions=schema['regions'];TEXT=schema['text']
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;base=u32(opt+28);sec=opt+u16(pe+20)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000);vm.mem_map(0x4ac30000,0x10000)
get=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
original_map=bytes(vm.mem_read(0x5e047c,288));retained=json.loads((HERE/'native.json').read_text());instructions={};owned={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for key,e in retained['records'].items():
 assert sha(ROOT/e['cache_path'])==e['cache_sha256'];f=e['assembly'];assert not f['has_more'] and f['total']==len(f['items']);owned[key]=set()
 for x in f['items']:
  a=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw;instructions[a]=x;owned[key].add(a);i=next(cs.disasm(raw,a,1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
assert len(instructions)==367
# Authenticate only the deeper sprite's concrete cleanup facts, not its algorithm.
sprite_evidence=None
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 for e in json.loads(path.read_text())['entries']:
  if int(e['requested_address'],16)==0x53e590:sprite_evidence=(path,e)
assert sprite_evidence
sprite_path,sprite=sprite_evidence;assert not sprite['assembly']['has_more']
rets=[x for x in sprite['assembly']['items'] if x['instruction'].startswith('RET')];assert rets and all(x['instruction']=='RET 0xc' for x in rets),[x['instruction'] for x in rets]
for x in rets:assert bytes(vm.mem_read(int(x['address'],16),len(bytes.fromhex(x['bytes']))))==bytes.fromhex(x['bytes'])
hits=set();edges=set();mutations=set();native_reads=set();native_writes=set();events=[];p=None;previous=None;last_case=None
def value(name):return p[slot[name]]
def mutation(phase):
 if value('MUTPHASE')==phase and (not value('MUTORD') or value('MUTORD')==len(events)+1):
  j=value('MUTFIELD');assert j<len(fields);f=fields[j];put(f['address'],value('MUTVALUE'),f['width']);mutations.add((j,phase))
def code(cpu,a,size,user):
 global previous
 if a==0x53e590:
  assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;sp=cpu.reg_read(UC_X86_REG_ESP);ret=get(sp)
  argv=[get(sp+4+i*4) for i in range(3)];mutation(1)
  events.append([a,0xf55bb0]+argv+[get(f['address'],f['width']) for f in fields])
  # Before incrementing the event ordinal, both entry/exit mutations share it.
  event=events.pop();mutation(2);events.append(event)
  cpu.reg_write(UC_X86_REG_EAX,value('RESULT'));cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+16);cpu.reg_write(UC_X86_REG_EIP,ret);previous=None;return
 assert a in instructions,('unqualified native instruction',hex(a));hits.add(a)
 if previous in branches:assert a in branches[previous];edges.add((previous,a))
 previous=a
def watched(a,n):return any(lo<=a and a+n<=lo+size for lo,size in regions)
def read_hook(cpu,access,a,n,v,user):
 if STACK<=a and a+n<=STACK+0x10000:return
 assert watched(a,n),('unwatched native read',hex(a),n,hex(cpu.reg_read(UC_X86_REG_EIP)));native_reads.add((a,n))
def write_hook(cpu,access,a,n,v,user):
 if STACK<=a and a+n<=STACK+0x10000:return
 assert any(a==f['address'] and n==f['width'] for f in fields),('unqualified native write',hex(a),n);native_writes.add((a,n))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,read_hook);vm.hook_add(UC_HOOK_MEM_WRITE,write_hook)
patterns={}
def pattern(seed,j,n):
 key=(seed&255,j,n)
 if key not in patterns:
  period=bytes((seed+j*29+k*37)&255 for k in range(256));patterns[key]=period*(n//256)+period[:n%256]
 return patterns[key]
samples=[b'xy\0',b'\0',b'A B\0',b'#__\0',bytes([199,231,255,0]),b'  \0',b'XYZ\0']
def prepare(case):
 global p,events
 p=case;events=[]
 for j,(a,n) in enumerate(regions):vm.mem_write(a,pattern(value('SEED'),j,n))
 vm.mem_write(0x5e047c,original_map)
 if value('GLYPHS')==0:vm.mem_write(0x5e047c,bytes(288))
 elif value('GLYPHS')==1:vm.mem_write(0x5e047c,bytes(i%35+1 for i in range(288)))
 else:assert value('GLYPHS')==2
 data=bytearray(vm.mem_read(0x10d65f8,0x20420))
 for index in range(-16,2048):
  u=index&0xffffffff;profile=value('METRICS')
  w,h=(7+u%17,4+u%31) if profile==0 else (u%63-31,u%47-23) if profile==1 else (32767 if index&1 else -32768,32767 if index&2 else -32768) if profile==2 else (0,0)
  struct.pack_into('<hh',data,1024+index*64+20,w,h)
 vm.mem_write(0x10d65f8,bytes(data))
 ch=value('CHARACTER')&255;n=value('LONGCOUNT');assert n<=12000
 texts=[b'',bytes([ch]),b'ABC',b'A B',b'A__  B',b'#',b'A#B',b'##',bytes(range(1,32)),bytes(range(128,256)),b'__  A  __',b'#_#',b'AA',bytes(range(1,256)),b' '*n,b'A'*n,b'X#Y  Z']
 text=texts[value('TEXT')];vm.mem_write(TEXT,text+bytes(12001-len(text)))
 vm.mem_write(0xe34338,bytes(1024));vm.mem_write(0xe44338,bytes(512))
 for offset in [0,256,512,65536]:vm.mem_write(0xe34338+offset,samples[value('SUBPROFILE')]+bytes(8-len(samples[value('SUBPROFILE')])))
 if value('SUBSTART'):put(0xe34338,ord('q'),1)
 for a,v in [(0xd3b500,11),(0xd3b510,value('DIRECT')),(0xd3b514,value('SHADOW')),(0xd3b518,value('OUTLINE')),(0xe20770,0xdead5678),(0xe20774,value('BASELINE')),(0xe20778,value('CURSOR')),(0xe2077c,value('SUBSTART')),(0xe20780,value('ENABLE')),(0xe20784,value('POSITION')),(0xe20788,value('READY')),(0xe20990,value('SELECTOR')),(0xe207ec+(value('SELECTOR')*4&0xffffffff),value('TARGET')),(0x126517c,value('COUNT')),(0xf56fb4,871),(0xf56fbc,1005),(0xf56f84,201)]:put(a,v)
 put(0xe20798,value('COLLAPSE'),1);put(0xe20799,value('BLOCK'),1)
 markers=value('MARKERS')
 if markers in [1,2]:
  put(0xf56fb4,value('FONT'));put(0xf56fbc,value('FONT'))
  if markers==1:put(0xf56f84,value('FONT'))
 elif markers==3:
  for a,v in [(0xf56fb4,1005),(0xf56fbc,201),(0xf56f84,871)]:put(a,v)
 else:assert markers==0
def native(case):
 global previous
 prepare(case);op=value('OP');entry=[0x4cc770,0x4ccb00,0x4cbf30,0x4cca60,0x4cc720][op]
 argv=[TEXT,value('X'),value('Y'),value('FONT'),value('COLOR')] if op==0 else [] if op==1 else [TEXT,value('FONT')] if op==2 else [value('FONT'),value('CHARACTER'),value('X'),value('Y')] if op==3 else [TEXT,value('X'),value('Y'),value('WIDTH'),value('FONT'),value('COLOR')]
 for repeat in range(value('REPEATS')):
  sp=STACK+0x8000;vm.mem_write(sp-0x1000,bytes([0xa5])*0x2000);put(sp,STOP)
  for j,v in enumerate(argv):put(sp+4+j*4,v)
  put(sp+4+len(argv)*4,0xdec0ad01);previous=None
  for reg,v in [(UC_X86_REG_EAX,0x87654321),(UC_X86_REG_ECX,0x2468ace0),(UC_X86_REG_EDX,0x13579bdf),(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,v)
  vm.emu_start(entry,STOP,count=10000000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
  assert [vm.reg_read(reg) for reg in [UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]]==[0x11111111,0x22222222,0x33333333,0x44444444]
  assert get(sp+4+len(argv)*4)==0xdec0ad01
 result=vm.reg_read(UC_X86_REG_EAX) if op in [1,2,3] else 0
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'result':result,'memory':h.hexdigest(),'events':events[:]}
cases=[];seen=set()
def case(**kw):
 defaults={'OP':0,'SEED':29,'FONT':335,'X':20,'Y':40,'WIDTH':128,'COLOR':0xabcd1234,'TEXT':2,'CHARACTER':65,'SUBSTART':0,'ENABLE':0,'DIRECT':0,'SHADOW':0,'OUTLINE':0,'COLLAPSE':0,'BLOCK':0,'READY':0,'POSITION':0,'TARGET':0,'SELECTOR':0,'CURSOR':10,'BASELINE':20,'COUNT':2048,'METRICS':0,'GLYPHS':1,'MUTPHASE':0,'MUTFIELD':0,'MUTVALUE':0,'MUTORD':1,'REPEATS':1,'MARKERS':0,'LONGCOUNT':0,'SUBPROFILE':0,'RESULT':0x89abcdef};defaults.update({k.upper():v for k,v in kw.items()});q=tuple(defaults[n]&0xffffffff for n in names)
 if q not in seen:seen.add(q);cases.append(q)
 return q
for ch in range(256):case(text=1,character=ch);case(op=3,character=ch,font=201)
styles=[(0,0,0),(1,0,0),(0,1,0),(0,0,1),(0,1,1),(256,256,256)]
for font,style,text,glyphs in itertools.product([201,335,871,1005],styles,[0,2,4,5,6,7,9,10,11,13],[0,1,2]):case(font=font,direct=style[0],shadow=style[1],outline=style[2],text=text,glyphs=glyphs,enable=1)
for text,collapse,block,ready in itertools.product([0,3,4,10,11,12],[0,1,256],[0,1,256],[0,1,256]):case(text=text,collapse=collapse,block=block,ready=ready,enable=1)
for op,font,metrics,count in itertools.product([2,3,4],[-4,-1,0,201,335,871,1005,0x40000000,0x80000000,0x7fffffff],range(4),[0,1,16,2048,0x80000000]):case(op=op,font=font,metrics=metrics,count=count,text=2,character=0)
for font,ch,metrics in itertools.product([-4,-1,0],[3,38,73],range(4)):case(op=3,font=font,character=ch,metrics=metrics)
for font,metrics in itertools.product([-4,-1,0],range(4)):case(op=2,font=font,text=1,character=35,metrics=metrics)
for font,op in itertools.product([201,335,871,1005],[2,4]):case(op=op,font=font,text=13 if op==2 else 2,glyphs=0)
for op,index,profile,repeats in itertools.product([0,1,4],[0,1,256,512,65536],range(7),[1,3]):case(op=op,substart=index,subprofile=profile,enable=1,text=6,repeats=repeats)
for op,font,markers,width in itertools.product([0,2,3,4],[201,335,871,1005],range(4),[0,1,127,0x7fffffff,0x80000000,0xffffffff]):case(op=op,font=font,markers=markers,width=width,character=128)
for selector,position,target in itertools.product([0,1,39,256,0x80000000],[0,1,0xffffffff],[0,1,3,0xffffffff]):case(selector=selector,position=position,target=target,text=4)
for x,y,cursor in itertools.product([0,0x7fffffff,0x80000000,0xffffffff],[0x7fffffff,0x80000000],[0,0x7fffffff,0xffffffff]):case(x=x,y=y,cursor=cursor,shadow=1,text=4,collapse=1)
for op,n in itertools.product([0,2,4],[9999,10000,10001,12000]):case(op=op,text=14,longcount=n,glyphs=0);case(op=2,text=15,longcount=n)
mutation_values={0:0x80000000,1:0,2:256,3:256,4:256,5:0xffff,6:0x80000001,7:0x7fffffff,8:0xffffffff,9:0x7fffffff,10:65536,11:1,12:0xffffffff,13:0,14:0x80000000,15:0x7fffffff,16:0x80000000,17:1,18:1,19:256,20:335,21:335,22:335,23:0,24:0,25:32768,26:32767}
for text,phase,j in itertools.product([4,6,11,12],[1,2],range(len(fields))):case(text=text,enable=1,shadow=1,mutphase=phase,mutfield=j,mutvalue=mutation_values[j])
for op,phase,j in itertools.product([3,4],[1,2],range(len(fields))):case(op=op,text=12,character=65,mutphase=phase,mutfield=j,mutvalue=mutation_values[j])
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
total=len(cases);selected=cases[:args.limit] if args.limit else cases
sources=list((HERE/'src').glob('*.c'))+list((HERE/'include').glob('*.h'))+list((HERE/'tests').glob('*'))+[HERE/'fixture-schema.json',HERE/'native.json',HERE/'project/CMakeLists.txt',Path(__file__),ROOT/'include/sprite_render.h',ROOT/'include/sprite_libraries.h',ROOT/'include/standalone_data.h',sprite_path]
executables=[ROOT/'build/menu-text-glyph'/cfg/'restore_menu_text_glyph_verify.exe' for cfg in ['Debug','Release']]
before={p.relative_to(ROOT).as_posix():sha(p) for p in sources};binaries={p.relative_to(ROOT).as_posix():sha(p) for p in executables}
processes=[subprocess.Popen([str(p),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in executables]
report={'format':1,'production':False,'passed':False,'complete_case_set':not(args.limit or args.case),'checked':0,'complete_cases':total,'native_sha256':digest,'source_hashes':before,'binary_hashes':binaries,'original_game_executed':False,'C_sharp_used_as_exact_behavior':False,'limitations':['All five367-instruction text algorithms execute actual C/native bodies. Only the deeper53E590 sprite engine is controlled with exact ECX receiver/three arguments/RET12 and declared state mutations; no font pixels or physical rendering are claimed.','Full watched RAM and guards, ordered sprite args/global snapshots, consumed fullEAX returns and saved registers/ESP are compared. Ordinary C data-read ordering and unobservable intermediate stores are not claimed.','Finite mapped text/substitution/metric buffers are required. Rendering has no invented scan cutoff; only measurement has the native10000-byte limit. Negative/wrapped image probes are synthetic mapped-memory tests, not a claim that invalid fonts are ordinary game resources.']}
checked=0;start=time.monotonic();out=HERE/('partial-emulation.json' if args.limit or args.case else 'private-emulation.json')
try:
 assert all(json.loads(p.stdout.readline())=={'fields':len(names)} for p in processes)
 for q in selected:
  last_case=q;expected=native(q)
  for label,process in zip(['Debug','O2'],processes):
   process.stdin.write(' '.join(map(str,q))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,(label,checked,process.poll(),process.stderr.read());actual=json.loads(line)
   if actual!=expected:
    (HERE/'last-difference.json').write_text(json.dumps({'case':q,'named_case':dict(zip(names,q)),'variant':label,'C':actual,'native':expected},indent=2)+'\n');raise AssertionError((label,checked,next(k for k in actual if actual[k]!=expected[k])))
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(selected),'seconds':round(time.monotonic()-start,1)}),flush=True)
 if not(args.limit or args.case):
  assert set(instructions)<=hits,('uncovered instructions',[hex(a) for a in sorted(set(instructions)-hits)])
  assert {(a,b) for a,bs in branches.items() for b in bs}<=edges,('uncovered conditional outcomes',[[hex(a),hex(b)] for a,bs in branches.items() for b in bs if (a,b) not in edges])
 assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in sources} and binaries=={p.relative_to(ROOT).as_posix():sha(p) for p in executables}
 report['passed']=True
except Exception as e:report.update(error=str(e),last_case=last_case,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)));raise
finally:
 for process in processes:
  try:process.stdin.close();process.wait(timeout=10)
  except Exception:process.kill();process.wait()
  if process.returncode:report['passed']=False
 report.update(checked=checked,comparisons=checked*2,native_instruction_coverage=[len(hits),len(instructions)],conditional_outcomes=[len(edges),len(branches)*2],coverage={k:[len(z&hits),len(z)] for k,z in owned.items()},uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,bs in branches.items() for b in bs if (a,b) not in edges],mutated_fields_and_phases=[[fields[j]['name'],phase] for j,phase in sorted(mutations)],native_writes=[[hex(a),n] for a,n in sorted(native_writes)],elapsed_seconds=round(time.monotonic()-start,2))
 out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','binary_hashes','limitations','last_case','mutated_fields_and_phases']},indent=2));sys.exit(0 if report['passed'] else 1)
