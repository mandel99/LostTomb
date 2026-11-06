"""Original WNDPROC capture/default paths through the real compiled adapter."""
import struct
ENTRY=0x414cd0
DISPATCH=0x31000900
EVENT=bytes(range(16))
HWND,MESSAGE,KEY,VALUE,TIME,POSITION=0x12345678,0x481,0x89abcdef,0xfedcba98,0xdeadbeef,0xf123cdef
MSG=struct.pack('<7I',HWND,MESSAGE,KEY,VALUE,TIME,POSITION&0xffff,POSITION>>16)

def raw(n):return struct.pack('<I',n&0xffffffff)
def imported(token,name,arity,value):
 return dict(token=token,import_name=name,arity=arity,native_cleanup=arity*4,
  compiled_cleanup=arity*4,result_kind='full32',value=value)

def definitions(registry=None):
 out=[]
 for label,enabled,initial,changed in [('disabled',False,0xaabbccdd,None),
   ('converter-only',True,0,None),('controller-reloaded',True,0x11112222,0x55667788),
   ('controller-cleared',True,0x11112222,0),('controller-created',True,0,0xaabbccdd)]:
  final=initial if changed is None else changed
  second=enabled and final!=0
  first=dict(pointer_args={1:'qt-event'},bind_pointer_roles=['qt-event'],
   private_input_args={0:dict(role='native-message',width=28)},
   writes_arg_bytes=[[1,0,EVENT.hex()]],value=0xa5b61234)
  if changed is not None:first['mutations']=[(0x6fdc3c,4,changed)]
  last=dict(private_input_args={1:dict(role='component-parameters',segments=[[0,4]],
   indirect_inputs=[dict(offset=4,width=16,role='qt-event',bound_role='qt-event')])},value=0x89abcdef)
  response=([first]+([last]if second else[]))if enabled else[]
  qt=dict(token='qt-dispatch',controlled_address=DISPATCH,arity=2,
   native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=0,
   responses=response,registers=['eax'])
  counts={'time':int(enabled),'position':int(enabled),'qt-dispatch':len(response),'default-window':1}
  args={'time':[[]]if enabled else[],'position':[[]]if enabled else[],
   'default-window':[[HWND,MESSAGE,KEY,VALUE]],'qt-dispatch':[]}
  if enabled:
   args['qt-dispatch'].append([['stack-input','native-message',MSG.hex()],['stack-output','qt-event']])
   if second:args['qt-dispatch'].append([final,['stack-input','component-parameters',dict(segments=['00040700'],indirect_inputs=[['qt-event',EVENT.hex()]])]])
  c=dict(id='c7-qt-window/'+label,entry=ENTRY,receiver=0x12345678,
   arguments=[HWND,MESSAGE,KEY,VALUE],named_arguments=[HWND,MESSAGE,KEY,VALUE],
   eflags=0xad7,stores=[(0x6fdc3c,raw(initial)),(0x6fdc40,raw(0x100 if enabled else 0)),
    (0x6fdc54,raw(0)),(0xe38dda,b'T'),(0x604804,raw(0)),(0x604808,raw(DISPATCH))],
   watched=[(0x6fdc3c,8),(0x6fdc54,4),(0xe38dda,1)],
   write_ranges=[(0x6fdc3c,4),(0x604804,16)],
   function_pointers=[dict(address=a,native=0x560350,compiled_symbol='restore_quicktime_missing_dispatcher')for a in [0x60480c,0x604810]],
   endpoints=[imported('time','GetMessageTime',0,TIME),imported('position','GetMessagePos',0,POSITION),qt,
    imported('default-window','DefWindowProcA',4,0xfacee0ee)],
   expected_endpoint_counts=counts,expected_endpoint_arguments=args,
   authored_final_memory=[(0x6fdc3c,raw(final).hex())],result_mask=0xffffffff,
   expected_eax=0xfacee0ee,native_cleanup=16,named_cleanup=16,canonical_cleanup=0,
   canonical_registers_and_flags=False,compare_x87_values=True,compare_x87_control=True,
   initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'],
   expected_selector_sequence=[0x1d000a]+([0x10000]if second else[])if enabled else[])
  out.append(c)
 return out
