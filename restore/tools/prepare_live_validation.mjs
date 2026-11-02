// Bind the live comparison script to the exact just-built DLL and linker map.
import {readFile,writeFile} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const runtimePath=path.resolve(process.argv[2]??path.join(root,'artifacts/reconstruction-current/Release/PharaohRestoreRuntime.dll'));
const output=path.resolve(process.argv[3]??path.join(root,'build/live-c-validation.js'));
const map=await readFile(runtimePath.replace(/\.dll$/i,'.map'),'utf8');
const baseMatch=map.match(/Preferred load address is\s+([0-9a-f]+)/i);
if(!baseMatch) throw new Error('Missing linker preferred base');
const base=Number.parseInt(baseMatch[1],16);
const specs=[
 ['004ded50',4,'pointer',52],
 ['004e1750',0,'scalar'],
 ['004e7e20',0,'global',0x00ea1820,4],
 ['004e92c0',2,'scalar'],['004e94a0',1,'scalar'],['004e94d0',1,'scalar'],
 ['004e9700',0,'city'],['004e9760',2,'scalar'],['004ea280',1,'scalar'],
 ['004ea930',0,'scalar'],['004ea9a0',1,'scalar'],['004ea9c0',1,'scalar'],['004eada0',1,'scalar'],
];
const targets=specs.map(([address,argc,mode,start,size])=>{
 const name='FUN_'+address;
 const match=map.match(new RegExp('\\s_'+name+'\\s+([0-9a-f]+)\\s+f\\s','i'));
 if(!match) throw new Error('Missing C symbol '+name);
 return {name,address:Number.parseInt(address,16),rva:Number.parseInt(match[1],16)-base,argc,mode,start,size};
});
const config={runtimePath,runtimeSha256:createHash('sha256').update(await readFile(runtimePath)).digest('hex'),gameDirectory:path.join(root,'build/smoke-game'),targets,maxSamples:64};
const template=await readFile(path.join(root,'tools/frida_validate_c.js'),'utf8');
await writeFile(output,'const validationConfig='+JSON.stringify(config)+';\n'+template);
console.log(JSON.stringify({output,runtimeSha256:config.runtimeSha256,targets:targets.length}));
