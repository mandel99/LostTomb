"""Bind the frozen four-site proof and the existing ordinary prey C caller.

This creates a local typed-call contract only. The global raw-drop flag remains
false, and this does not extend the CLI current-batch scope from four to five.
"""
from pathlib import Path
import hashlib,json
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in [HERE,*HERE.parents] if (p/'direct_c/native_test_pe.py').exists() and (p/'runtime').is_dir())
DATA=HERE if (HERE/'actor-animal-following-native.json').exists() else ROOT/'metadata'
receipt=DATA/'actor-animal-following-cue-caller-audit.json'
audit=json.loads(receipt.read_text())
current=['0x49da20','0x49e692','0x49ea25','0x49f3dd']
assert audit['selected_scope']=='current-batch' and audit['selected_scope_passed']
assert not audit['diagnostic_allow_open']
assert audit['current_batch_callers']==current
assert not audit['passed'] and not audit['production_raw_adapter_ready']
sites=[*current,'0x49c50c']
results={site:audit['caller_results'][site] for site in sites}
assert all(not r['unknown'] and not r['unresolved'] for r in results.values())
prey=ROOT/'runtime/actor_prey.c';text=prey.read_text()
assert 'void FUN_0049bd40(void)' in text
assert 'extern void FUN_004b5050(int32_t);' in text
assert text.count('FUN_004b5050((int32_t)id);')==1
assert results['0x49c50c']['states_checked']==17
assert any(k['pc']=='0x49c55f' for k in results['0x49c50c']['kill_addresses'])
paths=[receipt,Path(__file__),prey,ROOT/'include/actor_update.h',ROOT/'include/actor_civic.h',ROOT/'runtime/actor_command.c',ROOT/'include/actor_command.h']
proof=dict(schema_version=1,passed=True,contract_scope='Five supported ordinary named/canonical cue callers only; no full RAW cue comparison.',
           canonical_contract='void FUN_004b5050(int32_t actor): full DWORD, cdecl RET0, effects plus nonvolatile/stack ABI; incidental EAX/ECX/EDX/flags are not promised.',
           current_batch_callers=current,current_batch_scope_count=4,
           additional_existing_C_caller=dict(site='0x49c50c',native_owner='0x49bd40',source='runtime/actor_prey.c',function='FUN_0049bd40',source_inventory_calls=1),
           supported_typed_callers=sites,local_typed_contract=True,global_raw_drop=False,
           global_audit_passed=False,production_raw_adapter_ready=False,
           retained_compatibility='Entire original RAW004b5050/model/ROM/reader/prototype remains unchanged for legacy frame calls.',
           caller_results=results,source_hashes={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
           integration_requirement='Combined audit must separately verify four ordinary C actor_command.c calls plus the single actor_prey.c call after C5 integration. This frozen receipt does not claim those four staged C implementations are already in main.',
           limitations=['Global handler-cell lifecycle/computed-alias invariant is unproved; default global audit remains OPEN at5653DD CALL EAX. No null-forever assumption or raw cue output equivalence is asserted.'])
(DATA/'actor-animal-following-cue-typed-contract.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS local typed5 contract; current-batch scope remains4; global raw-drop=false')
