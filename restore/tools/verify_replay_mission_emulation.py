"""Replay callback and campaign fallback versus pinned x86, actual profile getter and compiled C."""
from pathlib import Path
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;PHASE=ROOT/'artifacts/replay_mission_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path);args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff;ALT=0x31000000;STOP=0x33000000;STACK=0x34000000
schema=(PHASE/'tests/replay_case.h').read_text();names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum {',1)[1].split('};',1)[0]) if x!='FIELDS'];FIELDS=len(names);globals().update({'P_'+name.upper():j for j,name in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(ALT,0x2000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def string(a):
 out=bytearray()
 for j in range(100):
  value=read(a+j,1)
  if value==0:return bytes(out)
  out.append(value)
 raise AssertionError('Unterminated bounded filename/profile')
def text_hash(a):
 h=2166136261
 for value in string(a):h=((h^value)*16777619)&MASK
 return h
def make_text(length,style,max_length):
 alphabets=[b'ABCDEFGHIJKLMNOPQRSTUVWXYZ',b'abcdefghijklmnopqrstuvwxyz0123456789',b'Ab 9_-.',b'0123456789'];assert length<=max_length and style<4
 alphabet=alphabets[style];return bytes(alphabet[j%len(alphabet)] for j in range(length))+b'\0'
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
functions={int(a,16):packet['functions'][a]['assembly']['items'] for a in ['00418510','004181e0','005169c0']}
for a in ['00418510','004181e0','005169c0']:assert not packet['functions'][a]['assembly'].get('has_more',False)
instructions={int(i['address'],16):i for body in functions.values() for i in body}
cap=Cs(CS_ARCH_X86,CS_MODE_32);decoded={};branches={}
for a,i in instructions.items():
 encoded=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(encoded)))==encoded
 d=next(cap.disasm(encoded,a));decoded[a]=d
 if d.mnemonic.startswith('j') and d.mnemonic!='jmp':branches[a]=(a+d.size,int(d.op_str,16))
assert [string(a) for a in [0x5e0494,0x5761d0,0x57bfc4,0x5763bc]]==[b'save\\',b'\\',b'autosave_replay.sav',b'rb']
assert len(functions[0x418510])==102 and len(functions[0x4181e0])==11 and len(functions[0x5169c0])==38
owned_functions={0x418510,0x4181e0};owned={int(i['address'],16) for fn in owned_functions for i in functions[fn]}
regions=[(0xe38db0,0x130),(0x6059d8,0x80),(0x702798,0x40),(0x6fdc98,0x40),(0x6de500,0x30),(0xd3b110,0x40),(0xe37068,0x60),(0xeade90,0x80),(ALT,0x2000)]
immutable=[(0x5e0494,6),(0x5761d0,2),(0x57bfc4,20),(0x5763bc,3)]
deep={0x561a0a,0x561994,0x4d51b0,0x4cb6a0,0x5405b0,0x41aa20,0x418140}
events=[];writes=[];paths=[];frames=[];hits=set();edges=set();mutated=set();controlled_reached=set();local_offsets=set()
params=None;seen=0;active_filename=0;last_block=None;block_cache={};fill_cache={}
def path_observe(fn,phase,a):paths.append([fn,phase,string(a).hex()])
def observe(fn,phase,values,result=0):
 global seen
 p=params;e=[fn,phase]+list(values)+[0]*(4-len(values))+[result]
 e +=[read(a,n) for a,n in [(0xe38ebc,4),(0x7027b4,1),(0x6059e8,4),(0x605a3c,4),(0x6de515,1),(0x6fdcb4,1),(0xe38dda,1),(0xe38dca,1),(0x7027a8,4),(0xd3b124,4),(0xd3b130,4),(0xd3b138,4)]]
 e +=[text_hash(a) for a in [0xe38e74,0xeadea4,0xe37078]]
 e +=[read(a) for a in [ALT+0x100c,ALT+0x1010,ALT+0x110c,ALT+0x1110]]
 e +=[len(string(a)) for a in [0xe38e74,0xeadea4,0xe37078]]+[read(0xe38eb8),read(0xe38ec0),text_hash(active_filename) if active_filename else 0]
 assert len(e)==32;events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   n=p[P_MBITS];mutated.add((fn,phase))
   if n&1:
    for a,v,width in [(0xe38ebc,p[P_NEWKIND],4),(0x7027b4,p[P_NEWCAMPAIGN],1),(0x6059e8,p[P_NEWCHOOSER],4)]:write(a,v,width)
   if n&2:
    for a,v,width in [(0x605a3c,p[P_NEWEDITOR],4),(0x6de515,p[P_NEWFLAG],1),(0x6fdcb4,p[P_NEWFALLBACK],1),(0xe38dda,p[P_NEWMODE],1),(0xe38dca,p[P_NEWDIRTY],1)]:write(a,v,width)
   if n&4:
    for a,v in [(0x7027a8,p[P_NEWSETUP]),(0xd3b124,p[P_NEWSELECTION]),(0xd3b130,p[P_NEWPLAYER]),(0xd3b138,p[P_NEWPROGRESS])]:write(a,v)
   if n&8:vm.mem_write(0xe38e74,make_text(p[P_NEWPROFILE_LENGTH],p[P_NEWPROFILE_STYLE],31))
   if n&16:vm.mem_write(0xeadea4,make_text(p[P_NEWPATH_LENGTH],p[P_NEWPATH_STYLE],36))
   if n&32:vm.mem_write(0xe37078,make_text(p[P_NEWMISSION_LENGTH],p[P_NEWMISSION_STYLE],32))
   if n&64:
    stream=ALT+(0x1100 if p[P_STREAM]==2 else 0x1000);write(stream+12,p[P_NEWSTREAM_FLAG]);write(stream+16,p[P_NEWSTREAM_FILE])
   if n&128:
    assert active_filename;vm.mem_write(active_filename,make_text(p[P_NEWPATH_LENGTH],p[P_NEWPATH_STYLE],36))
