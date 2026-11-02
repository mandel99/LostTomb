"""Conservative staged-only primitive-leaf selection. Compilation is not verification."""
import collections,json,re,sys
from pathlib import Path
from common import ROOT,PRIVATE,PIN,sha,confined
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_IMM,X86_REG_EAX,X86_REG_AX,X86_REG_AL,X86_REG_AH
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;CODE_SLOTS=set()
TYPES={'char':('int8_t',1),'byte':('uint8_t',1),'uchar':('uint8_t',1),'bool':('uint8_t',1),'short':('int16_t',2),'ushort':('uint16_t',2),'int':('int32_t',4),'uint':('uint32_t',4),'long':('int32_t',4),'ulong':('uint32_t',4),'undefined1':('uint8_t',1),'undefined2':('uint16_t',2),'undefined4':('uint32_t',4)}
def primitive(d):
 if not d:return None
 name=d['display'];p=TYPES.get(name)
 if not p or d['size_bytes']!=p[1] or any(q in d['java_class'] for q in ['Pointer','Array','Structure','Union','Enum','Float','Double','TypeDef','DefaultDataType']):return None
 return p
class Rejected(Exception):pass
def need(q,reason):
 if not q:raise Rejected(reason)
def check(f,seed):
 need(not seed.get('boundary_conflict'),'qualified_native_boundary_conflict');need(not seed['protected_function'],'protected_accepted_or_staged_address');need(seed.get('unit') is not None,'no_existing_unit_or_unqualified_new_entry');need(f.get('status')=='facts_complete','typed_facts_unavailable');native=f['native_instructions'];expected={int(a,16):b for a,b in seed['expected_instructions']};need({int(q['address'],16):q['bytes'] for q in native}==expected,'native_body_set_mismatch');proto=f['logical_prototype'];ret=primitive(proto['return_datatype']);need(ret and ret[1]==4,'ambiguous_void_partial_or_nonprimitive_return');need(not f['native_callsites'],'native_call');need(f['native_return_sites'] and all(q['instruction']=='RET' for q in f['native_return_sites']),'native_ret_cleanup_or_missing_return')
 for i,p in enumerate(proto['parameters']):
  need(isinstance(p,dict),'logical_parameter_unavailable')
  t=primitive(p['datatype']);need(t is not None,'pointer_or_unknown_parameter_type');s=p['storage'];need(len(s)==1 and s[0]['address']=='stack:'+format(4+i*4,'x') and s[0]['size_bytes']==t[1],'register_or_noncanonical_parameter_storage')
 code=f['raw_pseudocode'].replace('\r\r\n','\n').replace('\r\n','\n');clean=re.sub(r'/\*.*?\*/','',code,flags=re.S)
 body_text=clean.split('{',1)[-1]
 for parameter in proto['parameters']:need(re.search(r'\b'+re.escape(parameter['name'])+r'\b',body_text) is not None,'unused_logical_parameter_or_stack_slot_gap_requires_ABI_review')
 need(not re.search(r'\b(?:in_[A-Z]\w*|unaff_\w*|extraout_\w*|in_stack_\w*|register0x\w*|stack0x\w*|ExceptionList)\b',clean),'unresolved_register_stack_or_exception_model');need(not re.search(r'\b(?:float|double|float10|longlong|ulonglong|undefined8)\b',clean),'floating_or_wide_value');need(not re.search(r'\[|\]|\._|->|\([^()\n]*\*\s*\)|\b(?:int|uint|char|short|void)\s*\*',clean),'pointer_array_or_field_projection');need(not re.search(r'\b(?:PTR_|LAB_|s_)[A-Za-z_0-9]*\b|\bcode\b|__fastcall|__thiscall',clean),'indirect_code_data_token_or_receiver');entry=seed['entry'];foreign=set(re.findall(r'\bFUN_([0-9a-fA-F]{8})\b',clean))-{entry};need(not foreign,'foreign_function_or_code_token');need(len(re.findall(r'\bFUN_'+entry+r'\b',clean))==1,'self_code_address_token');calls=set(re.findall(r'\b([A-Za-z_]\w*)\s*\(',clean));need(calls<={'FUN_'+entry,'if','else','while','for','switch','return','sizeof'},'unsupported_C_helper_or_call')
 globals={};byaddr=collections.defaultdict(set)
 for token in f['clang_tokens']:
  h=token.get('high_variable')
  if not h:continue
  t=primitive(h['datatype']);need(t is not None,'pointer_unknown_or_wide_HighVariable')
  if h['class']!='HighGlobal':continue
  name=token['text'];v=token.get('varnode');need(re.fullmatch(r'[A-Za-z_]\w*',name) is not None,'nonidentifier_global_token');need(v is not None and v['size_bytes']==h['size_bytes']==t[1],'token_varnode_width_conflict');need(h['representative'].startswith('ram:'),'nonRAM_global');addr=int(h['representative'].split(':')[1],16);need(not re.search(r'&\s*'+re.escape(name)+r'\b',clean),'global_address_projection');record=(addr,t[0],t[1]);need(name not in globals or globals[name]['key']==record,'mixed_same_token_types');globals[name]={'key':record,'HighVariable':h,'varnode':v};byaddr[addr].add(t[1])
 need(set(re.findall(r'\b(?:_?(?:DAT|UNK|BYTE)_[0-9a-f]{8}|[ciu]Ram[0-9a-f]{8}|ram0x[0-9a-f]{8})\b',clean))<=set(globals),'unbound_global_identifier')
 for addr,widths in byaddr.items():
  need(addr not in CODE_SLOTS,'known_indirect_code_pointer_slot')
  need(len(widths)==1,'mixed_global_alias_widths');access=set()
  for ins in native:
   for op in ins['raw_pcode']:
    for v in op['inputs']+[op.get('output')]:
     if v and v['memory'] and v['address']=='ram:'+format(addr,'x'):access.add(v['size_bytes'])
  need(access==widths,'native_global_access_width_conflict')
 decoded={}
 for q in native:
  pc=int(q['address'],16);ins=next(cs.disasm(bytes.fromhex(q['bytes']),pc,count=1));decoded[pc]=ins
  need(not any(s in q['instruction'].upper() for s in ['XMM','MM0','MM1','MM2','MM3','MM4','MM5','MM6','MM7','ST0','ST(','FLOAT']),'floating_or_vector_register');need(not ins.mnemonic.startswith('f'),'floating_instruction')
  need(not ins.mnemonic.startswith('call'),'native_call');need(ins.mnemonic not in ['int','int3','iret','rdtsc','cpuid','sysenter','syscall','hlt','cli','sti'],'system_or_nonordinary_ABI')
  for operand in ins.operands:
   if operand.type==X86_OP_IMM and not q['native_jump']:need(not 0x401000<=operand.imm<0x56f000,'native_code_address_constant')
  if q['native_jump']:
   need(len(q['flow_targets'])==1,'indirect_or_computed_jump');need(int(q['flow_targets'][0],16) in expected,'out_of_function_tail_or_branch')
 # Conservative physical ABI gate: no nonvolatile scratch use; only canonical EBP frames, no stack shims.
 ordered=sorted(decoded);has_frame=any(any(r.lower() in ['ebp','bp'] for r in q.get('register_reads',[])+q.get('register_writes',[])) for q in native)
 if has_frame:need(len(ordered)>=3 and decoded[ordered[0]].mnemonic=='push' and decoded[ordered[0]].op_str=='ebp' and decoded[ordered[1]].mnemonic=='mov' and decoded[ordered[1]].op_str=='ebp, esp','noncanonical_frame_ABI')
 volatile={'eax':0,'ecx':1,'edx':2}
 def base_register(name):
  if name in ['al','ah','ax','eax']:return 'eax'
  if name in ['cl','ch','cx','ecx']:return 'ecx'
  if name in ['dl','dh','dx','edx']:return 'edx'
  return name
 for pc,ins in decoded.items():
  q=next(q for q in native if int(q['address'],16)==pc);rr,ww=ins.regs_access();names=[ins.reg_name(r) for r in rr+ww]
  need(not any(r in names for r in ['ebx','bx','bl','bh','esi','si','edi','di']),'nonvolatile_register_scratch_requires_ABI_review')
  if ins.mnemonic in ['push','pop']:need(has_frame and ins.op_str=='ebp' and ((ins.mnemonic=='push' and pc==ordered[0]) or ins.mnemonic=='pop'),'stack_shim_or_temporary_requires_review')
  elif 'esp'in [ins.reg_name(r) for r in ww]:need(ins.mnemonic in ['leave','ret'],'dynamic_stack_write_requires_review')
  if 'ebp'in [ins.reg_name(r) for r in ww]:need((pc==ordered[1] and ins.mnemonic=='mov') or ins.mnemonic in ['pop','leave'],'frame_register_write_requires_review')
  if has_frame and q['native_jump']:need(int(q['flow_targets'][0],16)>ordered[1],'branch_into_frame_prologue')
 todo=[(int(entry,16),0,not has_frame)];seen=set()
 while todo:
  pc,known,restored=todo.pop()
  if (pc,known,restored) in seen:continue
  seen.add((pc,known,restored));need(pc in decoded,'CFG_escaped_body');ins=decoded[pc];q=next(q for q in native if int(q['address'],16)==pc);reads,writes=ins.regs_access()
  zero=ins.mnemonic in ['xor','sub'] and len(ins.operands)==2 and ins.operands[0].reg==ins.operands[1].reg and ins.operands[0].size==4
  for reg in reads:
   name=base_register(ins.reg_name(reg))
   if name in volatile:need(known&(1<<volatile[name]) or zero,'entry_or_partial_volatile_register_input')
  for reg in writes:
   name=ins.reg_name(reg)
   if name in volatile:known|=1<<volatile[name]
  if ins.mnemonic=='mov' and ins.op_str=='ebp, esp':restored=False
  if ins.mnemonic in ['pop','leave']:restored=True
  if ins.mnemonic=='ret':need(known&1,'partial_or_unresolved_EAX_return');need(restored,'callee_frame_not_restored');continue
  if ins.mnemonic=='jmp':targets=[int(q['flow_targets'][0],16)]
  elif q['native_jump']:targets=[int(q['flow_targets'][0],16),pc+ins.size]
  else:targets=[pc+ins.size]
  todo.extend((p,known,restored) for p in targets)
 return code,globals,{'native_return_cleanup':0,'logical_model_advisory':proto['calling_model_advisory'],'full_EAX_on_all_return_paths':True,'register_parameters':False,'body_arithmetic_not_verified':True}
