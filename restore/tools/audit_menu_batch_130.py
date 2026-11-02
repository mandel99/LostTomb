"""Audit batch130 application, generated Win32 targets and qualified production proofs."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,json,re,struct,xml.etree.ElementTree as ET
SCRIPT=Path(__file__).resolve()
ROOT=next(p for p in SCRIPT.parents if (p/'metadata/integration-batch-130-application.json').is_file())
PRIVATE=SCRIPT.parent!=ROOT/'tools'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,default=(SCRIPT.parents[1]/'menu-130-wiring-audit.json' if PRIVATE else ROOT/'metadata/menu-130-wiring-audit.json'))
args=parser.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
rel=lambda p:p.relative_to(ROOT).as_posix()
tracked=set()
def track_hashes(values,label):
 for p,h in values.items():assert sha(ROOT/p)==h,(label,p)
 tracked.update(values)
app=read('metadata/integration-batch-130-application.json')
assert app['complete_menu_reconstruction'] is False
assert sha(ROOT/app['predecessor'])==app['predecessor_sha256']
track_hashes(app['written'],'application written drift');track_hashes(app['frozen_inputs'],'frozen input drift')
tracked.update(['metadata/integration-batch-130-application.json',app['predecessor'],rel(SCRIPT)])
before=ROOT/'artifacts/integration-batch-130-before'
records=[]
for phase in ['menu_keyboard_modal_phase','menu_cursor_phase']:
 f=read('artifacts/'+phase+'/metadata/freeze-manifest.json');assert f['frozen'];track_hashes(f['files'],'phase freeze drift')
 assert sha(ROOT/f['apply_map'])==f['apply_map_sha256']
 for r in read(f['apply_map'])['files']:records.append((r['staged'],r['target'],r['before_sha256'],r['candidate_sha256']))
f=read('artifacts/training_queries_phase/manifest.json');track_hashes({**f['frozen_file_hashes'],**f['external_tool_hash']},'query freeze drift')
assert sha(ROOT/f['apply_map'])==f['apply_map_sha256']
qmap=read(f['apply_map']);u=qmap['unchanged_dependency'];assert sha(ROOT/u['path'])==u['sha256'];tracked.add(u['path'])
records.extend((r['source'],r['destination'],r['before_sha256'],r['candidate_sha256']) for r in qmap['records'])
for source,destination,old,new in records:
 assert sha(ROOT/source)==sha(ROOT/destination)==new and app['written'][destination]==new,destination
 saved=before/destination
 assert (sha(saved) if saved.exists() else None)==old,('before archive',destination)
 tracked.update([source,destination]);
 if saved.exists():tracked.add(rel(saved))
assert len(records)==len({r[1] for r in records})
wire=app['dispatcher_wiring'];assert wire['path']=='src/unit_01868.c' and len(wire['replacements'])==2
data=(before/wire['path']).read_bytes();assert hashlib.sha256(data).hexdigest()==wire['before_sha256']
for r in wire['replacements']:
 old,new=r['old'].encode('ascii'),r['new'].encode('ascii');assert data.count(old)==r['expected_count']==1;data=data.replace(old,new,1)
assert data==(ROOT/wire['path']).read_bytes() and hashlib.sha256(data).hexdigest()==wire['after_sha256']
registry='runtime/menu_callbacks.c';data=(before/registry).read_bytes();assert len(app['registry_insertions'])==2
modal_recipe=read('artifacts/menu_keyboard_modal_phase/metadata/integration-recipe.json')
assert hashlib.sha256(data).hexdigest()==modal_recipe['callback_registry']['observed_before_sha256']
for r in app['registry_insertions']:
 a,b=r['anchor'].encode('ascii'),r['inserted'].encode('ascii');assert data.count(a)==1 and data.count(b)==0;data=data.replace(a,a+b,1)
assert data==(ROOT/registry).read_bytes()
track_hashes({app['refresh_product_unchanged']['path']:app['refresh_product_unchanged']['sha256']},'refresh source drift')
tracked.update([rel(before/wire['path']),rel(before/registry),'artifacts/menu_keyboard_modal_phase/metadata/integration-recipe.json','artifacts/menu_cursor_phase/metadata/integration-recipe.json'])
modal=read('metadata/menu-keyboard-modal-emulation.json');cursor=read('metadata/menu-cursor-emulation.json');queries=read('metadata/training-queries-emulation.json')
for name,p,count in [('menu-keyboard-modal',modal,1806),('menu-cursor',cursor,4360),('training-queries',queries,9309)]:
 assert p['passed'] and p['checked']==count and p['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
 track_hashes({**p['source_hashes'],**p.get('native_exports',{})},'production proof source drift');tracked.add('metadata/'+name+'-emulation.json')
 if name!='training-queries':assert p['production'] and p['complete_case_set'] and not p['original_game_process_executed'] and not p['original_machine_code_fallback_used'] and p['comparisons']==2*count
assert modal['qualified_controller']=={'complete':True,'entries':['00538940','00538c80','00538cd0','00539170','00539220','00538920','004dd7b8'],'instructions':415,'executed':415,'missing_instructions':[],'conditional_edges':32,'feasible_edges':31,'feasible_executed':31,'missing_edges':[],'dispatch_indices':[0,1,2,3,4],'excluded_edges':[['00538b8d','00538c46']]}
qc=cursor['qualified_controller'];assert qc['complete'] and qc['instructions']==qc['executed']==99 and qc['dispatch_indices']==list(range(19)) and qc['conditional_edges']==qc['edges_executed']==2 and qc['missing_instructions']==qc['excluded_instructions']==qc['excluded_edges']==[]
assert cursor['watched_memory']['all_native_writes_guarded'] and cursor['watched_memory']['all_bytes_compared'] and cursor['native_ABI']['full_EAX_return_compared_in_named_helper']
assert queries['comparison_checked']==9309 and queries['comparison_count']==18618 and queries['case_selection']=={'limit':None,'start':0,'single_case':False}
assert queries['reachable_instruction_coverage']==[506,506] and queries['owned_instruction_coverage']==[462,462] and queries['native_instruction_coverage']==[506,517]
assert queries['feasible_conditional_outcomes']==[193,193] and queries['ordinary_conditional_outcomes']==[191,193]
assert queries['conditional_outcomes_seen_only_with_declared_mutation']==[['0x443bf3','0x443bf5'],['0x443d10','0x443d4e']]
assert len(queries['declared_dead_instructions'])==11 and len(queries['declared_dead_edges'])==7 and queries['controlled_endpoints']==[] and len(queries['mutated_helper_phases'])==18
assert not queries['original_game_executed']
for p in [modal,cursor]:assert any('GetCursorPos' in x and 'FALSE' in x for x in p['limitations'])
gap_source='src/unit_01768.c';assert 'if(!restore_input_cursor(&point)) return;' in (ROOT/gap_source).read_text()
gap={'path':gap_source,'sha256':sha(ROOT/gap_source),'closed':False,'native':'4CF210 ignores GetCursorPos BOOL and may consume unspecified POINT on FALSE.','C':'Existing1768 returns early on FALSE. New modal/selector proofs qualify successful POINT domain; they do not prove FALSE equivalence.'};tracked.add(gap_source)
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
recipes=[modal_recipe['oracle_CMake'],read('artifacts/menu_cursor_phase/metadata/integration-recipe.json')['oracle_CMake'],f['shared_target']]
proofs=[modal,cursor,queries];configurations={}
for cfg,build,variant in [('Debug','c-only-debug','Debug'),('Release','c-only','O2')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for name in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+name+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-130-'+cfg.lower()+'-build.log';text=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 134' in text and 'C source build completed. No hybrid game was built or launched.' in text
 tracked.update([log,rel(directory/'CMakeCache.txt'),rel(directory/'CTestTestfile.cmake')]);rows=[]
 ctest=(directory/'CTestTestfile.cmake').read_text();product=directory/'pharaoh_restored.vcxproj';tree=ET.parse(product);product_sources={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 required={ROOT/p for p in app['written'] if p.endswith('.c') and p.startswith(('src/','runtime/'))}
 assert required<=product_sources and all(ROOT in p.parents for p in product_sources)
 assert not any('artifacts' in p.relative_to(ROOT).parts for p in product_sources)
 assert tree.findtext("m:PropertyGroup[@Condition=\"'$(Configuration)|$(Platform)'=='"+cfg+"|Win32'\"]/m:ConfigurationType",default='',namespaces=ns)=='StaticLibrary'
 product_binary=directory/cfg/'pharaoh_restored.lib';link_binary=directory/cfg/'restore_link_check.exe'
 for p in required|{product,ROOT/'src/unit_01867.c',ROOT/'src/unit_01868.c'}:assert product_binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,('product freshness',cfg,p)
 assert product_binary.read_bytes().startswith(b'!<arch>\n')
 assert link_binary.stat().st_mtime_ns>=product_binary.stat().st_mtime_ns
 raw=link_binary.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert raw[pe:pe+4]==b'PE\x00\x00' and struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 tracked.update([rel(product_binary),rel(link_binary)])
 tracked.add(rel(product))
 for recipe,p in zip(recipes,proofs):
  target=recipe.get('target',recipe.get('name'));project=directory/(target+'.vcxproj');tree=ET.parse(project)
  assert all('Win32'==x.get('Include').split('|')[-1] for x in tree.findall('.//m:ProjectConfiguration',ns))
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1;g=groups[0];cc=g.find('m:ClCompile',ns);link=g.find('m:Link',ns)
  get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns);lget=lambda n:link.findtext('m:'+n,default='',namespaces=ns)
  assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
  definitions=get('PreprocessorDefinitions');assert '_CRT_SECURE_NO_WARNINGS' in definitions and not re.search(r'\b[A-Z_]*(?:STAGED|TRACE)\b',definitions)
  if target=='restore_menu_keyboard_modal_verify':assert 'MODAL_KEYBOARD_CONTEXT_SOURCE="'+(ROOT/'src/unit_00274.c').as_posix()+'"' in definitions
  assert get('WarningLevel')=='Level4'
  if target!='restore_training_queries_verify':assert get('TreatWarningAsError')=='true'
  assert lget('BaseAddress')=='0x10000' and lget('FixedBaseAddress')=='true' and lget('RandomizedBaseAddress')=='false' and '/machine:X86' in lget('AdditionalOptions')
  for lib in recipe['link_libraries']:assert (cfg+'\\restore_data.lib' if lib=='restore_data' else lib+'.lib') in lget('AdditionalDependencies')
  actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/x for x in recipe['sources']},target
  includes={Path(x).resolve() for x in get('AdditionalIncludeDirectories').split(';') if not x.startswith('%(')};assert includes=={ROOT/x for x in recipe['include_directories']}
  assert target in ctest and target.replace('restore_','standalone_').replace('_verify','') in ctest
  binary=directory/cfg/(target+'.exe');digest=sha(binary)
  expected=(p['variants'][variant]['sha256'] if 'variants' in p else p['compiled_C_sha256' if cfg=='Debug' else 'comparison_C_sha256']);assert digest==expected,(target,cfg)
  if 'variants' in p:assert p['variants'][variant]['path']==rel(binary) and p['variants'][variant]['checked']==p['checked']
  compiled_inputs=actual|{project,ROOT/'CMakeLists.txt'}|{ROOT/x for x in p['source_hashes'] if x.endswith(('.c','.h'))}
  for source in compiled_inputs:assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns,('oracle freshness',target,cfg,source)
  raw=binary.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
  rows.append({'target':target,'path':rel(binary),'sha256':digest,'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[rel(x) for x in sorted(actual)],'Win32':True,'fixed_base':'10000','recipe_sources_exact':True,'compiled_inputs_fresh':True})
  tracked.update([rel(project),rel(binary)])
 configurations[cfg]={'CTest_passed':134,'oracles':rows,'product_path':rel(product_binary),'product_sha256':sha(product_binary),'link_check_path':rel(link_binary),'link_check_sha256':sha(link_binary)}
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'source_only_build':True,'original_game_executed':False,'application_records_verified':len(records),'application_written_verified':len(app['written']),'dispatcher_bytewise_replacements':2,'registry_bytewise_insertions':2,'refresh_product_unchanged':app['refresh_product_unchanged'],'configurations':configurations,'qualified_new_proofs':{'modal':{'cases':1806,'instructions':[415,415],'feasible_edges':[31,31],'excluded_edge':['00538b8d','00538c46']},'cursor':{'cases':4360,'instructions':[99,99],'selector_indices':list(range(19))},'queries':{'cases':9309,'reachable_instructions':[506,506],'owned_instructions':[462,462],'feasible_outcomes':[193,193],'ordinary_outcomes':[191,193],'mutation_only_edges':queries['conditional_outcomes_seen_only_with_declared_mutation'],'dead_unchanged_threshold_instructions':11,'controlled_endpoints':[]}},'known_1768_GetCursorPos_FALSE_gap':gap,'limitations':{name:p['limitations'] for name,p in [('modal',modal),('cursor',cursor),('queries',queries)]},'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'output':str(args.output),'actual_target_configurations':6,'CTest_per_configuration':134,'new_native_cases':15475,'new_comparisons':30950,'complete_menu_reconstruction':False}))
