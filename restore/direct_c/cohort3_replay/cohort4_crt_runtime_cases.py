"""Finite CRT stack, error, ctype and NLS boundaries; no host code executes."""
import struct
from cohort4_crt_math_cases import w
S=0x32000000
SP=0x30008000
FRAME,REG,EXC,TABLE,TEXT,OUTPUT=[S+i*0x1000 for i in range(1,7)]
EXIT,BANNER,BOX,ACTIVE,POPUP=0x31000700,0x31000710,0x31000720,0x31000730,0x31000740

def base(entry,label,args,named=None,result=None):
 return dict(id=f'c4-runtime/{entry:08x}/{label}',entry=entry,receiver=0x89abcdef,
  arguments=args,named_arguments=args if named is None else named,eflags=0x202,
  initial_edx=0x76543210,stores=[],watched=[],write_ranges=[],result_mask=result,
  native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False,
  canonical_owner='crt_purecall.obj'if entry==0x562cf7 else'crt_runtime_tail_entries.obj')

def api(name,arity,value=1):
 return dict(token=name,import_name=name,arity=arity,result_kind='full32',value=value,native_cleanup=arity*4,compiled_cleanup=arity*4)

def controlled(token,address,arity,value=1,cleanup=0):
 return dict(token=token,controlled_address=address,arity=arity,result_kind='full32',value=value,native_cleanup=cleanup,compiled_cleanup=cleanup)

