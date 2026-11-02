// Static layout recovery from retained disassembly, never executing native code.
// Model byte origins through the one straight-line record conversion. Reject
// every unrecognized instruction and every read of an unknown register byte.
import {readFile,writeFile} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const assembly=await readFile(path.join(root,'metadata/city-migration-native.asm'),'utf8');
const instructions=[...assembly.matchAll(/^  ([0-9A-F]{8}):\s+(.+)$/gm)].map(m=>({address:parseInt(m[1],16),text:m[2].trim().replace(/\s+/g,' ')}));
if(!instructions.some(i=>i.address===0x4d1add&&i.text==='xor ebp,ebp')) throw new Error('Unverified zero register');
const size=0x49d8,output=Array(size).fill(null),registers={eax:Array(4).fill(null),ecx:Array(4).fill(null),ebp:Array(4).fill(-1)};
const pointers={};
const number=text=>text.endsWith('h')?parseInt(text.slice(0,-1),16):Number(text);
const alias=register=>({eax:['eax',4],ax:['eax',2],al:['eax',1],ecx:['ecx',4],cx:['ecx',2],cl:['ecx',1],ebp:['ebp',4]})[register];
const widths={dword:4,word:2,byte:1};
let count=null,finished=false,processed=0,scalarStores=0;
function transfer(destination,origins) {
 if(destination<0||destination+origins.length>size||origins.some(x=>x===null)) throw new Error('Unknown/out-of-bounds byte transfer');
 origins.forEach((origin,i)=>output[destination+i]=origin);
}
for(const instruction of instructions.filter(i=>i.address>=0x4d1ae4&&i.address<=0x4d2f8c)) {
 const text=instruction.text;
 let match;
 processed++;
 if((match=text.match(/^xor (eax|ecx),\1$/))) {
  registers[match[1]].fill(-1); if(match[1]==='ecx') count=0;
 } else if((match=text.match(/^mov ecx,([0-9A-F]+h?|\d+)$/))) {
  count=number(match[1]); registers.ecx=Array(4).fill(null);
 } else if((match=text.match(/^mov (eax|ecx|ax|cx|al|cl),(dword|word|byte) ptr \[edx\+([0-9A-F]+h?)\]$/))) {
  const [reg,width]=alias(match[1]),offset=number(match[3]);
  if(width!==widths[match[2]]||offset+width>0x4958) throw new Error('Unexpected field read');
  for(let i=0;i<width;i++) registers[reg][i]=offset+i;
  if(reg==='ecx') count=null;
 } else if((match=text.match(/^mov (dword|word|byte) ptr \[esp\+([0-9A-F]+h?)\],(eax|ecx|ax|cx|al|cl|ebp)$/))) {
  const [reg,width]=alias(match[3]);
  if(width!==widths[match[1]]) throw new Error('Store width mismatch');
  transfer(number(match[2])-16,registers[reg].slice(0,width)); scalarStores++;
 } else if((match=text.match(/^lea (esi|edi),\[(edx|esp)\+([0-9A-F]+h?)\]$/))) {
  pointers[match[1]]={space:match[2]==='edx'?'input':'temp',offset:number(match[3])-(match[2]==='esp'?16:0)};
 } else if(text==='mov esi,edx') pointers.esi={space:'input',offset:0};
 else if(text==='mov edi,edx') pointers.edi={space:'input',offset:0};
 else if(text==='rep stos dword ptr es:[edi]') {
  if(count===null||pointers.edi?.space!=='temp'||registers.eax.some(x=>x!==-1)) throw new Error('Unknown fill at '+instruction.address.toString(16)+' '+JSON.stringify({count,pointers,eax:registers.eax}));
  transfer(pointers.edi.offset,Array(count*4).fill(-1)); pointers.edi.offset+=count*4; count=0; registers.ecx.fill(-1);
 } else if(text==='rep movs dword ptr es:[edi],dword ptr [esi]') {
  if(count===null||!pointers.esi||!pointers.edi) throw new Error('Unknown bulk copy');
  const length=count*4;
  if(pointers.edi.space==='input') {
   if(instruction.address!==0x4d2f8c||pointers.esi.space!=='temp'||pointers.esi.offset!==0||pointers.edi.offset!==0||length!==size) throw new Error('Unexpected copy back');
   finished=true;
  } else {
   if(pointers.esi.space!=='input'||pointers.esi.offset+length>0x4958) throw new Error('Unexpected bulk source');
   transfer(pointers.edi.offset,Array.from({length},(_,i)=>pointers.esi.offset+i));
  }
  pointers.esi.offset+=length; pointers.edi.offset+=length; count=0; registers.ecx.fill(-1);
 } else if(!['imul edx,edx,49D8h','add edx,0EA1870h','inc ebx','cmp ebx,2'].includes(text)) throw new Error('Unmodeled instruction '+instruction.address.toString(16)+': '+text);
}
if(!finished||output.some(x=>x===null)) throw new Error('Incomplete output coverage');
const fields=[];
for(let offset=0;offset<size;) {
 const origin=output[offset]; let end=offset+1;
 while(end<size&&output[end]===(origin<0?-1:origin+end-offset)) end++;
 fields.push({destination:offset,source:origin,bytes:end-offset}); offset=end;
}
const header='/* Static byte-origin layout recovered from 004d1ad0; see city-migration-layout.json. */\n'+fields.map(f=>`    {0x${f.destination.toString(16)},${f.source<0?'-1':'0x'+f.source.toString(16)},0x${f.bytes.toString(16)}}`).join(',\n')+'\n';
await writeFile(path.join(root,'runtime/city_migration_layout.h'),header);
await writeFile(path.join(root,'metadata/city-migration-layout.json'),JSON.stringify({format:1,original_sha256:'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d',retained_disassembly_sha256:createHash('sha256').update(assembly).digest('hex'),method:'static byte-origin dataflow; no native execution',modeled_instructions:processed,scalar_stores:scalarStores,output_bytes:size,field_runs:fields.length,fields},null,2)+'\n');
console.log(JSON.stringify({modeled_instructions:processed,scalar_stores:scalarStores,output_bytes:size,field_runs:fields.length}));
