"""Compile staged leaves privately; successful compilation never changes verification status."""
import argparse,json,subprocess,sys
from pathlib import Path
from common import ROOT,confined,sha
CMAKE=Path(r'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe')
def compile_candidates(run,tag=""):
 if tag and not __import__("re").fullmatch(r"[a-z][a-z0-9_-]{0,20}",tag):raise ValueError("Unsafe compile tag")
 suffix=("-"+tag) if tag else ""
 run=confined(run);selection=json.loads((run/'selection.json').read_text());rows=[q for q in selection['rows'] if q['status']=='staged_unverified'];candidate_dir=ROOT/selection['candidate_directory'];source=run/('compile-project'+suffix);build=run/('compile-out'+suffix)
 if source.exists() or build.exists():raise ValueError('Fresh private compile paths required')
 source.mkdir();cm=['cmake_minimum_required(VERSION 3.25)','project(PrivateBulkLeaves LANGUAGES C)','set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")']
 for q in rows:
  path=(ROOT/q['candidate']).as_posix();name='leaf_'+q['entry'];cm.extend([f'add_library({name} OBJECT "{path}")',f'target_include_directories({name} PRIVATE "{candidate_dir.as_posix()}")',f'target_compile_options({name} PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>")'])
 (source/'CMakeLists.txt').write_text('\n'.join(cm)+'\n');report={'candidate_count':len(rows),'compilation_is_not_verification':True,'all_functions_verified':False,'configurations':{}}
 cfg=subprocess.run([str(CMAKE),'-S',str(source),'-B',str(build),'-A','Win32'],cwd=ROOT,capture_output=True,text=True);(run/('compile-configure'+suffix+'.log')).write_text(cfg.stdout+cfg.stderr);report['configure_returncode']=cfg.returncode
 if cfg.returncode==0:
  for config in ['Debug','Release']:
   r=subprocess.run([str(CMAKE),'--build',str(build),'--config',config,'--parallel','4'],cwd=ROOT,capture_output=True,text=True);log=run/('compile-'+config.lower()+suffix+'.log');log.write_text(r.stdout+r.stderr);result=[]
   for q in rows:
    objects=list((build/('leaf_'+q['entry']+'.dir')/config).glob('*.obj'));result.append({'entry':q['entry'],'source':q['candidate'],'source_sha256':q['candidate_sha256'],'object_written':len(objects)==1,'object_sha256':sha(objects[0]) if len(objects)==1 else None,'verified':False})
   report['configurations'][config]={'aggregate_returncode':r.returncode,'objects_compiled':sum(q['object_written'] for q in result),'candidates':result,'log':log.relative_to(ROOT).as_posix(),'log_sha256':sha(log)}
 (run/('compilation'+suffix+'.json')).write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--run-root',type=Path,required=True);ap.add_argument('--tag',default='');a=ap.parse_args();r=compile_candidates(a.run_root,a.tag);print(json.dumps({'configure_returncode':r['configure_returncode'],'candidate_count':r['candidate_count'],'compiled':{k:v['objects_compiled'] for k,v in r['configurations'].items()},'verified':False}))
