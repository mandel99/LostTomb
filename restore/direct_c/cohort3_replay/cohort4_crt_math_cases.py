"""Finite live-x87 and math-error cases; reserved/uninitialized fields are excluded."""
import struct
S=0x32000000
SP=0x30008000
RECORD,CONTROL,OPERAND,RESULT,EXPONENT=[S+i*0x1000 for i in range(1,6)]

def w(value):return struct.pack('<I',value&0xffffffff)
def q(value):return struct.pack('<Q',value&0xffffffffffffffff)
def words(value):return [value&0xffffffff,value>>32]
def raw80(bits):
 fraction=bits&0xfffffffffffff;exponent=(bits>>52)&0x7ff;sign=(bits>>48)&0x8000
 if exponent==0x7ff:mantissa=(1<<63)|(fraction<<11);power=0x7fff
 elif exponent:mantissa=((1<<52)|fraction)<<11;power=exponent-1023+16383
 elif fraction:
  leading=fraction.bit_length()-1;mantissa=fraction<<(63-leading);power=leading-1074+16383
 else:mantissa=power=0
 return (mantissa.to_bytes(8,'little')+(power|sign).to_bytes(2,'little')).hex()

def base(entry,label,args,named=None,result=None):
 return dict(id=f'c4-math/{entry:08x}/{label}',entry=entry,receiver=0x89abcdef,
  arguments=args,named_arguments=args if named is None else named,eflags=0x202,
  initial_edx=0x76543210,stores=[],watched=[],write_ranges=[],result_mask=result,
  native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False,
  canonical_owner='crt_runtime_tail_entries.obj'if entry==0x567500 else'crt_asin_entries.obj')

