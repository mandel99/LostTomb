"""Record the C# record orientation and preserve historical GUI source snapshots."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
refs=['../PharaohRewrite/src/Pharaoh.Engine/Walkers/Walker.cs','../PharaohRewrite/src/Pharaoh.Engine/Walkers/Walkers.cs']
assert 'public const int RecordSize = 388;' in (ROOT/refs[1]).read_text()
out={'format':1,'C_sharp_used_as_exact_behavior':False,'references':[{'path':p,'sha256':sha(p)} for p in refs],
 'use':'C# Walker/Walkers identifies a388-byte legacy actor record and separates high-level direction, formation and action properties. It is orientation, not the native40BA40 command algorithm or its serialized field contract.',
 'native_authority':'Authenticated40BA40(215instructions) and4B5520(23), exact signedWORD coordinate/head/link/tag fields, BYTE state/direction flags and DWORD selected actor/cursor handles. Native pointer arithmetic wraps at32bits; no C# actor count clamp or high-level WalkerState interpretation is substituted.',
 'remaining':'Three real movement/action functions40C180,40BDA0,4B6400 remain controlled in this proof and are being reconstructed separately. Full actor-order gameplay and the remaining4D8310 dispatcher are not proved.',
 'historical_gui_snapshots_preserved':[{'path':p,'sha256':sha(p),'qualification':'Historical GUI/process observation with its original source hashes. Not refreshed to pretend it exercised the current source.'} for p in ['metadata/debug-session.json','metadata/startup-launch-repair.json'] if (ROOT/p).exists()]}
(ROOT/'metadata/menu-window-cursor-consumer-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'references':len(refs),'C_sharp_used_as_exact_behavior':False,'historical_GUI_records_preserved':len(out['historical_gui_snapshots_preserved'])}))
