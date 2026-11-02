"""Complete message input and actual button/callback composition against pinned native x86."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys,random
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-message-panel-input/out/Debug/restore_message_panel_input_verify.exe');parser.add_argument('--production',action='store_true');parser.add_argument('--debug-limit',type=int);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/message_panel_input_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s32=lambda x:struct.unpack('<i',struct.pack('<I',x&0xffffffff))[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a,n in [(BASE,0x10000),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
regions=[(0xd3a6e0,0xb00),(0xe38dc0,256),(0x7056a8,36),(0x5c7da8,736),(0x5e02c0,84),(BASE+0x4f8,128)];buttons=bytes(vm.mem_read(0x5c7da8,736));choices=bytes(vm.mem_read(0x5e02c0,84));tables=[0x5c7da8,0x5c7dd0,0x5c7df8,0x5c7e20,0x5c7e48,0x5c7e70,0x5c7ec0,0x5c7ee8,0x5c7f38,0x5c7f88,0x5c7fb0,0x5c7fd8,0x5c8050];action_tables=[0x5c7e48,0x5c7ec0,0x5c7f88,0x5c7fb0,0x5c7f38,0x5c7ee8,0x5c7fd8,0x5c8050]
exports=['metadata/message-panel-input-native.json','metadata/message-panel-omitted-callbacks-native.json','metadata/message-panel-hover-native.json','metadata/rich-message-text-next-native.json'];audit={};ranges=[]
def audit_body(entry,ins):
 for x in ins:assert bytes(vm.mem_read(int(x['address'],16),len(bytes.fromhex(x['bytes']))))==bytes.fromhex(x['bytes'])
 ranges.append((int(ins[0]['address'],16),int(ins[-1]['address'],16)+len(bytes.fromhex(ins[-1]['bytes']))));audit[entry]={'instructions':len(ins),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in ins)}
for entry,f in json.loads((ROOT/exports[0]).read_text())['functions'].items():assert not f['assembly'].get('has_more',False);audit_body(entry,f['assembly']['items'])
omitted=json.loads((ROOT/exports[1]).read_text())
for entry,f in omitted['callbacks'].items():
 ins=f['instructions'];stop=next(i for i,x in enumerate(ins) if x['instruction'].strip()=='ret');audit_body(entry,ins[:stop+1]);assert bytes(vm.mem_read(int(entry,16),80))==bytes.fromhex(f['region_bytes'])
for entry,f in omitted['button_regions'].items():assert bytes(vm.mem_read(int(entry,16),40))==bytes.fromhex(f['region_bytes'])
audit_body('00425bc0',json.loads((ROOT/exports[2]).read_text())['assembly']['items']);audit_body('004c8040',json.loads((ROOT/exports[3]).read_text())['functions']['004c8040']['assembly']['items'])
ranges += [(0x425990,0x425bc0),(0x4261d0,0x4264c4),(0x4cde40,0x4cde82),(0x47d350,0x47d353),(0x51f780,0x51f781)]
assert bytes(vm.mem_read(0x47d350,3))==bytes.fromhex('33c0c3') and byte(0x51f780)==0xc3
callbacks={read(a+16) for a in tables}|{read(a+20) for a in tables}|{u32(choices,i*28+8) for i in range(3)}|{u32(choices,i*28+12) for i in range(3)};owned={0x4c7f30,0x4c7e90,0x4c7ee0};controlled=callbacks-owned-{0x51f780}
counts={0x425990:4,0x4261d0:4,0x426450:4,0x4c8040:0,0x4cde40:2,0x4cde60:2,0x47d350:1,0x539170:0};events=[];p=[];reached=set()
probes={0x4c7786:'fullWORD mandatory buttons',0x4c7799:'Escape close',0x4c77cc:'media choice hover',0x4c77ea:'media choice actual buttons',0x4c7816:'media action1/2/4/5/7',0x4c78df:'media default action',0x4c790e:'media OK button',0x4c791e:'media no-hit cursor reset',0x4c7963:'normal history tooltip',0x4c7998:'normal history button',0x4c79ff:'normal choice actual buttons',0x4c7a55:'normal OK tooltip',0x4c7a96:'normal actual OK button',0x4c7aef:'scroll up actual button',0x4c7b60:'scroll down actual button',0x4c7b6c:'drag controller',0x4c7b79:'link selection',0x4c7b96:'history push DWORD message',0x4c7bab:'history count DWORD',0x4c7bb1:'link target fullDWORD',0x4c7c0b:'normal action1/2/3/4/5/6/7',0x4c7d19:'normal default action',0x4c7d68:'subtype1/7 extra tooltip',0x4c7d87:'subtype1/7 extra button',0x4c7dba:'drag nonpositive maximum',0x4c7df4:'drag left bound',0x4c7dff:'drag right bound',0x4c7e16:'drag top bound',0x4c7e22:'drag bottom bound',0x4c7e35:'actual percent helper',0x4c7e41:'actual scale helper',0x4c7e6f:'negative thumb clamp',0x4c7f39:'media cleanup',0x4c7f43:'fullDWORD media clear',0x4c7f52:'fullDWORD action clear',0x4c7fb9:'inclusive hyperlink rectangle',0x4c7eba:'scroll DWORD change',0x4c7ebf:'scroll clear links',0x4c7ee9:'history back fullDWORD records',0x425c30:'tooltip fullDWORD output'}
def event(a,argv):
 row=[a,len(argv)]+argv+[0]*(20-2-len(argv));row[8:19]=[read(0xd3b198),read(0xd3b194),read(0xd3b124),read(0xd3b130),read(0xd3aae0),read(0xd3a6ec),byte(0xe38dda),byte(0xe38dca),read(0xd3b180),read(0xd3a6e8),int.from_bytes(vm.mem_read(0xd3b186,2),'little')];events.append(row)
def code(cpu,a,n,user):
 if a==STOP:return
 if a in probes:reached.add(a)
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if a in callbacks:
  dynamic=0x425990<=read(sp)<0x425bc0 or 0x4261d0<=read(sp)<0x426450
  if dynamic:event(a,[])
  if a in owned or a==0x51f780:return
  assert a in controlled and dynamic,'Uncontrolled callback '+hex(a)
  cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 if a in counts:
  argv=[read(sp+4+i*4) for i in range(counts[a])];event(a,argv)
  if a!=0x539170:return
  if p[29]&1:write(0xd3b17c,p[30],1)
  if p[29]&2:write(0xd3a6e8,p[30])
  cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 assert any(lo<=a and a+n<=hi for lo,hi in ranges),'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,code)
def native(params):
 global p
 p=params;events.clear();op,ident,kind,subtype,action,mask,media,x,y,cols,rows,top,viscols,visrows,first,maximum,history,links,selector,flags,target,dx,dy,hover,linkid,previous,index,style,pressed,mutation,value,_=p;x=s32(x);y=s32(y);cols=s32(cols);rows=s32(rows);top=s32(top);viscols=s32(viscols);visrows=s32(visrows)
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(0x5c7da8,buttons);vm.mem_write(0x5e02c0,choices);record=0xe209b8+ident*80;vm.mem_write(record-8,b'\xa5'*96)
 for off,v in [(0,kind),(2,subtype),(10,cols),(12,rows)]:write(record+off,v,2)
 for a,v in [(0xd3b124,ident),(0xd3a6e8,action),(0xd3b180,media),(0xd3b160,x),(0xd3b15c,y),(0xd3b154,x+16),(0xd3b150,top),(0xd3b14c,viscols),(0xd3b148,visrows),(0xd3b130,first),(0xd3b134,maximum),(0xd3aae0,history),(0xd3a6ec,links),(0xd3a6f0,links),(0xd3b198,selector),(0xd3b194,0x12345678)]:write(a,v)
 write(0xd3b186,mask,2);write(0xd3b17c,previous,1);write(0xe38dda,5,1)
 for i in range(200):write(0xd3aae4+i*4,256+i);write(0xd3ae04+i*4,300+i)
 for i in range(50):
  row=0xd3a6f8+i*20
  for off,v in [(0,x+50+i*40),(4,x+80+i*40),(8,y+100),(12,y+120),(16,linkid+i)]:write(row+off,v)
 for a in tables:
  for off,v in [(24,0 if style&1 else 1),(25,pressed),(26,hover),(27,1 if style&2 else 0)]:write(a+off,v,1)
 bottom=y+struct.unpack('<h',struct.pack('<H',rows&65535))[0]*16-36;sx=x+16+viscols*16-1;mx=my=-10000
 if target==1:mx,my=(x+448,y+422) if media else(x+struct.unpack('<h',struct.pack('<H',cols&65535))[0]*16-26,bottom+12)
 elif target==2:
  b=bytes(vm.mem_read(action_tables[action if action<=7 else 0],8));w,h=struct.unpack_from('<hh',b,4);mx=x+16+int(w/2);my=(y+408 if media else bottom)+int(h/2)
 elif target==3:mx,my=sx+19,top+13
 elif target==4:mx,my=sx+19,top+visrows*16-13
 elif target==5:mx,my=sx+20,top+int(visrows*16/2)
 elif target==6:mx,my=x+50+index*40,y+100
 elif target==7:mx,my=x+31,bottom+10
 elif target==8:mx,my=x+76,top+48
 elif target in [9,10,11]:mx,my=x+103+(target-9)*134,y+292
 if op==6:mx,my=x+target*50,y
 write(0xe38e34,mx+dx);write(0xe38e30,my+dy)
 for bit,a in enumerate([0xe38df8,0xe38e03,0xe38e00,0xe38e04,0xe38df7,0xe38e02,0xe38df4,0xe38df6]):write(a,1 if flags&(1<<bit) else 0,1)
 for i in range(3):
  a=BASE+0x500+i*36;vm.mem_write(a,bytes(36));write(a,i*50,2);write(a+4,24,2);write(a+6,24,2)
 sp=STACK+0x1e0000;write(sp,STOP)
 if op==6:
  for i,v in enumerate([x,y,BASE+0x500,index,selector]):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x4c7770,0x4c7db0,0x4c7f30,0x4c7f70,0x4c7e90,0x4c7ee0,0x425bc0][op],STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 assert read(0xd3a6f4)==0xa5a5a5a5,'Link prefix guard changed'
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)+bytes(vm.mem_read(record-8,96))
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op in [1,3] else 0,'state':state.hex(),'events':events.copy()}
fields=['op','ident','kind','subtype','action','mask','media','x','y','cols','rows','top','viscols','visrows','first','maximum','history','links','selector','flags','target','dx','dy','hover','linkid','previous','index','style','pressed','mutation','value','spare'];default=[0,249,2,4,0,0,0,192,144,40,30,220,36,23,0,20,0,0,0,0,0,0,0,0,310,1,0,0,0,0,0,0];cases=[]
def add(**kw):
 q=default.copy()
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(q)
for op in range(7):add(op=op)
for media,target,flags in itertools.product([0,1,256],[0,1,2,3,4,5,6,7,8,9,10,11],[0,1,2,4,6,8]):add(media=media,target=target,flags=flags,links=1,history=1,kind=0 if target==7 else 2,subtype=1 if target==8 else 4)
for action,mask,media,kind in itertools.product([0,1,2,3,4,5,6,7,8,256,264],[0,1,256,0x8000],[0,1,256],[0,1,2]):add(action=action,mask=mask,media=media,kind=kind,target=0)
for action,media in itertools.product(range(10),[0,1]):add(action=action,media=media,target=2,flags=6)
for mask,target,flags in itertools.product([1,2,4,256,0x8000],[9,10,11,0],[0,1,2,6,8]):add(action=8,mask=mask,target=target,flags=flags)
for ident,offset,target in itertools.product([1,249,256,499],[(0,0),(192,144),(512,400),(-300,-100)],[1,3,4,5,6,7]):add(ident=ident,x=offset[0],y=offset[1],top=offset[1]+76,target=target,flags=6,links=3,history=3,kind=0 if target==7 else 2)
for selector,first,maximum in itertools.product([0,1,2,256],[-1,0,1,255,256,999],[0,1,256,999]):add(op=4,selector=selector,first=first,maximum=maximum,links=50)
for history in [0,1,2,198,199,0x80000000,0xffffffff]:add(op=5,history=history,links=50)
for links,index,dx,dy in itertools.product([0,1,3,50],[0,1,2,49],[-1,0,30,31],[-1,0,20,21]):add(op=3,links=links,index=index,target=6,flags=2,dx=dx,dy=dy)
for history in [0,1,198,199,256]:add(history=history,target=6,flags=2,links=1,first=256)
for maximum,target,dx,dy in itertools.product([0,1,256,999],[5],[-20,-19,-18,20,21,22,23],[-200,0,200,201]):add(op=1,maximum=maximum,target=target,flags=4,dx=dx,dy=dy)
for rows in [3,4,23,30,256]:add(op=1,visrows=rows,target=5,flags=4)
for x,y,index,target,dx,dy in itertools.product([0,192,512,-300],[0,144,400],[0,1,3],[0,1,2],[-1,0,23,24],[-1,0,23,24]):add(op=6,x=x,y=y,index=index,target=target,dx=dx,dy=dy,selector=300)
for media,previous,mutation in itertools.product([0,1,256],[0,1,73,255],[0,1,2,3]):add(op=2,media=media,previous=previous,mutation=mutation,value=48,action=0x12345678)
for hover,pressed,style in itertools.product([0,1,2,128],[0,1,20],[0,1,2,3]):add(target=1,flags=6,hover=hover,pressed=pressed,style=style)
for maximum in [65536,0x1000000]:add(op=1,maximum=maximum,target=5,flags=4)
for selector,first in [(0,65536),(1,65535),(1,65536)]:add(op=4,selector=selector,first=first,maximum=65536)
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'stage':'production' if args.production else str(phase).replace('\\','/'),'native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete message input, drag, close, hyperlink hit, omitted scroll/history callbacks and DWORD tooltip helper versus handwritten C. Actual original/C425990+helpers,4261D0/426450, link clear, percent/scale and zero bodies are composed, with real immutable descriptor callbacks routed to restored implementations; full guard bytes, ordered engine args/callback state, returns, native stack and saved registers compared.','limitations':['Other game-action callbacks40A0E0,4E2360/4E2380 and44EE30/44EEA0/44EEF0, plus media cleanup539170, are named controlled deep endpoints. This proves their ordered selection/arguments and controller effects, not their full gameplay/media algorithms.','Caller-produced history entries lie in0..199 and links0..50; negative native counter guards and safe saturated history push are additionally checked. Native unchecked invalid record IDs/cursor invariants are not converted into successful no-op actions.','Drag fixtures use native panel geometry and maximum scroll up to0x1000000; the unchanged scale helper multiplication therefore fits signed32bit. Original unsigned wrap is explicit in all newly rewritten geometry/counter arithmetic.','No framebuffer drawing or actual city input replay is claimed here; root performs the owned-C diagnostic after completed mode5 capture.']}
checked=0;case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':32}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  for k in expected:
   if actual[k]!=expected[k]:
    detail=[actual[k],expected[k]]
    if k=='state':
     aa=bytes.fromhex(actual[k]);bb=bytes.fromhex(expected[k]);ix=next((i for i,(a,b) in enumerate(zip(aa,bb)) if a!=b),None);detail={'first_difference':ix,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':bb[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    raise AssertionError(k+' differs '+str(detail))
  checked+=1
  if checked%1000==0:print('checked',checked,file=sys.stderr,flush=True)
 if not args.debug_limit:assert set(probes)<=reached,'Missing native probes '+str([hex(a)+':'+probes[a] for a in set(probes)-reached])
 report.update(passed=True,checked=checked,complete_case_set=not bool(args.debug_limit))
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['branch_probes']={hex(a):{'name':name,'reached':a in reached} for a,name in probes.items()};report['native_byte_audit']=audit;report['newly_restored_native_instructions']=sum(v['instructions'] for k,v in audit.items() if k!='004c8040');report['native_bytes_verified']=sum(v['bytes'] for v in audit.values());report['smoke']={'passed':True,'output':smoke.stdout.strip()};report['callback_addresses']={hex(a):'restored' if a in owned else 'actual bare ret' if a==0x51f780 else 'controlled deep game action' for a in sorted(callbacks)}
sources=[str(phase/'include/message_panel_input.h').replace('\\','/'),str(phase/'runtime/message_panel_input.c').replace('\\','/')]+[str(phase/f'src/unit_{n:05d}.c').replace('\\','/') for n in [407,1626,1627,1628,1629]]+['tests/message_panel_input.c','tests/message_panel_input_buttons.c','tools/verify_message_panel_input_emulation.py','include/message_panel.h','include/menu_input.h','include/rich_message_text.h']+[f'src/unit_{n:05d}.c' for n in [404,405,406,416,417,1631,1730,1731,1130]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports};name='message-panel-input'+('' if args.production else '-staged')+('-debug' if args.debug_limit else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