def enter(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);frames.append((a,sp));observe(a,0,[])
def leave(cpu,a,n,user):
 global active_filename
 sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native RET frame';fn,old=frames.pop()
 if fn==0x418510:active_filename=0
 observe(fn,1,[],cpu.reg_read(UC_X86_REG_EAX) if fn==0x5169c0 else 0)
def endpoint(cpu,a,n,user):
 global active_filename
 p=params;sp=cpu.reg_read(UC_X86_REG_ESP);controlled_reached.add(a);result=p[P_RESULT]
 if a==0x561a0a:
  name=read(sp+4);mode=read(sp+8);assert mode==0x5763bc and string(mode)==b'rb';active_filename=name
  path_observe(a,0,name);observe(a,0,[text_hash(name),len(string(name)),mode,text_hash(mode)])
  assert p[P_STREAM]<=2;result=ALT+(0x1100 if p[P_STREAM]==2 else 0x1000) if p[P_STREAM] else 0
  path_observe(a,1,name);observe(a,1,[text_hash(name),len(string(name)),mode,text_hash(mode)],result)
 elif a==0x561994:
  stream=read(sp+4);assert p[P_STREAM] and stream==ALT+(0x1100 if p[P_STREAM]==2 else 0x1000);observe(a,0,[stream]);result=p[P_CLOSERET];observe(a,1,[stream],result)
 elif a==0x4d51b0:
  name=read(sp+4);assert name==active_filename;path_observe(a,0,name);observe(a,0,[text_hash(name),len(string(name))]);result=p[P_LOADRET];path_observe(a,1,name);observe(a,1,[text_hash(name),len(string(name))],result)
 elif a==0x5405b0:
  name=read(sp+4);kind=read(sp+8)&255;assert name==0xe37078;path_observe(a,0,name);observe(a,0,[name,kind,text_hash(name),len(string(name))]);path_observe(a,1,name);observe(a,1,[name,kind,text_hash(name),len(string(name))])
 elif a==0x41aa20:
  values=[read(sp+4),read(sp+8),read(sp+12)];assert values==[0,53,1000];observe(a,0,values);observe(a,1,values)
 elif a==0x418140:observe(a,0,[]);result=p[P_QUERYRET];observe(a,1,[],result)
 elif a==0x4cb6a0:observe(a,0,[]);observe(a,1,[])
 else:raise AssertionError(hex(a))
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def block(cpu,a,n,user):
 global last_block
 if a in deep:return
 assert a in instructions,'Native escaped '+hex(a)
 if last_block is not None:edges.add((last_block,a))
 if(a,n) not in block_cache:block_cache[a,n]=sorted(x for x in instructions if a<=x<a+n)
 values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:
  ip=cpu.reg_read(UC_X86_REG_EIP)
  if ip in instructions and decoded[ip].mnemonic.startswith('rep movs') and frames[-1][0]==0x418510:
   low=frames[-1][1]-100;assert low<=a and a+n<=low+100,'Native filename overflow';local_offsets.update(range(a-low,a-low+n))
  return
 assert any(s<=a and a+n<=s+z for s,z in regions),'Unwatched native write '+hex(a)
 if frames and frames[-1][0] in owned_functions:writes.append([a,n,value&((1<<(8*n))-1)])
