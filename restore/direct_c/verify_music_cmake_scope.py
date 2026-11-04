"""Prove that two late CMake edits affect only the standalone music-tick test.

Retain original report hashes. Accept no production-source or production-target
delta, and require an incremental build to leave both tested EXEs/maps identical.
"""
from pathlib import Path
import hashlib,json,subprocess
ROOT=Path(__file__).resolve().parents[1]
def sha(data):return hashlib.sha256(data).hexdigest()
current=(ROOT/'CMakeLists.txt').read_bytes()
assert sha(current)=='05e915d77aab28ee045f8b6cdf5218c28a30543c55d165d8c4c575ca7d4ea099'
old=current.replace(b'src/unit_02480.c tests/music_tick_streams.c src/unit_02825.c',b'src/unit_02480.c src/unit_02705.c src/unit_02825.c')
old=b''.join(line for line in old.splitlines(keepends=True) if line.strip() not in [b'target_compile_options(restore_music_tick_verify PRIVATE /Gy)',b'target_link_options(restore_music_tick_verify PRIVATE /OPT:REF /INCREMENTAL:NO)'])
earlier=b''.join(line for line in old.splitlines(keepends=True) if line.strip()!=b'target_sources(restore_music_tick_verify PRIVATE runtime/music_population_range.c)')
assert sha(old)=='d5e41540a770b75fbce792825a35cdd5a97078e5d3148c00dc055a9e9ae96b50'
assert sha(earlier)=='cb064b7bd42b49a6ab8fff87671c58bcc39d0d47b54a3224622555b3b76512f2'
# Exact whole-file reconstruction above permits only one source-list replacement
# and three literal target-local commands. No source properties/variables change.
assert b'function(target_' not in current and b'macro(target_' not in current
snapshots={}
directory=ROOT/'build/music-configuration-cmake-epochs';directory.mkdir(exist_ok=True)
for data in [earlier,old,current]:
 p=directory/f'CMakeLists-{sha(data)}.txt';p.write_bytes(data);snapshots[str(p.relative_to(ROOT)).replace('\\','/')]=sha(data)
startup_path=ROOT/'metadata/audio-global-startup-audit.json';startup_bytes=startup_path.read_bytes()
startup=json.loads(startup_bytes);compatible_startup=[]
for data in [earlier,old]:
 historical=json.loads(startup_bytes);historical['source_hashes']['CMakeLists.txt']=sha(data)
 raw=(json.dumps(historical,indent=2)+'\n').replace('\n','\r\n').encode()
 p=directory/f'audio-global-startup-{sha(raw)}.json';p.write_bytes(raw)
 snapshots[str(p.relative_to(ROOT)).replace('\\','/')]=sha(raw);compatible_startup.append(sha(raw))
for suffix in ['diagnostic','game']:
 device=json.loads((ROOT/f'metadata/sound-device-{suffix}.json').read_text())
 assert device['source_hashes']['metadata/audio-global-startup-audit.json'] in [sha(startup_bytes),*compatible_startup]
before={}
for name,suffix in [('restore_link_check','diagnostic'),('PharaohRestoreC','game')]:
 report=json.loads((ROOT/f'metadata/music-configuration-{suffix}.json').read_text())
 binary=ROOT/f'build/c-only/Release/{name}.exe';mapping=binary.with_suffix('.map')
 assert sha(binary.read_bytes())==report['compiled_C_sha256']
 assert report['source_hashes']['CMakeLists.txt'] in [sha(earlier),sha(old),sha(current)]
 before[name]=dict(compiled_C_path=str(binary.resolve()),binary_sha256=sha(binary.read_bytes()),map_sha256=sha(mapping.read_bytes()))
command=['C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe','--build',str(ROOT/'build/c-only'),'--config','Release','--target','restore_link_check','restore_game','--parallel','8']
log=ROOT/'build/build-music-production-unchanged.log'
with log.open('w') as output:result=subprocess.run(command,stdout=output,stderr=subprocess.STDOUT)
assert result.returncode==0,(result.returncode,str(log))
for name,item in before.items():
 binary=Path(item['compiled_C_path']);assert sha(binary.read_bytes())==item['binary_sha256'],name
 assert sha(binary.with_suffix('.map').read_bytes())==item['map_sha256'],name
paths=['direct_c/verify_music_cmake_scope.py','CMakeLists.txt','tests/music_tick_streams.c','build/build-music-production-unchanged.log',*snapshots]
paths+=['metadata/audio-global-startup-audit.json']
report=dict(passed=True,sole_changed_target='restore_music_tick_verify',prior_CMake_sha256=[sha(earlier),sha(old)],current_CMake_sha256=sha(current),compatible_startup_audit=dict(current_sha256=sha(startup_bytes),prior_sha256=compatible_startup,only_changed_field='source_hashes.CMakeLists.txt'),unchanged_production_artifacts=before,incremental_build_exit=0,command=command,source_hashes={p:sha((ROOT/p).read_bytes()) for p in paths},limitations=['Exact one-time CMake scope audit, not a replacement for behavioral tests. Original report CMake hashes remain unchanged and their full historical files are retained.','Only the standalone music-tick source list and target-local compile/link options differ. All production and other fixture target definitions, source properties, variables and algorithm files are unchanged between these CMake epochs. The music-tick artifact itself is excluded from this compatibility exception and must pass fresh tests.','The corresponding historical startup-audit records differ only at source_hashes.CMakeLists.txt. Their reconstructed full bytes must match the exact hash pinned by the original device reports; those device reports are not edited.','An actual current-config incremental build must preserve both tested Release EXEs and their maps byte for byte. Any other CMake edit, algorithm change, stale non-CMake source, binary difference or unlisted historical hash fails closed.'])
(ROOT/'metadata/music-cmake-scope-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS exact CMake scope: standalone music-tick target only; both current production EXEs/maps unchanged; original test-report hashes retained')
