// Analysis-only PE: preserve the callback, its switch tables and original data
// addresses/imports. Never linked into, copied to or executed by the C build.
import {readFile,writeFile,mkdir} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const original=await readFile(path.join(root,'build/smoke-game/Pharaoh.exe'));
const hash=bytes=>createHash('sha256').update(bytes).digest('hex');
if(hash(original)!=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d') throw new Error('Wrong original');
const pe=original.readUInt32LE(0x3c),opt=pe+24,table=opt+original.readUInt16LE(pe+20);
const chunks=[Buffer.from(original.subarray(0,0x1000))];
const header=chunks[0];
header.writeUInt32LE(0x14cd0,opt+16);
header.writeUInt32LE(0x14000,opt+20);
header.writeUInt32LE(0x3000,opt+4);
let raw=0x1000;
const mappings=[];
for(let i=0;i<4;i++) {
 const p=table+i*40;
 const oldRva=original.readUInt32LE(p+12),oldRaw=original.readUInt32LE(p+20),oldSize=original.readUInt32LE(p+16);
 let payload;
 if(i===0) {
  payload=Buffer.alloc(0x3000);
  original.copy(payload,0xcd0,oldRaw+0x14cd0-oldRva,oldRaw+0x168d0-oldRva);
  header.writeUInt32LE(0x28d0,p+8); header.writeUInt32LE(0x14000,p+12); header.writeUInt32LE(payload.length,p+16);
 } else payload=original.subarray(oldRaw,oldRaw+oldSize);
 header.writeUInt32LE(raw,p+20); raw+=payload.length;
 chunks.push(payload);
 mappings.push({section:original.subarray(p,p+8).toString('ascii').replace(/\0.*$/,''),virtual_address:header.readUInt32LE(p+12),bytes:payload.length});
}
const result=Buffer.concat(chunks),directory=path.join(root,'build/callback-analysis');
await mkdir(directory,{recursive:true});
await writeFile(path.join(directory,'window-procedure-analysis.exe'),result);
await writeFile(path.join(directory,'window-procedure-analysis.json'),JSON.stringify({analysis_only:true,never_execute:true,original_sha256:hash(original),sha256:hash(result),code_range:['0x00414cd0','0x004168cf'],entry:'0x00414cd0',mappings},null,2)+'\n');
console.log(JSON.stringify({bytes:result.length,sha256:hash(result),analysis_only:true}));
