"""Replay frozen whole4D75D0 oracle against actual C; retain all native semantics."""
from pathlib import Path
import argparse,hashlib,json,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/refresh_menu_dispatch_phase'
ORACLE=PHASE/'verify_refresh.py';MANIFEST=PHASE/'freeze-manifest.json'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(MANIFEST)=='ddd403e31bc13a45fc51c88838cbc99342ef8f78b2e94e4f5d31e71e9f59271b','frozen manifest drift'
frozen=json.loads(MANIFEST.read_text())
for p,h in frozen['files'].items():assert sha(ROOT/p)==h,('frozen phase drift',p)
assert sha(ORACLE)=='6660a9edefc94ab96885f41a3616a1b75290032de900f568b93943732ff66086'
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path);ap.add_argument('--optimized-exe',type=Path)
ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path)
args=ap.parse_args()
if args.limit is not None:assert args.limit>0
if args.production:assert args.exe and args.optimized_exe,'both actual production executables required'
executables=[(p or ROOT/'build/refresh-menu-dispatch'/cfg/'restore_refresh_menu_dispatch_verify.exe').resolve() for p,cfg in zip([args.exe,args.optimized_exe],['Debug','Release'])]
assert len(set(executables))==2
for p in executables:assert p.is_file(),p
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
projects=[];configuration_evidence={}
expected_sources={PHASE/'tests/refresh_dispatch.c',PHASE/'tests/actual_mission_buttons.c'}|{ROOT/'src'/f'unit_{n:05d}.c' for n in [286,293,295,296,297,2276]}|{(ROOT if args.production else PHASE)/'src/unit_01867.c'}
for binary,cfg in zip(executables,['Debug','Release']):
 project=binary.parent.parent/'restore_refresh_menu_dispatch_verify.vcxproj';assert project.is_file(),project
 tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed')
 assert get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual==expected_sources,(cfg,actual^expected_sources)
 definitions=get('PreprocessorDefinitions');assert not any(x in definitions for x in ['STAGED','TRACE'])
 for p in actual|{project,(ROOT if args.production else PHASE)/'include/refresh_menu_dispatch.h',PHASE/'tests/fixture_schema.h',PHASE/'tests/boundaries.inc'}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,('stale executable',p)
 if args.production:
  cache=(project.parent/'CMakeCache.txt').read_text()
  import re
  for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',cache,re.M)==['']
  assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
  product=ET.parse(project.parent/'pharaoh_restored.vcxproj')
  product_sources={Path(n.get('Include')).resolve() for n in product.findall('.//m:ClCompile[@Include]',ns)}
  assert ROOT/'src/unit_01867.c' in product_sources and ROOT/'runtime/refresh_menu_dispatch_boundary.c' in product_sources
 configuration_evidence[cfg]={'binary':binary.relative_to(ROOT).as_posix(),'sha256':sha(binary),'optimization':get('Optimization'),'runtime':get('RuntimeLibrary'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]}
 projects.append(project)
extra_sources=[Path(__file__),MANIFEST,ROOT/'CMakeLists.txt']+projects
text=ORACLE.read_text()
def replace_once(old,new):
 global text
 assert text.count(old)==1,('oracle plumbing anchor drift',old[:100]);text=text.replace(old,new,1)
replace_once("ap=argparse.ArgumentParser();ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path);args=ap.parse_args()",'args=WRAPPER_ARGS')
replace_once("executables=[ROOT/'build/refresh-menu-dispatch'/cfg/'restore_refresh_menu_dispatch_verify.exe' for cfg in ['Debug','Release']]",'executables=WRAPPER_EXECUTABLES')
if args.production:
 for name in ['src/unit_01867.c','include/refresh_menu_dispatch.h','runtime/refresh_menu_dispatch_boundary.c']:replace_once("HERE/'"+name+"'","ROOT/'"+name+"'")
replace_once("before={p.relative_to(ROOT).as_posix():sha(p) for p in sources}","sources+=WRAPPER_EXTRA_SOURCES\nbefore={p.relative_to(ROOT).as_posix():sha(p) for p in sources}")
# Cache only immutable prepared RAM bytes, copied afresh by Unicorn every case.
# The 256-byte period is the identical expression used by the frozen oracle.
replace_once(" for j,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[options['seed']]+j*29+k*37)&255 for k in range(n)))"," for j,(a,n) in enumerate(regions):vm.mem_write(a,WRAPPER_SEED(p[options['seed']],j,n))")
# The two routes already occur in the baseline compared corpus. Include them in
# warmup selection too, to exercise entry/exit mutations at all96 endpoints.
replace_once("for q in warmups:\n", "warmups += [case(g00e38dda=0,g00e3908c=1),case(g00e38dda=9,g00eadd50=2)]\nfor q in warmups:\n")
replace_once("'production':False", "'production':WRAPPER_ARGS.production")
replace_once("'native_game_executed':False", "'native_game_executed':False,'compiled_C_sha256':sha(executables[0]),'comparison_C_sha256':sha(executables[1]),'configuration_evidence':WRAPPER_CONFIGURATION,'baseline_case_count':11437,'supplemental_all_endpoint_mutations':True,'case_selection':{'limit':args.limit,'single_case':bool(args.case)}")
replace_once("out=HERE/('partial-emulation.json' if args.limit or args.case else 'private-emulation.json')","out=ROOT/'metadata'/('refresh-menu-dispatch-partial-emulation.json' if args.limit or args.case else 'refresh-menu-dispatch-emulation.json')")
replace_once("(HERE/'last-difference.json')", "(ROOT/'metadata/refresh-menu-dispatch-last-difference.json')")
replace_once(" if not(args.limit or args.case):\n  assert set(instructions)"," if not(args.limit or args.case):\n  assert complete_count==11633 and len(mutated)==192 and len(reached)==96,(complete_count,len(mutated),len(reached))\n  assert set(instructions)")
cache={}
def seed_bytes(seed,index,length):
 key=(seed&255,index,length)
 if key not in cache:
  period=bytes((seed+index*29+k*37)&255 for k in range(256))
  value=period*(length//256)+period[:length%256]
  assert value==bytes((seed+index*29+k*37)&255 for k in range(length))
  cache[key]=value
 return cache[key]
environment={'__file__':str(ORACLE),'__name__':'__main__','WRAPPER_ARGS':args,'WRAPPER_EXECUTABLES':executables,'WRAPPER_EXTRA_SOURCES':extra_sources,'WRAPPER_CONFIGURATION':configuration_evidence,'WRAPPER_SEED':seed_bytes}
exec(compile(text,str(ORACLE),'exec'),environment)
