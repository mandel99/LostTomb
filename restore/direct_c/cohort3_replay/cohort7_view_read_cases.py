"""Native view/state units, render snapshots and connected low-I/O fixtures."""
import struct
from cohort7_startup_cases import base as startup_base,put,child
MODE,STATE,DIRTY,PENDING,ONLINE=0x605a3c,0xe38dda,0xe38dca,0xe38ebc,0x78d828
BLOCKS=[(0xe38da0,0x120),(0xe94d90,0x84),(0x775918,0x20),(0xd3b550,16),(0x7027a0,0x30),(0xf5400c,0x10c)]
GLOBALS=[MODE,PENDING,ONLINE,0x5d2e88,0xf53fdc,0x579638,0xd3b17c,0xd3a6c4,0xeaac28,
 0xe20764,0xe20768,0x784f14,0x784f18,0x7056c8,0xf53f68,0xf56f8c,0xf56fb4]

def base(entry,label,args=()):
 c=startup_base(entry,label,None);c['id']=c['id'].replace('c7-startup/','c7-view-read/')
 c.update(arguments=list(args),named_arguments=list(args),endpoints=[],expected_endpoint_counts={})
 for at,n in BLOCKS:c['stores'].append((at,b'\xa5'*n));c['watched'].append((at,n));c['write_ranges'].append((at,n))
 for at in GLOBALS:put(c,at,0);c['watched'].append((at,4));c['write_ranges'].append((at,4))
 put(c,MODE,0x101);put(c,PENDING,0);put(c,ONLINE,0)
 c['stores'] += [(STATE,b'\1'),(0xe38ea8,b'\0'),(0x7027c6,b'\0')]
 return c

def endpoint(c,token,address,arity,args=(),value=0):
 e=child(token,address,arity,value);e['result_kind']='void'
 c['endpoints'].append(e);c['expected_endpoint_counts'][token]=1
 c.setdefault('expected_endpoint_arguments',{})[token]=[list(args)]
 return e

def primary_cases():
 out=[]
 for mode in ['captured-mode-fresh-title-arguments','DWORD-editor-state-not1','DWORD-network-popup','signed-BYTE-and-adjacent-flag','DWORD-pending-skips-popup']:
  c=base(0x409fa0,mode)
  if mode=='captured-mode-fresh-title-arguments':
   put(c,MODE,1);put(c,0xe38e68,1)
   e=endpoint(c,'title-first',0x4cc110,2,[5,123]);e['mutations']=[(MODE,4,0x101),(0xe20764,4,0x81234567)]
   e=endpoint(c,'title-second',0x4cc160,2,[5,124]);e['mutations']=[(0xe20768,4,0xf2345678)]
   endpoint(c,'place-title',0x538cd0,3,[0xf2345678,0x81234567,0])
  elif mode=='DWORD-editor-state-not1':
   put(c,MODE,1);put(c,0xe38e68,0x101);e=endpoint(c,'editor-action',0x442800,0);e['mutations']=[(DIRTY,1,0)]
  elif mode=='DWORD-network-popup':put(c,ONLINE,0x100);endpoint(c,'popup',0x4e2970,3,[13,0,0])
  elif mode=='signed-BYTE-and-adjacent-flag':
   c['stores'] += [(0xe38ea8,b'\xff'),(0x7027c6,b'\0\xaa\xbb\xcc')];put(c,0x579638,0xffffffff)
   e=endpoint(c,'activate-action',0x417ae0,1,[14]);e['mutations']=[(0xd3b17c,4,0x12345678)]
  else:put(c,PENDING,0x100);put(c,ONLINE,0x100)
  out.append(c)
 return out

def toggle_cases():
 out=[]
 for mode in ['popup-first','real-primary-action','fallback']:
  c=base(0x40b590,mode)
  if mode=='popup-first':put(c,ONLINE,0x100);endpoint(c,'popup',0x4e2970,3,[13,0,0])
  elif mode=='real-primary-action':put(c,MODE,1);put(c,0xe38e68,0x101);endpoint(c,'editor-action',0x442800,0)
  else:put(c,MODE,1);c['stores'].append((STATE,b'\2'));endpoint(c,'fallback-action',0x40a090,0)
  out.append(c)
 return out

def tool_cases():
 out=[]
 for state in [30,73,31,32,18,35,1,47]:
  c=base(0x4168e0,f'state-{state}',[0x12345678]);c['stores'].append((STATE,bytes([state])))
  put(c,0xd3a6c4,0x101 if state==18 else 1);put(c,0xe94e0c,0x12345678 if state in [35,1]else 0)
  if state not in [18,47]:endpoint(c,'redraw-tools',0x547a60,0)
  out.append(c)
 return out

def leaf_cases():
 out=[]
 c=base(0x417e70,'full-DWORD-store-preserves-registers');put(c,0x7027a8,0xaabbccdd)
 c['context_compare']=['eax','ecx','edx','eflags'];out.append(c)
 for value in [0,0x100,0xffffffff]:
  c=base(0x4d6070,f'pending-{value:x}');put(c,0xe94d98,value);out.append(c)
 for width,height in [(0xf1234567,0x81234567),(0x12340000,0)]:
  c=base(0x5370d0,f'snapshot-{height:x}');put(c,0xe38dac,width);put(c,0xe38db0,height)
  c['context_compare']=['eax'];out.append(c)
 return out

