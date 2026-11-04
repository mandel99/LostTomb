"""Native-derived partial-allocation expectations in the compiled C fixture."""
from pathlib import Path
import hashlib,json,re,subprocess
ROOT=Path(__file__).resolve().parents[1]
variants={};sources={}
for config in ['Debug','Release']:
 path=ROOT/f'build/c-only/{config}/restore_sound_verify.exe'
 rel='metadata/sound-initialization-emulation'+('-release' if config=='Release' else '')+'.json'
 native=json.loads((ROOT/rel).read_text())
 assert native['passed'] and native['checked']==391
 assert hashlib.sha256(path.read_bytes()).hexdigest()==native['compiled_C_sha256']
 for source,sha in native['source_hashes'].items():assert hashlib.sha256((ROOT/source).read_bytes()).hexdigest()==sha,(config,source)
 sources.update(native['source_hashes'])
 result=subprocess.run([str(path)],capture_output=True,text=True,timeout=30)
 match=re.search(r'Sound initialization: (\d+) checks across (\d+) scenarios;',result.stdout)
 assert result.returncode==0 and match and tuple(map(int,match.groups()))==(265,18),(config,result.returncode,result.stdout,result.stderr)
 variants[config]=dict(compiled_C_path=str(path.resolve()),compiled_C_sha256=native['compiled_C_sha256'],checks=265,scenarios=18,exit_code=0)
sources['direct_c/verify_sound_allocation_host.py']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
report=dict(passed=True,variants=variants,source_hashes=sources,expectations_derived_from_native=True,limitations=['Compiled fixture uses controlled heap/device endpoints and validates retained partial allocations and successful partial handle allocation. Physical audio and allocator internals are outside the fixture.','391 original/C initialization cases are bound independently in both configurations; huge allocation requests are verified by the separate paired Unicorn oracle.'])
(ROOT/'metadata/sound-allocation-host.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS Debug/Release native-derived initialization ownership: 265 assertions /18 scenarios each; 391 native/C cases each')
