"""Analysis-only city HUD width/call audit; writes new phase notes/staging diff.

Does not edit any production/test/proof file or execute a game/build. Native
instructions are decoded from the canonical PE and matched to retained Ghidra
records. The emitted patch is a proposal, not an applied change.
"""
from pathlib import Path
import difflib
import hashlib
import json
import re
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_AC_READ, CS_AC_WRITE
from capstone.x86_const import X86_OP_MEM

ROOT=Path(__file__).resolve().parents[1]
NATIVE='metadata/city-ui-next-native.json'
SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
TARGETS={'0051e990':'src/unit_02344.c','0051efb0':'src/unit_02348.c'}
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()==SHA
j=json.loads((ROOT/NATIVE).read_text(encoding='utf-8-sig'))
assert j['native_sha256']==SHA
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
sections=[(u32(table+i*40+12),u32(table+i*40+20),u32(table+i*40+16)) for i in range(u16(pe+6))]
def data(address,n):
 for rva,raw,length in sections:
  if rva<=address-base and address-base+n<=rva+length:
   return image[raw+address-base-rva:raw+address-base-rva+n]
 raise ValueError(hex(address))
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
functions={};patch=[]
for address,source in TARGETS.items():
 body=j['functions'][address];assert not body['has_more'] and len(body['items'])==body['total']
 memory={};calls=[]
 for row in body['items']:
  a=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert data(a,len(raw))==raw
  ins=list(md.disasm(raw,a));assert len(ins)==1 and ins[0].size==len(raw);ins=ins[0]
  if ins.mnemonic=='call':calls.append(dict(site=row['address'],target=f'{ins.operands[0].imm:08x}'))
  for operand in ins.operands:
   if operand.type!=X86_OP_MEM or operand.mem.base or operand.mem.index:continue
   target=operand.mem.disp&0xffffffff
   record=memory.setdefault(target,dict(address=f'{target:08x}',read_widths=set(),write_widths=set(),instructions=[]))
   if operand.access&CS_AC_READ:record['read_widths'].add(operand.size)
   if operand.access&CS_AC_WRITE:record['write_widths'].add(operand.size)
   record['instructions'].append(row)
 before=(ROOT/source).read_text();after=before;corrections=[]
 for target,row in sorted(memory.items()):
  row['read_widths']=sorted(row['read_widths']);row['write_widths']=sorted(row['write_widths'])
  name=f'DAT_{target:08x}'
  macro=re.search(r'^#define '+name+r'\s+\(\*\(([^)]+)\s*\*\)0x'+f'{target:08x}'+r'u\)',before,re.M)
  if not macro:continue
  old_type=macro[1].strip();row['C_macro_type']=old_type
  widths=set(row['read_widths']+row['write_widths'])
  if widths=={4} and old_type=='undefined':
   replacement=macro[0].replace('(undefined *)','(int32_t *)')
   after=after.replace(macro[0],replacement)
   corrections.append(dict(global_name=name,address=f'{target:08x}',old_type=old_type,proposed_type='int32_t',native_width=4,evidence=row['instructions']))
 functions[address]=dict(source=source,instruction_count=body['total'],
  full_body_PE_bytes_match=True,memory=list(memory.values()),calls=calls,proposed_width_corrections=corrections)
 patch.extend(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+source,tofile='b/'+source))
