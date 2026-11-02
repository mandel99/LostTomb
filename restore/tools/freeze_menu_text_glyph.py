"""Freeze the complete five-function text proof before production application."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/menu_text_glyph_phase'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
proof=json.loads((PHASE/'private-emulation.json').read_text());abi=json.loads((PHASE/'abi-reference-review.json').read_text())
assert proof['passed'] and proof['complete_case_set'] and proof['checked']==3062 and proof['comparisons']==6124
assert proof['native_instruction_coverage']==[367,367] and proof['conditional_outcomes']==[78,78] and len(proof['mutated_fields_and_phases'])==54
assert abi['complete'] and abi['native_instructions']==367 and len(abi['records'])==5
for p,h in {**proof['source_hashes'],**proof['binary_hashes'],**abi['inputs']}.items():assert sha(ROOT/p)==h,p
reference=ROOT.parent/abi['C_sharp_reference']['path'];assert sha(reference)==abi['C_sharp_reference']['sha256']
state=json.loads((PHASE/'stage-state.json').read_text());state.update(C_equivalence_verified=True,product_applied=False,private_native_cases=3062,private_C_comparisons=6124,native_instruction_coverage=[367,367],conditional_outcomes=[78,78],mutated_field_phases=54,remaining_for_application=['Guarded source copy and actual shared Debug/Release comparison.','Deeper53E590 sprite/raster and physical pixels are outside this five-function proof; full menu is not claimed.'])
(PHASE/'stage-state.json').write_text(json.dumps(state,indent=2)+'\n')
paths=[f'src/unit_{n:05d}.c' for n in [1667,1691,1692,1693,1694]]+['include/menu_text_glyph.h','include/menu_text_glyph_memory.h']
records=[]
for p in paths:
 dest=ROOT/p;source=PHASE/p;records.append({'staged':source.relative_to(ROOT).as_posix(),'target':p,'before_sha256':sha(dest) if dest.exists() else None,'candidate_sha256':sha(source)})
mapping=PHASE/'apply-map.json';assert not mapping.exists();mapping.write_text(json.dumps({'format':1,'files':records,'scope':'Whole367-instruction five-function text pipeline. Private actual C/native full proof3062x2; one typed deeper sprite boundary. Root must replay actual shared builds.'},indent=2)+'\n')
manifest=PHASE/'freeze-manifest.json';assert not manifest.exists()
files={p.relative_to(ROOT).as_posix():sha(p) for p in sorted(PHASE.rglob('*')) if p.is_file() and '__pycache__' not in p.parts}
manifest.write_text(json.dumps({'format':1,'frozen':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'files':files,'apply_map':mapping.relative_to(ROOT).as_posix(),'apply_map_sha256':sha(mapping),'external_reference':{'path':'../'+abi['C_sharp_reference']['path'],'sha256':sha(reference)},'private_cases':3062,'private_comparisons':6124,'native_instruction_coverage':[367,367],'conditional_outcomes':[78,78],'limitations':proof['limitations']},indent=2)+'\n')
print(json.dumps({'frozen_files':len(files),'manifest_sha256':sha(manifest),'apply_map_sha256':sha(mapping)}))
