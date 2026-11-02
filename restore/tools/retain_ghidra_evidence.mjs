import {readFile,writeFile,readdir} from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath,pathToFileURL} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const snapshot=JSON.parse(await readFile(path.join(root,'raw/rea/analysis-snapshot.json'),'utf8'));
if(!snapshot.evidence_bundle?.records?.length) throw new Error('Retained snapshot has no Evidence');
const ids=new Set(snapshot.evidence_bundle.records.map(record=>record.evidence_id));
for(const filename of (await readdir(path.join(root,'raw/rea'))).filter(name=>/^batch-\d+\.json$/.test(name))) {
 const record=JSON.parse(await readFile(path.join(root,'raw/rea',filename),'utf8'));
 if(!record.evidence_id) throw new Error('Missing Evidence identity: '+filename);
 if(!ids.has(record.evidence_id)) {snapshot.evidence_bundle.records.push(record);ids.add(record.evidence_id);}
}
const output=path.join(root,'build/retained-ghidra-evidence.json');
const {createImmutableEvidenceBundle}=await import(pathToFileURL(path.join(root,'../PharaohRewrite/tools/rea/node_modules/rea-agents/dist/domain/evidenceBundle.js')));
const bundle=createImmutableEvidenceBundle(snapshot.evidence_bundle.records,snapshot.evidence_bundle.unknowns);
await writeFile(output,JSON.stringify(bundle));
console.log(JSON.stringify({output,records:snapshot.evidence_bundle.records.length}));
