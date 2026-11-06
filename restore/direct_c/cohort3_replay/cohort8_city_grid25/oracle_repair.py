"""Three native-derived expectation corrections over immutable city25 cases."""
import copy
RESULT_CASE='c8-city-grid25/00446a30/signed-fallback-entrance-and-counter-wrap'
MEMORY_CASES={'c8-city-grid25/004e7200/low-clamp-high-building-ID','c8-city-grid25/004e7200/high-clamp-actor-state-migration'}
ACTOR=0xbcd600+257*388
def correct_cases(cases):
 result=copy.deepcopy(cases)
 for c in result:
  if c['id']==RESULT_CASE:
   assert c['expected_masked_result']==0x1234+3*228-2;c['expected_masked_result']=0x1234
  if c['id']in MEMORY_CASES:
   blocks=c['authored_final_memory'];index=next(i for i,(a,data)in enumerate(blocks)if a==ACTOR)
   a,data=blocks[index];raw=bytearray.fromhex(data);assert raw[0x2c:0x30]==bytes.fromhex('0000a5a5')
   raw[0x2c:0x30]=bytes.fromhex('a5a50000');blocks[index]=(a,raw.hex())
 return result