def console_endpoints():
 standard=api('GetStdHandle',1,0x13579bdf)
 write=api('WriteFile',5);write.update(pointer_args={3:'written-count'},writes_args=[[3,4,0x42]])
 return [standard,write]

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']};out=[]
 for size in (0,1,0xfff,0x1000,0x1001,0x2000,0x3456):
  c=base(0x561f30,f'{size:x}',[],[SP+4,size]);c.update(initial_eax=size,native_cleanup=-size,canonical_cleanup=-size,expected_route_results={'named':SP+4-size})
  if size>=0x1000:
   locations=[];at=SP+4;remaining=size
   while remaining>=0x1000:at-=0x1000;remaining-=0x1000;locations.append([at,4])
   # The relocated RET also reads within this range. Record that separately;
   # its PC/ESP obligation is checked independently from TEST/MOV data probes.
   locations.append([at-remaining,4]);c.update(read_ranges=[(SP-0x5000,0x4008)],expected_reads=locations,read_skip_return_fetches=True)
  out.append(c)
 for mode in range(6):
  for abnormal in (0,1):
   comparison=0x13579bdf;c=base(0x563dd6,f'{mode}/{abnormal}',[],[FRAME,REG,EXC,comparison])
   c.update(initial_ebp=FRAME,initial_esi=REG,initial_edi=EXC,initial_ebx=comparison,
    stores=[(FRAME-64,b'\xa5'*128),(REG-16,b'\xa5'*32),(EXC,b'\0'*64),
     (FRAME-0x28,w(0x10203040)),(FRAME-0x1c,w(0x50607080)),(FRAME-0x20,w(0x90a0b0c0)),
     (EXC,w(0xe06d7363 if mode else 0)),(EXC+0x10,w(3 if mode!=1 else 2)),(EXC+0x14,w(0x19930520 if mode!=2 else 0)),
     (FRAME-0x24,w(comparison if mode!=3 else 0)),(FRAME-0x2c,w(comparison if mode==4 else 0))],
    watched=[(FRAME-64,128),(REG-16,32),(EXC,64),(0x1266418,8)],write_ranges=[(REG-4,4),(0x1266418,8)])
   e=dict(token='abnormal-termination',native=0x561852,compiled_symbols=['restore_crt_abnormal_termination'],arity=0,result_kind='full32',value=abnormal,native_cleanup=0,compiled_cleanup=0)
   destructor=dict(token='exception-destructor',native=0x56400a,compiled_symbols=['FUN_0056400a'],arity=2,result_kind='void',native_cleanup=0,compiled_cleanup=0)
   c.update(endpoints=[e,destructor],expected_endpoint_counts={'abnormal-termination':int(mode==5),'exception-destructor':int(mode==5)});out.append(c)
 for entry in (0x5630e0,0x562cf7,0x567dcc):
  for mode in (0,1,2):
   error=25 if entry==0x562cf7 else 0xdeadbeef
   c=base(entry,f'{mode}',[error]if entry==0x5630e0 else[],result=0xffffffff if entry!=0x567dcc else None)
   c.update(stores=[(0x1266408,w(mode)),(0x60496c,w(1)),(0x604968,w(EXIT)),(0x12666a0,w(BANNER)),(0x12666e8,w(BOX)),(0x12666ec,w(0)),(0x12666f0,w(0))],
    endpoints=console_endpoints()+[controlled('exit-callback',EXIT,1,0xfedcba98),controlled('banner-callback',BANNER,0)])
   banners=2 if entry==0x567dcc and mode in(0,1)or entry!=0x567dcc and mode==1 else 0
   count=banners+int(entry==0x562cf7 and mode in(0,1))
   module=api('GetModuleFileNameA',3,0);module['pointer_args']={1:'module-path'}
   message=controlled('message-box',BOX,4,1,16);message['private_input_args']={1:dict(role='formatted-error-message',cstring_max=160)}
   c['endpoints'] += [module,message]
   c['expected_endpoint_counts']={'GetStdHandle':count,'WriteFile':count,'banner-callback':int(banners==2),'exit-callback':int(entry!=0x567dcc),'GetModuleFileNameA':int(entry==0x562cf7 and mode==2),'message-box':int(entry==0x562cf7 and mode==2)}
   if entry!=0x567dcc:c['expected_result']=0xfedcba98
   out.append(c)
 for error in (0xfc,0xff,20,0xdeadbeef):
  for mode in (1,2):
   for path in ('short','long','fallback')if mode==2 and error not in(0xfc,0xdeadbeef)else('none',):
    c=base(0x567e05,f'{error:x}/{mode}/{path}',[error],[error,SP+0x100])
    c.update(stores=[(0x1266408,w(mode)),(0x60496c,w(1))],watched=[(SP+4,4)],
     route_watched={'named':[(SP+0x100,4)]},route_stores={'named':[(SP+0x100,w(error).hex())]})
    endpoints=console_endpoints();matched=error!=0xdeadbeef;console=matched and mode==1;box=matched and mode==2 and error!=0xfc
    module=api('GetModuleFileNameA',3,0 if path=='fallback'else 80 if path=='long'else 11)
    module.update(pointer_args={1:'module-path'})
    if path!='fallback':module['writes_arg_bytes']=[[1,0,((b'C:\\'+b'x'*77 if path=='long'else b'C:\\game.exe')+b'\0').hex()]]
    message=dict(token='message-box-helper',native=0x569c11,compiled_symbols=['restore_crt_message_box'],arity=3,result_kind='full32',value=1,native_cleanup=0,compiled_cleanup=0,private_input_args={0:dict(role='formatted-error-message',cstring_max=160)})
    c.update(endpoints=endpoints+[module,message],expected_endpoint_counts={'GetStdHandle':int(console),'WriteFile':int(console),'GetModuleFileNameA':int(box),'message-box-helper':int(box)})
    out.append(c)
 for character in (0xffffffff,0,0x41,0xff,0x100,0x8141,0xffff8141):
  for lead in (False,True):
   for success in (0,1):
    mask=0x1234abcd;c=base(0x565f66,f'{character:x}/{int(lead)}/{success}',[character,mask],[character,SP+0x104,SP+0x102],0xffffffff)
    c.update(stores=[(TABLE-16,b'\0'*1040),(0x604c58,w(TABLE)),(TABLE+0x41*2,struct.pack('<H',0x5a5a)),(TABLE+0x81*2+1,bytes([0x80 if lead else 0]))],
     watched=[(SP+6,6)],route_watched={'named':[(SP+0x102,6)]},route_stores={'named':[(SP+0x100,(w(character)+w(mask)).hex())]})
    direct=(character+1)&0xffffffff<=0x100
    count=2 if lead and(character>>8)&255==0x81 else 1
    nls=dict(token='ctype-nls',native=0x569335,compiled_symbols=['FUN_00569335'],arity=7,result_kind='full32',value=success,native_cleanup=0,compiled_cleanup=0,
     pointer_args={3:'classification-output'},private_input_args={1:dict(role='significant-multibyte-input',width=count)},writes_arg_bytes=[[3,0,'3412cdab'if count==2 else'3412']])
    c.update(endpoints=[nls],expected_endpoint_counts={'ctype-nls':int(not direct)});out.append(c)
 # ANSI, wide, backend probing and failed conversion routes. All imports are isolated.
 for backend in (0,1,2,3):
  for path in ('success','probe-A','probe-fail','size-fail','convert-fail')if backend==0 else('success','size-fail','convert-fail')if backend==1 else('success',):
   c=base(0x569335,f'{backend}/{path}',[1,TEXT,2,OUTPUT,0,0,1],[1,TEXT,2,OUTPUT,SP+0x100,0,1],0xffffffff)
   c.update(initial_fs0=0xffffffff,stores=[(0x12666dc,w(backend)),(0x126649c,w(932)),(0x126648c,w(0x409)),(TEXT,b'AB\0'),(OUTPUT,b'\xa5'*8)],
    watched=[(0x12666dc,4),(SP+20,4),(OUTPUT,8)],route_watched={'named':[(0x12666dc,4),(SP+0x100,4),(OUTPUT,8)]},route_stores={'named':[(SP+0x100,w(0).hex())]},write_ranges=[(0,4),(0x12666dc,4),(OUTPUT,8)])
   getw=api('GetStringTypeW',4,0x13579bdf);geta=api('GetStringTypeA',5,0x13579bdf);convert=api('MultiByteToWideChar',6,2)
   getw.update(private_input_args={1:dict(role='converted-wide-text',width=4)},mutations=[[OUTPUT,2,0x1234],[OUTPUT+2,2,0x5678]])
   geta['mutations']=[[OUTPUT,2,0x1234],[OUTPUT+2,2,0x5678]]
   is_wide=backend==1 or backend==0 and path not in('probe-A','probe-fail')
   if backend==0:
    probe_w=dict(value=0 if path in('probe-A','probe-fail')else 1,pointer_args={3:'backend-probe-output'},private_input_args={},mutations=[])
    getw['responses']=[probe_w]+([{}]if is_wide and path not in('size-fail','convert-fail')else[])
    if path in('probe-A','probe-fail'):
     geta['responses']=[dict(value=0 if path=='probe-fail'else 1,pointer_args={4:'backend-probe-output'},mutations=[])]+([{}]if path=='probe-A'else[])
   convert['responses']=[dict(value=0 if path=='size-fail'else 2),dict(value=0 if path=='convert-fail'else 2,pointer_args={4:'wide-allocation'},writes_arg_bytes=[[4,0,'41004200']])]
   c['endpoints']=[getw,geta,convert]
   c['expected_endpoint_counts']={'GetStringTypeW':int(backend==0)+int(is_wide and path not in('size-fail','convert-fail')),'GetStringTypeA':int(backend==0 and path in('probe-A','probe-fail'))+int(backend==2 or backend==0 and path=='probe-A'),'MultiByteToWideChar':(1 if path=='size-fail'else 2)if is_wide else 0}
   out.append(c)
 for active,popup in ((0,0),(0x1234,0),(0x1234,0x5678)):
  c=base(0x569c11,f'cached/{active:x}/{popup:x}',[TEXT,TEXT+16,0x12010],result=0xffffffff)
  c.update(stores=[(TEXT,b'text\0'),(TEXT+16,b'caption\0'),(0x12666e8,w(BOX)),(0x12666ec,w(ACTIVE)),(0x12666f0,w(POPUP if popup else 0))],
   endpoints=[controlled('active-window',ACTIVE,0,active),controlled('last-popup',POPUP,1,popup,4),controlled('message-box',BOX,4,0x13579bdf,16)],
   expected_endpoint_counts={'active-window':1,'last-popup':int(bool(active and popup)),'message-box':1},expected_result=0x13579bdf)
  out.append(c)
 for missing in ('library','entry'):
  c=base(0x569c11,f'missing-{missing}',[TEXT,TEXT+16,0x12010],result=0xffffffff)
  c.update(stores=[(0x12666e8,w(0))],watched=[(0x12666e8,12)],write_ranges=[(0x12666e8,4)],endpoints=[api('LoadLibraryA',1,0 if missing=='library'else 0x1234),api('GetProcAddress',2,0)],expected_endpoint_counts={'LoadLibraryA':1,'GetProcAddress':int(missing=='entry')},expected_result=0)
  out.append(c)
 return [c for c in out if c['entry']in present]
