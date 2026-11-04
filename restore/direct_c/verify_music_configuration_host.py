"""Bind fresh compiled music/campaign/tick fixtures and six native reports."""
from verify_labor_access_current import ROOT,digest,source_matches
import json,re,subprocess
inputs={};sources={'direct_c/verify_music_configuration_host.py','direct_c/verify_labor_access_current.py','metadata/music-cmake-scope-audit.json'}
variants={}
for config in ['Debug','Release']:
 suffix='-release' if config=='Release' else '';variant={}
 for family,target,expected_native,expected_checks,expected_scenarios in [('music-manager','restore_music_verify',316,294,16),('campaign-parser','restore_campaign_parser_verify',333,315,12),('music-tick','restore_music_tick_verify',1668,None,None)]:
  path=f'metadata/{family}-emulation{suffix}.json';report=json.loads((ROOT/path).read_text());binary=ROOT/f'build/c-only/{config}/{target}.exe'
  assert report['passed'] and report['checked']==expected_native,(config,family)
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==report.get('original_sha256',report.get('native_sha256'))
  assert digest(binary)==report['compiled_C_sha256'] and str(binary.resolve())==report['compiled_C_path']
  for p,sha in report['source_hashes'].items():assert source_matches(p,sha,allow_scope=family!='music-tick'),(config,family,p)
  result=subprocess.run([str(binary)],capture_output=True,text=True,timeout=30)
  assert result.returncode==0,(config,family,result.returncode,result.stdout,result.stderr)
  if expected_checks is not None:
   match=re.search(r': (\d+) checks across (\d+) scenarios;',result.stdout)
   assert match and list(map(int,match.groups()))==[expected_checks,expected_scenarios],result.stdout
  else:assert 'Music tick: native state machine, real selection/volume/RNG and owned clock boundary.' in result.stdout
  variant[family]=dict(compiled_C_path=str(binary.resolve()),compiled_C_sha256=digest(binary),exit_code=0,checks=expected_checks,scenarios=expected_scenarios,native_cases=expected_native)
  inputs[path]=digest(ROOT/path);sources.update(report['source_hashes']);sources.add(path)
 variants[config]=variant
proof=dict(passed=True,variants=variants,native_input_reports=inputs,source_hashes={p:digest(ROOT/p) for p in sorted(sources)},user_CD_policy='Original fallback retained as commented source; compiled parser uses local files only.',limitations=['Fresh source-only test EXEs execute with controlled files/Miles/clock boundaries and original data backing. No game or audio device is launched.','Native reports retain their original source hashes. Exact one-target CMake scope evidence bridges only unrelated music-tick target edits; music-tick reports themselves require strict current hashes and fresh native comparisons.','The1668-case music-tick corpus uses actual selection, volume and RNG, with stream playback/time/backend endpoints controlled. It does not certify its full real-stream composition, arbitrary malformed receivers or physical audio.'])
(ROOT/'metadata/music-configuration-host.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS Debug/Release music294/16, campaign315/12, music-tick fixtures; native316/333/1668 cases each; CD fallback disabled; no game launched')
