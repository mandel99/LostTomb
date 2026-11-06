"""Check cohort8 source/PE/native-width facts; does not run the game or prove behavior."""
from pathlib import Path
import hashlib,json,re,struct,sys
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM
def read(path):return json.loads(path.read_bytes())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
 source=read(ROOT/'metadata/cohort8-source100-local-value-repairs.json')
 facts=read(ROOT/'metadata/cohort8-native-local-view-facts.json')
 build=read(ROOT/'metadata/cohort8-source100-build-checkpoint.json')
 expected_views=source['explicit_local_view_repairs']
 assert source['changed_existing_C_functions']==100 and expected_views>=177
 assert len(source['members'])==100 and len(facts['facts'])==facts['local_views']==expected_views
 views={}
 for member in source['members']:
  path=ROOT/member['source'];assert sha(path)==member['source_sha256'],member['source']
  current={m[1]:m[2]for m in re.finditer(r'(?m)^#define[ \t]+([A-Za-z_]\w*)[ \t]+([^\r\n]+)',path.read_text())}
  for v in member['local_view_changes']:
   assert current[v['name']]==v['expression'],(member['address'],v['name'])
   views[(member['address'],v['name'])]=v
  assert member['providers']['retained']
 for image in build['compiled_images'].values():
  assert sha(ROOT/image['path'])==image['sha256'] and sha(ROOT/image['map_path'])==image['map_sha256']
  raw=(ROOT/image['path']).read_bytes();pe=struct.unpack_from('<I',raw,60)[0]
  assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 native_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(native_path)==facts['native_sha256']
 native=PE(native_path);instructions=0;address_bases=0;record_bases=0;address_cursors=0;byte_projections=0
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
 assert len({(f['function'],f['view'])for f in facts['facts']})==expected_views
 assert set(views)=={(f['function'],f['view'])for f in facts['facts']}
 for fact in facts['facts']:
  key=('0x%08x'%int(fact['function'],16),fact['view']);assert key in views
  assert fact['local_unit_bytes']in(1,2,4)
  role=fact.get('view_kind','operand_or_word_array')
  byte_address_base=role=='byte_address_base'
  record_address_base=role=='record_address_base'
  unused_address_cursor=role=='unused_address_cursor'
  low_byte_projection=role=='low_byte_projection'
  assert role in('operand_or_word_array','scalar_or_word_field','byte_record_array','word_record_base','word_flag_array',
                'byte_address_base','shifted_field_view','record_address_base','unused_address_cursor','low_byte_projection')
  widths=sorted({item['native_access_bytes']for item in fact['matching_native_observations']})
  if 'explicit_native_access_widths'in fact:assert widths==sorted(fact['explicit_native_access_widths'])
  if unused_address_cursor:
   address_cursors+=1
   assert not fact['matching_native_observations'] and fact.get('address_producer_instructions')
  else:assert fact['matching_native_observations']
  if byte_address_base:
   address_bases+=1
   assert fact['local_unit_bytes']==1 and fact['explicit_native_access_widths']==[4]
   current=(ROOT/fact['source']).read_text()
   assert fact['source_use_lines']and all(line in current and '&'+fact['view']in line for line in fact['source_use_lines'])
  if record_address_base:
   record_bases+=1
   assert fact.get('address_producer_instructions') and fact['source_use_lines']
   assert all('&'+fact['view']in line for line in fact['source_use_lines'])
  if low_byte_projection:
   byte_projections+=1
   assert fact['local_unit_bytes']==fact['array_stride_bytes']==4 and fact['projection_mask']==4
   assert widths==[1] and fact['source_use_lines']
   assert all(re.search(r'&\s*4u?\b',line)for line in fact['source_use_lines'])
  if 'source_use_lines'in fact:
   current=(ROOT/fact['source']).read_text()
   assert all(line in current for line in fact['source_use_lines'])
  for item in fact['matching_native_observations']:
   assert item['native_access_bytes']in(1,2,4)
   if not(record_address_base or low_byte_projection):assert item['native_access_bytes']==(4 if byte_address_base else fact['local_unit_bytes'])
   at=int(item['pc'],16)-native.base;raw=bytes.fromhex(item['bytes'])
   assert native.data[at:at+len(raw)]==raw,(fact['function'],item['pc']);instructions+=1
   decoded=list(decoder.disasm(raw,int(item['pc'],16)))
   assert len(decoded)==1 and decoded[0].size==len(raw)
   assert any(o.type==X86_OP_MEM and o.size==item['native_access_bytes']for o in decoded[0].operands)
   if low_byte_projection:assert decoded[0].mnemonic=='test'
  for item in fact.get('address_producer_instructions',[]):
   pc=item.get('address',item.get('pc'));assert pc is not None
   at=int(pc,16)-native.base;raw=bytes.fromhex(item['bytes'])
   assert native.data[at:at+len(raw)]==raw
 assert not source['whole_game_equivalence'] and source['joint_behavior_comparison_pending']
 print(json.dumps(dict(changed_C_sources=100,local_view_facts=expected_views,pinned_native_observations=instructions,
  byte_address_bases_with_explicit_DWORD_accesses=address_bases,
  record_address_bases_with_separate_operand_widths=record_bases,unused_address_cursors_without_value_claim=address_cursors,
  DWORD_arrays_with_low_BYTE_flag_projection=byte_projections,
  source_and_image_hashes_valid=True,i386_images=3,emulation_performed=False,
  behavior_comparison_complete=False,host_execution=False)))

if __name__=='__main__':main()
