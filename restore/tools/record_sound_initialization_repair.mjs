import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=f=>readFile(path.join(root,f),'utf8'),parse=async f=>JSON.parse((await read(f)).replace(/^\uFEFF/,''));
const hash=async f=>createHash('sha256').update(await readFile(path.join(root,f))).digest('hex');
const save=(f,v)=>writeFile(path.join(root,f),JSON.stringify(v,null,2)+'\n');
const debug=await parse('metadata/debug-session.json'),native=await parse('metadata/sound-initialization-emulation.json');
if(!native.passed||native.checked!==355)throw new Error('Missing native sound-state verification');
for(const [f,h] of Object.entries(native.source_hashes))if(await hash(f)!==h)throw new Error('Stale native comparison: '+f);
if(await hash('build/c-only-debug/Debug/restore_sound_verify.exe')!==native.compiled_C_sha256)throw new Error('C oracle changed');
for(const f of ['artifacts/sound-debug-final-build.log','artifacts/sound-release-final-build.log'])
 if(!(await read(f)).includes('100% tests passed, 0 tests failed out of 29'))throw new Error('Missing final test success: '+f);
const log='artifacts/startup-sound-first.log',probe=await read(log);
if(!probe.includes('FUN_00539a40')||!probe.includes('_RTC_UninitUse')||!probe.includes('EXIT 80000003'))throw new Error('Unexpected last failure');
const watched=Object.fromEntries([...probe.matchAll(/WATCH ([0-9a-f]{8})=([0-9a-f]{8})/gi)].map(m=>[m[1].toLowerCase(),parseInt(m[2],16)]));
for(const [a,v] of Object.entries({'00e92d90':5,'00e92bf8':500,'00f55b88':1}))if(watched[a]!==v)throw new Error('Unexpected sound state at '+a);
for(const a of ['00e92d70','00e92d94','00e92da0','00e92da4','00e92c00'])if(!watched[a])throw new Error('Missing live sound allocation at '+a);
const archive='metadata/debug-session-campaign.json';
try {await access(path.join(root,archive));}catch(error){if(error.code!=='ENOENT')throw error;await save(archive,debug);}
const changed=['src/unit_02474.c','src/unit_01735.c','src/unit_02688.c','src/unit_02693.c','src/unit_02699.c','src/unit_02468.c',
 'src/unit_02682.c','src/unit_02683.c','runtime/sound_initialization.c','runtime/sound_platform_win32.c','tools/prepare_c.py','CMakeLists.txt','README.md','tools/record_sound_initialization_repair.mjs'];
const files=new Set([...Object.keys(debug.source_hashes),...Object.keys(native.source_hashes),...changed]);
const source_hashes={};for(const f of files)source_hashes[f]=await hash(f);
const evidence_hashes={...debug.evidence_hashes};
for(const f of [log,'artifacts/sound-debug-final-build.log','artifacts/sound-release-final-build.log','metadata/sound-initialization-emulation.json',
 'metadata/sound-initialization-before.json','metadata/sound-initialization-native.asm','metadata/sound-stop-native.asm','metadata/sound-file-entries-native.asm',
 'metadata/sound-constructor-binding-native.asm','metadata/sound-0055e6c4-xrefs.json','metadata/sound-0055dea2-xrefs.json',
 'metadata/sound-0055e5d2-xrefs.json','metadata/sound-0055dc63-xrefs.json','metadata/sound-005394f0-xrefs.json','metadata/string-binding-verification.json'])evidence_hashes[f]=await hash(f);
