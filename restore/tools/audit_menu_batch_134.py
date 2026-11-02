"""Audit actual whole LOS integration, retained x87 precision and qualified edges."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,struct,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
app=read('metadata/integration-batch-134-application.json');assert sha(app['predecessor'])==app['predecessor_sha256']
for p,h in {**app['written'],**app['protected_unchanged'],**app['frozen_inputs']}.items():assert sha(p)==h,('application drift',p)
phase='artifacts/actor_order_los_phase/';mapping=read(phase+'metadata/apply-map.json');frozen=read(phase+'metadata/freeze-manifest.json');numeric=read('artifacts/menu_full_input_phase/numeric-freeze.json')
for p,h in {**frozen['files'],**numeric['files']}.items():assert sha(p)==h,('frozen drift',p)
for r in mapping['files']+mapping['shared_numeric_files']:
 assert sha(r['staged'])==sha(r['target'])==r['candidate_sha256']
 old='artifacts/integration-batch-134-before/'+r['target'];assert (sha(old) if (ROOT/old).exists() else None)==r['before_sha256']
proof=read('metadata/actor-order-los-emulation.json');assert proof['passed'] and proof['production'] and proof['complete_case_set'] and proof['checked']==7480 and proof['comparisons']==14960
assert proof['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d' and not proof['original_game_process_executed'] and not proof['original_machine_code_fallback_used'] and not proof['complete_actor_order_gameplay_claimed']
q=proof['qualified_controller'];qualification=read('metadata/actor-order-los-qualification.json')
assert q['complete'] and q['native_instructions']==q['executed']==538 and q['conditional_edges']==152 and q['feasible_edges']==q['edges_executed']==146 and q['missing_instructions']==q['missing_edges']==[]
assert q['excluded_edges']==qualification['excluded_edges'] and len(q['excluded_edges'])==6 and qualification['excluded_instructions']==[]
assert set(proof['actual_dependency_coverage'])=={'0040f8f0','0051ce90','0056133c'}
assert proof['actual_dependency_coverage']['0040f8f0']=={'executed_in_whole':22,'total':22,'executed_primitive_or_whole':22}
assert proof['actual_dependency_coverage']['0056133c']=={'executed_in_whole':15,'total':15,'executed_primitive_or_whole':15}
assert proof['actual_dependency_coverage']['0051ce90']=={'executed_in_whole':175,'total':187,'executed_primitive_or_whole':175}
assert set(proof['control_words'])=={0x7f|(pc<<8)|(rc<<10)|(ic<<12) for pc in [0,2,3] for rc in range(4) for ic in range(2)}
assert proof['numeric_dependency']['used_API']=='restore_menu_input_trunc80 only'
logline=(ROOT/'artifacts/actor-order-los-production.log').read_text().splitlines()[0];population=json.loads(logline)
assert population=={'selected_cases':7480,'full_case_count':7480,'first_whole_case':6960}
for p,h in {**proof['source_hashes'],**proof['native_exports']}.items():assert sha(p)==h,('actual source proof drift',p)
reference=read('metadata/actor-order-los-reference-review.json');assert reference['C_sharp_used_as_exact_behavior'] is False
for r in reference['references']:assert sha(r['path'])==r['sha256']
tracked=set(app['written'])|set(app['protected_unchanged'])|set(app['frozen_inputs'])|set(frozen['files'])|set(numeric['files'])|set(proof['source_hashes'])|set(proof['native_exports'])|{'metadata/integration-batch-134-application.json','metadata/actor-order-los-emulation.json','metadata/actor-order-los-reference-review.json','tools/record_actor_los_reference.py','artifacts/actor-order-los-production.log',Path(__file__).relative_to(ROOT).as_posix()}
tracked.update(r['path'] for r in reference['references'])
recipe=read(mapping['recipe'])['oracle_CMake'];ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};configurations={}
for cfg,build,label in [('Debug','c-only-debug','Debug'),('Release','c-only','O2')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-134-'+cfg.lower()+'-build.log';s=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 139' in s and 'C source build completed. No hybrid game was built or launched.' in s
 project=directory/'restore_actor_order_los_verify.vcxproj';tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);link=groups[0].find('m:Link',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns);lget=lambda n:link.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
 assert not any(x in get('PreprocessorDefinitions') for x in ['STAGED','TRACE']) and 'LOS_FILL_SOURCE="'+(ROOT/'src/unit_00119.c').as_posix()+'"' in get('PreprocessorDefinitions')
 assert lget('BaseAddress')=='0x10000' and lget('FixedBaseAddress')=='true' and lget('RandomizedBaseAddress')=='false' and '/machine:X86' in lget('AdditionalOptions')
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/p for p in recipe['sources']} and ROOT/'src/unit_00119.c' not in actual
 binary=directory/cfg/'restore_actor_order_los_verify.exe';variant=proof['variants'][label];assert binary.relative_to(ROOT).as_posix()==variant['path'] and sha(variant['path'])==variant['sha256'] and variant['checked']==7480 and variant['smoke_exit']==0
 for source in actual|{project,ROOT/'CMakeLists.txt'}|{ROOT/p for p in proof['source_hashes'] if p.endswith(('.c','.h'))}:assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns,source
 product=directory/'pharaoh_restored.vcxproj';t=ET.parse(product);product_sources=[Path(n.get('Include')).resolve() for n in t.findall('.//m:ClCompile[@Include]',ns)]
 required={ROOT/p for p in ['src/unit_00086.c','src/unit_00087.c','runtime/actor_order_los_numeric.c','runtime/menu_full_input_numeric.c']}
 assert all(product_sources.count(p)==1 for p in required) and not any('artifacts' in p.relative_to(ROOT).parts for p in product_sources)
 library=directory/cfg/'pharaoh_restored.lib';link_check=directory/cfg/'restore_link_check.exe'
 for source in required|{ROOT/'include/actor_order_los.h',ROOT/'include/menu_full_input_numeric.h',product}:assert library.stat().st_mtime_ns>=source.stat().st_mtime_ns
 assert link_check.stat().st_mtime_ns>=library.stat().st_mtime_ns
 raw=link_check.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 configurations[cfg]={'CTest_passed':139,'oracle_path':variant['path'],'oracle_sha256':variant['sha256'],'optimization':get('Optimization'),'runtime':get('RuntimeLibrary'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)],'product_sha256':sha(library.relative_to(ROOT)),'link_check_sha256':sha(link_check.relative_to(ROOT))}
 tracked.update([log,project.relative_to(ROOT).as_posix(),variant['path'],product.relative_to(ROOT).as_posix(),library.relative_to(ROOT).as_posix(),link_check.relative_to(ROOT).as_posix(),'build/'+build+'/CMakeCache.txt'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'complete_actor_order_gameplay_claimed':False,'source_only_build':True,'original_game_executed':False,'cases':7480,'comparisons':14960,'whole_function_cases':520,'whole_function_comparisons':1040,'primitive_cases':6960,'primitive_comparisons':13920,'owned_instruction_coverage':[538,538],'feasible_conditional_outcomes':[146,146],'qualified_excluded_edges':q['excluded_edges'],'actual_dependency_coverage':proof['actual_dependency_coverage'],'whole_context_dependency_qualification':'Whole LOS context executes175/187 flood instructions; separate accepted flood proof covers185 feasible/187 retained. Primitive-site hits cannot satisfy owned whole-function coverage.','masked_control_words':24,'controlled_game_or_OS_boundaries':[],'implicit_global2775_unchanged':True,'route_builder_application':False,'C_sharp_used_as_exact_behavior':False,'configurations':configurations,'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-134-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['passed','cases','comparisons','whole_function_cases','owned_instruction_coverage','complete_menu_reconstruction']}))
