"""Freeze the city25 byte/source/recipe contract; no PE code execution."""
from pathlib import Path
import hashlib,json,struct,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
import city_cases as recipes
from native_test_pe import PE
from registry import symbol
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save(p,v):Path(p).write_bytes((json.dumps(v,indent=2)+'\n').encode())
def packed(v):return json.dumps(v,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
DERIVE={
0x416a20:'Mode1 cycles six formation slots from current5/6, checks three actualBYTE fields, chooses first eligible. SignedWORD x/y become origin+x+228*y. Saved selection and dirtyBYTE are exact outputs; scroll is controlled void.',
0x446a30:'Five real stack arguments. Validrow1,count4 marksBYTErow+32h. WORD775900 increments31999->32000 or32000->1. Class9 uses WORD slot8 entrance coords; primary-1 triggers signedWORD fallback(-2,3). Population0 and row+36h0 are legitimate no-creation branches; return full tileO+y*228+x, not a count0/invalid-row fixture.',
0x461950:'Four signedDWORD percentage quotas and four capacities use contiguous eightWORD32 locals. Two active buildings1/257 oftype1, mapped by controlled lookup to eligible record0. Param+1 exhausts quota after first removal; param-1 remains negative and removes both. All other records explicitly inactive.',
0x46a320:'Clear ten DWORD group links at9783D8+28*j; native first pass writesBYTEusage at9783A4+group*28. Second pass fills link9783BC+group*28 or invokes allocator; duplicate/high257 and unassigned256 paths. Table guards preserve unused bytes.',
0x46b320:'Clear WORD links at790180+196*j. Native usage markersBYTE7900C0+group*196; linksWORD7900BC+group*196; buildingBYTEgroup at+0C1h. Scripted allocator changes onlyAL; parent stores that actualAL. Final DWORD78D788 is highest marked group, not aBYTE alias.',
0x46f030:'DWORD dock links clear atEAB124+224*j; DWORD usage atEAAF68+group*224; linkedDWORD EAB044+group*224. BuildingBYTEgroup+CAh assigned from actualAL result. Duplicate/high257 and unassigned256 branches.',
0x46f190:'Count257 traverses buildings1..257 with264-byte stride; activeBYTE0==1 and kindWORD10h==B6h. Returns full32 count2/1; cached callers store completeEAX intoBCB9B0.',
0x476750:'One live row, tileO+228+left. DWORD sprite IDs base-1/base/base+7/base+8 exercise inclusive range. Matching tiles OR DWORDflag20h, clearBYTEhighbit and zeroBYTEAE8E7C; no DWORD aliasing of BYTE grids.',
0x47d720:'Native BYTE nibbles0..5 and BYTE orientation marker. Rotation0 matches8*n; rotation4 matchesn; mismatch remains cleared40h while high80h preserved. Row-left-2 is an explicit signed metadata edge with valid positive tile addresses.',
0x47f200:'Full DWORD bridge outputs: count, direction, signed step and tile. Scripted neighbor count3/0 allows two-cell south step228 or west step-1. Actual eight pinned clearance offsets are preserved; final endpoint queried atT+step.',
0x47fb70:'Native BYTE six-state terrain map and DWORD flag4. Turn1 maps[1..6]->[6,5,4,1,2,3]; turn-1 maps->[4,5,6,3,2,1]. Real rows traverse BYTE terrain +1 and DWORD flags+4 independently.',
0x47fc40:'High building257 supplies signedWORD center x/y. Radius clips min0/maxcity-1; native WORD tile->building map selects300. Set only building300 dirtyBYTE3Fh; final scratchx/y are clipped right+1/bottom+1.',
0x482a90:'Rectangle params(3,4,2,1) and(-1,0,2,1), city64. Clip signed edges then OR BYTE80h atO+y*228+x. Current typed BC9768 and scratch counters are DWORD; final TILE=O+(bottom+1)*228+left.',
0x482b50:'Same native rectangle geometry as482A90, AND BYTE7Fh. Guarded neighbors retained. Initial touched values have bit80h set, so the native AND is observable.',
0x4867e0:'Actual native486810 executes with original four arguments plus0,0,width,height. Two positive candidate tiles; chooser2 succeeds and chooser0 clears gate. Native outer consumers4A314F/4ACB4E TEST EAX prove BOOL32 despite canonical source void prototype; retained machine forwards childEAX unchanged.',
0x486810:'Eight actual stack words. One positive row or explicit negative-bounds row[-1] with seeded physically valid metadata cells. Both produce two DWORD candidates. Chooser2 gives signed delta-230 in negative case: native IDIV228 quotient-1,remainder-2. No invented helper/model frame.',
0x488840:'Six real stack words/nativeRET24. Actual56133C executes. Native FLD10.0 yields10, product10*20=200; budget500 gives /5=100, so first priority fails. Flag1 chooses mode3,index2; flag0 computes FILD200/FILD501*9, truncated3. Point pairs are[(0,-2),(1,-1),(2,0),(-1,-1),(0,0),(1,1),(-2,0),(-1,1),(0,2)]. Sprite records64B, signedWORD offsets, BYTEterrain. Ratio is repeated for allPC24/53/64 and fourRC modes; two exact callerST0 values retained. No FPSW/trap proof.',
0x4a2320:'Fixed actor table has one eligible ID300/700 and real signedWORD coords. Native contiguous private buffers hold tile then ID in second30-DWORD half. Endpoint consumes actual stack tile bytes; selector1 and signedWORD current actor distance6 choose return300/remove1 vs return0. No absolute synthetic stack pointer.',
0x4a38a0:'Actor5 group2 list contains child7. Controlled count1, real building300 WORD coords2/3. Controlled position outputDWORD5/6 or-2/3 propagates WORD positions/pixels and DWORD tile. Type15 follower8 query returns7 and receives identical coordinates. Native intentionally writes child7 markerFh in follower branch. Cached scope needs only final RET byte4A3A94.',
0x4de200:'Two five-cell rows of maskBYTE codes16,11,4,8,3. Origin>255, row skip223, source row stride1000. Interleaved DWORD table offsets0/0 or-3/2. Codes replace DWORD flags2/1/4/40h or OR BYTEterrain20h.',
0x4e7200:'Param-5/20 clamps3/9. High building257 updates two DWORD counters by5000/(timer+1), capsBYTEstock; fixed actor loop stops type5Fh actor1 and migrates type19h actor257 states9/10->13, copying WORD coords. Final78D7AC/B0 are full DWORD clamp*330h. Refresh(-15) controlled.',
0x4eb800:'Nine cdecl words; actual external DWORD output pointers. WORD tile actor300 matches kindBYTE19h, or excluded300 follows signedWORDnext301. Negative-x,maxring0 executes four invalid cell checks, returns0, preserving exactx/y and restoredDWORD TILE. No frame or helper result stub.',
0x4f3690:'Actual building300 plus four pinned corner offsets. Stage3/partial mask upgrades four tile DWORD flag/sprite values, subtype4, calls refresh1/2 with sibling offset subtraction. Stage10/full mask changes four264-byte building records to11 and refresh2/1. Known callers discard computedEAX; focal scalar return deliberately unasserted.',
0x4f4320:'Native parent gets controlledAL1 and two DWORD outputs via real private stack pointers (callee4F3370 stores full DWORDs). Copy to WORD actor destination; signedWORD existing pixel inputs and full destination*15 feed move callback. WORD tile map/owner updates preserve IDs400/401. Original4AAD63/4AB4DF consumers TEST AL.',
0x4f4440:'WORD wait counter+1 and full delay. Belowthreshold returnsAL1; reachedthreshold executes actual4F3690 on destination7/8, then zeros previous ownerWORD and returnsAL0. Original4AB47C consumer TEST AL. Real child is not an endpoint.'}
def main():
 inp=json.loads((OUT/'author-inputs.json').read_bytes());cases=recipes.cases();native=PE(ROOT/'build/smoke-game/Pharaoh.exe');md=Cs(CS_ARCH_X86,CS_MODE_32)
 def decoded(a,n):return[dict(address=hex(i.address),bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str)for i in md.disasm(native.data[a-native.base:a-native.base+n],a)]
 supplemental=[]
 for a in(0x4aad63,0x4ab4df,0x4ab47c,0x4a3141,0x4acb40,0x41577d):supplemental.append(dict(role='actual native scalar consumer outside batch function cache',instructions=decoded(a,24)))
 supplemental.append(dict(role='Ghidra omitted last RET',instructions=decoded(0x4a3a94,1)))
 ftol=[]
 for i in md.disasm(native.data[0x56133c-native.base:0x56137c-native.base],0x56133c):
  ftol.append(dict(address=hex(i.address),bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str))
  if i.mnemonic=='ret':break
 supplemental.append(dict(role='actual ST0-consuming int64 helper, never an endpoint',instructions=ftol))
 constants=[dict(address=hex(a),bytes_hex=native.data[a-native.base:a-native.base+8].hex(),double_value=struct.unpack_from('<d',native.data,a-native.base)[0])for a in(0x56f5a8,0x56f658)]
 cache=json.loads((ROOT/'metadata/native-function-audit-cache/batch-2112.json').read_bytes());q=next(r for r in cache['entries']if int(r['requested_address'],16)==0x4f3370)
 supplemental.append(dict(role='private output widths:4F3370 actual DWORD stores, AL success',instructions=q['assembly']['items'][-28:]))
 retained=inp['compiled_images']['retained'];pe=PE(ROOT/retained['path']);text=(ROOT/retained['map_path']).read_text();a=symbol(text,'FUN_004867e0')['va'];w=[]
 for i in md.disasm(pe.data[a-pe.base:a-pe.base+100],a):
  w.append(dict(address=hex(i.address),bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str))
  if i.mnemonic=='ret':break
 call=next(j for j,i in enumerate(w)if i['instruction']=='call '+hex(symbol(text,'FUN_00486810')['va']))
 assert [i['instruction']for i in w[call+1:]]==['add esp, 0x20','pop ebp','ret ']
 save(OUT/'additional-byte-proof.json',dict(native=supplemental,constants=constants,current_retained_4867e0_full_EAX_forwarding=w,
  negative_division=dict(delta=-230,divisor=228,quotient=-1,remainder=-2),ratio=dict(first_conversion=10,product=200,denominator=501,multiplier=9,exact_rational='1800/501',truncated_result=3,all_PC_RC_results_3=True)))
 ev=json.loads((OUT/'native-evidence.json').read_bytes());der=[]
 for m in ev['members']:
  a=int(m['requested_address'],16);ii=m['assembly']['items'];der.append(dict(entry=f'{a:08x}',derivation=DERIVE[a],callsite_windows=[dict(call=i['address'],target=i['instruction'][5:],instructions=ii[max(0,j-18):j+5])for j,i in enumerate(ii)if i['instruction'].startswith('CALL ')]))
 save(OUT/'native-derivation.json',dict(basis='Pinned native bytes and explicit fixture RAM only; no observed replay results used.',members=der,
  declared_boundary='All endpoint values/mutations are explicit controlled boundaries; no real external helper outcome is inferred. Actual children are marked in cases.',FPSW_limits='CW and live raw80 stack values compared; not wholeFPSW, unmasked traps, asynchronous exceptions or hardware/DLL/render equivalence.'))
 source=dict(inp['source_guards'])
 for f in('direct_c/cohort3_replay/replay.py','direct_c/cohort3_replay/registry.py','direct_c/cohort3_replay/compiled_flood_regions/cohort7_compiled_acceptance_cases.py','src/unit_02775.c','include/restore_types.h'):
  source[f]=digest(ROOT/f)
 maps={k:(ROOT/v['map_path']).read_text()for k,v in inp['compiled_images'].items()};used={e['token']for c in cases for e in c['endpoints']}
 providers={token:{label:symbol(t,f'FUN_{recipes.HELPERS[token][0]:08x}')for label,t in maps.items()}for token in sorted(used)}
 for m in inp['members']:
  a=int(m['address'],16);m.update(logical_result_mask=recipes.MASKS.get(a),logical_signature=('AL8 result; one stack word'if a in(0x4f4320,0x4f4440)else'fullEAX32; source stack signature below'if a in recipes.MASKS else'side effects only; scalar return unasserted'),native_cleanup=24 if a==0x488840 else 0)
 p=dict(schema='cohort8-city-grid25-protocol-v1',entries=25,cases=len(cases),planned_invocations=sum(c['expected_invocations']for c in cases),authored_endpoint_events=sum(len(c['authored_endpoint_order'])for c in cases),
  native=dict(path='build/smoke-game/Pharaoh.exe',sha256=digest(ROOT/'build/smoke-game/Pharaoh.exe')),compiled_images=inp['compiled_images'],image_profile_path='build/actor-animal-following/cohort8-current-images.json',
  source100_metadata='metadata/cohort8-source100-local-value-repairs.json',members=inp['members'],source_guards=source,endpoint_providers=providers,
  evidence_inputs=[dict(path=f,sha256=digest(ROOT/f))for f in sorted({p for m in inp['members']for p in m['native_cache_inputs']}|{'build/actor-animal-following/cohort8-city-index-local-views/field-role-and-consumer-proof.json'})],
  ABI='Actual canonical cdecl functions only. NativeRET24 for488840/canonicalRET0; other nativeRET0. No raw/native/model frames. Ordinary sprite endpoint nativeECX=F55BB0/three stack wordsRET12; C global facade three cdecl words. AL endpoints preserve live incomingEAX high24.',
  application='Canonical-only; game/diagnostic4867E0/4F4320/4F4440 absent intentionally, retained present. No new wrappers or retention request.',
  signature_qualification='4867E0 current Cvoid prototype contradicts original fullEAX consumer contract, but retained machine preserves the childEAX exactly; this packet asserts real current forwarding rather than rewriting correct behavior.4F3690 computedID return is discarded by known callers and is not newly exposed/tested.',
  coverage_limits=['Finite62 cases/25 existing entries; no complete function/game claim.','Declared endpoints isolate allocators/queries/actions/rendering, their own algorithms are not proved here.','Only marked actual486810/4F3690/56133C child paths run; no guessed raw frames or zeroed full-register inputs.','Negative bounds/metadata cases are explicit bounded native arithmetic calibrations, not a claim those states occur in normal gameplay.','No fullFPSW/trap or unmasked exception equivalence; all twelve precision/rounding CW combinations keep exception masks set.','Unresolved computed/indirect callers remain outside the bounded consumer proof; scalar significance is not inferred from void C alone.'],
  source_expansion=False,new_source_credit=0,new_entry_credit=0,new_algorithms=0,execution_performed=False,host_execution=False)
 save(OUT/'protocol.json',p);save(OUT/'case-commitments.json',dict(cases={c['id']:hashlib.sha256(packed(c)).hexdigest()for c in cases}))
 files=['author_inputs.py','author-inputs.json','collect_consumers.py','consumer-evidence.json','native-evidence.json','native.asm.txt','city_cases.py','additional-byte-proof.json','native-derivation.json','protocol.json','case-commitments.json','supplemental_registry.py','guard.py','run.py','freeze_packet.py','FINDINGS.txt','HANDOFF.txt']
 save(OUT/'manifest.json',dict(files=[dict(path=f,sha256=digest(OUT/f))for f in files]));print(json.dumps(dict(cases=p['cases'],routes=p['planned_invocations'],endpoint_events=p['authored_endpoint_events'],manifest=digest(OUT/'manifest.json'))))
if __name__=='__main__':main()
