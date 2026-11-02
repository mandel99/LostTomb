// Static actor callback evidence only; no generated code is imported into C.
import {readFile,writeFile,mkdir,stat} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath,pathToFileURL} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'),workspace=path.dirname(root);
const modules=path.join(workspace,'PharaohRewrite/tools/rea/node_modules');
const {Client}=await import(pathToFileURL(path.join(modules,'@modelcontextprotocol/client/dist/index.mjs')));
const {StdioClientTransport}=await import(pathToFileURL(path.join(modules,'@modelcontextprotocol/client/dist/stdio.mjs')));
const nodeDir=path.join(workspace,'PharaohRewrite/artifacts/tools/rea/node-v24.22.1-win-x64');
const javaHome=path.join(process.env.USERPROFILE,'.codex/tools/jdk-21.0.12.101-hotspot');
const out=path.join(root,'metadata/actor-callbacks-rea');
const table=JSON.parse(await readFile(path.join(root,'metadata/actor-callback-table-native.json'),'utf8'));
const addresses=[...new Set(table.entries.map(x=>x.address))];
await mkdir(out,{recursive:true});
const client=new Client({name:'restore-actor-evidence',version:'1.0.0'},{capabilities:{}});
const transport=new StdioClientTransport({command:path.join(nodeDir,'node.exe'),args:['--max-old-space-size=4096',path.join(modules,'rea-agents/scripts/rea.mjs'),'--mcp'],env:{...process.env,JAVA_HOME:javaHome,GHIDRA_INSTALL_DIR:path.join(workspace,'ghidra_12.1.4_PUBLIC'),REA_GHIDRA_STARTUP_TIMEOUT_MS:'900000',REA_GHIDRA_DISABLED_ANALYZERS:'Decompiler Parameter ID',REA_GHIDRA_SEED_FILE:path.join(root,'metadata/actor-callback-seeds.tsv'),PATH:nodeDir+';'+javaHome+'/bin;'+process.env.PATH},stderr:'inherit'});
async function call(name,args={}) {
 const r=await client.callTool({name,arguments:args},{timeout:1800000});
 if(r.isError)throw new Error(JSON.stringify(r));
 const d=r.structuredContent??JSON.parse(r.content.find(x=>x.type==='text').text);
 if(d.error)throw new Error(JSON.stringify(d.error));return d;
}
let opened=false;
try {
 await client.connect(transport);
 const target=await call('open_binary',{path:path.join(root,'build/smoke-game/Pharaoh.exe'),provider_id:'ghidra'});opened=true;
 if(target.result.sha256!==table.native_sha256)throw new Error('Original digest differs');
 console.log('Actor seed profile admitted; static Ghidra import running.');
 const controller=await call('analyze_function',{procedure:'0x00489b70'});
 await writeFile(path.join(root,'metadata/actor-update-rea-function.json'),JSON.stringify({data:controller},null,2)+'\n');
 console.log('Actor controller REA evidence retained.');
 const index={native_sha256:table.native_sha256,addresses,batches:[],C_imported:false,completed:false};
 for(let i=0;i<addresses.length;i+=32) {
  const name='batch-'+String(i).padStart(3,'0')+'.json',destination=path.join(out,name);
  try{await stat(destination);throw new Error('Refusing to overwrite retained batch '+destination);}catch(e){if(e.code!=='ENOENT')throw e;}
  const batch=await call('batch_decompile',{addresses:addresses.slice(i,i+32)});
  const result=batch.normalized_result,expected=addresses.slice(i,i+32);
  if(result.failed!==0||result.succeeded!==expected.length||
     JSON.stringify(result.items.map(x=>x.address))!==JSON.stringify(expected)||
     result.items.some(x=>x.status!=='ok'||typeof x.pseudocode!=='string'))
      throw new Error('Actor callback export is incomplete: '+name);
  await writeFile(destination,JSON.stringify(batch,null,2)+'\n');
  index.batches.push({file:name,evidence_id:batch.evidence_id,count:Math.min(32,addresses.length-i)});
  await writeFile(path.join(out,'index.json'),JSON.stringify(index,null,2)+'\n');
  console.log('Retained actor callbacks '+Math.min(i+32,addresses.length)+'/'+addresses.length);
 }
 await call('close_binary',{snapshot_path:path.join(out,'snapshot.json')});opened=false;index.completed=true;
 await writeFile(path.join(out,'index.json'),JSON.stringify(index,null,2)+'\n');
 console.log('Static actor callback evidence complete; no C source imported.');
} finally {
 if(opened){try{await call('close_binary');}catch(e){console.error('REA close: '+e.message);}}
 await client.close();
}
