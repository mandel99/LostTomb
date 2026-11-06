"""Native monument table units, partial EAX preservation and low-I/O seeks."""
import struct
from cohort7_startup_cases import base as startup_base,put,child
MODE=0x5e4ef4
BUILDING,ACTOR=20,2
def record(index):return (0x7999d8+index*264)&0xffffffff

def base(entry,label,args=(),result=0xffffffff):
 c=startup_base(entry,label,result);c['id']=c['id'].replace('c7-startup/','c7-monument-seek/')
 c.update(arguments=list(args),named_arguments=list(args),endpoints=[],expected_endpoint_counts={})
 c['stores'].append((record(BUILDING),b'\0'*264));put(c,MODE,0x105)
 c['watched'].append((record(BUILDING),264));return c

def word(c,at,value):c['stores'].append((at,struct.pack('<H',value&0xffff)))

def dispatch_cases():
 out=[]
 for mode in [5,0x105]:
  c=base(0x4f0190,f'mode-{mode:x}',[0x12345678,0xabcdef01,0xffffffff]);put(c,MODE,mode)
  if mode==5:c['expected_eax']=0
  else:
   e=child('select-work',0x4efe30,4,0xf1234567)
   c.update(endpoints=[e],expected_endpoint_counts={'select-work':1},expected_endpoint_arguments={'select-work':[[0x12345678,0xabcdef01,0xffffffff,1]]},expected_eax=0xf1234567)
  out.append(c)
 return out

def pair_cases():
 out=[]
 specs=[('mode5-left',5,9,[9,13,-1,0],24),('full-DWORD-mode-right',0x105,9,[50,9,-1,0],61),
  ('signed-WORD-piece',0x105,-2,[30,-2,-1,0],52),('negative-sentinel-first',0x105,-2,[-2,30,-1,0],-1),
  ('second-eight-byte-pair',0x105,9,[3,4,5,9,-1,0],16)]
 for label,mode,piece,pairs,result in specs:
  c=base(0x4f2720,label,[BUILDING]);put(c,MODE,mode);word(c,record(BUILDING)+18,piece)
  c['stores'].append((record(BUILDING)+84,b'\2'));table=(0x5f5d58+2*160)if mode==5 else(0x5f5f38+2*624)
  c['stores'].append((table,struct.pack('<'+'i'*len(pairs),*pairs)));c['expected_eax']=result&0xffffffff;out.append(c)
 return out

def target_cases():
 out=[]
 for mode,piece in [(5,3),(0x105,3),(0x105,-1)]:
  c=base(0x4f3000,f'mode-{mode:x}-piece-{piece}',[ACTOR,BUILDING],None);put(c,MODE,mode)
  word(c,record(BUILDING)+18,piece);root=BUILDING-piece;target=root+5
  c['stores'] += [(record(root)+84,b'\2'),(record(target),b'\0'*264)]
  put(c,(0x5f5498+2*128)if mode==5 else(0x5f3878+2*960),5)
  word(c,record(target)+38,0xf123);word(c,record(target)+40,0x8abc);put(c,record(target)+12,0xf1234567)
  actor=0xbcd5f8+ACTOR*388;c['stores'].append((actor,b'\xa5'*388));c['watched'].append((actor,388));c['write_ranges'].append((actor,388));out.append(c)
 return out

