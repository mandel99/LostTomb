"""Production routing around the immutable 3090-case remaining-menu oracle."""
from pathlib import Path
import argparse,hashlib,json
ROOT=Path(__file__).resolve().parent.parent
STAGE=ROOT/'artifacts/remaining_menu_actions_phase'
FROZEN=STAGE/'verify_remaining_menu_actions.py'
MANIFEST=STAGE/'remaining_menu_actions_apply_map.json'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(FROZEN)=='3dca377d7560ef5ac880081ff4c24f87967f0ee15dcfe97c907b3647b9ffa6ba','immutable oracle drift'
assert sha(MANIFEST)=='535475324dbfd9e6cf2d01505fd92bc0db081d90f6d241469fdc169edd1a15aa','accepted manifest drift'
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('--production',action='store_true');ap.add_argument('--limit',type=int)
ap.add_argument('--exe',type=Path);ap.add_argument('--plain-exe',type=Path)
ap.add_argument('--optimized-exe',type=Path);ap.add_argument('--optimized-plain-exe',type=Path)
args=ap.parse_args()
manifest=json.loads(MANIFEST.read_text())
for name,want in manifest['proof_hashes'].items():assert sha(ROOT/name)==want,name
for mapping in manifest['mappings']:
    assert sha(ROOT/mapping['staged'])==mapping['staged_sha256'],mapping['staged']
    if args.production:assert sha(ROOT/mapping['target'])==mapping['staged_sha256'],mapping['target']
if args.production:assert args.exe and args.plain_exe and args.optimized_exe and args.optimized_plain_exe,'all four actual shared binaries required'
executables=[args.exe or STAGE/'build/Debug/remaining_menu_trace.exe',args.plain_exe or STAGE/'build/Debug/remaining_menu_plain.exe',args.optimized_exe or STAGE/'build/Release/remaining_menu_trace.exe',args.optimized_plain_exe or STAGE/'build/Release/remaining_menu_plain.exe']
executables=[p.resolve() for p in executables]
if args.production:
    assert [p.name.lower() for p in executables]==['restore_remaining_menu_actions_verify.exe','restore_remaining_menu_actions_plain_verify.exe']*2
    assert executables[0]!=executables[2] and executables[1]!=executables[3]
    assert all('remaining_menu_actions_phase/build' not in p.as_posix() for p in executables),'private executables are not production proof'
sources=[ROOT/m['target' if args.production else 'staged'] for m in manifest['mappings']]
sources += [STAGE/'remaining_menu_fixture.c',STAGE/'remaining_menu_native.json',STAGE/'shared_targets.cmake',FROZEN,MANIFEST,Path(__file__),ROOT/'src/unit_01628.c',ROOT/'include/message_panel_input.h',ROOT/'include/city_prepare.h']
before={str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in sources}
output=ROOT/'metadata' if args.production else ROOT/'artifacts/remaining_menu_click_phase'
output.mkdir(parents=True,exist_ok=True)
destination=output/('remaining-menu-actions-emulation-partial.json' if args.limit else 'remaining-menu-actions-emulation.json')
text=FROZEN.read_text()
patches=[
 ('HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent',"ROOT=WRAPPER_ROOT;HERE=WRAPPER_STAGE"),
 ("parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--limit',type=int);args=parser.parse_args()",'args=WRAPPER_ARGS'),
 ("executables=[HERE/'build'/cfg/f'remaining_menu_{kind}.exe' for cfg in ['Debug','Release'] for kind in ['trace','plain']]",'executables=WRAPPER_EXECUTABLES'),
 ("files=[HERE/n for n in ['unit_00049.c','unit_00077.c','remaining_menu_route.c','remaining_menu_actions.h','remaining_menu_fixture.c','remaining_menu_native.json','CMakeLists.txt']]+[Path(__file__),ROOT/'src/unit_01628.c']",'files=WRAPPER_SOURCES'),
 ("out=HERE/('remaining-menu-actions-emulation-partial.json' if args.limit else 'remaining-menu-actions-emulation.json');out.write_text(json.dumps(report,indent=2)+'\\n')", """report.update({'production':WRAPPER_ARGS.production,'product_applied':WRAPPER_ARGS.production,'compiled_C_path':relative(executables[0]),'frozen_oracle_sha256':WRAPPER_ORACLE_HASH,'frozen_manifest_sha256':WRAPPER_MANIFEST_HASH,'source_hashes_before_run':WRAPPER_BEFORE,'source_drift':[relative(p) for p in files if sha(p.read_bytes())!=WRAPPER_BEFORE[relative(p)]]})
assert not report['source_drift'],'source changed during proof'
if not args.limit:assert report['checked']==3090 and report['native_algorithm_instructions']==70
out=WRAPPER_DESTINATION;out.write_text(json.dumps(report,indent=2)+'\\n')""")
]
for old,new in patches:
    assert text.count(old)==1,'immutable routing patch no longer matches: '+old[:90]
    text=text.replace(old,new,1)
scope={'__name__':'__main__','__file__':str(Path(__file__).resolve()),'WRAPPER_ROOT':ROOT,'WRAPPER_STAGE':STAGE,'WRAPPER_ARGS':args,'WRAPPER_EXECUTABLES':executables,'WRAPPER_SOURCES':sources,'WRAPPER_DESTINATION':destination,'WRAPPER_ORACLE_HASH':sha(FROZEN),'WRAPPER_MANIFEST_HASH':sha(MANIFEST),'WRAPPER_BEFORE':before}
exec(compile(text,str(FROZEN),'exec'),scope)