def memory_read(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions+immutable),'Unwatched native read '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write);vm.hook_add(UC_HOOK_MEM_READ,memory_read)
for fn,body in functions.items():
 vm.hook_add(UC_HOOK_CODE,enter,begin=fn,end=fn)
 for i in body:
  a=int(i['address'],16)
  if decoded[a].mnemonic.startswith('ret'):vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
for a in deep:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global params,seen,active_filename,last_block
 params=p;seen=0;active_filename=0;events.clear();writes.clear();paths.clear();frames.clear()
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 for a,v,n in [(0xe38ebc,p[P_KIND],4),(0x7027b4,p[P_CAMPAIGN],1),(0x6059e8,p[P_CHOOSER],4),(0x605a3c,p[P_EDITOR],4),(0x6de515,p[P_FLAG],1),(0x6fdcb4,p[P_FALLBACK],1),(0xe38dda,p[P_MODE],1),(0xe38dca,p[P_DIRTY],1),(0x7027a8,p[P_SETUP],4),(0xd3b124,p[P_SELECTION],4),(0xd3b130,p[P_PLAYER],4),(0xd3b138,p[P_PROGRESS],4)]:write(a,v,n)
 vm.mem_write(0xe38e74,make_text(p[P_PROFILE_LENGTH],p[P_PROFILE_STYLE],31));vm.mem_write(0xe37078,make_text(p[P_MISSION_LENGTH],p[P_MISSION_STYLE],32));vm.mem_write(0xeadea4,b'previous-profile-path\0')
 fn=[0x418510,0x4181e0,0x5169c0][p[P_OP]];saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp-0x400,b'\xcc'*0x600);write(sp,STOP)
  for reg,value in saved:vm.reg_write(reg,value)
  for reg,value in [(UC_X86_REG_EAX,0xa5a55a5a),(UC_X86_REG_ECX,0x12345678),(UC_X86_REG_EDX,0x87654321),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  last_block=None;vm.emu_start(fn,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native budget/frame';assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack cleanup';assert all(vm.reg_read(reg)==value for reg,value in saved),'Native saved registers';assert bytes(vm.mem_read(sp+4,128))==b'\xcc'*128,'Native caller argument guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'memory':h.hexdigest(),'events':events.copy(),'writes':writes.copy(),'paths':paths.copy()}

default=[0]*FIELDS;defaults={'seed':1,'campaign':1,'chooser':0x12345678,'editor':0x100,'flag':0x5a,'fallback':0xa5,'mode':53,'dirty':0x66,'setup':0xabcdef01,'selection':0x12345678,'player':0x87654321,'progress':0x89abcdef,'profile_length':7,'mission_length':12,'stream':1,'closeret':MASK,'loadret':0x80000000,'queryret':0x12345678,'result':0x87654321,'repeats':1,'mocc':1,'newkind':0x100,'newcampaign':2,'newchooser':0x76543210,'neweditor':0x89abcdef,'newflag':0x77,'newfallback':0x88,'newmode':14,'newdirty':0x99,'newsetup':0x13579bdf,'newselection':0x2468ace0,'newplayer':0xfedcba98,'newprogress':0x87654321,'newprofile_length':31,'newprofile_style':1,'newpath_length':36,'newpath_style':2,'newmission_length':32,'newmission_style':3,'newstream_flag':0x12345678,'newstream_file':0x87654321}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
for op in range(3):case(op=op)
for kind,campaign,stream in itertools.product([0,1,2,127,128,255,256,257,65536,0x1000000,0x80000000,MASK],range(256),[0,1,2]):case(kind=kind,campaign=campaign,stream=stream)
for length,style,stream in itertools.product(range(32),range(4),[0,1,2]):case(profile_length=length,profile_style=style,stream=stream)
for length,style in itertools.product(range(33),range(4)):case(kind=0x100,mission_length=length,mission_style=style)
for op,length,style in itertools.product([1,2],range(32),range(4)):case(op=op,profile_length=length,profile_style=style)
values=[0,1,2,127,128,255,256,257,65535,0x1000001,0x7fffffff,0x80000000,MASK]
for value in values:
 case(closeret=value,loadret=value,queryret=value,chooser=value,editor=value,setup=value,selection=value,player=value,progress=value)
 case(op=1,queryret=value,setup=value,selection=value,player=value,progress=value)
 for campaign in [0,1,2]:case(campaign=campaign,mode=value,dirty=value,flag=value,fallback=value)
def mutate(fn,phase,bits,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 for op,fn in enumerate([0x418510,0x4181e0,0x5169c0]):mutate(fn,phase,1|2|4|8|32|64,op=op)
 for fn in [0x561a0a,0x561994,0x4d51b0]:mutate(fn,phase,1|2|4|8|16|32|64)
 mutate(0x4cb6a0,phase,1|2|4|8|16|32|64,stream=0)
 mutate(0x5405b0,phase,1|2|4|8|16|32|64,kind=0x100)
 for fn in [0x41aa20,0x418140]:mutate(fn,phase,1|2|4|8|16|32|64,campaign=0)
 for fn in [0x561a0a,0x561994,0x4d51b0]:mutate(fn,phase,128,newpath_length=36)
 for length in [0,1,2,3,4,5,6,7,30,31]:
  mutate(0x5169c0,phase,8,profile_length=length,newprofile_length=31-length)
  mutate(0x5169c0,phase,16,profile_length=length,newpath_length=36-length)
 for fn in [0x561994,0x4d51b0]:mutate(fn,phase,1|2,profile_length=31,newkind=0,newcampaign=0)
for repeat in [2,3,5]:
 for kind,campaign,stream in [(0,1,1),(0,1,0),(0,0,1),(256,1,1)]:case(kind=kind,campaign=campaign,stream=stream,repeats=repeat,profile_length=31)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-replay-mission/Debug/replay_mission_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-replay-mission/Release/replay_mission_verify.exe')
variants=[(exe,False),(exe,True)]+([(optimized,False),(optimized,True)] if optimized else[])
oracles=[subprocess.Popen([str(path),'--oracle-plain-batch' if plain else '--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path,plain in variants]
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete replay callback418510, actual fallback4181E0 and unchanged real profile getter5169C0 against compiled traced/plain C. Full watched bytes/guards, exact filename bytes/hashes, ordered calls/arguments/results/state/writes, helper mutations, native data-read/write guards and native/C callee-saved registers/stack compared.','limitations':['Seven named deep boundaries: legacy fopen561A0A/fclose561994, load4D51B0, missing-file4CB6A0, mission5405B0, modal41AA20 and query418140. The production calls actual existing C bindings; this deterministic proof does not claim physical I/O, persistence, nested loaders or full campaign UI.','Profile names are NUL-terminated strings up to31bytes in the actual32-byte E38E74 UI field. Real getter adds save\\ (5bytes); replay adds \\ and19-byte autosave filename, at most57bytes including NUL in native100-byte buffer. No invented truncation or successful fake-I/O branch is added.','Helper-exit mutation is an explicit stress contract. Real stdio does not ordinarily modify const path arguments; path-mutation cases test that the existing same local filename reaches the loader after fclose. Byte campaign/mode/flags and full DWORD kind/editor/counter fields are preserved.']}
started=time.monotonic();checked=0;p=None
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,(o,(path,plain)) in enumerate(zip(oracles,variants)):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line);want=dict(expected,writes=[]) if plain else expected
   if actual!=want:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':want},indent=2)+'\n')
   assert actual==want,repr({'oracle':number,'memory':[actual['memory'],want['memory']],'event_counts':[len(actual['events']),len(want['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],want['events'])) if a!=b][:1],'paths':[actual['paths'],want['paths']],'writes':[actual['writes'],want['writes']]})
  checked+=1
  if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges];assert not missing,'Uncovered branches '+repr(missing)
  assert deep<=controlled_reached;assert len(mutated)==20,'Missing mutation phases'
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:
  try:o.stdin.close();o.wait(timeout=5)
  except Exception:o.kill()
prefix='' if args.production else 'artifacts/replay_mission_phase/'
sources=[prefix+s for s in ['runtime/replay_mission.c','src/unit_00222.c','include/replay_mission.h']]+['src/unit_02282.c','src/unit_02802.c','src/unit_02800.c','include/family_menu.h','include/restore_types.h','include/standalone_data.h','tools/verify_replay_mission_emulation.py']+['artifacts/replay_mission_phase/'+s for s in ['tests/replay_case.h','tests/replay_mission.c','tests/replay_actual_trace.c','tests/replay_actual_plain.c','project/CMakeLists.txt','retained-native.json']]
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges]
report.update(checked=checked,plain_checked=checked,comparison_checked=checked if optimized else 0,comparison_count=checked*len(variants),comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},native_instruction_coverage=[len(hits),len(instructions)],owned_instruction_coverage=[len(hits&owned),len(owned)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_filename_written_offsets=sorted(local_offsets),controlled_endpoints=[hex(a) for a in sorted(deep)],reached_controls=[hex(a) for a in sorted(controlled_reached)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/replay-mission-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_filename_written_offsets','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
