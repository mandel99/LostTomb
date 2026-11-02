"""Whole unchanged load/save/delete/quit entries with actual profile/list/buffer/caret C bodies."""
from pathlib import Path
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;PHASE=ROOT/'artifacts/menu_existing_actions_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path);args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff;ALT=0x31000000;STOP=0x33000000;STACK=0x34000000
schema=(PHASE/'tests/existing_case.h').read_text();names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum{',1)[1].split('};',1)[0]) if x!='FIELDS'];FIELDS=len(names);globals().update({'P_'+name.upper():j for j,name in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(ALT,0x5000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return (v&0x7fffffff)-(v&0x80000000)
def string(a):
 b=bytearray()
 for j in range(260):
  c=read(a+j,1)
  if not c:return bytes(b)
  b.append(c)
 raise AssertionError('Unterminated bounded string')
def text_hash(a):
 h=2166136261
 for v in string(a):h=((h^v)*16777619)&MASK
 return h
def make_text(n,style,max_length):
 alphabets=[b'ABCDEFGHIJKLMNOPQRSTUVWXYZ',b'abcdefghijklmnopqrstuvwxyz0123456789',b'Ab 9_-.',b'0123456789'];assert n<=max_length and style<4;a=alphabets[style];return bytes(a[j%len(a)] for j in range(n))+b'\0'
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
functions={int(a,16):f['assembly']['items'] for a,f in packet['functions'].items()}
instructions={int(i['address'],16):i for body in functions.values() for i in body};decoded={};branches={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
for a,i in instructions.items():
 encoded=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(encoded)))==encoded;d=next(cap.disasm(encoded,a));decoded[a]=d
 if d.mnemonic.startswith('j') and d.mnemonic!='jmp':branches[a]=(a+d.size,int(d.op_str,16))
for f in packet['functions'].values():assert not f['assembly']['has_more']
assert len(instructions)==424
entries=[0x402450,0x402540,0x402630,0x402bb0,0x4cdff0,0x5169c0,0x40f8f0,0x4cc500,0x4cc3c0,0x4cc3e0,0x4ce3a0];primary={int(i['address'],16) for fn in entries[:4] for i in functions[fn]}
regions=[(0xe38db0,0x130),(0xe370a0,0xd0),(0xe20980,0x30),(0xd3b1a0,0x30),(0x6057b0,0x2b0),(0xeade90,0x80),(0xe2078c,0x1c0),(0xe36ee0,0x30),(ALT,0x5000)]
immutable=[(0x5e0494,6),(0x5761c4,7),(0x576260,11)]
filenames=[b'Alpha.sav',b'Beta.extra.sav',b'.hidden.sav',b'no_extension',b'Gamma.SAV',b'zeta.sav',b'  spaced.name.sav',b'Last.']
events=[];paths=[];frames=[];hits=set();edges=set();mutated=set();controlled_reached=set();params=None;seen=0;last_block=None;block_cache={};fill_cache={};deep={0x41c740,0x41ca10}
def path_event(fn,phase,a):paths.append([fn,phase,string(a).hex()])
def observe(fn,phase,values,r=0):
 global seen
 p=params;e=[fn,phase]+list(values)+[0]*(4-len(values))+[r]
 e +=[read(a,n) for a,n in [(0xe38dda,1),(0xe38dca,1),(0x6059e8,4),(0xe20998,4),(0xd3b1bc,4),(0xd3b1c0,1),(0xe20990,4),(0x6057d0,4),(0x6059f8,4),(0x6059f4,4),(0x6059f0,4),(0xd3b1b3,1),(0x6059ec,4)]]
 e +=[text_hash(a) for a in [0xe38e74,0xeadea4,0xe370bc,0xe37100]]+[len(string(0xe370bc)),len(string(0xe37100))]+[read(a) for a in [0xe207f4,0xe208a8,0xe208f8,0xe207f8,0xe208ac,0xe208fc]]
 assert len(e)==32;events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   bits=p[P_MBITS];mutated.add((fn,phase))
   if bits&1:vm.mem_write(0xe38e74,make_text(p[P_NEWPROFILE_LENGTH],p[P_NEWPROFILE_STYLE],31))
   if bits&2:vm.mem_write(0xe370bc,make_text(p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63))
   if bits&4:vm.mem_write(0xe37100,make_text(p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63))
   if bits&8:
    for a,v,n in [(0xe38dda,p[P_NEWMODE],1),(0xe38dca,p[P_NEWDIRTY],1),(0x6059e8,p[P_NEWCHOOSER],4),(0xe20998,p[P_NEWVALUE],4),(0xd3b1bc,p[P_NEWBACKMODE],4),(0xd3b1c0,p[P_NEWACTION],1)]:write(a,v,n)
   if bits&16:
    assert p[P_NEWINDEX]<20
    for a,v in [(0xe20990,p[P_NEWINDEX]),(0xe208fc,p[P_NEWMAX]),(0xe208ac,p[P_NEWLENGTH]),(0xe207f8,p[P_NEWCURSOR])]:write(a,v)
   if bits&32:assert signed(p[P_NEWFILECOUNT])<=8;write(0x6057d0,p[P_NEWFILECOUNT])
   if bits&64:vm.mem_write(0xeadea4,make_text(p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63))
