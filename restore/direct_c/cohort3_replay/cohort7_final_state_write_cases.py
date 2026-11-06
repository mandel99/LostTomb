"""Final cohort state/input paths and native text-write frame fixtures."""
import struct
from cohort7_view_read_cases import base as viewbase,endpoint
from cohort7_startup_cases import put,child,imp,TEXT

def base(entry,label,args=(),result=None):
 c=viewbase(entry,label,args);c['id']=c['id'].replace('c7-view-read/','c7-final-state-write/');c['result_mask']=result
 for at in [0xd3b198,0xeadd6c,0xf53e6c,0x6fdc38,0x6fdc3c,0x6fdc40,0xe92e20,0x78d7c0,0xbc9694,0xe950e0,0xe92e48,0xe39080,0xf55b94]:
  put(c,at,0);c['watched'].append((at,4));c['write_ranges'].append((at,4))
 return c

def cell(c,address,value,width=4):
 c['stores'].append((address,(value&((1<<(width*8))-1)).to_bytes(width,'little')))
 c['watched'].append((address,width));c['write_ranges'].append((address,width))

def method(c,token,at,arity,args,receiver,native_cleanup=0):
 e=endpoint(c,token,at,arity,args);e.update(native_receiver=receiver,native_cleanup=native_cleanup,compiled_arity=arity+1,compiled_receiver_arg=0)
 return e

def offset_cases():
 out=[]
 for mode in ['decrement-clamp','decrement-wrap','callback-fresh-building','real-page-size']:
  c=base(0x40c6b0,mode);put(c,0xeadd6c,2)
  for index,value in [(2,0 if mode=='decrement-clamp'else 0x80000000 if mode=='decrement-wrap'else 7),(3,9),(4,11)]:cell(c,0x799ab8+index*264,value)
  if mode=='callback-fresh-building':
   put(c,0xd3b198,0x100);put(c,0xf53e6c,12)
   e=child('page-size',0x502780,0);e['responses']=[dict(value=5,mutations=[(0xeadd6c,4,3)]),dict(value=6,mutations=[(0xeadd6c,4,4),(0xf53e6c,4,30)])]
   c.update(endpoints=[e],expected_endpoint_counts={'page-size':2})
  elif mode=='real-page-size':put(c,0xd3b198,0x100);put(c,0xf53e6c,40);put(c,0xe38e68,3)
  out.append(c)
 return out

def capture_cases():
 out=[]
 for mode in ['inactive-keeps-block','fresh-callback-pointers','callbacks-clear-context']:
  c=base(0x416bc0,mode);block=0x6fdb38;c['stores'].append((block,b'\xa5'*272));c['watched'].append((block,272));c['write_ranges'].append((block,272))
  put(c,0x6fdc38,0 if mode=='inactive-keeps-block'else 0x10000)
  if mode!='inactive-keeps-block':
   e=endpoint(c,'capture-stop',0x560560,1,[0x10000])
   e['mutations']=[(0x6fdc3c,4,0x20000 if mode=='fresh-callback-pointers'else 0),(0x6fdc38,4,0x30000 if mode=='fresh-callback-pointers'else 0)]
   if mode=='fresh-callback-pointers':
    e=endpoint(c,'capture-other',0x5605c0,1,[0x20000]);e['mutations']=[(0x6fdc38,4,0x40000)]
    e=endpoint(c,'capture-release',0x560580,1,[0x40000]);e['mutations']=[(block+268,4,0xf1234567)]
  out.append(c)
 return out

def key_cases():
 out=[]
 specs=[('state-block',0x100,67,0x100,2),('wrong-message',0x101,67,0x100,1),('C',0x100,67,0x12340100,1),
  ('c',0x100,99,0x8000,1),('low-alt-only',0x100,67,1,1),('shot1',0x104,49,0,1),('shot2',0x104,50,0,1),
  ('unlockF',0x100,70,0x100,1),('unlockf',0x100,102,0x8000,1)]
 for label,message,key,state,view in specs:
  c=base(0x41d110,label,[message,key],0xff);c['stores'].append((0xe38dda,bytes([view])));put(c,0xe92e20,0x100)
  e=imp('GetAsyncKeyState',1,state);c['endpoints']=[e];c['expected_endpoint_counts']={'GetAsyncKeyState':1};c['expected_endpoint_arguments']={'GetAsyncKeyState':[[18]]};result=0
  if label in ['C','c']:endpoint(c,'debug-C',0x41d3d0,0);result=1
  elif label in ['shot1','shot2']:endpoint(c,'screen-shot',0x4ceed0,1,[0x57c768 if label=='shot1'else 0x57c774])
  elif label.startswith('unlock'):
   e=method(c,'unlock-campaign',0x41bae0,0,[],0x6fdd08);e['compiled_symbols']=['restore_campaign_unlock_all']
   e=endpoint(c,'profile-path',0x516a20,0);e.update(result_kind='full32',value=TEXT)
   method(c,'save-profile',0x41b8a0,1,[TEXT],0x6fdd08,4);result=1
  c['expected_logical_byte']=result;out.append(c)
 return out

