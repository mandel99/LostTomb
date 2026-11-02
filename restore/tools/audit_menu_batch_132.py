"""Audit full actor-order integration and explicit retirement of an old cursor fact."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,struct,xml.etree.ElementTree as ET
from menu_cursor_consumer_proof import verified_consumer_proof
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
app=read('metadata/integration-batch-132-application.json')
assert sha(app['predecessor'])==app['predecessor_sha256']
for p,h in {**app['written'],**app['protected_unchanged'],**app['frozen_inputs']}.items():assert sha(p)==h,('application drift',p)
phase='artifacts/menu_window_cursor_consumer_phase/'
mapping=read(phase+'metadata/apply-map.json');frozen=read(phase+'metadata/freeze-manifest.json')
for p,h in frozen['files'].items():assert sha(p)==h,('frozen drift',p)
for r in mapping['files']:
 assert sha(r['staged'])==sha(r['target'])==r['candidate_sha256']
 saved='artifacts/integration-batch-132-before/'+r['target'];assert (sha(saved) if (ROOT/saved).exists() else None)==r['before_sha256']
summary=verified_consumer_proof(ROOT);proof=read(summary['evidence'])
selector=read('metadata/menu-cursor-emulation.json');old=read('artifacts/integration-batch-132-before/metadata/menu-cursor-emulation.json')
assert selector['passed'] and selector['production'] and selector['complete_case_set'] and selector['checked']==4360 and selector['comparisons']==8720
assert selector['qualified_controller']==old['qualified_controller'] and selector['native_ABI']==old['native_ABI'] and selector['watched_memory']==old['watched_memory']
assert selector['consumer_gap_supersession']=={k:v for k,v in summary.items() if k!='verified_source_and_evidence_hashes'}
assert old['source_hashes']['src/unit_00085.c']=='9c78635615701cf1328a6fff55580e158404aeb1219fca5d2a6f45a0fc16059a'
for p in ['src/unit_01769.c','include/menu_cursor.h','runtime/menu_cursor_platform.c','tests/menu_cursor.c','tests/menu_cursor_case.h']:
 assert selector['source_hashes'][p]==old['source_hashes'][p]==sha(p)
assert 'still uses BYTE' in old['limitations'][-1] and 'superseded' in selector['limitations'][-1] and selector['limitations'][:-1]==old['limitations'][:-1]
for p,h in {**selector['source_hashes'],**selector['native_exports']}.items():assert sha(p)==h,('fresh selector proof drift',p)
assert sha('src/unit_01768.c')==app['protected_unchanged']['src/unit_01768.c']
assert 'if(!restore_input_cursor(&point)) return;' in (ROOT/'src/unit_01768.c').read_text()
negative=read('metadata/menu-cursor-consumer-proof-negative.json');assert negative['passed'] and negative['checked']==11 and negative['product_files_modified'] is False
reference=read('metadata/menu-window-cursor-consumer-reference-review.json');assert reference['C_sharp_used_as_exact_behavior'] is False
for r in reference['references']+reference['historical_gui_snapshots_preserved']:assert sha(r['path'])==r['sha256']
tracked=set(app['written'])|set(app['protected_unchanged'])|set(app['frozen_inputs'])|set(frozen['files'])|set(summary['verified_source_and_evidence_hashes'])|set(selector['source_hashes'])|set(selector['native_exports'])|{'metadata/integration-batch-132-application.json','metadata/menu-cursor-emulation.json','metadata/menu-cursor-consumer-proof-negative.json','metadata/menu-window-cursor-consumer-reference-review.json','tools/test_menu_cursor_consumer_proof_guard.py','tools/record_menu_consumer_reference.py',Path(__file__).relative_to(ROOT).as_posix()}
tracked.update(r['path'] for r in reference['references']+reference['historical_gui_snapshots_preserved'])
tracked.add('artifacts/integration-batch-132-before/metadata/menu-cursor-emulation.json')
recipe=read(mapping['recipe'])['oracle_CMake'];target=recipe['target'];configurations={};ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
for cfg,build,label in [('Debug','c-only-debug','Debug'),('Release','c-only','O2')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-132-'+cfg.lower()+'-build.log';s=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 136' in s and 'C source build completed. No hybrid game was built or launched.' in s
 project=directory/(target+'.vcxproj');tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);link=groups[0].find('m:Link',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns);lget=lambda n:link.findtext('m:'+n,default='',namespaces=ns)
 assert lget('BaseAddress')=='0x10000' and lget('FixedBaseAddress')=='true' and lget('RandomizedBaseAddress')=='false' and '/machine:X86' in lget('AdditionalOptions')
 assert cfg+'\\restore_data.lib' in lget('AdditionalDependencies')
 assert not any(x in get('PreprocessorDefinitions') for x in ['STAGED','TRACE'])
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/p for p in recipe['sources']}
 assert (ROOT/'src/unit_01455.c') not in actual,'lookup must be compiled by exact traced include, not duplicate TU'
 product=directory/'pharaoh_restored.vcxproj';t=ET.parse(product);actual_product={Path(n.get('Include')).resolve() for n in t.findall('.//m:ClCompile[@Include]',ns)}
 assert {ROOT/'src/unit_00085.c',ROOT/'src/unit_01455.c'}<=actual_product and not any('artifacts' in p.relative_to(ROOT).parts for p in actual_product)
 library=directory/cfg/'pharaoh_restored.lib';link_check=directory/cfg/'restore_link_check.exe'
 for p in [ROOT/'src/unit_00085.c',ROOT/'src/unit_01455.c',ROOT/'include/menu_window_cursor_consumer.h',product]:assert library.stat().st_mtime_ns>=p.stat().st_mtime_ns
 assert link_check.stat().st_mtime_ns>=library.stat().st_mtime_ns
 raw=link_check.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 v=proof['variants'][label];assert sha(v['path'])==v['sha256']
 sv=selector['variants'][label];assert sha(sv['path'])==sv['sha256'];tracked.add(sv['path'])
 configurations[cfg]={'CTest_passed':136,'oracle_path':v['path'],'oracle_sha256':v['sha256'],'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)],'optimization':get('Optimization'),'runtime':get('RuntimeLibrary'),'product_sha256':sha(library.relative_to(ROOT)),'link_check_sha256':sha(link_check.relative_to(ROOT))}
 tracked.update([log,project.relative_to(ROOT).as_posix(),product.relative_to(ROOT).as_posix(),library.relative_to(ROOT).as_posix(),link_check.relative_to(ROOT).as_posix(),'build/'+build+'/CMakeCache.txt'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'complete_actor_order_gameplay_claimed':False,'source_only_build':True,'original_game_executed':False,'owned_native_instructions':[238,238],'owned_conditional_edges':[32,32],'dispatch_indices':list(range(12)),'actual_other_helpers_instructions':168,'cursor_composed_context_instructions':[19,99],'separate_complete_selector_instructions':[99,99],'cases':2635,'comparisons':5270,'watched_bytes':2864,'negative_proof_checks':11,'cursor_gap_fact_explicitly_superseded':True,'known_GetCursorPos_FALSE_gap_closed':False,'C_sharp_used_as_exact_behavior':False,'configurations':configurations,'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-132-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['passed','cases','comparisons','owned_native_instructions','cursor_gap_fact_explicitly_superseded','complete_menu_reconstruction']}))
