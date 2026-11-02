import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=f=>readFile(path.join(root,f),'utf8'),parse=async f=>JSON.parse((await read(f)).replace(/^\uFEFF/,''));
const hash=async f=>createHash('sha256').update(await readFile(path.join(root,f))).digest('hex');
const save=(f,v)=>writeFile(path.join(root,f),JSON.stringify(v,null,2)+'\n');
const debug=await parse('metadata/debug-session.json'),native=await parse('metadata/campaign-parser-emulation.json');
if(!native.passed||native.checked!==333||native.parser_cases!==330||native.constructor_cases!==3)throw new Error('Missing parser/constructor equivalence');
for(const [file,digest] of Object.entries(native.source_hashes))if(await hash(file)!==digest)throw new Error('Stale native comparison: '+file);
if(await hash('build/c-only-debug/Debug/restore_campaign_parser_verify.exe')!==native.compiled_C_sha256)throw new Error('C oracle binary changed');
for(const file of ['artifacts/campaign-debug-final-build.log','artifacts/campaign-release-final-build.log'])
 if(!(await read(file)).includes('100% tests passed, 0 tests failed out of 28'))throw new Error('Missing final test success: '+file);
const strings=await parse('metadata/string-binding-verification.json');
if(!strings.passed||strings.verified_declarations!==762||strings.retired_to_verified_C_literals!==30)throw new Error('Missing string audit');
const log='artifacts/startup-campaign-final.log',probe=await read(log);
if(!probe.includes('FUN_0055e6c4')||!probe.includes('_RTC_UninitUse')||!probe.includes('mss32.dll')||!probe.includes('EXIT 80000003'))throw new Error('Unexpected final live failure');
const watched=Object.fromEntries([...probe.matchAll(/WATCH ([0-9a-f]{8})=([0-9a-f]{8})/gi)].map(m=>[m[1].toLowerCase(),parseInt(m[2],16)]));
for(const [a,v] of Object.entries({'006ff608':56,'00700994':53,'01265184':30,'0126517c':25140}))
 if(watched[a]!==v)throw new Error('Unexpected startup state at '+a);
const mediaManifest='build/c-only-debug/Debug/media-dependencies.json',media=await parse(mediaManifest);
if(media.original_executable_copied||media.entries.length!==3)throw new Error('Unexpected media dependency staging');
for(const entry of media.entries)if(await hash(path.relative(root,entry.destination))!==entry.sha256)throw new Error('Staged media DLL changed');
const archive='metadata/debug-session-render.json';
try {await access(path.join(root,archive));}catch(error){if(error.code!=='ENOENT')throw error;await save(archive,debug);}
const files=new Set([...Object.keys(debug.source_hashes),...Object.keys(native.source_hashes),
 'src/unit_00177.c','src/unit_00250.c','src/unit_02287.c','CMakeLists.txt','README.md','tools/Stage-MediaDependencies.ps1',
 'tools/repair_string_bindings.mjs','metadata/string-binding-retirements.json','tools/record_campaign_parser_repair.mjs']);
const source_hashes={};for(const file of files)source_hashes[file]=await hash(file);
const evidence_hashes={...debug.evidence_hashes};
for(const file of [log,'artifacts/startup-campaign-first.log','artifacts/startup-campaign-media.log',
 'artifacts/campaign-debug-final-build.log','artifacts/campaign-release-final-build.log','metadata/campaign-parser-emulation.json',
 'metadata/campaign-parser-native.asm','metadata/campaign-parser-before.json','metadata/campaign-constructor-before.json',
 'metadata/campaign-reset-xrefs.json','metadata/campaign-append-xrefs.json','metadata/campaign-constructor-xrefs.json','metadata/campaign-parser-xrefs.json',
 'metadata/string-binding-verification.json',mediaManifest])evidence_hashes[file]=await hash(file);