def text_entry_cases():
 out=[]
 for selected in [0x101,1]:
  c=base(0x41d210,f'captured-selection-{selected:x}');put(c,0xe38da4,1000);put(c,0xe38da8,2000)
  e=endpoint(c,'entry-background',0x536820,4,[2010,1160,24,8]);e['mutations']=[(0xe38da4,4,3000),(0xe38da8,4,4000),(0x7027e4,4,selected)]
  c['watched'].append((0x7027e4,4));c['write_ranges'].append((0x7027e4,4))
  flag=int(selected==1)
  e=endpoint(c,'entry-frame',0x536c30,6,[0,4050,3246,150,30,flag]);e['mutations']=[(0x7027e4,4,1),(0xe38da4,4,5000),(0xe38da8,4,6000),(0xf56f8c,4,0x81234567)]
  e=endpoint(c,'entry-label',0x4ccbb0,6,[191,1,6055,5256,0x81234567,flag]);e['mutations']=[(0xe38da4,4,7000),(0xe38da8,4,8000)]
  endpoint(c,'entry-border',0x536a30,4,[8042,7208,20,2])
  e=endpoint(c,'entry-text-reset',0x4cc540,0);e['mutations']=[(0xe38da4,4,9000),(0xe38da8,4,10000),(0xf56f9c,4,0xf1234567)]
  c['watched'].append((0xf56f9c,4));c['write_ranges'].append((0xf56f9c,4))
  e=endpoint(c,'entry-text',0x4cc770,5,[0x7027ec,10058,9216,0xf1234567,0]);e['mutations']=[(0xe38da4,4,11000),(0xe38da8,4,12000)]
  endpoint(c,'entry-cursor',0x537140,3,[12058,11217,65535]);out.append(c)
 return out

def refresh_cases():
 out=[]
 for mode in [0,3,6,9,0x100,0x103,1]:
  c=base(0x46e180,f'fresh-DWORD-mode-{mode:x}',[0x81234567]);put(c,0x78d7c0,9)
  for token,at,n,args in [('start',0x46e1c0,0,[]),('parameter',0x46e970,1,[0x81234567]),('middle',0x46ef50,0,[]),('last',0x46e7c0,0,[])]:e=endpoint(c,token,at,n,args)
  e['mutations']=[(0x78d7c0,4,mode)]
  if mode in [0,3,6,9]:endpoint(c,'finish',0x46e1e0,0)
  out.append(c)
 return out

def initialize_case():
 c=base(0x47d6e0,'ordered-subsystems-DWORD-reset');put(c,0xbc9694,0xaabbccdd)
 e=endpoint(c,'first',0x47d870,1,[1]);e['mutations']=[(0xbc9694,4,0xf1234567)]
 for token,at in [('second',0x47d720),('third',0x4dea50),('fourth',0x46d840),('fifth',0x47fb70),('sixth',0x5178e0),('seventh',0x497b90),('last',0x4ddf90)]:
  e=endpoint(c,token,at,0)
  if token=='second':e['observe']=[(0xbc9694,4)]
 return [c]

def group_cases():
 out=[]
 for mode in ['none','negative-score','last-tie-signed-actor','sixteen-actor-cap']:
  c=base(0x4c41b0,mode,(),0xffffffff);c['stores'].append((0xbcb900,b'\0'*0x1e00));put(c,0xe950e0,0xaabbccdd);chosen=3
  if mode!='none':
   for row,score,identity in [(0,-1 if mode=='negative-score'else 5,2),(1,-1 if mode=='negative-score'else 5,3)]:
    p=0xbcbad2+row*144;c['stores'] += [(p-122,b'\1'),(p-56,b'\1'),(p,struct.pack('<h',score)),(p-116,struct.pack('<h',identity))]
  if mode in ['none','negative-score']:c['expected_eax']=0;out.append(c);continue
  ids=list(range(1,18))if mode=='sixteen-actor-cap'else[-1,5,0]
  c['stores'].append((0xbcb9d0+chosen*144,struct.pack('<'+'h'*len(ids),*ids)))
  for actor in ids:
   if not actor:continue
   at=(0xbcd606+actor*388)&0xffffffff;c['stores'].append((at-1,b'\xa5'*4));c['watched'].append((at-1,4));c['write_ranges'].append((at,1))
  r=chosen*264
  cell(c,0x7999db+r,255,1);cell(c,0x7999de+r,-10,2);cell(c,0x7999e0+r,-20,2)
  endpoint(c,'select-group',0x46d100,2,[chosen,0]);endpoint(c,'activate-group',0x4695d0,2,[chosen,1]);endpoint(c,'action',0x53b090,1,[17])
  e=endpoint(c,'first-rectangle',0x516f90,5,[(-265)&0xffffffff,(-275)&0xffffffff,245,235,1]);e['mutations']=[(0x7999db+r,1,3),(0x7999de+r,2,100),(0x7999e0+r,2,200)]
  endpoint(c,'second-rectangle',0x5173a0,4,[97,197,103,203]);c['expected_eax']=1;out.append(c)
 return out

