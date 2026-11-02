"""Local equivalence only: five unmeasured51BC60 rollover blocks; no whole-path capacity claim."""
from pathlib import Path
import hashlib,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
phase=ROOT/'artifacts/menu_drag_land_route_phase';inputs=[phase/n for n in ['land_route.c','land_route_model.c','land_route_model.h','test_land.py']]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();before=[sha(p) for p in inputs]
assert before[:3]==['f3f49f237466cf15cafb933d8ed90f0272ffbe8fa2ef2b29b5f78262561fb27d','712df31b6e1c7a561d25f255acb5c281ab38aa4f422ef3e316fa7eb7e6ad419a','f1a458ffe3526aca9c34a3d2c7e65d94bd504bf4109a94ab188dfdb13c463699']
exe=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(exe)=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';native=PE(exe)
blocks=[(0x51bf50,10),(0x51c0a2,10),(0x51c1f7,10),(0x51c349,10),(0x51c365,7)]
for start,length in blocks:
 expected=bytes.fromhex('33c0a30ce0ea00' if length==7 else 'c70508e0ea0000000000')
 assert bytes(native.data[start-native.base:start-native.base+length])==expected
BUILD=ROOT/'build/agent-menu-land-roll-blocks';BUILD.mkdir(parents=True,exist_ok=True)
# Only entry/exit instrumentation changes. Every tested operation remains the
# exact frozen C statement. The derivative and shim exist solely under build/.
model=inputs[1].read_text();declaration='uint32_t a,b,t,p; uint16_t sw;'
assert model.count(declaration)==1
dispatch='switch(restore_block_selector){case 0:break;'+''.join(f'case {j}:goto restore_roll_{j};' for j in range(1,6))+'default:abort();}'
model='unsigned restore_block_selector;\n'+model.replace(declaration,declaration+'\n    '+dispatch)
for j,(start,length) in enumerate(blocks,1):
 marker=f'/* {start:08x} */';assert model.count(marker)==1;model=model.replace(marker,f'restore_roll_{j}:;\n    '+marker)
 end=start if length==10 else 0x51c367
 marker=f'/* {end:08x} */';begin=model.index(marker);stop=model.index('\n',model.index('\n',begin)+1)
 model=model[:stop]+'\n    return;'+model[stop:]
(BUILD/'block_model.c').write_text(model)
(BUILD/'block_shim.c').write_text('#include "land_route_model.h"\nextern unsigned restore_block_selector;\nvoid restore_land_block(struct restore_menu_input_frame *f,unsigned entry){restore_block_selector=entry;restore_C_0051bc60(f);}\n')
(BUILD/'CMakeLists.txt').write_text(f'''cmake_minimum_required(VERSION 3.25)
project(LandRollBlocks C)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_library(land_roll_blocks SHARED block_model.c block_shim.c)
target_include_directories(land_roll_blocks PRIVATE "{phase.as_posix()}")
target_compile_options(land_roll_blocks PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(land_roll_blocks PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF /EXPORT:restore_land_block)
''')
cmake='C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
def command(args):
 p=subprocess.run(args,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 if p.returncode:print(p.stdout[-12000:]);raise SystemExit(p.returncode)
command([cmake,'-S',str(BUILD),'-B',str(BUILD/'out'),'-G','Visual Studio 18 2026','-A','Win32'])
STACK=0x30000000;SP=STACK+0x8000;STOP=0x31000000;FRAME=0x32000000
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
bits=[1,4,16,64,128,2048]
for config in ['Debug','Release']:
 command([cmake,'--build',str(BUILD/'out'),'--config',config]);candidate=PE(BUILD/'out'/config/'land_roll_blocks.dll');machines=[];events=[];cases=0
 for compiled in [False,True]:
  u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(native.base,(len(native.data)+4095)&~4095);u.mem_write(native.base,bytes(native.data))
  if compiled:u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
  for a,n in [(STACK,0x10000),(STOP,4096),(FRAME,4096)]:u.mem_map(a,n)
  def access(cpu,kind,a,n,value,unused):events.append((a,n,value&((1<<(n*8))-1)))
  u.hook_add(UC_HOOK_MEM_WRITE,access,begin=0xeae004,end=0xeae013);machines.append(u)
 for j,(start,length) in enumerate(blocks,1):
  for seed in range(64):
   flags=0x202|sum(bit for k,bit in enumerate(bits) if seed>>k&1);values=[((0x87654321+seed*0x10204081)^(k*0x13579bdf))&0xffffffff for k in range(8)];values[4]=SP;out=[]
   for compiled,u in enumerate(machines):
    u.mem_write(0xeae004,struct.pack('<4I',0x87654321,0xabcdef01,0x76543210,0x12345678));events.clear()
    for r,v in zip(regs,values):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,flags);normalized=u.reg_read(UC_X86_REG_EFLAGS)
    if compiled:
     u.mem_write(FRAME,struct.pack('<10I',*values,normalized,0));u.mem_write(SP,struct.pack('<3I',STOP,FRAME,j));u.emu_start(candidate.entry('restore_land_block'),STOP,count=100000)
     assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
     frame=struct.unpack('<10I',bytes(u.mem_read(FRAME,40)));result=list(frame[:8]);returned_flags=frame[8]
    else:
     u.emu_start(start,start+length,count=10);assert u.reg_read(UC_X86_REG_EIP)==start+length
     result=[u.reg_read(r) for r in regs];returned_flags=u.reg_read(UC_X86_REG_EFLAGS)
    # XOR's AF is architecturally undefined. All other tested flags, and
    # every flag for the four MOV-only blocks, have their precise meaning.
    mask=0xffffffff^(16 if length==7 else 0)
    out.append((result,returned_flags&mask,list(events),bytes(u.mem_read(0xeae004,16))))
   assert out[0]==out[1],(config,hex(start),seed,out)
   cases+=1
 print(f'PASS {config}: {cases} local block cases,5 blocks/6 original instructions,all GPR + defined flags/ordered writes/counters; derivative only in build; whole-function capacity paths remain OPEN',flush=True)
assert [sha(p) for p in inputs]==before,'Frozen input changed'
