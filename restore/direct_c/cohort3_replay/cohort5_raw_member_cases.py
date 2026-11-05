"""Private cohort5 recipes for the sixty captured-context raw providers.

Recipes only: importing this file does not build, emulate, launch, or update
metadata.  The shared replay owns execution and the final evidence binding.
Native object/argument addresses remain at SP; C's control stack is separate.
Finite domains below do not establish full gameplay or indirect-call coverage.
"""
import struct
import copy

S, SP, CONTEXT = 0x32000000, 0x30008000, 0x32030000
Q, P, VTABLE = S+0x1000, S+0x10000, S+0x28000
SAVED = (0x12345678, 0x23456789, 0x3456789a, 0x456789ab)
OFFSETS = dict(edi=0, esi=4, ebp=8, ebx=16, edx=20, ecx=24, eax=28, eflags=32)
DETAIL = (0x4493f0,0x449440,0x449550,0x4495c0,0x4495d0,0x449600,0x449630,0x4496b0,0x449b30)
SIDEBAR = (0x401480,0x40d890,0x416fb0,0x4e6660,0x545720,0x546660,0x54f450,0x555e60,0x558bc0)
MISSION = (0x40d7f0,0x4e65a0,0x547ad0)
ACTION_KINDS = {0x44c660:2,0x44c7e0:2,0x44c8a0:3,0x44c9e0:1,0x44cdc0:1,
 0x44d780:4,0x44d870:5,0x44d970:15,0x44da30:16,0x44db20:8,0x44dbd0:9,
 0x44dca0:10,0x44dd60:11,0x44de20:12,0x44dee0:13,0x44df90:14,
 0x44e000:24,0x44e070:25,0x44e0e0:26,0x44e150:27,0x44e1c0:28,
 0x44e230:29,0x44e2a0:17,0x44e5d0:18,0x44e890:23,0x451540:2}
EVENT_LEAVES = (0x44ef60,0x44ef90,0x44f120,0x44f170,0x44f360,0x44f3d0,
 0x44f440,0x44f470,0x44f8c0,0x44fb20,0x44fc00,0x44fce0,0x44fd30)
ENTRIES = set(DETAIL+SIDEBAR+MISSION+EVENT_LEAVES) | set(ACTION_KINDS)
RET4 = {0x401480,0x40d7f0,0x40d890,0x416fb0,0x449440,0x449550,
 0x44f120,0x44fd30,0x4e65a0,0x4e6660,0x545720,0x547ad0,0x54f450,0x555e60,0x558bc0}
COMMON_GAPS = [
 'Mapped nonfaulting original-address inputs; no concurrency, guard faults, exceptions, or host execution.',
 'Only selected direct/recognized virtual targets are visited; other indirect consumers remain unresolved.',
 'Logical results, observed RAM/endpoints, saved registers and cleanup are compared; unconsumed volatile registers and undefined flags are excluded.',
]

def b8(v): return bytes([v&255])
def b16(v): return struct.pack('<H',v&65535)
def b32(v): return struct.pack('<I',v&0xffffffff)
def words(values): return b''.join(b32(v) for v in values)
def put(stores,at,v,width=4): stores.append((at,{1:b8,2:b16,4:b32}[width](v)))

def endpoint(at,arity,kind='void',value=None,**kw):
 d=dict(token=f'raw-boundary/{at:08x}',native=at,compiled_symbols=[f'FUN_{at:08x}'],
        arity=arity,native_cleanup=0,compiled_cleanup=0,result_kind=kind)
 if value is not None:d['value']=value
 d.update(kw);return d

def case(entry,label,receiver=Q,args=(),stores=(),watched=(),writes=(),mask=0xffffffff,
         endpoints=(),eax=0x89abcdef,edx=0x76543210,flags=0x202,**kw):
 cleanup=12 if entry==0x546660 else 4 if entry in RET4 else 0
 snapshot=words([SAVED[3],SAVED[2],SAVED[1],SP-4,SAVED[0],edx,receiver,eax,flags])
 c=dict(id=f'c5-raw/{entry:08x}/{label}',entry=entry,receiver=receiver,
  arguments=list(args),named_arguments=[CONTEXT],initial_eax=eax,initial_edx=edx,
  initial_ebx=SAVED[0],initial_ebp=SAVED[1],initial_esi=SAVED[2],initial_edi=SAVED[3],
  eflags=flags,stores=[*stores,(CONTEXT,snapshot)],watched=list(watched),
  write_ranges=[*writes,(CONTEXT,36)],result_mask=mask,
  result_memory={'named':[CONTEXT+28,4]},native_cleanup=cleanup,named_cleanup=0,
  canonical_cleanup=cleanup,canonical_registers_and_flags=False,
  context_outputs={'named':{name:[CONTEXT+OFFSETS[name],4] for name in ('ebx','ebp','esi','edi')}},
  context_compare=['ebx','ebp','esi','edi'],machine_sp={'named':SP-0x20000},
  borrowed_native_stack=True,endpoints=list(endpoints),budget=12000000,
  gap_domains=list(COMMON_GAPS))
 c.update(kw);return c

