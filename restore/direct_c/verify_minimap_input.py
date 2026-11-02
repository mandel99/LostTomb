"""Minimap click with actual projection, lookup and camera clamp; stdout only."""
from pathlib import Path
import array,hashlib,itertools,json,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_MEM_WRITE
from unicorn.x86_const import *
origin=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(origin.read_bytes()).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=PE(origin);candidate=PE(sys.argv[1]);functions={};new={0x4061e0,0x43ef10}
wanted=new|{0x43e670,0x4304c0,0x430cd0,0x4de080,0x4ddea0,0x4ddeb0,0x430ca0,0x430cb0}
for p in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 for e in json.loads(p.read_text())['entries']:
  a=int(e['requested_address'],16)
  if a not in wanted:continue
  assert not e['assembly']['has_more'];functions[a]=set()
  for i in e['assembly']['items']:
   pc=int(i['address'],16);b=bytes.fromhex(i['bytes'])
   assert native.data[pc-native.base:pc-native.base+len(b)]==b;functions[a].add(pc)
assert functions.keys()==wanted
owned=set.union(*(functions[a] for a in new));hits=set()
stack,sp,stop=0x30000000,0x3000c000,0x31000000
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
regions=[(0x605a30,0x20),(0x6fdc40,0x30),(0x705690,0x30),(0x70e3c0,0x80),(0x70e470,0x24),
 (0xbca2b0,0x40),(0xd3b4d0,0x30),(0xe38dc0,0x80),(0xe94db0,0x40),(0xf53f50,0x30)]
grid=0x70e494;grid_data=array.array('I',[0x0fff0000])*(231*457)
def body(a):
 for _ in range(5):
  p=a-candidate.base
  if candidate.data[p]!=0xe9:return a
  a+=5+struct.unpack_from('<i',candidate.data,p+1)[0]
 raise AssertionError('unresolved C entry thunk')
cstart,csecond,cend=[body(candidate.entry('FUN_'+a)) for a in ['004061e0','0043ef10','0043e670']]
assert cstart<csecond<cend
def machine(is_c):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(native.base,(len(native.data)+4095)&~4095);u.mem_write(native.base,bytes(native.data))
 if is_c:u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
 u.mem_map(stack,0x10000);u.mem_map(stop,4096);state={}
 def code(uc,a,n,user):
  if not is_c and a in owned:hits.add(a)
  if is_c:raise AssertionError(('C executed original text',hex(a)))
 if is_c:u.hook_add(UC_HOOK_CODE,code,begin=0x400000,end=0x55ffff)
 else:
  for a in new:u.hook_add(UC_HOOK_CODE,code,begin=min(functions[a]),end=max(functions[a]))
 def access(uc,kind,a,n,value,user):
  pc=uc.reg_read(UC_X86_REG_EIP)
  if not ((cstart<=pc<cend) if is_c else pc in owned):return
  if kind==UC_MEM_WRITE:state['trace'].append(('W',a,n,value&((1<<(8*n))-1)))
  else:state['trace'].append(('R',a,n,int.from_bytes(uc.mem_read(a,n),'little')))
 for a,n in regions:u.hook_add(UC_HOOK_MEM_READ|UC_HOOK_MEM_WRITE,access,begin=a,end=a+n-1)
 return u,state,is_c