def city_cases():
 out=[];city=(0x1fffffff*0x49d8)&0xffffffff
 for entry in [0x4e2d80,0x4e2dd0]:
  for mode in ['exact-threshold-signed-half','population299','comparison-rejected']:
   c=base(entry,mode);put(c,0xeaac28,0x1fffffff);put(c,PENDING,0x100)
   put(c,(0xea1894+city)&0xffffffff,299 if mode=='population299'else 300)
   if entry==0x4e2d80:put(c,(0xea446c+city)&0xffffffff,10 if mode=='comparison-rejected'else 11)
   else:
    put(c,(0xea5e1c+city)&0xffffffff,0xfffffffd)
    put(c,(0xea41dc+city)&0xffffffff,0 if mode=='comparison-rejected'else 0xffffffff)
   if mode=='exact-threshold-signed-half':endpoint(c,'city-notice',0x4e2970,3,[92 if entry==0x4e2d80 else 93,0,0])
   out.append(c)
 return out

def selected_entry_cases():
 out=[]
 for mode in ['active','inactive-negative-centering','gate-exact1','DWORD-mode-not1']:
  c=base(0x52d940,mode);put(c,MODE,0 if mode=='active'else 0x101);put(c,0x784f14,2);put(c,0x784f18,0x7fffffff)
  c['stores'] += [(0x77f6e6+2*98,bytes([2 if mode=='gate-exact1'else 1])),((0x77de03+0x7fffffff*106)&0xffffffff,b'\xd3'),(0xe39088,bytes([0 if mode=='inactive-negative-centering'else 1]))]
  for at,value in [(0xe38da4,0x11223344),(0xe38da8,0x55667788),(0xe38dac,640),(0xe38db0,331 if mode=='inactive-negative-centering'else 481),(0xf56fb4,0x81234567)]:put(c,at,value)
  if mode!='gate-exact1':endpoint(c,'draw-selected',0x4d6250,9,[195 if mode=='inactive-negative-centering'else 21,211,0 if mode=='inactive-negative-centering'else 74,522,332,0x81234567,0,0x55667788,0x11223344])
  out.append(c)
 return out

def setting_cases():
 out=[]
 for flag in [1,0x101]:
  c=base(0x5345a0,f'flag-{flag:x}');put(c,0xe38da4,1000);put(c,0xe38da8,2000);put(c,0x7056c8,flag)
  e=endpoint(c,'setting-frame',0x4d63d0,8,[0,2240,1176,160,32,1 if flag==1 else 0,2000,1000])
  e['mutations']=[(0xe38da4,4,3000),(0xe38da8,4,4000),(0xf56f8c,4,0xf1234567)]
  endpoint(c,'setting-label',0x4d6250,9,[18,3,2240,1187,160,0xf1234567,0,4000,3000]);out.append(c)
 return out

def network_cases():
 out=[]
 for enabled in [0,0x100]:
  c=base(0x536620,f'enabled-{enabled:x}');put(c,0xf53f68,enabled);put(c,0xe38da4,0x7fffffff)
  if enabled:
   e=endpoint(c,'network-frame',0x536dc0,6,[6,0,218,12,15,1]);e['mutations']=[(0xe38da4,4,1),(0xf56f8c,4,0x81234567)]
   endpoint(c,'network-text',0x4cc720,6,[0x5f8f28,0,224,192,0x81234567,65535])
  out.append(c)
 return out

def video_cases():
 out=[]
 for mode in ['inactive','DWORD-saved-state','restore-fresh-handle','no-handle']:
  c=base(0x5391e0,mode);put(c,0xf54054,0 if mode=='inactive'else 0x100)
  put(c,0xf5404c,0 if mode=='restore-fresh-handle'else 0x100);put(c,0xf54114,0 if mode=='no-handle'else 0x71234567)
  c['function_pointers']=[dict(address=0x56f32c,native=0x31000900,compiled_symbol='_SmackClose_4')]
  if mode!='inactive':
   handle=0x71234567
   if mode=='restore-fresh-handle':
    e=endpoint(c,'restore-video',0x4ce100,1,[0xf5400c]);e['mutations']=[(0xf54114,4,0x72345678)];handle=0x72345678
   if mode!='no-handle':
    e=endpoint(c,'close-video',0x31000900,1,[handle]);e.update(compiled_symbols=['_SmackClose_4'],native_cleanup=4)
    e['mutations']=[(0xf54114,4,0xdeadbeef),(0xf54054,4,0)]
   endpoint(c,'finish-video',0x4ce2d0,0)
  out.append(c)
 return out

def definitions(registry):
 from cohort7_lowio_read_close_cases import definitions as lowio_definitions
 present={int(m['address'],16)for m in registry['members']}
 root=primary_cases()+toggle_cases()+tool_cases()+leaf_cases()+city_cases()+selected_entry_cases()+setting_cases()+network_cases()+video_cases()
 return [c for c in root if c['entry']in present]+lowio_definitions(registry)
