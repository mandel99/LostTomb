"""Replay the frozen map cases against the canonical message provider; stdout only."""
from pathlib import Path
import hashlib
ROOT=Path(__file__).resolve().parents[1]
frozen=ROOT/'artifacts/order_actions_finish_phase/verify_actions.py'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(frozen)=='c9e2dfeaa95f3e2c762b78b039a7bd20f6615e137a7c5108b92c62f3a13aa67b'
source=frozen.read_text()
old="('004e2970','unit_01927.obj')"
assert source.count(old)==1
source=source.replace(old,"('004e2970','pharaoh_restored:message_controller.obj')")
files=[ROOT/'runtime'/n for n in ['message_controller.c','message_strings.c','action_tail.c']]
files += [ROOT/'src'/f'unit_{n:05}.c' for n in [88,89,1458,1468,1469,1470]]
before={p:sha(p) for p in files}
exec(compile(source,str(frozen),'exec'),{'__file__':str(frozen),'__name__':'__main__'})
for p,h in before.items():assert sha(p)==h,str(p)
print('Canonical message/text provider from actual main library verified; original frozen oracle unchanged.')
