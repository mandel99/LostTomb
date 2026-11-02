"""Repair only guard comma expressions whose entire result is discarded.

Outputs apply_patch plans; never edits procedure source files directly. Calls,
arguments and guards whose results are consumed remain untouched.
"""
from pathlib import Path
import argparse,collections,hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
MESSAGE_PANEL_PROOF='metadata/message-panel-emulation.json'
MESSAGE_PANEL_REWRITTEN={'src/unit_01622.c':'004c5e60','src/unit_01623.c':'004c6c70','src/unit_01625.c':'004c7340'}
MESSAGE_PANEL_SOURCES={f'src/unit_{n:05d}.c' for n in [1619,1620,1621,1622,1623,1624,1625,2523]}|{
 'include/message_panel.h','include/message_panel_calls.h','tests/message_panel.c','tests/message_panel_raster.c','tools/verify_message_panel_emulation.py'}
MESSAGE_PANEL_NATIVE={'metadata/render-dialog-discarded-returns-native.json','metadata/message-panel-next-native.json','metadata/city-ui-next-native.json','metadata/message-panel-support-native.json','metadata/message-panel-dimensions-callers-native.json'}
CANONICAL_SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
def verified_popup_caller_supersession(root=ROOT):
 """Retire one old BYTE-argument literal only for the exact native DWORD wiring."""
 root=root.resolve()
 def checked(relative):
  path=(root/relative).resolve()
  if not path.is_relative_to(root) or not path.is_file():raise ValueError('Missing popup proof input: '+relative)
  return path
 def sha(relative):return hashlib.sha256(checked(relative).read_bytes()).hexdigest()
 path='metadata/selection-popup-emulation.json';proof=json.loads(checked(path).read_text());q=proof.get('qualified_controller',{})
 if proof.get('passed') is not True or proof.get('checked')!=6491 or proof.get('comparisons')!=12982 or proof.get('native_sha256')!=CANONICAL_SHA:raise ValueError('Complete popup module comparison has not passed')
 if not(q.get('complete') is True and q.get('native_instructions')==q.get('executed')==815 and q.get('conditional_edges')==q.get('edges_executed')==132 and q.get('excluded_instructions')==q.get('excluded_edges')==[]):raise ValueError('Popup module coverage incomplete')
 required={'src/unit_00033.c','src/unit_01858.c','src/unit_02421.c','runtime/selection_popup.c','include/selection_popup.h','tests/selection_popup.c','tools/verify_selection_popup_emulation.py'}
 if not required<=proof.get('source_hashes',{}).keys():raise ValueError('Popup proof omits applied module sources')
 for name,digest in {**proof['source_hashes'],**proof.get('native_exports',{})}.items():
  if sha(name)!=digest:raise ValueError('Stale popup module evidence: '+name)
 if set(proof.get('variants',{}))!={'Debug','O2'}:raise ValueError('Both popup C configurations required')
 for variant in proof['variants'].values():
  if variant.get('checked')!=6491 or sha(variant['path'])!=variant['sha256']:raise ValueError('Stale popup C binary')
 recipe='artifacts/selection_popup_phase/metadata/integration-recipe.json'
 if sha(recipe)!='cf136b2c74a42a93d643f7e199b7f8b407d661a668613e36334655aac0b49031':raise ValueError('Popup wiring recipe drift')
 if sha('metadata/selection-popup-integration-native-windows.json')!='2801f025849dcb8513bacd25d6ff68ceea4af4db4e1ef0f1e1a4a8df021d2e2e':raise ValueError('Popup native caller evidence drift')
 item=next(x for x in json.loads(checked(recipe).read_text())['root_owned_narrow_wiring'] if x['path']=='src/unit_01866.c')
 backup='artifacts/integration-batch-125-before/src/unit_01866.c';before=checked(backup).read_bytes()
 if sha(backup)!=item['expected_before_sha256']:raise ValueError('Popup caller predecessor drift')
 eol=b'\r\n' if before.count(b'\r\n')==before.count(b'\n') else b'\n'
 old=item['expected_substring'].encode().replace(b'\n',eol);new=item['replacement'].encode().replace(b'\n',eol)
 if before.count(old)!=1:raise ValueError('Popup caller predecessor literal drift')
 if before.replace(old,new,1)!=checked(item['path']).read_bytes():return verified_initial_dispatch_supersession(root)
 return {'file':item['path'],'replacement_kind':'narrow-native-caller-wiring','whole_caller_procedure_proven':False,'function':'0x004d71c0','evidence':path,'evidence_sha256':sha(path),'cases':6491,'source_sha256':sha(item['path']),'old_literal_required':False,'reason':'Same discarded renderer invocation, with original DWORD loads in rows/y/x order; every other caller byte retained.'}

