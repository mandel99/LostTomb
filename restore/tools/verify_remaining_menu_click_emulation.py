"""Replay the exact frozen3628-case clicked-row oracle against shared root targets."""
from pathlib import Path
import argparse,hashlib,json,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parent.parent
STAGE=ROOT/'artifacts/remaining_menu_click_phase'
ORACLE=STAGE/'verify_remaining_menu_click.py'
FREEZE=STAGE/'remaining_menu_click_freeze.json'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
relative=lambda p:p.relative_to(ROOT).as_posix()
assert sha(ORACLE)=='a4677679325364b771faad1b88caa7b31c55c2253a73d7b4e713cc2d7c8fb19a','frozen oracle drift'
assert sha(FREEZE)=='0dbd10b68abcc8ba2fa24f4429881fababce8bfcd0e72c29bed479e17c1f70e9','frozen manifest drift'
freeze=json.loads(FREEZE.read_text())
for name,want in freeze['files'].items():assert sha(STAGE/name)==want,name
old=json.loads((STAGE/'remaining-menu-click-emulation.json').read_text())
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('--production',action='store_true');ap.add_argument('--limit',type=int)
for name in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+name,type=Path)
args=ap.parse_args()
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided),'all four actual shared executables required'
executables=[(p or STAGE/'build'/cfg/f'remaining_menu_click_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,[(c,k) for c in ['Debug','Release'] for k in ['trace','plain']])]
assert len(set(executables))==4,'four distinct variants required'
for p in executables:assert p.is_file() and p.with_suffix('.map').is_file(),p
sources=[ROOT/p for p in old['source_hashes']]+[Path(__file__),FREEZE,ROOT/'CMakeLists.txt']
registry_extension=False
# Actual applied source and header identities are fixed by the accepted composition.
for name,want in old['source_hashes'].items():
    if name.startswith('artifacts/') or sha(ROOT/name)==want:continue
    # The subsequently proved popup adds one resolver, leaving this route intact.
    # Admit only that exact two-line extension of the frozen registry bytes.
    assert name=='runtime/menu_callbacks.c',('applied source drift',name)
    before=ROOT/'artifacts/integration-batch-125-before/runtime/menu_callbacks.c'
    assert sha(before)==want,('registry predecessor drift',name)
    expected=before.read_bytes()
    for anchor,insertion in [(b'#include "remaining_menu_actions.h"\n',b'#include "selection_popup.h"\n'),(b'    if(restore_remaining_menu_action(address))return;\n',b'    if(restore_selection_popup_action(address))return;\n')]:
        assert expected.count(anchor)==1
        expected=expected.replace(anchor,anchor+insertion,1)
    # Batch126 adds only authenticated editor and display routes. Retain the
    # exact predecessor and bytewise extension guard; arbitrary drift fails.
    predecessor=ROOT/'artifacts/integration-batch-126-before/runtime/menu_callbacks.c'
    assert sha(predecessor)=='68dbd3b45a89b69f3ec2a805fb6cf4b528a595a3a0730b33e23eba5cebb08e9d'
    assert expected==predecessor.read_bytes(),'batch126 predecessor mismatch'
    for anchor,insertion in [(b'#include "selection_popup.h"\n',b'#include "editor_validation.h"\n#include "display_confirmation.h"\n'),(b'    if(restore_selection_popup_action(address))return;\n',b'    if(address==0x00409830u){FUN_00409830();return;}\n    if(restore_display_confirmation_action(address))return;\n')]:
        assert expected.count(anchor)==1
        expected=expected.replace(anchor,anchor+insertion,1)
    for added,wanted in {
        'runtime/editor_validation.c':'f587ab92eb44773491fd10766adddc65004192d346a746d2ab7f16ddec96d17b',
        'include/editor_validation.h':'e8de3f3ddf03d761c0c750406d5a5543a61219180fdb2df35b6cb36153cbd63a',
        'runtime/display_confirmation.c':'1028ed98cc86b8b1fdc79bdf4b9b2c5026a31f385d182081dbe56bd5cd92c3a6',
        'runtime/display_confirmation_clock.c':'b37aa1b65cc38932a65e2749c88db63a08cd6b6698efb9a720cd16b56a806e5a',
        'include/display_confirmation.h':'d8e089b9c861633bc935762cccee91ba41db6591719384fb2d6240dea3f1ceaf'}.items():
        assert sha(ROOT/added)==wanted,('unqualified registry dependency',added)
        sources.append(ROOT/added)
    sources.append(predecessor)
    predecessor127=ROOT/'artifacts/integration-batch-127-before/runtime/menu_callbacks.c'
    assert sha(predecessor127)=='d7484ed726680ea5edfd4fdf175e91666e229ed8d6f007b92058f24488d4b529'
    assert expected==predecessor127.read_bytes(),'batch127 predecessor mismatch'
    for anchor,insertion in [(b'#include "display_confirmation.h"\n',b'#include "replay_mission.h"\n'),(b'    if(restore_display_confirmation_action(address))return;\n',b'    if(address==0x00418510u){FUN_00418510();return;}\n')]:
        assert expected.count(anchor)==1
        expected=expected.replace(anchor,anchor+insertion,1)
    for added,wanted in {
        'runtime/replay_mission.c':'642b89db205a5665d71e40b93b78b3a1163eded53c736553d266545e74eb9a67',
        'include/replay_mission.h':'4c69ef54913e6da4473e7b0c34cce5671cc771a464489159fbebb2492ed0ce09',
        'src/unit_00222.c':'6164d16417b42afca5df5c77d759d4050f8bdc6b9f3a578ea2137fce98797b62'}.items():
        assert sha(ROOT/added)==wanted,('unqualified replay dependency',added)
        sources.append(ROOT/added)
    sources.append(predecessor127)
    predecessor130=ROOT/'artifacts/integration-batch-130-before/runtime/menu_callbacks.c'
    assert sha(predecessor130)=='9a2eba29c04765b6e6fda8458887d75b4a501b09de2989008bcd517c8225b1d9'
    assert expected==predecessor130.read_bytes(),'batch130 predecessor mismatch'
    for anchor,insertion in [(b'#include "replay_mission.h"\n',b'#include "menu_keyboard_modal.h"\n'),(b'    if(address==0x00418510u){FUN_00418510();return;}\n',b'    if(restore_menu_modal_action(address))return;\n')]:
        assert expected.count(anchor)==1
        expected=expected.replace(anchor,anchor+insertion,1)
    for added,wanted in {
        'runtime/menu_keyboard_modal.c':'5429676e998d3f8e7d6332c310a8e6ba53d1b5433dddaba67096600642cdd5fd',
        'include/menu_keyboard_modal.h':'6ec23f72a3994e51715af5d916cf162b763e1f7b7cccf4c7f30ea246bad260f6'}.items():
        assert sha(ROOT/added)==wanted,('unqualified modal registry dependency',added)
        sources.append(ROOT/added)
    sources.append(predecessor130)
    assert expected==(ROOT/name).read_bytes(),('unqualified registry change',name)
    assert sha(ROOT/'runtime/selection_popup.c')=='e81f2049c582ce209c93e6b83331923fdb4f9f8b99364c9e4cc2e289de89ce1c'
    assert sha(ROOT/'include/selection_popup.h')=='27b3686797df89c7035dad2275edb33f797d0fcb665b27c5a0657295d0572725'
    sources += [before,ROOT/'runtime/selection_popup.c',ROOT/'include/selection_popup.h']
    registry_extension=True
labels=['Debug_trace','Debug_plain','O2_trace','O2_plain']
configs=['Debug','Debug','Release','Release'];libraries=[];config_evidence={}
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
def inspect_project(path,cfg):
    tree=ET.parse(path);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')]
    assert len(groups)==1,(path,cfg)
    group=groups[0];cc=group.find('m:ClCompile',ns);assert cc is not None
    get=lambda name:cc.findtext('m:'+name,default='',namespaces=ns)
    compiled=[Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)]
    return tree,compiled,{'runtime':get('RuntimeLibrary'),'optimization':get('Optimization'),'options':get('AdditionalOptions'),'definitions':get('PreprocessorDefinitions')}
if args.production:
    assert [p.name.lower() for p in executables]==['restore_remaining_menu_click_verify.exe','restore_remaining_menu_click_plain_verify.exe']*2
    assert all('remaining_menu_click_phase/build' not in p.as_posix() for p in executables),'private targets cannot serve as production evidence'
    expected_sources={STAGE/'remaining_menu_click_fixture.c',ROOT/'src/unit_00049.c',ROOT/'src/unit_00077.c',ROOT/'runtime/remaining_menu_actions.c',ROOT/'src/unit_01628.c'}
    router_sources={ROOT/p for p in ['runtime/menu_callbacks.c','runtime/dropdown_menu_boundary.c','src/unit_00410.c','src/unit_00413.c']}
    for i,(exe,cfg) in enumerate(zip(executables,configs)):
        assert exe.parent.name==cfg,(exe,cfg)
        build=exe.parent.parent;project=build/(exe.stem+'.vcxproj');libproject=build/'pharaoh_restored.vcxproj'
        tree,compiled,evidence=inspect_project(project,cfg)
        assert set(compiled)==expected_sources,(project,compiled)
        runtime='MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded'
        assert evidence['runtime']==runtime,(project,evidence)
        assert evidence['optimization']==('Disabled' if cfg=='Debug' else 'MaxSpeed'),(project,evidence)
        assert ('REMAINING_MENU_TRACE' in evidence['definitions'])==(i%2==0),(project,evidence)
        # ItemDefinitionGroup also contains a ProjectReference policy element.
        # Only ItemGroup references have Include; relative paths use the project.
        refs={(project.parent/n.get('Include')).resolve() for n in tree.findall('.//m:ProjectReference[@Include]',ns)}
        assert libproject in refs,(project,'missing actual root library reference')
        _,libcompiled,libevidence=inspect_project(libproject,cfg)
        assert router_sources<=set(libcompiled),(libproject,'actual router source missing')
        assert libevidence['runtime']==runtime and (libevidence['optimization']=='Disabled' or '/Od' in libevidence['options']),(libproject,libevidence)
        library=exe.parent/'pharaoh_restored.lib';assert library.is_file(),library
        assert exe.stat().st_mtime_ns>=library.stat().st_mtime_ns,('shared executable predates library',exe,library)
        for source in expected_sources|router_sources|{project,libproject}:
            assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('shared executable predates source/project',exe,source)
        libraries.append(library)
        config_evidence[labels[i]]={'target_project':relative(project),'target':evidence,'library_project':relative(libproject),'library':libevidence,'qualification':'Target-local callback/fixture/close objects Debug /Od or Release /O2; actual configuration-specific product router/click library /Od. Debug MTd; Release MT.'}
        sources.extend([project,libproject])
else:
    # Private replay is plumbing evidence only; preserve the frozen link provenance.
    assert all(sha(e)==old['variants'][label]['sha256'] for label,e in zip(labels,executables)),'private executable differs from frozen proof'
    libraries=[None]*4
sources=list(dict.fromkeys(sources));before={relative(p):sha(p) for p in sources}
artifacts=list(dict.fromkeys(executables+[e.with_suffix('.map') for e in executables]+[p for p in libraries if p]))
artifact_before={relative(p):sha(p) for p in artifacts}
lib_records={label:({'path':relative(p),'sha256':sha(p),'configuration':cfg} if p else dict(old['linked_library'],qualification='Frozen private Debug122 link provenance; current root library is not substituted.')) for label,p,cfg in zip(labels,libraries,configs)}
destination=(ROOT/'metadata' if args.production else STAGE)/('remaining-menu-click-emulation-wrapper-partial.json' if args.limit else ('remaining-menu-click-emulation.json' if args.production else 'remaining-menu-click-emulation-wrapper.json'))
text=ORACLE.read_text()
patches=[
 ('HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent','ROOT=WRAPPER_ROOT;HERE=WRAPPER_STAGE'),
 ("parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--limit',type=int);args=parser.parse_args()",'args=WRAPPER_ARGS'),
 ("executables=[HERE/'build'/cfg/f'remaining_menu_click_{kind}.exe' for cfg in ['Debug','Release'] for kind in ['trace','plain']]",'executables=WRAPPER_EXECUTABLES'),
 ("files=[HERE/'remaining_menu_click_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),FROZEN/'remaining_menu_native.json',ROOT/'metadata/dropdown-menu-native.json']+[ROOT/n for n in ['src/unit_00049.c','src/unit_00077.c','runtime/remaining_menu_actions.c','include/remaining_menu_actions.h','src/unit_01628.c','runtime/menu_callbacks.c','runtime/dropdown_menu_boundary.c','src/unit_00410.c','src/unit_00413.c','include/dropdown_menu.h','include/menu_input.h']]",'files=WRAPPER_SOURCES'),
 ("library=ROOT/'build/c-only-debug/Debug/pharaoh_restored.lib'",'library=WRAPPER_ORACLE'),
 ("report['library_drift']=sha(library.read_bytes())!=library_before", "report['library_drift']=any(sha(p.read_bytes())!=WRAPPER_ARTIFACT_BEFORE[relative(p)] for p in WRAPPER_ARTIFACTS)"),
 ("out=HERE/('remaining-menu-click-emulation-partial.json' if args.limit else 'remaining-menu-click-emulation.json');out.write_text(json.dumps(report,indent=2)+'\\n')", """assert report['source_hashes']==WRAPPER_BEFORE,'source changed before oracle launch'
if not args.limit:assert report['corpus_sha256']=='c151d0295101778530bc0799db104ca537fff7f8f634089819aa693a5121881a'
report.pop('linked_library')
report.update({'production':args.production,'compiled_C_path':relative(executables[0]),'frozen_oracle_sha256':WRAPPER_ORACLE_HASH,'frozen_manifest_sha256':WRAPPER_FREEZE_HASH,'linked_libraries':WRAPPER_LIB_RECORDS,'configuration_evidence':WRAPPER_CONFIGS,'artifact_hashes_before_run':WRAPPER_ARTIFACT_BEFORE,'method':'Exact frozen3628-case native controller and selected-click oracle against actual shared Debug/Release central/dropdown/click routes, target-local Debug /Od and Release /O2 callback/fixture/close objects; product library bodies /Od. Eight named deep endpoints controlled.' if args.production else 'Exact frozen3628-case private-binary plumbing replay; original private Debug122 link provenance retained.'})
for label,entry in report['variants'].items():entry['linked_library']=WRAPPER_LIB_RECORDS[label]
out=WRAPPER_DESTINATION;out.write_text(json.dumps(report,indent=2)+'\\n')""")]
for oldtext,newtext in patches:
    assert text.count(oldtext)==1,('immutable patch anchor',oldtext)
    text=text.replace(oldtext,newtext)
namespace={'__name__':'__main__','__file__':str(ORACLE),'WRAPPER_ROOT':ROOT,'WRAPPER_STAGE':STAGE,'WRAPPER_ARGS':args,'WRAPPER_EXECUTABLES':executables,'WRAPPER_SOURCES':sources,'WRAPPER_ORACLE':ORACLE,'WRAPPER_ARTIFACTS':artifacts,'WRAPPER_ARTIFACT_BEFORE':artifact_before,'WRAPPER_BEFORE':before,'WRAPPER_ORACLE_HASH':sha(ORACLE),'WRAPPER_FREEZE_HASH':sha(FREEZE),'WRAPPER_LIB_RECORDS':lib_records,'WRAPPER_CONFIGS':config_evidence,'WRAPPER_DESTINATION':destination}
exec(compile(text,str(ORACLE),'exec'),namespace)
