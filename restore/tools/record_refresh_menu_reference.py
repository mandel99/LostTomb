"""Record inspected C# orientation without substituting it for native behavior."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
paths=['../PharaohRewrite/src/Pharaoh.Engine/UI/LegacyMenuFlow.cs','../PharaohRewrite/src/Pharaoh.Engine/Buildings/LegacyBuildingOwnership.cs']
records=[{'path':p,'sha256':hashlib.sha256((ROOT/p).read_bytes()).hexdigest()} for p in paths]
flow=(ROOT/paths[0]).read_text();ownership=(ROOT/paths[1]).read_text()
assert 'ModalDialog = 0x54' in flow and 'Gameplay = 0x01' in flow and 'RestorePrevious()' in flow
assert '18904' in ownership and 'EAAC28' in ownership and 'if ((uint)selected < 2)' in ownership
out={'format':1,'references':records,'C_sharp_used_as_exact_behavior':False,
 'use':'LegacyMenuFlow provides orientation for named front-end BYTE states, including gameplay01, mission3D and modal54. It is a coarse transition router, not the whole per-frame native4D75D0 controller. LegacyBuildingOwnership identifies the serialized active-city EAAC28 selector and18904/49D8 stride.',
 'native_authority':'Pinned original PE, authenticated917 controller instructions plus42 actual getter instructions and bareRET, exact ordered call arguments, full watched RAM hashes and native ABI guards define this implementation. C# convenience transitions, RestorePrevious semantics and two-city index clamp are not transplanted into4D75D0.',
 'repairs':'Forty-four scalar widths and three indexed addressing expressions were reviewed against native operands; mode is sampled once at entry. Selected city remains fullDWORD, actor type signedWORD at264 stride, record flag BYTE at98 stride and resource policy WORD at49D8 city stride. Coordinate additions/multiplications preserve x86 wrapping.',
 'limitations':'Ninety-six deeper functions remain typed controlled endpoints in this controller proof. This record and the C# source do not prove pixel output, live OS behavior, complete menu fidelity or gameplay.'}
(ROOT/'metadata/refresh-menu-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'references':len(records),'C_sharp_used_as_exact_behavior':False}))
