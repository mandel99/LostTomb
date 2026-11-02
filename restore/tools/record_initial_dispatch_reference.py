"""Keep the C# reference comparison separate from original-byte authority."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
source=ROOT.parent/'PharaohRewrite/src/Pharaoh.Engine/UI/LegacyMenuFlow.cs'
text=source.read_text();assert '[OriginalAddress(0x004D71C0, "FUN_004d71c0")]' in text and 'public static LegacyMenuState? Resolve' in text
native=json.loads((ROOT/'metadata/initial-menu-dispatch-emulation.json').read_text())
assert native['passed'] and native['production'] and native['switch_mode_coverage']==[88,88]
out={'format':1,'reference_source':{'path':'../PharaohRewrite/src/Pharaoh.Engine/UI/LegacyMenuFlow.cs','sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'relevant_line':108},
 'reference_role':'Frontend transition names and related callbacks are useful orientation. Resolve combines several OriginalAddress attributes into a high-level transition router.',
 'native_authority':'Authenticated4D71C0 is an initial draw dispatcher with dirty/force handling, full-DWORD alternate flag/mode, zero-extended UI BYTE and88-entry two-level switch. It does not implement the C# Resolve event transition algorithm.',
 'implementation_decision':'Preserve the complete original dispatcher semantics; do not copy the higher-level C# transition router as its body.',
 'native_proof':{'path':'metadata/initial-menu-dispatch-emulation.json','sha256':hashlib.sha256((ROOT/'metadata/initial-menu-dispatch-emulation.json').read_bytes()).hexdigest()},'C_sharp_used_as_exact_behavior':False}
(ROOT/'metadata/initial-dispatch-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'reference_reviewed':True,'native_authority_preserved':True}))
