// Conservative inventory: compiling a function is not evidence that it replaces
// native code, and replacing native code is not evidence of full-game equivalence.
import {readFile,writeFile} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const units=JSON.parse(await readFile(path.join(root,'units.json'),'utf8'));
const host=await readFile(path.join(root,'runtime/hybrid_main.c'),'utf8');
const hooks=await readFile(path.join(root,'runtime/experimental_hooks.h'),'utf8');
let manifest={};
try {
 const current=JSON.parse(await readFile(path.join(root,'artifacts/current-build.json'),'utf8'));
 const exe=path.join(root,current.executable);
 manifest=JSON.parse(await readFile(exe.replace(/\.exe$/i,'.manifest.json'),'utf8'));
} catch(error) {if(error.code!=='ENOENT') throw error;}
const addresses=text=>[...text.matchAll(/0x[0-9a-f]+/gi)].map(match=>Number.parseInt(match[0],16));
const defaultLimit=Number(host.match(/#define RESTORE_DEFAULT_EXPERIMENTAL_HOOK_LIMIT\s+(\d+)/)[1]);
const base=addresses(host.match(/static const DWORD hook_addresses[^=]*=\{([^}]+)\}/s)[1]);
const quarantine=addresses(host.match(/experimental_hook_quarantine\[\]=\{([^}]+)\}/s)[1]);
const selected=addresses(hooks.match(/experimental_hook_addresses[^=]*=\{([^}]+)\}/s)[1]).slice(0,defaultLimit);
const active=new Set([...base,...selected.filter(address=>!quarantine.includes(address))]);
const sources=await Promise.all(units.map(async unit=>({unit,source:await readFile(path.join(root,unit.file),'utf8')})));
const remaining=units.filter(unit=>!active.has(Number.parseInt(unit.provenance.address,16))).map(unit=>({name:unit.name,address:unit.provenance.address,file:unit.file}));
const unresolvedReturns=sources.reduce((total,{source})=>total+[...source.matchAll(/\brestore_unresolved_return\s*\(/g)].length,0);
const independent=manifest.standalone_c_reconstruction===true && manifest.original_code_fallback===false && remaining.length===0 && unresolvedReturns===0;
const result={format:1,generated_at:new Date().toISOString(),standalone_c_game:independent,
 source_build_requires_original_exe:false,
 original_code_fallback:manifest.original_code_fallback??null,
 original_file_required_by_hybrid_loader:true,
 compiled_procedure_sources:units.length,
 default_selected_experimental_prefix:defaultLimit,
 default_c_entry_replacements:active.size,
 quarantined_addresses:quarantine.map(address=>'0x'+address.toString(16).padStart(8,'0')),
 unreplaced_recovered_procedure_entries:remaining.length,
 current_unresolved_return_diagnostics:unresolvedReturns,
 independent_c_subsystems:[{source:'runtime/city_history.c',test:'standalone_city_history',mapped_procedures:['0x004e92c0','0x004e94a0','0x004e94d0','0x004e9700','0x004e9760']}],
 limitations:['Entry replacement counts do not measure completed reconstruction or code coverage.','Asset files and licensed audio/video DLLs remain separate dependencies.','C-only game entry, original global layout and all reachable native fallbacks still need reconstruction.'],
 remaining_procedures:remaining};
const output=path.join(root,'metadata/independence-status.json');
try {
 const diagnostic=JSON.parse(await readFile(path.join(root,'metadata/standalone-readiness.json'),'utf8'));
 result.source_only_diagnostic={artifact:diagnostic.artifact,recovered_public_entries_present:diagnostic.recovered_public_entries_present,original_executable_required:false,original_machine_code_embedded:false,external_C_runtime_required:diagnostic.external_C_runtime_required,playable_game:false};
 result.hybrid_field_scope='original_code_fallback and entry-replacement counts above describe only the separately selected historical hybrid; not the source-only diagnostic';
} catch(error) {if(error.code!=='ENOENT') throw error;}
result.limitations[2]='The source-only diagnostic links all recovered C entries and initializes data/resources; typed callbacks, exception frames, unresolved contracts and a game entry remain unfinished.';
await writeFile(output,JSON.stringify(result,null,2)+'\n');
console.log(JSON.stringify({output,...Object.fromEntries(Object.entries(result).filter(([key])=>!['remaining_procedures','limitations','independent_c_subsystems'].includes(key)))}));
if(process.argv.includes('--require-independent') && !result.standalone_c_game) process.exitCode=1;