def verified_initial_dispatch_supersession(root=ROOT):
 """Retire the old literal only after the complete production dispatcher proof."""
 root=root.resolve()
 def sha(relative):
  p=(root/relative).resolve()
  if not p.is_relative_to(root) or not p.is_file():raise ValueError('Missing dispatcher proof input: '+relative)
  return hashlib.sha256(p.read_bytes()).hexdigest()
 path='metadata/initial-menu-dispatch-emulation.json';proof=json.loads((root/path).read_text())
 if not(proof.get('passed') is True and proof.get('production') is True and proof.get('complete_case_set') is True and proof.get('checked')==proof.get('complete_cases')==5795 and proof.get('comparisons')==23180 and proof.get('native_sha256')==CANONICAL_SHA):raise ValueError('Complete production initial-dispatch proof has not passed')
 for key,want in {'native_instruction_coverage':[179,179],'conditional_outcomes':[8,8],'switch_mode_coverage':[88,88],'switch_target_coverage':[47,47],'native_call_site_coverage':[58,58],'deep_endpoint_coverage':[50,50],'mutation_phases':100,'source_drift':False}.items():
  if proof.get(key)!=want:raise ValueError('Incomplete initial dispatcher evidence: '+key)
 required={'src/unit_01866.c','include/initial_menu_dispatch.h','runtime/initial_menu_dispatch_boundary.c','tools/verify_initial_menu_dispatch_emulation.py','artifacts/initial_menu_dispatch_phase/tests/initial_dispatch.c','artifacts/initial_menu_dispatch_phase/tests/actual_trace.c','artifacts/initial_menu_dispatch_phase/tests/actual_plain.c','artifacts/initial_menu_dispatch_phase/tests/renderer_boundaries.inc','artifacts/initial_menu_dispatch_phase/native-provider.json'}
 if not required<=proof.get('source_hashes',{}).keys():raise ValueError('Initial dispatcher proof omits actual source inputs')
 for p,h in proof['source_hashes'].items():
  if sha(p)!=h:raise ValueError('Stale initial dispatcher input: '+p)
 if sha('artifacts/initial_menu_dispatch_phase/native-provider.json')!='408c3e945a69dc9f0c505771106d11ea12fa5c78b4c6983cf1ff4718c6c7dbe9':raise ValueError('Initial dispatcher native scope drift')
 if set(proof.get('variants',{}))!={'Debug_trace','Debug_plain','O2_trace','O2_plain'}:raise ValueError('All four dispatcher variants required')
 for label,v in proof['variants'].items():
  expected='build/'+('c-only-debug/Debug/' if label.startswith('Debug') else 'c-only/Release/')+'restore_initial_menu_dispatch_verify.exe'
  if v.get('path')!=expected or sha(expected)!=v.get('sha256'):raise ValueError('Stale or private initial dispatcher binary')
 return {'file':'src/unit_01866.c','replacement_kind':'complete-procedure','whole_caller_procedure_proven':True,'function':'0x004d71c0','evidence':path,'evidence_sha256':sha(path),'cases':5795,'source_sha256':sha('src/unit_01866.c'),'old_literal_required':False,'limitations':proof.get('limitations',[]),'reason':'All179 original caller instructions,88 mode entries,47 jump targets and58 call sites compared against actual compiled C; deeper renderer algorithms remain qualified.'}
