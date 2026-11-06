"""Freeze authored data and proof references, without running PE code."""
from pathlib import Path
import hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import ui_cases as recipes
from registry import symbol
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save(path,value):Path(path).write_bytes((json.dumps(value,indent=2)+'\n').encode())
def packed(v):return json.dumps(v,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
DERIVATIONS={
0x414c40:'Three render calls. Panel=(X+80h,Y+A0h,18h,7); labels groupBh/items5,6 atY+BCh/DCh. The source origins are repaired DWORDs; native colors are loaded DWORDs and pushed intact.',
0x417e80:'CALL4CF1C0 then PUSH Y,X,[F55BE4] and MOV ECX,F55BB0/CALL53E590. Full-DWORD image ID must survive the source view. No top EAX assertion: native returns the child EAX, but callback consumer significance is not established here.',
0x417eb0:'Same callback pattern as417E80, image slotF55BDC. Native receiverF55BB0, three stack words, childRET12; C facade is three cdecl words.',
0x417ee0:'Same callback pattern as417E80, image slotF55BD4. Endpoint result is deliberately opaque; no invented full-EAX API.',
0x417f10:'DB8=X+60h; DB4=Y+A0h. D3B56C=trunc((1024-W)/2); D3B568=trunc((768-H)/2); D3B574=W; D3B570=H, all DWORD stores. Native panel height isAh. Variant1 uses W1025/H769 to prove truncation of negative odd differences to zero.',
0x4484b0:'Signed count785D70-12 gates ratio; nonzero amount78A620 is fallback100 when denominator<=0. Declared ratio returns50; scale(196,50)=declared98. Other case scale(196,100)=declared196. Sprite=(F56494+27h,X+204h,Y+50h+scale). No claim about actual ratio child implementation.',
0x524840:'Gate[F53F48]>5. Native F53F0C/08 are full panel coordinates, player offsets EA5CBC/CC0/FF4/620C andEA4508/50C are PLAYER*49D8 bytes. Two fixtures take missing/present assignment branches, then five independently controlled full32 eligibility results alternate0/1. Text response WIDTH is DWORD and is consumed after each actual reload; resets occur at248F1,24970 and24Bxx.',
0x524e20:'Panel/sprite/header/inset then exactly11 rows IDs4..14. Each row resets DWORD WIDTH; value=(1000+37*j), second X=PX+B0h+WIDTH. Selection compares ID-2. Final text47h vs4Dh follows seeded signed playerEA5D44 and comparisons; footer PY+400(decimal).',
0x5250f0:'Amounts0/500/2000/5000 at PX+7Ch/CCh/11Ch/16Ch; numeric child receives all8 words. Source/native final text uses EA5CCC+PLAYER*49D8. All origins are live final parameters of typed text boundaries.',
0x5253c0:'Seven buttons: first5 have X=PX+80h+j*50h,Y=PY+D8h, selection3..7; footer selections1/2. Native compares full DWORD SELECT, and pushes full DWORD PX/PY-derived coordinates.',
0x5255a0:'Panel/sprite/header/inset; reset WIDTH then label34h:32h. Native ordinal8,4,playerEA5DCC,x=PX+90h+WIDTH,y=PY+130h; footer X=PX+400decimal. BYTE-width input/store traps remain visible.',
0x525720:'Seed population50 versus300, thresholds100/200/300. First case only warning paragraph and final button; second takes all three >= threshold rows, each DWORD width reset/read, selected color gates and final action. Boundary predicate order follows native signed DWORD compares.',
0x525b90:'Faction statusBYTE at77DE58+index*106. Zero => paragraph1 atY+98h; nonzero => paragraph1/3 atY+78h/B8h. Both retain header/footer origins.',
0x52d4e0:'Controlled children52D630/52E6F0/52EC50. First rect X=10decimal, fill F73Bh immediate. Eight small rectangles are column-major X0Fh then77h, each four rows H-56h/42h/2Eh/1Ah. Final rect startsDFh. W/H are full DWORD inputs.',
0x52db10:'Native index is full DWORD784F18; LEA*3; LEA*9; SHL; SUB index; SHL yields106*index. Read BYTE at77DE02+offset. Seeds status0/2 produce labels1/13h at(trunc((W-F0h)/2),H-2Eh). Source current &undefined4 indexing gives424 stride; explicit decoy is0. Trade graph states1/3/5 are outside these two bounded cases.',
0x52e610:'StateBYTEE38DDA=14h; selected DWORD5D2E88*20 selects record1/2 fromF535D0. Count4 admits both. Native(SHL5-SUB)*4 gives124*record+785D74. BYTE+2=1; WORD+30h=5/3 picks label10h/11h. Explicit distinct248-stride source decoys have type0; no null pointer fixture.',
0x52e6f0:'Eight labels, four atX0Fh then four atX77h; Y=H-54h/40h/2Ch/18h; labels0,1,2,3,A6h,A7h,DFh,7. DWORD selected1/15 lights corresponding first/second-column entry withF800h.',
0x5305e0:'Native salary signedWORD7848D8: CMPWORD/JGE then MOVSX/NEG. Seed-5 versus345; label14h:item0/1, centered magnitude5/345. Panels/buttons retain signed full-DWORD origins. Full width reset is native DWORD.',
0x530780:'Descriptor rows are28-byte records; signedWORD+2=10/30/50. Native signed division magic yields delays30/15,60/30,40/20 or450/15,900/30,600/20; labelsAAh+q,BBh+q,C1h+q. Descriptor pointer and both real origin words are arguments of4D6380. Three selected button gates preserved.',
0x5309b0:'Two real cdecl arguments px,py; no receiver argument. Native keypad has10 text cells at Y+23h/41h/5Fh/7Dh, three X columns5/23h/41h; key text uses identical box Y (no+6). Last two centered labels use+6. Gate605978=0/1 omits/includes numeric value98765, all8 words. Origins remain actual final parameters of last two center calls.',
0x532920:'Ten full settings rows. WORD salary is sign extended; values48F8/C94/C98/C9C/CA0/DDC/F08/F0C are native DWORDs. Last three salary additions use native DWORD width reload plusX+11Fh. Each column’s y/selection/title constants are derived from cached PUSH/LEA sites, not current output.',
0x532fe0:'StateBYTE3Fh excludes unlock-popup branches. Count child fullEAX3/4 yields1/2 rows, reevaluated after each row. First column X=X+10h+9; Y=Y+20h+32h+j*14h. Color gate reads BYTE784CE0+ID. Count0/early routes and callback popup graph are not claimed.',
0x533470:'Nine settings rows, signedWORD784B60 selects packed-byte fields vs0. The native unusual nonzero-mode row3 Y is(Y+20h)+6FDh =Y+71Dh and is retained. SignedWORD salary is added to DWORDsC4C/C54. Native4F4FF0 seven arguments remain live; PUSH EAX then4CCE90 receives eight. Current compiled game does the same; no C repair requested for this continuation.',
0x534320:'16 DWORD IDs, all-1 except index0/9. Controlled status child result3/5 is consumed with fullEAX TEST/ADD54h; these values are withinBYTE too, no invented high result domain. Native rows use baseY=Y+20h plus3Ch/78h, and columns wrap by400 after8. Sprite ID is fullF571FC+ID.',
0x534610:'Debug gateEA181C DWORD1; one activeBYTE atEA627C-32h, seven inactive following49D8-byte records. First population DWORD at-10h (123), cap200,spare77,births4,deaths5. Query458F80 receives actual PLAYER1; declared123/124 exercises full-EAX compare andFFFF/F800 style. Dock IDs are ten signedWORDs atEA5CD0+PLAYER*49D8+2*j. Every string/number width accumulates declared step; native outer tail clears fullDWORD WIDTH for the final inactive records.'}
def main():
 inp=json.loads((OUT/'author-inputs.json').read_bytes());cases=recipes.cases()
 evidence=json.loads((OUT/'native-evidence.json').read_bytes());by={int(m['requested_address'],16):m for m in evidence['members']}
 sprite=json.loads((ROOT/'build/actor-animal-following/cohort7-joint-sprite/native-contract-excerpts.json').read_bytes())
 sprite_rec=next(m for m in sprite['records']if int(m['entry'],16)==0x53e590)
 save(OUT/'sprite-endpoint-evidence.json',dict(native_sha256=sprite['native_sha256'],source_cache=sprite['source_cache'],instructions=sprite_rec['instructions'],
   contract='Native ECX=F55BB0, image/x/y stack, RET12; C FUN53E590(image,x,y), caller cleanup. Caller sets ECX explicitly; no fabricated receiver frame.'))
 deriv=[]
 for a,rec in by.items():
  ii=rec['assembly']['items'];windows=[]
  for j,i in enumerate(ii):
   if i['instruction'].startswith('CALL '):windows.append(dict(call_address=i['address'],target=i['instruction'][5:],instructions=ii[max(0,j-25):j+5]))
  deriv.append(dict(entry=f'{a:08x}',derivation=DERIVATIONS[a],origin_input_sites=[i for i in ii if any(s in i['instruction'].lower()for s in('0x00e38da4','0x00e38da8'))],callsite_windows=windows))
 save(OUT/'native-derivation.json',dict(schema='ui25-independent-native-derivation-v1',basis='Pinned instruction bytes and explicit fixture seed RAM only. No emulator or observed native/C results were read to author this packet.',
  declared_boundary_behavior='All TEXT boundaries add17 or273 to the current DWORD E20778; all other side effects empty. Query results explicitly scripted in recipes. This is a bounded caller test and does not assert renderer or child-query equivalence.',members=deriv))
 guards=dict(inp['source_guards'])
 for path in('direct_c/cohort3_replay/replay.py','direct_c/cohort3_replay/registry.py','direct_c/cohort3_replay/compiled_flood_regions/cohort7_compiled_acceptance_cases.py','include/restore_types.h','src/unit_02524.c'):
  guards[path]=digest(ROOT/path)
 maps={k:(ROOT/v['map_path']).read_text()for k,v in inp['compiled_images'].items()}
 providers={token:{label:symbol(text,f'FUN_{pc:08x}')for label,text in maps.items()}for token,(pc,arity)in recipes.HELPERS.items()if any(token in c['expected_endpoint_counts']for c in cases)}
 assert all(hit for byimage in providers.values()for hit in byimage.values())
 for member in inp['members']:
  if int(member['address'],16)in(0x417e80,0x417eb0,0x417ee0):
   member['logical_signature']='opaque sprite-child EAX(void); side effects verified; callback result-consumer significance unresolved'
   member['logical_output']='Endpoint sequence/args/snapshots only; no top-EAX comparison. No void/full32 behavioral claim for unqualified callback consumers.'
 p=dict(schema='cohort8-ui-origin-joint25-protocol-v1',entries=25,cases=50,planned_invocations=200,authored_endpoint_events=sum(len(c['authored_endpoint_order'])for c in cases),
  native=dict(path='build/smoke-game/Pharaoh.exe',sha256=digest(ROOT/'build/smoke-game/Pharaoh.exe')),
  compiled_images=inp['compiled_images'],image_profile_input=dict(path='build/actor-animal-following/cohort8-current-images.json',sha256=digest(ROOT/'build/actor-animal-following/cohort8-current-images.json')),
  source100_metadata='metadata/cohort8-source100-local-value-repairs.json',source100_metadata_historical_sha256=digest(ROOT/'metadata/cohort8-source100-local-value-repairs.json'),
  excluded_entries=[0x52da40,0x409eb0,0x44c1a0,0x4c42b0,0x4e0f90],source_guards=guards,members=inp['members'],endpoint_providers=providers,
  evidence_inputs=[dict(path=path,sha256=digest(ROOT/path))for path in sorted({path for m in inp['members']for path in m['native_cache_inputs']}|{sprite['source_cache'],'build/actor-animal-following/cohort7-joint-sprite/native-contract-excerpts.json'})],
  abi=dict(native='RET0; nullary except5309B0 takes2 DWORD stack words. Incoming ECX is nonzero poison, not a logical receiver. Saved incoming GPRs are stack saves/alignment, not fixture-created raw/model inputs.',canonical='Existing real FUN cdecl entry only, identicalPC named interface omitted. All three image providers present.',result='22 void source procedures: no top-level scalar result. Three sprite callbacks forward opaque child EAX; no full-EAX claim without native caller consumer qualification.',endpoints='Controlled ordinary FUN boundaries only. No raw/model frame hooks. Sprite original thiscall ECX F55BB0/RET12 normalized to actual C three-argument cdecl facade.'),
  coverage_limits=['50 finite caller recipes, not complete function/game equivalence.','Rendering/text/ratio/query/three52D4E0 children are declared controlled endpoints; their algorithms are outside this packet.','52DB10 trade states1/3/5 and532FE0 unlock-popup callbacks remain unvisited.','Three417E sprite callback result-consumer significance remains unqualified; no fabricated EAX result.','Current compiled533470 seven-pending-argument continuation is verified from machine bytes, but old-style C declaration depends on the current compiler cleanup behavior.'],
  source_expansion=False,new_source_credit=0,new_entry_credit=0,new_algorithms=0,execution_performed=False,host_execution=False)
 save(OUT/'protocol.json',p)
 save(OUT/'case-commitments.json',dict(schema='ui25-case-commitments-v1',cases={c['id']:hashlib.sha256(packed(c)).hexdigest()for c in cases}))
 save(OUT/'authored-case-inputs.json',dict(cases=cases))if False else None
 files=['author_inputs.py','author-inputs.json','ui_cases.py','supplemental_registry.py','guard.py','freeze_packet.py','native-evidence.json','native.asm.txt','native-derivation.json','sprite-endpoint-evidence.json','compiled-533470-callsite.json','protocol.json','case-commitments.json','FINDINGS.txt','HANDOFF.txt']
 save(OUT/'manifest.json',dict(schema='ui25-private-packet-manifest-v1',files=[dict(path=f,sha256=digest(OUT/f))for f in files]))
 print(json.dumps(dict(entries=25,cases=len(cases),invocations=sum(c['expected_invocations']for c in cases),endpoint_events=p['authored_endpoint_events'],manifest_sha256=digest(OUT/'manifest.json'))))
if __name__=='__main__':main()
