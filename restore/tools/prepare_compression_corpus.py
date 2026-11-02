"""Extract bounded licensed block fixtures, without changing game or save files."""
from pathlib import Path
import hashlib,json,struct
ROOT=Path(__file__).resolve().parent.parent
out=ROOT/'artifacts/compression-fixtures';out.mkdir(exist_ok=True)
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
def scan(data,label):
 version,count=u32(data,4),u32(data,8);assert 1<=count<=300
 descriptors=[struct.unpack_from('<5I',data,12+i*20) for i in range(count)]
 schema=next(i for i,d in enumerate(descriptors) if d[0]==0 and d[2:4]==(20,300));position=6012;blocks=[]
 for i in range(schema+1,count):
  codec,address,size,number,migration=descriptors[i];expected=size*number;assert codec in [0,1] and expected<=1200000
  if codec:
   marker=u32(data,position);position+=4
   stored=expected if marker==0x80000000 else marker;assert stored<=1200000
  else:stored=expected;marker=None
  assert position+stored<=len(data),(label,i,position,stored,len(data))
  if codec and marker not in [0,0x80000000] and expected<=250000:
   blocks.append({'origin':label,'version':version,'chunk':i,'destination':hex(address),'capacity':expected,'data':data[position:position+stored]})
  position+=stored
 assert position==len(data),(label,position,len(data))
 return blocks
pak_path=ROOT/'build/debug-startup-game/mission1.pak';pak=pak_path.read_bytes();table=u32(pak,0)
offsets=[u32(pak,i) for i in range(0,table,4) if u32(pak,i)]
blocks=[]
for index in [0,1,len(offsets)-1]:
 start=offsets[index];end=offsets[index+1] if index+1<len(offsets) else len(pak)
 blocks+=scan(pak[start:end],f'mission1.pak entry {index}')[0:12]
save_path=ROOT/'build/smoke-game/Save/Khensthoth/autosave.sav';save=save_path.read_bytes();blocks+=scan(save,'owned smoke-game autosave')[0:12]
entries=[]
for index,block in enumerate(blocks):
 payload=block.pop('data');path=out/f'{index:03d}.bin';path.write_bytes(payload)
 entries.append(dict(block,path=str(path.relative_to(ROOT)).replace('\\','/'),input_length=len(payload),sha256=hashlib.sha256(payload).hexdigest()))
report={'method':'Read-only parsing of serialized 20-byte descriptors; exact encoded bytes extracted from licensed local assets and the existing isolated smoke-game save.',
 'sources':{str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in [pak_path,save_path]},'entries':entries}
(ROOT/'metadata/compression-corpus.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'blocks':len(entries),'versions':sorted(set(e['version'] for e in entries))}))