def select(run,limit=50,output_name='candidates-complete'):
 run=confined(run);seed=json.loads((run/'seed.json').read_text());overrides=json.loads((ROOT/'tools/bulk_type_export_v2/protection_overrides.json').read_text());extra=set(overrides['explicit_protected_entries'])
 # Recheck live/staged protection after the potentially long export. Concurrent root work cannot become a candidate.
 approvals=json.loads((ROOT/'metadata/full-function-review-approvals.json').read_text())
 for q in approvals.get('functions',[])+approvals.get('additional_native_entries',[]):
  if q.get('address'):extra.add(format(int(q['address'],16),'08x'))
 before=json.loads((run/'protected-before.json').read_text())
 for q in seed['functions']:
  unit=q.get('unit')
  if unit and unit in before and (ROOT/unit).is_file() and sha(ROOT/unit)!=before[unit].get('sha256'):extra.add(q['entry'])
 mapping={q['unit']:q['entry'] for q in seed['functions'] if q.get('unit')}
 pointer_inventory=json.loads((ROOT/'metadata/standalone-data.json').read_text())
 CODE_SLOTS.update(int(q['location'],16) for q in pointer_inventory.get('code_pointer_candidates',[]))
 for evidence in seed['cache_evidence']:
  for row in json.loads((ROOT/evidence['path']).read_text())['entries']:
   for ins in row['assembly']['items']:
    match=re.fullmatch(r'CALL dword ptr \[0x([0-9a-f]+)\]',ins['instruction'])
    if match:CODE_SLOTS.add(int(match[1],16))
 for path in (ROOT/'artifacts').rglob('*.c'):
  if '_phase' not in path.as_posix():continue
  if ('src/'+path.name) in mapping:extra.add(mapping['src/'+path.name])
  for address in re.findall(r'(?m)^\w[^;\n]*\b\w*?([0-9a-fA-F]{8})\s*\([^;\n]*\)\s*(?:\n\s*)?\{',path.read_text(encoding='utf-8-sig',errors='replace')):extra.add(address.lower())
 need(re.fullmatch(r'[a-z][a-z0-9_-]{0,31}',output_name),'unsafe_candidate_name');out=run/output_name;need(not out.exists(),'candidate_directory_must_be_fresh');out.mkdir();rows=[];emitted=[];pinned=0
 for original in seed['functions']:
  s=dict(original);s['protected_function']=s['protected_function'] or s['entry'] in extra
  path=run/'facts'/(s['entry']+'.json');record={'entry':s['entry'],'unit':s.get('unit'),'protected':s['protected_function']}
  if not path.exists():record.update(status='facts_missing',reason='no_export_record');rows.append(record);continue
  d=json.loads(path.read_text());need(d['native_sha256']==PIN,'wrong_native_fact_SHA');f=d['function'];record['fact_sha256']=sha(path)
  if f.get('native_instructions') and {q['address']:q['bytes'] for q in f['native_instructions']}==dict(s['expected_instructions']):pinned+=len(f['native_instructions'])
  try:
   code,bindings,abi=check(f,s)
   if len(emitted)>=limit:record.update(status='eligible_not_emitted_limit',reason='first50_limit');rows.append(record);continue
   prefix=['#include "bulk_leaf_types.h"'];detail=[]
   for name,b in sorted(bindings.items()):
    addr,t,width=b['key'];prefix.append(f'#define {name} (*(volatile {t} *)0x{addr:08x}u)');detail.append({'token':name,'address':f'{addr:08x}','C_type':t,'size':width,'source':'same_token_HighVariable_varnode_and_native_access','HighVariable':b['HighVariable']})
   dest=out/(s['entry']+'.c');dest.write_text('\n'.join(prefix)+'\n'+code,encoding='utf-8');record.update(status='staged_unverified',candidate=dest.relative_to(ROOT).as_posix(),candidate_sha256=sha(dest),bindings=detail,ABI=abi,raw_body_sha256=__import__('hashlib').sha256(code.encode()).hexdigest());emitted.append(record)
  except Rejected as e:record.update(status='quarantined',reason=str(e))
  rows.append(record)
 header='#ifndef PRIVATE_BULK_LEAF_TYPES_H\n#define PRIVATE_BULK_LEAF_TYPES_H\n#include <stdint.h>\ntypedef unsigned char byte,uchar,undefined1,bool;\ntypedef unsigned short ushort,word,undefined2;\ntypedef unsigned int uint,ulong,dword,undefined4;\n#define true 1\n#define false 0\n#endif\n';(out/'bulk_leaf_types.h').write_text(header)
 report={'native_sha256':PIN,'functions_considered':len(rows),'emitted':len(emitted),'native_instruction_records_reconfirmed_in_fact_bodies':pinned,'status_counts':dict(collections.Counter(q['status'] for q in rows)),'rejection_counts':dict(collections.Counter(q.get('reason') for q in rows if q['status']=='quarantined')),'rows':rows,'candidate_limit':limit,'candidate_directory':out.relative_to(ROOT).as_posix(),'selector_source_sha256':sha(Path(__file__)),'protection_overrides_sha256':sha(ROOT/'tools/bulk_type_export_v2/protection_overrides.json'),'actor_seed_inventory_deferred':overrides['future_actor_seed_inventory'],'new_actor_entries_seeded':False,'all_functions_verified':False,'compilation_is_not_verification':True,'application_operation':False};(run/'selection.json').write_text(json.dumps(report,indent=2)+'\n');return {k:report[k] for k in ['functions_considered','emitted','status_counts','rejection_counts','all_functions_verified']}
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--run-root',type=Path,required=True);p.add_argument('--output-name',default='candidates-complete');a=p.parse_args();print(json.dumps(select(a.run_root,50,a.output_name)))
