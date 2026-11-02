import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=f=>readFile(path.join(root,f),'utf8'),parse=async f=>JSON.parse((await read(f)).replace(/^\uFEFF/,''));
const hash=async f=>createHash('sha256').update(await readFile(path.join(root,f))).digest('hex');
const save=(f,v)=>writeFile(path.join(root,f),JSON.stringify(v,null,2)+'\n');
const debug=await parse('metadata/debug-session.json'),music=await parse('metadata/music-manager-emulation.json'),lookup=await parse('metadata/sound-file-lookup-emulation.json');
for(const [report,cases,exe] of [[music,324,'restore_music_verify.exe'],[lookup,2000,'restore_sound_files_verify.exe']]) {
 if(!report.passed||report.checked!==cases)throw new Error('Missing native verification: '+exe);
 for(const [f,h] of Object.entries(report.source_hashes))if(await hash(f)!==h)throw new Error('Stale native comparison: '+f);
 if(await hash('build/c-only-debug/Debug/'+exe)!==report.compiled_C_sha256)throw new Error('C oracle binary changed: '+exe);
}
for(const f of ['artifacts/music-debug-final-build.log','artifacts/music-release-final-build.log'])
 if(!(await read(f)).includes('100% tests passed, 0 tests failed out of 31'))throw new Error('Missing final tests: '+f);
const strings=await parse('metadata/string-binding-verification.json');
if(!strings.passed||strings.verified_declarations!==758||strings.retired_to_verified_C_literals!==34)throw new Error('String audit missing');
const log='artifacts/startup-music-final.log',probe=await read(log);
if(!probe.includes('FUN_004013f0')||!probe.includes('_RTC_UninitUse')||!probe.includes('EXIT 80000003'))throw new Error('Unexpected final failure');
const watched=Object.fromEntries([...probe.matchAll(/WATCH ([0-9a-f]{8})=([0-9a-f]{8})/gi)].map(m=>[m[1].toLowerCase(),parseInt(m[2],16)]));
for(const [a,v] of Object.entries({'00e3ac88':4,'00e3ac8c':3,'00e92bd0':0,'00e92bd4':0,'00e92bd8':3,'00e92be0':0,'00f55b88':1,'00e92bfc':246}))
 if(watched[a]!==v)throw new Error('Unexpected live state at '+a);
const assets=[];
for(const [f,count] of [['metadata/music-local-assets.json',39],['metadata/sound-effect-local-assets.json',263]]) {
 const manifest=await parse(f);if(manifest.original_executable_copied||manifest.original_installation_modified||manifest.entries.length!==count)throw new Error('Unexpected asset staging: '+f);
 for(const e of manifest.entries)if(await hash(path.relative(root,path.join(manifest.destination,e.file)))!==e.sha256)throw new Error('Staged asset changed: '+e.file);
 assets.push({manifest:f,files:count,bytes:manifest.entries.reduce((n,e)=>n+e.bytes,0),source:manifest.source,destination:manifest.destination});
}
const archive='metadata/debug-session-sound.json';
try {await access(path.join(root,archive));}catch(error){if(error.code!=='ENOENT')throw error;await save(archive,debug);}
const changed=['src/unit_02474.c','src/unit_02478.c','src/unit_01737.c','src/unit_02483.c','runtime/music_manager.c','CMakeLists.txt','README.md',
 'metadata/string-binding-retirements.json','tools/record_music_manager_repair.mjs'];
const source_hashes={};for(const f of new Set([...Object.keys(debug.source_hashes),...Object.keys(music.source_hashes),...Object.keys(lookup.source_hashes),...changed]))source_hashes[f]=await hash(f);
const evidence_hashes={...debug.evidence_hashes};
for(const f of [log,'artifacts/startup-manager-recheck.log','artifacts/startup-music-first.log','artifacts/music-debug-final-build.log','artifacts/music-release-final-build.log',
 'metadata/music-manager-emulation.json','metadata/sound-file-lookup-emulation.json','metadata/music-manager-before.json','metadata/sound-file-lookup-before.json',
 'metadata/sound-manager-load-native.asm','metadata/sound-manager-state-native.asm','metadata/sound-file-lookup-native.asm','metadata/music-startup-callers-native.asm',
 'metadata/music-00539a40-xrefs.json','metadata/music-00539250-xrefs.json','metadata/music-00539550-xrefs.json','metadata/music-00539730-xrefs.json','metadata/music-00539a50-xrefs.json',
 'metadata/music-local-assets.json','metadata/sound-effect-local-assets.json','metadata/string-binding-verification.json'])evidence_hashes[f]=await hash(f);