def event_record(kind=2,subtype=0,flags=0,amount=3,parameter=1,status=0):
 r=bytearray(124)
 def w(off,v):r[off:off+2]=b16(v)
 w(0,-1);r[2]=kind&255;r[3]=4;w(4,parameter);w(12,amount);w(20,10);w(28,3)
 for off in (36,38,52,82,84):w(off,-1)
 r[40:44]=b32(flags);w(44,2);w(46,2);w(48,status);r[54]=4;r[86]=0;r[96]=subtype&255
 w(100,-1);w(102,-321);w(104,678)
 for off in (4,12,20,28,88):
  w(off+2,int.from_bytes(r[off:off+2],'little'));w(off+4,-1);w(off+6,-1)
 return bytes(r)

def detail_cases():
 out=[]
 # These original constant tables remain in the shared native data seed.
 for entry in DETAIL:
  if entry==0x449440:
   settings=[(mode,value) for mode in range(8) for value in (0,1,8,12,21,27,36,37,0xffffffff)]
  elif entry==0x449550:
   settings=[(kind,value) for kind in (0,1,2,13,16,19,23,29,255) for value in (-1,0,21,27,37,255)]
  elif entry==0x4493f0:settings=[(kind,0) for kind in (0,1,2,13,19,20,23,29)]
  else:settings=[(kind,subtype) for kind,subtype in ((0,0),(1,0),(1,1),(1,2),(1,3),(2,0),(19,0),(20,0),(23,0),(255,255))]
  for i,(first,second) in enumerate(settings):
   r=bytearray(event_record(kind=1 if entry==0x449440 else first,subtype=first if entry==0x449440 else second))
   args=[second] if entry in (0x449440,0x449550) else []
   watched=[(Q,124)];writes=[]
   if entry==0x4493f0:watched.append((0x6057f4,256));writes.append((0x6057f4,256))
   out.append(case(entry,f'kind-range-{i}',args=args,stores=[(Q,bytes(r))],watched=watched,writes=writes,
    mask=255 if entry in (0x449440,0x449550,0x4495d0,0x449600,0x449630,0x4496b0) else 0xffffffff,
    eax=0x89abcdef if i%2 else 0xfedcba10,flags=0x202 if i%2 else 0xac7,
    coverage={'domain':'original kind/subtype tables and signed selection boundaries','settings':[first,second]}))
 # Actual caller stack alias: object is above the original return slot, not on C's control stack.
 r=event_record(kind=2)
 out.append(case(0x449630,'caller-stack-object',receiver=SP+4,stores=[(SP+4,r)],watched=[(SP+4,124)],mask=255))
 return out

