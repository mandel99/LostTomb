"""Additional whole-function queue rollover cases; original frozen oracle unchanged."""
from pathlib import Path
import hashlib
ROOT=Path(__file__).resolve().parents[1]
frozen=ROOT/'artifacts/menu_route_seed_phase/test_seed.py'
assert hashlib.sha256(frozen.read_bytes()).hexdigest()=='3059046185a26fdf8feb9fdd55080063f8f3d1c9baab789b92d01aa5489b661c'
source=frozen.read_text();parts=source.split('\nfocused()\n');assert len(parts)==2
marker='        u.mem_write(STACK,bytes(0x10000));'
assert parts[0].count(marker)==1
fixture='''        if cfg.get('one_way'):
            seed=cfg['one_way']
            state=seed+1;terrain=bytearray(COUNT)
            for n in range(COUNT):
                state=(state*1664525+1013904223)&0xffffffff
                terrain[n]=250 if state>>30==0 else 252
            for row in range(227):terrain[row*228+(row*17+seed*13)%228]=250
            terrain[COUNT-228:]=bytes([252])*228
            u.mem_write(0xee0c50,bytes(terrain));u.mem_write(0xaf598c,dw(8)*COUNT)
'''
prefix=parts[0].replace(marker,fixture+marker)
namespace={'__file__':str(frozen),'__name__':'__main__'}
before=hashlib.sha256(frozen.read_bytes()).hexdigest()
exec(compile(prefix,str(frozen),'exec'),namespace)
for function,cfg in [(0x51c790,dict(full=True,origin=16756)),
                     (0x51b680,dict(full=True,origin=16756)),
                     (0x51b680,dict(full=True,origin=51759,one_way=3,mode=1))]:
    namespace['run'](function,cfg)
    print('PASS whole-map capacity case',hex(function),cfg,flush=True)
open_edges={(0x51ba03,0x51ba05),(0x51bb0e,0x51bb10),(0x51c838,0x51c83a),(0x51c875,0x51c877),(0x51c8b5,0x51c8b7)}
covered=open_edges&namespace['edges']
assert before==hashlib.sha256(frozen.read_bytes()).hexdigest()
print('Newly covered capacity outcomes:',[(hex(a),hex(b)) for a,b in sorted(covered)])
print('Remaining capacity outcomes:',[(hex(a),hex(b)) for a,b in sorted(open_edges-covered)])
assert covered=={(0x51ba03,0x51ba05),(0x51bb0e,0x51bb10),(0x51c8b5,0x51c8b7)}