def log_cases():
 out=[]
 for mode in ['create-even-if-write-fails','append-captured-before-open','open-failure']:
  c=base(0x4ce860,mode,[0x81234567]);append=mode!='create-even-if-write-fails';put(c,0xe92e48,0x100 if append else 0)
  e=endpoint(c,'open-status',0x568eab,3,[0x5e0714,0x8101 if append else 0x8301,384]);e.update(result_kind='full32',value=0xffffffff if mode=='open-failure'else 35)
  if mode=='append-captured-before-open':e['mutations']=[(0xe92e48,4,0)]
  if mode!='open-failure':
   if append:endpoint(c,'seek-status',0x5662e3,3,[35,0,2])
   endpoint(c,'write-status',0x5664d5,3,[35,0xe36c90,0x81234567]);endpoint(c,'commit-status',0x562517,1,[35]);endpoint(c,'close-status',0x5647eb,1,[35])
  out.append(c)
 return out

def mission_cases():
 out=[]
 for mode,title in [('open-failure',b'AB\0'),('success-word-and-bytes',b'ABCDEF\0'),('success-two-words',b'ABCDEFG\0')]:
  c=base(0x4d51b0,mode,[TEXT],0xffffffff);put(c,0xe39080,0x40000002);cell(c,0xd3b1b8,0xa5,1)
  c['stores'] += [(TEXT,b'mission.sav\0'),(0xe38e74,b'old\0'),(0xea184c,b'\xa5'*32)]
  c['watched'].append((0xea184c,32));c['write_ranges'].append((0xea184c,32));cell(c,0x5d1968,77)
  e=method(c,'load-settings',0x4bd330,0,[],0xd38360);e['mutations']=[(0xe39080,4,0x100)]
  endpoint(c,'reset-world',0x46fa40,0);endpoint(c,'close-media',0x4ce2d0,0)
  e=endpoint(c,'open-mission',0x568eab,2,[TEXT,0x8000]);e.update(result_kind='full32',value=0xffffffff if mode=='open-failure'else 35)
  if mode=='open-failure':c['expected_eax']=1;out.append(c);continue
  method(c,'stop-stream',0x55f561,1,[2],0xe92bf8,4)
  city=(0x1fffffff*0x49d8)&0xffffffff;old=(0xea187c+city)&0xffffffff;new=0xea187c+2*0x49d8
  cell(c,old,88);cell(c,new,99)
  e=endpoint(c,'read-mission',0x4d41e0,1,[35]);e['mutations']=[(0xeaac28,4,0x1fffffff)]
  endpoint(c,'close-mission',0x5647eb,1,[35])
  e=endpoint(c,'limit-stat',0x4cde40,2,[88,77]);e.update(result_kind='full32',value=0x81234567,mutations=[(0xeaac28,4,2)])
  for token,at in [('world-a',0x4d52a0),('world-b',0x46b320),('world-c',0x46f030),('world-d',0x46a320)]:e=endpoint(c,token,at,0)
  e['mutations']=[(0xe38e74+i,1,v)for i,v in enumerate(title)];c['expected_eax']=0;out.append(c)
 return out

def reset_case():
 c=base(0x4e6eb0,'DWORD-sentinel-before-dispatch');cell(c,0x78d79c,0xaabbccdd)
 e=endpoint(c,'reset-map',0x4e8e80,1,[0xfffffff1]);e['observe']=[(0x78d79c,4)];return [c]

def definitions(registry):
 from cohort7_lowio_write_cases import definitions as write_definitions
 present={int(m['address'],16)for m in registry['members']}
 root=offset_cases()+capture_cases()+key_cases()+text_entry_cases()+refresh_cases()+initialize_case()+group_cases()+log_cases()+mission_cases()+reset_case()
 return [c for c in root if c['entry']in present]+write_definitions(registry)
