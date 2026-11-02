"""Audit actual whole-panel source, build provenance and qualified native proof."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
app=read('metadata/integration-batch-129-application.json')
for p,h in {**app['written'],**app['protected_unchanged']}.items():assert sha(ROOT/p)==h,('source/application drift',p)
frozen=read('artifacts/training_panel_phase/manifest.json')
for p,h in {**frozen['frozen_file_hashes'],**frozen['external_tool_hash']}.items():assert sha(ROOT/p)==h,('frozen drift',p)
proof=read('metadata/training-panel-emulation.json')
assert proof['passed'] and proof['checked']==proof['plain_checked']==proof['comparison_checked']==3865 and proof['comparison_count']==15460
assert proof['case_selection']=={'limit':None,'start':0,'single_case':False}
assert proof['native_instruction_coverage']==[1001,1001] and proof['native_conditional_outcomes']==[90,90] and proof['feasible_computed_edges']==[15,15]
assert proof['native_uncovered_instructions']==proof['missing_conditional_outcomes']==[] and len(proof['mutated_helper_phases'])==26
assert set(proof['declared_infeasible_computed_edges'])=={'0x55b343->0x55b359'}
assert {'src/unit_02637.c','include/training_panel.h'}<=proof['source_hashes'].keys()
for p,h in proof['source_hashes'].items():assert sha(ROOT/p)==h,p
abi=read('artifacts/training_panel_phase/abi-contract.json');registers=read('artifacts/training_panel_phase/register-contract.json')
assert abi['native_instruction_count']==1001 and abi['native_bytes']==3530 and abi['complete_cfg'] and abi['entry_stack_delta']==abi['all_ret_deltas']==0
assert len(abi['stack_operands'])==15 and all(row['entry_sp_relative']<0 for row in abi['stack_operands'])
assert registers['complete_cfg_instructions']==1001 and registers['incoming_GPR_inputs']==[]
for p,h in registers['inputs'].items():assert sha(ROOT/p)==h,p
recipe=read('artifacts/training_panel_phase/integration-recipe.json');target=recipe['name'];ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};configurations={}
tracked=set(app['written'])|set(app['protected_unchanged'])|set(proof['source_hashes'])|set(frozen['frozen_file_hashes'])|{'metadata/integration-batch-129-application.json','metadata/training-panel-emulation.json','artifacts/training_panel_phase/manifest.json',Path(__file__).relative_to(ROOT).as_posix()}
for cfg,build in [('Debug','c-only-debug'),('Release','c-only')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for name in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+name+r':[^=]*=(.*)$',cache,re.M)==[''],name
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-129-'+cfg.lower()+'-build.log';text=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 131' in text and 'C source build completed.' in text
 project=directory/(target+'.vcxproj');tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert not any(n in get('PreprocessorDefinitions') for n in ['TRAINING_PANEL_STAGED','TRAINING_PANEL_TRACE'])
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/p for p in recipe['sources']}
 binary=directory/cfg/(target+'.exe')
 for p in actual|{project,ROOT/'src/unit_02637.c',ROOT/'include/training_panel.h'}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,p
 digest=sha(binary);assert digest==proof['compiled_C_sha256' if cfg=='Debug' else 'comparison_C_sha256']
 product=directory/'pharaoh_restored.vcxproj';tree=ET.parse(product);actual_product={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert ROOT/'src/unit_02637.c' in actual_product
 configurations[cfg]={'path':binary.relative_to(ROOT).as_posix(),'sha256':digest,'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]}
 tracked.update([log,project.relative_to(ROOT).as_posix(),product.relative_to(ROOT).as_posix(),binary.relative_to(ROOT).as_posix(),'build/'+build+'/'+cfg+'/restore_link_check.exe'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'source_only_build':True,'initial_and_refresh_callers_and_registry_unchanged':True,'ignored_context_and_no_GPR_inputs_proven':True,'original_game_executed':False,'configurations':configurations,'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-129-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'actual_target_configurations':len(configurations),'native_cases':3865,'comparisons':15460,'native_instructions':[1001,1001]}))
