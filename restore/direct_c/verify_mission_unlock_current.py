"""Replay the frozen unlock oracle on canonical sources; stdout only."""
from pathlib import Path
import hashlib
import sys

root=Path(__file__).resolve().parents[1]
phase=root/'artifacts/mission_unlock_phase'
test=phase/'test.py'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(test)=='68771c9063d7f4ec352b1dc35edc8f894abbe90a0f95fe56dbda7b42fbc5c951'
inputs={root/'runtime/mission_unlock.c':phase/'mission_unlock.c',
        root/'runtime/tool_availability.c':phase/'tool_availability.c',
        root/'include/mission_unlock.h':phase/'mission_unlock.h',
        root/'include/tool_availability.h':phase/'tool_availability.h'}
for current,frozen in inputs.items():
    assert sha(current)==sha(frozen),(current,frozen)
before={p:sha(p) for p in inputs}
dll=Path(sys.argv[1]).resolve()
checkpoint=sys.argv[2] if len(sys.argv)>2 else '100'
sys.argv=[str(test),str(dll),'--main-checkpoint',checkpoint]
ns={'__file__':str(test),'__name__':'__main__'}
exec(compile(test.read_text(),str(test),'exec'),ns)
assert ns['checked']==107
# Three real signed-WORD city modifiers and guards, with genuine providers.
ns['regions'].append((0xea61e4,0x26))
for row,kind in [(60,0),(0,1)]:
    ns['run'](0x404350,row=row,type=kind,resources=True,stock=1,gate=0x100,
              culture=6,unlocked=1,special=1,scene=5)
assert ns['checked']==109
for p,h in ns['hashes'].items():assert sha(p)==h,p
for p,h in before.items():assert sha(p)==h,p
print('PASS core107+extended2=109 C/native cases;404350 executed',
      len(ns['functions'][0x404350]&ns['hits']),'/892;frozen oracle unchanged')
print('Normal-entry main caller requires605A3C=0 by native CFG proof. '
      'Canonical provider optimization follows project options;'
      'general editor, pixels and audio drivers remain outside this replay.')
