// Recover resources as ordinary payload inputs for a Win32 resource object.
// No executable headers/code are copied into the new program.
import {readFile,writeFile,mkdir} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
if(!process.argv[2]) throw new Error('Supply canonical original for one-time resource recovery.');
const bytes=await readFile(process.argv[2]);
const sha=createHash('sha256').update(bytes).digest('hex');
if(sha!=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d') throw new Error('Unexpected original digest');
const pe=bytes.readUInt32LE(0x3c),opt=pe+24,table=opt+bytes.readUInt16LE(pe+20),count=bytes.readUInt16LE(pe+6);
const sections=Array.from({length:count},(_,i)=>{const p=table+i*40;return {rva:bytes.readUInt32LE(p+12),raw:bytes.readUInt32LE(p+20),size:bytes.readUInt32LE(p+16),flags:bytes.readUInt32LE(p+36)};});
function offset(rva,size) {
 const section=sections.find(s=>rva>=s.rva&&rva-s.rva+size<=s.size);
 if(!section||(section.flags&0x20000000)) throw new Error('Resource outside non-executable file-backed data');
 return section.raw+rva-section.rva;
}
const rva=bytes.readUInt32LE(opt+112),resourceSize=bytes.readUInt32LE(opt+116),resourceRoot=offset(rva,resourceSize);
const resources=[],visited=new Set();
function stringAt(value) {
 if(!(value&0x80000000)) return value;
 const p=resourceRoot+(value&0x7fffffff),length=bytes.readUInt16LE(p);
 if(p+2+length*2>resourceRoot+resourceSize) throw new Error('Resource string bounds');
 return bytes.subarray(p+2,p+2+length*2).toString('utf16le');
}
function walk(relative,keys) {
 if(visited.has(relative)||keys.length>2||relative+16>resourceSize) throw new Error('Invalid resource tree');
 visited.add(relative);
 const p=resourceRoot+relative,n=bytes.readUInt16LE(p+12)+bytes.readUInt16LE(p+14);
 if(relative+16+n*8>resourceSize) throw new Error('Resource entry bounds');
 for(let i=0;i<n;i++) {
  const entry=p+16+i*8,key=stringAt(bytes.readUInt32LE(entry)),value=bytes.readUInt32LE(entry+4);
  if(value&0x80000000) walk(value&0x7fffffff,[...keys,key]);
  else {
   if(keys.length!==2||typeof key!=='number'||value+16>resourceSize) throw new Error('Expected type/name/language leaf');
   const data=resourceRoot+value,dataRva=bytes.readUInt32LE(data),size=bytes.readUInt32LE(data+4),codepage=bytes.readUInt32LE(data+8);
   const payload=bytes.subarray(offset(dataRva,size),offset(dataRva,size)+size);
   let fnv=0x811c9dc5; for(const b of payload) fnv=Math.imul(fnv^b,0x1000193)>>>0;
   resources.push({type:keys[0],name:keys[1],language:key,size,codepage,fnv1a:fnv,sha256:createHash('sha256').update(payload).digest('hex'),payload});
  }
 }
}
walk(0,[]);
await mkdir(path.join(root,'generated/resources'),{recursive:true});
const ckey=value=>typeof value==='number'?`MAKEINTRESOURCEW(${value})`:'L'+JSON.stringify(value);
let header='/* Generated resource identities and byte checksums. */\nstatic const struct {LPCWSTR type,name; WORD language; DWORD bytes,fnv1a;} resources[]={\n';
for(const [i,resource] of resources.entries()) {
 const name=`resource-${String(i).padStart(3,'0')}.bin`;
 await writeFile(path.join(root,'generated/resources',name),resource.payload);
 header+=` {${ckey(resource.type)},${ckey(resource.name)},${resource.language},${resource.size},0x${resource.fnv1a.toString(16)}u},\n`;
}
header+='};\n';
await writeFile(path.join(root,'generated/resource_manifest.h'),header);
await writeFile(path.join(root,'metadata/standalone-resources.json'),JSON.stringify({format:1,original_sha256:sha,original_executable_required_for_build:false,original_executable_required_at_runtime:false,resources:resources.map(({payload,...resource})=>resource)},null,2)+'\n');
await import('./build_resource_object.mjs');
console.log('Recovered '+resources.length+' resources as ordinary resource-object inputs.');
