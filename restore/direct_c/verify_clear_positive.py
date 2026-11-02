"""Positive-city composition of whole47C480; frozen oracle stays read-only. No finite endpoints or output files."""
from pathlib import Path
import itertools,sys
ROOT=Path(__file__).resolve().parents[1]
source=ROOT/'artifacts/menu_drag_clear_footprint_phase/test_clear.py'
prefix=source.read_text().split('\nfor building,typ,match,flags',1)[0]
prefix=prefix.replace('(0xbca2e8,65536)',"(0xbca2e8,cfg.get('world_x',0))")
prefix=prefix.replace('REGIONS=[','REGIONS=[(0x5ca658,48*16),(0x5cbfb8,26*16),(0x78d79c,0x30),(0xf56b40,0x810),')
prefix=prefix.replace('if a==0x5cc8c0 else bytes(n)','if a in {0x5cc8c0,0x5ca658,0x5cbfb8} else bytes(n)')
prefix=prefix.replace('(0xacf85c,255)',"(0xacf85c,cfg.get('animation',255))")
prefix=prefix.replace("    if cfg.get('shift'):","""    put(u,0xbca2e0,cfg.get('orientation',0));put(u,0x78d7b0,0)
    for base in [0xf56bd4,0xf56c34,0xf56c9c,0xf56d24,0xf56b64,0xf5721c,0xf57224,0xf5722c,0xf57234,0xf57264,0xf57324]:put(u,base,1000)
    for j,delta in enumerate([-228,-227,1,229,228,227,-1,-229]):
        if cfg.get('neighbors',0)&(1<<j):put(u,0xaf598c+(cell+delta)*4,4)
    if cfg.get('shift'):""")
prefix=prefix.replace('        if a in main:', '        if a in postpass:postpass_hits.add(a)\n        if a in main:')
namespace={'__file__':str(source),'__name__':'clear_positive_oracle'}
exec(compile(prefix,str(source),'exec'),namespace)
namespace['postpass']={int(i['address'],16) for i in namespace['cache'][0x47a220]['assembly']['items']};namespace['postpass_hits']=set()
for orientation,neighbors,animation in itertools.product([0,2,4,6],[0,1,3,5,15,85,170,255],[0,255]):namespace['run'](0x47c480,dict(world_x=0,flags=0x604,orientation=orientation,neighbors=neighbors,animation=animation))
for size in [1,2,3,6]:namespace['run'](0x47c480,dict(world_x=0,type=213,size=size,flags=0x604,neighbors=255))
assert namespace['cases']==68 and len(namespace['postpass_hits'])>50
print(f"PASS {namespace['cases']} positive-city clear-parent cases; actual47A220 {len(namespace['postpass_hits'])}/599 instructions exercised; ordered owned reads + all watched graph writes/state/real args; aggregate={namespace['aggregate']}")
print('Whole599-instruction postpass coverage is the separate retained prior proof; this replay certifies the new positive parent composition only.')
