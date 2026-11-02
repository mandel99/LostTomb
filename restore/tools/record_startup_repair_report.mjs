import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=file=>readFile(path.join(root,file),'utf8');
const parse=async file=>JSON.parse(await read(file));
const hash=async file=>createHash('sha256').update(await readFile(path.join(root,file))).digest('hex');
const save=(file,data)=>writeFile(path.join(root,file),JSON.stringify(data,null,2)+'\n');
const previous=await parse('metadata/debug-session.json');
const archive='metadata/debug-session-allocator-descriptors.json';
try {await access(path.join(root,archive));} catch(error) {if(error.code!=='ENOENT')throw error;await save(archive,previous);}
const bindings=await parse('metadata/startup-pointer-binding-repairs.json');
const emulation=await parse('metadata/startup-lookups-emulation.json');
if(!emulation.passed)throw new Error('Native comparison did not pass');
for(const file of ['artifacts/repair-startup-debug.log','artifacts/repair-startup-release.log'])
 if(!(await read(file)).includes('100% tests passed, 0 tests failed out of 23'))throw new Error('Missing final test success: '+file);
for(const [file,digest] of Object.entries(emulation.source_hashes))
 if(await hash(file)!==digest)throw new Error('Native comparison source snapshot is stale: '+file);
for(const repair of [...bindings.text.repairs,...bindings.frame_buffer.repairs]) {
 const source=await read(repair.file);
 if(repair.retired_into_shared_helper) {
  if(!source.includes('restore_text_select'))throw new Error('Retired binding has no shared helper: '+repair.file);
 } else if(!source.includes(repair.after))throw new Error('Pointer binding repair absent: '+repair.file);
 if(source.includes(repair.before))throw new Error('Byte binding still present: '+repair.file);
}
const log='artifacts/debug-startup-repair-final.log',probe=await read(log);
if(!probe.includes('Graphics state: game=640x480 display=640x480 fullscreen=1 depth=16 desktop-depth=32.') ||
 !probe.includes('Restored C startup returned 0; game-ready flag=0.') || !probe.includes('EXIT 00000003') ||
 /EXCEPTION c0000005|_RTC_UninitUse|restore_unresolved_return/i.test(probe))
 throw new Error('Startup observation does not match the recorded outcome');
const sourceFiles=new Set([...Object.keys(previous.source_hashes),...Object.keys(emulation.source_hashes),
 ...bindings.text.repairs.map(r=>r.file.replaceAll('\\','/')),...bindings.frame_buffer.repairs.map(r=>r.file.replaceAll('\\','/')),
 'src/unit_00184.c','src/unit_00223.c','src/unit_00225.c','src/unit_00249.c','src/unit_01654.c',
 'include/legacy_format.h','src/unit_02805.c','tests/legacy_format.c','tests/frame_buffer.c',
 'runtime/standalone_link_check.c','tools/record_startup_repair_report.mjs']);
const source_hashes={};
for(const file of sourceFiles)source_hashes[file]=await hash(file);
const evidenceFiles=['artifacts/repair-startup-debug.log','artifacts/repair-startup-release.log',
 'artifacts/debug-startup-campaign-fixed.log','artifacts/debug-startup-format-fixed.log',
 'artifacts/debug-startup-text-fixed.log','artifacts/debug-startup-grid-fixed.log',
 'artifacts/debug-startup-frame-fixed.log',log,'artifacts/debug-frame-verify.log',
 'metadata/startup-lookups-emulation.json','metadata/startup-pointer-binding-repairs.json',
 'metadata/campaign-parser-entry-native.asm','metadata/campaign-callers-native.asm',
 'metadata/campaign-navigation-native.asm','metadata/campaign-predicates-native.asm',
 'metadata/campaign-predicate-caller-native.asm','metadata/text-selection-native.asm',
 'metadata/startup-grid-native.asm','metadata/startup-grid-callers-native.asm','metadata/frame-buffer-native.asm'];