def refresh_cases():
 out=[]
 for mode in ['no-eligible','WORD-type-high-byte','callback-live-mode-kind']:
  c=base(0x4ee560,mode,(),None);root=record(1)
  c['stores'] += [(root,b'\0'*264),(root,b'\1'),(root+84,b'\1')];word(c,root+16,0x1b7 if mode=='WORD-type-high-byte'else 0xb7)
  if mode=='no-eligible':c['stores'].append((root,b'\0'))
  e=child('refresh-parts',0x4ee4a0,1);e['result_kind']='void';calls=0
  if mode=='callback-live-mode-kind':
   put(c,MODE,5);put(c,0x56f7d8+8,3999);put(c,0x56f7ec+8,1)
   e['mutations']=[(MODE,4,0x105),(root+84,1,2)]
   c['write_ranges'] += [(MODE,4),(root,264)];c['watched'] += [(MODE,4),(root,264)]
   c['read_ranges']=[(0x56f7d8+8,4),(0x56f7ec+8,4)];c['expected_reads']=[[0x56f7d8+8,4]];calls=1
   c['expected_endpoint_arguments']={'refresh-parts':[[1]]}
  c.update(endpoints=[e],expected_endpoint_counts={'refresh-parts':calls});out.append(c)
 return out

def worker_query_cases():
 out=[]
 for mode in ['disabled5','negative-count-upper-EAX','child-full-EAX','exhausted-fresh-count-upper-EAX','fresh-kind-captured-bound-kind','real-slot-helper']:
  c=base(0x4f5d10,mode,[BUILDING]);root=record(BUILDING);c['stores'].append((root+84,b'\2'))
  put(c,0x570070+8,1);put(c,0x5f3878+2*960,3);expected=0;eps=[];counts={}
  if mode=='disabled5':put(c,MODE,5)
  elif mode=='negative-count-upper-EAX':put(c,0x570070+8,0x800001ff);expected=0x80000100
  elif mode=='real-slot-helper':
   put(c,0x5f3878+2*960,0);put(c,0x5f3878+2*960+4,1);put(c,0x5f3878+2*960+8,1000)
   word(c,root+18,0);c['stores'] += [(root+85,b'\0'),(root+86,b'\3')];expected=1
  else:
   e=child('available-slot',0x4ef100,1,0)
   c['write_ranges'] += [(root,264),(0x570070+8,4),(0x570070+12,4)]
   if mode=='child-full-EAX':e['value']=0x12345600;expected=0x12345601;counts={'available-slot':1}
   elif mode=='exhausted-fresh-count-upper-EAX':
    e['mutations']=[(0x570070+8,4,0x80000123)];expected=0x80000100;counts={'available-slot':1}
   else:
    put(c,0x570070+8,2);put(c,0x5f3878+3*960+20,9)
    e['responses']=[dict(value=0,mutations=[(root+84,1,3),(0x570070+12,4,0xffffff00)]),dict(value=2)]
    expected=1;counts={'available-slot':2};c['expected_endpoint_arguments']={'available-slot':[[BUILDING+3],[BUILDING+9]]}
   eps=[e]
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def grid_cases():
 out=[]
 for mode in ['gate-zero','record-BYTE-set','wide-coordinates-high-result','zero-child-result']:
  c=base(0x4d6aa0,mode,[2]);flag=0xbcd728+2*388
  for at,value in [(0xd05708,0 if mode=='gate-zero'else 0x100),(0xbca2cc,2),(0xbca2c8,0x100),(0xbca2d8,3)]:put(c,at,value)
  c['stores'].append((flag,bytes([1 if mode=='record-BYTE-set'else 0])))
  calls=0 if mode in ['gate-zero','record-BYTE-set']else 1;e=child('tile-worker-tag',0x4b5520,1,0x100 if mode=='wide-coordinates-high-result'else 0)
  c.update(endpoints=[e],expected_endpoint_counts={'tile-worker-tag':calls},expected_eax=1 if mode=='wide-coordinates-high-result'else 0)
  if calls:c['expected_endpoint_arguments']={'tile-worker-tag':[[0x100+2*228+3]]}
  out.append(c)
 return out

def definitions(registry):
 from cohort7_lowio_seek_cases import definitions as seek_definitions
 present={int(m['address'],16)for m in registry['members']}
 root=dispatch_cases()+pair_cases()+target_cases()+refresh_cases()+worker_query_cases()+grid_cases()
 return [c for c in root if c['entry']in present]+seek_definitions(registry)
