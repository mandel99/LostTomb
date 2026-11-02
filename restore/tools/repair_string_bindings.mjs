// Mechanical correction to generated data bindings only. A Ghidra s_* symbol
// denotes an array. The old scalar-char lvalue passed its first byte to APIs.
// Emit an apply_patch diff; never overwrite procedure bodies or source files.
import {readFile,writeFile} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
import {createHash} from 'node:crypto';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const original=await readFile(path.join(root,'build/smoke-game/Pharaoh.exe'));
if(createHash('sha256').update(original).digest('hex')!=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d') throw new Error('Wrong original');
const pe=original.readUInt32LE(0x3c),opt=pe+24,base=original.readUInt32LE(opt+28),table=opt+original.readUInt16LE(pe+20);
const sections=Array.from({length:original.readUInt16LE(pe+6)},(_,i)=>{const p=table+i*40;return {rva:original.readUInt32LE(p+12),raw:original.readUInt32LE(p+20),size:original.readUInt32LE(p+16),flags:original.readUInt32LE(p+36)};});
function stringLength(address) {
 const rva=address-base,section=sections.find(s=>rva>=s.rva&&rva<s.rva+s.size);
 if(!section||(section.flags&0x20000000)) throw new Error('String is not initialized non-executable data: '+address.toString(16));
 const p=section.raw+rva-section.rva,end=original.indexOf(0,p);
 if(end<0||end>=section.raw+section.size) throw new Error('Unbounded string: '+address.toString(16));
 return end-p+1;
}
const units=JSON.parse(await readFile(path.join(root,'units.json'),'utf8'));
const files=[];
for(const unit of units) {
 const source=await readFile(path.join(root,unit.file),'utf8'),changes=[];
 for(const match of source.matchAll(/^#define (s_\w+) \(\*\(char \*\)(0x([0-9a-f]+)u)\)/gm)) {
  const length=stringLength(parseInt(match[3],16));
  changes.push({name:match[1],address:match[2],bytes:length,old:match[0],replacement:`#define ${match[1]} (*(char (*)[${length}])${match[2]})`});
 }
 if(changes.length) files.push({file:unit.file,changes});
}
if(process.argv.includes('--verify')) {
 const inventory=JSON.parse((await readFile(path.join(root,'metadata/string-binding-repairs.json'),'utf8')).replace(/^\uFEFF/,''));
 const retirements=JSON.parse(await readFile(path.join(root,'metadata/string-binding-retirements.json'),'utf8')).entries;
 const issues=files.map(entry=>({file:entry.file,issue:'scalar string bindings remain',count:entry.changes.length})),hashes={};
 let checked=0,retired=0;
 for(const entry of inventory) {
  const source=await readFile(path.join(root,entry.file),'utf8');
  hashes[entry.file]=createHash('sha256').update(source).digest('hex');
  for(const change of entry.changes) {
   const length=stringLength(parseInt(change.address,16));
   const expected=`#define ${change.name} (*(char (*)[${length}])${change.address})`;
   const retirement=retirements.find(r=>r.file===entry.file&&r.name===change.name);
   let retiredValid=false;
   if(retirement) {
    const address=parseInt(change.address,16)-base,section=sections.find(s=>address>=s.rva&&address<s.rva+s.size);
    const start=section.raw+address-section.rva,literal=original.subarray(start,start+length-1).toString('ascii');
    retiredValid=retirement.address===change.address&&retirement.bytes===length&&retirement.literal===literal&&
     !new RegExp('\\b'+change.name+'\\b').test(source)&&source.includes(JSON.stringify(literal));
    if(retiredValid)++retired;
   }
   if(length!==change.bytes||change.replacement!==expected||(!source.split(/\r?\n/).includes(expected)&&!retiredValid))
    issues.push({file:entry.file,symbol:change.name,issue:'array binding or evidenced length differs'});
   ++checked;
  }
 }
 const report={format:1,generated_at:new Date().toISOString(),passed:issues.length===0,
  original_sha256:createHash('sha256').update(original).digest('hex'),original_executed:false,
  method:'Check every retained repair against current source and initialized non-executable PE data through NUL',
  verified_declarations:checked-retired,retired_to_verified_C_literals:retired,verified_files:inventory.length,remaining_scalar_bindings:files.reduce((n,e)=>n+e.changes.length,0),
  procedure_bodies_replaced:false,source_hashes:hashes,issues};
 await writeFile(path.join(root,'metadata/string-binding-verification.json'),JSON.stringify(report,null,2)+'\n');
 console.log(JSON.stringify({passed:report.passed,declarations:checked-retired,retired_to_verified_C_literals:retired,files:inventory.length,issues}));
 process.exit(report.passed?0:1);
}
const offset=Number(process.argv[2]||0),limit=Number(process.argv[3]||20);
if(process.argv.includes('--inventory')) {console.log(JSON.stringify(files)); process.exit(0);}
let patch='*** Begin Patch\n';
for(const entry of files.slice(offset,offset+limit)) {
 patch+='*** Update File: '+path.join(root,entry.file)+'\n';
 for(const change of entry.changes) patch+='@@\n-'+change.old+'\n+'+change.replacement+'\n';
}
patch+='*** End Patch\n';
process.stdout.write(patch);
