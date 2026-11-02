import {readFile,writeFile} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const build=path.resolve(root,process.argv[2]||'build/c-only');
const units=JSON.parse(await readFile(path.join(root,'units.json'),'utf8'));
const map=await readFile(path.join(build,'Release/restore_link_check.map'),'utf8');
const executable=await readFile(path.join(build,'Release/restore_link_check.exe'));
const pe=executable.readUInt32LE(0x3c),optional=pe+24,sectionTable=optional+executable.readUInt16LE(pe+20);
const sections=Array.from({length:executable.readUInt16LE(pe+6)},(_,i)=>{const p=sectionTable+i*40;return {rva:executable.readUInt32LE(p+12),size:executable.readUInt32LE(p+16),virtualSize:executable.readUInt32LE(p+8),raw:executable.readUInt32LE(p+20),flags:executable.readUInt32LE(p+36)};});
const imageBase=executable.readUInt32LE(optional+28);
const legacyCodeExecutable=sections.some(section=>(section.flags&0x20000000)&&imageBase+section.rva<0x56f000&&imageBase+section.rva+Math.max(section.size,section.virtualSize)>0x401000);
function offset(rva) {const section=sections.find(s=>rva>=s.rva&&rva<s.rva+s.size);if(!section)throw new Error('Unmapped PE data');return section.raw+rva-section.rva;}
function cstring(rva) {const p=offset(rva),end=executable.indexOf(0,p);if(end<0)throw new Error('Unterminated import name');return executable.subarray(p,end).toString('ascii');}
const imports=[];
for(let p=offset(executable.readUInt32LE(optional+104));executable.readUInt32LE(p+12);p+=20) imports.push(cstring(executable.readUInt32LE(p+12)));
const externalCrt=imports.filter(name=>/^(?:VCRUNTIME|MSVCP|MSVCR|api-ms-win-crt-)/i.test(name));
const resourceManifest=JSON.parse(await readFile(path.join(root,'metadata/standalone-resources.json'),'utf8'));
const symbols=new Set([...map.matchAll(/^\s+[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+[0-9a-f]{8,16}/gmi)].map(match=>match[1].replace(/^_/,'').replace(/@\d+$/,'')));
const absent=units.filter(unit=>!symbols.has(unit.name)).map(unit=>({name:unit.name,file:unit.file}));
const contracts=[];
let unresolvedReturns=0;
for(const unit of units) {
 const source=await readFile(path.join(root,unit.file),'utf8');
 const returns=[...source.matchAll(/\brestore_unresolved_return\s*\(/g)].length;
 const registers=[...new Set([...source.matchAll(/\bin_(?:EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP)\b/g)].map(match=>match[0]))];
 unresolvedReturns+=returns;
 if(returns||registers.length) contracts.push({name:unit.name,file:unit.file,unresolved_returns:returns,register_inputs_requiring_review:registers});
}
const data=JSON.parse(await readFile(path.join(root,'metadata/standalone-data.json'),'utf8'));
const report={format:1,generated_at:new Date().toISOString(),artifact:path.relative(root,path.join(build,'Release/restore_link_check.exe')),sha256:createHash('sha256').update(executable).digest('hex'),kind:'whole-program C link and data diagnostic; not a game',original_executable_required_for_build:false,original_executable_required_for_diagnostic:false,original_machine_code_embedded:false,recovered_procedure_sources:units.length,recovered_public_entries_present:units.length-absent.length,missing_recovered_entries:absent,unresolved_return_sites:unresolvedReturns,procedures_with_return_or_register_review:contracts.length,unreviewed_data_code_pointer_candidates:data.unreviewed_code_pointer_candidates,playable_game:false,game_runtime_tested:false,remaining_work:['Resolve remaining return registers and register-based input contracts against native evidence.','Rebuild typed callbacks, vtables, exception handling and startup in C; numeric code-address matches alone are insufficient.','Connect a C-only game entry and validate resource consumers.','Then perform gameplay validation when the user permits it.'],contracts};
report.imported_libraries=imports;
report.external_C_runtime_required=externalCrt.length!==0;
report.embedded_resource_entries=resourceManifest.resources.length;
report.original_code_address_range_executable=legacyCodeExecutable;
const windowStartup=await readFile(path.join(root,'src/unit_01774.c'),'utf8');
report.startup_callback_obligations=/lpfnWndProc\s*=\s*\(WNDPROC\)\s*&LAB_00414cd0/.test(windowStartup)?[
 {address:'0x00414cd0',role:'Win32 window message callback',status:'original absolute callback remains; no recovered entry in units.json',source:'src/unit_01774.c'}
]:[];
report.additional_recovered_entries=symbols.has('restore_window_proc')?[
 {address:'0x00414cd0',symbol:'restore_window_proc',source:'runtime/window_messages.c',abi:'Win32 WNDPROC; stdcall, four arguments, LRESULT',linked:true}
]:[];
report.conditional_callback_obligations=report.additional_recovered_entries.length?[
 {address:'0x00560330',role:'optional legacy message-trace dispatcher',condition:'DWORD 0x006fdc40 != 0',status:'selector/callback ABI not yet reconstructed; production fails explicitly when enabled'}
]:[];
try {
 const debug=JSON.parse(await readFile(path.join(root,'metadata/debug-session.json'),'utf8'));
 let current=true;
 for(const [file,expected] of Object.entries(debug.source_hashes)) {
  if(createHash('sha256').update(await readFile(path.join(root,file))).digest('hex')!==expected)current=false;
 }
 report.live_C_startup_probe={report:'metadata/debug-session.json',current_source_snapshot:current,
  stage:debug.last_startup_failure.stage,failure:debug.last_startup_failure,
  full_game_loop_reached:current&&!!debug.full_game_loop_reached,gameplay_tested:current&&!!debug.gameplay_tested};
 report.game_runtime_tested=current&&!!debug.restored_C_startup_executed;
} catch(error) {if(error.code!=='ENOENT')throw error;}
await writeFile(path.join(root,'metadata/standalone-readiness.json'),JSON.stringify(report,null,2)+'\n');
console.log(JSON.stringify(Object.fromEntries(Object.entries(report).filter(([key])=>!['contracts','remaining_work'].includes(key))),null,2));
if(absent.length||externalCrt.length||legacyCodeExecutable) process.exitCode=1;
