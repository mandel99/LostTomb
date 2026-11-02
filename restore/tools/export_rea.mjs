// Read-only REA export. Retain each batch before requesting the next one.
import { readFile, writeFile, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';
import { spawn } from 'node:child_process';
const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const workspace = path.dirname(root);
const modules = path.join(workspace, 'PharaohRewrite/tools/rea/node_modules');
const { Client } = await import(pathToFileURL(path.join(modules, '@modelcontextprotocol/client/dist/index.mjs')));
const { StdioClientTransport } = await import(pathToFileURL(path.join(modules, '@modelcontextprotocol/client/dist/stdio.mjs')));
const nodeDir = path.join(workspace, 'PharaohRewrite/artifacts/tools/rea/node-v24.22.1-win-x64');
const javaHome = path.join(process.env.USERPROFILE, '.codex/tools/jdk-21.0.12.101-hotspot');
const client = new Client({name:'restore-export', version:'1.0.0'}, { capabilities: {} });
const transport = new StdioClientTransport({command:path.join(nodeDir,'node.exe'),
  args:['--max-old-space-size=4096',path.join(modules,'rea-agents/scripts/rea.mjs'), '--mcp'],
  env:{...process.env, JAVA_HOME:javaHome, GHIDRA_INSTALL_DIR:path.join(workspace,'ghidra_12.1.4_PUBLIC'),
    REA_GHIDRA_STARTUP_TIMEOUT_MS:'1800000',
    REA_GHIDRA_DISABLED_ANALYZERS:'Decompiler Parameter ID',
    PATH:`${nodeDir};${javaHome}/bin;${process.env.PATH}`}, stderr:'inherit'});
function unpack(result) {
  if (result.isError) throw new Error(JSON.stringify(result));
  const data = result.structuredContent ?? JSON.parse(result.content.find(x=>x.type==='text').text);
  if (data.error) throw new Error(JSON.stringify(data.error));
  return data;
}
async function call(name,args={}) {
  return unpack(await client.callTool({name,arguments:args}, {timeout:1800000}));
}
await mkdir(path.join(root,'raw/rea'),{recursive:true});
let watchdog;
let binaryOpened=false;
try {
  await client.connect(transport);
  const opened = await call('open_binary',{path:process.argv[2]??'C:\\Program Files (x86)\\GOG.com\\Pharaoh\\Pharaoh.exe',provider_id:'ghidra'});
  binaryOpened=true;
  if (opened.result.sha256!=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d') {
    throw new Error('Unexpected executable digest; retained export was not changed.');
  }
  await writeFile(path.join(root,'raw/rea/target.json'),JSON.stringify(opened,null,2));
  console.log('Target admitted; running Ghidra analysis.');
  const procedures = await call('list_procedures');
  await writeFile(path.join(root,'raw/rea/procedures.json'),JSON.stringify(procedures,null,2));
  if (process.platform==='win32') {
    watchdog=spawn('powershell.exe',['-NoProfile','-File',path.join(root,'tools/watch_decompiler.ps1'),
      '-ExportProcessId',String(process.pid),'-ReaServerProcessId',String(transport.pid)],
      {windowsHide:true,stdio:'ignore'});
  }
  const all = procedures.normalized_result.filter(p=>!p.procedure?.external);
  console.log(`Analysis ready: ${all.length} local procedures.`);
  for(let i=0;i<all.length;i+=32) {
    const saved = path.join(root,`raw/rea/batch-${String(i).padStart(5,'0')}.json`);
    try {
      const previous = JSON.parse(await readFile(saved,'utf8'));
      const expected = all.slice(i,i+32).map(p=>p.address);
      if (previous.subject.digest.sha256 !== opened.result.sha256 ||
          previous.analysis_profile.parameters.upstream_analysis_profile.digest !== procedures.analysis_profile.digest ||
          JSON.stringify(previous.normalized_result.items.map(p=>p.address)) !== JSON.stringify(expected)) {
        throw new Error('Saved batch belongs to a different target, profile, or inventory.');
      }
      console.log(`Retained ${Math.min(i+32,all.length)}/${all.length}`);
      continue;
    } catch(error) { if(error.code!=='ENOENT') throw error; }
    const batch = await call('batch_decompile',{addresses:all.slice(i,i+32).map(p=>p.address)});
    await writeFile(path.join(root,`raw/rea/batch-${String(i).padStart(5,'0')}.json`),JSON.stringify(batch,null,2));
    console.log(`Exported ${Math.min(i+32,all.length)}/${all.length}`);
  }
  for (const type of ['CRect','tagRECT','tagMSG','_WIN32_FIND_DATAA','WNDCLASSA']) {
    const data = await call('inspect_native_data_type',{type});
    await writeFile(path.join(root,`raw/rea/type-${type}.json`),JSON.stringify(data,null,2));
  }
  await call('export_evidence_bundle',{path:path.join(root,'raw/rea/evidence.json'),overwrite:true});
  await call('close_binary',{snapshot_path:path.join(root,'raw/rea/analysis-snapshot.json'),overwrite:true});
  binaryOpened=false;
  console.log('REA export complete.');
} finally {
  watchdog?.kill();
  if (binaryOpened) {
    try { await call('close_binary'); }
    catch(error) { console.error(`REA cleanup: ${error.message}`); }
  }
  await client.close();
}
