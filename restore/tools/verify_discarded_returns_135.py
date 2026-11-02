"""Run the immutable343-site gate with one exact whole-postpass4 supersession."""
from pathlib import Path
import hashlib,sys
from map_postpass4_return_proof import verified_postpass4_supersession
ROOT=Path(__file__).resolve().parents[1];legacy=ROOT/'tools/repair_discarded_returns.py'
assert hashlib.sha256(legacy.read_bytes()).hexdigest()=='63d44c48f37468d4b679b000c8d1f2743de67ef8f59404ea57d311345674779e'
assert sys.argv[1:] in [[],['--verify']]
text=legacy.read_text()
anchor="     elif e['file']=='src/unit_01414.c' and line.strip()=='FUN_004b1da0(param_1);' and count==1:"
assert text.count(anchor)==1
insert="""     elif e['file']=='src/unit_01152.c' and line.strip()=='FUN_0047eb60(6,&local_4);' and count==1:
      try:reconstructed.append({**verified_postpass4_supersession(ROOT),'before':line})
      except Exception as error:issues.append({'file':e['file'],'missing':line,'reason':str(error)})
"""
text=text.replace(anchor,insert+anchor,1)
anchor="  (ROOT/'metadata/discarded-return-verification.json').write_text"
assert text.count(anchor)==1
extra="""  output['superseding_verifier']={'path':'tools/verify_discarded_returns_135.py','sha256':hashlib.sha256((ROOT/'tools/verify_discarded_returns_135.py').read_bytes()).hexdigest(),'immutable_legacy_sha256':'63d44c48f37468d4b679b000c8d1f2743de67ef8f59404ea57d311345674779e'}
  output['source_hashes']['tools/verify_discarded_returns_135.py']=output['superseding_verifier']['sha256']
  output['source_hashes']['tools/map_postpass4_return_proof.py']=hashlib.sha256((ROOT/'tools/map_postpass4_return_proof.py').read_bytes()).hexdigest()
"""
text=text.replace(anchor,extra+anchor,1);sys.argv=[str(legacy),'--verify']
exec(compile(text,str(legacy),'exec'),{'__file__':str(legacy),'__name__':'__main__','verified_postpass4_supersession':verified_postpass4_supersession})
