"""Audit exact wiring, actual build configuration and complete native replays."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
application=read('metadata/integration-batch-126-application.json')
for p,h in application['written'].items():assert sha(ROOT/p)==h,('applied source drift',p)
w=application['narrow_caller_wiring'];before=ROOT/'artifacts/integration-batch-126-before'/w['path']
assert sha(before)==w['expected_before_sha256'];data=before.read_bytes()
for r in w['replacements']:
 a,b=r['expected'].encode(),r['replacement'].encode();assert data.count(a)==r['expected_count'];data=data.replace(a,b)
assert data==(ROOT/w['path']).read_bytes() and sha(ROOT/w['path'])==w['expected_after_sha256']
registry=(ROOT/'artifacts/integration-batch-126-before/runtime/menu_callbacks.c').read_bytes()
for r in application['central_registry_insertions']:
 a,b=r['anchor'].encode(),r['inserted'].encode();assert registry.count(a)==1;registry=registry.replace(a,a+b,1)
assert registry==(ROOT/'runtime/menu_callbacks.c').read_bytes()
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
evidence={};tracked=set(application['written'])|{'metadata/integration-batch-126-application.json',w['path'],'runtime/menu_callbacks.c','CMakeLists.txt',str(Path(__file__).relative_to(ROOT)).replace('\\','/')}
editor_recipe=read('artifacts/editor_validation_phase/integration-recipe.json')
display_recipe=read('artifacts/display_confirmation_phase/metadata/integration-recipe.json')['verification_target']
for cfg,build in [('Debug','c-only-debug'),('Release','c-only')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for name in ['RESTORE_ORIGINAL_EXE','RESTORE_HYBRID_ORIGINAL_EXE']:
  values=re.findall(r'^'+name+r':[^=]*=(.*)$',cache,re.M)
  assert values==[''] if name=='RESTORE_ORIGINAL_EXE' else values in [[],['']],(name,values)
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-126-'+cfg.lower()+'-build.log';text=(ROOT/log).read_text(encoding='utf-8',errors='replace')
 assert '100% tests passed, 0 tests failed out of 127' in text and 'C source build completed.' in text,cfg
 tracked.add(log)
 for recipe in [editor_recipe,display_recipe]:
  target=recipe['name'];project=directory/(target+'.vcxproj');tree=ET.parse(project)
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
  cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
  assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
  assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
  assert not any(flag in get('PreprocessorDefinitions') for flag in ['EDITOR_VALIDATION_STAGED','EDITOR_VALIDATION_TRACE'])
  actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
  assert actual=={ROOT/p for p in recipe['sources']},target
  binary=directory/cfg/(target+'.exe');assert binary.is_file()
  sources={ROOT/p for p in (['runtime/editor_validation.c','src/unit_02079.c','include/editor_validation.h','src/unit_00071.c','src/unit_00621.c','src/unit_01739.c','src/unit_01745.c'] if target=='restore_editor_validation_verify' else recipe['sources'])}
  for p in sources|actual|{project}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,('binary predates source',p)
  evidence[cfg+'_'+target]={'path':binary.relative_to(ROOT).as_posix(),'sha256':sha(binary),'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]}
  tracked.update([project.relative_to(ROOT).as_posix(),binary.relative_to(ROOT).as_posix()])
 product=ET.parse(directory/'pharaoh_restored.vcxproj');compiled={Path(n.get('Include')).resolve() for n in product.findall('.//m:ClCompile[@Include]',ns)}
 assert {ROOT/p for p in ['runtime/editor_validation.c','runtime/display_confirmation.c','runtime/display_confirmation_clock.c','runtime/menu_callbacks.c']}<=compiled
 tracked.add('build/'+build+'/'+cfg+'/restore_link_check.exe')
editor=read('metadata/editor-validation-emulation.json');display=read('metadata/display-confirmation-emulation.json')
assert editor['passed'] and editor['checked']==6150 and editor['comparison_count']==24600
assert editor['case_selection']=={'limit':None,'start':0,'single_case':False}
assert editor['reachable_instruction_coverage']==[571,571] and editor['feasible_conditional_outcomes']==[138,138]
assert display['production'] and display['passed'] and display['complete_case_set'] and display['checked']==3978 and display['comparisons']==7956
q=display['qualified_controller'];assert q['complete'] and q['executed']==q['instructions']==126 and q['conditional_edges']==q['edges_executed']==10
assert q['excluded_instructions']==q['excluded_edges']==[]
for name in ['editor-validation','display-confirmation']:
 report=read('metadata/'+name+'-emulation.json')
 for p,h in {**report['source_hashes'],**report.get('native_exports',{})}.items():assert sha(ROOT/p)==h,p
 tracked.add('metadata/'+name+'-emulation.json');tracked.update(report['source_hashes']);tracked.update(report.get('native_exports',{}))
assert editor['compiled_C_sha256']==evidence['Debug_restore_editor_validation_verify']['sha256']
assert editor['comparison_C_sha256']==evidence['Release_restore_editor_validation_verify']['sha256']
for cfg,key in [('Debug','Debug'),('Release','O2')]:assert display['variants'][key]['sha256']==evidence[cfg+'_restore_display_confirmation_verify']['sha256']
out={'format':1,'passed':True,'complete_menu_reconstruction':False,'generated_at':datetime.now(timezone.utc).isoformat(),'bytewise_other_input_modes_preserved':True,'whole_1868_procedure_proven':False,'source_only_build':True,'configurations':evidence,'original_game_executed':False,'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-126-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'checked_configurations':len(evidence),'editor_cases':6150,'display_cases':3978}))
