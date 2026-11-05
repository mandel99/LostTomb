from pathlib import Path
import hashlib,json
ROOT=next(p for p in Path(__file__).resolve().parents if(p/'runtime/actor_civic.c').is_file())
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def admit(path,go):
 path=Path(path).resolve();f=json.loads(path.read_bytes())
 assert go==f['epoch']and f['schema']==2
 assert f['source_frozen']and f['execution_authorized']and f['no_host_game']
 for key,item in [('native',f['native']),*f['compiled_images'].items()]:
  assert digest(ROOT/item['path'])==item['sha256'],key
  if key!='native':assert digest(ROOT/item['map_path'])==item['map_sha256'],key
 for name,sha in f['source_hashes'].items():assert digest(ROOT/name)==sha,('changed source',name)
 snap=ROOT/f['source_snapshot']['path'];assert digest(snap)==f['source_snapshot']['sha256']
 files=json.loads(snap.read_bytes())['files']
 for name,sha in files.items():assert digest(ROOT.parent/name)==sha,('changed source snapshot',name)
 assert len(f['provider_map'])==100
 return f,dict(path=str(path),sha256=digest(path),source_snapshot_sha256=digest(snap),source_files_checked=len(files),classification_counts=f['classification_counts'])
