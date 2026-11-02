"""Pinned-byte, bit-aware EAX liveness audit after all38 melee call sites."""
from pathlib import Path
import argparse,json,hashlib,struct,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/actor_combat_phase'
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--production',action='store_true');args=p.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import *
b=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();native_sha=hashlib.sha256(b).hexdigest();assert native_sha=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';pe=struct.unpack_from('<I',b,0x3c)[0];o=pe+24;table=o+struct.unpack_from('<H',b,pe+20)[0];base=struct.unpack_from('<I',b,o+28)[0]
def read(a,n):
 for j in range(struct.unpack_from('<H',b,pe+6)[0]):
  q=table+j*40;vs,va,rs,raw=struct.unpack_from('<IIII',b,q+8)
  if va<=a-base<va+rs:return b[raw+a-base-va:raw+a-base-va+n]
 return b''
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;bits={X86_REG_EAX:0xffffffff,X86_REG_AX:0xffff,X86_REG_AL:255,X86_REG_AH:0xff00}
def audit(start):
 pending=[(start,0xffffffff)];seen=set();reads=[];forward=[];unresolved=[];kills=[]
 while pending:
  pc,mask=pending.pop()
  if not mask or (pc,mask) in seen:continue
  seen.add((pc,mask))
  if len(seen)>2048 or not 0x401000<=pc<0x565000:unresolved.append(hex(pc));continue
  ins=list(md.disasm(read(pc,15),pc,count=1))
  if not ins:unresolved.append(hex(pc));continue
  i=ins[0];rr,ww=i.regs_access();readmask=0
  for reg in rr:readmask|=bits.get(reg,0)
  zeroing=i.mnemonic in ['xor','sub'] and len(i.operands)==2 and all(op.type==X86_OP_REG for op in i.operands) and i.operands[0].reg==i.operands[1].reg
  if len(i.operands)==2 and i.operands[0].type==X86_OP_REG and i.operands[0].reg in bits and i.operands[1].type==X86_OP_IMM:
   if i.mnemonic=='and':readmask&=i.operands[1].imm&0xffffffff
   if i.mnemonic=='or':readmask&=~i.operands[1].imm&0xffffffff
  if readmask&mask and not zeroing:reads.append({'address':hex(pc),'instruction':i.mnemonic+' '+i.op_str,'original_bits':hex(readmask&mask)});continue
  for reg in ww:mask&=~bits.get(reg,0)&0xffffffff
  if not mask:kills.append(hex(pc));continue
  if i.mnemonic.startswith('ret'):forward.append(hex(pc));continue
  if i.mnemonic=='call':kills.append(hex(pc)+' call-clobber');continue
  if i.mnemonic.startswith('j'):
   if i.operands[0].type!=X86_OP_IMM:unresolved.append(hex(pc)+' indirect');continue
   pending.append((i.operands[0].imm,mask))
   if i.mnemonic!='jmp':pending.append((pc+i.size,mask))
  else:pending.append((pc+i.size,mask))
 return {'instructions_states_checked':len(seen),'reads_before_overwrite':reads,'forwarding_returns':sorted(set(forward)),'unresolved':sorted(set(unresolved)),'overwrites_or_call_clobbers':sorted(set(kills))}
report=ROOT/'metadata/actor-combat-caller-contracts.json' if args.production else PHASE/'metadata/caller-contracts.json';native=ROOT/'metadata/actor-combat-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(report.read_text());n=json.loads(native.read_text());refs=n['functions']['004b17d0']['xrefs']['items'];assert len(refs)==38 and len(d['all38_main_call_sites'])==38
for row,x in zip(d['all38_main_call_sites'],refs):
 assert row['call']==x['from'];a=int(row['call'],16);call=list(md.disasm(read(a,5),a))[0];assert call.mnemonic=='call' and call.op_str=='0x4b17d0'
 row['bounded_CFG']=audit(a+5);assert not row['bounded_CFG']['reads_before_overwrite'] and not row['bounded_CFG']['unresolved'],row
 row['classification']='epilogue-forwarding-only' if row['bounded_CFG']['forwarding_returns'] else 'EAX-overwritten-on-all-reachable-paths'
d['caller_analysis_limit']='Pinned-byte bounded CFG tracks original EAX/AX/AL/AH bits after all38 call sites, including partial overwrites and AND/OR masks, treating subsequent calls as ABI clobbers. No read of original result bits is reachable in these bounded caller continuations. Two forwarding epilogues are listed without inventing a consumed result;493610 is an accepted void helper and15 direct consumers of4991a0 are separately audited below, without interpreting forwarding epilogues as consumed results.'
d['caller_ABI_audit']={'passed':True,'checked':38,'native_sha256':native_sha,'auditor_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'categories':{k:sum(x['classification']==k for x in d['all38_main_call_sites']) for k in sorted(set(x['classification'] for x in d['all38_main_call_sites']))},'limitations':['Bounded static continuation audit, not complete execution of all actor callbacks. Subsequent calls are treated as conventional EAX clobbers; explicit argument setup/indirect call operands are inspected before that boundary. Forwarded EAX through void epilogues is not counted as a consumed result.']}
forwarding=ROOT/'metadata/actor-combat-forwarding-native.json' if args.production else PHASE/'metadata/forwarding-native.json';fr=json.loads(forwarding.read_text());assert fr['info']['address']=='004991a0' and not fr['xrefs']['has_more'] and fr['xrefs']['total']==15
follow=[]
for x in fr['xrefs']['items']:
 a=int(x['from'],16);call=list(md.disasm(read(a,5),a))[0];assert call.mnemonic=='call' and call.op_str=='0x4991a0'
 r=audit(a+5);assert not r['reads_before_overwrite'] and not r['unresolved'],r
 follow.append({'call':x['from'],'bounded_CFG':r})
d['forwarding4991a0']={'native_dossier_sha256':hashlib.sha256(forwarding.read_bytes()).hexdigest(),'info':fr['info'],'call_sites':follow,'limitation':'No direct consumer of4991a0 reads its originalEAX before overwrite/epilogue. Forwarding through additional actor-wrapper epilogues is not a return contract; their eventual table dispatch is outside this bounded audit.'}
d['caller_ABI_audit']['additional4991a0_call_sites']=15
d['caller_ABI_audit']['auditor_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
report.write_text(json.dumps(d,indent=2)+'\n');print(json.dumps(d['caller_ABI_audit'],indent=2))
