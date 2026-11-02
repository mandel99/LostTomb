"""Record selected-city layout reference without importing rewrite-only bounds."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sources=['PharaohRewrite/src/Pharaoh.Engine/Buildings/LegacyBuildingOwnership.cs','PharaohRewrite/analysis/ghidra/save-map-field-audit.md']
texts=[(ROOT.parent/p).read_text() for p in sources]
assert 'active-city global EAAC28' in texts[0] and 'ReadCityIndex' in texts[0] and '18904' in texts[0]
assert 'stride is 49D8' in texts[1] and 'active city index 00EAAC28' in texts[1]
out={'format':1,'references':[{'path':'../'+p,'sha256':hashlib.sha256((ROOT.parent/p).read_bytes()).hexdigest()} for p in sources],
 'use':'Selected-city record orientation and serialized stride49D8/18904. Frozen native proof uses the historical alias player for the same DWORD selector; numeric memory addressing is unchanged.',
 'authority':'Native1001-instruction panel code and original PE define actual arithmetic, reads, writes, labels and query order. The C# ReadCityIndex guard/default is RewriteOnly convenience and is not copied into native panel code.',
 'not_claimed':'Serialized two-record layouts and synthetic RAM width fixtures do not establish arbitrary active-city indices as ordinary UI states. No production bounds/fallback are invented.',
 'C_sharp_used_as_exact_behavior':False}
(ROOT/'metadata/training-panel-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'reference_reviewed':True,'rewrite_only_clamp_not_imported':True}))
