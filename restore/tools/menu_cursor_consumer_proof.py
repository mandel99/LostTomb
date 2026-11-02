"""Fail-closed retirement of the historical BYTE cursor-consumer gap fact."""
from pathlib import Path
import hashlib,json,xml.etree.ElementTree as ET
CANONICAL_SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
PROOF='metadata/menu-window-cursor-consumer-emulation.json'
VERIFIER='tools/verify_menu_window_cursor_consumer_emulation.py'
NATIVE='metadata/menu-window-cursor-consumer-native.json'
FROZEN='artifacts/menu_window_cursor_consumer_phase/metadata/freeze-manifest.json'
def _validate_consumer(proof,root):
 root=Path(root);sha=lambda p:hashlib.sha256((root/p).read_bytes()).hexdigest()
 assert proof['passed'] is True and proof['production'] is True and proof['complete_case_set'] is True
 assert proof['checked']==2635 and proof['comparisons']==5270 and proof['native_sha256']==CANONICAL_SHA
 assert proof['original_game_process_executed'] is False and proof['original_machine_code_fallback_used'] is False and proof['complete_actor_order_gameplay_claimed'] is False
 q=proof['qualified_controller'];assert q['complete'] and q['owned_entries']==['0040ba40','004b5520']
 assert q['instructions']==q['executed']==238 and q['conditional_edges']==q['edges_executed']==32 and q['dispatch_indices']==list(range(12))
 assert q['missing_instructions']==q['missing_edges']==q['excluded_instructions']==q['excluded_edges']==[]
 for key,instructions,edges in [('004b3250',17,4),('004cde60',12,2),('0051cd80',139,34)]:
  h=proof['actual_helper_coverage'][key];assert h['executed']==h['total']==instructions and h['conditional_edges']==h['edges_executed']==edges and h['missing']==h['missing_edges']==[]
 c=proof['actual_helper_coverage']['004cf460'];assert c['executed']==19 and c['total']==99 and c['edges_executed']==c['conditional_edges']==2
 assert proof['watched_memory']['minimum_bytes']==proof['watched_memory']['maximum_bytes']==2864 and proof['watched_memory']['all_native_writes_guarded']
 assert proof['outcomes']['controller_state_inputs']==list(range(256)) and proof['outcomes']['direct_direction_AL_returns']==list(range(8))
 assert proof['outcomes']['direct_lookup_full_DWORD_returns']==[0,1,5,32767,4294934528,4294967295]
 assert proof['outcomes']['deep_action_full_DWORD_returns']==[0,1,256,2147483648] and proof['outcomes']['mutation_masks']==[0,1,2,3,4,8,15,16,32,63]
 required={'src/unit_00085.c','src/unit_01455.c','include/menu_window_cursor_consumer.h','tests/menu_window_cursor_consumer.c','tests/menu_window_cursor_consumer_actual.c','tests/menu_window_cursor_consumer_case.h','src/unit_01427.c','src/unit_02334.c','src/unit_01731.c','src/unit_01769.c',VERIFIER}
 assert required<=proof['source_hashes'].keys()
 assert sha(VERIFIER)=='493ea88098f7a4414f2c88d3e31278222b1ef00be0267d37c220d9883621c9b0'
 assert proof['native_exports']=={NATIVE:'816f7be62d24ef880da10e058d5120d813dad2bb170c1dfcf26b330003fed24b'}
 assert sha(FROZEN)=='17f9085908c29f0f53e0a3e7e757799f8b932a928fe158c2740b8db037e7daed'
 frozen=json.loads((root/FROZEN).read_text())
 consumed={**proof['source_hashes'],**proof['native_exports'],**frozen['files']}
 for p,h in consumed.items():assert sha(p)==h,('consumer evidence drift',p)
 assert sha('src/unit_00085.c')=='98c74e11da800aa358707f6a430985e5ed34f56601779b88c50aec412a705b56'
 assert sha('src/unit_01455.c')=='1f489207a9987ccb31aa6865e402541cd26616a5a335820494242033b609b312'
 ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
 for label,cfg,build in [('Debug','Debug','c-only-debug'),('O2','Release','c-only')]:
  v=proof['variants'][label];p=f'build/{build}/{cfg}/restore_menu_window_cursor_consumer_verify.exe'
  assert v['path']==p and v['checked']==2635 and v['smoke_exit']==0 and sha(p)==v['sha256']
  if label=='Debug':assert proof['compiled_C_path']==p and proof['compiled_C_sha256']==v['sha256']
  project=root/'build'/build/'restore_menu_window_cursor_consumer_verify.vcxproj';tree=ET.parse(project)
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
  cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
  assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
  actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
  assert actual=={root/x for x in ['tests/menu_window_cursor_consumer.c','tests/menu_window_cursor_consumer_actual.c','src/unit_00085.c','src/unit_01769.c']}
  for key,unit in [('ROUTE',1427),('DIRECTION',2334),('SCALE',1731),('LOOKUP',1455)]:assert 'CONSUMER_'+key+'_SOURCE="'+(root/'src'/f'unit_{unit:05d}.c').as_posix()+'"' in get('PreprocessorDefinitions')
  for source in actual|{project}|{root/x for x in required if x.endswith(('.c','.h'))}:assert (root/p).stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale consumer binary',source)
  consumed[p]=v['sha256'];consumed[project.relative_to(root).as_posix()]=sha(project.relative_to(root))
 consumed[PROOF]=sha(PROOF);consumed[FROZEN]=sha(FROZEN);consumed['tools/menu_cursor_consumer_proof.py']=sha('tools/menu_cursor_consumer_proof.py')
 return {'evidence':PROOF,'evidence_sha256':sha(PROOF),'cases':2635,'comparisons':5270,'whole_owned_instructions':[238,238],'conditional_edges':[32,32],'historical_fact_retired':'Old85 BYTE cursor-handle comparisons','complete_actor_order_gameplay_claimed':False,'controlled_deep_functions':['0040c180','0040bda0','004b6400'],'verified_source_and_evidence_hashes':consumed}
def verified_consumer_proof(root):
 return _validate_consumer(json.loads((Path(root)/PROOF).read_text()),root)