def event_leaf_cases():
 out=[]
 variants=[(1,m,f) for m in range(8) for f in (0,255)] + [(0,0,0),(2,0,0),(2,4,0),(19,0,0),(19,4,0),(20,3,0),(23,0,0),(29,0,0),(255,255,255)]
 for entry in EVENT_LEAVES:
  if entry==0x44ef60:
   configs=[(-1,0,0),(0,0,0),(0,1,0),(0,1,16),(1,1,16),(149,150,0xffffffff)]
   for i,(link,count,fl) in enumerate(configs):
    r=bytearray(event_record());r[52:54]=b16(link);st=[(Q,bytes(r)),(0x785d70,b32(count))]
    if 0<=link<150:st.append((0x785d74+124*link+40,b32(fl)))
    out.append(case(entry,f'extension-{i}',stores=st,watched=[(Q,124)],mask=255,
       coverage={'domain':'negative/out-of-range link, empty list and linked completion flag','settings':[link,count,fl]}))
  elif entry==0x44ef90:
   for i,fl in enumerate((0,4,0x80000100,0xffffffff)):
    r=bytearray(event_record(flags=fl));r[50]=255
    out.append(case(entry,f'completion-{i}',stores=[(Q,bytes(r))],watched=[(Q,124)],writes=[(Q,124)],mask=None))
  else:
   for i,(kind,subtype,flag) in enumerate(variants):
    r=bytearray(event_record(kind,subtype));r[86]=flag
    args=[i%5] if entry==0x44f120 else [i%6] if entry==0x44fd30 else []
    out.append(case(entry,f'message-table-{i}',args=args,stores=[(Q,bytes(r))],watched=[(Q,124)],
      coverage={'domain':'kind switch, in/out-of-range subtype, flag and selector','settings':[kind,subtype,flag,*args]}))
 # Visit the actual linked-record/mapping branch as well as direct policy
 # selection. The native mapping at7854A8 remains the pinned image seed.
 for selector,offset in enumerate((36,38,82,84)):
  for policy in (6,7):
   r=bytearray(event_record(1,1));r[offset:offset+2]=b16(0);r[106+selector]=policy
   child=event_record(1,2)
   out.append(case(0x44fd30,f'linked-policy-{selector}-{policy}',args=[selector],
    stores=[(Q,bytes(r)),(0x785d70,b32(1)),(0x785d74,child)],
    watched=[(Q,124),(0x785d70,128)],
    coverage={'domain':'valid linked record, native follow-up mapping and policy boundary','settings':[selector,policy]}))
 return out

# Broad but bounded observer regions include the real descendants used by the
# effects below. They exclude immutable native binary bytes and C scratch.
ACTION_REGIONS=[(0x5d2c40,0x200),(0x604900,0x1300),(0x776400,0x9400),(0x784800,0x6000),
 (0x78d780,0x100),(0x7999d8,0x800),(0xbcd780,0x1000),(0xd38000,0x3900),
 (0xe20750,0x600),(0xe38d00,0x600),(0xe94e00,0x640),(0xea0000,0xb000),(0xf53e00,0x3900)]

def action_fixture(kind=2,flags=0,amount=3,parameter=1,status=0,subtype=0):
 st=[(a,bytes(n)) for a,n in ACTION_REGIONS]
 st += [(Q,event_record(kind,subtype,flags,amount,parameter,status)),(0x785d70,b32(1)),
  (0x785d74,event_record(21,flags=1)),(0x7848d8,b16(-3500)),(0x78d7bc,b32(-3490)),
  (0x78d7c0,b32(4)),(0x5d9a40,b32(-1)),(0x78a634,b8(1)),(0x605a3c,b32(0)),
  (0xeaac28,b32(0)),(0xea1888,b32(70)),(0xea447c,b32(20)),
  (0xea5cec,b16(1)),(0xea5cee,b16(1)),(0x784cb0,b32(1)),
  (0x77646c+4*(parameter+3*36),b32(20)),(0x5d2c48+parameter*8,words([20,20])),
  (0xd3b374,b32(0x1234567f)),(0xd3b380,b32(0x87654321)),(0xd3b384,b32(0x12345678)),
  (0xd38360+28,b32(50)),(0x604950,b32(1)),(0xe38dda,b8(2))]
 # Resource records0..60, exact native106-byte record unit.
 resources=bytearray(61*106)
 for i in range(61):resources[i*106]=1;resources[i*106+4]=(i*7+3)&255
 st.append((0x77de00,bytes(resources)))
 return st

def action_endpoints():
 # The real child graph executes; only typed publication/UI/accounting edges
 # are controlled. nullable captures all30 bytes only for nonnull stack packets.
 pub=endpoint(0x4e0cf0,5,private_input_args={'3':dict(role='message-packet30',width=30,nullable=True)})
 return [pub,endpoint(0x4e96c0,1),endpoint(0x417ae0,1),endpoint(0x469c60,1),
  endpoint(0x4425c0,1,'full32',1),endpoint(0x443000,2,'full32',1),endpoint(0x443040,2,'full32',1),
  endpoint(0x47c2b0,5),endpoint(0x47fe60,4,'full32',0),endpoint(0x4806c0,3,'full32',0),
  endpoint(0x4b3250,1),endpoint(0x4b4800,5,'full32',0),endpoint(0x4b5420,1),
  endpoint(0x4c7f30,0),endpoint(0x4cde40,2,'full32',0),endpoint(0x4cde60,2,'full32',0),
  endpoint(0x4ea9c0,1,'full32',0),endpoint(0x516f90,5),endpoint(0x5173a0,4),
  endpoint(0x53b090,1),endpoint(0x5596d0,0),
  endpoint(0x55f5a1,1,'full32',0,native_cleanup=4,compiled_arity=2,compiled_receiver_arg=0,native_receiver=0xe92bf8),
  endpoint(0x55f561,1,native_cleanup=4,compiled_arity=2,compiled_receiver_arg=0,native_receiver=0xe92bf8),
  endpoint(0x55f449,3,'full32',0,native_cleanup=12,compiled_arity=4,compiled_receiver_arg=0,native_receiver=0xe92bf8)]