def dispatch_endpoint(binary=False):
 segments=[[0,32]]if binary else[[0,16],[24,8]]
 return dict(token='math-dispatch',native=0x56971f,compiled_symbols=['restore_crt_math_dispatch'],
  arity=3,result_kind='void',native_cleanup=0,compiled_cleanup=0,
  private_input_args={1:dict(role='exception-defined-fields',segments=segments),2:dict(role='saved-control',width=2)})

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']};out=[]
 # Classifier returns the complete high operand only for exponent7FF.
 for high in (0,0x80000000,0x3fefffff,0x3ff00000,0x7fefffff,0x7ff00000,0xfff00000,0x7ff00001,0x7ff80000):
  c=base(0x5675e8,f'{high:x}',[0x12345678,high],[high],0xffffffff)
  c['expected_result']=high if high&0x7ff00000==0x7ff00000 else high&0x7ff00000;out.append(c)
 for cw in (0x7f,0x27f,0x37f,0x77f,0xb7f,0xf7f):
  for upper in (0x13570000,0xfedc0000):
   slot=upper|cw;c=base(0x567575,f'{cw:x}/{upper:x}',[slot],[SP+0x100],0xffffffff)
   c.update(watched=[(SP+4,4)],route_watched={'named':[(SP+0x100,4)]},route_stores={'named':[(SP+0x100,w(slot).hex())]},compare_x87=True,result_registers=dict(native='edx',canonical='edx',named='eax'),expected_result=(cw&0x300)|0x7f)
   out.append(c)
 for classification in (0x7ff00001,0x7ff80000,0xfff80000):
  for bits in (0x7ff0000000000001,0x7ff8000012345678):
   c=base(0x56758c,f'{classification:x}/{bits:x}',[],[classification],0xffffffff)
   c.update(initial_eax=classification,initial_x87=[raw80(bits)],compare_x87=True,compare_x87_values=True,expected_result=7 if classification&0x80000 else 1)
   out.append(c)
 # Exercise both the physical ST0 bridge and the explicit live-input core.
 operands=(0,0x8000000000000000,0x3fe0000000000000,0xbfe0000000000000,
  0x3ff0000000000000,0xbff0000000000000,0x3ff0000000000001,
  1,0x0010000000000000,0x7ff0000000000000,0x7ff8000012345678,0x7ff0000000000001)
 for entry in (0x562d00,0x562d1d):
  for cw in (0x27f,0x37f,0xb7f):
   for bits in operands:
    high=bits>>32;classification=high if high&0x7ff00000==0x7ff00000 else high&0x7ff00000
    flags=0x242 if high&0x7ff00000==0x7ff00000 else 0x202
    args=[]if entry==0x562d00 else words(bits)
    named=args if entry==0x562d00 else[classification,flags,*words(bits),0x76543210]
    c=base(entry,f'{cw:x}/{bits:x}',args,named)
    c.update(initial_fpcw=cw,eflags=flags,initial_eax=classification,initial_x87=[raw80(bits)],compare_x87_values=True,stores=[(0x12663c0,w(1))])
    out.append(c)
 # Actual80 input has extra precision beyond its materialized binary64 argument.
 c=base(0x562d00,'extended-half',[]);c.update(initial_x87=['0100000000000080fe3f'],compare_x87_values=True,stores=[(0x12663c0,w(1))]);out.append(c)
 for entry in (0x567517,0x567500):
  for cw in (0x27f,0x37f,0xb7f):
   for bits in (0x3fe0000000000000,0x8000000000000000,0x7ff8000012345678):
    first=0x3ff8000000000000;second=0xc004000000000000
    args=[0x13570000|cw,0x2468ace0,*words(first)]+(words(second)if entry==0x567500 else[])
    named=[3,0x604960,0x16 if entry==0x567500 else 0xe,*words(first)]+(words(second)if entry==0x567500 else[])+[SP+0x100]
    c=base(entry,f'{cw:x}/{bits:x}',args,named)
    c.update(initial_eax=3,receiver=0x604960,initial_edx=0x16 if entry==0x567500 else 0xe,
     initial_fpcw=0x27f,initial_x87=[raw80(bits)],compare_x87_values=True,route_stores={'named':[(SP+0x100,w(0x13570000|cw).hex())]},
     endpoints=[dispatch_endpoint(entry==0x567500)],expected_endpoint_counts={'math-dispatch':1})
    out.append(c)
 for cw in (0x27f,0x37f,0xb7f):
  for status in (0,0x20):
   bits=0x3fe0000000000000;saved=0x13570000|cw
   c=base(0x56760b,f'{cw:x}/{status:x}',words(bits),[cw,*words(bits),0xe,0x604960])
   c.update(stack_prefix=dict(native=[saved],canonical=[saved]),native_cleanup=4,canonical_cleanup=4,
    initial_x87=[raw80(bits)],initial_fpcw=0x27f,initial_fpsw=status,receiver=0x604960,initial_edx=0xe,
    compare_x87_values=True,endpoints=[dispatch_endpoint()],expected_endpoint_counts={'math-dispatch':int(cw!=0x27f and status&0x20!=0)})
   out.append(c)
 for entry in (0x568532,0x568540):
  for status in (0,1,0x25,0x4100):
   c=base(entry,f'{status:x}',[],result=0xffffffff);c.update(initial_fpsw=status,compare_x87=True,expected_result=status);out.append(c)
 for cw in (0x7f,0x27f,0x37f,0xb7f):
  for value,mask in ((0,0),(0x200,0x300),(0xc00,0xc00),(0x12345678,0xffff0000)):
   c=base(0x56854f,f'{cw:x}/{value:x}/{mask:x}',[value,mask],[value,mask,0x89abcdef,SP+8],0xffffffff)
   c.update(initial_fpcw=cw,watched=[(SP+8,4)],compare_x87=True,expected_result=cw);out.append(c)
 for exceptions in (0,1,4,8,0x10,0x20,0x3d):
  c=base(0x568572,f'{exceptions:x}',[exceptions]);c.update(compare_x87=True,compare_x87_values=True);out.append(c)
 for entry in (0x568448,0x568471):
  for bits in (0,0x8000000000000000,1,0x000fffffffffffff,0x0010000000000000,0x3ff8000000000000,0xc004000000000000,0x7ff0000000000000,0x7ff8000012345678):
   exponents=(-1024,0,1024)if entry==0x568448 else(EXPONENT,)
   for exponent in exponents:
    c=base(entry,f'{bits:x}/{exponent:x}',[*words(bits),exponent]);c.update(compare_x87_values=True)
    if entry==0x568471:c.update(stores=[(EXPONENT,b'\xa5'*16)],watched=[(EXPONENT,16)],write_ranges=[(EXPONENT,4)])
    out.append(c)
 for exceptions in (0,8,4,1,2,0x10,0x1f):
  for control in (0,0x37f,0x77f,0xb7f,0xf7f):
   for bits in (0x3ff8000000000000,0xbff8000000000000):
    c=base(0x56820b,f'{exceptions:x}/{control:x}/{bits:x}',[exceptions,RESULT,control],result=0xffffffff)
    c.update(stores=[(RESULT,q(bits)+b'\xa5'*8)],watched=[(RESULT,16)],write_ranges=[(RESULT,8)],compare_x87_values=True)
    out.append(c)
 for exceptions in (0,1,2,4,8,0x10,0x1f):
  for mutated in (False,True):
   c=base(0x567f58,f'{exceptions:x}/{int(mutated)}',[RECORD,CONTROL,exceptions,0x16,OPERAND,RESULT])
   c.update(stores=[(RECORD,b'\xa5'*96),(CONTROL,w(0x37f)),(OPERAND,q(0x3ff8000000000000)),(RESULT,q(0xbfe0000000000000))],
    watched=[(RECORD,96),(CONTROL,4),(OPERAND,8),(RESULT,8)],write_ranges=[(RECORD,88),(CONTROL,4),(RESULT,8)],compare_x87_values=True)
   e=dict(token='raise-ieee',import_name='RaiseException',arity=4,result_kind='void',native_cleanup=16,compiled_cleanup=16,
    private_input_args={3:dict(role='record-pointer',width=4)},observe=[[RECORD,4],[RECORD+4,4],[RECORD+8,4],[RECORD+12,4],[RECORD+64,4],[RECORD+68,4]])
   if mutated:e['mutations']=[[RECORD,4,9],[RECORD+8,4,0x1f],[RECORD+64,4,0],[RECORD+68,4,0x40040000]]
   c.update(endpoints=[e],expected_endpoint_counts={'raise-ieee':1});out.append(c)
 for error_type in (0,1,2,3,4,5,7,8,99):
  for callback_result in (0,1):
   c=base(0x56971f,f'{error_type}/{callback_result}',[0x16,RECORD,CONTROL])
   c.update(stores=[(RECORD,w(error_type)+w(0x604960)+q(0x3ff8000000000000)+q(0x4004000000000000)+q(0x3fe0000000000000)),(CONTROL,w(0x37f)),(0x605490,w(0))],
    watched=[(RECORD,32),(CONTROL,4)],write_ranges=[(RECORD,32)],compare_x87_values=True)
   callback=dict(token='matherr',native=0x560909,compiled_symbols=['FUN_00560909'],arity=1,result_kind='full32',value=callback_result,native_cleanup=0,compiled_cleanup=0)
   errno=dict(token='set-errno',native=0x568422,compiled_symbols=['FUN_00568422'],arity=1,result_kind='void',native_cleanup=0,compiled_cleanup=0)
   c.update(endpoints=[callback,errno],expected_endpoint_counts={'matherr':int(error_type!=8),'set-errno':int(error_type==8 or callback_result==0)});out.append(c)
 return [c for c in out if c['entry']in present]
