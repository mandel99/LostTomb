import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=file=>readFile(path.join(root,file),'utf8');
const parse=async file=>JSON.parse(await read(file));
const hash=async file=>createHash('sha256').update(await readFile(path.join(root,file))).digest('hex');
const save=(file,data)=>writeFile(path.join(root,file),JSON.stringify(data,null,2)+'\n');
const debug=await parse('metadata/debug-session.json');
const archive='metadata/debug-session-startup-lookups.json';
try {await access(path.join(root,archive));} catch(error) {if(error.code!=='ENOENT')throw error;await save(archive,debug);}
const native=await parse('metadata/stream-slots-emulation.json');
if(!native.passed||native.checked!==120)throw new Error('Native selector comparison failed');
for(const [file,digest] of Object.entries(native.source_hashes))if(await hash(file)!==digest)throw new Error('Stale selector comparison: '+file);
for(const file of ['artifacts/local-assets-debug-tests.log','artifacts/local-assets-release-build.log'])
 if(!(await read(file)).includes('100% tests passed, 0 tests failed out of 24'))throw new Error('Missing test success: '+file);
const probe='artifacts/startup-local-assets-debug.log',probeText=await read(probe);
if(!probeText.includes('FUN_0053dd10')||!probeText.includes('EXIT 80000003')||!probeText.includes('Front Buffer:')||!probeText.includes('Back Buffer:'))throw new Error('Unexpected final startup observation');
const hr=await read('artifacts/directdraw-hresult-debug.log');
if((hr.match(/OBSERVED BREAKPOINT .*EAX=80004001/g)||[]).length!==3)throw new Error('Missing display-mode HRESULT evidence');
const sourceFiles=new Set([...Object.keys(debug.source_hashes),
 'tools/debug_child.c','tools/Debug-Standalone.ps1','src/unit_00422.c','src/unit_01755.c','src/unit_01749.c',
 'src/unit_01756.c','src/unit_01758.c','src/unit_02530.c','src/unit_02839.c','src/unit_02892.c',
 'include/legacy_streams.h','runtime/legacy_streams.c','tests/legacy_streams.c','tools/verify_stream_slots_emulation.py',
 'tools/record_local_assets_repair.mjs','CMakeLists.txt','tools/prepare_c.py']);
const source_hashes={};for(const file of sourceFiles)source_hashes[file]=await hash(file);
const evidenceFiles=['artifacts/directdraw-hresult-debug.log','artifacts/directdraw-compatible-startup.log',
 'artifacts/startup-after-directdraw.log',probe,'artifacts/legacy-streams-first-debug.log',
 'artifacts/local-assets-debug-tests.log','artifacts/local-assets-release-build.log','metadata/stream-slots-emulation.json',
 'metadata/debug-directdraw-call-sites.asm','metadata/resource-read-native.asm','metadata/legacy-stream-bootstrap-native.asm',
 'metadata/legacy-stream-selector-native.asm','metadata/sprite-library-init-native.asm','metadata/sprite-library-startup-native.asm'];
