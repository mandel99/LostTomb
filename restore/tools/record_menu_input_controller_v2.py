"""Record expanded whole-entry evidence and its exact remaining scope."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1];P=ROOT/'artifacts/menu_input_controller_phase'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
rel=lambda p:p.relative_to(ROOT).as_posix()
paths={Path(__file__)};coverage=set();edges=set();sqrt=set();cases=0;comparisons=0;suites=[];binary_sets=[]
for name,expected in [('baseline-v2',1024),('routes-v2',569)]:
 path=P/'metadata'/(name+'-emulation.json');report=read(path)
 assert report['pilot_passed'] and not report['native_profile_only'] and report['complete_case_set']
 assert report['whole_entry'] and report['checked']==report['complete_cases']==expected and report['comparisons']==expected*2
 assert not report['whole_input_dynamic_proof'] and not report['product_applied'] and not report['original_game_executed']
 for source,digest in {**report['source_hashes'],**report['binary_hashes']}.items():
  assert sha(ROOT/source)==digest,('current input proof drift',source)
  paths.add(ROOT/source)
 paths.add(path);coverage.update(report['covered_instructions']);edges.update(tuple(e) for e in report['covered_conditional_outcomes']);sqrt.update(report['sqrt_sites_executed'])
 cases+=expected;comparisons+=expected*2;binary_sets.append(report['binary_hashes'])
 suites.append({'name':name,'path':rel(path),'sha256':sha(path),'cases':expected,'comparisons':expected*2,'covered_instructions':report['owned_instruction_coverage']})
assert binary_sets[0]==binary_sets[1]
route=read(P/'metadata/routes-v2-emulation.json');assert route['ftol_instruction_coverage']==[15,15] and len(sqrt)==7
qualification=read(P/'metadata/first-append-qualification.json');excluded=set(qualification['excluded_instructions']);assert len(excluded)==155 and '004db4f6' in excluded and not(coverage&excluded)
assert sqrt==set(route['sqrt_sites'])-{'004db4f6'}
flags=read(P/'metadata/flag-reference-contract.json');assert flags['passed'] and not flags['C_sharp_used_as_exact_behavior']
assert flags['call_status_flag_output_obligations']==flags['consumed_undefined_integer_flags']==[]
for reference in flags['references']:assert sha(ROOT.parent/reference['path'])==reference['sha256']
generation=read(P/'metadata/generation.json');assert generation['authenticated_instructions']==6564 and generation['authenticated_bytes']==21947
for source,digest in {**generation['sources'],**qualification['sources'],**flags['sources']}.items():
 assert sha(ROOT/source)==digest
 paths.add(ROOT/source)
assert sha(P/'runtime/menu_input_controller.c')==generation['generated_C_sha256']
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
for cfg in ['Debug','Release']:
 project=ROOT/'build/menu-input-controller/menu_input_controller_verify.vcxproj';tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);link=groups[0].find('m:Link',ns);get=lambda name:cc.findtext('m:'+name,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true' and 'MENU_INPUT_TRACE' in get('PreprocessorDefinitions')
 assert link.findtext('m:BaseAddress',namespaces=ns)=='0x10000' and link.findtext('m:RandomizedBaseAddress',namespaces=ns)=='false'
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 assert actual=={P/'tests/controller_v2.c',P/'runtime/menu_input_controller.c',ROOT/'artifacts/menu_full_input_phase/runtime/menu_full_input_numeric.c'}
 binary=ROOT/'build/menu-input-controller'/cfg/'menu_input_controller_verify.exe'
 for source in actual|set((P/'include').glob('*.h'))|{P/'tests/controller_schema.h',project}:assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns
 paths.add(project)
# Reuse the unchanged independent integer proof, not its obsolete first-pilot
# project/configuration or whole-controller source assumptions.
historical=P/'reference/stage-state-first.json';prior=read(historical);paths.add(historical)
integer_paths=[P/'tests/integer.c',P/'include/menu_input_integer.h',P/'include/menu_input_controller.h',P/'project_integer/CMakeLists.txt',ROOT/'build/menu-input-integer/menu_input_integer_verify.vcxproj']
for cfg in ['Debug','Release']:
 record=prior['integer_variants'][cfg];binary=ROOT/record['binary'];assert sha(binary)==record['sha256']
 assert record['passed'] and record['ALU_cases']==983040 and record['shift_cases']==190464 and record['multiply_cases']==65536
 integer_paths.append(binary)
for path in integer_paths:
 assert sha(path)==prior['source_and_evidence_hashes'][rel(path)],('integer proof drift',path)
 paths.add(path)
paths.update([P/'metadata/generation.json',P/'metadata/flag-reference-contract.json',P/'metadata/first-append-qualification.json',P/'prepare_route_corpus.py'])
corpus=read(P/'fixtures/routes-v2.json');assert corpus['producer_sha256']==sha(P/'prepare_route_corpus.py') and corpus['seed_sha256']==sha(P/'fixtures/route-seeds.json');paths.add(P/'fixtures/route-seeds.json')
checkpoint=read(ROOT/'metadata/integration-batch-134-applied.json')
for source,digest in checkpoint['source_and_evidence_hashes'].items():assert sha(ROOT/source)==digest,('product134 drift during private stage',source)
missing=set(f'{int(row["address"],16):08x}' for row in read(ROOT/'artifacts/menu_full_input_phase/native-provider.json')['assembly']['items'])-coverage-excluded
out={'format':2,'generated_at':datetime.now(timezone.utc).isoformat(),'stage_checks_passed':True,'verification_in_progress':False,
 'complete_menu_reconstruction':False,'whole_input_dynamic_proof':False,'product_applied':False,'frozen_for_application':False,
 'authenticated_original_instructions':6564,'candidate_retains_all_original_instructions':True,
 'candidate_form':'Direct compiled C CFG; explicit integer temporaries/call frame and exact hardware x87 operations. No instruction decoder, dispatch interpreter or original code fallback.',
 'generated_C_build_requires_original':False,'original_game_executed':False,
 'whole_entry_pilot_cases':cases,'whole_entry_C_comparisons':comparisons,'suites':suites,
 'owned_instruction_coverage':[len(coverage),6564],'qualified_unreachable_instructions':qualification['excluded_instructions'],
 'remaining_uncovered_instructions':sorted(missing),'remaining_uncovered_count':len(missing),
 'conditional_outcomes':[len(edges),1188],'whole_entry_x87_FTOL_coverage':[15,15],
 'sqrt_sites_executed':sorted(sqrt),'sqrt_site_infeasible_in_prepared_domain':'004db4f6',
 'route_masked_control_words':24,'independent_integer_checks_per_build':1239040,
 'integer_variants':prior['integer_variants'],'existing_product_checkpoint':134,
 'C_sharp_used_as_exact_behavior':False,
 'qualifications':['All tests enter4D8310 normally. Route writes stay within actual prepared/requested capacities; original failing allocation inputs are not executed.',
  'The first append block can only start with zero vertices, creates one and skips its sample builder.155 instructions are qualified unreachable under bounded nonalias/single-threaded route state; all are retained.',
  'Actual raw80 ST0 and full FTOL EDX:EAX return are compared. Incoming EAX/EDX/stack words are not FTOL arguments; FPU TOP/exception residue in those unused integer registers is not promoted to behavior.',
  'Consumed integer flags are defined by owned instructions. No caller branch consumes undefined integer flags or external callee flag outputs.'],
 'remaining_obligations':['Cover remaining owned instructions/conditional outcomes and prove any additional exclusions from actual native dataflow.',
  'Compose103 exact typed endpoint adapters and actual accepted algorithms; this private whole-controller proof still controls deeper calls except native FTOL.',
  'Add endpoint entry/exit field mutations and independent trace/plain checks before guarded production application.',
  'Complete physical menu rendering, OS/persistence effects and usable full menu flow.'],
 'source_and_evidence_hashes':{rel(path):sha(path) for path in sorted(paths)}}
(P/'metadata/stage-state.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({key:out[key] for key in ['stage_checks_passed','whole_entry_pilot_cases','whole_entry_C_comparisons','owned_instruction_coverage','remaining_uncovered_count','whole_input_dynamic_proof']}))
