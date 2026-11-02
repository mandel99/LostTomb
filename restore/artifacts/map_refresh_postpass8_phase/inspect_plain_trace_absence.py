"""Read-only COFF check: traced owners referencehooks, plain owners reference none."""
from pathlib import Path
import hashlib,json,struct
H=Path(__file__).resolve().parent;R=H.parent.parent;report=[]
for cfg in ['Debug','Release']:
 for kind in ['trace','plain']:
  for unit,expected in [('01099',{'_map_post8_read','_map_post8_write','_map_post8_helper'}),('01233',{'_map_post2_read','_map_post2_write'}),('01139',{'_map_neighbors_read','_map_neighbors_write'})]:
   p=H/'build'/('map_post8_'+kind+'.dir')/cfg/('unit_'+unit+'.obj');b=p.read_bytes();assert struct.unpack_from('<H',b)[0]==0x14c;ptr,count=struct.unpack_from('<II',b,8);strings=ptr+count*18;names=set();i=0
   while i<count:
    q=ptr+i*18;name=b[q:q+8]
    if name[:4]==b'\0'*4:
     n=strings+struct.unpack_from('<I',name,4)[0];name=b[n:b.index(0,n)]
    else:name=name.split(b'\0')[0]
    if struct.unpack_from('<h',b,q+12)[0]==0 and b[q+16]==2:names.add(name.decode('ascii'))
    i+=1+b[q+17]
   hooks={n for n in names if n.startswith(('_map_post8_','_map_post2_','_map_neighbors_'))};assert hooks==(expected if kind=='trace' else set()),(p,hooks)
   report.append({'path':p.relative_to(R).as_posix(),'sha256':hashlib.sha256(b).hexdigest(),'configuration':cfg,'kind':kind,'undefined_trace_hook_references':sorted(hooks)})
(H/'plain-trace-coff-check.json').write_text(json.dumps({'passed':True,'original_game_executed':False,'objects':report,'scope':'Undefined external hook references in all3 instrumentation-capable compiledowners. Fixturehookdefinitions do not imply productcalls. OrdinaryCOFF, noLTCG; wholealgorithm equivalence remains the separate9844x4proof.'},indent=2)+'\n');print('12 COFF owner variants passed')
