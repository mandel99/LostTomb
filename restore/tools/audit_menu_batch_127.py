"""Verify actual replay target provenance, exact routing and full native evidence."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
application=read('metadata/integration-batch-127-application.json')
for p,h in application['written'].items():assert sha(ROOT/p)==h,('applied source drift',p)
before=ROOT/'artifacts/integration-batch-127-before/runtime/menu_callbacks.c'
assert sha(before)=='d7484ed726680ea5edfd4fdf175e91666e229ed8d6f007b92058f24488d4b529'
data=before.read_bytes()
for r in application['registry_insertions']:
 a,b=r['anchor'].encode(),r['inserted'].encode();assert data.count(a)==1;data=data.replace(a,a+b,1)
assert data==(ROOT/'runtime/menu_callbacks.c').read_bytes()
assert sha(ROOT/'src/unit_01868.c')=='55dcbd800976b31ffe58ade1ddd01a166bf18da4e63c67d252250b48a1972c29'
recipe=read('artifacts/replay_mission_phase/integration-recipe.json')
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
target=recipe['name'];configurations={}
tracked=set(application['written'])|{'metadata/integration-batch-127-application.json','src/unit_01868.c',Path(__file__).relative_to(ROOT).as_posix()}
for cfg,build in [('Debug','c-only-debug'),('Release','c-only')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 assert re.findall(r'^RESTORE_ORIGINAL_EXE:[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-127-'+cfg.lower()+'-build.log';text=(ROOT/log).read_text(encoding='utf-8',errors='replace')
 assert '100% tests passed, 0 tests failed out of 128' in text and 'C source build completed.' in text,cfg
 project=directory/(target+'.vcxproj');tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
 assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert not any(flag in get('PreprocessorDefinitions') for flag in ['REPLAY_MISSION_STAGED','REPLAY_MISSION_TRACE'])
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 assert actual=={ROOT/p for p in recipe['sources']},target
 binary=directory/cfg/(target+'.exe');assert binary.is_file()
 included={ROOT/p for p in ['runtime/replay_mission.c','include/replay_mission.h','src/unit_00222.c','src/unit_02282.c']}
 for p in actual|included|{project}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,('binary predates source',p)
 product=directory/'pharaoh_restored.vcxproj';product_tree=ET.parse(product)
 product_sources={Path(n.get('Include')).resolve() for n in product_tree.findall('.//m:ClCompile[@Include]',ns)}
 assert {ROOT/p for p in ['runtime/replay_mission.c','src/unit_00222.c','runtime/menu_callbacks.c']}<=product_sources
 configurations[cfg]={'path':binary.relative_to(ROOT).as_posix(),'sha256':sha(binary),'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]}
 tracked.update([log,project.relative_to(ROOT).as_posix(),product.relative_to(ROOT).as_posix(),binary.relative_to(ROOT).as_posix(),'build/'+build+'/'+cfg+'/restore_link_check.exe'])
proof=read('metadata/replay-mission-emulation.json')
assert proof['passed'] and proof['checked']==proof['plain_checked']==proof['comparison_checked']==10138 and proof['comparison_count']==40552
assert proof['case_selection']=={'limit':None,'start':0,'single_case':False}
assert proof['native_instruction_coverage']==[151,151] and proof['owned_instruction_coverage']==[113,113]
assert proof['native_conditional_outcomes']==[6,6] and proof['native_uncovered_instructions']==proof['missing_conditional_outcomes']==[]
assert proof['native_filename_written_offsets']==list(range(57)) and len(proof['mutated_helper_phases'])==20
assert proof['compiled_C_sha256']==configurations['Debug']['sha256'] and proof['comparison_C_sha256']==configurations['Release']['sha256']
for p,h in proof['source_hashes'].items():assert sha(ROOT/p)==h,p
bindings=read('artifacts/replay_mission_phase/native-contracts.json')
for entry in bindings['bindings'].values():assert sha(ROOT/entry['source'])==entry['source_sha256'],entry['source']
for p,h in bindings['input_hashes'].items():assert sha(ROOT/p)==h,p
tracked.update(proof['source_hashes']);tracked.update(bindings['input_hashes']);tracked.update(e['source'] for e in bindings['bindings'].values())
tracked.update(['metadata/replay-mission-emulation.json','artifacts/replay_mission_phase/native-contracts.json','artifacts/replay_mission_phase/manifest.json'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'source_only_build':True,'exact_registry_extension':True,'unchanged_popup_and_display_input':True,'original_game_executed':False,'configurations':configurations,'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-127-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'cases':10138,'comparisons':40552,'native_instructions':[151,151],'conditional_outcomes':[6,6]}))