def action_cases():
 out=[]
 for entry,kind in ACTION_KINDS.items():
  variants=[('active',0,3,0,0),('completed-edge',4,-1,9,255),('amount-boundary',0,0,0,2)]
  if entry in (0x44c9e0,0x44cdc0):variants=[('active',0,3,0,1),('status-finished',0,3,8,0),('subtype-edge',0,0,4,6)]
  if entry==0x44e890:variants=[('active',0,3,0,0),('status-finished',0,-1,9,1),('late-phase',0,0,6,2)]
  for label,fl,amount,status,subtype in variants:
   parameter=37 if entry==0x44c9e0 else 1
   st=action_fixture(kind,fl,amount,parameter,status,subtype)
   if entry==0x44e2a0:
    # Commodity10 is resource slot1; slot0 is the native false-result arm.
    st.extend([(Q+28,b16(10)),(Q+30,b16(10))])
   if entry==0x451540:
    # Full event update sees a live effect record and completion/extension state.
    st.append((Q+52,b16(-1)))
   c=case(entry,label,stores=st,watched=[(Q,124),*ACTION_REGIONS],
    writes=[(Q,124),*ACTION_REGIONS],mask=None,endpoints=action_endpoints(),
    coverage={'domain':'real due predicate, effects, packet construction and terminal/completed boundary',
              'settings':dict(kind=kind,flags=fl,amount=amount,status=status,subtype=subtype)})
   c['gap_domains'] += ['Empty building/actor collections avoid unrelated creation/destruction loops; only stated selected effect kind is visited.',
    'Follow-up links are -1: recursive/immediate scheduling chains are not newly covered by this packet.',
    'Publication is a controlled endpoint; queue/audio/UI behavior is outside these cases.']
   out.append(c)
   if entry==0x44cdc0 and label=='active':
    history=copy.deepcopy(c);history['id']+='-stack-history'
    history['stores'].append((SP-0x200,bytes((i*37+19)&255 for i in range(0x200))))
    history['gap_domains'].append('Original stack history is deliberately nonuniform, including packet bytes not initialized by this caller.')
    out.append(history)
 # Dispatcher additionally takes the genuine no-effect/default arm.
 for kind in (0,6,7,255):
  out.append(case(0x44c660,f'dispatch-default-{kind}',stores=action_fixture(kind),
   watched=[(Q,124),*ACTION_REGIONS],writes=[(Q,124),*ACTION_REGIONS],mask=None,endpoints=action_endpoints()))
 return out

