"""Connected callback registration and floating initialization recipes."""
import struct
from fp_control_cases import CONTROL_WORDS
S=0x32000000
A,B,D=S+0x10000,S+0x20000,S+0x30000
BASE,NEXT=0x1266a8c,0x1266a88
QUERY=0x31000800

def word(value):return struct.pack('<I',value&0xffffffff)

def base_case(entry,label,args,stores,watched,writes,result=0xffffffff):
 return dict(id=f'crt-lifecycle/{entry:08x}/{label}',entry=entry,
  receiver=0x22222222,arguments=args,named_arguments=args,eflags=0x202,
  stores=stores,watched=watched,write_ranges=writes,result_mask=result,
  native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
  canonical_registers_and_flags=False)

def callback_cases():
 out=[]
 modes=('spare','grow','grow-fail','unaligned-grow','negative-grow',
        'capacity-mutates','realloc-mutates','next-slot-alias','base-slot-alias')
 for entry in (0x5618e6,0x561953):
  for callback in (0,0x13579bdf,0xfdb97531):
   for mode in modes:
    base=A;next_=A+8;capacity=8;replacement=B;size_mutations=[];realloc_mutations=[]
    if mode=='spare':capacity=64
    elif mode=='grow-fail':replacement=0
    elif mode=='unaligned-grow':next_=A+11
    elif mode=='negative-grow':next_=A-1;capacity=0
    elif mode=='capacity-mutates':
     capacity=64;size_mutations=[(BASE,4,D),(NEXT,4,D+12)]
    elif mode=='realloc-mutates':realloc_mutations=[(BASE,4,D),(NEXT,4,D+13)]
    elif mode.endswith('slot-alias'):
     base=BASE-12;next_=NEXT if mode=='next-slot-alias'else BASE;capacity=64
    grow=mode in ('grow','grow-fail','unaligned-grow','negative-grow','realloc-mutates')
    stores=[(p-16,b'\xa5'*0x400)for p in (A,B,D)]+[(NEXT-4,b'\xa5'*16),(BASE,word(base)),(NEXT,word(next_))]
    watched=[(p-16,0x400)for p in (A,B,D)]+[(NEXT-4,16)]
    c=base_case(entry,f'{mode}/{callback:08x}',[callback],stores,watched,
     [(p-16,0x400)for p in (A,B,D)]+[(NEXT,8)])
    observe=[(BASE,4),(NEXT,4)]
    c['endpoints']=[
     dict(token='capacity',native=0x5647c2,compiled_symbols=['FUN_005647c2'],arity=1,
      native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=capacity,
      mutations=size_mutations,observe=observe),
     dict(token='realloc',native=0x5645a0,compiled_symbols=['FUN_005645a0'],arity=2,
      native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=replacement,
      mutations=realloc_mutations,observe=observe)]
    c['expected_endpoint_counts']={'capacity':2 if grow else 1,'realloc':int(grow)}
    out.append(c)
 return out

def floating_cases():
 out=[]
 pointers=[dict(address=at,native=target,compiled_symbol=f'FUN_{target:08x}')for at,target in
           ((0x604c30,0x56452a),(0x604c38,0x564225),(0x604c3c,0x564165),(0x604c44,0x56452a))]
 for entry in (0x56413c,0x561896):
  for cw in CONTROL_WORDS:
   for branch in ('no-module','no-query','feature-zero','feature-one'):
    initialize=entry==0x561896
    stores=[(0x12663c0,b'\xa5'*12),(0x604c2c,b'\xa5'*32)]
    watched=[(0x12663c0,12)]
    writes=[]
    if initialize:
     watched += [(0x604c2c,4),(0x604c34,4),(0x604c40,4),(0x604c48,4)]
     writes += [(0x12663c4,4),(0x604c30,24)]
    else:watched += [(0x604c2c,32)]
    c=base_case(entry,f'{cw:x}/{branch}',[],stores,watched,writes,None if initialize else 0xffffffff)
    c.update(initial_fpcw=cw,compare_x87=True)
    if initialize:c['expected_function_pointers']=pointers
    module=branch!='no-module';query=branch.startswith('feature')
    c['endpoints']=[
     dict(token='module',import_name='GetModuleHandleA',arity=1,native_cleanup=4,
      compiled_cleanup=4,result_kind='full32',value=0x6a100000 if module else 0),
     dict(token='query-address',import_name='GetProcAddress',arity=2,native_cleanup=8,
      compiled_cleanup=8,result_kind='full32',value=QUERY if query else 0),
     dict(token='feature',controlled_address=QUERY,arity=1,native_cleanup=4,
      compiled_cleanup=4,result_kind='full32',value=int(branch=='feature-one'))]
    c['expected_endpoint_counts']={'module':1,'query-address':int(module),'feature':int(query)}
    out.append(c)
 return out

def definitions(registry):
 assert {0x5618e6,0x561953,0x56413c,0x561896}<={int(m['address'],16)for m in registry['members']}
 return callback_cases()+floating_cases()