const evidence_hashes={...debug.evidence_hashes};for(const file of evidenceFiles)evidence_hashes[file]=await hash(file);
const failure={stage:'Sprite-library startup after successful DirectDraw and local configuration loading',function:'FUN_0053dd10',address:'0x0053dd10',
 source:'src/unit_02517.c:12',caller:'FUN_00414420',caller_source:'src/unit_00179.c:127',
 reason:'Uninitialized in_ECX: generated sprite-library method has no explicit object receiver',native_receiver_at_caller:'0x00f55bb0',
 native_evidence:['metadata/sprite-library-init-native.asm','metadata/sprite-library-startup-native.asm'],
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Restore typed library receivers and the contiguous 30-name startup table; review the nested sprite-file reader before enabling it'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Independent C startup: process-local 16-bit compatibility, legacy streams and user-requested CD bypass',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,
 source_only_tests:{Debug:{passed:24,total:24},Release:{passed:24,total:24}},
 directdraw:{unmitigated_mode_results:['0x80004001','0x80004001','0x80004001'],
  child_compatibility:'16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation',registry_changed:false,
  device_and_front_back_surfaces_created:true,pixel_format:'RGB565',full_rendering_verified:false,
  primary_source:'https://learn.microsoft.com/en-us/windows/deployment/planning/compatibility-fixes-for-windows-8-windows-7-and-windows-vista'},
 local_media_policy:{explicitly_requested_by_user:true,startup_check:'success without optical drive enumeration or signature checks',
  media_drive_prefix:'empty: relative to local asset directory',missing_file_retry:'IDCANCEL immediately, no insert-CD dialog',
  missing_files_accepted_as_success:false,native_equivalence_claimed:false},
 legacy_streams:{table_slots:512,original_static_records:20,real_simultaneous_binary_files:300,
  exact_contents_and_seek_verified:true,close_reuse_cleanup_verified:true,native_selector_comparisons:120,report:'metadata/stream-slots-emulation.json',
  host_FILE_layout_used_as_legacy:false,limitation:'This repairs legacy bootstrap/selector, not all CRT/FILE/TLS/standard-handle contracts'},
 isolated_assets:{working_directory:'build/debug-startup-game',root_Data_files_copied:422,root_Data_bytes:311901395,
  original_installation_modified:false,original_exe_or_media_DLLs_copied:false,
  note:'The required root Data files were copied. A copy of the unrelated extracted Fonts tree was interrupted; some ignored extra files may remain.'},
 debugger:{one_shot_breakpoints:'up to eight explicit addresses, validated within owned executable image; restore bytes and resume once',
  timeout:'15 seconds by default, maximum 60; snapshots owned primary thread before termination',
  retained_timeout_site:'MessageBoxA <- FUN_004ce9e0 <- FUN_004ceb60',
  final_log:probe,ui_automation_used:false},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,
 source_hashes,evidence_hashes};
await save('metadata/local-assets-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/local-assets-repair.json');
debug.observations.push({log:'artifacts/directdraw-hresult-debug.log',cause:'Mode calls return DDERR_UNSUPPORTED/E_NOTIMPL',status:'fixed for this host by process-local color compatibility',report:'metadata/local-assets-repair.json'},
 {log:'artifacts/startup-after-directdraw.log',site:'MessageBoxA <- FUN_004ce9e0 <- FUN_004ceb60',status:'fixed: initialize legacy streams; user-requested insert-CD wait disabled',report:'metadata/local-assets-repair.json'},
 {log:probe,site:'FUN_0053dd10',status:'open: missing sprite-library object receiver',report:'metadata/local-assets-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/local-assets-repair.json';debug.previous_snapshot=archive;
debug.source_only_tests=report.source_only_tests;debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;
debug.reproduce.command='.\\tools\\Debug-Standalone.ps1';debug.reproduce.child_compatibility=report.directdraw.child_compatibility;
debug.reproduce.asset_isolation=report.isolated_assets;
debug.limitations=['A real DirectDraw device and RGB565 surfaces were reached; complete rendering, game loop, sound/codecs and gameplay remain unverified.',
 'The current next failure is the missing sprite-library receiver. Other generated methods and callback/data contracts remain unfinished.',
 'The CD bypass is an intentional user-requested local-data policy; required data is still mandatory.'];
for(const [name,file] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file,sha256:await hash(file),bytes:(await readFile(path.join(root,file))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[
 {addresses:['0x00564a39','0x01266a90','0x01267aa0'],change:'Initialize separate legacy stream table and preserve original static records; restore full table/count slots and selection through the table pointer',test:'standalone_legacy_streams',verification:'300 real binary files plus 120 exact native/C slot-reset comparisons',report:'metadata/local-assets-repair.json'},
 {addresses:['0x004268c0','0x004ce9e0'],change:'User-requested local-data startup bypasses optical media checks and insert-CD waits; restore int return at four retry callers',native_equivalence_claimed:false,test:'standalone_legacy_streams',report:'metadata/local-assets-repair.json'},
 ...manual.repairs.filter(r=>r.report!=='metadata/local-assets-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,stream_comparisons:120,CD_bypass:true,DirectDraw_surfaces:true,next_failure:failure.function,playable:false}));
