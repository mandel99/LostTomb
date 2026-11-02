// Shared observation addresses for offline C/native callback comparison.
import {readFile,writeFile} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'),fields=new Map();
for(const filename of ['runtime/window_messages.c','runtime/display_configuration.c']) {
 const text=await readFile(path.join(root,filename),'utf8');
 for(const match of text.matchAll(/\b([BWIUSH])\((0x[0-9a-f]+)\)/gi)) {
  const address=parseInt(match[2],16),width=match[1]==='B'?1:['W','S'].includes(match[1])?2:4;
  fields.set(address,width);
 }
}
for(const [address,width] of [[0x6fdc54,4],[0xe92e2c,4],[0xe92e30,4],[0xe92e34,4],[0xe92e38,4],
 [0x7999e8+7*264,2],[0xbcb9cc+4*144,2],[0xbcba46+4*144,2],[0xbcba3f+4*144,1],
 [0xbcb9f2+4*144,2],[0xbcba2a+4*144,2],[0xe39130,4],[0xe39134,4],[0xe39138,4],[0xe3913c,4]]) fields.set(address,width);
const rows=[...fields].sort((a,b)=>a[0]-b[0]);
await writeFile(path.join(root,'tests/window_oracle_fields.h'),'/* Generated observation data, not reference behavior. */\nstatic const struct {uintptr_t address;unsigned int width;} oracle_fields[]={\n'+rows.map(([a,w])=>` {0x${a.toString(16)}u,${w}}`).join(',\n')+'\n};\n');
console.log('Window comparison observes '+rows.length+' memory fields.');