const failure={stage:'Sound-object initialization after graphics, campaign parsing and multimedia DLL loading',function:'FUN_0055e6c4',address:'0x0055e6c4',
 source:'src/unit_02692.c:30',caller:'FUN_005394f0',caller_source:'src/unit_02474.c:17',
 reason:'Sound initialization uses an uninitialized in_ECX object receiver after calling the loaded AIL_startup middleware entry; constructor/defaults and downstream receiver contracts remain incomplete',
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Reconstruct the sound object and its initialization/cleanup receivers and defaults, verify with controlled media endpoints, then repeat a bounded C startup'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Independent C campaign parser and prefix construction; next observed sound startup failure',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,UI_automation_used:false,
 source_only_tests:{Debug:{passed:28,total:28},Release:{passed:28,total:28}},component_checks:{checks:315,parser_scenarios:12},
 native_comparison:{report:'metadata/campaign-parser-emulation.json',parser_cases:330,constructor_cases:3,
  entries:['0x0041ac00','0x0041ac50','0x0041ac90','0x0041ad20','0x0041ad50','0x0041adc0','0x0041adf0','0x0041ae40','0x00561db1'],
  native_game_helpers_execute:true,controlled_endpoints:['file IO','ASCII digit/atoi','ASCII case comparison','string formatting','logging'],
  original_frame_padding_normalized_to_zero:true,compared:native.compared,local_campaign_sha256:native.local_asset_sha256},
 reconstruction:{header:'include/campaign_parser.h',object_prefix_bytes:0x4a84,record_bytes:64,record_capacity:100,name_capacity:100,name_bytes:50,
  explicit_parser_callers:['src/unit_00177.c','src/unit_02287.c'],prefix_constructor_call_adapted:'src/unit_00250.c',
  fixes:['Explicit campaign/record receivers, a contiguous record frame and a contiguous 13-entry section table',
   'Restore numeric reader status and low-word text storage; remove false unresolved-return traps after logging',
   'Preserve complete tokenizer cursor pointer and replace a single delimiter byte',
   'Preserve native branch validation, record carryover and historical merge-token consumption',
   'Restore the prefix constructor and its local temporary receiver without claiming reconstruction of the enclosing save/load body'],
  string_bindings:{array_declarations_verified:762,retired_to_evidenced_C_literals:30,retirement_manifest:'metadata/string-binding-retirements.json'}},
 live_startup:{loaded_libraries:watched['01265184'],next_image:watched['0126517c'],campaign_records:watched['006ff608'],mission_names:watched['00700994'],
  campaign_parser_returned_successfully:true,mss32_loaded:true,log},
 media_dependencies:{manifest:mediaManifest,entries:media.entries,original_executable_copied:false,
  required_for_build:false,required_for_console_tests:false,scope:'Existing user-installed third-party Win32 media DLLs; not reconstructed Pharaoh game code',staged_configuration:'Debug'},
 user_preferences_preserved:{CD_check_bypassed:true,child_only_16bit_compatibility:true,original_exe_required_for_build:false},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,
 limitations:['The sound object and downstream sound helpers remain unfinished; no game loop or mission is confirmed.',
  'The save/load enclosing procedure still has unresolved outer receivers and IO arguments. Only its constructor argument was adapted.',
  'Native frame padding and unused merge bytes were unspecified; C initializes them deterministically and emulator fixtures normalize them.',
  'ASCII CRT and IO boundaries are controlled in differential tests; actual local campaign reading is additionally confirmed by the bounded C startup.',
  'Safe numeric parsing uses defined 32-bit wraparound; errors are bounded to the existing message buffer and append rejects negative counts.',
  'The final loading-image descriptor still had a zero pixel pointer in the preceding renderer snapshot; remaining raster paths are unreviewed.',
  'Third-party media DLLs and game data remain separate runtime dependencies, and are not needed to build the independent C executable.'],
 source_hashes,evidence_hashes};
await save('metadata/campaign-parser-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/campaign-parser-repair.json');
debug.observations.push({log,site:'FUN_0055e6c4',status:'Campaign parsed (53 names, 56 links); media DLL loaded, then missing sound receiver',report:'metadata/campaign-parser-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/campaign-parser-repair.json';debug.previous_snapshot=archive;
debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;debug.source_only_tests=report.source_only_tests;debug.limitations=report.limitations;
debug.reproduce.watch_addresses='6ff608,700994,1265184,126517c';debug.reproduce.media_dependencies=report.media_dependencies;
for(const [name,file] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file,sha256:await hash(file),bytes:(await readFile(path.join(root,file))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[{addresses:report.native_comparison.entries,change:'Campaign parser/prefix constructor/record helper receivers, contiguous frame and sections, numeric status, tokenizer pointer/byte writes and both parser caller contracts',
 verification:'28 source-only tests in Debug/Release, 330 native/C parser cases and 3 constructor cases; bounded startup confirms 53 names and 56 records',
 report:'metadata/campaign-parser-repair.json',limitation:'Remaining sound and save/load contracts unreviewed; native padding normalized, CRT/IO test boundaries controlled'},
 ...manual.repairs.filter(r=>r.report!=='metadata/campaign-parser-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_cases:native.checked,live:report.live_startup,next_failure:failure.function,playable:false}));
