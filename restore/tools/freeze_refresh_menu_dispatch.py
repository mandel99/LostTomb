"""Preserve the passing whole-refresh candidate and its native oracle unchanged."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/refresh_menu_dispatch_phase'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
proof=json.loads((PHASE/'private-emulation.json').read_text())
assert proof['passed'] and proof['complete_case_set'] and proof['checked']==11437 and proof['comparisons']==22874
assert proof['native_instruction_coverage']==[960,960] and proof['controller_instruction_coverage']==[917,917] and proof['conditional_outcomes']==[250,250]
assert len(proof['reached_endpoints'])==96 and len(proof['mutated_endpoint_phases'])==188
for p,h in {**proof['source_hashes'],**proof['binary_hashes']}.items():assert sha(ROOT/p)==h,p
assert sha(ROOT/'src/unit_01867.c')=='638bc1f8ed206a710ce6c3afa82e324dff781a3a3ecf0528bacd7d9f7ded6b9c'
state=json.loads((PHASE/'stage-state.json').read_text())
state.update(C_equivalence_verified=True,product_applied=False,
 private_compile='Actual executable fixture and candidate/provider archive compiled Debug /Od and Release /O2, /W4 /WX; see private-emulation.json for behavior evidence.',
 fixture_preparation='Executable fixture and pinned original-instruction comparator passed 11437 cases in both configurations; 960/960 instructions and 250/250 branch outcomes. Ninety-six complex endpoints remain controlled.',
 next_required_work=['Guarded application to production source and shared CMake targets.','Repeat complete comparison against actual production Debug/Release builds.','Independently reconstruct the 96 named deeper algorithms and verify physical screen/OS behavior before whole-menu completion.'])
(PHASE/'stage-state.json').write_text(json.dumps(state,indent=2)+'\n')
manifest=PHASE/'freeze-manifest.json';assert not manifest.exists()
files={p.relative_to(ROOT).as_posix():sha(p) for p in sorted(PHASE.rglob('*')) if p.is_file() and '__pycache__' not in p.parts and p.name not in ['partial-emulation.json','last-difference.json']}
manifest.write_text(json.dumps({'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production_applied':False,'whole_menu_complete':False,'files':files,'external_sources':proof['source_hashes'],'private_binaries':proof['binary_hashes'],'private_cases':11437,'private_comparisons':22874,'instructions':[960,960],'controller_instructions':[917,917],'conditional_outcomes':[250,250],'controlled_endpoints':96,'mutated_endpoint_phases':188,'limitations':proof['limitations']},indent=2)+'\n')
print(json.dumps({'frozen_files':len(files),'manifest_sha256':sha(manifest)}))
