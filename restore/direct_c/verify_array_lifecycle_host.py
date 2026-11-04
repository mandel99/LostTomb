"""Host C SEH cleanup contracts; independent of original OS dispatch proof."""
from pathlib import Path
import hashlib,json,subprocess
ROOT=Path(__file__).resolve().parents[1]
sources=['CMakeLists.txt','tests/array_lifecycle_exceptions.c','runtime/array_lifecycle.c','runtime/owned_buffer_lifecycle.c','runtime/owned_buffer.c','runtime/sound_file_lifecycle.c','include/array_lifecycle.h','include/owned_buffer.h','include/sound_file_lookup.h','include/sound_initialization.h','direct_c/verify_array_lifecycle_host.py']+[f'src/unit_{n:05}.c' for n in [2682,2683,2684,2685,2724,2725,2726,2776,2778,2779]]
variants={}
for config in ['Debug','Release']:
 path=ROOT/f'build/c-only/{config}/restore_array_lifecycle_verify.exe'
 normal=subprocess.run([str(path)],capture_output=True,text=True,timeout=30)
 assert normal.returncode==0 and 'allocation cleanup passed' in normal.stdout,(config,normal.returncode,normal.stdout,normal.stderr)
 fatal=subprocess.run([str(path),'--cpp-unwind'],capture_output=True,text=True,timeout=30)
 assert fatal.returncode==77,(config,'expected test terminate target',fatal.returncode,fatal.stdout,fatal.stderr)
 variants[config]=dict(compiled_C_path=str(path.resolve()),compiled_C_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),normal_exit=normal.returncode,controlled_terminate_exit=fatal.returncode,contracts=6)
report=dict(passed=True,variants=variants,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},native_OS_exception_dispatch_proven=False,contracts=['normal forward constructor traversal','constructor interruption destroys only completed predecessors, in reverse order','destructor interruption finishes the remaining predecessors','non-C++ destructor exception propagates through the unwind filter','access violation during the allocation cookie store frees the raw allocation','C++ destructor exception reaches the controlled nonreturning terminate target'],limitations=['Actual Windows C SEH and callback cleanup execute in both configurations. The terminate implementation is a test-only exit77, not a reconstruction proof of5637EE.','Original native OS exception dispatch and its five exception-only5613E3 instructions remain outside this host test; explicit native frame cleanup seams are covered separately by the paired lifecycle oracle.'])
(ROOT/'metadata/array-lifecycle-host.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS Debug/Release C SEH: six cleanup/filter contracts each; normal0 and controlled terminate77; native OS dispatcher not claimed')
