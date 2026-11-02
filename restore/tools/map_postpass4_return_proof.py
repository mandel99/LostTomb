"""Fail-closed recognition of the fully reconstructed47E870 caller/count writer."""
from pathlib import Path
import hashlib,json,xml.etree.ElementTree as ET
PROOF='metadata/map-refresh-postpass4-emulation.json'
MANIFEST='artifacts/map_refresh_postpass4_phase/postpass4_apply_map.json'
VERIFIER='artifacts/map_refresh_postpass4_phase/verify_postpass4.py'
NATIVE='artifacts/map_refresh_postpass4_phase/postpass4-native.json'
def verified_postpass4_supersession(root,proof=None):
 root=Path(root);sha=lambda p:hashlib.sha256((root/p).read_bytes()).hexdigest()
 if proof is None:proof=json.loads((root/PROOF).read_text())
 assert proof['passed'] is True and proof['production'] is True and proof['product_applied'] is True
 assert proof['checked']==proof['complete_cases']==4674 and proof['comparisons']==18696
 assert proof['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d' and proof['original_game_executed'] is False
 assert proof['coverage']=={'0047e5c0':[101,101],'0047eb60':[85,85],'0047e870':[198,198]}
 assert proof['source_drift']==proof['binary_drift']==[]
 assert proof['counter_changes']=={'increment':312,'wrap':104} and all(proof['routes'][str(n)]>0 for n in range(3))
 assert len(proof['branches'])==12 and all(n>0 for n in proof['branches'].values())
 assert sha(MANIFEST)=='2f2def7be2230355a51d941905c82a141aba2d26a9e276a24d686c63bd4de207'
 assert sha(VERIFIER)=='7004f508e3069ba6ea9347d2c05b3fc829f3e4f9060bf661dd9465c2266cdd5f'
 assert proof['native_exports']=={NATIVE:'688243b47be458f3ae9c724519e1174f2d19e8a6193a3903d6153b34d3ba9465'}
 manifest=json.loads((root/MANIFEST).read_text());assert proof['corpus_sha256']==manifest['corpus_sha256']
 required={'src/unit_01149.c','src/unit_01152.c','src/unit_01153.c','include/map_refresh_postpass4.h',VERIFIER}
 assert required<=proof['source_hashes'].keys()
 consumed={**proof['source_hashes'],**proof['native_exports']}
 for p,h in consumed.items():assert sha(p)==h,('postpass4 evidence drift',p)
 for m in manifest['mappings']:assert sha(m['staged'])==sha(m['target'])==m['staged_sha256']
 assert sha('src/unit_01152.c')=='59951cea202f1b0e90e5ae9b84612057bb15546fbc40992369c70b877f9f87b9'
 assert (root/'src/unit_01152.c').read_text().count('FUN_0047eb60(6,&count);')==1
 assert 'void FUN_0047eb60(int32_t direction,uint32_t *count);' in (root/'include/map_refresh_postpass4.h').read_text()
 ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};expected=set()
 for cfg,directory,prefix in [('Debug','c-only-debug','Debug'),('Release','c-only','O2')]:
  for kind in ['trace','plain']:
   target='restore_map_refresh_postpass4'+('_plain' if kind=='plain' else '')+'_verify'
   path=f'build/{directory}/{cfg}/{target}.exe';v=proof['variants'][prefix+'_'+kind]
   assert v['path']==path and v['checked']==4674 and sha(path)==v['sha256']
   project=root/'build'/directory/(target+'.vcxproj');tree=ET.parse(project)
   groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
   cc=groups[0].find('m:ClCompile',ns);get=lambda name:cc.findtext('m:'+name,default='',namespaces=ns)
   assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
   assert ('MAP_REFRESH_POSTPASS4_TRACE' in get('PreprocessorDefinitions'))==(kind=='trace')
   actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
   assert actual=={root/p for p in ['artifacts/map_refresh_postpass4_phase/postpass4_fixture.c','src/unit_01149.c','src/unit_01152.c','src/unit_01153.c']}
   for source in actual|{project,root/'include/map_refresh_postpass4.h'}:assert (root/path).stat().st_mtime_ns>=source.stat().st_mtime_ns
   consumed[path]=v['sha256'];consumed[project.relative_to(root).as_posix()]=sha(project.relative_to(root).as_posix());expected.add(prefix+'_'+kind)
 assert set(proof['variants'])==expected and proof['compiled_C_sha256']==proof['variants']['Debug_trace']['sha256']
 consumed.update({PROOF:sha(PROOF),MANIFEST:sha(MANIFEST),'tools/map_postpass4_return_proof.py':sha('tools/map_postpass4_return_proof.py')})
 return {'file':'src/unit_01152.c','function':'0x0047e870','replacement_kind':'complete-procedure','whole_procedure_proven':True,
  'evidence':PROOF,'evidence_sha256':sha(PROOF),'cases':4674,'comparisons':18696,'whole_group_instruction_coverage':[384,384],
  'historical_call':'FUN_0047eb60(6,&local_4);','actual_call':'FUN_0047eb60(6,&count);',
  'return_contract':'Native caller ignores helper EAX; actual helper updates the DWORD counter/output flags. Full real caller198+helper85+peer101 native instructions are compared in four compiled variants.',
  'whole_map_refresh_complete':False,'verified_source_and_evidence_hashes':consumed,'limitations':proof['limitations']}
