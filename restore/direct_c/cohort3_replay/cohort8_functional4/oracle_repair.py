"""Snapshot-free overlay: correct only six authored final WORD cells."""
from pathlib import Path
import copy,json
OUT=Path(__file__).resolve().parent
IDS={'c8-functional4/004c42b0/house-kind10-WORD-food-and-resource-switch':0,
     'c8-functional4/004c42b0/house-kind29-phase2-WORD-wrap':2}
BUILDING=0x7999d8+256*264


def initial_value(case,address,width):
 found=[]
 for start,data in case['stores']:
  if start<=address and address+width<=start+len(data):found.append(int.from_bytes(data[address-start:address-start+width],'little'))
 assert len(found)==1,(hex(address),width,len(found))
 return found[0]


def derive(case):
 """Native table dispatch + original seed RAM; never reads an actual report."""
 proof=json.loads((OUT/'derivation.json').read_bytes())
 phase=initial_value(case,BUILDING+3,1);level=initial_value(case,BUILDING+18,2)
 assert phase==IDS[case['id']]and level==2
 out=[]
 for row in proof['native_goods_dispatch']:
  index=row['stock_index'];address=BUILDING+84+index*2
  assert initial_value(case,address,2)==1
  table=row['table_base']+level*88
  value=initial_value(case,table,4)
  expected=((value*(8 if phase>1 else 2))&0xffffffff)&0xffff
  out.append(dict(address=address,width=2,index=index,resource=row['resource'],table=table,seed_value=value,expected=expected))
 return out


def correct_expected_final_memory(cases):
 result=copy.deepcopy(cases);seen=set()
 for case in result:
  if case['id']not in IDS:continue
  seen.add(case['id']);patches=[]
  for row in derive(case):
   address=row['address'];wanted=row['expected']
   for i,(base,payload)in enumerate(case['authored_final_memory']):
    raw=bytearray.fromhex(payload)
    if base<=address and address+2<=base+len(raw):
     previous=int.from_bytes(raw[address-base:address-base+2],'little')
     if previous!=wanted:
      assert row['index']in(8,10,11)
      raw[address-base:address-base+2]=wanted.to_bytes(2,'little')
      case['authored_final_memory'][i]=(base,raw.hex());patches.append(address)
     else:assert row['index']==9
     break
   else:raise AssertionError(('missing expected block',hex(address)))
  assert patches==[BUILDING+100,BUILDING+104,BUILDING+106]
 assert seen==set(IDS)
 return result


# Public integration API requested by root; same deep-copy-only overlay.
correct_cases=correct_expected_final_memory


def delta_only(original,corrected):
 assert len(original)==len(corrected);changes=[]
 for old,new in zip(original,corrected):
  assert old['id']==new['id']
  if old==new:continue
  assert old['id']in IDS
  assert {k:v for k,v in old.items()if k!='authored_final_memory'}=={k:v for k,v in new.items()if k!='authored_final_memory'}
  cell_changes=[]
  for (address,before),(address2,after)in zip(old['authored_final_memory'],new['authored_final_memory']):
   assert address==address2 and len(before)==len(after)
   a,b=bytes.fromhex(before),bytes.fromhex(after)
   allowed={byte for row in derive(old)if row['index']in(8,10,11)for byte in range(row['address'],row['address']+2)}
   assert all(address+i in allowed for i,(x,y)in enumerate(zip(a,b))if x!=y)
  for row in derive(old):
   if row['index']==9:continue
   at=row['address'];before=next(int.from_bytes(bytes.fromhex(data)[at-base:at-base+2],'little')for base,data in old['authored_final_memory']if base<=at and at+2<=base+len(bytes.fromhex(data)))
   cell_changes.append(dict(address=hex(at),width=2,before=before,after=row['expected'],stock_index=row['index']))
  changes.append(dict(id=old['id'],field='authored_final_memory',cells=cell_changes))
 assert len(changes)==2 and sum(len(v['cells'])for v in changes)==6
 return changes
