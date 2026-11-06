"""Read-only native/C scalar-view review; never edits or certifies C behavior.

Only active canonical providers whose MAP owner matches the catalogue source
are considered. Native widths come from exact pinned instruction bytes, not
Ghidra DAT types. Different widths and address-taking are review leads: shared
overlays, narrowing consumers and explicit casts may make existing C correct.
"""
from pathlib import Path
import argparse,collections,hashlib,json,re,sys
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c'),str(ROOT/'direct_c/cohort3_replay')]
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_AC_READ,CS_AC_WRITE
from capstone.x86_const import X86_OP_MEM
from native_test_pe import PE
from registry import symbol_index
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
SIZES={'undefined':1,'undefined1':1,'undefined2':2,'undefined3':4,'undefined4':4,'undefined8':8,'bool':1,
 'byte':1,'uchar':1,'char':1,'unsigned char':1,'signed char':1,
 'short':2,'ushort':2,'unsigned short':2,'signed short':2,'word':2,
 'int':4,'uint':4,'unsigned int':4,'long':4,'unsigned long':4,'dword':4,
 'int8_t':1,'uint8_t':1,'int16_t':2,'uint16_t':2,'int32_t':4,'uint32_t':4,
 'int64_t':8,'uint64_t':8,'longlong':8,'ulonglong':8}
ALIAS=re.compile(r'^#define\s+((?:_?DAT)_([0-9a-fA-F]+))\s+([^\r\n]+)',re.M)
SCALAR=re.compile(r'^\(\s*\*\s*\(\s*(?:volatile\s+)?([\w ]+?)\s*\*\s*\)\s*(0x[0-9a-fA-F]+)[uUlL]*\s*\)$')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def ref(p):return {'path':p.relative_to(ROOT).as_posix(),'sha256':sha(p)}

def aliases(text):
 result={}
 for match in ALIAS.finditer(text):
  name,address,expression=match.groups();shape=SCALAR.fullmatch(expression.strip())
  if not shape:continue
  kind=' '.join(shape[1].split());width=SIZES.get(kind)
  if not width:continue
  at=int(address,16);assert int(shape[2],16)==at,(name,expression)
  uses=[]
  for line_no,line in enumerate(text.splitlines(),1):
   if line.lstrip().startswith('#'):continue
   if re.search(r'\b'+re.escape(name)+r'\b',line):
    taking=bool(re.search(r'&\s*'+re.escape(name)+r'\b',line))
    uses.append(dict(line=line_no,address_taken=taking,text=line.strip()[:200]))
  if uses:result[name]=dict(name=name,address=at,declared_type=kind,declared_unit_bytes=width,uses=uses,
    only_address_taken=all(u['address_taken']for u in uses))
 return result

def groups(rows,limit):
 # Planning by shared suspect storage, then by cached direct calls. Splits are
 # explicit; a connected component is not proof of native dataflow equivalence.
 parent={r['address']:r['address']for r in rows};by_global={}
 def find(a):
  while parent[a]!=a:parent[a]=parent[parent[a]];a=parent[a]
  return a
 def union(a,b):
  a,b=find(a),find(b)
  if a!=b:parent[max(a,b)]=min(a,b)
 for row in rows:
  for v in row['scalar_width_leads']:
   address=v['global_address'];other=by_global.setdefault(address,row['address']);union(other,row['address'])
 for row in rows:
  for callee in row['cached_direct_callees']:
   at=int(callee,16)if isinstance(callee,str)else None
   target=f'0x{at:08x}'if at is not None else None
   if target in parent:union(row['address'],target)
 components=collections.defaultdict(list)
 for a in parent:components[find(a)].append(a)
 result=[]
 for component,addresses in sorted(components.items()):
  addresses.sort()
  for start in range(0,len(addresses),limit):
   result.append(dict(component=component,component_size=len(addresses),
    split=len(addresses)>limit,addresses=addresses[start:start+limit]))
 return result