const failure={stage:'Following game-subsystem initialization after the sound startup routine returned',function:'FUN_004013f0',address:'0x004013f0',
 source:'src/unit_00002.c:9',caller:'FUN_00413ed0',caller_source:'src/unit_00177.c:220',
 reason:'Generated routine uses an uninitialized in_ECX while passing a member at self+0x4c into FUN_004e52b0; outer receiver and downstream pool/record initialization ABI require reconstruction',
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Restore the receiver and initialization contract at 0x004013f0/0x004e52b0 and subsequent startup consumers, verify offline, then repeat a bounded C startup'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Startup retest and independent C music-manager and sound-file registration repairs',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,UI_automation_used:false,
 source_only_tests:{Debug:{passed:31,total:31},Release:{passed:31,total:31}},component_checks:{music:{checks:295,scenarios:16},sound_file_lookup:{checks:10,scenarios:8}},
 native_comparison:{music:{report:'metadata/music-manager-emulation.json',cases:324,compared:music.compared,
  controlled_endpoints:['file IO','formatting','decimal conversion'],local_music_sha256:music.local_music_sha256,local_file_availability_controlled:true},
  sound_file_lookup:{report:'metadata/sound-file-lookup-emulation.json',cases:2000,compared:lookup.compared,controlled_endpoints:['strcmp','strdup']}},
 reconstruction:{music_header:'include/music_manager.h',music_manager_bytes:0x59a74,global_music_manager:'0x00e39180',category_count:53,track_bytes:112,
  music_entries:['0x00539250','0x00539550','0x005395e0','0x00539730','0x00539a40','0x00539a50'],file_lookup_entries:['0x0055dd3f','0x00539c60'],
  first_start_owner:'runtime/music_manager.c',file_table_receiver:'RESTORE_SOUND->files',
  fixes:['Explicit music constructor/load/insert/select/setter receivers and real contiguous track/category layouts',
   'Full-width population/profile access and native signed population selection',
   'Bounded loader paths, retained numeric defaults across lines, ambient markers and main/ambient capacity contracts',
   'Explicit sound-file table lookup receiver, exact-name deduplication and copy ownership; unused generated string/SEH temporary eliminated'],
  deliberate_repairs:['Reject category 53/negative categories, negative/full counters and filenames that exceed their storage',
   'Ignore AMBIENT marker after 50 main tracks rather than overwrite the count field',
   'Ignore missing type token and reject invalid selector profile/current bounds',
   'With CD bypass and no media drive, retain the relative missing-file path instead of writing a leading NUL drive prefix',
   'If owned-name duplication fails, return failure without inserting a null filename'],
  caller_only_change:'src/unit_02478.c',caller_only_limitation:'Only the nested selector receiver is repaired; the enclosing playback body remains unreviewed',
  string_bindings:{array_declarations:758,retired_to_verified_C_literals:34}},
 live_startup:{music_category0_main:watched['00e3ac88'],music_category0_ambient:watched['00e3ac8c'],selected_first:watched['00e92bd4'],selected_last:watched['00e92bd8'],
  current_track:watched['00e92be0'],sound_initialized:watched['00f55b88'],registered_sound_files:watched['00e92bfc'],sound_startup_routine_returned:true,log},
 assets_staged:assets,original_installation_modified:false,
 user_preferences_preserved:{CD_check_bypassed:true,child_only_16bit_compatibility:true,original_exe_required_for_build:false},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,game_audio_playback_confirmed:false,
 limitations:['The startup probe still stops before the complete game loop, at the following subsystem receiver error.',
  'Native comparisons cover the documented fixtures and controlled endpoints; unsafe native index/path behaviors intentionally differ in C.',
  'The native music constructor preserves ambient counters; owned first startup begins with zero data. Reentrant reset behavior is not repaired.',
  'The larger music playback body and downstream sound callbacks, 3D, shutdown, campaign save/load and special raster paths retain separate obligations.',
  'Actual music.txt contains existing missing-file/format typos (Hapj-A.mp3, amber.mp3A, Jrj-Hb-sd.mp38); its contents were preserved. Missing-file startup handling is not proof of playback.',
  'The strdup endpoint is controlled in file-lookup comparisons; the source-only tests additionally verify safe duplicate-allocation failure.',
  'Local licensed music/effect files were copied only to the isolated run directory. Data and multimedia DLLs remain separate runtime dependencies, not build dependencies.'],
 source_hashes,evidence_hashes};
await save('metadata/music-manager-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/music-manager-repair.json');
debug.observations.push({log,site:'FUN_004013f0',status:'Music config/selection and sound registration returned (246 IDs); next subsystem receiver fails',report:'metadata/music-manager-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/music-manager-repair.json';debug.previous_snapshot=archive;
debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;debug.source_only_tests=report.source_only_tests;debug.limitations=report.limitations;
debug.reproduce.watch_addresses='e3ac88,e3ac8c,e92bd0,e92bd4,e92bd8,e92be0,f55b88,e92bfc';debug.reproduce.assets_staged=assets;
for(const [name,f] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file:f,sha256:await hash(f),bytes:(await readFile(path.join(root,f))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[{addresses:[...report.reconstruction.music_entries,...report.reconstruction.file_lookup_entries],change:'Music object/startup ABI, bounded music parser/insertion/selection and sound-file registration ownership/receiver',
 verification:'31 source-only tests in both configurations; 324 native/C music cases, 2000 native/C file lookup cases; bounded startup reaches 246 registered files and the following subsystem',
 report:'metadata/music-manager-repair.json',limitation:'Playback and downstream initialization remain unfinished; invalid bounds/paths and allocation failure are deliberately safer than native'},
 ...manual.repairs.filter(r=>r.report!=='metadata/music-manager-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_cases:2324,registered_sound_files:246,next_failure:failure.function,playable:false}));
