"""Bounded original message-controller/handwritten-C and composed pixel comparison."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-message-panel/out/Debug/restore_message_panel_verify.exe');parser.add_argument('--debug-limit',type=int);args=parser.parse_args();exe=args.exe.resolve()
sha=lambda b:hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;ALT=BASE+0x10000;SRC=0x31000000;DST=0x31100000;STOP=0x33000000;STACK=0x34000000;CAPACITY=0x300000
for a,n in [(BASE,0x500000),(SRC,0x100000),(DST,CAPACITY),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
def string(a):
 out=bytearray()
 for i in range(2048):
  c=byte(a+i)
  if not c:return bytes(out)
  out.append(c)
 raise AssertionError('Native string unterminated '+hex(a))
def string_hash(a):
 v=2166136261
 for c in string(a):v=((v^c)*16777619)&0xffffffff
 return v
fixed_regions=[(0xd3a6e0,2732),(0xd3b500,232),(0xe20760,64),(0xe38da0,24),(0xe38dc4,16),(0xe38dd4,16),(0x5c7da8,684)]
descriptors=bytes(vm.mem_read(0x5c7da8,684));sprite=bytearray(0x30f5d8);struct.pack_into('<I',sprite,0x30f5cc,5000)
for i in range(1000):struct.pack_into('<I',sprite,i*8+4,100+i*4)
for i in range(5000):
 p=0x180e48+i*64;struct.pack_into('<II',sprite,p,SRC,64);struct.pack_into('<Ihh',sprite,p+16,1,8,9);sprite[p+0x32]=1
sprite=bytes(sprite);key_sprite=bytearray(sprite)
for i in range(5000):key_sprite[0x180e48+i*64+0x32]=0
key_sprite=bytes(key_sprite)
counts={0x4c8040:0,0x4dddc0:0,0x51e990:0,0x448330:3,0x447840:0,0x47d350:2,0x4c8070:6,0x4cc720:6,0x4cc770:5,0x4ccbb0:6,0x4ccc30:7,0x4cccb0:6,0x4cce00:7,0x4cd010:6,0x4cd9d0:5,0x4cd680:5,0x536820:4,0x536990:4,0x536a30:4,0x538dc0:5,0x561c96:1,0x425730:4,0x4cd240:7,0x4cd1a0:5,0x53e590:3}
events=[];pixel_mode=False;measured=advance=body_height=draw_height=post_redraw=variant=render_eax=0;reached=set();pixel_checks=0;pixel_observations=[]
# Probes are native instruction addresses. Full branch cases and data guards are also compared.
probes={0x4c5bfd:'setup template flag',0x4c5c11:'setup WORD event flags',0x4c5c8b:'setup media guard',0x4c5cc9:'media dispatch',0x4c5cce:'normal dispatch',
 0x4c5d1e:'media buttons bit4',0x4c5d9b:'media buttons bit1',0x4c5e16:'media buttons bit2',0x4c60e4:'picture1 WORD x write',0x4c60ea:'picture1 WORD y write',
 0x4c6373:'body measurement',0x4c639c:'unscrolled width path',0x4c6623:'normal family getter',0x4c67c8:'subtype5 variant',0x4c68c5:'subtype6 variant',
 0x4c6e5d:'media sprite dimensions',0x4c700f:'media family getter',0x4c7204:'media external record WORD',0x4c7275:'media button submission',
 0x4c72c3:'thumb top',0x4c72cb:'thumb bottom',0x4c72d2:'thumb fraction',0x4c72fc:'thumb drag override',0x4c7353:'special WORD mask',0x4c7494:'normal controls',
 0x53e573:'dimension valid',0x53e587:'dimension failure',0x40fb60:'actual opaque raster',0x40ffd0:'actual uncut transparent raster',0x410090:'actual clipped transparent raster'}
def code(cpu,a,n,user):
 if a==STOP:return
 if a in probes:reached.add(a)
 if a not in counts:
  assert any(lo<=a and a+n<=hi for lo,hi in [(0x4c5bf0,0x4c776d),(0x53e530,0x53e58f),(0x4cde40,0x4cde80),(0x425730,0x425850),(0x53e590,0x53f243),(0x4cdad0,0x4cdc2e),(0x40fb60,0x40fc40),(0x40ffd0,0x4100f0)]),'Native escaped '+hex(a)
  return
 sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(counts[a])];values=argv.copy();cleanup=0;result=render_eax
 if a==0x448330:values=[argv[0],argv[2]]
 elif a in [0x4c8070,0x4cc720,0x4cc770,0x4cd010,0x538dc0,0x561c96]:values[0]=string_hash(argv[0])
 elif a==0x4cce00:values[1]&=255;values[2]=string_hash(argv[2])
 elif a==0x53e590:
  assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;values.insert(0,0xf55bb0);cleanup=12
 row=[a,len(values)]+values+[0]*(20-2-len(values));row[12:18]=[read(0xd3b124),read(0xd3b138),read(0xd3b150),read(0xe20778),byte(0xe38dca),byte(0xe38dc9)];events.append(row)
 if pixel_mode and a in [0x425730,0x53e590]:return
 if a==0x4c8040:vm.mem_write(0xd3a6f8,bytes(1000));write(0xd3a6ec,0);write(0xd3a6f0,0)
 elif a==0x4dddc0:write(0xe38dca,post_redraw,1)
 elif a==0x448330:
  value=('Template%u'%argv[0]).encode()+b'\0';assert len(value)<=argv[2];vm.mem_write(argv[1],value)
 elif a==0x447840:write(0xe20778,read(0xe20778)+19);result=BASE+0x100
 elif a==0x47d350:write(0xd3b16c,read(0xd3b16c)+2);result=variant
 elif a==0x4c8070:
  if argv[4]:write(0xd3b138,measured)
  else:write(0xe20774,read(0xe20774)+body_height)
 elif a in [0x4cc720,0x4cc770,0x4ccbb0,0x4cccb0,0x4cce00]:write(0xe20778,read(0xe20778)+advance)
 elif a==0x4ccc30:write(0xe20778,advance)
 elif a==0x4cd010:write(0xe20774,read(0xe20774)+31)
 elif a==0x538dc0:result=7
 elif a==0x561c96:result=int(string(argv[0]))
 elif a==0x53e590:write(0xd3b5bc,draw_height,2);result=0x55667788
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
vm.hook_add(UC_HOOK_CODE,code)
def native(p):
 global pixel_mode,measured,advance,body_height,draw_height,post_redraw,variant,render_eax,pixel_checks
 op,ident,kind,subtype,action,media,history,template,flags,lines,measured,first,maximum,drag,dragpos,ox,oy,x,y,cols,rows,titlex,picture1,picture2,captionx,resource,quantity,external,external_count,external_class,status,value=p
 pixel_mode=op>=8;events.clear();advance=37;body_height=48;draw_height=value;post_redraw=(status>>8)&255;variant=value&7;render_eax=value
 vm.mem_write(BASE,bytes(0x10000))
 for a,n in fixed_regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(0x5c7da8,descriptors);record=0xe209b8+ident*80;evt=0xe95228+2*48
 vm.mem_write(record-8,b'\xa5'*96);vm.mem_write(evt-8,b'\xa5'*64)
 for offset,v in [(0,kind),(2,subtype),(6,x),(8,y),(10,cols),(12,rows),(26,titlex),(28,17),(14,picture1),(16,12),(18,28),(20,picture2),(22,40),(24,20),(30,captionx),(32,45),(38,20),(40,22)]:write(record+offset,v,2)
 for offset,v in [(60,0 if status&64 else 0x200),(68,0x300),(72,0x400),(76,0x500)]:write(record+offset,v)
 for a,s in [(BASE+0x100,b'Fixture Family'),(BASE+0x200,b'clip.smk' if status&4 else b'@17'),(BASE+0x300,b'Fixture title'),(BASE+0x400,b'Fixture caption'),(BASE+0x500,b'Fixture body')]:vm.mem_write(a,s+b'\0')
 for offset,v in [(16,1 if template in [1,3] else -1),(18,2 if template in [1,4] else -1),(20,3 if template in [1,5] else -1),(24,quantity),(26,resource),(28,43),(38,flags)]:write(evt+offset,v,2)
 width=640+ox*2;height=480+oy*2;pitch=width*2+(0 if pixel_mode and not(status&0x80000000) else 16)
 for a,v in [(0xd3b124,ident),(0xe951e4,2),(0xe36c74,BASE),(0xd3a6e8,action),(0xd3b180,media),(0xd3aae0,history),(0xd3b138,lines),(0xd3b130,first),(0xd3b134,maximum),(0xd3b12c,drag),(0xd3b128,dragpos),
  (0xe38da8,ox),(0xe38da4,oy),(0xe38db0,width),(0xe38dac,height),(0xe38d9c,width),(0xe38d98,height),(0xe38d94,pitch),(0xe92e18,DST+256),(0xe92e1c,BASE+0x8000),(0xd3b140,256 if status&2 else 0),(0xe950dc,1 if status&8 else 0),
  (0xd3b150,120+oy),(0xd3b15c,oy),(0xd3b160,ox),(0xd3b154,ox+16),(0xd3b148,rows if op in [3,9] else 23),(0xd3b14c,cols if op in [3,9] else 36),(0xd3b13c,22),(0xd3b144,48),(0xd3b16c,7),(0xd3b170,external),(0xd3b174,17),(0xd3b178,-1500),
  (0xe20778,0x12345678),(0xe20774,11),(0x785d70,external_count),(0x978d7c,2 if status&32 else 0),(0xeaac28,1),(0xea5db0+0x49d8,value%7)]:write(a,v)
 for a,v in [(0xd3b184,template),(0xe38dca,status&1),(0xe38dc9,0),(0xe38dc8,0x5a),(0xe38dda,5),(0xe39088,(status>>4)&1)]:write(a,v,1)
 write(0xd3b186,flags,2)
 if external<300:
  er=0x785d74+external*124;vm.mem_write(er,bytes(124));write(er+2,1,1);write(er+48,external_class,2);write(0x77de03+external*106,3,1)
 fixture=key_sprite if pixel_mode and value&16 else sprite
 vm.mem_write(ALT,fixture);vm.mem_write(0xf55bb0,fixture)
 for a,v in [(0xf56f8c,335),(0xf56f9c,1005),(0xf56fa4,201),(0xf56fac,871),(0xf56fb4,18765),(0xf55c04,296),(0xf55c7c,18000),(0xf5690c,20000),(0xf571fc,5700),(0xf5731c,23908),(0xf56494,1000),(0xf55c34,1500),(0x7056c8,value&3)]:write(a,v)
 write(0x70e120,0,2)
 frame_bytes=pitch*height+320;assert frame_bytes<CAPACITY;vm.mem_write(DST,b'\xa5'*CAPACITY)
 for i in range(256):write(SRC+i*2,0xf81f if i%7==0 else i*37+123,2)
 if pixel_mode:
  for a,v in [(0xd3b55c,0),(0xd3b554,0),(0xd3b558,width),(0xd3b550,height)]:write(a,v)
  if op==8:
   for a,v in [(0xd3b160,ox+x),(0xd3b15c,oy+y),(0xd3b154,ox+x+16),(0xd3b150,120+oy+y)]:write(a,v)
  for a in [0x5c7da8,0x5c7dd0,0x5c7df8,0x5c7e20,0x5c7e48,0x5c7e70,0x5c7ec0,0x5c7ee8,0x5c7f38,0x5c7f88,0x5c7fb0,0x5c7fd8,0x5c8050]:
   for off,v in [(24,not(value&8)),(25,20 if value&1 else 0),(26,2 if value&2 else 0),(27,0x41 if value&4 else 0)]:write(a+off,v,1)
 sp=STACK+0x1e0000;vm.mem_write(STACK,b'\0'*0x200000);write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 if op==0:
  for i,v in enumerate([ident,256 if status&2 else 0,template]):write(sp+4+i*4,v)
 if op==6:
  write(ALT+0x30f5cc,picture2);write(ALT+0x180e48+20,captionx,2);write(ALT+0x180e48+22,resource,2)
  if picture1<5000:write(ALT+0x180e48+picture1*64+20,captionx,2);write(ALT+0x180e48+picture1*64+22,resource,2)
  write(BASE+0x800,0xa5a5a5a5);write(BASE+0x804,0xa5a5a5a5)
  for i,v in enumerate([picture1,BASE+0x800,BASE+0x804]):write(sp+4+i*4,v)
  vm.reg_write(UC_X86_REG_ECX,ALT)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 entry=[0x4c5bf0,0x4c5e60,0x4c6c70,0x4c72b0,0x4c7340,0x4c5ce0,0x53e530,0x4c5cc0,0x4c7340,0x4c72b0][op]
 vm.emu_start(entry,STOP,count=500000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native budget/return differs'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+(16 if op==6 else 4),'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 pixels=bytes(vm.mem_read(DST,frame_bytes)) if pixel_mode else b''
 if pixel_mode:
  assert pixels[:256]==b'\xa5'*256 and pixels[-64:]==b'\xa5'*64,'Framebuffer guard changed'
  padding_writes=sum(c!=0xa5 for row in range(height) for c in pixels[256+row*pitch+width*2:256+(row+1)*pitch])
  assert bytes(vm.mem_read(SRC,512))==b''.join((0xf81f if i%7==0 else i*37+123).to_bytes(2,'little') for i in range(256)),'Sprite source changed'
  pixel_checks+=1;pixel_observations.append({'operation':op,'width':width,'height':height,'pitch':pitch,'bytes':frame_bytes,'sha256':sha(pixels),'native_padding_bytes_written':padding_writes})
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in fixed_regions)+bytes(vm.mem_read(record-8,96))+bytes(vm.mem_read(evt-8,64))+bytes(vm.mem_read(BASE+0x800,8))
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255 if op==6 else 0,'state':state.hex(),'events':events.copy(),'pixels':pixels}
default=[1,3,0,0,0,0,0,0,0,0,40,0,20,0,0,192,144,0,0,40,30,0,0,0,0,24,3,1,3,0,0x100,32];cases=[]
fields=['op','ident','kind','subtype','action','media','history','template','flags','lines','measured','first','maximum','drag','dragpos','ox','oy','x','y','cols','rows','titlex','picture1','picture2','captionx','resource','quantity','external','external_count','external_class','status','value']
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[fields.index(k)]=v&0xffffffff
 cases.append(p)
# Balanced first cases catch composition and endpoint problems early.
for op in range(10):add(op=op,picture1=5 if op==6 else 0,picture2=5000 if op==6 else 0,captionx=8 if op==6 else 0,resource=9 if op==6 else 24)
for ident,template,flags,status in itertools.product([0,1,251,256],[0,1,128],[0,1,256,0x8000],[0,8,10,72]):add(op=0,ident=ident,template=template,flags=flags,status=status)
for media,action,kind,offset in itertools.product([0,1,256],range(10),[0,2],[(0,0),(80,60),(192,144)]):add(op=4,media=media,action=action,kind=kind,history=256,flags=256,maximum=1,ox=offset[0],oy=offset[1])
for maximum,first,drag,rows in itertools.product([-1,0,1,20,256],[-1,0,1,19,20,255,256],[0,1,256],[4,23]):add(op=3,maximum=maximum,first=first,drag=drag,dragpos=300,rows=rows)
for flags,value in itertools.product([0,1,2,3,4,5,6,7,0x100],[0,1,2,3]):add(op=5,flags=flags,value=value)
for image,next_image,dimensions in itertools.product([-1,0,1,509,25499,25500],[0,5000],[(0,0),(8,9),(-3,600),(32767,-32768)]):add(op=6,picture1=image,picture2=next_image,captionx=dimensions[0],resource=dimensions[1])
for kind,subtype,ident,offset in itertools.product([0,2],range(9),[0,251,256],[(0,0),(80,60),(192,144)]):add(op=1,kind=kind,subtype=subtype,ident=ident,ox=offset[0],oy=offset[1],picture1=1001,picture2=3,captionx=32,template=1,resource=25,external_class=1)
for subtype,resource,quantity,offset in itertools.product([0,1,2],[10,24,25,26,28,30,35,36,37,7],[1,3],[(0,0),(192,144)]):add(op=2,kind=2,subtype=subtype,resource=resource,quantity=quantity,ox=offset[0],oy=offset[1],template=1,media=1,action=8,flags=0x100)
rng=random.Random(0x4c5e60)
for _ in range(500):
 op=rng.choice([1,2,7]);kind=rng.choice([0,1,2]);subtype=rng.randrange(9);media=rng.choice([0,1,256])
 add(op=op,kind=kind,subtype=subtype,ident=rng.choice([0,1,251,256]),media=media,action=rng.randrange(11),history=rng.choice([0,1,256]),template=rng.randrange(6),flags=rng.choice([0,1,7,256]),lines=rng.choice([0,1,256]),measured=rng.choice([0,1,20,22,23,256]),first=rng.choice([0,1,255]),maximum=rng.choice([0,1,20,256]),drag=rng.choice([0,1,256]),dragpos=rng.choice([0,1,300]),titlex=rng.choice([0,12]),picture1=rng.choice([0,1,999,1000,1001]),picture2=rng.choice([0,3]),captionx=rng.choice([0,32]),resource=rng.choice([10,24,25,26,28,30,35,36,37,7]),quantity=rng.choice([-3,0,1,3]),external_class=rng.choice([0,1,2]),status=rng.choice([0,1,2,0x100,0x200,0xff00,0x134]),value=rng.randrange(7))
for op,offset,value in itertools.product([8,9],[(0,0),(80,60),(192,144)],range(32)):add(op=op,kind=2,subtype=1,action=3,flags=0,maximum=20,first=1,ox=offset[0],oy=offset[1],value=value,rows=23,cols=36)
# Clip actual sprite paths at each edge without changing the pixel implementation.
for x,y in [(-16,-16),(-544,-335),(99,145),(1000,730)]:add(op=8,kind=2,action=3,maximum=0,ox=0,oy=0,x=x,y=y,value=16,cols=36,rows=23)
for op,offset,value in itertools.product([8,9],[(0,0),(80,60),(192,144)],[0,16]):add(op=op,kind=2,subtype=1,action=3,maximum=20,first=1,ox=offset[0],oy=offset[1],value=value,rows=23,cols=36,status=0x80000000)
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
rejection=subprocess.run([str(exe),'--reject-invalid-external'],cwd=ROOT,capture_output=True,text=True,timeout=30)
assert rejection.returncode!=0 and 'invalid external record index' in rejection.stderr,'Invalid native invariant was not rejected explicitly'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,
 'method':'Complete original setup/dispatcher/media helper/normal+media producer/thumb/button submission/dimension getter in bounded Unicorn versus handwritten compiled C. Controlled nested algorithms record ordered arguments, consumed returns and data effects. Separate cases compose actual native and C small-button/sprite/raster code and compare entire16bit framebuffer bytes, native row-padding writes, source pixels and prefix/suffix guard bands; frames transfer as raw binary with no lossy encoding.',
 'limitations':['Rich body layout004c8070, substitution, text primitives, family label, variant selection, video and link-clear endpoints are controlled. Their complete algorithms and final text/media pixels are separate dependencies.','The media type2/subtype2 native body dereferences NULL+48 for an invalid external-record index. Differential fixtures obey its valid-index invariant; C diagnoses and aborts at that read instead. A separate rejection child proves this fail-fast behavior; it is excluded from native equivalence.','0053e530 consumed result is AL; unused upper EAX differs by implementation. Its sole native caller binds the global manager; alternate-receiver cases additionally prove the explicit receiver.','Raster cases use deterministic in-memory8x9 raw sprite payloads, with actual native/C dispatch, clipping and copies, for both packed and padded pitches. Native padding writes are retained and compared; only prefix/suffix guard bands must remain untouched. These fixtures prove algorithms rather than production asset identity.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':32}
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read().decode(errors='replace')
  actual=json.loads(line);actual['pixels']=process.stdout.read(actual.pop('pixels_bytes'));expected=native(p)
  for key in expected:
   if actual[key]!=expected[key]:
    detail=[actual[key],expected[key]]
    if key in ['state','pixels']:
     aa=bytes.fromhex(actual[key]) if key=='state' else actual[key];bb=bytes.fromhex(expected[key]) if key=='state' else expected[key];idx=next((i for i in range(min(len(aa),len(bb))) if aa[i]!=bb[i]),None);detail={'first_difference':idx,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(idx or 0)-8):(idx or 0)+16].hex(),'native_slice':bb[max(0,(idx or 0)-8):(idx or 0)+16].hex()}
    raise AssertionError(key+' differs: '+str(detail))
  checked+=1
 if not args.debug_limit:assert set(probes)<=reached,'Missing native probes: '+str([probes[x] for x in set(probes)-reached])
 report.update(passed=True,checked=checked,complete_case_set=not bool(args.debug_limit))
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['pixel_cases']=pixel_checks;report['pixel_observations']=pixel_observations;report['pixel_bytes_compared']=sum(x['bytes'] for x in pixel_observations);report['pixel_padded_pitch_cases']=sum(x['pitch']!=x['width']*2 for x in pixel_observations);report['branch_probes']={hex(a):{'name':name,'reached':a in reached} for a,name in probes.items()};report['smoke']={'passed':smoke.returncode==0,'output':smoke.stdout.strip()}
report['invalid_external_rejection']={'passed':True,'exit_code':rejection.returncode,'diagnostic':rejection.stderr.strip(),'native_equivalence_claimed':False}
report['actual_raster_bodies']={'00425730':'small-button variants','0053e590':'sprite dispatch','004cdad0/004cdb80':'clip construction','0040fb60':'opaque copy','0040ffd0/00410090':'transparent copy with unclipped/clipped source'}
sources=['include/message_panel.h','include/message_panel_calls.h','tests/message_panel.c','tests/message_panel_raster.c','tools/verify_message_panel_emulation.py']+[f'src/unit_{n:05d}.c' for n in [1619,1620,1621,1622,1623,1624,1625,2523,1730,1731,2524,401,1725,1726,128,136,137,140,146,149]]+['runtime/sprite_pixels.c']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
exports=['metadata/render-dialog-discarded-returns-native.json','metadata/message-panel-next-native.json','metadata/city-ui-next-native.json','metadata/message-panel-support-native.json','metadata/message-panel-dimensions-callers-native.json']
report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports}
name='message-panel-debug-emulation.json' if args.debug_limit else 'message-panel-emulation.json'
(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
