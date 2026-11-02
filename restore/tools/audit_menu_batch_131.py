"""Audit actual whole-refresh integration and complete qualified native evidence."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,struct,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
app=read('metadata/integration-batch-131-application.json')
assert sha(ROOT/app['predecessor'])==app['predecessor_sha256']
for p,h in {**app['written'],**app['protected_unchanged']}.items():assert sha(ROOT/p)==h,('application drift',p)
f=app['frozen_manifest'];assert sha(ROOT/f['path'])==f['sha256']=='ddd403e31bc13a45fc51c88838cbc99342ef8f78b2e94e4f5d31e71e9f59271b'
frozen=read(f['path'])
for p,h in frozen['files'].items():assert sha(ROOT/p)==h,('frozen drift',p)
for p in ['src/unit_01867.c','include/refresh_menu_dispatch.h','runtime/refresh_menu_dispatch_boundary.c']:assert (ROOT/p).read_bytes()==(ROOT/'artifacts/refresh_menu_dispatch_phase'/p).read_bytes(),p
assert sha(ROOT/'artifacts/integration-batch-131-before/src/unit_01867.c')==app['old_controller_sha256']
proof=read('metadata/refresh-menu-dispatch-emulation.json')
assert proof['passed'] and proof['production'] and proof['complete_case_set']
assert proof['checked']==proof['complete_cases']==11633 and proof['comparisons']==23266
assert proof['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert proof['native_instruction_coverage']==[960,960] and proof['controller_instruction_coverage']==[917,917] and proof['conditional_outcomes']==[250,250]
assert proof['uncovered_instructions']==proof['missing_conditional_outcomes']==[]
assert len(proof['reached_endpoints'])==96 and len(proof['mutated_endpoint_phases'])==192
assert proof['case_selection']=={'limit':None,'single_case':False} and not proof['native_game_executed']
assert proof['native_writes']==[['0x7056c8',4],['0xe38dc9',1],['0xe38df8',1]]
required={'src/unit_01867.c','include/refresh_menu_dispatch.h','runtime/refresh_menu_dispatch_boundary.c','tools/verify_refresh_menu_dispatch_emulation.py','CMakeLists.txt'}
assert required<=proof['source_hashes'].keys()
for p,h in {**proof['source_hashes'],**proof['binary_hashes']}.items():assert sha(ROOT/p)==h,('production proof drift',p)
contract=read('artifacts/refresh_menu_dispatch_phase/fixture-contracts.json');schema=read('artifacts/refresh_menu_dispatch_phase/fixture-schema.json')
assert len(contract['controlled_endpoints'])==96 and len(schema['fields'])==49 and len(schema['regions'])==15 and sum(n for a,n in schema['regions'])==222728
assert {(int(x['native_entry'],16),phase) for x in contract['controlled_endpoints'] for phase in [1,2]}=={(int(a,16),phase) for a,phase in proof['mutated_endpoint_phases']}
native_caches={x['native_source']:x['native_source_sha256'] for x in contract['controlled_endpoints']}
for p,h in native_caches.items():assert sha(ROOT/p)==h,('consumed native cache drift',p)
reference=read('metadata/refresh-menu-reference-review.json');assert reference['C_sharp_used_as_exact_behavior'] is False
for p in reference['references']:assert sha(ROOT/p['path'])==p['sha256']
tracked=set(app['written'])|set(app['protected_unchanged'])|set(frozen['files'])|set(proof['source_hashes'])|set(proof['binary_hashes'])|set(native_caches)|{'metadata/integration-batch-131-application.json','metadata/refresh-menu-dispatch-emulation.json','metadata/refresh-menu-reference-review.json',f['path'],Path(__file__).relative_to(ROOT).as_posix(),'tools/record_refresh_menu_reference.py'}
tracked.update(p['path'] for p in reference['references'])
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};configurations={}
for cfg,build in [('Debug','c-only-debug'),('Release','c-only')]:
 directory=ROOT/'build'/build;cache=(directory/'CMakeCache.txt').read_text()
 for name in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+name+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 log='artifacts/menu-131-'+cfg.lower()+'-build.log';s=(ROOT/log).read_text(encoding='utf-8',errors='replace');assert '100% tests passed, 0 tests failed out of 135' in s and 'C source build completed. No hybrid game was built or launched.' in s
 project=directory/'restore_refresh_menu_dispatch_verify.vcxproj';tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 expected={ROOT/'artifacts/refresh_menu_dispatch_phase/tests'/name for name in ['refresh_dispatch.c','actual_mission_buttons.c']}|{ROOT/'src'/f'unit_{n:05d}.c' for n in [1867,286,293,295,296,297,2276]}
 assert actual==expected
 assert ROOT/'runtime/refresh_menu_dispatch_boundary.c' not in actual,'actual OS/engine facade must not replace declared test endpoints'
 suppressed=[n for n in tree.findall('.//m:ClCompile[@Include]',ns) if any('4133' in (v.text or '').split(';') for v in n.findall('m:DisableSpecificWarnings',ns))]
 assert len(suppressed)==1 and Path(suppressed[0].get('Include')).resolve()==ROOT/'src/unit_00295.c'
 binary=directory/cfg/'restore_refresh_menu_dispatch_verify.exe'
 assert sha(binary)==proof['compiled_C_sha256' if cfg=='Debug' else 'comparison_C_sha256']
 for p in actual|{project,ROOT/'CMakeLists.txt',ROOT/'include/refresh_menu_dispatch.h'}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,p
 product=directory/'pharaoh_restored.vcxproj';t=ET.parse(product);product_sources={Path(n.get('Include')).resolve() for n in t.findall('.//m:ClCompile[@Include]',ns)}
 assert {ROOT/'src/unit_01867.c',ROOT/'runtime/refresh_menu_dispatch_boundary.c'}<=product_sources and not any('artifacts' in p.relative_to(ROOT).parts for p in product_sources)
 library=directory/cfg/'pharaoh_restored.lib';link=directory/cfg/'restore_link_check.exe'
 for p in [ROOT/'src/unit_01867.c',ROOT/'runtime/refresh_menu_dispatch_boundary.c',ROOT/'include/refresh_menu_dispatch.h',product]:assert library.stat().st_mtime_ns>=p.stat().st_mtime_ns
 assert link.stat().st_mtime_ns>=library.stat().st_mtime_ns
 raw=link.read_bytes();pe=struct.unpack_from('<I',raw,60)[0];assert struct.unpack_from('<H',raw,pe+4)[0]==0x14c
 configurations[cfg]={'oracle_path':binary.relative_to(ROOT).as_posix(),'oracle_sha256':sha(binary),'product_sha256':sha(library),'link_check_sha256':sha(link),'optimization':get('Optimization'),'runtime':get('RuntimeLibrary'),'CTest_passed':135}
 tracked.update([log,project.relative_to(ROOT).as_posix(),product.relative_to(ROOT).as_posix(),library.relative_to(ROOT).as_posix(),link.relative_to(ROOT).as_posix(),'build/'+build+'/CMakeCache.txt'])
out={'format':1,'passed':True,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'production_controller':True,'source_only_build':True,'original_game_executed':False,'controller_instructions':[917,917],'composed_instructions':[960,960],'conditional_outcomes':[250,250],'native_cases':11633,'comparisons':23266,'controlled_deep_endpoints':96,'mutated_endpoint_phases':192,'watched_bytes':222728,'actual_getter_instructions':42,'actual_bare_RET_instructions':1,'configurations':configurations,'C_sharp_used_as_exact_behavior':False,'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(tracked)}}
(ROOT/'metadata/menu-131-wiring-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['passed','native_cases','comparisons','controller_instructions','composed_instructions','complete_menu_reconstruction']}))