def sidebar_cases():
 out=[]
 for entry in SIDEBAR:
  if entry<0x500000:
   for i,(size,special,x,y,kind) in enumerate(((0,0,3,4,0),(1,0,-1,2,1),(2,9,10,-1,2))):
    obj=bytearray(0x7000);obj[0x54+52*kind:0x58+52*kind]=b32(size);obj[0x58+52*kind:0x5c+52*kind]=b32(special)
    record=bytearray(264);record[6:8]=b16(x);record[8:10]=b16(y);record[18:20]=b16(kind)
    st=[(Q,bytes(obj)),(0x7999d8+264,bytes(record)),(0xbc9738,words([0x1234,0x5678]))]
    ep=[endpoint(0x47c480,3),endpoint(0x477a40,3)]
    out.append(case(entry,f'square-{i}',args=[1],stores=st,
     watched=[(Q,len(obj)),(0x7999d8+264,264),(0xbc9738,8)],writes=[(0xbc9738,8)],mask=None,endpoints=ep,
     coverage={'domain':'signed coordinates, original264-byte record and52-byte descriptor units, zero/one/two-cell side','settings':[size,special,x,y,kind]},
     expected_endpoint_counts={ep[0]['token']: ((1+size*size) if entry==0x40d890 else size*size),
       ep[1]['token']:size*size if (entry==0x401480 and special!=9) or (entry==0x4e6660 and special==0) else 0}))
  else:
   wrapper={0x545720:(0x1e5c,167),0x54f450:(0x5c88,513),0x555e60:(0x4ee0,437),0x558bc0:(0x1cbc,158)}
   for i,(size,x,y,kind,threshold) in enumerate(((0,3,4,0,0),(1,-1,2,1,1),(2,10,-1,2,0))):
    obj=bytearray(0x7000);obj[0x50:0x54]=b32(threshold)
    offset,tag=wrapper.get(entry,(0,167));table=Q+offset if entry!=0x546660 else P
    table_data=bytearray(256);table_data[8+52*kind:12+52*kind]=b32(size)
    if entry!=0x546660:obj[offset:offset+256]=table_data
    rec=bytearray(264);rec[6:8]=b16(x);rec[8:10]=b16(y);rec[18:20]=b16(kind)
    st=[(Q,bytes(obj)),(0x7999d8+264,bytes(rec)),(0xbca2d8,b32(1000))]
    if entry==0x546660:st.append((P,bytes(table_data)))
    regions=[]
    for row in range(size):
     tile=1000+x+y*228+row*228
     for base,width in ((0xaf598c,4),(0xb4e6fc,2),(0xb285cc,1),(0xb67d1c,1),(0xae8e7c,1),(0xa9cc1c,2),(0xadc36c,1)):
      at=base+tile*width;payload=bytes([0xff])*(size*width);st.append((at,payload));regions.append((at,len(payload)))
    args=[1,table,tag] if entry==0x546660 else [1]
    out.append(case(entry,f'clear-square-{i}',args=args,stores=st,
      watched=[(Q,len(obj)),(0x7999d8+264,264),*regions],writes=regions,mask=None,
      coverage={'domain':'actual wrapper-to-clear descendant, signed tile origin and terrain bit masks','settings':[size,x,y,kind,threshold]}))
 return out

def mission_cases():
 out=[]
 # Controlled virtual tile commit is below the tested mission method. Native
 # thiscall(index)/RET4 matches semantic C(self,index)/cdecl; record loops run.
 commit=endpoint(0x5468d0,1,native_cleanup=4,compiled_symbols=['restore_monument_commit_footprint'],
  compiled_arity=2,compiled_receiver_arg=0,native_receiver=Q)
 for entry in MISSION:
  for variant,start in enumerate((2,5)):
   count=25 if entry==0x40d7f0 else 167 if entry==0x547ad0 else 1
   first=start-(1 if entry==0x547ad0 and variant else 0)
   obj=bytearray(0x7000);obj[:4]=b32(VTABLE)
   vt=bytearray(64);vt[24:28]=b32(0x5468d0);vt[60:64]=b32(0x545f70)
   recs=bytearray((count+2)*264)
   for i in range(count+2):
    recs[i*264+18:i*264+20]=b16(1 if entry==0x547ad0 and variant and i==1 else 0)
    recs[i*264+104:i*264+108]=b32(17 if i%2 else 0)
   st=[(Q,bytes(obj)),(VTABLE,bytes(vt)),(0x7999d8+first*264,bytes(recs))]
   out.append(case(entry,f'record-group-{variant}',args=[start],stores=st,
    watched=[(Q,len(obj)),(0x7999d8+first*264,len(recs))],
    writes=[(0x7999d8+first*264,len(recs))],mask=None,endpoints=[commit],
    expected_endpoint_counts={commit['token']:count},
    coverage={'domain':'actual virtual receiver, per-record writes and25/167/one iteration contracts',
              'settings':dict(start=start,first=first,count=count)},
    gap_domains=[*COMMON_GAPS,'Only recognized count getter545F70 and controlled tile commit5468D0; other vtables and tile rendering remain outside this comparison.']))
 return out

def definitions(registry):
 present={int(m['address'],16) for m in registry['members']}
 out=[c for c in detail_cases()+event_leaf_cases()+action_cases()+sidebar_cases()+mission_cases() if c['entry'] in present]
 assert len({c['id'] for c in out})==len(out)
 assert {c['entry'] for c in out}==ENTRIES&present
 return out
