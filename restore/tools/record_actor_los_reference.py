"""Record inspected C# orientation without importing its different roam controller."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
refs=['../PharaohRewrite/src/Pharaoh.Engine/Walkers/Walkers.cs','../PharaohRewrite/src/Pharaoh.Engine/Walkers/Walkers.Roaming.cs']
source=(ROOT/refs[1]).read_text();assert 'Math.Sign' in source and '* 228' in source and 'Math.Clamp' in source
out={'format':1,'C_sharp_used_as_exact_behavior':False,'references':[{'path':p,'sha256':sha(p)} for p in refs],
 'use':'Walkers and Roaming orient the fixed228-cell row stride, heading/direction concepts and nearby-road flags. Their service-roam/turn controllers are different algorithms; their world clamps and high-level direction choices are not transplanted into40BDA0/40C180.',
 'native_authority':'Complete538 original LOS instructions, six byte-pinned edge qualifications, actual187-instruction flood/22-instruction fill and15 FTOL instructions. The original retained extended dy/dx followed by rounded reciprocal1/slope is preserved, with no binary32 slope spill or guessed direct dx/dy substitution.',
 'remaining':'4B6400 group action/path graph, other actor callers, physical UI/gameplay and the complete menu input controller remain separately unproved.'}
(ROOT/'metadata/actor-order-los-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'references':2,'C_sharp_used_as_exact_behavior':False}))
