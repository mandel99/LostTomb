"""Audit exact native member receivers, current wiring and narrow regeneration."""
from pathlib import Path
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'tools'),str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
from repair_member_receivers import repair
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
j=json.loads((ROOT/'metadata/processing-receiver-native.json').read_text());native=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(native.read_bytes()).hexdigest()==j['native_sha256']
pe=PE(native)
for ev in j['callers']:
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==j['native_sha256']
 result=ev['normalized_result'];a=int(result['address'],16);raw=bytes.fromhex(result['bytes_hex']);assert result['complete']
 assert raw==bytes(pe.data[a-pe.base:a-pe.base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));assert [(i.mnemonic,i.op_str) for i in ins[:3]]==[('mov','ecx, 0xd05720'),('call','0x4bf9d0'),('test','al, al')]
source=(ROOT/'src/unit_02503.c').read_text()
assert 'void FUN_0053c100(int32_t actor){restore_actor_update_message(actor);}' in source
assert 'FUN_004bf9d0();' not in source
for file,site in [('runtime/actor_laborer_model.c','004ab491')]:
 text=(ROOT/file).read_text();end=text.index('0x'+site+'u,0x004bf9d0u,f)');assert 'f->r[1]=0x00d05720u;' in text[end-180:end]
text=(ROOT/'runtime/actor_message.c').read_text()
assert text.count('FUN_004bf9d0(0xd05720u)')==1
text=(ROOT/'runtime/menu_actor_message.c').read_text()
assert 'restore_actor_update_message_result((int32_t)MI_U32(sp+4),f->r[0])' in text
sample='extern uint FUN_004bf9d0();\ncVar1 = FUN_004bf9d0();'
fixed=repair(sample,'FUN_0053c100');assert fixed!=sample and repair(fixed,'FUN_0053c100')==fixed
assert repair(sample,'FUN_004ab000')==sample
assert repair('cVar2 = FUN_004bf9d0();','FUN_0053c100')=='cVar2 = FUN_004bf9d0();'
assert repair('cVar1 = FUN_004bf9d0(self);','FUN_0053c100')=='cVar1 = FUN_004bf9d0(self);'
assert 'repair_member_receivers(' in (ROOT/'tools/prepare_c.py').read_text()
print('PASS two native ECX receivers, current typed/legacy wiring and narrow idempotent regeneration rule')
