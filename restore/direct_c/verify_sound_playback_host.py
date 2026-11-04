"""Existing source-only playback contracts in Debug and Release; no device IO."""
from pathlib import Path
import hashlib,json,re,subprocess
ROOT=Path(__file__).resolve().parents[1]
sources=['CMakeLists.txt','tests/sound_samples.c','runtime/sound_playback.c','runtime/sound_sample_reset.c','include/sound_playback.h','include/sound_initialization.h','include/media_bindings.h','direct_c/verify_sound_playback_host.py']+[f'src/unit_{n:05}.c' for n in [2691,2698,2699,2701,2702,2703]]
variants={}
for config in ['Debug','Release']:
 binary=ROOT/f'build/c-only/{config}/restore_sound_samples_verify.exe'
 result=subprocess.run([str(binary)],capture_output=True,text=True,timeout=30)
 match=re.search(r'Sound samples: (\d+) checks;',result.stdout)
 assert result.returncode==0 and match and int(match[1])==160,(config,result.returncode,result.stdout,result.stderr)
 variants[config]=dict(compiled_C_path=str(binary.resolve()),compiled_C_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),checks=160,exit_code=0)
report=dict(passed=True,variants=variants,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Existing actual compiled C fixture uses test-only Miles endpoints, covers 160 assertions per configuration, and does not establish physical audio or native backend behavior. Native/C owned-body equivalence is checked separately in the paired playback oracle.'])
(ROOT/'metadata/sound-playback-host.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS Debug/Release source-only sample playback: 160 assertions each, test-only Miles endpoints')
