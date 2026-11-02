"""Pinned native placement controller versus its handwritten C, without a game process."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,functools,hashlib,importlib.util,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-placement-preview/out/Debug/restore_placement_preview_verify.exe');parser.add_argument('--production',action='store_true');parser.add_argument('--debug-limit',type=int);parser.add_argument('--debug-start',type=int,default=0);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/placement_preview_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s32=lambda x:struct.unpack('<i',struct.pack('<I',x&0xffffffff))[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;ALT=BASE+0x10000;OBJ=BASE+0x1000;SRC=BASE+0x400000;ACTOR=BASE+0x500000;OUT=BASE+0x600000;STOP=0x33000000;STACK=0x34000000;KEY=STOP+0x1000
for a,n in [(BASE,0x800000),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def write(a,v,n=4):vm.mem_write(a&0xffffffff,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a&0xffffffff,4),'little')
def byte(a):return vm.mem_read(a&0xffffffff,1)[0]
objects=[0x6fd5d8,0x6de650,0x6fd368,0x6f9320,0x6f5648,0x6de9e0,0x6e9188,0x6de548];vtables=[0x56f48c,0x56f438,0x56f4dc,0x570160,0x570268,0x570228,0x5701e8,0x56f6e8];adapters=[0x40d870,0x401420,0x416f90,0x5456e0,0x558b80,0x555e20,0x54f410,0x4e6610];offsets=[0x4c,0x4c,0x4c,0x1e5c,0x1cbc,0x4ee0,0x5c88,0x4c];counts=[25,16,9,167,158,437,513,2];member_modes={0xe1:0,0xe6:1,0xe3:2,0xe5:3,0xea:4,0xeb:5,0xec:6,0xe4:7}
tags=[0xf564dc,0xf5653c,0xf56544,0xf566ac,0xf566b4,0xf56764,0xf56b24,0xf56b84,0xf56c34,0xf56c44,0xf56c94,0xf56d34,0xf56d54,0xf56d5c,0xf56d64]
regions=[(0x576140,12),(0x605980,240),(0x6de50c,16),(0x70e118,336),(0x70e440,84),(0x705c68,12),(0xbc96f0,152),(0xbca2c8,152),(0xd3b4d8,280),(0xe38dc8,12),(OUT,64)]
exports=['metadata/viewport-endpasses-next-native.json','metadata/camera-sidebar-coordinate-native.json','metadata/placement-preview-callee-native.json','metadata/placement-preview-member-draw-native.json','metadata/placement-preview-predicate-cfg-native.json','metadata/placement-preview-rule-predicates-native.json','metadata/placement-preview-object-ctors-native.json'];audit={};ranges=[];instruction_map={};function_addresses={};executed=set();edges=set();last=None
def audit_body(entry,ins):
 for x in ins:
  address=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw,'Native byte disagreement '+x['address'];instruction_map[address]=x
 ranges.append((int(ins[0]['address'],16),int(ins[-1]['address'],16)+len(bytes.fromhex(ins[-1]['bytes']))));audit[entry]={'instructions':len(ins),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in ins)};function_addresses[entry]={int(x['address'],16) for x in ins}
def dossier(index,entries):
 d=json.loads((ROOT/exports[index]).read_text())['functions']
 for a in entries:
  f=d[a];assert not f['assembly'].get('has_more',False);audit_body(a,f['assembly']['items'])
dossier(0,['0043b630']);dossier(1,['004f0ba0']);dossier(2,['0053f4f0','004e5490']+[f'{a:08x}' for a in adapters]);dossier(3,['00545fc0'])
predicates=set();rules=set()
for i,targets in [(4,predicates),(5,rules)]:
 for entry,f in json.loads((ROOT/exports[i]).read_text())['functions'].items():audit_body(entry,f['instructions']);targets.add(int(entry,16))
ctor_stores=[(0x40d2d9,objects[0],0,vtables[0]),(0x401089,objects[1],0,vtables[1]),(0x416c7a,objects[2],0,vtables[2]),(0x54313c,objects[3],0,vtables[3]),(0x556827,objects[4],0,vtables[4]),(0x54ff47,objects[5],0,vtables[5]),(0x54813b,objects[6],0,vtables[6]),(0x4e6455,objects[7],0,vtables[7]),(0x54314b,objects[3],0x50,0xa6),(0x556837,objects[4],0x50,0xa),(0x54ff57,objects[5],0x50,0x1b4),(0x548147,objects[6],0x50,0x1f)]
ctor_items={int(x['address'],16):x for f in json.loads((ROOT/exports[6]).read_text())['functions'].values() for x in f['assembly']['items']}
for a,obj,off,value in ctor_stores:
 ins=ctor_items[a];expected=(b'\xc7\x46\x50' if off else b'\xc7\x06')+struct.pack('<I',value);assert bytes.fromhex(ins['bytes'])==expected;audit_body(f'constructor-store-{a:08x}',[ins])
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
seed_provenance='metadata/preview-footprints-provenance.json';seed_native='metadata/preview-footprints-native.json';seed_sets=json.loads((ROOT/seed_provenance).read_text())['sets'];geometry_dossier=json.loads((ROOT/'metadata/placement-preview-objects-native.json').read_text())['functions'];geometry_entries=set()
spec=importlib.util.spec_from_file_location('placement_native_seed_recovery',ROOT/'tools/extract_preview_footprint_seeds.py');seed_recovery=importlib.util.module_from_spec(spec);spec.loader.exec_module(seed_recovery)
_,_,independent_sets,independent_rows,seed_prefix_hits=seed_recovery.recover(ROOT/seed_native)
assert seed_sets==independent_sets,'Seed provenance differs from independent native static and constructor-prefix execution'
assert (ROOT/'runtime/preview_footprint_seeds.c').read_text()==seed_recovery.runtime_source(independent_sets)
for s in seed_sets:
 entry=int(s['geometry'],16);geometry_entries.add(f'{entry:08x}');f=geometry_dossier[f'{entry:08x}']['assembly'];assert not f['has_more'];audit_body(f'{entry:08x}',f['items'])
f=json.loads((ROOT/seed_native).read_text())['functions']['004e52b0']['assembly'];assert not f['has_more'];audit_body('004e52b0',f['items']);geometry_entries.add('004e52b0')
core=[]
for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(vm.mem_read(0x4e5190,0x100)),0x4e5190):
 core.append({'address':hex(ins.address),'bytes':ins.bytes.hex(),'instruction':ins.mnemonic+' '+ins.op_str})
 if ins.mnemonic=='ret':break
assert int(core[-1]['address'],16)==0x4e5203;audit_body('004e5190',core);geometry_entries.add('004e5190')
# Every retained caller of the global masked-draw facade explicitly binds this receiver.
mask_xrefs=json.loads((ROOT/exports[2]).read_text())['functions']['0053f4f0']['xrefs']
def xref_items(d):
 if isinstance(d,list):return d
 return d.get('items',d.get('references',[]))
mask_bindings=[]
for x in xref_items(mask_xrefs):
 address=int(x.get('from_address',x.get('from',x.get('address','0'))),16)
 if address:
  assert bytes(vm.mem_read(address-5,5))==bytes.fromhex('b9b05bf500');mask_bindings.append(address)
assert len(mask_bindings)==34
deep={0x43d720:0,0x441490:0,0x541c00:0,0x4debe0:0,0x4edbb0:0,0x43e090:0,0x4bfcd0:0,0x441f80:0,0x43d300:0,0x43df10:1,0x43f5b0:1,0x4c0f30:1,0x4c2f50:1,0x43dd70:2,0x482650:5,0x482720:5,0x4828e0:5,0x4829b0:6,0x46f9a0:3,0x4ec410:2,0x477d40:2,0x46fde0:1,0x47ed60:1,0x47b920:0,0x487f80:1,0x489470:4,0x489050:4,0x51c510:1,0x51c5f0:2,0x51c390:1,0x51c460:1,0x47ed30:2,0x47ed00:2,0x51c630:1,0x47e870:0,0x484be0:4,0x47f200:3,0x4f0ab0:4,0x53e590:3,0x53f250:3,0x4cdad0:0,0x4cdb80:0,0x4100f0:2,0x4101c0:2,0x410700:2,0x4109d0:2,0x410a70:2,KEY:1}
return_bits={0x4ec410:0,0x477d40:1,0x46fde0:2,0x47ed60:3,0x47b920:4,0x489470:5,0x51c510:6,0x51c390:7,0x47ed30:8,0x47ed00:9,0x51c630:10}
events=[];p=[];region_calls=0;current_record=0
def event(a,argv):
 row=[a,len(argv)]+[v&0xffffffff for v in argv]+[0]*(24-2-len(argv));row[10:21]=[int.from_bytes(vm.mem_read(0x70e120,2),'little'),read(0xbc9768),read(0x70e13c),read(0xd3b580),read(0xd3b57c),read(0xd3b578),read(0xd3b544),read(0x6059e8),read(0xbc9714),read(0xbc96f8),read(0xbca300)];events.append(row)
def record_for(self,image):return (self+0x180e48+(image if s32(image)<s32(read(self+0x30f5cc)) else 0)*64)&0xffffffff
def code(cpu,a,n,user):
 global region_calls,current_record,last
 if a==STOP:return
 if last is not None:edges.add((last,a))
 last=a;executed.add(a)
 if a not in deep and a not in [0x4f0ba0,0x53f4f0] and a not in adapters and a not in predicates and a not in rules:
  assert a in instruction_map,'Native escaped '+hex(a)
  return
 sp=cpu.reg_read(UC_X86_REG_ESP);self=cpu.reg_read(UC_X86_REG_ECX)
 if a==0x4f0ba0:
  v=[read(sp+4+i*4) for i in range(4)];event(a,v[:2]+[1,1 if v[2]==v[3] else 2])
 elif a==0x53f4f0:
  v=[read(sp+4+i*4) for i in range(4)];event(a,[self]+v);current_record=record_for(self,v[0])
 elif a in adapters:event(a,[self])
 elif a in predicates:event(a,[self,read(sp+4),read(sp+8)])
 elif a in rules:event(a,[read(sp+4)])
 elif a in deep:
  v=[read(sp+4+i*4) for i in range(deep[a])];shown=v.copy();ret=0;cleanup=0
  if a==0x4f0ab0:shown=v[:2]+[1,1 if v[2]==v[3] else 2]
  if a in [0x53e590,0x53f250]:assert self==0xf55bb0;shown=[self]+v;cleanup=12
  if a in [0x487f80,0x489470,0x489050]:assert self==0xbca350;shown=[self]+v;cleanup=4 if a==0x487f80 else 16
  event(0x56f1e4 if a==KEY else a,shown)
  if a in return_bits:ret=(p[12]>>return_bits[a])&1
  elif a==0x482650:
   if p[41] and not region_calls:write(0x6059e8,p[41])
   ret=p[18] if region_calls else p[17];region_calls+=1
  elif a in [0x482720,0x4828e0]:ret=p[18]
  elif a==0x4829b0:ret=int(p[18]!=0)
  elif a==0x46f9a0:
   ret=p[19]
   if p[0]==3 and p[41]&0x100:write(0xbca2e0,p[41]&255)
  elif a==0x487f80:ret=p[23]
  elif a==0x489050:ret=p[22]
  elif a==0x51c5f0:ret=p[20]
  elif a==0x51c460:ret=p[21]
  elif a==0x47e870:write(0xbc9748,p[22]&1)
  elif a==0x484be0:ret=1
  elif a==0x47f200:
   for target,value in zip([0xbc96f4,0xbc96f8,0xbc96fc],p[50:53]):write(target,value)
  elif a==0x4f0ab0:write(v[2],p[61]);write(v[3],p[62])
  elif a==KEY:ret=p[49]&65535;cleanup=4
  elif a==0x53e590:
   if 0<s32(v[0])<25500:
    for target,value in zip([0xd3b580,0xd3b57c,0xd3b578],v):write(target,value)
   ret=p[28]
   if p[27]&16:write(0x70e13c,0xffffffff)
   if p[27]&32:write(0xbc96f8,1)
  elif a==0x53f250:ret=1
  elif a==0x4cdad0:
   write(0xd3b530,p[30],1)
   if p[27]&2:write(0xd3b544,p[28]+256)
   if p[27]&4:write(current_record+51,1,1)
   if p[27]&8:
    offset=s32(read(0x5cfe10+(p[5]//2)*144+4));write(0xd05720+(p[4]+offset)*4,ACTOR)
  elif a==0x4cdb80:write(0xd3b531,p[31],1)
  elif a in [0x4100f0,0x4101c0,0x410700,0x4109d0,0x410a70]:
   if p[27]&1:write(0xd3b544,read(0xd3b544)+1)
  cpu.reg_write(UC_X86_REG_EAX,ret&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);return
 assert a in instruction_map,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,code);write(0x56f1e4,KEY)
@functools.lru_cache(maxsize=16)
def sprite_fixture(width,height,color,compressed,present,next_image,extra):
 b=bytearray(0x30f5d8);r=bytearray(64);struct.pack_into('<I',r,0,SRC+extra if present else 0);struct.pack_into('<HH',r,20,(width+(3 if extra else 0))&65535,(height+(5 if extra else 0))&65535);r[50]=color&255;r[51]=compressed&255;b[0x180e48:0x30f548]=r*25500;struct.pack_into('<I',b,0x30f5cc,next_image);return bytes(b)
def piece(ptr,side,kind):
 for off,value in [(0,1),(4,-1),(8,side),(12,kind)]+[(20+i*4,10+i*3) for i in range(4)]+[(36+i*4,20-i*2) for i in range(4)]:write(ptr+off,value)
def prepare(params):
 global p,region_calls,current_record,last
 p=params;events.clear();region_calls=0;current_record=0;last=None;op=p[0];ident=member_modes[p[1]] if op==0 and p[1] in member_modes else p[37]%8;obj=OBJ if p[36] else objects[ident];ofs=offsets[ident];tile=s32(p[4])
 vm.mem_write(BASE,bytes(0x10000));vm.mem_write(OUT,b'\xa5'*64);vm.mem_write(0x6de000,bytes(0x20000))
 for address,extra in [(0xf55bb0,0),(ALT,0x1000)]:vm.mem_write(address,sprite_fixture(p[28],p[29],p[33],p[32],p[34],p[35],extra))
 for a,n in regions:
  if a!=OUT:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(0xaf598c,struct.pack('<I',p[8])*51984);vm.mem_write(0xd05720,bytes(51984*4));vm.mem_write(0xb350dc,struct.pack('<H',p[10]&65535)*51984);vm.mem_write(0xb285cc,bytes([p[56]&128])*51984);vm.mem_write(0xadc36c,bytes([p[56]&1])*51984)
 if p[9]:
  for i in range(36):write(0xaf598c+(tile+s32(read(0x5cfe10+(p[5]//2)*144+i*4)))*4,p[8] if i&1 else p[8]^p[9])
 vm.mem_write(ACTOR,b'\xa5'*64);write(ACTOR+24,0 if p[11]==1 else p[11],1)
 if p[11]:write(0xd05720+tile*4,ACTOR)
 for i,a in enumerate(tags):write(a,p[24]+i*16)
 write(0xf56be4,p[25]);write(0xf55bb4+p[26]*8,p[24]);definition=(0x5d06dc+p[1]*20)&0xffffffff
 if p[1]<300:
  for off,value in [(0,p[6]),(8,p[26]),(12,p[63])]:write(definition+off,value)
 values={0x6059e8:p[1],0x605a3c:p[3],0xbca300:0 if p[2]&1 else tile,0xe38dcc:256 if p[2]&2 else 0,0x605994:256 if p[2]&8 else 0,0x6059e4:p[53],0xbc9724:p[54],0xbc9744:p[57],0xbc9748:p[22]&1,0x605a48:0,0x605a28:p[59],0xbca2d0:228,0xbca2d4:228,0xbca2d8:0,0xbca2e0:p[5],0xbca2e8:tile%228,0xbca2ec:int(tile/228),0xbca304:p[14],0xbca308:p[15],0x70e45c:p[43],0x70e44c:p[44],0x70e448:3,0x70e490:p[45],0x70e48c:p[46],0x70e474:2,0x70e478:3,0xeaac28:0,0xea187c:p[7],0xea620c:(p[59]>>16)&1,0xea5e70:(p[59]>>17)&1,0xea6240:(p[59]>>18)&1,0x705c6c:(p[59]>>19)&1,0xbc9714:p[48],0x576144:p[55],0x70e13c:0}
 for a,v in values.items():write(a,v)
 write(0x605a5c,int(bool(p[2]&4)),1);write(0xea5cf6,(p[59]>>8)&65535,2);write(0x70e120,0x1234,2)
 for i in range(7):write(0x6059b4+i*4,0)
 if p[13]:write([0,0x6059cc,0x605a48,0x6059c8,0x6059c4,0x6059c0,0x6059bc,0x6059b8,0x6059b4][p[13]],0x405b40+(p[58]%3)*32 if p[13]==2 else 256)
 lookup=0x70e494+(2*231+3)*4;write(lookup,0xfff0000 if p[47] else tile);vm.mem_write(SRC,b'\x55'*256)
 if not p[16]:
  for a,obj0,off,value in ctor_stores:write(obj0+off,value)
 vm.mem_write(OBJ-8,bytes(0xe010))
 if p[36]:write(OBJ,vtables[ident]);write(OBJ+0x50,p[40])
 if p[16]:
  for s in seed_sets:
   address=int(s['object'],16)+s['offset']
   for record in s['records']:vm.mem_write(address+record['index']*52,struct.pack('<4i',*record['tuple']))
  for s in sorted(seed_sets,key=lambda s:[0x4013f0,0x40d7e0,0x416f60,0x4e6590,0x5456a0,0x558b40,0x555de0,0x54f3d0].index(int(s['geometry'],16))):
   sp=STACK+0x180000;write(sp,STOP);vm.reg_write(UC_X86_REG_ECX,int(s['object'],16));vm.reg_write(UC_X86_REG_ESP,sp);vm.emu_start(int(s['geometry'],16),STOP,count=100000)
   assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
  for a,obj0,off,value in ctor_stores:write(obj0+off,value)
 elif op==0 and p[1]>=0xe0:
  for i in range(8):
   selected=read(objects[i]+0x50) if 3<=i<=6 else 0;piece(objects[i]+offsets[i]+selected*52,p[39],9)
   if selected:piece(objects[i]+offsets[i],p[39],0)
 else:
  piece(obj+ofs,p[39],9)
  if p[38]>1:piece(obj+ofs+52,p[39],0)
 if op==3 and p[41]&1:
  center=tile-228+1
  for grid in [0xaf55fc,0xaf5d1c,0xaf5988,0xaf5990]:write(grid+center*4,2)
  write(0xaf598c+center*4,0)
 if op==5:
  for obj0 in objects:write(obj0,0);vm.mem_write(obj0-8,b'\xa5'*8);vm.mem_write(obj0+4,b'\xa5'*8)
  for obj0 in objects[3:7]:write(obj0+0x50,0)
 return ident,obj,lookup,definition
def native(params):
 ident,obj,lookup,definition=prepare(params);op=p[0];sp=STACK+0x1e0000;write(sp,STOP);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_EAX,0xa5a5a500);cleanup=0;result=0
 if op==5:
  for a,obj0,off,value in ctor_stores:vm.reg_write(UC_X86_REG_ESI,obj0);vm.emu_start(a,a+len(bytes.fromhex(ctor_items[a]['bytes'])),count=1)
 else:
  entry=0x43b630;argv=[]
  if op==1:entry=0x4f0ba0;argv=[p[14],p[15],OUT+8,OUT+(8 if p[60] else 24)]
  elif op==2:entry=0x53f4f0;argv=[p[24],p[14],p[15],p[42]];vm.reg_write(UC_X86_REG_ECX,ALT if p[36] else 0xf55bb0);cleanup=16
  elif op==3:entry=0x545fc0 if 3<=ident<=6 else 0x4e5490;argv=[obj+offsets[ident],p[38]];vm.reg_write(UC_X86_REG_ECX,obj);cleanup=8
  elif op==4:entry=read(vtables[ident]+16);argv=[p[42],p[40]];vm.reg_write(UC_X86_REG_ECX,obj);cleanup=8
  elif op==6:entry=0x405b40+(p[58]%3)*32;argv=[p[42]]
  elif op==7:entry=0x4e52b0;argv=[obj+offsets[ident],p[38]];cleanup=8
  for i,v in enumerate(argv):write(sp+4+i*4,v)
  vm.emu_start(entry,STOP,count=2000000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup,'Native stack/budget differs'
  assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
  if op in [2,4,6]:result=vm.reg_read(UC_X86_REG_EAX)&(255 if op in [4,6] else 0xffffffff)
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)+bytes(vm.mem_read(lookup-8,20))
 if p[1]<300:state+=bytes(vm.mem_read(definition,20))
 for obj0 in objects:state+=bytes(vm.mem_read(obj0-8,20))+bytes(vm.mem_read(obj0+0x48,16))
 if op in [3,4,7] or op==0 and p[1] in member_modes:state+=bytes(vm.mem_read(obj,offsets[ident]+counts[ident]*52+16))
 if op==2:state+=bytes(vm.mem_read(current_record-8,80))
 if op==5:state+=bytes(vm.mem_read(0x6de000,0x20000))
 state+=bytes(vm.mem_read(ACTOR,64))
 for i in range(36):state+=bytes(vm.mem_read(0xd05720+((p[4]+read(0x5cfe10+(p[5]//2)*144+i*4))&0xffffffff)*4,4))
 return {'result':result,'state':state.hex(),'events':events.copy()}
fields=['op','mode','gates','editor','tile','orientation','size','treasury','terrain','terrain_xor','building','actor','returns','priority','x','y','startup','first_region','region','edge','road_variant','road_shape','well_style','special_AL','asset','foot_asset','group','mutation','width','height','hidden','clip','compressed','color_key','source','next_image','alternate','adapter','count','side','selected','after_mode','mask','dx','dy','screen_x','screen_y','sentinel','key_global','key_short','bridge_direction','bridge_length','bridge_valid','well_layout','well_alternative','resolution','byte_grid','well_flag','rule','setting','alias','fallback_y','fallback_x','asset_offset']
default=[0]*64
for k,v in dict(mode=1,tile=2300,size=2,treasury=0,x=10,y=10,region=1,asset=335,foot_asset=100,width=8,height=9,source=1,next_image=25500,count=1,side=1,dx=29,dy=15,screen_x=192,screen_y=144,bridge_length=7,bridge_valid=1,resolution=1,fallback_y=4,fallback_x=7,road_shape=15).items():default[fields.index(k)]=v
cases=[]
def add(**kw):
 q=default.copy()
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(q)
for mode in range(0xed):add(mode=mode)
for mode,gates,editor in itertools.product([0,1,5,8,0x53,0x7e,0x84,0xe5,0xec,0xffffffff],[0,1,2,4,6,8],[0,1,256]):add(mode=mode,gates=gates,editor=editor)
for mode,orientation,terrain in itertools.product([1,5,8,10,11,0x1d,0x26,0x38,0x3b,0x47,0x48,0x77,0x7e,0x7f,0x80,0x82,0x83,0x84,0x85,0x8a,0x8b,0xac,0xad,0xbf,0xc1,0xc2,0xc3,0xcf,0xd0,0xd6],[0,2,4,6],[0,2,4,8,0x40,0x100,0x10000,0x20000002]):add(mode=mode,orientation=orientation,terrain=terrain,returns=2047,edge=1)
for priority,first,other in itertools.product(range(9),[0,1,256],[0,1,256]):add(priority=priority,first_region=first,region=other)
for mode,setting in itertools.product([0xbb,0xbc,0xbd,0x4d,0x4e,0x4f,0x5f,0x32,0x5e],[0,1<<8,1<<16,1<<17,1<<18,1<<19]):add(mode=mode,setting=setting)
for size,terrain,terrain_xor,building,actor in itertools.product([1,2,3,4,5,6],[0,0x10000],[0,0x10000],[0,256],[0,1,2]):add(size=size,terrain=terrain,terrain_xor=terrain_xor,building=building,actor=actor,returns=4)
for mode,x,y,editor in itertools.product([1,0x48],[2,3,221,222],[2,3,221,222],[0,256]):add(mode=mode,x=x,y=y,editor=editor)
for mode,treasury in itertools.product([1,5,8,0x52,0x53],[-5001,-5000,-4999,0,0x7fffffff]):add(mode=mode,treasury=treasury)
for key_global,key_short in itertools.product([-1,0,1,2,256],[0,1,0x8000,0x8001,0xffff]):add(key_global=key_global,key_short=key_short)
for mode,orientation,length,valid in itertools.product([0x52,0x53],[0,2,4,6],range(17),[0,1]):add(mode=mode,orientation=orientation,bridge_direction=0,bridge_length=length,bridge_valid=valid)
for orientation,length,direction in itertools.product([0,2,4,6],[5,7,9,10,11,12,13,14,15,16],[0,2,4,6]):add(mode=0x53,orientation=orientation,bridge_direction=direction,bridge_length=length)
for mode,terrain,returns,byte_grid,style in itertools.product([5,8],[0,0x40,0x100,0x10000],[0,2047],[0,128],[0,1]):add(mode=mode,terrain=terrain,returns=returns,byte_grid=byte_grid,well_style=style)
for well_layout,well_alternative,well_flag in itertools.product([0,1,256],[0,1,256],[0,1,256]):add(mode=8,well_layout=well_layout,well_alternative=well_alternative,well_flag=well_flag,terrain=0x40,returns=2047)
for special_AL,returns,style in itertools.product([0,1,256],[0,32],[0,14,27,28,0xffffffff]):add(special_AL=special_AL,returns=returns,well_style=style)
for mode,resolution in itertools.product([0xcf,0xd0],[0,1,2,3,256]):add(mode=mode,resolution=resolution)
for op,orientation,sentinel,alias in itertools.product([1],[0,1,2,3,4,5,6,7],[0,1],[0,1]):add(op=op,orientation=orientation,sentinel=sentinel,alias=alias)
for alternate,image,hidden,clip,compressed,source in itertools.product([0,1],[0,1,25499,25500,0xffffffff],[0,5],[0,1,2,3],[0,1],[0,1]):add(op=2,alternate=alternate,asset=image,hidden=hidden,clip=clip,compressed=compressed,source=source,mask=0xfffff863)
for alternate,width,mutation,next_image in itertools.product([0,1],[0,1,32767,32768,65535],[0,1,2,3,4],[0,1,256,25500]):add(op=2,alternate=alternate,width=width,mutation=mutation,next_image=next_image,mask=0x12341fe3)
for adapter,orientation,side,terrain in itertools.product(range(8),[0,2,4,6],[0,1,2,6],[0,2,0x20000002,0x10000]):add(op=3,adapter=adapter,orientation=orientation,side=side,terrain=terrain,count=2,alternate=1,selected=1)
for adapter,terrain,index in itertools.product(range(8),[0,2,0x20000002,0x10000,0xffffffff],[0,1,2,9]):add(op=4,adapter=adapter,mask=terrain,selected=index,alternate=1,count=2)
for mode,orientation,terrain in itertools.product(member_modes,[0,2,4,6],[0,2,0x20000002]):add(mode=mode,orientation=orientation,terrain=terrain)
for edge in [0,1,256]:add(op=3,adapter=3,alternate=1,count=2,selected=1,edge=edge)
for rule,terrain in itertools.product(range(3),[0,2,0x100002,0x20000002,0x20100002,0x100000,0xffffffff]):add(op=6,rule=rule,mask=terrain)
for mode,orientation,terrain in itertools.product(member_modes,[0,2,4,6],[0,0x20000002]):add(mode=mode,orientation=orientation,terrain=terrain,startup=1)
for mode in member_modes:add(mode=mode,startup=1,edge=1)
for mode,actor in itertools.product([5,8],[1,2,128]):add(mode=mode,actor=actor,tile=2290)
add(mode=5,returns=512)
add(mode=8,terrain=0x40,returns=1024,well_layout=1,well_alternative=1)
for terrain in [0,4]:add(mode=0x7e,setting=9,edge=1,terrain=terrain)
add(terrain=0x10000,returns=0)
for road_shape in [1,15,16,36,42]:add(mode=5,terrain=0x100,returns=128,road_shape=road_shape)
for adapter,terrain,index in itertools.product(range(8),[0,2,0x20000002,0x10000,0xffffffff],[0,1,2,9]):add(op=4,adapter=adapter,mask=terrain,selected=index,alternate=0,count=2)
for after_mode in [0x29,0x2a,0x2b]:add(after_mode=after_mode,key_global=256)
add(terrain=0x10000,mutation=8)
for adapter,count in itertools.product(range(8),[0,0xffffffff]):add(op=3,adapter=adapter,count=count,alternate=1)
for mode in [0x7e,0x84,0x85,0x7f,0x80,0x77,0x82,0x83,0x8b,0xd6,0x58,0x59,0x5d,0x81]:add(mode=mode,editor=1)
for x,y in [(225,10),(10,225)]:add(x=x,y=y)
for mode,treasury in itertools.product([1,11],[0,-5000]):add(mode=mode,size=0,treasury=treasury)
add(mode=0x8a,terrain=0x10040);add(mode=0x8a,terrain=0x40,byte_grid=1)
for mode,direction,length in itertools.product([0x52,0x53],[1,3,5,7],[7,12]):add(mode=mode,bridge_direction=direction,bridge_length=length)
add(mode=0x52,bridge_direction=1,bridge_valid=0)
for mutation in [16,32]:add(mode=0x53,mutation=mutation)
add(op=3,adapter=3,alternate=1,count=2,selected=1,after_mode=1)
for adapter in [0,3]:add(op=3,adapter=adapter,alternate=1,count=2,selected=1,after_mode=0x108)
for adapter,count in itertools.product(range(8),[0,0xffffffff,1,2]):add(op=7,adapter=adapter,alternate=1,count=count)
add(op=5)
cases=list(dict.fromkeys(tuple(c) for c in cases))
if args.debug_start:cases=cases[args.debug_start:]
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'stage':'production' if args.production else str(phase).replace('\\','/'),'native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete43B630 dispatcher plus actual projection, masked draw controller, eight receiver adapters, two complete footprint controllers, four virtual predicates and three rule predicates executed as pinned x86 and compared with handwritten C. Ordered endpoint args and consumed returns, typed memory and adjacent canaries, original stack cleanup and saved registers are compared. Additional zero-BSS startup profiles compose all1327 recovered native seed tuples, all8 actual geometry wrappers+record bodies, and12 verified class-token/selector stores before the full original/C dispatcher.','limitations':['Deep terrain validators/game actions, 4F0AB0 fallback coordinate lookup, clipping classifiers and sprite raster endpoints are named controlled endpoints. This proves controller selection, argument/state order and consumed return contracts, not those deep algorithms or framebuffer pixels. Untouched region validators such as unit01203 still declare map origin/dimensions as BYTE; their full production validation for a nonzero DWORD origin is not established here.','Semantic fixtures explicitly vary piece sizes/type/selector; separate startup profiles use exactly the independently proved1327 tuples and actual geometry. Only the12 evidenced class stores are reconstructed; unrelated C++ constructor state is outside this boundary.','Unowned toolbar unit00042 currently writes BYTE605A48 although native writes a full function token. The owned resolver accepts only405B40/405B60/405B80 and diagnoses unknown/truncated IDs; no guessed recovery or native code-pointer call is used. Actual toolbar selection and complete construction gameplay are not claimed.','Controlled region and mask/clip mutations additionally exercise mode and occupancy reloads, including the later29..2B variant and second-pass live-actor arm. These mutation fixtures establish boundary effects, not an assertion that the current deep algorithms perform those writes. Valid map indices, size0..6 and even orientation0/2/4/6 are used for the main/member controllers; direct projection additionally checks untouched outputs on unsupported orientations.']}
checked=0;case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':64}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  for key in expected:
   if actual[key]!=expected[key]:
    detail=[actual[key],expected[key]]
    if key=='state':
     aa=bytes.fromhex(actual[key]);bb=bytes.fromhex(expected[key]);ix=next((i for i,(a,b) in enumerate(zip(aa,bb)) if a!=b),None);detail={'first_difference':ix,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':bb[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    elif key=='events':
     ix=next((i for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b),min(len(actual[key]),len(expected[key])));detail={'first_difference':ix,'C_count':len(actual[key]),'native_count':len(expected[key]),'C_event':actual[key][ix:ix+1],'native_event':expected[key][ix:ix+1]}
    raise AssertionError(key+' differs '+str(detail))
  checked+=1
  if checked%500==0 or case[16] and checked%10==0:print('checked',checked,file=sys.stderr,flush=True)
 if not (args.debug_limit or args.debug_start):assert all(a in executed for a in instruction_map if 0x43b630<=a<=0x43d003),'Complete dispatcher instruction coverage not reached'
 for token in [0,0x40,0x60,0x80,0x405b00,0xdeadbeef]:
  rejection=subprocess.run([str(exe),'--reject-rule',hex(token)],cwd=ROOT,capture_output=True,text=True,timeout=10);assert rejection.returncode!=0 and f'Unreconstructed or truncated placement rule {token:08x}' in rejection.stderr
 report.update(passed=True,checked=checked,complete_case_set=not bool(args.debug_limit or args.debug_start),unknown_rule_rejection=True)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
main_addresses={a for a in instruction_map if 0x43b630<=a<=0x43d003};report['controller_instruction_coverage']={'executed':len(main_addresses&executed),'total':len(main_addresses),'unreached':[hex(a) for a in sorted(main_addresses-executed)]};report['native_byte_audit']=audit;report['newly_restored_native_instructions']=sum(v['instructions'] for k,v in audit.items() if k not in geometry_entries);report['existing_geometry_native_instructions']=sum(audit[k]['instructions'] for k in geometry_entries);report['native_bytes_verified']=sum(v['bytes'] for v in audit.values());report['global_mask_receiver_bindings']=[hex(a) for a in mask_bindings];report['smoke']={'passed':True,'output':smoke.stdout.strip()};report['controlled_endpoints']={hex(a):{'stack_arguments':n,'native_cleanup_bytes':12 if a in [0x53e590,0x53f250] else 16 if a in [0x489470,0x489050] else 4 if a in [KEY,0x487f80] else 0} for a,n in deep.items()}
report['startup_profiles']=sum(bool(c[16]) for c in cases[:checked]);report['seed_records_independently_recovered']=sum(s['count'] for s in independent_sets);report['seed_native_prefix_instructions']=len(seed_prefix_hits);report['unknown_or_truncated_tokens_rejected']=[hex(t) for t in [0,0x40,0x60,0x80,0x405b00,0xdeadbeef]] if report.get('unknown_rule_rejection') else []
branch_coverage={}
for a,item in instruction_map.items():
 ins=next(iter(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(item['bytes']),a)),None)
 if ins and ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':
  target=int(ins.op_str,16);fall=a+len(ins.bytes);branch_coverage[hex(a)]={'instruction':item['instruction'],'taken':(a,target) in edges,'fallthrough':(a,fall) in edges}
report['native_conditional_branch_coverage']=branch_coverage
qualified={
 '0x4e5582':{'excluded_outcome':'taken','reason':'First row:4E5562..556C reads[EDI]=side and skips the entire piece when side<=0;4E5572..5582 has no calls or stores throughEDI. Later rows:4E5740/5742 requires nonnegative row<side, then jumps directly to4E557E XOR EBX;4E5580/5582 repeats side<=0. For valid nonaliasing object/stack/global regions there is no intervening external call or side-field store on either edge.'},
 '0x5461bf':{'excluded_outcome':'taken','reason':'First row:54619B..5461A5 reads[ESI]=side and skips the piece when side<=0;5461AB..5461BF has no calls or stores throughESI. Later rows:546390/546392 requires nonnegative row<side, then jumps directly to5461B7 XOR EBX;5461B9/5461BF repeats side<=0. For valid nonaliasing object/stack/global regions there is no intervening external call or side-field store on either edge.'}}
report['qualified_unreachable_conditional_outcomes']=qualified
if report.get('complete_case_set'):
 missing={(a,outcome) for a,v in branch_coverage.items() for outcome in ['taken','fallthrough'] if not v[outcome]};allowed={(a,v['excluded_outcome']) for a,v in qualified.items()};unqualified=missing-allowed;report['unqualified_missing_conditional_outcomes']=sorted(unqualified)
 if unqualified:report.update(passed=False,error='Unqualified native conditional outcomes '+str(sorted(unqualified)))
report['native_function_instruction_coverage']={entry:{'executed':len(addresses&executed),'total':len(addresses),'unreached':[hex(a) for a in sorted(addresses-executed)]} for entry,addresses in function_addresses.items()}
sources=[str(phase/'include/placement_preview.h').replace('\\','/')]+[str(phase/f'src/unit_{n:05d}.c').replace('\\','/') for n in [565,2115,2527]]+[str(phase/f'runtime/placement_preview_{n}.c').replace('\\','/') for n in ['members','predicates','platform']]+['tests/placement_preview.c','tests/placement_preview_helpers.c','tools/verify_placement_preview_emulation.py','include/sprite_libraries.h','include/sprite_render.h','include/graphics_53f250.h']
sources+=['include/preview_footprint_seeds.h','runtime/preview_footprint_seeds.c','tools/extract_preview_footprint_seeds.py','include/startup_geometry.h']+[f'src/unit_{n:05d}.c' for n in [2,99,195,1993,2563,2617,2604,2591,1965,1968]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports+[seed_provenance,seed_native,'metadata/placement-preview-objects-native.json']};name='placement-preview'+('' if args.production else '-staged')+('-debug' if args.debug_limit else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