def verified_combat_supersession(root=ROOT):
 """Retire the old param_1 literal only with the full production body proof."""
 root=root.resolve()
 def sha(relative):
  path=(root/relative).resolve()
  if not path.is_relative_to(root) or not path.is_file():raise ValueError('Missing combat proof input: '+relative)
  return hashlib.sha256(path.read_bytes()).hexdigest()
 path='metadata/actor-combat-emulation.json';proof=json.loads((root/path).read_text())
 if proof.get('passed') is not True or proof.get('production') is not True or proof.get('checked')!=2989 or proof.get('native_sha256')!=CANONICAL_SHA:
  raise ValueError('Complete production combat proof has not passed')
 if proof.get('instructions_executed')!=515 or proof.get('instructions_total')!=515:
  raise ValueError('Combat graph instruction coverage is incomplete')
 expected={'0x4b17d0':387,'0x4b3250':17,'0x4b1d10':23,'0x4b1da0':18,'0x4b3f00':50,'0x4ea9a0':9,'0x4ea9c0':11}
 for entry,total in expected.items():
  item=proof.get('instruction_coverage',{}).get(entry,{})
  if item.get('executed')!=total or item.get('total')!=total or item.get('missing')!=[]:raise ValueError('Incomplete combat entry: '+entry)
 required={f'src/unit_{n:05d}.c' for n in [1414,1415,1417,1435,1427,2065,2066,1130]}|{'include/actor_combat.h','runtime/actor_combat_boundaries.c','tests/actor_combat.c','tools/verify_actor_combat_emulation.py','metadata/actor-combat-native.json'}
 hashes=proof.get('source_hashes',{})
 if not required<=hashes.keys():raise ValueError('Combat proof omits complete-body inputs')
 for relative,digest in hashes.items():
  if sha(relative)!=digest:raise ValueError('Stale combat proof input: '+relative)
 binary='build/c-only-debug/Debug/restore_actor_combat_verify.exe'
 if sha(binary)!=proof.get('compiled_C_sha256'):raise ValueError('Stale combat comparison binary')
 return {'file':'src/unit_01414.c','function':'0x004b17d0','replacement_kind':'complete-procedure','evidence':path,'evidence_sha256':sha(path),'cases':proof['checked'],'source_sha256':hashes['src/unit_01414.c'],'limitations':proof.get('limitations',[])}

