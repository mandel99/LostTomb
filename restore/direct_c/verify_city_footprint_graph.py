"""Reuse the frozen oracle with observations valid under production inlining."""
from pathlib import Path
import hashlib
import sys

ROOT = Path(__file__).resolve().parents[1]
frozen = ROOT / 'artifacts/city_footprint_validation_phase/test.py'
assert hashlib.sha256(frozen.read_bytes()).hexdigest() == '4a2aef4e72490570c7ec0a2eaacd5a543e0c41ccbb11f24c44707865aab0383e'
inputs = [Path(__file__), frozen, Path(sys.argv[1]),
          ROOT / 'runtime/city_footprint_validation.c', ROOT / 'include/city_footprint_validation.h']
before = [hashlib.sha256(p.read_bytes()).digest() for p in inputs]
source = frozen.read_text()
needle = "answers.append((value,state['writes'],state['calls'],[bytes(u.mem_read(a,n)) for a,n in regions]))"
assert source.count(needle) == 1
# The pure 68-instruction geometry helper may inline into its one parent. Its
# direct-entry cases still check all arguments, return pointer and 13 outputs.
# Parent comparisons retain every write, all watched state and actual bounds
# calls; only the internal geometry CALL/snapshot observation is omitted.
source = source.replace(needle, "calls=state['calls']\n  if fn==0x4def00:calls=[item for item in calls if item[0] not in [0x4ded50,'output13']]\n  answers.append((value,state['writes'],calls,[bytes(u.mem_read(a,n)) for a,n in regions]))")
needle = 'if compiled:u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))'
assert source.count(needle) == 1
source = source.replace(needle, 'if compiled:u.mem_write(0x401000,bytes(0x16e000));u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))')
exec(compile(source, str(frozen), 'exec'), {'__file__': str(frozen), '__name__': '__main__'})
assert before == [hashlib.sha256(p.read_bytes()).digest() for p in inputs]
print('Production inlining observation: internal geometry CALL/output snapshot omitted only in its parent; direct helper outputs, all writes/state/bounds calls remain exact. C VM original text zeroed; input guards PASS.')