def values_for(fn,raw):
 if fn==0x4cdff0:return [text_hash(raw[0]),len(string(raw[0])),raw[1]&255]
 if fn==0x40f8f0:return [raw[0],raw[1],raw[2]&255]
 return raw
def enter(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);count=2 if a==0x4cdff0 else 3 if a==0x40f8f0 else 1 if a in [0x4cc500,0x4ce3a0] else 0;raw=[read(sp+4+j*4) for j in range(count)];frames.append((a,sp,raw))
 if a==0x4cdff0:path_event(a,0,raw[0])
 observe(a,0,values_for(a,raw))
def leave(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native RET frame';fn,old,raw=frames.pop()
 if fn==0x4cdff0:path_event(fn,1,raw[0])
 observe(fn,1,values_for(fn,raw),cpu.reg_read(UC_X86_REG_EAX) if fn==0x5169c0 else 0)
def endpoint(cpu,a,n,user):
 p=params;sp=cpu.reg_read(UC_X86_REG_ESP);self=cpu.reg_read(UC_X86_REG_ECX);assert self==0x6057c8;controlled_reached.add(a)
 if a==0x41c740:
  pattern=read(sp+4);flag=read(sp+8)&255;assert signed(p[P_FILECOUNT])<=8;path_event(a,0,pattern);observe(a,0,[self,text_hash(pattern),len(string(pattern)),flag])
  for off,v in [(4,ALT+0x1000),(8,p[P_FILECOUNT]),(12,8),(16,ALT+0x2000),(20,512),(24,p[P_FILECOUNT]*64 if signed(p[P_FILECOUNT])>0 else 0)]:write(self+off,v)
  for j,name in enumerate(filenames):write(ALT+0x1000+j*4,j*64);vm.mem_write(ALT+0x2000+j*64,name+b'\0')
  r=p[P_ENUMRET]&255;path_event(a,1,pattern);observe(a,1,[self,text_hash(pattern),len(string(pattern)),flag],r);cleanup=8
 else:
  compare=read(sp+4);assert compare==0x562370;observe(a,0,[self,compare]);observe(a,1,[self,compare]);r=0;cleanup=4
 cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
def block(cpu,a,n,user):
 global last_block
 if a in deep:return
 assert a in instructions,'Native escaped '+hex(a)
 if last_block is not None:edges.add((last_block,a))
 if(a,n) not in block_cache:block_cache[a,n]=sorted(x for x in instructions if a<=x<a+n)
 values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions),'Unwatched native write '+hex(a)
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
 global params,seen,last_block
 params=p;seen=0;events.clear();paths.clear();frames.clear()
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 assert p[P_INDEX]<20 and signed(p[P_FILECOUNT])<=8 and p[P_SIZE]<=80
 for a,v,n in [(0xe38dda,p[P_INITIAL_MODE],1),(0xe38dca,p[P_DIRTY],1),(0x6059e8,p[P_CHOOSER],4),(0xe20998,p[P_VALUE],4),(0xd3b1bc,p[P_BACKMODE],4),(0xd3b1c0,p[P_ACTION],1),(0xe20990,p[P_INDEX],4)]:write(a,v,n)
 vm.mem_write(0xe38e74,make_text(p[P_PROFILE_LENGTH],p[P_PROFILE_STYLE],31));vm.mem_write(0xe370bc,make_text(p[P_NAME_LENGTH],p[P_NAME_STYLE],63));vm.mem_write(0xe37100,b'EnteredPrevious\0');vm.mem_write(0xeadea4,b'previous-profile-path\0');vm.mem_write(ALT+0x3a00,b'outer-unused\0')
 for j in range(20):
  for a,v in [(0xe2079c+j*4,ALT+0x3000+j*100),(0xe208f0+j*4,p[P_MAX]),(0xe208a0+j*4,p[P_LENGTH]),(0xe207ec+j*4,p[P_CURSOR])]:write(a,v)
  vm.mem_write(ALT+0x3000+j*100,make_text(p[P_NAME_LENGTH],p[P_NAME_STYLE],63))
 write(0xe207a4,0xe37100)
 for a,v in [(0x6057cc,ALT+0x1000),(0x6057d0,p[P_FILECOUNT]),(0x6057d4,8),(0x6057d8,ALT+0x2000),(0x6057dc,512),(0x6057e0,p[P_FILECOUNT]*64 if signed(p[P_FILECOUNT])>0 else 0)]:write(a,v)
 for j,name in enumerate(filenames):write(ALT+0x1000+j*4,j*64);vm.mem_write(ALT+0x2000+j*64,name+b'\0')
 fn=entries[p[P_OP]];top=[ALT+0x3a00,p[P_MAPS]] if fn==0x4cdff0 else[ALT+0x4000,p[P_SIZE],p[P_FILL]] if fn==0x40f8f0 else[p[P_INDEX]] if fn==0x4cc500 else[ALT+0x2000] if fn==0x4ce3a0 else[]
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp-0x800,b'\xcc'*0xa00);write(sp,STOP)
  for j,value in enumerate(top):write(sp+4+j*4,value)
  for reg,value in saved:vm.reg_write(reg,value)
  for reg,value in [(UC_X86_REG_EAX,0xa5a55a5a),(UC_X86_REG_ECX,0x12345678),(UC_X86_REG_EDX,0x87654321),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  last_block=None;vm.emu_start(fn,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames;assert vm.reg_read(UC_X86_REG_ESP)==sp+4;assert all(vm.reg_read(reg)==value for reg,value in saved);assert bytes(vm.mem_read(sp+4+len(top)*4,128))==b'\xcc'*128,'Native caller argument guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'memory':h.hexdigest(),'events':events.copy(),'paths':paths.copy()}
default=[0]*FIELDS;defaults={'seed':1,'filecount':3,'profile_length':7,'name_length':12,'initial_mode':1,'dirty':0xa5,'chooser':0x12345678,'value':0x87654321,'backmode':0x89abcdef,'action':0x7f,'index':2,'max':32,'cursor':0x12345678,'length':0x87654321,'repeats':1,'enumret':1,'mocc':1,'newprofile_length':31,'newprofile_style':1,'newname_length':63,'newname_style':2,'newmode':53,'newdirty':0x77,'newchooser':0x76543210,'newvalue':0xabcdef01,'newbackmode':0xfedcba98,'newaction':255,'newindex':3,'newmax':MASK,'newlength':0x80000100,'newcursor':0x12345678,'newfilecount':8}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
for op in range(11):case(op=op)
for op,profile_length,name_length,filecount in itertools.product(range(4),[0,1,2,3,4,7,30,31],[0,1,2,3,4,7,31,32,62,63],[-1,0,1,8]):case(op=op,profile_length=profile_length,name_length=name_length,filecount=filecount)
for op,style,lengthvalue in itertools.product(range(4),range(4),range(32)):case(op=op,profile_length=lengthvalue,profile_style=style,name_style=style,name_length=lengthvalue*2)
for op,v in itertools.product(range(4),[0,1,2,127,128,255,256,257,0x1000001,0x7fffffff,0x80000000,MASK]):case(op=op,initial_mode=v,dirty=v,chooser=v,value=v,backmode=v,action=v,cursor=v,length=v)
for maps,filecount,profile_length in itertools.product([0,1,255,256],[-1,0,1,2,8],[0,1,3,4,7,31]):case(op=4,maps=maps,filecount=filecount,profile_length=profile_length)
for index,maxvalue,lengthvalue in itertools.product(range(20),[-1,0,1,7,31,32,63,64,256,0x7fffffff,0x80000000],[0,1,7,31,32,63]):case(op=7,index=index,max=maxvalue,name_length=lengthvalue)
for index,value in itertools.product(range(20),[0,1,255,256,65535,0x1000001,0x80000000,MASK]):case(op=9,index=index,length=value)
for size,fill in itertools.product(range(81),[0,1,127,128,255,256,0x1000001,MASK]):case(op=6,size=size,fill=fill)
def mutate(fn,phase,bits=8,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 for op,fn in enumerate(entries):mutate(fn,phase,op=op)
 for fn in deep:mutate(fn,phase,op=0)
 for op in range(3):
  for fn in [0x5169c0,0x4cdff0,0x40f8f0,0x4cc500,0x4cc3c0,0x4cc3e0]:mutate(fn,phase,bits=1|2|8|16,op=op)
  mutate(0x5169c0,phase,bits=1|64,op=op,mocc=1,newprofile_length=31)
  mutate(0x5169c0,phase,bits=1,op=op,mocc=2,newprofile_length=1)
  mutate(0x40f8f0,phase,bits=2|4,op=op,newname_length=63)
  mutate(0x4cc3c0,phase,bits=16,op=op,newindex=3,newlength=0x80000100)
 for fn in deep:mutate(fn,phase,bits=32,op=0,newfilecount=8)
for op in range(4):case(op=op,repeats=3,profile_length=31,name_length=63)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-menu-existing-actions/Debug/menu_existing_actions_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-menu-existing-actions/Release/menu_existing_actions_verify.exe')
variants=[exe]+([optimized] if optimized else[]);oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in variants]
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Whole unchanged402450/402540/402630/402BB0 entries plus actual unchanged profile, selector/trim, memset and three caret algorithms against compiled plain C. Full guarded watched bytes, exact pattern bytes/hash, ordered meaningful helper calls/state/results, declared helper mutations, native read/write guards and native/C saved registers/stack compared.','limitations':['Only filesystem enumeration41C740 and sorting41CA10 are controlled algorithm boundaries; the C product still calls real existing bindings. Physical filesystem and full subsequent load/save/delete execution are outside this four-entry proof.','Stable valid file table with0..8 entries or nonpositive count. The known selector invalid-index fallback is infeasible here; no list-count mutation is applied during actual trim.','Registry indexes are0..19 and pointer/count/cursor/max arrays are distinct valid fields.4CC500 native captures max at entry; current C may reload max internally, equivalent in this stable nonaliased pure-helper domain. No asynchronous mutation or invalid registry aliasing is claimed.','All tested C algorithms are actual unchanged source, without production trace hooks. Store ordering is compared at helper boundaries and in final watched bytes; this proof does not claim exact instruction-for-instruction internal C store order. Unrelated row/input functions sharing load_save_menu.c are outside the graph and have aborting test-only link sentinels.']}
started=time.monotonic();checked=0;p=None;dead_instructions={0x4ce0ad};dead_edges={(0x4ce090,0x4ce0ad),(0x4ce098,0x4ce0ad)}
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,o in enumerate(oracles):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
   if actual!=expected:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
   assert actual==expected,repr({'oracle':number,'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1],'paths':[actual['paths'],expected['paths']]})
  checked+=1
  if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)-dead_instructions<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-dead_instructions-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges and(a,t) not in dead_edges];assert not missing,'Uncovered branches '+repr([(hex(a),hex(t)) for a,t in missing]);assert deep<=controlled_reached;assert len(mutated)==26
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:
  try:o.stdin.close();o.wait(timeout=5)
  except Exception:o.kill()
sources=['src/unit_00020.c','src/unit_00024.c','runtime/load_save_menu.c','src/unit_00119.c','src/unit_01682.c','src/unit_01683.c','src/unit_01686.c','src/unit_02282.c','src/unit_01739.c','src/unit_01745.c','include/restore_types.h','include/load_save_menu.h','include/file_list.h','include/menu_input.h','include/standalone_data.h','tools/verify_menu_existing_actions_emulation.py']+['artifacts/menu_existing_actions_phase/'+s for s in ['tests/existing_case.h','tests/menu_existing_actions.c','tests/existing_actual_entries.c','tests/existing_actual_loadsave.c','tests/existing_actual_helpers.c','tests/unreached_link_sentinels.c','project/CMakeLists.txt','retained-native.json']]
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges]
report.update(checked=checked,comparison_checked=checked if optimized else 0,comparison_count=checked*len(variants),comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},native_instruction_coverage=[len(hits),len(instructions)],reachable_instruction_coverage=[len(hits-dead_instructions),len(set(instructions)-dead_instructions)],primary_instruction_coverage=[len(hits&primary),len(primary)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],feasible_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2-len(dead_edges)],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},controlled_endpoints=[hex(a) for a in sorted(deep)],reached_controls=[hex(a) for a in sorted(controlled_reached)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/menu-existing-actions-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
