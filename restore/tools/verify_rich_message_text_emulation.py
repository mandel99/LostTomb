"""Native rich-message layout, glyphs, links and composed raster versus staged/production C."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-rich-message-text/out/Debug/restore_rich_message_text_verify.exe');parser.add_argument('--production',action='store_true');parser.add_argument('--debug-limit',type=int);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/rich_message_text_phase')
sha=lambda b:hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;SRC=0x31000000;DST=0x31100000;STOP=0x33000000;STACK=0x34000000
for a,n in [(BASE,0x10000),(SRC,0x10000),(DST,0x100000),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
regions=[(0xd3a6e0,4096),(0xe20758,80),(0xe34394,216),(0x1265174,16),(0xf55bfc,12),(0xf56f7c,12)]
texts=[b'',b'ABC DEF',b' ABC  DEF ',b'One @Ptwo @Lthree',b'@G3 ABC',b'ABC @G3 DEF',b'@17linked word then rest',b'@123ABC @2DEF',b'@0 X',b'A@17B C',b'ABC\nDEF',b'@L@P@L',b'@G0@G12Z end',b'@-7ABC DEF',b'@999999999ABC',b'@17A @P@3B',bytes([199,231,255,128])+b' ab',b'ABCDEFGHIJKLMNOPQRSTUVWXYZ',b'A',b'@17 ',b'2147483647',b'-2147483648',b'-123456789',b'000003',b'0',b'A'*2001,b'A @L  '*250,b'@17',b'ABC @LDEF @G3 GH']
exports=['metadata/rich-message-text-next-native.json','metadata/rich-message-text-helpers-native.json'];byte_audit={};ranges=[]
for path in exports:
 for entry,f in json.loads((ROOT/path).read_text())['functions'].items():
  ins=f['assembly']['items'];assert not f['assembly'].get('has_more',False)
  for x in ins:assert bytes(vm.mem_read(int(x['address'],16),len(bytes.fromhex(x['bytes']))))==bytes.fromhex(x['bytes']),path+':'+x['address']
  ranges.append((int(ins[0]['address'],16),int(ins[-1]['address'],16)+len(bytes.fromhex(ins[-1]['bytes']))));byte_audit[entry]={'instructions':len(ins),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in ins)}
ranges += [(0x53e590,0x53f243),(0x4cdad0,0x4cdc2e),(0x40fb60,0x40fc40),(0x40ffd0,0x4100f0)]
probes={0x4c7fd7:'signed50link capacity',0x4c8027:'fullDWORD link cursor increment',0x4c805f:'fullDWORD link reset',0x4c80aa:'fullDWORD measurement reset',0x4c80d4:'999line guard',0x4c81a4:'paragraph indent',0x4c81b6:'linebreak',0x4c81c8:'embedded sprite directive',0x4c81d7:'partial text before sprite',0x4c8229:'64byte signedWORD embedded height',0x4c8276:'reserved sprite lines',0x4c82e7:'visible or measured line',0x4c830d:'WORD shadow color',0x4c8318:'mode2 WORD shadow color',0x4c834b:'WORD shadow restore',0x4c8372:'deferred embedded sprite',0x4c83b5:'64byte signedWORD embedded width',0x4c83f2:'scroll crossing sprite',0x4c8411:'embedded sprite receiver',0x4c842d:'maximum fullDWORD width',0x4c8444:'fullDWORD measured lines',0x4c8534:'1999word guard',0x4c8540:'word break directive',0x4c8553:'word image directive',0x4c85f3:'link bounds insertion',0x4c860f:'highlight font',0x4c862e:'BYTE glyph map',0x4c8661:'missing glyph6pixel advance',0x4c8698:'DWORD current glyph',0x4c86a0:'empty glyph return',0x4c86c7:'64byte metadata with alternate baseline',0x4c86dc:'64byte metadata with normal baseline',0x4c86ea:'negative baseline clamp',0x4c86fd:'extended glyph baseline exception',0x4c8719:'global glyph sprite receiver',0x4c871e:'post-sprite glyph reload',0x4c873a:'signedWORD glyph advance',0x40fb60:'actual opaque pixels',0x40ffd0:'actual transparent pixels',0x410090:'actual clipped transparent pixels'}
reached=set();events=[];p=[];pixel_observations=[];fixture_cache={}
def code(cpu,a,n,user):
 if a==STOP:return
 if a in probes:reached.add(a)
 if a!=0x53e590:
  assert any(lo<=a and a+n<=hi for lo,hi in ranges),'Native escaped '+hex(a)
  return
 sp=cpu.reg_read(UC_X86_REG_ESP);image,x,y=[read(sp+4+i*4) for i in range(3)];assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
 events.append([image,x,y,int.from_bytes(vm.mem_read(0xd3b51c,2),'little'),read(0xd3b5c0),read(0xe20778),read(0xe20760),read(0xd3a6ec),read(0xd3a6f0),read(0xd3b138)])
 if p[21]:return
 if p[19]&1:write(0xd3b5c0,p[20])
 if p[19]&2:write(0xe20778,read(0xe20778)+13)
 if p[19]&4:write(0xd3b51c,0x4567,2)
 cpu.reg_write(UC_X86_REG_EAX,0x12345678);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+16)
vm.hook_add(UC_HOOK_CODE,code)
def sprite_fixture(width,height,pixel):
 key=(width,height,bool(pixel&2))
 if key not in fixture_cache:
  b=bytearray(0x30f5d8);struct.pack_into('<I',b,0x30f5cc,25500)
  r=bytearray(64);struct.pack_into('<II',r,0,SRC,128);struct.pack_into('<I',r,16,1);struct.pack_into('<HH',r,20,width&65535,height&65535);r[0x32]=0 if pixel&2 else 1;b[0x180e48:0x30f548]=r*25500;fixture_cache[key]=bytes(b)
 return fixture_cache[key]
def native(params):
 global p
 p=params;events.clear();op,textid,font,highlight,special,limit,glyph,width,height,x,y,layoutwidth,calculate,flags,first,visible,cols,count,cursor,mutation,value,pixel,mode,pad=p
 vm.mem_write(0xf55bb0,sprite_fixture(width,height,pixel))
 for i in range(128):write(SRC+i*2,0xf81f if i%7==0 else i*37+123,2)
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(BASE,b'\x5a'*0x10000);vm.mem_write(BASE+0x1000,texts[textid]+b'\0')
 for a,v in [(0xd3b168,font),(0xd3b164,highlight),(0xf56f84,special),(0xf55c04,100),(0x126517c,limit),(0xd3b130,first),(0xd3b13c,visible),(0xd3b14c,cols),(0xd3a6ec,count),(0xd3a6f0,cursor),(0xe20774,0),(0xe92dfc,mode),
  (0xe38d9c,640),(0xe38d98,480),(0xe38d94,1280+pad),(0xe38db0,640),(0xe38dac,480),(0xe92e18,DST+256),(0xe92e1c,BASE+0x8000),(0xd3b55c,0),(0xd3b554,0),(0xd3b558,640),(0xd3b550,480)]:write(a,v)
 write(0xd3b51c,0x1234,2);write(0xe38dda,5,1);write(0x70e120,0,2);frame_bytes=(1280+pad)*480+320;vm.mem_write(DST,b'\xa5'*0x100000)
 sp=STACK+0x1e0000;argv=[[value,x,(x+layoutwidth)&0xffffffff,y],[],[BASE+0x1000,x,y,layoutwidth,calculate,flags],[BASE+0x1000],[BASE+0x1000,x,y,calculate,flags],[font,glyph,x,y,calculate,flags],[BASE+0x1000]][op]
 for i,v in enumerate([STOP]+argv):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x4c7fd0,0x4c8040,0x4c8070,0x4c8480,0x4c8590,0x4c8680,0x4cc070][op],STOP,count=3000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native budget/return differs'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 pixels=bytes(vm.mem_read(DST,frame_bytes)) if pixel else b''
 if pixel:
  assert pixels[:256]==b'\xa5'*256 and pixels[-64:]==b'\xa5'*64,'Framebuffer guards changed'
  assert bytes(vm.mem_read(SRC,256))==b''.join((0xf81f if i%7==0 else i*37+123).to_bytes(2,'little') for i in range(128)),'Source changed'
  padding=sum(c!=0xa5 for row in range(480) for c in pixels[256+row*(1280+pad)+1280:256+(row+1)*(1280+pad)])
  pixel_observations.append({'operation':op,'text':textid,'font':font,'calculate':calculate,'pitch':1280+pad,'bytes':frame_bytes,'sha256':sha(pixels),'native_padding_bytes_written':padding})
 assert bytes(vm.mem_read(0xe34394,8))==b'\xa5'*8 and bytes(vm.mem_read(0xe34464,8))==b'\xa5'*8,'Line buffer guard changed'
 assert read(0xd3a6f4)==0xa5a5a5a5 and read(0xd3aae0)==0xa5a5a5a5,'Link buffer guard changed'
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)+bytes(vm.mem_read(BASE+0xff8,2020))
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op in [3,5,6] else 0,'state':state.hex(),'events':events.copy(),'pixels':pixels}
fields=['op','text','font','highlight','special','limit','glyph','width','height','x','y','layoutwidth','calculate','flags','first','visible','cols','count','cursor','mutation','value','pixel','mode','pad'];default=[2,1,335,1024,18765,25500,33,5,7,10,20,150,0,0,0,20,40,0,0,0,0,0,2,0];cases=[]
def add(**kw):
 q=default.copy()
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(q)
for op in range(7):add(op=op)
for count,cursor in [(0,0),(1,1),(49,49),(50,50),(256,0),(0x7fffffff,0)]:add(op=0,count=count,cursor=cursor,value=0x12345678)
for count,cursor in [(0x12345678,0xabcdef98),(50,49),(0xffffffff,0xffffffff)]:add(op=1,count=count,cursor=cursor)
for glyph,font,calculate,flags in itertools.product(range(256),[0,335,18765],[0,1],[0,1]):add(op=5,glyph=glyph,font=font,calculate=calculate,flags=flags)
for glyph,width,height,mutation in itertools.product([0,33,96,199,231],[0,5,300,-3],[-1,7,17,80],[0,1,2,4,7]):add(op=5,glyph=glyph,width=width,height=height,mutation=mutation,value=2)
for text,font in itertools.product(range(27),[335,18765]):add(op=3,text=text,font=font)
for text in range(19,25):add(op=6,text=text)
for text,calculate,first in itertools.product(range(20),[0,1,256],[0,1,256]):add(text=text,calculate=calculate,first=first,layoutwidth=200)
for text,font,mode in itertools.product([1,3,4,5,6,7,12,15,16,26],[335,18765],[0,2,256]):add(text=text,font=font,mode=mode,layoutwidth=160,mutation=2)
for text,first,visible,height in itertools.product([4,5,12],[0,1,2],[0,1,20],[7,40]):add(text=text,first=first,visible=visible,height=height)
for width,calculate in itertools.product([0,1,-1],[0,1]):add(layoutwidth=width,calculate=calculate,text=18)
for text,count in itertools.product([6,7,15],[0,49,50,256]):add(op=4,text=text,count=count,cursor=count if count<=50 else 0)
for limit,font,glyph in itertools.product([0,1,100,25500],[0,335,18765],[33,199]):add(op=5,limit=limit,font=font,glyph=glyph)
for op,text,font,pixel,pad,position in itertools.product([2,4,5],[1,6,16],[335,18765],[1,3],[0,16],[(10,20),(-3,-2),(638,478)]):add(op=op,text=text,font=font,pixel=pixel,pad=pad,x=position[0],y=position[1],glyph=199 if text==16 else 33)
for first,calculate in itertools.product([0,1,2],[0,1]):add(text=28,first=first,calculate=calculate,height=40)
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'stage':'production' if args.production else 'artifacts/rich_message_text_phase','native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Six complete native rich-message bodies (593instructions), actual glyph-width and decimal helpers versus handwritten C; full state/line/link buffers, ordered sprite arguments, consumed metric returns, WORD colors, native stack/saved registers. Pixel cases compose actual native/C sprite dispatch, clipping and raw copies and compare full framebuffer bytes including padded rows.','limitations':['Raw glyph fixtures use controlled5x7 deterministic sprite pixels and actual immutable character map; asset identity and compressed sprites are separate proofs. Sprite endpoint is controlled in nonpixel cases to test consumed metadata reloads and deep side effects.','Native unchecked text copies require assembled lines to fit the200byte scratch buffer and linked IDs/font image records to be valid. Numeric link markers require their native following delimiter/text; an isolated marker immediately at NUL makes the original scanner consume beyond that NUL and is excluded. Word/line budget guards are exercised separately without overflowing the native scratch buffer. Negative image indices are outside the valid font domain; no new production guard is inserted.','The existing read-only glyph measurement and decimal parser are composed. Native decimal input remains within its power-table capacity; tests include signed32bit bounds. Windows32bit compiled arithmetic is compared, with explicit unsigned wrap in the rewritten six bodies.','Completed city rendering, actual font payload identity and other downstream renderer paths require root integration replay.']}
checked=0;case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':24}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read().decode(errors='replace');actual=json.loads(line);actual['pixels']=process.stdout.read(actual.pop('pixels_bytes'));expected=native(case)
  for k in expected:
   if actual[k]!=expected[k]:
    detail=[actual[k],expected[k]]
    if k in ['state','pixels']:
     aa=bytes.fromhex(actual[k]) if k=='state' else actual[k];bb=bytes.fromhex(expected[k]) if k=='state' else expected[k];ix=next((i for i,(a,b) in enumerate(zip(aa,bb)) if a!=b),None);detail={'first_difference':ix,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':bb[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    raise AssertionError(k+' differs '+str(detail))
  checked+=1
  if checked%500==0:print('checked',checked,file=sys.stderr,flush=True)
 if not args.debug_limit:assert set(probes)<=reached,'Missing probes '+str([hex(a)+':'+probes[a] for a in set(probes)-reached])
 report.update(passed=True,checked=checked,complete_case_set=not bool(args.debug_limit))
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
if not args.debug_limit:
 terminal=default.copy();terminal[1]=27;terminal[11]=200
 child=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
 assert json.loads(child.stdout.readline())=={'fields':24}
 child.stdin.write((' '.join(map(str,terminal))+'\n').encode());child.stdin.flush();observed=json.loads(child.stdout.readline());child.stdin.close();child.wait(timeout=10)
 reached_before=reached.copy();budget_error=None
 try:native(terminal)
 except AssertionError as error:budget_error=str(error)
 reached.clear();reached.update(reached_before)
 terminal_audit={'format':1,'native_sha256':digest,'fixture_hex':'40313700','following_guard_byte':90,'case':terminal,'native_instruction_budget':3000000,'native_budget_exhausted':budget_error=='Native budget/return differs','native_eip':hex(vm.reg_read(UC_X86_REG_EIP)),'native_partial_y_advance':read(0xe20774),'C_completed':child.returncode==0,'C_sprite_calls':len(observed['events']),'C_y_advance':u32(bytes.fromhex(observed['state']),4096+28),'native_equivalence_claimed':False,'production_guard_added':False,'explanation':'The numeric link has no following delimiter/text. Native scanner consumes its NUL inside markup and then scans following90guard bytes; layout retries the oversized token. C preserves that behavior and completes its999line guard; bounded native observation is retained separately and does not assert complete equivalence. Actual MM terminal numeric selectors occur only media field60; no such markers occur in title/caption/body fields.'}
 assert terminal_audit['native_budget_exhausted'] and terminal_audit['C_y_advance']==20*16
 (ROOT/'metadata/rich-message-text-terminal-marker-native.json').write_text(json.dumps(terminal_audit,indent=2)+'\n');report['terminal_marker_native_audit']=terminal_audit
report['branch_probes']={hex(a):{'name':name,'reached':a in reached} for a,name in probes.items()};report['native_byte_audit']=byte_audit;report['native_instructions_verified']=sum(x['instructions'] for x in byte_audit.values());report['native_bytes_verified']=sum(x['bytes'] for x in byte_audit.values());report['pixel_cases']=len(pixel_observations);report['pixel_bytes_compared']=sum(x['bytes'] for x in pixel_observations);report['pixel_observations']=pixel_observations;report['smoke']={'passed':True,'output':smoke.stdout.strip()}
sources=[str(phase/'include/rich_message_text.h').replace('\\','/')]+[str(phase/f'src/unit_{n:05d}.c').replace('\\','/') for n in range(1630,1636)]+['tests/rich_message_text.c','tests/rich_message_text_raster.c','tools/verify_rich_message_text_emulation.py','include/sprite_libraries.h','include/sprite_render.h','runtime/sprite_pixels.c']+[f'src/unit_{n:05d}.c' for n in [1669,1670,2524,1725,1726,128,136,137,140,146,149]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports}
name='rich-message-text'+('' if args.production else '-staged')+('-debug' if args.debug_limit else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
