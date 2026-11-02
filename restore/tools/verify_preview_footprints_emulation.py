"""Prove the preview footprint constants and seed-only startup stores.

Independent static push/affine recovery and real native constructor/helper
execution establish each tuple. Compiled C is compared over complete object
storage and guards, both before and after the ten unchanged geometry helpers.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,json,struct,subprocess,sys,time,bisect
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir())
PHASE=ROOT/'artifacts/preview_footprints_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-preview-footprints/Debug/restore_preview_footprints_verify.exe');args=parser.parse_args();exe=args.exe.resolve()
OWN=ROOT if args.production else PHASE;NATIVE=ROOT/'metadata/preview-footprints-native.json' if args.production else PHASE/'metadata/native.json';REPORT=ROOT/'metadata/preview-footprints-emulation.json' if args.production else PHASE/'metadata/emulation.json'
spec=importlib.util.spec_from_file_location('preview_seed_extract',Path(__file__).with_name('extract_preview_footprint_seeds.py'));recover=importlib.util.module_from_spec(spec);spec.loader.exec_module(recover)
from unicorn import UC_HOOK_BLOCK,__version__
from unicorn.x86_const import *
sha=lambda b:hashlib.sha256(b).hexdigest();started=time.monotonic()
image,dossier,static,rows,prefix_hits=recover.recover(NATIVE)
second,second_hits=recover.native_recover(image,dossier['functions'],static,0x5a);assert rows==second
assert (OWN/'runtime/preview_footprint_seeds.c').read_text()==recover.runtime_source(static),'C constants are not the independently recovered native tuples'
vm=recover.make_vm(image);STACK=0x34000000;STOP=0x33000000;FIRST=0x6de500;BYTES=0x1f640
vm.mem_map(STACK,0x10000);vm.mem_map(STOP,4096)
# Add actual unchanged geometry wrappers, array loop and record arithmetic.
helpers={};wrapper_dossier=json.loads((ROOT/'metadata/placement-preview-objects-native.json').read_text())
for s in static:
 a=int(s['geometry'],16);f=wrapper_dossier['functions'][f'{a:08x}']['assembly'];assert not f['has_more'] and len(f['items'])==f['total'];helpers[a]=f['items']
helpers[0x4e52b0]=dossier['functions']['004e52b0']['assembly']['items']
md=recover.capstone.Cs(recover.capstone.CS_ARCH_X86,recover.capstone.CS_MODE_32)
core=[]
for i in md.disasm(bytes(vm.mem_read(0x4e5190,0x100)),0x4e5190):
 core.append({'address':hex(i.address),'bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str})
 if i.mnemonic=='ret':break
assert int(core[-1]['address'],16)==0x4e5203;helpers[0x4e5190]=core
allowed=sorted(int(i['address'],16) for f in helpers.values() for i in f);allowed_set=set(allowed);hits=set();cache={}
for f in helpers.values():
 for i in f:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw

def block(cpu,a,n,user):
 assert a in allowed_set,'Unexpected native geometry block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
vm.hook_add(UC_HOOK_BLOCK,block)
def write(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def execute(entry,receiver):
 sp=STACK+0x8000;vm.mem_write(sp-0x100,b'\xcc'*0x300);write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ECX,receiver),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start(entry,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+4,128))==b'\xcc'*128

def native(p):
 op,seed,a,b=p
 vm.mem_write(FIRST,bytes(((i*13+(i>>5)*7+seed)^0xa7)&255 for i in range(BYTES)))
 vm.mem_write(0x70e440,bytes(((i*31+seed)^0x69)&255 for i in range(48)));write(0x70e44c,a);write(0x70e45c,b)
 # Copy only the four DWORDs actually observed after real native record copies.
 for s,observed in zip(static,rows):
  address=int(s['object'],16)+s['offset']
  for index,values in enumerate(observed['tuples']):vm.mem_write(address+index*52,struct.pack('<4i',*values))
 if op in [1,3]:
  for s in static:execute(int(s['geometry'],16),int(s['object'],16))
 return bytes(vm.mem_read(FIRST,BYTES))+bytes(vm.mem_read(0x70e440,48))
cases=[(op,seed,a,b) for op in [0,1,2,3] for seed in [0,1,0x12345678,0xffffffff] for a,b in [(0,0),(15,29),(256,511),(1024,65535),(0xffffffff,0xffffffff),(0x80000000,0x7fffffff)]]
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':recover.SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,'record_count':1327,'seed_DWORD_count':1327*4,'full_watch_bytes':BYTES+48,
 'method':'Independent Capstone affine-register/push propagation and bounded Unicorn constructor prefixes agree on every seed tuple and destination. Actual4e5120/4e5140 instructions run with two initial memory patterns. Compiled seed initializer is compared over complete BSS object storage/canaries; unchanged native/C geometry methods compose afterward. Repeated seeding preserves derived and unrelated fields. Stack and callee-saved registers checked.',
 'limitations':['Only the four seed fields per52-byte record are initialized. Existing geometry must run afterward to produce fields4..12. Vtables, special selectors and unrelated base fields are separate preconditions.','Native provenance executes constructor prefixes through the final target record copy. Base/vector constructor calls are controlled, empty destructors execute as nativeRET, and SEH epilogues/RNG/post-seed tails are excluded. This is not complete C++ constructor reconstruction.','No placement rendering, map state, input or physical pixels are established by this seed/bootstrap proof.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
p=None
try:
 assert exact(4)==struct.pack('<I',0x53454544)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();actual=exact(BYTES+48);expected=native(p)
  assert actual==expected,repr({'case':p,'first_difference':next((hex(FIRST+i) if i<BYTES else hex(0x70e440+i-BYTES) for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None)})
  report['checked']+=1
 assert hits==allowed_set,'Native geometry coverage incomplete'
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
source_paths=[OWN/'include/preview_footprint_seeds.h',OWN/'runtime/preview_footprint_seeds.c',OWN/'tests/preview_footprint_seeds.c',Path(__file__),Path(__file__).with_name('extract_preview_footprint_seeds.py'),NATIVE,PHASE/'CMakeLists.txt',ROOT/'include/startup_geometry.h',ROOT/'metadata/placement-preview-objects-native.json']+[ROOT/f'src/unit_{n:05d}.c' for n in [2,99,195,1993,2563,2617,2604,2591,1965,1968]]
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in source_paths}
prior=json.loads((ROOT/'metadata/startup-geometry-emulation.json').read_text());report['geometry_source_drift']=[p for p,h in prior['source_hashes'].items() if p in report['source_hashes'] and report['source_hashes'][p]!=h];assert not report['geometry_source_drift']
report['sets']=[{k:v for k,v in s.items() if k not in ['records','prefix_instructions']} for s in static]
report['native_prefix_instruction_count']=len(prefix_hits);report['native_geometry_instruction_coverage']=[len(hits),len(allowed)]
report['tuple_sha256']=sha(b''.join(struct.pack('<4i',*r['tuple']) for s in static for r in s['records']))
report['elapsed_seconds']=round(time.monotonic()-started,3);REPORT.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','sets']},indent=2));sys.exit(0 if report['passed'] else 1)
