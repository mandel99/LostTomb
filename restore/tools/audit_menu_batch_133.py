"""Audit actual five-function text and actor-distance production integration."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,struct,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
app=read('metadata/integration-batch-133-application.json');assert sha(app['predecessor'])==app['predecessor_sha256']
for p,h in {**app['written'],**app['protected_unchanged'],**app['frozen_inputs']}.items():assert sha(p)==h,('application drift',p)
tracked=set(app['written'])|set(app['protected_unchanged'])|set(app['frozen_inputs'])|{'metadata/integration-batch-133-application.json',Path(__file__).relative_to(ROOT).as_posix()}
maps=['artifacts/menu_text_glyph_phase/apply-map.json','artifacts/actor_order_flood_phase/metadata/apply-map.json']
for manifest in ['artifacts/menu_text_glyph_phase/freeze-manifest.json','artifacts/actor_order_flood_phase/metadata/freeze-manifest.json']:
 frozen=read(manifest)
 for p,h in frozen['files'].items():assert sha(p)==h,('frozen drift',p)
 tracked.update(frozen['files'])
for mapping in maps:
 for r in read(mapping)['files']:
  assert sha(r['staged'])==sha(r['target'])==r['candidate_sha256']
  before='artifacts/integration-batch-133-before/'+r['target'];assert (sha(before) if (ROOT/before).exists() else None)==r['before_sha256']
  if (ROOT/before).exists():tracked.add(before)
text=read('metadata/menu-text-glyph-emulation.json');flood=read('metadata/actor-order-flood-emulation.json')
for name,p,count in [('menu-text-glyph',text,3062),('actor-order-flood',flood,174)]:
 assert p['passed'] and p['production'] and p['complete_case_set'] and p['checked']==count and p['comparisons']==count*2 and p['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
 for source,h in {**p['source_hashes'],**p.get('native_exports',{}),**p.get('binary_hashes',{})}.items():assert sha(source)==h,('proof drift',name,source)
 tracked.update(p['source_hashes']);tracked.update(p.get('native_exports',{}));tracked.update(p.get('binary_hashes',{}));tracked.add('metadata/'+name+'-emulation.json')
assert text['native_instruction_coverage']==[367,367] and text['conditional_outcomes']==[78,78] and text['uncovered_instructions']==text['missing_conditional_outcomes']==[] and len(text['mutated_fields_and_phases'])==54
assert text['coverage']=={'004cbf30':[65,65],'004cc720':[32,32],'004cc770':[206,206],'004cca60':[55,55],'004ccb00':[9,9]}
assert text['case_selection']=={'limit':None,'single_case':False} and not text['original_game_executed']
q=flood['qualified_controller'];assert q['complete'] and q['native_instructions']==187 and q['feasible_instructions']==q['executed']==185 and q['conditional_edges']==64 and q['feasible_edges']==q['edges_executed']==62
assert q['missing_instructions']==q['missing_edges']==[] and q['excluded_instructions']==['0051cf79','0051d13e'] and q['excluded_edges']==[['0051cf73','0051cf79'],['0051d13c','0051d13e']]
assert q['tail_wrap_sites']==['east','north','retry','south','west'] and flood['actual_fill_coverage']=={'native_instructions':22,'executed':22,'missing':[]}
assert flood['controlled_deep_boundaries']==[] and not flood['original_game_process_executed'] and not flood['original_machine_code_fallback_used']
assert flood['watched_memory']['flood_bytes_per_case']==520000 and flood['watched_memory']['every_payload_and_guard_byte_compared'] and flood['watched_memory']['native_writes_restricted_to_exact_payloads_and_stack']
assert len(flood['native_fixture_metrics'])==174
abi=read('artifacts/menu_text_glyph_phase/abi-reference-review.json');assert abi['complete'] and abi['native_instructions']==367 and sum(x['instructions'] for x in abi['records'])==367
assert len(abi['records'])==5 and all(x['complete_direct_CFG'] and x['RET0_addresses'] for x in abi['records'])
ref=abi['C_sharp_reference'];assert ref['used_as_exact_behavior'] is False and sha('../'+ref['path'])==ref['sha256'];tracked.add('../'+ref['path'])
for p,h in abi['inputs'].items():assert sha(p)==h
native=read('metadata/actor-order-flood-native.json');assert native['counter_qualification']['no_external_calls_after_initialization'] and native['counter_qualification']['excluded_instructions']==q['excluded_instructions']
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};configurations={}
text_sources=['artifacts/menu_text_glyph_phase/tests/text_glyph.c']+[f'src/unit_{n:05d}.c' for n in [1667,1691,1692,1693,1694]]
flood_recipe=read('artifacts/actor_order_flood_phase/metadata/integration-recipe.json')['oracle_CMake']
for cfg,build,label in [('Debug','c-only-debug','Debug'),('Release','c-only','O2')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-133-'+cfg.lower()+'-build.log';s=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 138' in s and 'C source build completed. No hybrid game was built or launched.' in s
 rows=[]
 for target,expected,p in [('restore_menu_text_glyph_verify',text_sources,text),('restore_actor_order_flood_verify',flood_recipe['sources'],flood)]:
  project=directory/(target+'.vcxproj');tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
  cc=groups[0].find('m:ClCompile',ns);link=groups[0].find('m:Link',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns);lget=lambda n:link.findtext('m:'+n,default='',namespaces=ns)
  assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
  assert not any(x in get('PreprocessorDefinitions') for x in ['STAGED','TRACE'])
  assert lget('BaseAddress')=='0x10000' and lget('FixedBaseAddress')=='true' and lget('RandomizedBaseAddress')=='false' and '/machine:X86' in lget('AdditionalOptions')
  actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={ROOT/s for s in expected}
  if target=='restore_actor_order_flood_verify':assert 'FLOOD_FILL_SOURCE="'+(ROOT/'src/unit_00119.c').as_posix()+'"' in get('PreprocessorDefinitions') and ROOT/'src/unit_00119.c' not in actual
  binary=directory/cfg/(target+'.exe');h=sha(binary.relative_to(ROOT));want=p['variants'][label]['sha256'] if 'variants' in p else p['compiled_C_sha256' if cfg=='Debug' else 'comparison_C_sha256'];assert h==want
  for source in actual|{project,ROOT/'CMakeLists.txt'}|{ROOT/s for s in p['source_hashes'] if s.endswith(('.c','.h'))}:assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns,('oracle freshness',target,source)
  rows.append({'target':target,'path':binary.relative_to(ROOT).as_posix(),'sha256':h,'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'sources':[s.relative_to(ROOT).as_posix() for s in sorted(actual)]});tracked.update([project.relative_to(ROOT).as_posix(),binary.relative_to(ROOT).as_posix()])
 product=directory/'pharaoh_restored.vcxproj';t=ET.parse(product);actual={Path(n.get('Include')).resolve() for n in t.findall('.//m:ClCompile[@Include]',ns)};required={ROOT/p for p in app['written'] if p.startswith('src/') and p.endswith('.c')};assert required<=actual and not any('artifacts' in p.relative_to(ROOT).parts for p in actual)
 library=directory/cfg/'pharaoh_restored.lib';link_check=directory/cfg/'restore_link_check.exe'
 for source in required|{ROOT/'include/menu_text_glyph.h',ROOT/'include/menu_text_glyph_memory.h',ROOT/'include/actor_order_flood.h',product}:assert library.stat().st_mtime_ns>=source.stat().st_mtime_ns
 assert link_check.stat().st_mtime_ns>=library.stat().st_mtime_ns
 raw=link_check.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 configurations[cfg]={'CTest_passed':138,'targets':rows,'product_sha256':sha(library.relative_to(ROOT)),'link_check_sha256':sha(link_check.relative_to(ROOT))}
 tracked.update([log,product.relative_to(ROOT).as_posix(),library.relative_to(ROOT).as_posix(),link_check.relative_to(ROOT).as_posix(),'build/'+build+'/CMakeCache.txt'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'source_only_build':True,'original_game_executed':False,'text':{'cases':3062,'comparisons':6124,'instructions':[367,367],'conditional_outcomes':[78,78],'mutated_fields_and_phases':54,'watched_bytes':148289,'sprite_boundary':'53E590;ECX=F55BB0;3args;RET12;actual pixels remain outside this proof'},'actor_distance':{'cases':174,'comparisons':348,'retained_instructions':187,'feasible_instructions':[185,185],'feasible_edges':[62,62],'qualified_excluded_instructions':q['excluded_instructions'],'qualified_excluded_edges':q['excluded_edges'],'tail_wrap_sites':q['tail_wrap_sites'],'actual_fill_instructions':[22,22],'controlled_boundaries':[],'bytes_per_full_distance_case':520000},'C_sharp_used_as_exact_behavior':False,'configurations':configurations,'limitations':{'text':text['limitations'],'distance':flood['limitations']},'source_and_evidence_hashes':{p:sha(p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-133-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'passed':True,'shared_target_configurations':4,'CTest_per_configuration':138,'new_cases':3236,'new_comparisons':6472,'complete_menu_reconstruction':False}))
