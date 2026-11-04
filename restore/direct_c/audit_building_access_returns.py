"""Check the five exact native discarded-return sites and regeneration repair."""
from pathlib import Path
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'tools'),str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
from native_test_pe import PE
from repair_discarded_returns import repair
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
audit=json.loads((ROOT/'metadata/building-access-unused-return-audit.json').read_text())
bundle=json.loads((ROOT/'metadata/building-access-caller-windows.json').read_text())
records={r['evidence_id']:r for r in bundle['records']}
binary=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(binary.read_bytes()).hexdigest()==audit['native_sha256']
pe=PE(binary);decoder=Cs(CS_ARCH_X86,CS_MODE_32)
assert len(audit['callers'])==5
for site in audit['callers']:
 at=int(site['address'],16);raw=bytes.fromhex(site['bytes_hex']);r=records[site['evidence_id']]
 assert r['authority']=='shipped-artifact' and r['confidence']=='observed'
 assert r['subject']['digest']['sha256']==audit['native_sha256']
 assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==site['bytes_hex']
 assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])
 ins=list(decoder.disasm(raw,at))
 assert ins[0].mnemonic=='call' and int(ins[0].op_str,16)==0x457af0
 assert ins[1].mnemonic=='mov' and ins[1].op_str=='eax, dword ptr [0xeadd6c]'
source=(ROOT/audit['source']).read_text()
new='FUN_00457af0(), (&DAT_007999f8)[DAT_00eadd6c * 0x84] == 0'
old='(FUN_00457af0(), restore_unresolved_return(0x00457af0u)), (&DAT_007999f8)[DAT_00eadd6c * 0x84] == 0'
assert source.count(new)==5 and 'restore_unresolved_return(0x00457af0u)' not in source
assert repair(source.replace(new,old))==source
# Consumed values and a different query retain their fail-closed behavior.
for consumed in ['value=(FUN_00457af0(), restore_unresolved_return(0x00457af0u));',
                 '((FUN_00457af0(), restore_unresolved_return(0x00457af0u)), other_query())']:
 assert repair(consumed)==consumed
print('PASS five native overwritten-return sites, current C source and exact regeneration rule')
