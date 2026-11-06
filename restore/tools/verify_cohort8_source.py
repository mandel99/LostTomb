"""Check cohort8 source/PE/native-width facts; does not run the game or prove behavior."""
from pathlib import Path
import hashlib,json,re,struct,sys
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'direct_c'))
from native_test_pe import PE
def read(path):return json.loads(path.read_bytes())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
 source=read(ROOT/'metadata/cohort8-source100-local-value-repairs.json')
 facts=read(ROOT/'metadata/cohort8-native-local-view-facts.json')
 build=read(ROOT/'metadata/cohort8-source100-build-checkpoint.json')
 assert source['changed_existing_C_functions']==100 and source['explicit_local_view_repairs']==177
 assert len(source['members'])==100 and len(facts['facts'])==177
 views={}
 for member in source['members']:
  path=ROOT/member['source'];assert sha(path)==member['source_sha256'],member['source']
  current={m[1]:m[2]for m in re.finditer(r'(?m)^#define\s+((?:_?DAT)_[0-9a-fA-F]+)\s+([^\r\n]+)',path.read_text())}
  for v in member['local_view_changes']:
   assert current[v['name']]==v['expression'],(member['address'],v['name'])
   views[(member['address'],v['name'])]=v
  assert member['providers']['retained']
 for image in build['compiled_images'].values():
  assert sha(ROOT/image['path'])==image['sha256'] and sha(ROOT/image['map_path'])==image['map_sha256']
  raw=(ROOT/image['path']).read_bytes();pe=struct.unpack_from('<I',raw,60)[0]
  assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 native_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(native_path)==facts['native_sha256']
 native=PE(native_path);instructions=0
 for fact in facts['facts']:
  key=('0x%08x'%int(fact['function'],16),fact['view']);assert key in views
  assert fact['local_unit_bytes']in(1,4) and fact['matching_native_observations']
  for item in fact['matching_native_observations']:
   assert item['native_access_bytes']==fact['local_unit_bytes']
   at=int(item['pc'],16)-native.base;raw=bytes.fromhex(item['bytes'])
   assert native.data[at:at+len(raw)]==raw,(fact['function'],item['pc']);instructions+=1
 assert not source['whole_game_equivalence'] and source['joint_behavior_comparison_pending']
 print(json.dumps(dict(changed_C_sources=100,local_view_facts=177,pinned_native_observations=instructions,
  source_and_image_hashes_valid=True,i386_images=3,emulation_performed=False,
  behavior_comparison_complete=False,host_execution=False)))

if __name__=='__main__':main()