const failure={stage:'Higher-level sound-manager setup, after digital device, five channels and 500 file slots initialized',function:'FUN_00539a40',address:'0x00539a40',
 source:'src/unit_02479.c:8',caller:'FUN_005394f0',caller_source:'src/unit_02474.c:19',
 reason:'Generated setter writes offset 0x59a50 through uninitialized in_ECX; native caller binds the larger manager at 0x00e39180',
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Restore the larger sound-manager receiver, constructor/state defaults and its startup setters/configuration/loading methods, then verify offline before another bounded C run'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Independent C sound-object construction, PCM startup, channel and file-table allocation',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,UI_automation_used:false,
 source_only_tests:{Debug:{passed:29,total:29},Release:{passed:29,total:29}},component_checks:{checks:278,scenarios:18,
  failure_cases:['four allocation positions','each of five sample-handle creation positions','unavailable OS version','unsupported legacy platform','wave device failure','file table allocation failure','size overflow rejection']},
 native_comparison:{report:'metadata/sound-initialization-emulation.json',cases:355,
  entries:['0x0055e38b','0x0055e6c4','0x0055dea2','0x0055e5d2','0x0055ec7c','0x0055dc63'],
  compared:native.compared,controlled_endpoints:['OS version/debug output','media APIs','allocator/free','vector construction/destruction'],
  constructor_and_void_result_markers_normalized:true,malformed_or_partial_allocation_failure_equivalence_claimed:false},
 reconstruction:{header:'include/sound_initialization.h',object_bytes:0x204,global_object:'0x00e92bf8',first_start_owner:'runtime/sound_initialization.c',
  platform_adapter:'runtime/sound_platform_win32.c',PCM_defaults:{rate:22050,bits:16,channels:2},native_provider_table_stride:12,
  runtime_fixes:['Explicit object/table receivers for PCM initialization, sample allocation/reset/stop and file allocation',
   'Owned first-start constructor sets provider table, format defaults, stream handles, volumes and complete global object pointer',
   '32-bit sound counter, pointer and startup-result bindings; complete PCM arithmetic and 16-bit format fields',
   'Correct old-handle/cache release order, file entry initialization and owned filename cleanup'],
  deliberate_repairs:['Allocation-size overflow returns failure before allocation',
   'Failed buffer allocation frees partial arrays, clears pointers and resets the channel count',
   'Failed sample-handle creation releases earlier handles and frees the new arrays instead of returning partial success'],
  caller_argument_only_changes:['src/unit_02688.c','src/unit_02693.c','src/unit_02699.c'],
  additional_binding_only_changes:['src/unit_02682.c','src/unit_02683.c','src/unit_02468.c']},
 live_startup:{initialization_return:watched['00f55b88'],digital_device:watched['00e92d70'],sample_count:watched['00e92d90'],
  sample_handles:watched['00e92d94'],sample_parameters:watched['00e92da0'],sample_cache:watched['00e92da4'],file_capacity:watched['00e92bf8'],file_entries:watched['00e92c00'],log},
 user_preferences_preserved:{CD_check_bypassed:true,child_only_16bit_compatibility:true,original_exe_required_for_build:false,existing_local_media_dependencies_used:true},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,game_audio_playback_confirmed:false,
 limitations:['Only first-start sound ownership is restored; complete shutdown, reentrant initialization, 3D setup and file lookup remain unreviewed.',
  'The larger manager at 0x00e39180 is the next runtime blocker; its remaining methods are not repaired by this low-level sound change.',
  'Three enclosing caller bodies and three additional consumers only received the listed argument/binding repairs; their other contracts remain unresolved.',
  'Native equivalence covers documented valid resource paths and device/version failures. C partial-allocation cleanup intentionally differs and is checked separately.',
  'Media/allocator/vector endpoints are controlled in emulation; actual device creation is additionally evidenced by the bounded owned-child run.',
  'The original executable was not run. Third-party media DLLs and data remain separate runtime dependencies.',
  'Earlier campaign save/load and special raster obligations remain; no complete game loop or mission has been reached.'],source_hashes,evidence_hashes};
await save('metadata/sound-initialization-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/sound-initialization-repair.json');
debug.observations.push({log,site:'FUN_00539a40',status:'Digital device, five channels and 500 file entries initialized; next missing larger-manager receiver',report:'metadata/sound-initialization-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/sound-initialization-repair.json';debug.previous_snapshot=archive;
debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;debug.source_only_tests=report.source_only_tests;debug.limitations=report.limitations;
debug.reproduce.watch_addresses='e92d70,e92d90,e92d94,e92da0,e92da4,e92bf8,e92c00,f55b88';
for(const [name,f] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file:f,sha256:await hash(f),bytes:(await readFile(path.join(root,f))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[{addresses:report.native_comparison.entries,change:'Owned sound constructor, explicit initialization/pool receivers, complete provider/format bindings, channel/file-table lifecycle and safe partial-failure cleanup',
 verification:'29 source-only tests in Debug/Release, 355 native/C sound-state cases, bounded C startup confirms device + 5 channels + 500 file entries',
 report:'metadata/sound-initialization-repair.json',limitation:'Partial-failure behavior deliberately repaired; larger manager, full shutdown, 3D and enclosing consumer contracts remain incomplete'},
 ...manual.repairs.filter(r=>r.report!=='metadata/sound-initialization-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_cases:native.checked,channels:report.live_startup.sample_count,file_slots:report.live_startup.file_capacity,next_failure:failure.function,playable:false}));
