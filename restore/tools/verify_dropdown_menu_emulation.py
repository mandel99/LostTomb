"""Whole native dropdown/square-button graph and five exact menu latch callbacks."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if Path(__file__).resolve().parent.parent.name=='dropdown_menu_phase' else Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-dropdown-menu/out/Debug/restore_dropdown_menu_verify.exe');ap.add_argument('--debug-limit',type=int);ap.add_argument('--debug-start',type=int,default=0);args=ap.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/dropdown_menu_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s32=lambda v:struct.unpack('<i',struct.pack('<I',v&0xffffffff))[0];s16=lambda v:struct.unpack('<h',struct.pack('<H',v&65535))[0]
pe=u32(image,60);opt=pe+24;base=u32(image,opt+28);tableoff=opt+u16(image,pe+20);size=u32(image,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 q=tableoff+i*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
STOP=0x33000000;TIMER=STOP+0x100;DUMMY=STOP+0x200;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x200000)
def put(a,v,n=4):vm.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def get(a,n=4):return int.from_bytes(vm.mem_read(a&0xffffffff,n),'little')
exports=['metadata/dropdown-menu-native.json','metadata/dropdown-items-render-native.json','metadata/dropdown-menu-render-latches-native.json','metadata/dropdown-latch-code-windows-native.json'];instructions={};function_sets={};audit={}
def attach(entry,f):
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];function_sets[entry]=set()
 for x in a['items']:
  address=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;instructions[address]=x;function_sets[entry].add(address)
 audit[entry]={'instructions':len(a['items']),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in a['items']),'evidence':'complete Ghidra body, pinned bytes verified'}
for name in [exports[0],exports[2]]:
 d=json.loads((ROOT/name).read_text());assert d['native_sha256']==digest
 for entry,f in d['functions'].items():
  if entry!='00536990' and 'items' in f.get('assembly',{}):attach(entry,f)
d=json.loads((ROOT/exports[1]).read_text());attach('004d5f60',d['render']);tokens=set()
for item in d['items'].values():
 b=bytes.fromhex(item['hex']);assert bytes(vm.mem_read(int(item['address'],16),len(b)))==b
 tokens.update(u32(b,o+4) for o in range(0,len(b),16))
cs=Cs(CS_ARCH_X86,CS_MODE_32);raw_cfg={}
for entry,w in json.loads((ROOT/exports[3]).read_text())['windows'].items():
 lo=int(entry,16);raw=bytes.fromhex(w['hex']);assert bytes(vm.mem_read(lo,len(raw)))==raw;todo=[lo];found={}
 while todo:
  a=todo.pop()
  if a in found:continue
  assert lo<=a<lo+len(raw)
  q=next(cs.disasm(raw[a-lo:],a,count=1));found[a]={'address':f'{a:08x}','bytes':q.bytes.hex(),'instruction':q.mnemonic.upper()+' '+q.op_str}
  if q.mnemonic=='ret':continue
  if q.mnemonic=='jmp':todo.append(int(q.op_str,16))
  elif q.mnemonic.startswith('j'):todo.extend([int(q.op_str,16),a+q.size])
  else:assert q.mnemonic!='call';todo.append(a+q.size)
 function_sets[entry]=set(found);instructions.update(found);audit[entry]={'instructions':len(found),'bytes':sum(len(bytes.fromhex(q['bytes'])) for q in found.values()),'evidence':'exact reachable CFG from native entry; all successors bounded by pinned raw window; alignment and adjacent functions excluded'};raw_cfg[entry]=list(found.values())
regions=[(0x5c8f20,0x3b0),(0x705680,192),(0xe38dc0,128),(0xd3b188,32),(0xd3b508,16),(0xd3b5b0,32),(0xe945c8,48),(0xf56f80,64),(0x6efff8,208),(0x6f0ff8,224),(0x6f1ff8,144),(0xe38ea0,96),(0xe39080,16),(0x605a30,32)]
initial=bytes(vm.mem_read(0x5c8f20,0x3b0));put(0x56f22c,TIMER);p=[];events=[];last=None;executed=set();edges=set()
def table():return 0x5c91f0 if p[16]==0 else 0x5c9230 if p[16]==1 else 0x6f2000
def event(a,v):events.append([a,len(v)]+[x&0xffffffff for x in v]+[0]*(6-len(v))+[get(0xd3b198),get(0xd3b194),get(0xd3b190),get(0xe38dda,1),get(0x705730),get(0x70572c),get(0x705724),get(0x705720)])
def apply_mutation():
 m=p[26]
 if m&1:put(0x70571c,p[44])
 if m&2:put(0xe38dda,p[45],1)
 if m&4:put(0x705724,table()+p[46]*16)
 if m&8:put(0x705720,0x6f1000+p[47]*16)
 if m&16:put(0x6f1002,0x1234,2)
 if m&32:put(0xf56f8c,0xabcdef01);put(0xf56fa4,0x87654321)
 if m&64:put(0x6f000f,3,1)
latches={int(x,16) for x in raw_cfg};callsites={0x425ca9,0x425de3,0x425e16,0x425e45,0x425f78}
def hook(cpu,a,n,user):
 global last
 if a==STOP:return
 if last is not None:edges.add((last,a))
 previous=last;last=a;executed.add(a)
 if a in latches and previous in callsites:event(a,[])
 controlled=a==TIMER or a==DUMMY or a in tokens-latches or a in [0x536990,0x4ccbb0]
 if controlled:
  sp=cpu.reg_read(UC_X86_REG_ESP);nargs=4 if a==0x536990 else 6 if a==0x4ccbb0 else 0
  event(0x56f22c if a==TIMER else a,[get(sp+4+i*4) for i in range(nargs)])
  if a!=TIMER:apply_mutation()
  cpu.reg_write(UC_X86_REG_EAX,p[9] if a==TIMER else 0xa1b2c3d4);cpu.reg_write(UC_X86_REG_EIP,get(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 assert a in instructions,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,hook)
def prepare(case):
 global p,last
 p=case;events.clear();last=None
 for a,n in regions:vm.mem_write(a,bytes([(0xa5^p[29])&255])*n)
 vm.mem_write(0x5c8f20,initial)
 for i in range(8,48):put(0x5c9268+i,i%2 if p[40] else 0,1)
 for i in range(8):
  a=0x6f0000+i*24
  for off,v in [(0,p[23]+i*80),(2,p[24]),(4,0x9876),(6,p[22])]:put(a+off,v,2)
  put(a+8,p[25]);put(a+12,p[21],1);put(a+13,p[19],1);put(a+14,p[20],1);put(a+15,p[43] if i==1 and p[43] else p[18],1);put(a+16,p[27]+i);put(a+20,p[28]+i)
 for i in range(13):
  a=0x6f1000+i*16;put(a,i*20,2);put(a+2,i+1,2);put(a+4,p[25]);put(a+8,0xa1b2c3d4);put(a+12,p[27]+i)
 for i in range(8):
  a=0x6f2000+i*16;put(a,10+i*100,2);put(a+2,100+i*100,2);put(a+4,6,2);put(a+6,i+1,2);put(a+8,0x6f1000);put(a+12,p[42],2);put(a+14,0xb6c7,2)
 if p[41]:
  for i in range(8):
   a=0x5c91f0+i*16;put(a,10+i%4*100,2);put(a+2,100+i%4*100,2);put(a+4,6,2)
 for i in range(6):put(0xe945d0+i*4,p[31]+i)
 for a,v in [(0x705690,p[10]),(0x705730,p[11]),(0x7056a8,p[12]),(0x70572c,p[13]),(0x705728,p[15]),(0x705724,table()+p[17]*16),(0x705720,0x6f1000),(0xe38e34,p[4]),(0xe38e30,p[5]),(0x605a38,p[30]),(0x605a3c,p[39]),(0xf56f8c,p[33]),(0xf56fa4,p[34]),(0xe38eac,p[35]),(0xe38eec,p[37])]:put(a,v)
 for a,v in [(0xe38e03,p[6]),(0xe38e02,p[7]),(0xe38e00,p[8]),(0xe38dda,p[14]),(0xe38ec1,p[36]),(0xe38ec3,p[38]),(0xe39088,p[39])]:put(a,v,1)
entries=[0x425c40,0x425e80,0x425f20,0x426030,0x426090,0x4260d0,0x426100,0x426150,0x4d5f60,0x425430,0x403410,0x403490,0x403500,0x403540,0x40c7d0,0,0x4d5ef0]
def invoke(entry,argv):
 global last
 last=None;sp=STACK+0x1e0000;put(sp,STOP)
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,0x99aabbcc);vm.reg_write(UC_X86_REG_ECX,0x88aabbcc);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(entry,STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee-saved registers differ'
 return vm.reg_read(UC_X86_REG_EAX)
def native(case):
 prepare(case)
 argv={0:[p[2],p[3],0x6f0000,p[1]],1:[p[2],p[3],0x6f0000,p[1],p[32]],2:[table(),p[1]],3:[table()],6:[table(),p[1]],7:[0x6f1000,p[1],p[2],p[3]],8:[p[32]],16:[p[32]]}
 if p[0]==15:
  invoke(0x426030,[table()]);put(0xe38e03,0,1);result=invoke(0x425f20,[table(),p[1]]);invoke(0x425430,[])
 else:result=invoke(entries[p[0]],argv.get(p[0],[]))
 return {'result':result if p[0] in [0,2,3,6,7,8,15,16] else 0,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
fields=['op','count','x','y','mx','my','click','alt','held','now','old','selected','hover','row','mode','saved','table','pointer_index','kind','delay','repeat','enabled','size','left','top','callback','mutation','primary','secondary','canary','editorflag','width','index','font','highlightfont','mousehelp','warnings','autosave','pyramid','citynames','pattern','geometry','itemcount','secondkind','newlatch','newmode','newpointer','newitem']
default=[0]*48
for k,v in dict(count=4,mx=11,my=7,now=100,old=1,selected=1,hover=1,mode=3,saved=1,kind=4,size=30,callback=DUMMY,primary=0x12345678,secondary=0x87654321,width=10,font=0x12345678,highlightfont=0x87654321,geometry=1,itemcount=4,newlatch=1,newmode=7,newpointer=1,newitem=1).items():default[fields.index(k)]=v
cases=[]
def add(**kw):
 q=default.copy()
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(tuple(q))
for op,count in itertools.product([0,1,6,7],[-1,0,1,2,4,8]):
 for mx,my in itertools.product([-32769,-1,0,9,10,11,29,30,31,89,90,99,100,109,110,111,299,300,710,800,65536],[-32769,-1,0,5,6,7,11,12,17,18,29,30,31,44,45,46,51,255,65536]):
  if count==8 and op==7:pass
  add(op=op,count=count,mx=mx,my=my,table=2 if op==6 else 0,x=10 if op==7 else 0,y=6 if op==7 else 0)
for kind,delay,repeat,enabled,click,alt,held in itertools.product([0,2,3,4,5,255],[0,1,2,255],[0,1,7,8,46,47,48,254,255],[0,1,128],[0,1],[0,1],[0,1]):
 add(kind=kind,delay=delay,repeat=repeat,enabled=enabled,click=click,alt=alt,held=held,count=2)
for now,old,repeat,pattern in itertools.product([0,1,29,30,31,0x7fffffff,0x80000000,0xffffffff],[0,1,0x7ffffff0,0xfffffff0],[0,6,7,8,46,47,254,255],[0,1]):add(now=now,old=old,repeat=repeat,delay=2,held=1,pattern=pattern)
for x,y,size in itertools.product([-2147483648,-32768,-1,0,32767,2147483647],[-2147483648,-1,0,32767,2147483647],[-32768,-1,0,1,30,32767]):
 for op in [0,1]:add(op=op,x=x,y=y,size=size,count=2,mx=x,my=y)
for op in [0,1]:
 for left,top in itertools.product([-32768,-1,0,32767],[-32768,-1,0,32767]):add(op=op,left=left,top=top,count=2,mx=left,my=top,kind=2)
for index,width in itertools.product([-2147483648,-1,0,1,2,3,4,5,255,256,2147483647],[0,1,256,0x7fff,0x8000,0xffff,0x12345678,0x80000000,0xffffffff]):add(op=8,index=index,width=width)
for selected,hover,click,mode,saved in itertools.product([0,1,2,4,256],[0,1,256,0xffffffff],[0,1],[0,1,3,255],[0,1,0x12345678,0xffffffff]):add(op=3,selected=selected,hover=hover,click=click,mode=mode,saved=saved)
for op,saved in itertools.product([4,5],[0,1,3,256,0x12345678,0xffffffff]):add(op=op,saved=saved)
for op,table0,selected,pointer,row in itertools.product([2,9],[0,1,2],[0,1,2,3,4],[0,1,2,3],[0,1,2,4]):
 add(op=op,table=table0,selected=selected,pointer_index=pointer,row=row,mx=11,my=36,itemcount=4)
for table0,pointer in itertools.product([0,1],range(4)):
 data=json.loads((ROOT/exports[1]).read_text())['items'];baraddr=0x5c91f0+table0*64+pointer*16;bar=initial[baraddr-0x5c8f20:baraddr-0x5c8f20+16];count=u16(bar,12)
 for row in range(count+1):add(op=2,table=table0,pointer_index=pointer,selected=pointer+1,row=row,click=1)
for op,value in itertools.product(range(10,15),[0,1,2,3,255,256,257,0x12345678,0x80000000,0xffffffff]):add(op=op,mousehelp=value,warnings=value,autosave=value,pyramid=value,citynames=value)
for token,value in itertools.product(sorted(latches),[0,1,2,256,0xffffffff]):
 for op in [0,2]:add(op=op,table=2,click=1,row=1,callback=token,mousehelp=value,warnings=value,autosave=value,pyramid=value,citynames=value)
for mutation,newlatch,newmode in itertools.product([1,2,3,4,5,8,16,32,64,127],[0,1,256,0x12345678],[1,3,7,255]):
 for op in [0,2,9]:add(op=op,table=2,click=1,row=1,mutation=mutation,newlatch=newlatch,newmode=newmode,count=4,delay=1,kind=5)
for kind,secondkind,mutation in itertools.product([2,3,4,5],[2,3,4,5],[0,64]):add(kind=kind,secondkind=secondkind,mutation=mutation,delay=1,count=2,mx=85)
for selected,table0 in itertools.product([0,1,2,3,4],[0,1]):add(op=15,selected=selected,table=table0,click=1,hover=1,mx=111,my=7,row=0)
for op in range(15):add(op=op,canary=0x5a,table=2,row=1,click=1)
for editorflag,citynames in itertools.product([0,1,256,0xffffffff],[0,1,256]):add(editorflag=editorflag,citynames=citynames,mode=1,kind=2)
for index in [-2147483648,-1,0,1,2,3,4,5,6,7,8,9,10,11,256,2147483647]:add(op=16,index=index)
for mode in [0,1,2,255]:add(op=9,mode=mode)
for count in [-32768,-1,0,1,13]:add(op=9,table=2,itemcount=count)
for mode in [0,3,255]:add(editorflag=1,citynames=1,mode=mode,kind=2)
cases=list(dict.fromkeys(cases));cases=cases[args.debug_start:];cases=cases[:args.debug_limit] if args.debug_limit else cases
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':0,'stage':'production' if args.production else phase.as_posix(),'native_sha256':digest,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete native dropdown and square-button control flow plus full renderer and actual width selector; five raw native latch callback CFGs recovered, authenticated and executed. Compare ordered endpoint arguments, context, canary-bounded memory, callback mutation/reloads, exact pointer/WORD/BYTE contracts, native stack/callee-saved registers.','limitations':['Actual536990 panel tiles and4CCBB0 localized text raster remain named controlled endpoints. Their exact4/6 arguments and memory/callback ordering are compared; this suite does not claim their pixel algorithms.','The five option callbacks are actual composed bodies in this suite; other25 unique table callback actions are controlled deep endpoints here. Additional complete callbacks and current routing gaps are tracked separately in metadata/dropdown-action-status.json. No complete settings/editor/overseer/action semantics is claimed by this controller proof.','Clock is controlled at native timeGetTime IAT56F22C and matching C platform seam; real wall-clock timing and OS input delivery are outside deterministic equivalence.','Tables and full DWORD pointer targets are valid mapped native/fixture records. Null/invalid pointer or out-of-range selected-row native faults are outside equivalence, never converted to successful rendering.411 retains native selectedindex0->bar-16 ordering; sequence fixtures establish actual hover replacement before rendering.','Legacy construction1018/1022 callback-token gap remains outside this scope.']}
case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':48}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  for key in expected:
   if actual[key]!=expected[key]:
    detail=[actual[key],expected[key]]
    if key=='state':
     a=bytes.fromhex(actual[key]);b=bytes.fromhex(expected[key]);ix=next((i for i,(x,y) in enumerate(zip(a,b)) if x!=y),None);detail={'first_difference':ix,'C_bytes':len(a),'native_bytes':len(b),'C_slice':a[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':b[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    elif key=='events':
     ix=next((i for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b),min(len(actual[key]),len(expected[key])));detail={'first_difference':ix,'C_count':len(actual[key]),'native_count':len(expected[key]),'C_event':actual[key][ix:ix+1],'native_event':expected[key][ix:ix+1]}
    raise AssertionError(key+' differs '+str(detail))
  report['checked']+=1
 report['passed']=True
except Exception as e:report['error']=str(e);report['failed_case']=dict(zip(fields,case)) if case else None
finally:
 process.stdin.close();process.terminate();process.wait(timeout=10)
 # Conditional successor accounting follows exact native instruction lengths/targets.
 conditional=[];computed=[]
 for a,x in instructions.items():
  ins=next(cs.disasm(bytes.fromhex(x['bytes']),a,count=1));fall=a+ins.size
  if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':conditional.extend([(a,int(ins.op_str,16)),(a,fall)])
  if ins.mnemonic=='jmp' and 'ptr' in ins.op_str:computed.extend([(a,t) for t in [0x4d5f01,0x4d5f07,0x4d5f0d,0x4d5f13,0x4d5f19,0x4d5f1f,0x4d5f25]])
 report['instruction_coverage']={'executed':len(set(instructions)&executed),'total':len(instructions),'missing':[f'{a:08x}' for a in sorted(set(instructions)-executed)]}
 report['conditional_coverage']={'executed':sum(e in edges for e in conditional),'total':len(conditional),'missing':[[f'{a:08x}',f'{b:08x}'] for a,b in conditional if (a,b) not in edges]}
 report['computed_coverage']={'executed':sum(e in edges for e in computed),'total':len(computed),'missing':[[f'{a:08x}',f'{b:08x}'] for a,b in computed if (a,b) not in edges]}
 report['qualified_unreachable_edges']=[{'from':'00425d38','to':'00425d44','reason':'EDI index is reset to0 at425CC4 after every first-pass callback. Each iteration reaches425D38 with0<=EDI<count and positive signedDWORD count. INC EDI425D68 followed by signedJL425D6B admits no negative index: index cannot overflow before reaching signed count<=INT_MAX. CMP EDI,-1 at425D30 therefore cannot takeJLE425D38. No external call or index-aliasing store lies between second-pass reset/loop guard and this comparison; callbacks in that pass return immediately from425C40 rather than reenter its loop.','instructions':['00425cc4','00425cc6','00425cc8','00425d2d','00425d30','00425d38','00425d65','00425d68','00425d69','00425d6b']}]
 report['complete_suite']=not(args.debug_start or args.debug_limit)
 if report['passed'] and report['complete_suite']:
  missing_edges=set((int(a,16),int(b,16)) for a,b in report['conditional_coverage']['missing'])
  if report['instruction_coverage']['missing'] or report['computed_coverage']['missing'] or missing_edges!={(0x425d38,0x425d44)}:report['passed']=False;report['error']='Full feasible coverage incomplete'
 report['functions']=audit;report['raw_latch_CFGs']=raw_cfg
 paths=[phase/'include/dropdown_menu.h']+[phase/f'src/unit_{n:05d}.c' for n in [397,408,409,410,411,412,413,414,415,1843]]+[phase/f'runtime/{name}.c' for name in ['dropdown_menu_options','dropdown_menu_boundary','dropdown_menu_clock']]+[phase/'tests/dropdown_menu.c',phase/'tests/dropdown_menu_helpers.c',phase/'tools/verify_dropdown_menu_emulation.py',Path('src/unit_01842.c')]
 report['source_hashes']={q.as_posix():sha((ROOT/q).read_bytes()) for q in paths};report['evidence_hashes']={q:sha((ROOT/q).read_bytes()) for q in exports};report['native_callback_tokens']=[f'{t:08x}' for t in sorted(tokens)]
 out=ROOT/phase/'metadata/dropdown-menu-emulation.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'passed':report['passed'],'checked':report['checked'],'instructions':[report['instruction_coverage']['executed'],report['instruction_coverage']['total']],'edges':[report['conditional_coverage']['executed'],report['conditional_coverage']['total']],'computed':[report['computed_coverage']['executed'],report['computed_coverage']['total']]}))
 if not report['passed']:print(report.get('error'),report.get('failed_case'));sys.exit(1)
