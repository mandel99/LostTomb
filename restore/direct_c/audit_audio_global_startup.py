"""Source/link ownership audit of the restored audio CRT subset, not a launch."""
from pathlib import Path
import hashlib,json,re,struct
ROOT=Path(__file__).resolve().parents[1]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def clean(p):return re.sub(r'/\*.*?\*/|//[^\n]*','',(ROOT/p).read_text(),flags=re.S)
catalogue=json.loads((ROOT/'metadata/sound-device-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-device-rea-evidence.json').read_text())['records']}
native=ROOT/'build/smoke-game/Pharaoh.exe';assert digest(native)==catalogue['native_sha256']
table=catalogue['crt_table'];ev=evidence[table['evidence_id']]
assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==table['bytes_hex']
entries=list(struct.unpack('<29I',bytes.fromhex(table['bytes_hex'])))
assert entries[21:25]==[0x4bd300,0x4cdf70,0x4cdfb0,0x4d1340]
functions={int(f['address'],16):f for f in catalogue['functions']}
assert len(functions)==13 and sum(f['instruction_count'] for f in functions.values())==474
for a in [0x4cdf70,0x4cdfa0,0x4cdfb0,0x4cdfe0]:
 f=functions[a];assert f['provider_body_available'] is False
 negative=evidence[f['not_in_procedure_evidence_id']]['normalized_result']
 assert negative['found'] is False and negative['reason']=='not_in_procedure'
main=clean('runtime/standalone_link_check.c')
ordered=['atexit(restore_data_close)','FUN_004bd310();','if(probe){restore_sound_initialize_owned();restore_music_initialize_owned();}','FUN_004d1340();','FUN_00413ed0(GetModuleHandleA(NULL))']
positions=[main.index(s) for s in ordered];assert positions==sorted(positions)
boot=clean('runtime/sound_boot.c');assert 'initialize_owned' not in boot and 'construct' not in boot
body=clean('runtime/audio_global_lifecycle.c')
for text in ['restore_sound_global_initialize(void){FUN_004cdf80();FUN_004cdf90();}',
             'restore_music_global_initialize(void){FUN_004cdfc0();FUN_004cdfd0();}',
             'restore_sound_global_register_cleanup(void){(void)atexit(FUN_004cdfa0);}',
             'restore_music_global_register_cleanup(void){(void)atexit(FUN_004cdfe0);}',
             'restore_sound_global_cleanup(void){FUN_0055e58a(RESTORE_SOUND);}',
             'restore_music_global_cleanup(void){FUN_0051f780();}']:
 assert text in body,text
for p,entry in [('runtime/sound_initialization.c','FUN_004cdf70()'),('runtime/music_manager.c','FUN_004cdfb0()')]:
 text=clean(p);assert f'if(!initialized) {{{entry};initialized=1;}}' in text
sources={'CMakeLists.txt','runtime/standalone_link_check.c','runtime/audio_global_lifecycle.c','runtime/sound_initialization.c','runtime/music_manager.c','runtime/music_constructor.c','runtime/sound_constructor.c','runtime/sound_device.c','runtime/sound_shutdown.c','runtime/sound_boot.c','include/audio_global_lifecycle.h','include/sound_device.h','include/music_manager.h','direct_c/audit_audio_global_startup.py','metadata/sound-device-native.json','metadata/sound-device-rea-evidence.json'}
# Bind the sources of actually reached existing child providers as well as the
# new parents. Their independent comparisons remain separate aggregate inputs.
for family in ['sound-allocation','sound-file-lifecycle','sound-bank','sound-sample','sound-playback','sound-stop','byte-string-compare']:
 report=json.loads((ROOT/f'metadata/{family}-game.json').read_text())
 sources.update(p for p in report['source_hashes'] if p.startswith(('runtime/','include/','src/')))
sources.update(f'src/unit_{n:05}.c' for n in [2473,2474,2475,2477,2479,2480,2689,2692,2693,1735,1736,1737,1738,2351,2849])
sources.update(['runtime/music_configuration.c','runtime/music_tracks.c','runtime/music_population_range.c','runtime/byte_tokenizer.c','include/music_configuration.h','include/byte_tokenizer.h','src/unit_02476.c','src/unit_02815.c'])
variants={}
for name in ['restore_link_check','PharaohRestoreC']:
 binary=ROOT/f'build/c-only/Release/{name}.exe';mapping=binary.with_suffix('.map').read_text()
 for a in functions:assert re.search(r'\b_FUN_%08x\s'%a,mapping),hex(a)
 variants[name]=dict(compiled_C_path=str(binary.resolve()),compiled_C_sha256=digest(binary))
report=dict(passed=True,native_sha256=catalogue['native_sha256'],owned_entries=13,owned_native_instructions=474,provider_absent_raw_entries=[hex(a) for a in [0x4cdf70,0x4cdfa0,0x4cdfb0,0x4cdfe0]],original_CRT_entries=[hex(a) for a in entries],restored_audio_order=['arena cleanup registration','preceding restored4BD310 constructor','sound CRT initializer4CDF70','music CRT initializer4CDFB0','next restored4D1340 initializer','application entry413ED0'],registered_cleanup_order=['arena','sound','music'],normal_exit_cleanup_order=['music','sound','arena'],variants=variants,source_hashes={p:digest(ROOT/p) for p in sorted(sources)},limitations=['Static source/link audit of the restored CRT subset; no host game process is launched and the complete29-entry native startup table is not reconstructed here.','Actual sound/music constructor-registration and reverse callback invocation are compared in the paired device oracle with controlled atexit. Host CRT allocator/registration internals and native OS exception dispatch are outside that proof.','The once-owned wrappers are C-only startup adapters. The native boot algorithm contains no constructor calls. Four raw entries have explicit not-in-procedure evidence and are not new Ghidra procedures or additions to the catalogue denominator.'])
(ROOT/'metadata/audio-global-startup-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS audio CRT subset ownership/order: 13 linked entries, 474 native instructions; music/sound cleanup precedes arena release; no game launched')