patch_path='metadata/city-ui-phase-widths-staged.patch'
(ROOT/patch_path).write_text(''.join(patch))
report=dict(kind='analysis-only-next-phase-plan',native_sha256=SHA,production_sources_changed=False,
 implementation_released=False,functions=functions,proposed_patch=patch_path,
 frame=dict(manifest='metadata/city-confirm-team-frame.json',image='artifacts/city-confirm-team-frame.bmp',
  observation='Completed 1024x768 framebuffer visibly contains misplaced/repeated panel assets and overlapping dialog borders. This visual observation alone does not isolate a single cause.',
  confirmed_width_effect='Current byte macro makes a stored screen width of1024 read as0 and height768 read as0 inside0051e990; native uses full dwords.'),
 corrections=dict(
  native_controller='0051e990: preserve algorithm and endpoint order; replace twelve byte macros with signed32-bit dword access. Keep e38dc9 byte. Native sprite calls bind ECX=0x00f55bb0; restored three-argument sprite facade already provides this binding.',
  minimap_controller='0051efb0: replace all six byte macros with dword access; clearing f53fd0 must clear4 bytes. Preserve collapsed-panel early return, dirty/force evaluation, both minimap rectangle origins and all three border segments.',
  pointer_argument='0051e990 passes literal0x005c7760 as the button-table address. Existing &PTR_DAT_005c7760 evaluates to that address; do not replace it with the pointer stored at the address.',
  return_contract='Both source interfaces remain void. Native EAX may incidentally forward endpoint results; no meaningful consumed return has been established in these controller bodies.'),
 ownership=dict(requested_after_release=['src/unit_02344.c','src/unit_02348.c'],
  new_files_after_release=['include/city_ui_controllers.h (optional typed controller declarations)','tests/city_ui_controllers.c','tests/city_ui_controllers_project/CMakeLists.txt','tools/verify_city_ui_controllers_emulation.py','metadata/city-ui-controllers-emulation.json'],
  not_owned=['CMakeLists.txt','build.ps1','runtime/startup_probe.c','existing sprite headers','all dependency procedure sources pending coordinator assignment'],
  staging_only=[patch_path,'metadata/city-ui-phase-analysis.json','tools/inspect_city_ui_phase.py']),
 dependencies=[
  dict(procedures=['0051ec90'],sources=['src/unit_02345.c'],severity='direct visible geometry',
   finding='Source still reads panelLeft/panelAlternate/panelState as byte; button-table origins remain truncated even after the two proposed controller fixes. Native body export needed before extending ownership.'),
  dict(procedures=['00537100','00537120'],sources=['src/unit_02454.c','src/unit_02455.c'],severity='direct art identity',
   finding='Top-edge helper sources read f567e4 art base as byte. They must be proved/restored or explicitly remain outside a two-controller claim.'),
  dict(procedures=['0051ed30','0051edd0','0051f0a0','0051f680'],sources=['src/unit_02346.c','src/unit_02347.c','src/unit_02349.c','src/unit_02350.c'],severity='HUD children',
   finding='Source inspection shows further byte globals for panel flags, coordinates, image IDs, status caches and text style IDs. These are unproved dependency candidates, not part of the staged18 width changes.'),
  dict(procedures=['00533e70'],sources=['src/unit_02433.c'],severity='alternate-panel child',
   finding='Source still reads panelLeft/style/coordinate globals as byte; child rendering must be audited before claiming full alternate-panel fidelity.'),
  dict(procedures=['0043e810','00425730','004cd510','004cd9d0','004cda40','004d62f0'],sources=[],severity='drawing/text leaves',
   finding='Initial isolated differential should control these endpoints and compare ordered arguments. Existing minimap and sprite proofs provide separate evidence; they do not make the new controller differential a full-pixel proof.'),
 ],
 proof_plan=dict(
  build='After release, isolated build/agent-city-ui only, actual two source files + owned C oracle + restore_data.lib, Win32 /BASE:0x10000 /DYNAMICBASE:NO /FIXED. Coordinator owns shared registration/build/live capture.',
  compare='Execute every original instruction of both controllers in Unicorn; compare compiled C ordered endpoint calls and arguments, relevant global bytes including surrounding guards, full dirty-dword clear, and byte redraw write.',
  native_ABI='Native0053e590 endpoint is thiscall with ECX global manager and RET12; other listed endpoints are caller-cleaned. Check stack cleanup and EBX/EBP/ESI/EDI preservation independently on native and compiled C boundaries.',
  matrix='Widths640/800/1024 plus unknown/full-dword high-byte values; heights480/600/768; panel origins above255; panel state0/1/2/0x100; mode0/1/2/0x100; full image/style IDs; direct0051efb0 force0/1/0x100 and dirty/special/redraw0/1/0x100/FFFFFFFF. Include collapsed early-return dirty preservation and exact clear of a high-byte-only dirty flag.',
  endpoint_mutation='Native0051e990 reloads dimensions/state after children; controlled endpoint variants should mutate width/panel state/image base/coordinates to catch accidental caching and endpoint ordering.',
  coverage='Require285/285 target original instruction addresses exercised. Add isolated C smoke for1024x768 bottom clear(0,759,1024,9), both minimap origins, border segments, full-ID sprites and dirty flag reset.',
  limits='Controlled raster/text and child controllers in first proof; no complete HUD, dialog, map or pixel correctness claim. Visual re-capture belongs to coordinator after shared integration.'),
 source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in list(TARGETS.values())+[NATIVE,'metadata/city-confirm-team-frame.json','tools/inspect_city_ui_phase.py']})
(ROOT/'metadata/city-ui-phase-analysis.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(native_instruction_counts={k:v['instruction_count'] for k,v in functions.items()},
 proposed_dword_macro_changes={k:len(v['proposed_width_corrections']) for k,v in functions.items()},
 production_sources_changed=False,staged_patch=patch_path),indent=2))
