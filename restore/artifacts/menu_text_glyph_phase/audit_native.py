"""Complete native CFG/stack audit for the five actual text routines."""
from pathlib import Path
from collections import deque
import hashlib,json,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_MEM,X86_OP_IMM,X86_OP_REG,X86_REG_ESP
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
data=json.loads((HERE/'native.json').read_text());cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
arguments={0x4cbf30:2,0x4cc720:6,0x4cc770:5,0x4cca60:4,0x4ccb00:0};records=[]
for key,entry in data['records'].items():
 fn=int(key,16);raw=entry['assembly'];ins={int(x['address'],16):next(cs.disasm(bytes.fromhex(x['bytes']),int(x['address'],16),1)) for x in raw['items']};states={fn:0};todo=deque([fn]);accesses=set();returns=[];calls=[];edges=set()
 while todo:
  a=todo.popleft();i=ins[a];delta=states[a];next_delta=delta;mn=i.mnemonic;ops=i.operands
  for op in ops:
   if op.type==X86_OP_MEM and op.mem.base==X86_REG_ESP:
    assert op.mem.index==0
    start=delta+op.mem.disp;accesses.add((a,start,op.size,op.access))
  if mn=='push':next_delta-=4
  elif mn=='pop':next_delta+=4
  elif mn in ['sub','add'] and ops[0].type==X86_OP_REG and ops[0].reg==X86_REG_ESP:
   assert ops[1].type==X86_OP_IMM;next_delta+=(1 if mn=='add' else -1)*ops[1].imm
  elif mn=='call':
   assert ops[0].type==X86_OP_IMM;target=ops[0].imm&0xffffffff;assert target in arguments or target==0x53e590
   cleanup=12 if target==0x53e590 else 0;next_delta+=cleanup;calls.append({'address':hex(a),'target':hex(target),'caller_delta_before':delta,'callee_cleanup':cleanup})
  if mn=='ret':
   assert not ops and delta==0;returns.append(hex(a));continue
  if mn=='jmp':assert ops[0].type==X86_OP_IMM;targets=[ops[0].imm&0xffffffff]
  elif mn.startswith('j'):assert ops[0].type==X86_OP_IMM;targets=[a+i.size,ops[0].imm&0xffffffff]
  else:targets=[a+i.size]
  for target in targets:
   assert target in ins,(key,hex(a),hex(target));edges.add((a,target))
   if target in states:assert states[target]==next_delta,('inconsistent stack join',key,hex(target))
   else:states[target]=next_delta;todo.append(target)
 assert len(states)==len(ins)==raw['total']
 public_accesses=[{'instruction':hex(a),'entry_SP_relative':offset,'bytes':size,'read':bool(access&1),'written':bool(access&2)} for a,offset,size,access in sorted(accesses) if offset>=4]
 assert all(4<=x['entry_SP_relative'] and x['entry_SP_relative']+x['bytes']<=4+arguments[fn]*4 for x in public_accesses)
 records.append({'entry':key,'instructions':len(ins),'complete_direct_CFG':True,'edges':len(edges),'calls':calls,'RET0_addresses':returns,'maximum_frame_below_entry_SP':-min(states.values()),'stack_arguments':arguments[fn],'public_argument_accesses':public_accesses})
reference=ROOT.parent/'PharaohRewrite/src/Pharaoh.Desktop/LegacyBitmapFont.cs';source=reference.read_text()
assert 'measuredSpaceWidth = 10;' in source and 'drawnSpaceWidth = 8;' in source and 'if (glyph == 0 && value !=' in source
out={'format':1,'native_instructions':367,'complete':True,'records':records,'original_game_executed':False,
 'argument_storage_qualification':'4CBF30 reuses the low byte of its by-value text argument slot as its current character. It has no nested calls and never exposes a pointer to that slot. C preserves the consumed return/data behavior, not a copy-back of callee scratch argument storage. Native/C tests guard the caller area above all argument bytes and check final ESP/saved registers.',
 'C_sharp_reference':{'path':reference.relative_to(ROOT.parent).as_posix(),'sha256':sha(reference),'used_as_exact_behavior':False,'review':'Reference confirms original single-byte glyph map, special font IDs201/871/1005 and different large-font measurement/drawing spacing. Its glyph-zero special case for a, bounds guards, Rectangle vertical alignment, encoding replacement, bitmap cache and unbounded foreach measurement are not transplanted. Native367 instructions decide substitution, scan limit, selection tracking, shadow order, WORD color, image bounds and centering clamp.'},
 'inputs':{'artifacts/menu_text_glyph_phase/native.json':sha(HERE/'native.json'),Path(__file__).relative_to(ROOT).as_posix():sha(Path(__file__))}}
(HERE/'abi-reference-review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'complete':True,'native_instructions':367,'functions':len(records),'RET0_paths':sum(len(x['RET0_addresses']) for x in records),'C_sharp_used_as_exact_behavior':False}))
