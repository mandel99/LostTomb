"""Production path plumbing for three immutable scalar/numeric component oracles.
Only parser, executable/source/report bindings change; native/corpus/expectations do not.
"""
from pathlib import Path
import argparse,hashlib,json,sys,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();rel=lambda p:p.relative_to(ROOT).as_posix()
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('--component',choices=['raw80','arithmetic','post8-leaf'],required=True)
ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path);ap.add_argument('--optimized-exe',type=Path);ap.add_argument('--limit',type=int)
args=ap.parse_args();manifest=HERE/'integration-manifest.json';plan=json.loads(manifest.read_text())
spec={
 'raw80':{'directory':'map_refresh_phase/x87_conversion','oracle':'verify_x87_conversion.py','sha256':'89dfab7867a0880c67391fcf3cdbfc861b404f18c5cfdf7d64c745c129ab39fa','private':'map_refresh_x87_conversion','target':'restore_map_refresh_x87_conversion_verify','count':42768,'fixture':'x87_conversion_fixture.c','sources':['runtime/map_x87_conversion.c'],'headers':['include/map_x87_conversion.h'],'output':'map-refresh-x87-component-emulation.json','old_output':"(HERE/('x87-conversion-partial.json' if args.limit else 'x87-conversion-emulation.json')).write_text(json.dumps(report,indent=2)+'\\n')",'owners':{'_restore_map_trunc80':'map_x87_conversion.obj'}},
 'arithmetic':{'directory':'map_refresh_arithmetic_phase','oracle':'verify_map_refresh_numeric.py','sha256':'223819fc4fd5e83781706ec6dab6445145f7a903e7c91998df3ded752af61a6e','private':'map_refresh_numeric','target':'restore_map_refresh_numeric_verify','count':20224,'fixture':'numeric_fixture.c','sources':['runtime/map_refresh_fp.c'],'headers':['include/map_refresh_arithmetic.h'],'output':'map-refresh-numeric-component-emulation.json','old_output':"(HERE/('map-refresh-numeric-partial.json' if args.limit else 'map-refresh-numeric-emulation.json')).write_text(json.dumps(report,indent=2)+'\\n')",'owners':{'_mr_fp_add':'map_refresh_fp.obj','_mr_fp_div':'map_refresh_fp.obj','_mr_fp_store64':'map_refresh_fp.obj'}},
 'post8-leaf':{'directory':'map_refresh_postpass8_phase/leaf_preflight','oracle':'verify_leaf.py','sha256':'207576d9f9b742f66ba592248597a9b476e33c05aabce8f3a08e09b175de460d','private':'map8_leaf','target':'restore_map_refresh_postpass8_leaf_verify','count':1465,'fixture':'fixture.c','sources':['src/unit_01235.c','src/unit_01238.c','src/unit_01098.c'],'headers':['include/restore_types.h','include/standalone_data.h'],'output':'map-refresh-postpass8-leaf-emulation.json','old_output':"(HERE/('leaf-partial.json' if args.limit else 'leaf-emulation.json')).write_text(json.dumps(report,indent=2)+'\\n')",'owners':{'_FUN_00484ee0':'unit_01235.obj','_FUN_00484f40':'unit_01238.obj','_FUN_0047a1f0':'unit_01098.obj'}}
}[args.component]
old=ROOT/'artifacts'/spec['directory'];oracle=old/spec['oracle'];assert sha(oracle)==spec['sha256'],'immutable oracle drift'
executables=[(p or old/'build'/cfg/(spec['private']+'.exe')).resolve() for p,cfg in zip([args.exe,args.optimized_exe],['Debug','Release'])]
projects={};maps={};selected={m['target']:m for m in plan['mappings']}
for p,want in plan['frozen_evidence_hashes'].items():assert sha(ROOT/p)==want,p
if args.production:
 own_freeze=json.loads((HERE/'integration-freeze.json').read_text())
 for name,want in own_freeze['files'].items():assert sha(ROOT/name)==want,name