machines=[machine(True),machine(False)];checked=0
def run(entry=0x43ef10,**c):
 global checked
 results=[]
 for u,state,is_c in machines:
  def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
  for a,n in regions:u.mem_write(a,b'\xa5'*n)
  u.mem_write(grid,grid_data.tobytes());u.mem_write(stack,b'\xa5'*0x10000)
  width,height=c.get('size',(7,9));x,y=c.get('origin',(20,30))
  mode=c.get('mode',0);selector=c.get('selector',1)
  if entry==0x4061e0:x=(c.get('offset',12)+8)&0xffffffff;y=30 if mode else 59;width,height=73,111
  for a,v in [(0x605a3c,mode),(0x6fdc50,c.get('expanded',0)),(0x7056a0,selector),(0xf53f64,c.get('offset',12)),
   (0x70e424,c.get('extent',(231,457))[0]),(0x70e420,c.get('extent',(231,457))[1]),(0x70e42c,0),(0x70e428,0),
   (0x70e464,40),(0x70e460,40),(0x70e478,c.get('camera',(0,0))[0]),(0x70e474,c.get('camera',(0,0))[1]),
   (0xe94dd0,c.get('bounds',(0,0,231,456))[0]),(0xe94dc8,c.get('bounds',(0,0,231,456))[1]),
   (0xe94dcc,c.get('bounds',(0,0,231,456))[2]),(0xe94dc4,c.get('bounds',(0,0,231,456))[3])]:put(a,v)
  u.mem_write(0xe38e03,bytes([c.get('click',1)]));u.mem_write(0xe38df7,bytes([c.get('key',0)]))
  # Use the real helper to construct the projection fixture, then compare the
  # complete call again. No replacement projection or synthetic callee return.
  put(sp,stop);put(sp+4,width);put(sp+8,height);u.reg_write(UC_X86_REG_ESP,sp)
  state['trace']=[];u.emu_start(candidate.entry('FUN_0043e670') if is_c else 0x43e670,stop,count=1000)
  source_x,source_y=[struct.unpack('<i',u.mem_read(a,4))[0] for a in [0x70e3dc,0x70e3d8]]
  dx,dy=c.get('pixel',(0,0));put(0xe38e34,(x+dx*2-(dy&1))&0xffffffff);put(0xe38e30,y+dy)
  col,row=source_x+dx,source_y+dy
  if 0<=col<231 and 0<=row<457:put(grid+(row*231+col)*4,c.get('tile',42))
  if c.get('miss'):put(0xe38e34,0x80000000)
  u.mem_write(stack,b'\xa5'*0x10000);put(sp,stop)
  for k,v in enumerate([x,y,width,height]):put(sp+4+k*4,v)
  initial=[0x12345678,0xabcdef01,0x89abcdef,0x22222222,sp,0x33333333,0x44444444,0x55555555]
  for r,v in zip(regs,initial):u.reg_write(r,v)
  u.reg_write(UC_X86_REG_EFLAGS,2);state['trace']=[]
  u.emu_start(candidate.entry('FUN_%08x'%entry) if is_c else entry,stop,count=3000000)
  assert u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==sp+4
  for k in [3,5,6,7]:assert u.reg_read(regs[k])==initial[k]
  assert u.mem_read(sp+20,16)==b'\xa5'*16 and u.mem_read(stack,32)==b'\xa5'*32
  results.append((u.reg_read(UC_X86_REG_EAX),tuple(bytes(u.mem_read(a,n)) for a,n in regions),state['trace']))
 assert results[0]==results[1],(hex(entry),c,[(i,a,b) for i,(a,b) in enumerate(zip(results[0],results[1])) if a!=b][:1])
 checked+=1
for size in [(7,9),(-8,-8),(-4,4),(-3,4),(0,0),(73,111)]:
 for pixel in [(-4,-4),(-3,-3),(-1,-1),(0,0),(1,1),(6,8),(7,9),(10,10)]:run(size=size,pixel=pixel)
for camera,extent,pixel in itertools.product([(0,0),(100,100),(0x80000000,0x80000000),(231,457)],[(231,457),(2,2),(0xffffffff,0xffffffff)],[(0,0),(1,1),(-4,-4)]):run(camera=camera,extent=extent,pixel=pixel)
for tile in [0,1,0x0ffeffff,0x0fff0000,0x7fffffff,0x80000000,0xffffffff]:run(tile=tile)
for mode,selector,click,key in itertools.product([0,1,0x100],[0,1,0x101,0x100],[0,1],[0,1]):run(0x4061e0,mode=mode,selector=selector,click=click,key=key)
for mode,tile,bounds in itertools.product([0,1,0x100],[0,42,0x0fff0000,0x80000000],[(0,0,231,456),(100,100,101,102)]):run(0x4061e0,mode=mode,tile=tile,bounds=bounds,expanded=1)
for offset in [0xff,0x100,0xfffffffc,0x7ffffffc]:run(0x4061e0,offset=offset)
run(miss=True);run(0x4061e0,miss=True)
missing=owned-hits;assert not missing,('uncovered new instructions',[hex(a) for a in sorted(missing)])
print(f'PASS {checked} actual-C/native minimap graph cases;143/143 new instructions;fullEAX/RET0/nonvolatiles/fields/ordered reads-writes/stack;real projection+tile scan+camera clamp;no finite callees or original-text execution')
