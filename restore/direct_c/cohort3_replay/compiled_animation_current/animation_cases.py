"""Native-width, physical-stack and live animation-table cases."""
import struct
SELF,FRAME,ENTRIES,ROWS,ALT_ROWS,ALT_ENTRIES=0x32001000,0x32040000,0x32002000,0x32003000,0x32004000,0x32005000
TABLE=0x5be9b8
SP=0x30008000
NAMES=['eax','ecx','edx','ebx','esp','ebp','esi','edi']
LIVE=[0xa5b6c7d8,SELF,0x33333333,0x12345678,SP,0x23456789,0x3456789a,0x456789ab]

def raw(n,w=4):return (n&((1<<(w*8))-1)).to_bytes(w,'little')
def put(c,a,n,w=4):c['stores'].append((a,raw(n,w)))
def base(entry,label):
 return dict(id='c7-animation/%08x/'%entry+label,entry=entry,receiver=SELF,
  arguments=[],named_arguments=[SELF],canonical_arguments=[SELF],eflags=0xad7,
  stores=[],watched=[],write_ranges=[],result_mask=None,native_cleanup=0,
  named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False,
  endpoints=[],expected_endpoint_counts={},expected_endpoint_arguments={},
  compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
  initial_x87=['0000000000000080ff3f','000000000000008000c0'])

def link_cases():
 out=[]
 for label,actor,alias in [('zero',0,False),('ordinary',3,False),
   ('negative',0xffffffff,False),('nonzero-DWORD-zero-WORD',0x40000000,False),
   ('WORD-write-aliases-state-source',3,True)]:
  src=(0xbcd630+actor*388)&0xffffffff;obj=src-18 if alias else SELF
  c=base(0x41fd80,label);c.update(receiver=obj,arguments=[actor],named_arguments=[FRAME],
   native_cleanup=4,routes=['native','named'],
   routes_reason='Complete existing C register-model API; no retained typed-helper canonical symbol. All input registers and the genuine native SP are supplied.',
   machine_sp={'named':SP-0x20000},borrowed_native_stack=True,
   context_compare=NAMES+['eflags'],context_flag_mask=0xcc5,
   context_outputs={'native':{},'named':{**{n:[FRAME+i*4,4]for i,n in enumerate(NAMES)},'eflags':[FRAME+32,4]}})
  registers=LIVE.copy();registers[1]=obj
  c['stores']=[(obj-8,b'\xa5'*48),(src,b'\xd7'),(FRAME,b''.join(raw(x)for x in registers+[0xad7,0]))]
  c['watched']=[(obj-8,48),(src,1)];c['write_ranges']=[(obj+18,3),(FRAME,40)]
  state=(actor&255)if alias else 0xd7
  c['authored_final_memory']=[(obj+18,raw(actor,2).hex()),(obj+20,raw(state if actor else 0xa5,1).hex())]
  c['expected_eax']=((actor&0xffffff00)|state)if actor else 0
  out.append(c)
 return out

