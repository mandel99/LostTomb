"""Replay the frozen complete367-instruction text oracle on actual shared C builds."""
from pathlib import Path
import argparse,hashlib,json,re,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/menu_text_glyph_phase'
MANIFEST=PHASE/'freeze-manifest.json';ORACLE=PHASE/'verify_text.py'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(MANIFEST)=='89f505c7c42416485a368859662455b50af7c219ce9992bd869d986fb1509544'
frozen=json.loads(MANIFEST.read_text())
for p,h in frozen['files'].items():assert sha(ROOT/p)==h,('frozen text drift',p)
ref=frozen['external_reference'];assert sha(ROOT/ref['path'])==ref['sha256']
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path);ap.add_argument('--optimized-exe',type=Path);ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path);args=ap.parse_args()
assert args.production and args.exe and args.optimized_exe,'actual shared production Debug/Release executables required'
if args.limit is not None:assert args.limit>0
executables=[args.exe.resolve(),args.optimized_exe.resolve()];assert len(set(executables))==2
units=[1667,1691,1692,1693,1694];product_paths=[f'src/unit_{n:05d}.c' for n in units]+['include/menu_text_glyph.h','include/menu_text_glyph_memory.h']
for p in product_paths:assert (ROOT/p).read_bytes()==(PHASE/p).read_bytes(),('unqualified product text drift',p)
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};projects=[];configuration={}
for binary,cfg,build in zip(executables,['Debug','Release'],['c-only-debug','c-only']):
 assert binary==ROOT/'build'/build/cfg/'restore_menu_text_glyph_verify.exe'
 project=binary.parent.parent/'restore_menu_text_glyph_verify.vcxproj';tree=ET.parse(project)
 groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);link=groups[0].find('m:Link',ns);get=lambda n:cc.findtext('m:'+n,default='',namespaces=ns);lget=lambda n:link.findtext('m:'+n,default='',namespaces=ns)
 assert get('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and get('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and get('WarningLevel')=='Level4' and get('TreatWarningAsError')=='true'
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)}
 assert actual=={ROOT/f'src/unit_{n:05d}.c' for n in units}|{PHASE/'tests/text_glyph.c'}
 assert not any(x in get('PreprocessorDefinitions') for x in ['STAGED','TRACE'])
 assert lget('BaseAddress')=='0x10000' and lget('FixedBaseAddress')=='true' and lget('RandomizedBaseAddress')=='false'
 for p in actual|{project,ROOT/'CMakeLists.txt',PHASE/'tests/text_case.h'}|{ROOT/p for p in product_paths}:assert binary.stat().st_mtime_ns>=p.stat().st_mtime_ns,('stale text binary',p)
 cache=(project.parent/'CMakeCache.txt').read_text()
 for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',cache,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',cache,re.M)
 configuration[cfg]={'binary':binary.relative_to(ROOT).as_posix(),'sha256':sha(binary),'optimization':get('Optimization'),'runtime':get('RuntimeLibrary'),'sources':[p.relative_to(ROOT).as_posix() for p in sorted(actual)]};projects.append(project)
extra=[Path(__file__),MANIFEST,ROOT/'CMakeLists.txt',PHASE/'abi-reference-review.json',ROOT/ref['path']]+projects
text=ORACLE.read_text()
def replace(old,new):
 global text
 assert text.count(old)==1,('frozen oracle plumbing changed',old[:90]);text=text.replace(old,new,1)
replace("ap=argparse.ArgumentParser();ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path);args=ap.parse_args()",'args=WRAPPER_ARGS')
replace("sources=list((HERE/'src').glob('*.c'))+list((HERE/'include').glob('*.h'))",'sources=WRAPPER_PRODUCT_SOURCES')
replace("executables=[ROOT/'build/menu-text-glyph'/cfg/'restore_menu_text_glyph_verify.exe' for cfg in ['Debug','Release']]",'executables=WRAPPER_EXECUTABLES')
replace("before={p.relative_to(ROOT).as_posix():sha(p) for p in sources}",'sources+=WRAPPER_EXTRA\nbefore={p.relative_to(ROOT).as_posix():sha(p) for p in sources}')
replace("'production':False","'production':True")
replace("'original_game_executed':False","'original_game_executed':False,'compiled_C_sha256':sha(executables[0]),'comparison_C_sha256':sha(executables[1]),'configuration_evidence':WRAPPER_CONFIGURATION,'case_selection':{'limit':args.limit,'single_case':bool(args.case)}")
replace("out=HERE/('partial-emulation.json' if args.limit or args.case else 'private-emulation.json')","out=ROOT/'metadata'/('menu-text-glyph-partial-emulation.json' if args.limit or args.case else 'menu-text-glyph-emulation.json')")
replace("(HERE/'last-difference.json')","(ROOT/'metadata/menu-text-glyph-last-difference.json')")
replace(" if not(args.limit or args.case):\n  assert set(instructions)"," if not(args.limit or args.case):\n  assert total==3062 and len(mutations)==54\n  assert set(instructions)")
exec(compile(text,str(ORACLE),'exec'),{'__file__':str(ORACLE),'__name__':'__main__','WRAPPER_ARGS':args,'WRAPPER_EXECUTABLES':executables,'WRAPPER_PRODUCT_SOURCES':[ROOT/p for p in product_paths],'WRAPPER_EXTRA':extra,'WRAPPER_CONFIGURATION':configuration})
