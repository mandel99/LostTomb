"""Record current dropdown-body/routing gaps without changing frozen proofs."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
proof=read('metadata/dropdown-actions-emulation.json')
assert proof['passed'] and proof['checked']==1632 and proof['comparisons']==6528
assert proof['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert sum(c['total'] for c in proof['coverage'].values())==90
assert all(c['executed']==c['total'] for c in proof['coverage'].values())
assert proof['conditional_outcomes']['checked']==proof['conditional_outcomes']['total']==4
for p,h in {**proof['source_hashes'],**proof['native_exports']}.items():assert sha(ROOT/p)==h,p
assert set(proof['variants'])=={'Debug','Debug_plain','O2','O2_plain'}
for v in proof['variants'].values():assert v['process_exit_code']==0 and sha(ROOT/v['path'])==v['sha256']
system=read('metadata/dropdown-system-actions-emulation.json')
assert system['passed'] and system['checked']==824 and system['comparisons']==1648
assert system['native_sha256']==proof['native_sha256']
assert sum(c['total'] for c in system['coverage'].values())==32
assert all(c['executed']==c['total'] for c in system['coverage'].values())
assert set(system['variants'])=={'Debug','O2'}
for p,h in {**system['source_hashes'],**system['native_exports']}.items():assert sha(ROOT/p)==h,p
for v in system['variants'].values():assert v['process_exit_code']==0 and sha(ROOT/v['path'])==v['sha256']
start=read('metadata/dropdown-start-actions-emulation.json')
assert start['passed'] and start['checked']==1250 and start['comparisons']==2500
assert start['native_sha256']==proof['native_sha256']
assert start['coverage']['004023a0']=={'executed':38,'total':38,'missing':[]}
assert start['coverage']['00409580']=={'executed':11,'total':11,'missing':[]}
assert start['coverage']['004d3c40']=={'executed':23,'total':23,'missing':[]}
assert start['coverage']['0055f449']=={'executed':77,'total':77,'missing':[]}
assert start['jump_table']['indices_executed']==[0,1,2,3]
assert set(start['variants'])=={'Debug','O2'}
for p,h in {**start['source_hashes'],**start['native_exports']}.items():assert sha(ROOT/p)==h,p
for v in start['variants'].values():assert v['process_exit_code']==0 and sha(ROOT/v['path'])==v['sha256']
remaining=read('metadata/remaining-menu-actions-emulation.json')
assert remaining['passed'] and remaining['production'] and remaining['checked']==remaining['complete_cases']==3090 and remaining['comparisons']==12360
assert remaining['native_sha256']==proof['native_sha256'] and remaining['source_drift']==[]
assert remaining['coverage']=={'00406090':[22,22],'0040a0e0':[27,27],'00489030':[11,11],'004c7f30':[10,10]}
assert remaining['frozen_oracle_sha256']=='3dca377d7560ef5ac880081ff4c24f87967f0ee15dcfe97c907b3647b9ffa6ba'
for p,h in {**remaining['source_hashes'],**remaining['native_exports']}.items():assert sha(ROOT/p)==h,p
assert set(remaining['variants'])=={'Debug_trace','Debug_plain','O2_trace','O2_plain'}
for v in remaining['variants'].values():assert v['checked']==3090 and sha(ROOT/v['path'])==v['sha256']
popup=read('metadata/selection-popup-emulation.json')
assert popup['passed'] and popup['checked']==6491 and popup['comparisons']==12982 and popup['native_sha256']==proof['native_sha256']
q=popup['qualified_controller']
assert q['complete'] and q['native_instructions']==q['executed']==815 and q['conditional_edges']==q['edges_executed']==132
assert q['excluded_instructions']==q['excluded_edges']==[]
for p,h in {**popup['source_hashes'],**popup['native_exports']}.items():assert sha(ROOT/p)==h,p
assert set(popup['variants'])=={'Debug','O2'}
for v in popup['variants'].values():assert v['checked']==6491 and sha(ROOT/v['path'])==v['sha256']
editor=read('metadata/editor-validation-emulation.json')
assert editor['passed'] and editor['checked']==editor['plain_checked']==editor['comparison_checked']==6150
assert editor['comparison_count']==24600 and editor['native_sha256']==proof['native_sha256']
assert editor['reachable_instruction_coverage']==[571,571] and editor['owned_instruction_coverage']==[408,408]
assert editor['feasible_conditional_outcomes']==[138,138]
assert editor['case_selection']=={'limit':None,'start':0,'single_case':False}
for p,h in editor['source_hashes'].items():assert sha(ROOT/p)==h,p
assert sha(ROOT/'build/c-only-debug/Debug/restore_editor_validation_verify.exe')==editor['compiled_C_sha256']
assert sha(ROOT/'build/c-only/Release/restore_editor_validation_verify.exe')==editor['comparison_C_sha256']
replay=read('metadata/replay-mission-emulation.json')
assert replay['passed'] and replay['checked']==replay['plain_checked']==replay['comparison_checked']==10138
assert replay['comparison_count']==40552 and replay['native_sha256']==proof['native_sha256']
assert replay['native_instruction_coverage']==[151,151] and replay['owned_instruction_coverage']==[113,113]
assert replay['native_conditional_outcomes']==[6,6] and replay['native_uncovered_instructions']==replay['missing_conditional_outcomes']==[]
assert replay['case_selection']=={'limit':None,'start':0,'single_case':False}
assert len(replay['mutated_helper_phases'])==20
for p,h in replay['source_hashes'].items():assert sha(ROOT/p)==h,p
assert sha(ROOT/'build/c-only-debug/Debug/restore_replay_mission_verify.exe')==replay['compiled_C_sha256']
assert sha(ROOT/'build/c-only/Release/restore_replay_mission_verify.exe')==replay['comparison_C_sha256']
existing=read('metadata/menu-existing-actions-emulation.json')
assert existing['passed'] and existing['checked']==existing['comparison_checked']==4193 and existing['comparison_count']==8386
assert existing['native_sha256']==proof['native_sha256'] and existing['case_selection']=={'limit':None,'start':0,'single_case':False}
assert existing['primary_instruction_coverage']==[235,235] and existing['reachable_instruction_coverage']==[423,423]
assert existing['feasible_conditional_outcomes']==[18,18] and len(existing['mutated_helper_phases'])==26
for p,h in existing['source_hashes'].items():assert sha(ROOT/p)==h,p
assert sha(ROOT/'build/c-only-debug/Debug/restore_menu_existing_actions_verify.exe')==existing['compiled_C_sha256']
assert sha(ROOT/'build/c-only/Release/restore_menu_existing_actions_verify.exe')==existing['comparison_C_sha256']
old=read('artifacts/dropdown_menu_phase/metadata/callback-routing-contract.json')
direct=set(re.findall(r'address==0x([0-9a-fA-F]{8})u',(ROOT/'runtime/menu_callbacks.c').read_text()))
new=set(proof['coverage']);old_owned={e['token'] for e in old['entries'] if e['state']=='owned full native callback'}
system_owned=set(system['coverage'])
start_owned={'004023a0','00409580'}
remaining_owned={'00406090','0040a0e0'}
popup_owned={'00409540'}
editor_owned={'00409830'}
replay_owned={'00418510'}
existing_owned={'00402450','00402540','00402630','00402bb0'}
entries=[]
for e in old['entries']:
    token=e['token'];n={k:e[k] for k in ['token','C_symbol','items','native_callback_ABI']}
    n['available_in_current_router']=token in new or token in direct or token in old_owned or token in system_owned or token in start_owned or token in remaining_owned or token in popup_owned or token in editor_owned or token in replay_owned
    if token in new:n.update(state='complete callback proved; downstream mode behavior remains separate',source='runtime/dropdown_actions.c',proof='metadata/dropdown-actions-emulation.json')
    elif token in system_owned:n.update(state='complete callback with real message helpers and explicit OS/media boundaries; downstream rendering remains separate',source='runtime/dropdown_system_actions.c',proof='metadata/dropdown-system-actions-emulation.json')
    elif token in start_owned:n.update(state='complete callback with actual reset/stream/selector/trim bodies and explicit family/file/media boundaries',source='runtime/dropdown_start_actions.c',proof='metadata/dropdown-start-actions-emulation.json')
    elif token in remaining_owned:n.update(state='complete local controller/ABI proof; eight deep algorithms and488E90FPU remain incomplete',source='src/unit_00049.c' if token=='00406090' else 'src/unit_00077.c',proof='metadata/remaining-menu-actions-emulation.json')
    elif token in popup_owned:n.update(state='complete popup controller/renderer/input composition; deeper pixels and gameplay remain qualified',source='runtime/selection_popup.c',proof='metadata/selection-popup-emulation.json')
    elif token in editor_owned:n.update(state='complete editor validation and real classifiers/selector/counter; language/filesystem/modal drawing remain qualified',source='runtime/editor_validation.c',proof='metadata/editor-validation-emulation.json')
    elif token in replay_owned:n.update(state='complete replay callback with actual fallback/profile getter; real I/O and deeper loaders/modal behavior remain qualified',source='runtime/replay_mission.c',proof='metadata/replay-mission-emulation.json')
    elif token in existing_owned:n.update(state='complete load/save/delete/quit callback with actual getter/selector/trim/memset/caret bodies; enumeration/sort and downstream page pixels remain qualified',source='src/unit_00020.c' if token=='00402450' else 'src/unit_00024.c' if token=='00402bb0' else 'runtime/load_save_menu.c',proof='metadata/menu-existing-actions-emulation.json')
    elif token in old_owned:n.update(state='previous complete option callback proof',source='runtime/dropdown_menu_options.c',proof='metadata/dropdown-menu-emulation.json')
    else:n.update(state=e['state'],source=e.get('source'))
    entries.append(n)
missing=[e['token'] for e in entries if e['state'].startswith('missing')]
unrouted=[e['token'] for e in entries if e['source'] and not e['available_in_current_router']]
assert len(entries)==30 and missing==[] and len(unrouted)==0
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'native_sha256':proof['native_sha256'],'complete_menu_reconstruction':False,'unique_tokens':30,'proved_callback_bodies':15,'existing_deeper_bodies_outside_callback_proof':6,'missing_callback_bodies':9,'existing_but_unrouted':unrouted,'missing_tokens':missing,'current_click_routes_available':sum(e['available_in_current_router'] for e in entries),'new_cases':proof['checked'],'new_comparisons':proof['comparisons'],'new_proof':{'path':'metadata/dropdown-actions-emulation.json','sha256':sha(ROOT/'metadata/dropdown-actions-emulation.json')},'limitation':'The count is callback bodies in these two proofs. It is not a count of complete settings pages, downstream dialogs, game systems or pixels. The previous frozen routing contract is retained unchanged.','entries':entries}
out.update(proved_callback_bodies=30,existing_deeper_bodies_outside_callback_proof=0,missing_callback_bodies=0)
out['new_cases']+=system['checked']+start['checked']+remaining['checked']+popup['checked']+editor['checked']+replay['checked']+existing['checked'];out['new_comparisons']+=system['comparisons']+start['comparisons']+remaining['comparisons']+popup['comparisons']+editor['comparison_count']+replay['comparison_count']+existing['comparison_count']
out['new_proofs']=[out.pop('new_proof')]+[{'path':path,'sha256':sha(ROOT/path)} for path in ['metadata/dropdown-system-actions-emulation.json','metadata/dropdown-start-actions-emulation.json','metadata/remaining-menu-actions-emulation.json','metadata/selection-popup-emulation.json','metadata/editor-validation-emulation.json','metadata/replay-mission-emulation.json','metadata/menu-existing-actions-emulation.json']]
out['additional_popup_callbacks_outside_top_table']=['00403950','004092b0','00409300']
out['limitation']='Top-table callback bodies across nine proofs, including explicitly controlled family/file/OS/media/gameplay and deeper drawing boundaries. All30 callback entries are proved with their stated boundaries; this does not prove complete settings pages, dialogs, game systems or pixels.'
(ROOT/'metadata/dropdown-action-status.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['unique_tokens','proved_callback_bodies','missing_callback_bodies','existing_but_unrouted','current_click_routes_available']}))
