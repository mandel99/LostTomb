"""Audit the actual source-only dispatcher/four-action target configurations."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
app=read('metadata/integration-batch-128-application.json')
for p,h in app['written'].items():assert sha(ROOT/p)==h,('application drift',p)
assert sha(ROOT/'runtime/menu_callbacks.c')==read('metadata/integration-batch-127-application.json')['written']['runtime/menu_callbacks.c']
assert sha(ROOT/'src/unit_01868.c')=='55dcbd800976b31ffe58ade1ddd01a166bf18da4e63c67d252250b48a1972c29'
assert sha(ROOT/'artifacts/initial_menu_dispatch_phase/freeze-manifest.json')=='36c33111e070618b8ad5f3d780b9c60aed39f62c5c0cd3fa034a905ae1a384f9'
for p,h in read('artifacts/initial_menu_dispatch_phase/freeze-manifest.json')['frozen_files'].items():assert sha(ROOT/p)==h,('private frozen drift',p)
initial=read('metadata/initial-menu-dispatch-emulation.json');existing=read('metadata/menu-existing-actions-emulation.json')
assert initial['passed'] and initial['production'] and initial['complete_case_set'] and initial['checked']==initial['complete_cases']==5795 and initial['comparisons']==23180
for key,value in {'native_instruction_coverage':[179,179],'conditional_outcomes':[8,8],'switch_mode_coverage':[88,88],'switch_target_coverage':[47,47],'native_call_site_coverage':[58,58],'deep_endpoint_coverage':[50,50],'mutation_phases':100,'source_drift':False}.items():assert initial[key]==value,key
assert existing['passed'] and existing['checked']==existing['comparison_checked']==4193 and existing['comparison_count']==8386
assert existing['case_selection']=={'limit':None,'start':0,'single_case':False} and existing['primary_instruction_coverage']==[235,235] and existing['reachable_instruction_coverage']==[423,423] and existing['feasible_conditional_outcomes']==[18,18] and len(existing['mutated_helper_phases'])==26
for report in [initial,existing]:
 for p,h in report['source_hashes'].items():assert sha(ROOT/p)==h,p
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
recipes={
 'initial_menu_dispatch':{'name':'restore_initial_menu_dispatch_verify','sources':['artifacts/initial_menu_dispatch_phase/tests/'+n for n in ['initial_dispatch.c','actual_trace.c','actual_plain.c']]},
 'menu_existing_actions':read('artifacts/menu_existing_actions_phase/integration-recipe.json')}
configurations={};tracked=set(app['written'])|{'metadata/integration-batch-128-application.json','runtime/menu_callbacks.c','src/unit_01868.c',Path(__file__).relative_to(ROOT).as_posix(),'metadata/initial-menu-dispatch-emulation.json','metadata/menu-existing-actions-emulation.json','artifacts/initial_menu_dispatch_phase/freeze-manifest.json','artifacts/menu_existing_actions_phase/manifest.json'}
for cfg,build in [('Debug','c-only-debug'),('Release','c-only')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for name in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+name+r':[^=]*=(.*)$',cache,re.M)==[''],name
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-128-'+cfg.lower()+'-build.log';text=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 130' in text and 'C source build completed.' in text;tracked.add(log)
 for scope,recipe in recipes.items():
  target=recipe['name'];project=directory/(target+'.vcxproj');tree=ET.parse(project)
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
  cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
  assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
  assert not any(n in get('PreprocessorDefinitions') for n in ['INITIAL_DISPATCH_STAGED','INITIAL_DISPATCH_TRACE'])
  actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/p for p in recipe['sources']}
  binary=directory/cfg/(target+'.exe');report=initial if scope=='initial_menu_dispatch' else existing
  for p in actual|{project}|{ROOT/p for p in report['source_hashes'] if p.startswith(('src/','runtime/','include/'))}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,p
  digest=sha(binary)
  if scope=='initial_menu_dispatch':
   for label in (['Debug_trace','Debug_plain'] if cfg=='Debug' else ['O2_trace','O2_plain']):assert initial['variants'][label]['sha256']==digest
  else:assert existing['compiled_C_sha256' if cfg=='Debug' else 'comparison_C_sha256']==digest
  configurations[cfg+'_'+scope]={'path':binary.relative_to(ROOT).as_posix(),'sha256':digest,'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]}
  tracked.update([project.relative_to(ROOT).as_posix(),binary.relative_to(ROOT).as_posix()])
 product=directory/'pharaoh_restored.vcxproj';tree=ET.parse(product);actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 assert {ROOT/p for p in ['src/unit_01866.c','runtime/initial_menu_dispatch_boundary.c']}<=actual
 tracked.update([product.relative_to(ROOT).as_posix(),'build/'+build+'/'+cfg+'/restore_link_check.exe'])
tracked.update(initial['source_hashes']);tracked.update(existing['source_hashes'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'source_only_build':True,'unchanged_input_dispatch_and_registry':True,'original_game_executed':False,'configurations':configurations,'limitations':initial['limitations']+existing['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-128-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'actual_target_configurations':len(configurations),'initial_dispatch_cases':5795,'existing_actions_cases':4193}))
