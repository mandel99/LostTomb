"""Read-only pilot for narrow scalar DAT views contradicted by pinned DWORD accesses.

This emits review leads, never source replacements or cohort credit. Only literal
sets, equality tests and self+1 counters are accepted C uses; address/index views,
overlapping native accesses, partial native scopes and unbound frames are skipped.
"""
from pathlib import Path
from collections import Counter
import argparse,hashlib,json,re,sys
ROOT=Path(__file__).resolve().parents[1]
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
sys.path[:0]=[str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM,X86_OP_IMM
COMMENT=re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
DEFINE=re.compile(r'^#define\s+(\w+)\s+\(\*\((undefined|undefined1)\s*\*\)0x([0-9a-fA-F]+)u?\)',re.M)
MARKER=re.compile(r'\bin_(?:E[A-Z]{2}|stack_\w+)|\bunaff_|restore_unresolved|restore_stack|ExceptionList|__asm')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inventory',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--exclude-address',action='append',default=[]);a=p.parse_args()
 out=a.report.resolve();assert out.is_relative_to((ROOT/'build').resolve()),'Report must stay under restore/build'
 inv=json.loads(a.inventory.read_bytes());assert inv['native_sha256']==PIN
 native_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(native_path)==PIN;pe=PE(native_path)
 exclude={int(s,16)for s in a.exclude_address};cache={};rows=[];skipped=Counter();reviewed=0
 cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
 for row in inv['functions']:
  address=int(row['address'],16)
  if address in exclude or row['category']!='existing_C_preserved' or not row.get('source_active')or row['native_evidence']['status']!='complete_pinned':continue
  source=ROOT/row['source'];raw_source=source.read_bytes()
  if sha(source)!=row['source_sha256']:skipped['changed_source']+=1;continue
  text=raw_source.decode('utf-8');definitions=list(DEFINE.finditer(text))
  if not definitions:continue
  body=COMMENT.sub(' ',re.sub(r'^#[^\n]*','',text,flags=re.M))
  if MARKER.search(body):skipped['unbound_or_physical_frame']+=1;continue
  definitions_in_file=[name for name in re.findall(r'(?:^|\n)[^\n;{}#]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{',body)if name not in {'if','while','for','switch'}]
  if definitions_in_file!=[f'FUN_{address:08x}']:skipped['shared_or_ambiguous_C_owner']+=1;continue
  entries=[]
  for filename in row['native_evidence']['cache_files']:
   if filename not in cache:
    doc=json.loads((ROOT/filename).read_bytes());assert doc['native_sha256']==PIN;cache[filename]=doc
   entries += [(filename,e)for e in cache[filename]['entries']if int(e['requested_address'],16)==address]
  if not entries:skipped['missing_cache_entry']+=1;continue
  filename,entry=entries[0];cursor=address;chunks=[];valid=True
  for item in entry['assembly']['items']:
   if int(item['address'],16)!=cursor:valid=False;break
   b=bytes.fromhex(item['bytes']);chunks.append(b);cursor+=len(b)
  if not valid:skipped['nonflat_scope']+=1;continue
  raw=b''.join(chunks)
  if raw!=pe.data[address-pe.base:cursor-pe.base]:skipped['native_byte_mismatch']+=1;continue
  instructions=list(cs.disasm(raw,address))
  if sum(i.size for i in instructions)!=len(raw):skipped['incomplete_decode']+=1;continue
  reviewed+=1;fields=[]
  for match in definitions:
   name,old,rva=match.group(1),match.group(2),int(match.group(3),16)
   if sum(int(m.group(3),16)==rva for m in definitions)>1:continue
   token=r'\b'+re.escape(name)+r'\b';literal=r'(?:0x[0-9a-fA-F]+|[0-9]+|-[0-9]+)[uUlL]*'
   residue=body
   patterns=[token+r'\s*=\s*'+token+r'\s*\+\s*1[uU]?\s*;',token+r'\s*=\s*'+literal+r'\s*;',token+r'\s*(?:==|!=)\s*'+literal,literal+r'\s*(?:==|!=)\s*'+token]
   for pattern in patterns:residue=re.sub(pattern,' ',residue)
   if re.search(token,residue)or not re.search(token,body):continue
   references=[];bad=False
   for instruction in instructions:
    for operand in instruction.operands:
     if operand.type==X86_OP_IMM and operand.imm&0xffffffff==rva:bad=True
     if operand.type!=X86_OP_MEM:continue
     mem=operand.mem;disp=mem.disp&0xffffffff
     if disp==rva:
      if mem.base or mem.index or operand.size!=4 or instruction.mnemonic=='lea':bad=True
      else:references.append(dict(address=hex(instruction.address),bytes=instruction.bytes.hex(),instruction=instruction.mnemonic+' '+instruction.op_str,width=4))
     elif not mem.base and not mem.index and disp<rva+4 and rva<disp+operand.size:bad=True
   if not bad and references:fields.append(dict(symbol=name,address=hex(rva),old_scalar_type=old,native_transfer_bytes=4,c_use='literal assignment/equality/self+1 only',references=references))
  if fields:rows.append(dict(address=row['address'],source=row['source'],source_sha256=sha(source),cache=dict(path=filename,sha256=sha(ROOT/filename)),native_scope_bytes=len(raw),native_scope_sha256=hashlib.sha256(raw).hexdigest(),fields=fields,disposition='requires_function_owner_and_ABI_review',source_replacement_emitted=False,algorithm_credit=0))
 report=dict(schema='narrow-scalar-transfer-unit-pilot-v1',native_sha256=PIN,inventory=dict(path=str(a.inventory),sha256=sha(a.inventory)),reviewed_flat_functions=reviewed,candidate_functions=len(rows),candidate_fields=sum(len(r['fields'])for r in rows),skipped=dict(skipped),functions=rows,source_installation=False,source_expansion_performed=False,complete_function_equivalence=False,whole_game_equivalence=False,host_execution=False,limitations=['DWORD transfer width alone does not prove logical signedness or whole-function correctness','Subsequent native masks/narrowing are not analyzed; a byte C view can be intentional and needs review','No pointers, indexed views, address escape or overlapping transfer is automatically converted','Candidates retain existing C and need ownership/ABI review before any edit'])
 out.parent.mkdir(parents=True,exist_ok=True);out.write_bytes((json.dumps(report,indent=2)+'\n').encode())
 print(json.dumps({k:report[k]for k in ['reviewed_flat_functions','candidate_functions','candidate_fields','source_installation','whole_game_equivalence']}))
if __name__=='__main__':main()
