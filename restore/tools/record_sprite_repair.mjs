import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=f=>readFile(path.join(root,f),'utf8'),parse=async f=>JSON.parse(await read(f));
const hash=async f=>createHash('sha256').update(await readFile(path.join(root,f))).digest('hex');
const save=(f,data)=>writeFile(path.join(root,f),JSON.stringify(data,null,2)+'\n');
const debug=await parse('metadata/debug-session.json'),native=await parse('metadata/sprite-reader-emulation.json');
const callers=await parse('metadata/sprite-call-arguments.json'),receiver=await parse('metadata/sprite-receiver-audit.json');
const archive='metadata/debug-session-local-assets.json';
try {await access(path.join(root,archive));} catch(e) {if(e.code!=='ENOENT')throw e;await save(archive,debug);}
if(!native.passed||native.checked!==40||receiver.recognized_global_receivers!==73||receiver.unresolved_receivers.length)throw new Error('Missing native reader/receiver verification');
for(const [f,digest] of Object.entries(native.source_hashes))if(await hash(f)!==digest)throw new Error('Stale native comparison: '+f);
if(await hash('build/c-only-debug/Debug/restore_sprites_verify.exe')!==native.compiled_C_sha256)throw new Error('Native C oracle binary changed');
for(const f of ['artifacts/sprites-debug-final-build.log','artifacts/sprites-release-build.log'])
 if(!(await read(f)).includes('100% tests passed, 0 tests failed out of 25'))throw new Error('Missing final test success: '+f);
const log='artifacts/startup-sprites-final.log',probe=await read(log);
if(!probe.includes('FUN_0053e590')||!probe.includes('EXIT 80000003'))throw new Error('Unexpected final failure');
const watched=Object.fromEntries([...probe.matchAll(/WATCH ([0-9a-f]{8})=([0-9a-f]{8})/gi)].map(m=>[m[1].toLowerCase(),parseInt(m[2],16)]));
for(const [a,value] of Object.entries({'01265184':1,'0126517c':683,'01265178':39,'01265180':39218,'00f64718':39218,'00f6471c':682,'00f64720':38}))
 if(watched[a]!==value)throw new Error('Unexpected live library state at '+a);
const files=new Set([...Object.keys(debug.source_hashes),...Object.keys(native.source_hashes),...callers.files,
 'src/unit_00179.c','tools/debug_child.c','CMakeLists.txt','tools/prepare_c.py','tools/record_sprite_repair.mjs']);
const source_hashes={};for(const f of files)source_hashes[f]=await hash(f);
const evidence_hashes={...debug.evidence_hashes};
for(const f of [log,'artifacts/startup-sprites-first.log','artifacts/sprites-debug-final-build.log','artifacts/sprites-release-build.log',
 'metadata/sprite-reader-emulation.json','metadata/sprite-receiver-audit.json','metadata/sprite-call-arguments.json','metadata/sprite-load-xrefs.json',
 'metadata/sprite-library-startup-native.asm','metadata/sprite-library-init-native.asm','metadata/sprite-library-manager-native.asm',
 'metadata/sprite-library-reader-native.asm','metadata/sprite-library-pixels-native.asm','metadata/sprite-tag-list-native.asm','metadata/sprite-color-conversion-native.asm'])evidence_hashes[f]=await hash(f);
const failure={stage:'Drawing the startup image after the first graphics library loaded',function:'FUN_0053e590',address:'0x0053e590',
 source:'src/unit_02524.c:113',caller:'FUN_00520b20',caller_source:'src/unit_02360.c:15',
 reason:'Generated rendering method reads uninitialized in_ECX; renderer object and downstream descriptor/argument contracts remain incomplete',
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Restore renderer receivers and callers, then validate image selection, clipping and rendering argument widths before executing drawing loops'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Independent C sprite-library reconstruction and bounded live startup validation',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,
 source_only_tests:{Debug:{passed:25,total:25},Release:{passed:25,total:25}},
 component_checks:{checks:661,reader_scenarios:40,scope:'Compiled C fixture tests, retained counts, tags/duplicates, system exclusion, object state and failure guards'},
 native_comparison:{report:'metadata/sprite-reader-emulation.json',cases:40,
  scope:'Native SG3 reader, native list and stack-probe helpers, pixel loader and color conversion versus compiled C',
  compared:['record state','image descriptors','tag mapping','exact pixels','data cursor','native stack cleanup and preserved registers'],
  endpoints_controlled:['filesystem','allocation/free'],heap_addresses_normalized:true,allocation_call_counts_equivalent:false,
  format_width_regression:{input:'0x00000102',cause:'Two converters treated the DWORD selector as a byte',fixed:true}},
 reconstruction:{entries:['0x00414420','0x0053dd10','0x0053dd40','0x0053dd80','0x0053dd90','0x0053dff0','0x0053f660'],
  header:'include/sprite_libraries.h',startup_name_count:30,updated_direct_call_sites:73,updated_caller_units:10,
  native_global_receiver:'0x00f55bb0',receiver_audit:'metadata/sprite-receiver-audit.json',
  constructor_replacement:'Explicit first-start C initialization of the owned manager; no claim of complete original C++ lifecycle reconstruction'},
 live_first_library:{name:'Pharaoh_Unloaded',loaded_libraries:watched['01265184'],image_descriptors:watched['00f6471c'],
  tags:watched['00f64720'],pixel_bytes:watched['00f64718'],pixel_pointer:watched['00f64714'],
  next_image:watched['0126517c'],next_tag:watched['01265178'],log},
 user_preferences_preserved:{CD_check_bypassed:true,child_only_16bit_compatibility:true,UI_automation_used:false,original_exe_required_for_build:false},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,
 limitations:['The first real library loaded; full startup, rendering, all remaining libraries and gameplay are unverified.',
  'Native equivalence checks cover the documented SG3 fixtures; filesystem, allocator traces and invalid-file behavior are not equivalent claims.',
  'The C pass replaces temporary native tag-list allocations; file bounds are checked and tag-count warnings use stderr instead of a modal legacy string dialog.',
  'Other enclosing procedures at the 73 corrected argument sites remain unreviewed.',
  'The original reset/destructor and other sprite/render methods remain unfinished; manager initialization is for the first independent startup.'],
 source_hashes,evidence_hashes};
await save('metadata/sprite-libraries-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/sprite-libraries-repair.json');
debug.observations.push({log,site:'FUN_0053e590',status:'First library loaded (682 images, 38 tags); next failure is missing renderer receiver',report:'metadata/sprite-libraries-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/sprite-libraries-repair.json';debug.previous_snapshot=archive;
debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;debug.source_only_tests=report.source_only_tests;debug.limitations=report.limitations;
debug.reproduce.watch_addresses='1265184,126517c,1265178,1265180,f64714,f64718,f6471c,f64720';
for(const [name,file] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file,sha256:await hash(file),bytes:(await readFile(path.join(root,file))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[{addresses:report.reconstruction.entries,change:'Explicit sprite-manager/library receivers, contiguous startup names, first-start object initialization, SG3 tags/images/files/pixels and 73 direct call arguments',
 verification:'661 component checks, 40 native/C reader comparisons and live first-library state',report:'metadata/sprite-libraries-repair.json',
 limitation:'Renderer and remaining lifecycle/caller bodies remain incomplete; allocator traces and malformed inputs differ intentionally'},
 ...manual.repairs.filter(r=>r.report!=='metadata/sprite-libraries-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_cases:40,live_first_library:report.live_first_library,next_failure:failure.function,playable:false}));
