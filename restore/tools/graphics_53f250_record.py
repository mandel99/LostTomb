"""Record the local upper-sprite reconstruction contract and proof inputs."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
load=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
rea=load('metadata/graphics_53f250_rea_function.json');rea=rea.get('data',rea)
calls=load('metadata/graphics_53f250_callers.json')
test=load('metadata/graphics-53f250-emulation.json')
assert test['passed'] and test['checked']==1829
assert test['function_instruction_coverage']==dict(total=165,executed=165,missing=[])
forwarded=[r for r in calls['consumed_or_forwarded'] if any(e['kind']=='forward_to_caller_return' for e in r['endpoints'])]
consumed=[r for r in calls['consumed_or_forwarded'] if any(e['kind']=='consumed' for e in r['endpoints'])]
report=dict(format=1,native_sha256=test['original_sha256'],restored_native_function_count=1,
 native_function='FUN_0053f250',body=rea['normalized_result']['procedure']['body'],
 rea_evidence_id=rea['evidence_id'],analysis_profile=rea['analysis_profile']['digest'],
 evidence_files=['metadata/city-draw-next-native.json','metadata/graphics_53f250_rea_function.json','metadata/graphics_53f250_rea_snapshot.json','metadata/graphics_53f250_callers.json','metadata/graphics-53f250-emulation.json'],
 source_files=['src/unit_02525.c','include/graphics_53f250.h'],
 receiver=dict(native='ECX; thiscall RET 0x0c',source_facade='int FUN_0053f250(int image,int x,int y)',
  explicit_receiver='int restore_sprite_draw_upper(RestoreSpriteLibraries *self,int image,int x,int y)',
  global_manager='0x00f55bb0',proved_call_sites=123,all_same_global=calls['all_bind_same_global']),
 contract=dict(image_range='1..25499 inclusive',next_image='signed dword at receiver+0x30f5cc',
  selection='image < signed next_image selects image; otherwise record zero. Count fallback is not failure by itself.',
  record='64 bytes at receiver+0x180e48+64*selected_index',enabled='byte at record+0x35; any nonzero byte',
  pixels='dword at record+0; stored to 0x00d3b5b8 before signed >0 test; add record+8 dword with x86 wraparound',
  width='record+0x14 word copied to 0x00d3b5be; clipping treats that word as signed',
  height='record+0x16 word; y expression sign-extends it; upper-height subtraction wraps to word',
  geometry=[dict(width=w,x_sub=30*(s-1),y_add=30*s,upper_height_sub=15*s+1) for s,w in enumerate([58,118,178,238,298,358],1)],
  unknown_width='Preserve existing 0x00d3b5e0/0x00d3b5dc/0x00d3b5bc before clipping; width and pixels still update.',
  clipping_order=['FUN_004cdad0','FUN_004cdb80'],
  raster_dispatch={'horizontal_1':'FUN_004105c0 then optional FUN_004109d0','horizontal_2':'FUN_00410660 then optional FUN_00410a70','other':'FUN_00410230 then optional FUN_00410700'},
  color='word at 0x0070e120; reload after base raster, along with horizontal byte and pixel pointer; zero skips color pass',
  returns='EAX=0 for invalid ID, disabled selected record, or signed nonpositive initial pixel pointer; EAX=1 after the clipping/raster path even when fully clipped'),
 return_flow=dict(direct_semantic_consumers_found=len(consumed),native_forwarding_candidates=len(forwarded),forwarding=forwarded,
  caveat='Forwarding through a native drawing epilogue does not establish a meaningful return contract of that caller. No caller C interfaces changed.'),
 verification=dict(passed=test['passed'],cases=test['checked'],instruction_coverage=test['function_instruction_coverage'],
  C_smoke='106 checks in 12 scenarios',method=test['method'],compiled_C_sha256=test['compiled_C_sha256'],
  memory_guards='Exact manager memcmp in C and Unicorn; source and frame pages read-only with full unchanged verification; all 224 draw-state/guard bytes compared.',
  ABI='C inline-assembly measurement only in test boundary checks ESP/EBP and EBX/ESI/EDI preservation. Native emulation checks RET 12, all four saved registers, and distant stack guards.'),
 integration=dict(production='unit_02525.c already belongs to RESTORE_UNITS; no runtime or shared sprite-header changes needed.',
  test_target='restore_graphics_53f250_verify',test_sources=['tests/graphics_53f250.c','src/unit_02525.c'],
  clipper_object_sources=['src/unit_01725.c','src/unit_01726.c'],
  clipper_only_definitions=['FUN_004cdad0=graphics_53f250_hclip_impl','FUN_004cdb80=graphics_53f250_vclip_impl'],
  include='include',link=['restore_data','/BASE:0x10000','/DYNAMICBASE:NO','/FIXED'],
  configure='cmake -S tests/graphics_53f250_project -B build/agent-graphics -G "Visual Studio 18 2026" -A Win32',
  build='cmake --build build/agent-graphics --config Debug --parallel 2',
  smoke='build/agent-graphics/Debug/graphics_53f250_verify.exe',
  differential='python tools/verify_graphics_53f250_emulation.py --exe build/agent-graphics/Debug/graphics_53f250_verify.exe',
  caller_audit='python tools/graphics_53f250_callers.py'),
 limitations=test['limitations'],source_hashes=test['source_hashes'])
(ROOT/'metadata/graphics_53f250_reconstruction.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(restored_native_functions=1,cases=test['checked'],forwarding_candidates=len(forwarded),semantic_consumers=len(consumed))))