def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--inventory',type=Path,required=True)
 p.add_argument('--map',type=Path,default=ROOT/'build/c-only/Release/PharaohRestoreC.map')
 p.add_argument('--binary',type=Path,default=ROOT/'build/smoke-game/Pharaoh.exe')
 p.add_argument('--report',type=Path,default=ROOT/'build/bulk-reconstruction/scalar-view-review.json')
 p.add_argument('--batch-size',type=int,default=100)
 args=p.parse_args();assert 1<=args.batch_size<=100
 assert sha(args.binary)==PIN
 native=PE(args.binary);index=symbol_index(args.map.read_text());inventory=json.loads(args.inventory.read_bytes())
 inputs=[ref(args.inventory.resolve()),ref(args.map.resolve())];cached={}
 for path in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
  data=json.loads(path.read_bytes());assert data['native_sha256']==PIN
  for entry in data['entries']:
   address=int(entry['requested_address'],16)
   if address in cached:raise AssertionError(('duplicate cached entry',hex(address)))
   cached[address]=(entry,ref(path))
 dis=Cs(CS_ARCH_X86,CS_MODE_32);dis.detail=True
 rows=[];canonical=0;with_aliases=0;address_taking=0;unavailable=[]
 for item in inventory['functions']:
  source=item.get('source');address=int(item['address'],16)
  if not source or not item.get('source_active'):continue
  hits=index.get(f'FUN_{address:08x}',[])
  if len(hits)!=1 or Path(source).stem+'.obj'not in hits[0][1]:continue
  canonical+=1;path=ROOT/source;views=aliases(path.read_text())
  if not views:continue
  with_aliases+=1;address_taking+=sum(v['only_address_taken']for v in views.values())
  native_addresses={v['address']for v in views.values()}
  member,cache=cached[address];info=member.get('info',{});assembly=member.get('assembly',{}).get('items',[])
  if int(info.get('address','0'),16)!=address or not assembly or int(assembly[0]['address'],16)!=address:
   unavailable.append(dict(address=hex(address),reason='Native provider scope starts elsewhere'));continue
  observed=collections.defaultdict(list)
  for pos,inst in enumerate(assembly):
   at=int(inst['address'],16);raw=bytes.fromhex(inst['bytes'])
   assert native.data[at-native.base:at-native.base+len(raw)]==raw,hex(at)
   decoded=list(dis.disasm(raw,at));assert len(decoded)==1 and decoded[0].size==len(raw),hex(at)
   ins=decoded[0]
   for operand in ins.operands:
    if operand.type!=X86_OP_MEM or operand.mem.segment:continue
    displacement=operand.mem.disp&0xffffffff
    if displacement not in native_addresses:continue
    indexed=bool(operand.mem.base or operand.mem.index)
    observed[displacement].append(dict(pc=hex(at),bytes=raw.hex(),instruction=inst['instruction'],
     native_access_bytes=operand.size,indexed=indexed,native_reads=bool(operand.access&CS_AC_READ),
     native_writes=bool(operand.access&CS_AC_WRITE),
     next_instructions=[x['instruction']for x in assembly[pos+1:pos+4]]))
  leads=[];indexed_leads=[]
  for view in views.values():
   at=view['address']
   seen=observed.get(at,[])
   scalar=[s for s in seen if not s['indexed']]
   mismatched=[s for s in scalar if s['native_access_bytes']!=view['declared_unit_bytes']]
   if mismatched and not view['only_address_taken']:
    leads.append(dict(global_address=hex(at),**view,native_widths=sorted({s['native_access_bytes']for s in scalar}),
     overlapping_C_aliases=[dict(name=v['name'],declared_type=v['declared_type'],declared_unit_bytes=v['declared_unit_bytes'])for v in views.values()if v['address']==at and v['name']!=view['name']],
     observations=mismatched,classification='review_consumed_width_and_C_expression_before_edit'))
   if any(u['address_taken']for u in view['uses'])and any(s['indexed']for s in seen):
    indexed_leads.append(dict(global_address=hex(at),**view,observations=[s for s in seen if s['indexed']],
     classification='address_unit_review_only_no_automatic_scaling_claim'))
  if leads or indexed_leads:
   rows.append(dict(address=f'0x{address:08x}',source=source,source_sha256=sha(path),
    canonical_provider=dict(va=int(hits[0][0],16),owner=hits[0][1].strip()),native_cache=cache,
    scalar_width_leads=leads,indexed_address_unit_leads=indexed_leads,
    existing_category=item['category'],selected_by=item.get('selected_by',[]),
    cached_direct_callees=item.get('cached_direct_callees',[]),C_changes=0))
 result=dict(schema='native-C-scalar-view-review-v1',native_sha256=PIN,inputs=inputs,
  canonical_units_examined=canonical,canonical_units_with_scalar_aliases=with_aliases,
  address_only_aliases_not_treated_as_scalar_reads=address_taking,
  functions_with_review_leads=len(rows),scalar_width_leads=sum(len(r['scalar_width_leads'])for r in rows),
  indexed_address_unit_leads=sum(len(r['indexed_address_unit_leads'])for r in rows),
  review_batches=groups(rows,args.batch_size),unavailable_native_entry_scopes=unavailable,functions=rows,
  interpretation='Read-only leads. Native memory width, DAT declaration unit, consumed result width and C pointer scaling are separate facts. A differing observed width never authorizes a replacement or widened global by itself.',
  behavior_equivalence_proved=False,source_changed=False,new_source_credit=0,host_execution=False)
 args.report.parent.mkdir(parents=True,exist_ok=True)
 args.report.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8',newline='\n')
 print(json.dumps({k:result[k]for k in ['canonical_units_examined','canonical_units_with_scalar_aliases','functions_with_review_leads','scalar_width_leads','indexed_address_unit_leads','source_changed','behavior_equivalence_proved']}|{'report':str(args.report)}))

if __name__=='__main__':main()
