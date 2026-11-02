"""Record current whole-controller pilots without promoting an incomplete prototype."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re
import subprocess
import xml.etree.ElementTree as ET

ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/menu_input_controller_phase'
read=lambda path:json.loads(path.read_text(encoding='utf-8-sig'))
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
rel=lambda path:path.relative_to(ROOT).as_posix()
generation=read(PHASE/'metadata/generation.json');pilot=read(PHASE/'metadata/pilot-emulation.json')
flags=read(PHASE/'metadata/flag-reference-contract.json')
assert generation['authenticated_instructions']==6564 and generation['authenticated_bytes']==21947
assert pilot['pilot_passed'] and pilot['whole_entry'] and pilot['checked']==1024 and pilot['comparisons']==2048
assert pilot['owned_instruction_coverage']==[3061,6564] and pilot['ftol_instruction_coverage']==[0,15]
assert not pilot['whole_input_dynamic_proof'] and not pilot['product_applied'] and not pilot['original_game_executed']
assert flags['passed'] and not flags['C_sharp_used_as_exact_behavior']
assert flags['call_status_flag_output_obligations']==flags['consumed_undefined_integer_flags']==[]
tracked={Path(__file__),PHASE/'metadata/generation.json',PHASE/'metadata/pilot-emulation.json',PHASE/'metadata/flag-reference-contract.json'}
for record,key in [(generation,'sources'),(pilot,'source_hashes'),(pilot,'binary_hashes'),(flags,'sources')]:
 for path,digest in record[key].items():
  assert sha(ROOT/path)==digest,('stale input stage evidence',path)
  tracked.add(ROOT/path)
for reference in flags['references']:
 assert sha(ROOT.parent/reference['path'])==reference['sha256']
assert sha(PHASE/'runtime/menu_input_controller.c')==generation['generated_C_sha256']
numeric_manifest=ROOT/'artifacts/menu_full_input_phase/numeric-freeze.json'
numeric=read(numeric_manifest);tracked.add(numeric_manifest)
for path,digest in numeric['files'].items():
 assert sha(ROOT/path)==digest
 tracked.add(ROOT/path)
# Confirm actual generated compile inputs and independent Win32 configurations.
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
for cfg in ['Debug','Release']:
 project=ROOT/'build/menu-input-controller/menu_input_controller_verify.vcxproj';tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda name:cc.findtext('m:'+name,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
 assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true' and 'MENU_INPUT_TRACE' in get('PreprocessorDefinitions')
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 assert actual=={PHASE/'tests/controller.c',PHASE/'runtime/menu_input_controller.c',ROOT/'artifacts/menu_full_input_phase/runtime/menu_full_input_numeric.c'}
 binary=ROOT/'build/menu-input-controller'/cfg/'menu_input_controller_verify.exe'
 for source in actual|set((PHASE/'include').glob('*.h')):
  assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns
 tracked.add(project)
integer_sources={PHASE/'tests/integer.c',PHASE/'include/menu_input_integer.h',PHASE/'include/menu_input_controller.h',PHASE/'project_integer/CMakeLists.txt'}
tracked|=integer_sources
before={rel(path):sha(path) for path in tracked};integer_variants={}
for cfg in ['Debug','Release']:
 binary=ROOT/'build/menu-input-integer'/cfg/'menu_input_integer_verify.exe'
 project=ROOT/'build/menu-input-integer/menu_input_integer_verify.vcxproj'
 tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda name:cc.findtext('m:'+name,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
 assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
 assert {Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}=={PHASE/'tests/integer.c'}
 for source in integer_sources-{PHASE/'project_integer/CMakeLists.txt'}:assert binary.stat().st_mtime_ns>=source.stat().st_mtime_ns
 before.update({rel(binary):sha(binary),rel(project):sha(project)})
 result=subprocess.run([str(binary)],text=True,capture_output=True,timeout=30);assert result.returncode==0,result.stderr
 observed=json.loads(result.stdout)
 assert observed=={'passed':True,'ALU_cases':983040,'shift_cases':190464,'multiply_cases':65536,'reference':'actual host x86 arithmetic; defined result/flags only'}
 (PHASE/(cfg.lower()+'-integer-test.log')).write_text(result.stdout+result.stderr)
 integer_variants[cfg]=observed|{'binary':rel(binary),'sha256':sha(binary)}
for path,digest in before.items():assert sha(ROOT/path)==digest,('evidence changed during audit',path)
checkpoint=read(ROOT/'metadata/integration-batch-134-applied.json')
for path,digest in checkpoint['source_and_evidence_hashes'].items():assert sha(ROOT/path)==digest,('product134 drift',path)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'stage_checks_passed':True,
 'complete_menu_reconstruction':False,'whole_input_dynamic_proof':False,'product_applied':False,'frozen_for_application':False,
 'authenticated_original_instructions':6564,'candidate_retains_all_original_instructions':True,
 'candidate_form':'Direct compiled C control flow and exact-width accesses with explicit temporaries/call frame. Typed hardware x87 operations; no instruction decoder, dispatch interpreter or original machine-code fallback.',
 'generated_C_build_requires_original':False,'original_game_executed':False,
 'whole_entry_pilot_cases':1024,'whole_entry_C_comparisons':2048,'owned_instruction_coverage':[3061,6564],
 'whole_entry_x87_FTOL_coverage':[0,15],'integer_variants':integer_variants,
 'existing_product_checkpoint':134,'existing_product_hashes_current':len(checkpoint['source_and_evidence_hashes']),
 'remaining_obligations':[
  '3503 owned instructions have no whole-entry dynamic hit in this initial prepared corpus; coverage alone will not establish all state/branch behavior.',
  'Exercise all eight route/sample x87 blocks and actual FTOL through whole entries, with masked control-word and aliased-output qualifications.',
  'Replace the103 potential controlled endpoint boundaries with exact argument/receiver/result adapters and compose accepted actual implementations.',
  'Prove reachable conditional outcomes and explicit infeasibility certificates, call-site state mutations, guarded private-frame/data domains, and trace/plain behavior.',
  'Integrate only after production replay; actual OS effects, persistence, complete rendering and playable menu flow remain separately required.'
 ],'source_and_evidence_hashes':before}
(PHASE/'metadata/stage-state.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({key:report[key] for key in ['stage_checks_passed','whole_entry_pilot_cases','whole_entry_C_comparisons','owned_instruction_coverage','product_applied','complete_menu_reconstruction']}))