def advance_cases():
 configs=[('forward',{}),('reverse',dict(direction=1)),
  ('linked-mismatch',dict(linked=3,mismatch=True)),('linked-negative-match',dict(linked=-1)),
  ('state2',dict(state=2)),('state3',dict(state=3)),('kind-negative',dict(kind=-1)),
  ('kind253',dict(kind=253)),('index-negative',dict(index=-1)),('index-count',dict(index=2)),
  ('clamp-forward',dict(state=1,frame=3)),('clamp-reverse',dict(state=1,frame=0,direction=1)),
  ('clamp-wrapped-negative',dict(state=1,frame=0x7fffffff)),
  ('fallback-no-rows',dict(frame=3,fallback=True)),
  ('fallback-empty-choices',dict(frame=3,fallback=True,rows=1)),
  ('transition-forward',dict(frame=3,transition=True)),
  ('transition-reverse',dict(frame=3,transition=True,next_direction=1)),
  ('transition-fresh-tables',dict(frame=3,transition=True,reload=True,next_direction=1)),
  ('transition-invalid-next',dict(frame=3,transition=True,next_index=2,fallback=True)),
  ('maximum-aliases-live-frame',dict(frame=3,alias=True))]
 out=[]
 for label,v in configs:
  c=base(0x41fdb0,label);obj=bytearray(b'\xa5'*32)
  kind,index,state,frame,direction=[v.get(k,d)for k,d in [('kind',0),('index',0),('state',0),('frame',1),('direction',0)]]
  linked=v.get('linked',0)
  for at,val,w in [(0,kind,4),(4,index,4),(8,frame,4),(16,state,2),(18,linked,2),(20,0x77,1),(21,direction,1)]:obj[at:at+w]=raw(val,w)
  c['stores']=[(SELF-8,b'\x5a'*48),(SELF,bytes(obj)),(ENTRIES,b'\xa7'*96),(ROWS,b'\xb6'*80),(ALT_ROWS,b'\xc8'*80),(ALT_ENTRIES,b'\xd9'*96),(TABLE,b'\xe3'*40)]
  c['watched']=[(SELF-8,48),(TABLE,40),(ENTRIES,96),(ROWS,80),(ALT_ROWS,80),(ALT_ENTRIES,96)]
  c['write_ranges']=[(SELF,32),(TABLE,40)]
  for at,val in [(TABLE+16,SELF-4 if v.get('alias')else ENTRIES),(TABLE+20,2),(TABLE+24,ROWS),(TABLE+28,v.get('rows',1 if v.get('transition')else 0)),(ENTRIES+12,3),(ENTRIES+60,7),(ALT_ENTRIES+60,9)]:put(c,at,val)
  if linked:
   addr=0xbcd630+linked*388;put(c,addr,0x11 if v.get('mismatch')else 0x77,1);c['watched'].append((addr,1))
  for rows in [ROWS,ALT_ROWS]:
   for at,val in [(rows,0),(rows+8,-1),(rows+16,-1),(rows+24,-1),(rows+32,-1)]:put(c,at,val)
   put(c,rows+4,direction,1)
  if v.get('transition'):
   nxt=v.get('next_index',1);nd=v.get('next_direction',0)
   for rows in [ROWS,ALT_ROWS]:put(c,rows+8,nxt);put(c,rows+12,nd,1)
   rng=dict(token='random',native=0x562348,compiled_symbols=['FUN_00562348'],arity=0,
    native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=0,
    clobber_ecx=0xdead1234,clobber_edx=0xbeef5678,
    observe=[(SELF+4,4),(SELF+8,4),(TABLE+16,4),(TABLE+24,4)])
   if v.get('reload'):rng['mutations']=[(TABLE+24,4,ALT_ROWS),(TABLE+16,4,ALT_ENTRIES)]
   c.update(endpoints=[rng],expected_endpoint_counts={'random':1},expected_endpoint_arguments={'random':[[]]})
  final=bytearray(obj)
  def set_(at,val,w=4):final[at:at+w]=raw(val,w)
  if v.get('mismatch'):set_(18,0,2);set_(20,0,1);set_(16,3,2)
  elif state not in [2,3]and 0<=kind<253 and 0<=index<2:
   f=(frame+(-1 if direction else 1))&0xffffffff;f=f if f<0x80000000 else f-0x100000000
   set_(8,f)
   if v.get('transition'):
    set_(4,v.get('next_index',1));set_(21,v.get('next_direction',0),1)
    if not v.get('fallback'):set_(8,(9 if v.get('reload')else 7)if v.get('next_direction')else 0)
   elif state==1:set_(8,max(0,min(3,f)))
   if v.get('fallback'):set_(16,3,2);set_(4,-1)
  c['authored_final_memory']=[(SELF,final.hex())]
  out.append(c)
 return out

def definitions(registry=None):return link_cases()+advance_cases()
