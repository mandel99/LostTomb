"""Pure case recipes. Additional cohort families remain explicit pending work."""
import struct
from formatter_cases import definitions as formatter_definitions
from draw_cases import definitions as draw_definitions
from fp_control_cases import definitions as fp_control_definitions
from stream_cases import definitions as stream_definitions
from crt_lifecycle_cases import definitions as crt_lifecycle_definitions
from exception_cases import definitions as exception_definitions
from gameplay_cases import definitions as gameplay_definitions
from cohort4_gameplay_cases import definitions as cohort4_gameplay_definitions
from cohort4_stream_io_cases import definitions as cohort4_stream_io_definitions
from cohort4_crt_math_cases import definitions as cohort4_crt_math_definitions
from cohort4_crt_runtime_cases import definitions as cohort4_crt_runtime_definitions
from cohort5_bridge_cases import definitions as cohort5_bridge_definitions
from cohort5_destructor_cases import definitions as cohort5_destructor_definitions
from cohort5_nls_cases import definitions as cohort5_nls_definitions
from cohort5_raw_member_cases import definitions as cohort5_raw_definitions
from cohort5_window_tag_cases import definitions as cohort5_window_tag_definitions
from cohort5_event_list_cases import definitions as cohort5_event_list_definitions
from cohort6_advisor_abi_cases import definitions as cohort6_advisor_abi_definitions
from cohort6_overlay_abi_cases import definitions as cohort6_overlay_abi_definitions
from cohort6_laborer_cases import definitions as cohort6_laborer_definitions
from cohort7_translation_cases import definitions as cohort7_translation_definitions
from cohort7_eh3_cases import definitions as cohort7_eh3_definitions
SCRATCH=0x32000000

from cohort7_startup_cases import definitions as cohort7_startup_definitions
from cohort7_multibyte_cases import definitions as cohort7_multibyte_definitions
from cohort7_consumer_cases import definitions as cohort7_consumer_definitions
from cohort7_environment_cases import definitions as cohort7_environment_definitions
from cohort7_calendar_cases import definitions as cohort7_calendar_definitions
from cohort7_scalar_lowio_cases import definitions as cohort7_scalar_lowio_definitions

def definitions(registry):
 out=[]
 for m in registry['members']:
  a=int(m['address'],16);rule=m.get('rule')or{}
  if rule.get('kind')=='lookup':
   self=SCRATCH+0x1000
   for index,value in [(0,0),(1,0xa5),(127,128),(255,255)]:
    for flags in [0x202,0xad7]:
     width=rule.get('table_width',1);table=rule['table_base']+index*width
     out.append(dict(id=f'lookup/{a:08x}/{index}/{flags:x}',entry=a,
      receiver=self,arguments=[],named_arguments=[self],eflags=flags,
      stores=[(self,b'\xa5'*64),(self+rule['offset'],bytes([index])),(table,value.to_bytes(width,'little'))],
      watched=[(self,64),(rule['table_base']&~4095,4096)],write_ranges=[],
      expected_eax=value,result_mask=0xffffffff,native_cleanup=0,named_cleanup=0,
      canonical_cleanup=0,canonical_registers_and_flags=True))
  elif a in [0x56896e,0x5689c4]:
   words=SCRATCH+0x1000
   for label,values in [('zero',[0,0,0]),('carry',[0xffffffff]*3),('mixed',[0x13579bdf,0x2468ace0,0x80000001])]:
    for bit in [0,31,32,63,64,95]:
     out.append(dict(id=f'bit96/{a:08x}/{label}/{bit}',entry=a,receiver=0x22222222,
      arguments=[words,bit],named_arguments=[words,bit],eflags=0x202,
      stores=[(words,b'\xa5'*64),(words,struct.pack('<III',*values))],
      watched=[(words,64)],write_ranges=[(words,12)],result_mask=0xffffffff,
      native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False))
 out.extend(formatter_definitions(registry))
 out.extend(draw_definitions(registry))
 out.extend(fp_control_definitions(registry))
 out.extend(stream_definitions(registry))
 out.extend(crt_lifecycle_definitions(registry))
 out.extend(exception_definitions(registry))
 out.extend(gameplay_definitions(registry))
 out.extend(cohort4_gameplay_definitions(registry))
 out.extend(cohort4_stream_io_definitions(registry))
 out.extend(cohort4_crt_math_definitions(registry))
 out.extend(cohort4_crt_runtime_definitions(registry))
 out.extend(cohort5_bridge_definitions(registry))
 out.extend(cohort5_destructor_definitions(registry))
 out.extend(cohort5_nls_definitions(registry))
 out.extend(cohort5_raw_definitions(registry))
 out.extend(cohort5_window_tag_definitions(registry))
 out.extend(cohort5_event_list_definitions(registry))
 out.extend(cohort6_advisor_abi_definitions(registry))
 out.extend(cohort6_overlay_abi_definitions(registry))
 out.extend(cohort6_laborer_definitions(registry))
 out.extend(cohort7_translation_definitions(registry))
 out.extend(cohort7_eh3_definitions(registry))
 out.extend(cohort7_startup_definitions(registry))
 out.extend(cohort7_multibyte_definitions(registry))
 out.extend(cohort7_consumer_definitions(registry))
 out.extend(cohort7_environment_definitions(registry))
 out.extend(cohort7_calendar_definitions(registry))
 out.extend(cohort7_scalar_lowio_definitions(registry))
 constructors={0x5605fa,0x560648,0x560671,0x5606a0,0x560f02,0x5609da,0x560d22,0x5611c8,0x56123e,0x561278}
 for c in out:
  if c['id'].startswith('formatter/'):
   c['canonical_owner']='crt_format_entries.obj'
  if c['id'].startswith('stream/'):
   c['canonical_owner']='crt_stream_constructor_entries.obj'if c['entry']in constructors else'crt_stream_destructor_entries.obj'
 return out