def verified_training_panel_supersession(root=ROOT):
 """Retire seven old drawing-call literals only for the whole native panel proof."""
 root=root.resolve()
 def sha(relative):
  p=(root/relative).resolve()
  if not p.is_relative_to(root) or not p.is_file():raise ValueError('Missing panel proof input: '+relative)
  return hashlib.sha256(p.read_bytes()).hexdigest()
 path='metadata/training-panel-emulation.json';proof=json.loads((root/path).read_text())
 if not(proof.get('passed') is True and proof.get('checked')==proof.get('plain_checked')==proof.get('comparison_checked')==3865 and proof.get('comparison_count')==15460 and proof.get('native_sha256')==CANONICAL_SHA):raise ValueError('Complete training panel comparison has not passed')
 if proof.get('case_selection')!={'limit':None,'start':0,'single_case':False}:raise ValueError('Partial panel case selection')
 for key,want in {'native_instruction_coverage':[1001,1001],'native_conditional_outcomes':[90,90],'feasible_computed_edges':[15,15],'native_uncovered_instructions':[],'missing_conditional_outcomes':[]}.items():
  if proof.get(key)!=want:raise ValueError('Incomplete panel native coverage: '+key)
 if set(proof.get('declared_infeasible_computed_edges',{}))!={'0x55b343->0x55b359'} or len(proof.get('mutated_helper_phases',[]))!=26:raise ValueError('Panel CFG/mutation qualification differs')
 required={'src/unit_02637.c','include/training_panel.h','tools/verify_training_panel_emulation.py','artifacts/training_panel_phase/tests/training_panel.c','artifacts/training_panel_phase/tests/training_actual_trace.c','artifacts/training_panel_phase/tests/training_actual_plain.c','artifacts/training_panel_phase/retained-native.json','artifacts/training_panel_phase/abi-contract.json'}
 if not required<=proof.get('source_hashes',{}).keys():raise ValueError('Panel proof omits applied inputs')
 for p,h in proof['source_hashes'].items():
  if sha(p)!=h:raise ValueError('Stale panel source evidence: '+p)
 if sha('tools/verify_training_panel_emulation.py')!='2fd4eef2ed7fc86ac3e6324fb1a082437bf4167a43f6165134b2a2dab519b2a6':raise ValueError('Frozen panel comparator changed')
 if sha('artifacts/training_panel_phase/retained-native.json')!='c70264590a0670702d433cce14dd42271fea3073ea85f92b44ba20ba1ec0969d':raise ValueError('Panel native scope changed')
 for p,key in [('build/c-only-debug/Debug/restore_training_panel_verify.exe','compiled_C_sha256'),('build/c-only/Release/restore_training_panel_verify.exe','comparison_C_sha256')]:
  if sha(p)!=proof.get(key):raise ValueError('Stale or private panel binary')
 return {'file':'src/unit_02637.c','function':'0x0055ad30','replacement_kind':'complete-procedure','whole_procedure_proven':True,'evidence':path,'evidence_sha256':sha(path),'cases':3865,'source_sha256':sha('src/unit_02637.c'),'old_literal_required':False,'limitations':proof.get('limitations',[])}
def verified_message_panel_proof(root=ROOT):
 """Fail closed before retiring a literal covered by the complete C proof."""
 root=root.resolve()
 def checked_path(relative):
  path=(root/relative).resolve()
  if not path.is_relative_to(root) or not path.is_file():raise ValueError('Missing or non-workspace proof input: '+str(relative))
  return path
 def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
 proof_path=checked_path(MESSAGE_PANEL_PROOF)
 proof=json.loads(proof_path.read_text(encoding='utf-8-sig'))
 if proof.get('passed') is not True or proof.get('complete_case_set') is not True or not isinstance(proof.get('checked'),int) or proof['checked']<=0:
  raise ValueError('Complete message-panel native comparison has not passed')
 if proof.get('native_sha256')!=CANONICAL_SHA:raise ValueError('Message-panel native identity differs')
 hashes=proof.get('source_hashes',{})
 if not MESSAGE_PANEL_SOURCES<=hashes.keys():raise ValueError('Message-panel proof omits required complete-procedure sources')
 for relative,digest in hashes.items():
  if sha(checked_path(relative))!=digest:raise ValueError('Message-panel source evidence is stale: '+relative)
 exports=proof.get('native_exports',{})
 if not MESSAGE_PANEL_NATIVE<=exports.keys():raise ValueError('Message-panel proof omits required native bodies/caller bindings')
 for relative,digest in exports.items():
  if sha(checked_path(relative))!=digest:raise ValueError('Message-panel native export is stale: '+relative)
 probes=proof.get('branch_probes',{})
 if not probes or any(v.get('reached') is not True for v in probes.values()):raise ValueError('Message-panel selected native probes are incomplete')
 image=checked_path(proof.get('compiled_C_path',''))
 if sha(image)!=proof.get('compiled_C_sha256'):raise ValueError('Message-panel compiled C evidence is stale')
 return proof,{'path':MESSAGE_PANEL_PROOF,'sha256':sha(proof_path),'cases':proof['checked'],'compiled_C_path':proof['compiled_C_path'],'compiled_C_sha256':proof['compiled_C_sha256'],'all_source_hashes_current':True,'complete_case_set':True}
