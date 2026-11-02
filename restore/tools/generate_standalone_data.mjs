// One-time recovery from the authenticated user-owned input. Ordinary C builds
// compile the checked-in generated arrays and never open an original executable.
import {readFile,writeFile,mkdir} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const input=process.argv[2];
if(!input) throw new Error('Supply the canonical original for one-time static data recovery.');
const bytes=await readFile(input);
const sha=createHash('sha256').update(bytes).digest('hex');
if(sha!=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d') throw new Error('Unexpected original digest; no files written.');
const pe=bytes.readUInt32LE(0x3c),optional=pe+24;
if(bytes.readUInt16LE(0)!==0x5a4d||bytes.readUInt32LE(pe)!==0x4550||bytes.readUInt16LE(pe+4)!==0x14c||bytes.readUInt16LE(optional)!==0x10b) throw new Error('Expected PE32 x86.');
const base=bytes.readUInt32LE(optional+28),count=bytes.readUInt16LE(pe+6),table=optional+bytes.readUInt16LE(pe+20);
const sections=[];
for(let i=0;i<count;i++) {
 const offset=table+i*40;
 const name=bytes.subarray(offset,offset+8).toString('ascii').replace(/\0.*$/s,'');
 const virtualSize=bytes.readUInt32LE(offset+8),address=base+bytes.readUInt32LE(offset+12),rawSize=bytes.readUInt32LE(offset+16),rawOffset=bytes.readUInt32LE(offset+20),flags=bytes.readUInt32LE(offset+36);
 if(rawOffset+rawSize>bytes.length) throw new Error('Truncated section');
 sections.push({name,address,size:Math.max(virtualSize,rawSize),rawSize,rawOffset,flags,executable:!!(flags&0x20000000),writable:!!(flags&0x80000000)});
}
const selected=sections.filter(s=>['.rdata','.data','.rsrc'].includes(s.name));
if(selected.length!==3||selected.some(s=>s.executable)) throw new Error('Unexpected data sections');
const units=JSON.parse(await readFile(path.join(root,'units.json'),'utf8'));
const names=new Map(units.map(u=>[Number.parseInt(u.provenance.address,16),u.name]));
const code=sections.filter(s=>s.executable);
const candidates=[];
const hex=n=>'0x'+n.toString(16).padStart(8,'0');
let source='/* Generated non-executable data only. See metadata/standalone-data.json. */\n#include "standalone_data.h"\n';
const descriptors=[];
for(const [index,section] of selected.entries()) {
 const payload=bytes.subarray(section.rawOffset,section.rawOffset+section.rawSize);
 let hash=0x811c9dc5;
 for(const value of payload) hash=Math.imul(hash^value,0x1000193)>>>0;
 section.initial_fnv1a=hex(hash);
 section.sha256=createHash('sha256').update(payload).digest('hex');
 source+=`static const unsigned char segment${index}[${payload.length}]={\n`;
 for(let offset=0;offset<payload.length;offset+=32) source+=Array.from(payload.subarray(offset,offset+32),v=>'0x'+v.toString(16).padStart(2,'0')).join(',')+',\n';
 source+='};\n';
 descriptors.push(` {"${section.name}",${hex(section.address)}u,${hex(section.size)}u,segment${index},sizeof(segment${index}),${hex(hash)}u,${Number(section.writable)}}`);
 // These are conservative candidates, not verified relocations: numeric data
 // can equal code addresses. Never silently patch them by value matching.
 if(section.name!=='.rsrc') for(let offset=0;offset+4<=payload.length;offset+=4) {
  const value=payload.readUInt32LE(offset);
  if(code.some(s=>value>=s.address&&value<s.address+s.size)) candidates.push({location:hex(section.address+offset),value:hex(value),entry:names.get(value)??null,status:'needs typed pointer/ABI review'});
 }
}
const arena=Math.floor(Math.min(...selected.map(s=>s.address))/0x10000)*0x10000;
const end=Math.ceil(Math.max(...selected.map(s=>s.address+s.size))/0x10000)*0x10000;
source+=`const restore_data_segment restore_data_segments[]={\n${descriptors.join(',\n')}\n};\nconst size_t restore_data_segment_count=sizeof(restore_data_segments)/sizeof(restore_data_segments[0]);\nconst uintptr_t restore_data_arena_address=${hex(arena)}u;\nconst size_t restore_data_arena_size=${hex(end-arena)}u;\n`;
await mkdir(path.join(root,'generated'),{recursive:true});
await writeFile(path.join(root,'generated/standalone_data.c'),source);
const report={format:1,original_sha256:sha,build_requires_original:false,runtime_requires_original:false,contains_original_executable_code:false,sections:selected,arena:{address:hex(arena),size:hex(end-arena)},unreviewed_code_pointer_candidates:candidates.length,limitations:['Data initialization is not a complete game runtime.','Original .text and PE headers are excluded.','Embedded code pointer candidates still require typed ABI reconstruction.','This .rsrc copy is data only; Win32 resource registration uses generated/pharaoh_resources.res.'],code_pointer_candidates:candidates};
await writeFile(path.join(root,'metadata/standalone-data.json'),JSON.stringify(report,null,2)+'\n');
console.log(JSON.stringify({sections:selected.map(s=>({name:s.name,initial:s.rawSize,total:s.size})),original_code_bytes:0,code_pointer_candidates:candidates.length}));