const evidence_hashes={...previous.evidence_hashes};
for(const file of evidenceFiles)evidence_hashes[file]=await hash(file);
const observations=[
 {log:'artifacts/debug-startup-campaign-fixed.log',site:'FUN_00564bc6 <- FUN_00561a36',cause:'Decompiled sprintf used an artificial local va_list buffer',status:'fixed by a real C varargs bridge'},
 {log:'artifacts/debug-startup-format-fixed.log',site:'FUN_004cc110',read_address:'0x00000082',cause:'Byte-truncated text pointers, wrong group stride and byte scan',status:'fixed with native byte selection and complete shared pointers'},
 {log:'artifacts/debug-startup-text-fixed.log',site:'restore_unresolved_return(0x004303a0)',cause:'False consumed-return check after a side-effect-only grid setter',status:'fixed against native call-site evidence; grid coordinate widths also repaired'},
 {log:'artifacts/debug-startup-grid-fixed.log',site:'FUN_00414bd0 -> FUN_00561bdc',read_address:'0x0000003c',cause:'Frame-buffer pointer truncated to 0x40 then passed to free',status:'fixed in owner and 14 other shared declarations'},
 {log,exit:'0x00000003',game_startup_return:0,game_ready_flag:0,debugger_exception:false,
  status:'No post-loader exception; C startup returns after DirectDraw display-mode setup fails'}
];
const failure={stage:'DirectDraw fullscreen display-mode initialization',function:'FUN_00426b40',address:'0x00426b40',
 source:'src/unit_00429.c',reason:'SetDisplayMode fails through the retained mode fallback; game startup returns zero',
 requested_mode:{width:640,height:480,bits:16,fullscreen:true},desktop_bits:32,
 kind:'Handled graphics initialization failure; no access violation or runtime-check exception',
 debugger_exit:'0x00000003',fixed:false,
 next_work:'Capture the DirectDraw HRESULT and validate supported display modes/compatibility before changing the rendering path',
 limitation:'An unsuccessful mode request does not by itself distinguish driver limitations from remaining integration issues'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Handwritten standalone C startup repair and explicitly authorized child-process debugging',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,
 source_only_tests:{Debug:{passed:23,total:23},Release:{passed:23,total:23}},
 native_comparisons:{campaign:emulation.campaign.checked,text:emulation.text.checked,grid:emulation.grid.checked,report:'metadata/startup-lookups-emulation.json'},
 frame_buffer_resize_scenarios:360,shared_pointer_inventory:'metadata/startup-pointer-binding-repairs.json',
 observations,last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,
 source_hashes,evidence_hashes,limitations:[
 'The independent program builds without the original EXE; a playable standalone game remains unfinished.',
 'Campaign parser, callers beyond the reviewed argument sites, and other consumers of repaired pointers remain incompletely reviewed.',
 'The formatter intentionally uses host CRT behavior; old formatter internals and unsupported format extensions are not established as equivalent.',
 'Finite native comparisons cover the documented fixtures, not arbitrary malformed campaign/text data.',
 'Real graphics driver DLLs loaded during the explicit startup probe; this does not verify working rendering or codecs.'
 ]};
await save('metadata/startup-repair.json',report);
for(const observation of previous.observations)if(observation.site?.includes('FUN_0041b920'))observation.status='fixed; see metadata/startup-repair.json';
previous.generated_at=report.generated_at;previous.observations.push(...observations);
previous.last_startup_failure=failure;previous.source_hashes=source_hashes;previous.evidence_hashes=evidence_hashes;
previous.source_only_tests=report.source_only_tests;previous.current_repair_report='metadata/startup-repair.json';
previous.previous_snapshot=archive;previous.limitations=report.limitations;
previous.build.images={};
for(const [name,file] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 previous.build.images[name]={file,sha256:await hash(file),bytes:(await readFile(path.join(root,file))).length};
await save('metadata/debug-session.json',previous);
const manual=await parse('metadata/standalone-manual-repairs.json');
const repairs=[
 {addresses:['0x00417dd0','0x0041b920','0x0041ba20','0x0041bab0'],change:'Explicit campaign receiver, full-width category comparison, signed selection and native record links/completion behavior; update direct argument sites',test:'standalone_campaign_index',verification:'3017 source-only checks; 3330 isolated native/C comparisons including complete object bytes, 23 global fields and x86 ABI',limitation:'Full campaign parser and enclosing callers beyond reviewed argument sites remain incomplete'},
 {addresses:['0x00561a36'],change:'Replace artificial decompiler va_list storage with a real C varargs sprintf bridge',test:'standalone_legacy_format',limitation:'Uses current host CRT semantics; no equivalence claim for original private formatter internals'},
 {addresses:['0x004cc110','0x004cc160'],change:'Restore complete text pointers, eight-byte groups, unsigned byte/control scanning and correct preceding-byte rule; repair shared slots in other consumers',test:'standalone_text_selection',verification:'40 isolated native/C comparisons; 24 source-only matrix cases and 10000 repeated lookups',limitation:'Other complete consumers and malformed/out-of-range text input contracts remain unreviewed'},
 {addresses:['0x004303a0','0x004ddec0','0x004ddf20','0x004ddf70'],change:'Restore full grid coordinates and mode-selector width; remove false return consumption after the void setter',test:'standalone_startup_grid',verification:'24 native/C comparisons over all 100 grid bytes',limitation:'Other render/grid consumers still have separate unresolved contracts'},
 {addresses:['0x00414bd0','0x00e92e1c'],change:'Preserve complete frame-buffer ownership pointer in owner and 14 other declarations; restore full dimensions, native wraparound sizing and mode selection',test:'standalone_frame_buffer',verification:'360 source-only resize scenarios with real host allocations and controlled unrelated callbacks',limitation:'Other enclosing consumers and full renderer integration remain incomplete'}
].map(repair=>({...repair,report:'metadata/startup-repair.json'}));
manual.repairs=[...repairs,...manual.repairs.filter(repair=>repair.report!=='metadata/startup-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_comparisons:report.native_comparisons,stage:failure.stage,playable:false}));
