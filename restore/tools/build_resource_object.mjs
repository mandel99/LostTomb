// Standard Win32 .res framing, with unchanged resource payload bytes.
// https://learn.microsoft.com/en-us/windows/win32/menurc/resourceheader
import {readFile,writeFile} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const align4=value=>(value+3)&~3;
function name(value) {
 if(typeof value==='string') return Buffer.from(value+'\0','utf16le');
 if(!Number.isInteger(value)||value<0||value>65535) throw new Error('Invalid resource ordinal');
 const bytes=Buffer.alloc(4); bytes.writeUInt16LE(0xffff); bytes.writeUInt16LE(value,2); return bytes;
}
function entry(type,id,language,payload,flags=0x1030) {
 const t=name(type),n=name(id),tail=align4(8+t.length+n.length),headerSize=tail+16;
 const header=Buffer.alloc(headerSize);
 header.writeUInt32LE(payload.length); header.writeUInt32LE(headerSize,4);
 t.copy(header,8); n.copy(header,8+t.length);
 header.writeUInt16LE(flags,tail+4); header.writeUInt16LE(language,tail+6);
 return Buffer.concat([header,payload,Buffer.alloc(align4(payload.length)-payload.length)]);
}
const manifest=JSON.parse(await readFile(path.join(root,'metadata/standalone-resources.json'),'utf8'));
const entries=[entry(0,0,0,Buffer.alloc(0),0)];
for(const [i,resource] of manifest.resources.entries()) {
 const filename='resource-'+String(i).padStart(3,'0')+'.bin';
 const payload=await readFile(path.join(root,'generated/resources',filename));
 if(payload.length!==resource.size) throw new Error('Resource size mismatch: '+filename);
 let hash=0x811c9dc5; for(const byte of payload) hash=Math.imul(hash^byte,0x1000193)>>>0;
 if(hash!==resource.fnv1a) throw new Error('Resource bytes changed: '+filename);
 entries.push(entry(resource.type,resource.name,resource.language,payload));
}
await writeFile(path.join(root,'generated/pharaoh_resources.res'),Buffer.concat(entries));
console.log('Built resource object from '+manifest.resources.length+' retained payloads; no original EXE read.');