PATTERN=re.compile(r'(?m)^([ \t]*)\((FUN_([0-9a-f]{8})\([^;]*\)), restore_unresolved_return\(0x([0-9a-f]{8})u\)\);[ \t]*$')
def valid(match):return match[3]==match[4] and 'restore_unresolved_return' not in match[2]
def repair(text):return PATTERN.sub(lambda m:m[1]+m[2]+';' if valid(m) else m[0],text)
def plan():
 entries=[]
 for file in sorted((ROOT/'src').glob('*.c')):
  before=file.read_text();after=repair(before)
  if before==after:continue
  changes=[{'before':m[0],'after':m[1]+m[2]+';','callee':'0x'+m[3],'line':before.count('\n',0,m.start())+1} for m in PATTERN.finditer(before) if valid(m)]
  entries.append({'file':file.relative_to(ROOT).as_posix(),'before_sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'changes':changes})
 return {'format':1,'reason':'Only the result-discarded top-level comma statement is replaced; its exact function invocation and argument expressions are retained. Consumed/assigned returns are not synthesized or removed.','sites':sum(len(e['changes']) for e in entries),'entries':entries}
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--plan',action='store_true');parser.add_argument('--patch',action='store_true');parser.add_argument('--verify',action='store_true');parser.add_argument('--offset',type=int,default=0);parser.add_argument('--limit',type=int,default=8);args=parser.parse_args()
 path=ROOT/'metadata/discarded-return-repairs.json'
 if args.plan:
  report=plan();path.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'sites':report['sites'],'files':len(report['entries'])}));return
 report=json.loads(path.read_text())
 base_sites=report['sites'];extra_sites=0
 extra_path=ROOT/'metadata/discarded-return-multiline-repairs.json'
 if extra_path.exists():
  extra=json.loads(extra_path.read_text());extra_sites=extra['sites']
  report={**report,'sites':base_sites+extra_sites,'entries':report['entries']+extra['entries']}
 if args.patch:
  print('*** Begin Patch')
  for e in report['entries'][args.offset:args.offset+args.limit]:
   print('*** Update File: '+(ROOT/e['file']).as_posix())
   for c in e['changes']:print('@@\n-'+c['before']+'\n+'+c['after'])
  print('*** End Patch');return
 if args.verify:
  issues=[];hashes={};adapted=[];reconstructed=[];message_summary=None;message_error=None
  for e in report['entries']:
   source=(ROOT/e['file']).read_text();hashes[e['file']]=hashlib.sha256((ROOT/e['file']).read_bytes()).hexdigest()
   for line,count in collections.Counter(c['after'] for c in e['changes']).items():
    exact=len(re.findall(r'(?m)^'+re.escape(line)+r'$',source))
    # Indentation carries no call semantics; keep multiline literals exact.
    indented=0 if '\n' in line else sum(s.strip()==line.strip() for s in source.splitlines())
    if max(exact,indented)<count:
     # This later, separately evidenced ABI repair adds the native receiver.
     revised=line.replace('FUN_0055f561(', 'FUN_0055f561(RESTORE_SOUND,')
     callers=json.loads((ROOT/'metadata/sound-stream-caller-repairs.json').read_text()) if (ROOT/'metadata/sound-stream-caller-repairs.json').exists() else []
     if e['file']=='src/unit_01866.c' and line.strip()=='FUN_00530d70(DAT_00605954,DAT_0060594c,DAT_00605950);' and count==1:
      try:reconstructed.append({**verified_popup_caller_supersession(ROOT),'before':line})
      except Exception as error:issues.append({'file':e['file'],'missing':line,'reason':str(error)})
     elif e['file']=='src/unit_01414.c' and line.strip()=='FUN_004b1da0(param_1);' and count==1:
      try:reconstructed.append({**verified_combat_supersession(ROOT),'before':line})
      except Exception as error:issues.append({'file':e['file'],'missing':line,'reason':str(error)})
     elif revised!=line and any(c['file']==e['file'] for c in callers) and source.splitlines().count(revised)>=count:
      adapted.append({'file':e['file'],'before':line,'after':revised,'evidence':'metadata/sound-stream-callers-native.json'})
     elif e['file']=='src/unit_00318.c' and line.strip()=='FUN_0041fca0(3);' and source.splitlines().count(line.replace('FUN_0041fca0(3)','FUN_0041fca0(in_ECX,3)'))>=count:
      evidence=json.loads((ROOT/'metadata/record-state-callers-native.json').read_text())
      assert any(c['address']=='0x0041fdb0' and c['file']==e['file'] and c['calls'] for c in evidence['callers'])
      adapted.append({'file':e['file'],'before':line,'after':line.replace('FUN_0041fca0(3)','FUN_0041fca0(in_ECX,3)'),'evidence':'metadata/record-state-callers-native.json'})
     elif e['file']=='src/unit_00579.c' and line.strip()=='FUN_0053f4a0(iVar9,iVar13,iVar14);':
      # The full native minimap reconstruction supersedes the old local names.
      # Accept it only while its actual C source is covered by a passing proof.
      proof_path=ROOT/'metadata/minimap-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      revised='            FUN_0053f4a0(image,screen_x,screen_y);continue;'
      if proof.get('passed') and proof.get('source_hashes',{}).get(e['file'])==hashes[e['file']] and source.splitlines().count(revised)>=count:
       reconstructed.append({'file':e['file'],'before':line,'after':revised,'evidence':'metadata/minimap-emulation.json','cases':proof['checked']})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Full native minimap reconstruction has not been freshly verified'})
     elif e['file']=='src/unit_00468.c' and line.strip() in {'FUN_004316f0();','FUN_0043e300();'}:
      proof_path=ROOT/'metadata/viewport-controller-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      fresh=proof.get('passed') is True and proof.get('native_sha256')==CANONICAL_SHA and proof.get('native_instruction_coverage')==[76,76]
      required={'src/unit_00468.c','src/unit_01878.c','include/viewport_controller.h','tests/viewport_controller.c','tools/verify_viewport_controller_emulation.py'}
      fresh=fresh and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in proof.get('source_hashes',{}).items())
      for name in ['restore_viewport_controller_verify.exe','restore_viewport_controller_plain_verify.exe']:
       image=ROOT/'build/c-only-debug/Debug'/name
       fresh=fresh and image.is_file() and proof.get('compiled_C_variants_sha256',{}).get(name)==hashlib.sha256(image.read_bytes()).hexdigest()
      if fresh:
       reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x00430d30','evidence':'metadata/viewport-controller-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete viewport source and traced/plain C evidence is not fresh'})
     elif e['file']=='src/unit_00547.c':
      proof_path=ROOT/'metadata/world-render-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      image=ROOT/'build/c-only-debug/Debug/restore_world_render_verify.exe'
      required={'src/unit_00547.c','src/unit_00548.c','src/unit_00549.c','include/world_render.h','tools/verify_world_render_emulation.py','metadata/world_render_member_binding_audit.json'}
      fresh=proof.get('passed') is True and proof.get('native_sha256')==CANONICAL_SHA
      fresh=fresh and proof.get('case_selection')=={'start':0,'limit':None,'single_case':False} and proof.get('checked')==11080
      fresh=fresh and proof.get('reachable_native_instruction_coverage')==[1398,1398] and proof.get('helper_instruction_coverage')==[12,12]
      fresh=fresh and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in proof.get('source_hashes',{}).items())
      fresh=fresh and image.is_file() and hashlib.sha256(image.read_bytes()).hexdigest()==proof.get('compiled_C_sha256')
      if fresh:reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x00436b60','evidence':'metadata/world-render-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete production world controller proof is not fresh'})
     elif e['file']=='src/unit_00565.c':
      proof_path=ROOT/'metadata/placement-preview-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      image=ROOT/'build/c-only-debug/Debug/restore_placement_preview_verify.exe'
      required={f'src/unit_{n:05d}.c' for n in [565,2115,2527]}|{
       'include/placement_preview.h','runtime/placement_preview_members.c','runtime/placement_preview_predicates.c',
       'runtime/placement_preview_platform.c','runtime/preview_footprint_seeds.c','include/preview_footprint_seeds.h',
       'tests/placement_preview.c','tests/placement_preview_helpers.c','tools/verify_placement_preview_emulation.py'}
      fresh=proof.get('passed') is True and proof.get('complete_case_set') is True and proof.get('stage')=='production'
      fresh=fresh and proof.get('native_sha256')==CANONICAL_SHA and proof.get('checked')==3938
      fresh=fresh and proof.get('controller_instruction_coverage')=={'executed':1963,'total':1963,'unreached':[]}
      fresh=fresh and proof.get('newly_restored_native_instructions')==2860 and proof.get('existing_geometry_native_instructions')==99
      fresh=fresh and proof.get('startup_profiles')==72 and proof.get('seed_records_independently_recovered')==1327
      fresh=fresh and proof.get('seed_native_prefix_instructions')==17497 and proof.get('unqualified_missing_conditional_outcomes')==[]
      coverage=proof.get('native_function_instruction_coverage',{})
      fresh=fresh and bool(coverage) and all(v.get('executed')==v.get('total') and v.get('unreached')==[] for v in coverage.values())
      fresh=fresh and set(proof.get('unknown_or_truncated_tokens_rejected',[]))=={'0x0','0x40','0x60','0x80','0x405b00','0xdeadbeef'}
      fresh=fresh and proof.get('original_machine_code_fallback_used') is False and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in {**proof.get('source_hashes',{}),**proof.get('native_exports',{})}.items())
      fresh=fresh and image.is_file() and hashlib.sha256(image.read_bytes()).hexdigest()==proof.get('compiled_C_sha256')
      if fresh:reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x0043b630','evidence':'metadata/placement-preview-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete production preview/output-helper and seeded startup proof is not fresh'})
     elif e['file']=='src/unit_02116.c':
      proof_path=ROOT/'metadata/actor-coordinates-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      image=ROOT/'build/c-only-debug/Debug/restore_actor_coordinates_verify.exe'
      required={f'src/unit_{n:05d}.c' for n in [2113,2114,2115,2116]}|{
       'include/actor_coordinates.h','tests/actor_coordinates.c','tools/verify_actor_coordinates_emulation.py'}
      fresh=proof.get('passed') is True and proof.get('native_sha256')==CANONICAL_SHA
      fresh=fresh and proof.get('checked')==4771 and proof.get('case_selection')=={'start':0,'limit':None,'single_case':False}
      fresh=fresh and proof.get('native_instruction_coverage')==[403,403] and proof.get('controlled_algorithm_endpoints')==[]
      fresh=fresh and proof.get('function_instruction_coverage')=={'0x4f08c0':[166,166],'0x4f0ab0':[78,78],'0x4f0d30':[159,159],'0x4f0ba0':[113,113]}
      fresh=fresh and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in {**proof.get('source_hashes',{}),**proof.get('native_exports',{})}.items())
      fresh=fresh and image.is_file() and hashlib.sha256(image.read_bytes()).hexdigest()==proof.get('compiled_C_sha256')
      if fresh:reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x004f0d30','evidence':'metadata/actor-coordinates-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete actor-coordinate/output-helper production proof is not fresh'})
     elif e['file']=='src/unit_01632.c':
      proof_path=ROOT/'metadata/rich-message-text-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      image=ROOT/'build/c-only-debug/Debug/restore_rich_message_text_verify.exe'
      required={f'src/unit_{n:05d}.c' for n in range(1630,1636)}|{'include/rich_message_text.h','tests/rich_message_text.c','tests/rich_message_text_raster.c','tools/verify_rich_message_text_emulation.py'}
      fresh=proof.get('passed') is True and proof.get('complete_case_set') is True and proof.get('native_sha256')==CANONICAL_SHA
      fresh=fresh and proof.get('checked')==4106 and proof.get('native_instructions_verified')==669 and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and len(proof.get('branch_probes',{}))==40 and all(v.get('reached') is True for v in proof.get('branch_probes',{}).values())
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in {**proof.get('source_hashes',{}),**proof.get('native_exports',{})}.items())
      fresh=fresh and image.is_file() and hashlib.sha256(image.read_bytes()).hexdigest()==proof.get('compiled_C_sha256')
      if fresh:reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x004c8070','evidence':'metadata/rich-message-text-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete production rich-text proof is not fresh'})
     elif e['file']=='src/unit_01626.c':
      proof_path=ROOT/'metadata/message-panel-input-emulation.json'
      proof=json.loads(proof_path.read_text()) if proof_path.exists() else {}
      image=ROOT/'build/c-only-debug/Debug/restore_message_panel_input_verify.exe'
      required={f'src/unit_{n:05d}.c' for n in [407,1626,1627,1628,1629]}|{'runtime/message_panel_input.c','include/message_panel_input.h','tests/message_panel_input.c','tests/message_panel_input_buttons.c','tools/verify_message_panel_input_emulation.py'}
      fresh=proof.get('passed') is True and proof.get('complete_case_set') is True and proof.get('stage')=='production' and proof.get('native_sha256')==CANONICAL_SHA
      fresh=fresh and proof.get('checked')==3145 and proof.get('newly_restored_native_instructions')==631 and required<=proof.get('source_hashes',{}).keys()
      fresh=fresh and len(proof.get('branch_probes',{}))==40 and all(v.get('reached') is True for v in proof.get('branch_probes',{}).values())
      fresh=fresh and all((ROOT/path).is_file() and hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==digest for path,digest in {**proof.get('source_hashes',{}),**proof.get('native_exports',{})}.items())
      fresh=fresh and image.is_file() and hashlib.sha256(image.read_bytes()).hexdigest()==proof.get('compiled_C_sha256')
      if fresh:reconstructed.append({'file':e['file'],'before':line,'replacement_kind':'complete-procedure','function':'0x004c7770','evidence':'metadata/message-panel-input-emulation.json','cases':proof['checked'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':'Complete production message input proof is not fresh'})
     elif e['file']=='src/unit_02637.c':
      try:reconstructed.append({**verified_training_panel_supersession(ROOT),'before':line})
      except Exception as error:issues.append({'file':e['file'],'missing':line,'reason':str(error)})
     elif e['file'] in MESSAGE_PANEL_REWRITTEN:
      # Exactly these three historical files now contain completely proved
      # handwritten procedures; their old local-variable literals are retired.
      if message_summary is None and message_error is None:
       try:_,message_summary=verified_message_panel_proof(ROOT)
       except Exception as error:message_error=str(error)
      if message_summary is not None:
       reconstructed.append({'file':e['file'],'function':'0x'+MESSAGE_PANEL_REWRITTEN[e['file']],'before':line,'replacement_kind':'complete-procedure','old_literal_required':False,'historical_sites':count,'evidence':MESSAGE_PANEL_PROOF,'evidence_sha256':message_summary['sha256'],'cases':message_summary['cases'],'source_sha256':hashes[e['file']]})
      else:issues.append({'file':e['file'],'missing':line,'reason':message_error or 'Complete message-panel proof is unavailable'})
     else:issues.append({'file':e['file'],'missing':line})
  remaining=plan()
  output={'format':1,'passed':not issues and not remaining['sites'],'verified_sites':report['sites'],'base_sites':base_sites,'multiline_sites':extra_sites,'files':len(hashes),'remaining_discarded_guard_sites':remaining['sites'],'issues':issues,'later_explicit_receiver_repairs':adapted,'later_complete_reconstructions':reconstructed,'source_hashes':hashes}
  if message_summary is not None:output['message_panel_superseding_proof']=message_summary
  (ROOT/'metadata/discarded-return-verification.json').write_text(json.dumps(output,indent=2)+'\n');print(json.dumps({k:v for k,v in output.items() if k!='source_hashes'}));raise SystemExit(0 if output['passed'] else 1)
 parser.error('Use --plan, --patch or --verify')
if __name__=='__main__':main()
