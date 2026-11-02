"""Immutable map-stage integration inventory. Writes only this new staging directory."""
from pathlib import Path
import argparse,collections,hashlib,json,re,sys
H=Path(__file__).resolve().parent;R=H.parent.parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();rel=lambda p:p.relative_to(R).as_posix();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
PHASES=[('MN','map_neighbors_phase','map_neighbors_phase_apply_map.json',[]),('RAW80','map_refresh_phase/x87_conversion','x87_conversion_freeze.json',[]),('LEAF','map_refresh_phase','map_refresh_leaf_apply_map.json',[]),('ARITH','map_refresh_arithmetic_phase','map_refresh_arithmetic_apply_map.json',['RAW80','LEAF']),('P1','map_refresh_postpass1_phase','postpass1_apply_map.json',['ARITH']),('P2','map_refresh_postpass2_phase','postpass2_apply_map.json',[]),('P3','map_refresh_postpass3_phase','postpass3_apply_map.json',['MN','P2','ARITH']),('P4','map_refresh_postpass4_phase','postpass4_apply_map.json',[]),('P5','map_refresh_postpass5_phase','postpass5_apply_map.json',['P2','P4']),('P6','map_refresh_postpass6_phase','postpass6_apply_map.json',[]),('P7','map_refresh_postpass7_phase','postpass7_apply_map.json',['P6']),('P8','map_refresh_postpass8_phase','postpass8_apply_map.json',['MN','P2'])]
# Independent suites. Counts are suite inputs, not deduplicated game scenarios.
SUITES=[('MN','verify_map_neighbors.py',17308,4,'restore_map_neighbors','map-neighbors-emulation.json'),('LEAF','verify_map_refresh_leaf.py',11358,4,'restore_map_refresh_leaf','map-refresh-leaf-emulation.json'),('ARITH','verify_weighted_terrain.py',2216,4,'restore_map_refresh_weighted','map-refresh-weighted-emulation.json'),('P1','verify_postpass1.py',3088,4,'restore_map_refresh_postpass1','map-refresh-postpass1-emulation.json'),('P1','verify_postpass1_byte_extensions.py',4,4,'restore_map_refresh_postpass1','map-refresh-postpass1-byte-extensions.json'),('P2','verify_postpass2.py',12288,4,'restore_map_refresh_postpass2','map-refresh-postpass2-emulation.json'),('P3','verify_postpass3.py',2395,4,'restore_map_refresh_postpass3','map-refresh-postpass3-emulation.json'),('P4','verify_postpass4.py',4674,4,'restore_map_refresh_postpass4','map-refresh-postpass4-emulation.json'),('P5','verify_postpass5.py',6144,4,'restore_map_refresh_postpass5','map-refresh-postpass5-emulation.json'),('P6','verify_postpass6.py',4675,4,'restore_map_refresh_postpass6','map-refresh-postpass6-emulation.json'),('P7','verify_postpass7.py',2738,4,'restore_map_refresh_postpass7','map-refresh-postpass7-emulation.json'),('P8','verify_postpass8.py',9844,4,'restore_map_refresh_postpass8','map-refresh-postpass8-emulation.json'),('RAW80','@component:raw80',42768,2,'restore_map_refresh_x87_conversion_verify','map-refresh-x87-component-emulation.json'),('ARITH','@component:arithmetic',20224,2,'restore_map_refresh_numeric_verify','map-refresh-numeric-component-emulation.json'),('P8','@component:post8-leaf',1465,2,'restore_map_refresh_postpass8_leaf_verify','map-refresh-postpass8-leaf-emulation.json')]
def generate():
 assert sha(R/'build/smoke-game/Pharaoh.exe')==PIN
 rows=collections.defaultdict(list);phases=[];frozen={};checks=[]
 def check(p,want):
  assert p.exists() and sha(p)==want,rel(p);frozen[rel(p)]=want
 for n,directory,name,deps in PHASES:
  p=R/'artifacts'/directory/name;d=json.loads(p.read_text());assert d.get('frozen') and d.get('native_sha256',PIN)==PIN;frozen[rel(p)]=sha(p)
  for name,want in d.get('proof_hashes',{}).items():check(R/name,want)
  if n=='RAW80':
   for name,want in d['files'].items():check(p.parent/name,want)
   maps=[dict(m,target=m['eventual_target'],staged_sha256=m['after_sha256']) for m in d['candidate_new_files']]
  else:maps=d['mappings']
  for m in maps:
   check(R/m['staged'],m['staged_sha256']);rows[m['target']].append({'phase':n,'manifest':rel(p),**m})
  phases.append({'id':n,'manifest':rel(p),'sha256':sha(p),'requires':deps,'dependency_qualification':'Package ordering; P1/P3 require the arithmetic/raw80 components, not additional worker calls. P8 depends on selected1233/1139, not legacy root bodies.' if n in ['P1','P3','P8'] else 'Actual native/C/header dependencies; chronology alone is not an edge.','frozen_recipe':rel(p.parent/'shared_targets.cmake') if (p.parent/'shared_targets.cmake').exists() else None})
 mappings=[];duplicates=[];conflicts=[];units=(R/'src/units.cmake').read_text()
 for target,decls in sorted(rows.items()):
  versions=collections.defaultdict(list)
  for m in decls:versions[m['staged_sha256']].append(m['phase'])
  if len(versions)!=1:conflicts.append({'target':target,'versions':versions});continue
  first=decls[0];after=first['staged_sha256'];p=R/target;current=sha(p) if p.exists() else None;expected={m.get('before_sha256',m.get('expected_target_preapply_sha256')) for m in decls};status='already_identical' if current==after else 'pending_exact_before' if current in expected else 'before_drift'
  if target.startswith('src/'):assert units.count('/'+target+'"')==1,target
  mappings.append({'target':target,'staged':first['staged'],'after_sha256':after,'staged_sha256':after,'before_sha256':current,'before_exists':p.exists(),'accepted_before_hashes':sorted(x for x in expected if x),'accepted_before_absent':None in expected,'status':status,'declared_by':[m['phase'] for m in decls],'canonical_unit_already_in_RESTORE_UNITS':target.startswith('src/'),'all_declarations':decls})
  if len(decls)>1:duplicates.append({'target':target,'classification':'identical content duplicate; copy once, no superseding version','sha256':after,'phases':[m['phase'] for m in decls]})
 assert not conflicts and not [m for m in mappings if m['status']=='before_drift']
 # Every prior proof root-source expectation: planned replacements are compared against selected-after,
 # not naively against historical current-root snapshots embedded in private proof reports.
 expectation_rows=[]
 for n,directory,name,deps in PHASES:
  p=R/'artifacts'/directory/name;d=json.loads(p.read_text())
  for key in ['unchanged_actual_C_dependencies','unchanged_actual_root_sources']:
   obj=d.get(key,{})
   if isinstance(obj,dict):
    for path,want in obj.items():
     if not isinstance(want,str):continue
     check(R/path,want);expectation_rows.append({'phase':n,'path':path,'required_sha256':want,'overlaps_apply_target':path in rows})
 # Include latest ledger evidence and prove all unchanged selected roots remain unchanged.
 lp=R/'artifacts/map_refresh_postpass8_phase/map-refresh-dependency-ledger.json';ledger=json.loads(lp.read_text());frozen[rel(lp)]=sha(lp)
 for path,want in ledger['proof_artifact_hashes'].items():check(R/path,want)
 for e in ledger['entries'].values():
  s=e.get('selected_C')
  if isinstance(s,dict) and s.get('path','').startswith('src/') and s['path'] not in rows:check(R/s['path'],s['sha256'])
 # Direct canonical reference inventory and ABI transitions, advisory caller audit only.
 symbols={};abi=[]
 for m in mappings:
  if not m['target'].startswith('src/'):continue
  text=(R/m['staged']).read_text(encoding='utf-8-sig');match=re.search(r'(?m)^([^\n;{}]+?)\s+(FUN_[0-9a-fA-F]{8})\s*\(([^)]*)\)\s*\{',text);assert match,m
  old=(R/m['target']).read_text(encoding='utf-8-sig');oldmatch=re.search(r'(?m)^([^\n;{}]+?)\s+'+match[2]+r'\s*\(([^)]*)\)',old)
  symbols[match[2]]={'target':m['target'],'staged_signature':match[1]+' '+match[2]+'('+match[3]+')','current_signature':oldmatch[1]+' '+match[2]+'('+oldmatch[2]+')' if oldmatch else None}
 for path in sorted((R/'src').glob('unit_*.c')):
  if rel(path) in rows:continue
  for line_no,line in enumerate(path.read_text(encoding='utf-8-sig').splitlines(),1):
   for symbol in symbols:
    if re.search(r'\b'+symbol+r'\s*\(',line):abi.append({'path':rel(path),'line':line_no,'symbol':symbol,'context':line.strip(),'classification':'legacy extern declaration' if 'extern ' in line else 'external caller lead; not whole-caller proof'})
 cmake=(R/'CMakeLists.txt').read_text();impacted=[]
 for match in re.finditer(r'add_executable\s*\((.*?)\)',cmake,re.S):
  block=match[1];touch=sorted({m['target'] for m in mappings if m['target'] in block})
  if touch:impacted.append({'target':block.split()[0],'replaced_sources':touch,'action':'Rebuild/replay this real existing target; no test-body or original-program run.'})
 recipes=[p['frozen_recipe'] for p in phases if p['frozen_recipe']];suites=[];target_names=[]
 for phase,script,count,variants,prefix,output in SUITES:
  ph=next(p for p in phases if p['id']==phase);manifest=R/ph['manifest'];argv=[]
  if script.startswith('@component:'):
   argv=['{python}',rel(H/'verify_map_numeric_components.py'),'--component',script.split(':')[1],'--production','--exe','{debug}/'+prefix+'.exe','--optimized-exe','{release}/'+prefix+'.exe'];targets=[prefix]
  else:
   script_path=manifest.parent/script;argv=['{python}',rel(script_path),'--production'];text=script_path.read_text()
   if '--manifest-sha256' in text:argv+=['--manifest-sha256',ph['sha256']]
   argv+=['--exe','{debug}/'+prefix+'_verify.exe','--plain-exe','{debug}/'+prefix+'_plain_verify.exe','--optimized-exe','{release}/'+prefix+'_verify.exe','--optimized-plain-exe','{release}/'+prefix+'_plain_verify.exe'];targets=[prefix+'_verify',prefix+'_plain_verify']
  suites.append({'phase':phase,'script':script,'checked':count,'variants':variants,'comparisons':count*variants,'targets':targets,'argv_template':argv,'output':'metadata/'+output,'corpus_changed':False})
  target_names.extend(targets)
 targets=list(dict.fromkeys(target_names));assert len(targets)==25
 report={'format':1,'read_only_audit':True,'frozen':True,'frozen_integration_plan':True,'product_applied':False,'native_sha256':PIN,'original_game_executed':False,'root_checkpoint':{'path':'metadata/integration-batch-134-applied.json','sha256':sha(R/'metadata/integration-batch-134-applied.json'),'CTest_count_at_checkpoint':139},'root_inputs_at_audit':{p:sha(R/p) for p in ['CMakeLists.txt','src/units.cmake','include/restore_types.h']},'phases_in_topological_order':phases,'mappings':mappings,'mapping_counts':dict(collections.Counter(m['target'].split('/')[0] for m in mappings)),'total_distinct_targets':len(mappings),'divergent_version_conflicts':conflicts,'identical_duplicate_declarations':duplicates,'unchanged_dependency_expectations':expectation_rows,'canonical_exports':symbols,'external_caller_references':abi,'external_caller_qualification':['Reference inventory is advisory, not whole-call verification. Legacy callers are not silently included in this promotion.','481AA0 returns canonical fullEAX0/1; old unsigned-char bool callers cannot lose a meaningful high24 result. Existing caller geometry/table/width gaps remain independent work.','481540 changes the translated3argument/void declaration to actual4argument/int32; its real481690 caller is replaced in the same arithmetic stage.','487B70 changes a spurious translatedpointer-x parameter to realint32 x; recursive and4877F0 calls are replaced by exactP6/P7.','488E90,canonical489030,475CC0 and canonical implicitST0/56133C remain open; do not replace an original ST0 ABI with the typed raw80 helper.'],'existing_directly_affected_test_targets':impacted,'normal_product_CMake':{'canonical_units':'All28 already occur exactlyonce in RESTORE_UNITS; do not append numberedunits to pharaoh_restored again.','add_runtime_sources':['runtime/map_refresh_fp.c','runtime/map_x87_conversion.c'],'runtime':'MultiThreaded$<$<CONFIG:Debug>:Debug> =>DebugMTd/ReleaseMT','root_pharaoh_restored_optimization':'/Od in both configurations; own verification targets use /Od Debug,/O2 Release','aggregate_recipe':rel(H/'shared_targets.cmake'),'include_after_targets':['pharaoh_restored','restore_data'],'verification_targets':targets,'additional_CTests_if_none_preexisting':25,'expected_CTest_count_if_root134_is_unchanged':164,'frozen_recipes_included':recipes},'production_replays':suites,'production_suite_totals':{'suites':len(suites),'canonical_suite_cases':sum(s['checked'] for s in suites),'variant_comparisons':sum(s['comparisons'] for s in suites),'counts_deduplicate_game_scenarios':False,'counts_include_original139tests_or97suites':False},'native_only_immutable_evidence':[{'path':'artifacts/map_refresh_postpass1_phase/postpass1-value-ranges.json','native_probes':12288,'C_cases':0},{'path':'artifacts/map_refresh_postpass8_phase/primary-record-reachability.json','native_probes':3072,'C_cases':0},{'path':'artifacts/map_refresh_postpass7_phase/rejected-intmin-bound-probe.json','rejected_out_of_prepared_domain_probe':1,'C_cases':0}],'remaining_closed_graph_work':{'ledger':rel(lp),'sha256':sha(lp),'nodes':ledger['remaining_algorithm_nodes_without_currentcandidate'],'FP_qualification':ledger['FP_qualification'],'whole_map_refresh_complete':False},'frozen_evidence_hashes':frozen,'implementation_order':['Authenticate this manifest/all frozen evidence and every current target beforehash.','Copy42distincttargets once; raw80duplicate declarations are identical, no last-writer version override. Keep original unit membership.','Include aggregate recipe once after pharaoh_restored/restore_data; it adds the2runtimeC and25verificationtargets, preserves rootDebugMTd/ReleaseMT.','Fresh Debug/Release buildall including linkcheck and all existing139 tests; no source/binary modifications during proofs.','Run all15productionreplays with actualDebug/Release paths; compare expectedcounts and retain native-only evidence separately.','Run root broader menu/map/terrain suites against fresh source/library/binary hashes; publish a new actualintegration checkpoint only after success.'],'limitations':['Audit/source composition and recipe only; no rootsource/CMake/registry changes and no new whole-nativealgorithm proof from the inventory.','All older frozen manifests/files remain immutable; production report paths are metadata, not the old artifact reports.','Masked-CW numeric/raw80 result domains and binary64spill qualifications retained. No generalizedFPSW/unmaskedtrap claim and no canonicalglobalST0 promotion.']}
 (H/'integration-manifest.json').write_text(json.dumps(report,indent=2)+'\n');return report
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--check-before',action='store_true');ap.add_argument('--check-applied',action='store_true');a=ap.parse_args()
if a.check_before or a.check_applied:
 d=json.loads((H/'integration-manifest.json').read_text())
 for p,want in d['frozen_evidence_hashes'].items():assert sha(R/p)==want,p
 for m in d['mappings']:
  assert sha(R/m['staged'])==m['after_sha256'];current=sha(R/m['target']) if (R/m['target']).exists() else None;assert current==(m['after_sha256'] if a.check_applied else m['before_sha256']),m['target']
 print(json.dumps({'passed':True,'mode':'applied' if a.check_applied else 'before','targets':len(d['mappings']),'product_written':False}))
else:
 assert not (H/'integration-freeze.json').exists(),'Frozen integration plan is immutable; use --check-before/--check-applied'
 d=generate();print(json.dumps({k:d[k] for k in ['total_distinct_targets','mapping_counts','identical_duplicate_declarations','production_suite_totals','existing_directly_affected_test_targets']},indent=2))
