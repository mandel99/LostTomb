"""Inspect native schema/migration data without executing the original program."""
from pathlib import Path
import hashlib,json,struct
ROOT=Path(__file__).resolve().parent.parent;data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(data).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',data,p)[0];u32=lambda p:struct.unpack_from('<I',data,p)[0]
pe=u32(0x3c);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20)
sections=[(u32(table+i*40+12),u32(table+i*40+16),u32(table+i*40+20)) for i in range(u16(pe+6))]
def words(a,count):
 for va,size,raw in sections:
  if va<=a-base and a-base+count*4<=va+size:return struct.unpack_from('<'+'I'*count,data,raw+a-base-va)
 raise ValueError(hex(a))
result={};callbacks=set()
for kind,address,count in [('save',0x5e09a0,181),('map',0x5e17d8,24)]:
 entries=[]
 for index in range(count):
  codec,destination,size,rows,migration=words(address+index*20,5);versions=[]
  if migration:
   for version_index in range(40):
    old_size,old_rows,cutoff,callback=words(migration+version_index*16,4)
    if not cutoff:break
    callbacks.add(callback);versions.append({'size':old_size,'count':old_rows,'cutoff':cutoff,'callback':hex(callback)})
   else:raise ValueError('Unterminated migration list')
  entries.append({'index':index,'codec':codec,'destination':hex(destination),'size':size,'count':rows,'migrations':versions})
 result[kind]={'address':hex(address),'entries':entries}
report={'native_sha256':hashlib.sha256(data).hexdigest(),'schemas':result,'callbacks':[hex(a) for a in sorted(callbacks)]}
(ROOT/'metadata/native-chunk-schemas.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'callbacks':report['callbacks'],'migrated_chunks':{kind:sum(bool(e['migrations']) for e in value['entries']) for kind,value in result.items()}},indent=2))
