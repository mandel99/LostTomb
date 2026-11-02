"""Apply exactly six reviewed binding substitutions to isolated copies only."""
from pathlib import Path
import hashlib,json
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sha=lambda b:hashlib.sha256(b).hexdigest()
FILES={123:'e9321f216647b15478d5465f61cabeeebd3188387dbf41cb9af9403a0be48cbd',124:'cba1730e6bc8e46cc83bb3f6e16627c3b6277fc3ba034379b599e24e14c2fb3c',126:'e2a0e1ac877e99230564cc8162a837b453f6a348f6ddd161968c8d3bca7bd882'}
AUDIT_TOOL='ba7d287c91edd66e220547a6fcee6a695a2eae4831d51e48335539818e5b320d'
AUDIT_REPORT='70ee6889d2dc955d887b29db6290a4d7d9d0549fce37ad35fb38320ea879da56'
assert sha((ROOT/'tools/audit_reconstruction_patterns.py').read_bytes())==AUDIT_TOOL
assert sha((ROOT/'metadata/reconstruction-pattern-audit.json').read_bytes())==AUDIT_REPORT
for directory in ['original','patched']:(HERE/directory).mkdir(exist_ok=True)
plan=[]
for n,want in FILES.items():
    name=f'unit_{n:05}.c';source=(ROOT/'src'/name).read_bytes();assert sha(source)==want,name
    (HERE/'original'/name).write_bytes(source);patched=source;sites=[]
    for global_address in ['00e38d94','00e92e18']:
        before=f'#define DAT_{global_address} (*(undefined *)0x{global_address}u)'.encode()
        after=f'#define DAT_{global_address} (*(uint32_t *)0x{global_address}u)'.encode()
        assert patched.count(before)==1,(name,before)
        line=patched[:patched.index(before)].count(b'\n')+1
        patched=patched.replace(before,after,1)
        sites.append({'line':line,'global':global_address,'before':before.decode(),'after':after.decode()})
    assert sum(a!=b for a,b in zip(source.splitlines(),patched.splitlines()))==2
    (HERE/'patched'/name).write_bytes(patched)
    plan.append({'source':f'src/{name}','original_copy':f'artifacts/scalar_binding_batch_001/original/{name}','patched_copy':f'artifacts/scalar_binding_batch_001/patched/{name}','before_sha256':want,'after_sha256':sha(patched),'sites':sites})
assert sum(len(p['sites']) for p in plan)==6
output={'format':1,'applied_to_product':False,'audit_tool_sha256':AUDIT_TOOL,'audit_report_sha256':AUDIT_REPORT,'transform':'Exactly two explicit undefined BYTE to uint32_t DWORD binding type substitutions per file; every other byte preserved.','plan':plan,'total_sites':6}
(HERE/'six_site_plan.json').write_text(json.dumps(output,indent=2)+'\n')
print(json.dumps(output,indent=2))