if args.production:
 assert args.exe and args.optimized_exe and len(set(executables))==2
 for source in spec['sources']+spec['headers']:
  if source in selected:assert sha(ROOT/source)==selected[source]['after_sha256'],source
 ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
 expected={old/spec['fixture'],*[ROOT/p for p in spec['sources']]}
 for k,exe in enumerate(executables):
  cfg=['Debug','Release'][k];assert exe.name==spec['target']+'.exe' and exe.parent.name==cfg
  project=exe.parent.parent/(spec['target']+'.vcxproj');tree=ET.parse(project)
  compiled={(project.parent/Path(n.get('Include'))).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert compiled==expected,(project,compiled)
  groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
  cc=groups[0].find('m:ClCompile',ns);runtime=cc.findtext('m:RuntimeLibrary',namespaces=ns);optimization=cc.findtext('m:Optimization',namespaces=ns)
  assert runtime==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded') and optimization==('Disabled' if cfg=='Debug' else 'MaxSpeed')
  for source in expected|{ROOT/p for p in spec['headers']}|{project}:assert exe.stat().st_mtime_ns>=source.stat().st_mtime_ns,('stale shared binary',exe,source)
  projects[rel(project)]={'sha256':sha(project),'configuration':cfg,'runtime':runtime,'optimization':optimization,'sources':[rel(p) for p in sorted(compiled)]}
  mp=exe.with_suffix('.map');lines=mp.read_text().splitlines();binding={}
  for symbol,owner in spec['owners'].items():
   found=[line for line in lines if symbol in line.split()];assert len(found)==1 and found[0].split()[-1]==owner,(mp,symbol,found);binding[symbol]=owner
  maps[rel(mp)]={'sha256':sha(mp),'bindings':binding}

remap={}
if args.production:
 for mapping in plan['mappings']:remap[(ROOT/mapping['staged']).resolve()]=(ROOT/mapping['target']).resolve()
wrapper_before=sha(Path(__file__));manifest_before=sha(manifest)
def bind_files(files):
 out=[remap.get(p.resolve(),p) for p in files]
 # Record physical fixture-local quoted headers as well as exact applied product headers.
 out.extend([Path(__file__),manifest,HERE/'shared_targets.cmake',oracle,*[ROOT/p for p in spec['headers'] if args.production],*[ROOT/p for p in spec['sources'] if args.production]])
 if args.production:
  out.extend(ROOT/p for p in projects)
  if args.component=='post8-leaf':out.extend(e.parent/'restore_data.lib' for e in executables)
 if args.component in ['raw80','arithmetic']:
  local=old/('map_x87_conversion.h' if args.component=='raw80' else 'map_refresh_arithmetic.h');out.append(local)
 return list(dict.fromkeys(out))
def write_report(report):
 assert report['checked']==(min(args.limit,spec['count']) if args.limit else spec['count']) and report['comparisons']==report['checked']*2
 assert sha(oracle)==spec['sha256'] and sha(Path(__file__))==wrapper_before and sha(manifest)==manifest_before
 report.update({'production':args.production,'component':args.component,'original_oracle':{'path':rel(oracle),'sha256':spec['sha256'],'source_body_and_corpus_changed':False},'path_binding_transform_sha256':transformed_hash,'project_evidence':projects,'production_link_map_bindings':maps,'wrapper_sha256':wrapper_before,'integration_manifest_sha256':manifest_before})
 report['limitations'].append('Integration wrapper changes parser/executable/source/report bindings only. Private plumbing is not product evidence; production compares actual root source owners with DebugMTd/ReleaseMT. Historical oracle scope qualifications stay intact.')
 output=(ROOT/'metadata' if args.production else HERE)/(('partial-' if args.limit else '')+spec['output']);output.write_text(json.dumps(report,indent=2)+'\n')
source=oracle.read_text(encoding='utf-8-sig')
old_parser="ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);args=ap.parse_args()"
assert source.count(old_parser)==1;source=source.replace(old_parser,'args=COMPONENT_ARGS')
# Leaf puts its executable/source/hash setup on one physical line; stop at the first semicolon.
start=source.index('executables=');end=source.find(';',start);newline=source.find('\n',start)
if end<0 or newline<end:end=newline
source=source[:start]+'executables=COMPONENT_EXECUTABLES'+source[end:]
marker=';rel=lambda p:' if args.component=='post8-leaf' else '\nrel=lambda p:'
assert source.count(marker)==1;source=source.replace(marker,';files=COMPONENT_BIND_FILES(files)'+marker if marker.startswith(';') else '\nfiles=COMPONENT_BIND_FILES(files)'+marker)
assert source.count("'production':False")==1;source=source.replace("'production':False","'production':COMPONENT_ARGS.production")
old_output=spec['old_output'].replace('\\n','\n')
# The text source contains the two literal characters backslash+n in its string literal.
old_output=old_output.replace("+'\n'", "+'\\n'")
assert source.count(old_output)==1,(args.component,repr(old_output));source=source.replace(old_output,'COMPONENT_WRITE(report)')
transformed_hash=hashlib.sha256(source.encode()).hexdigest()
exec(compile(source,str(oracle),'exec'),{'__name__':'__main__','__file__':str(oracle),'COMPONENT_ARGS':args,'COMPONENT_EXECUTABLES':executables,'COMPONENT_BIND_FILES':bind_files,'COMPONENT_WRITE':write_report})
