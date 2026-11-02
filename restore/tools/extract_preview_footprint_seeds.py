"""Recover the eight preview footprint seed arrays from pinned native evidence.

Two independent readings agree: Capstone register/push constant propagation,
and bounded Unicorn constructor prefixes executing the real record helpers.
Only four seed DWORDs per record are recovered. C++ base/vector construction,
destructors, exception machinery and post-seed constructor tails are excluded.
"""
from pathlib import Path
import argparse,bisect,hashlib,json,struct,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir())
PHASE=ROOT/'artifacts/preview_footprints_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
import capstone
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK
from unicorn.x86_const import *
SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
SETS=[('class_0040d280',0x40d280,0x6fd5d8,0x4c,25,0x40d7e0),('class_00401030',0x401030,0x6de650,0x4c,16,0x4013f0),('class_00416c20',0x416c20,0x6fd368,0x4c,9,0x416f60),('class_004e6400',0x4e6400,0x6de548,0x4c,2,0x4e6590),('class_005430e0',0x5430e0,0x6f9320,0x1e5c,167,0x5456a0),('class_005567d0',0x5567d0,0x6f5648,0x1cbc,158,0x558b40),('class_0054fef0',0x54fef0,0x6de9e0,0x4ee0,437,0x555de0),('class_005480e0',0x5480e0,0x6e9188,0x5c88,513,0x54f3d0)]
MASK=0xffffffff
S=lambda v:(v&MASK)-(1<<32) if v&0x80000000 else v&MASK
# Values are affine expressions coefficient*self+constant, modulo32bits.
def add(a,b):return None if a is None or b is None else ((a[0]+b[0])&MASK,(a[1]+b[1])&MASK)
def neg(a):return None if a is None else ((-a[0])&MASK,(-a[1])&MASK)

def static_recover(image,functions):
 md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;result=[]
 for name,entry,base,offset,count,geometry in SETS:
  regs={'ecx':(1,0)};pushed=[];pending=None;records=[];prefix=[]
  for item in functions[f'{entry:08x}']['assembly']['items']:
   address=int(item['address'],16);insn=next(md.disasm(bytes.fromhex(item['bytes']),address));ops=insn.operands;prefix.append(address)
   def value(o):
    if o.type==capstone.x86.X86_OP_IMM:return (0,o.imm&MASK)
    if o.type==capstone.x86.X86_OP_REG:return regs.get(insn.reg_name(o.reg))
    return None
   if insn.mnemonic=='push':pushed.append((address,value(ops[0])))
   elif insn.mnemonic=='call':
    target=ops[0].imm
    if target==0x4e5120:
     assert len(pushed)==4,(name,hex(address),pushed)
     args=[v for a,v in reversed(pushed)];assert all(v is not None and v[0]==0 for v in args),(name,hex(address),pushed)
     pending={'construct_call':hex(address),'push_sites':[hex(a) for a,v in reversed(pushed)],'tuple':[S(v[1]) for v in args]}
    elif target==0x4e5140:
     dest=regs.get('ecx');assert pending and dest is not None and dest[0]==1,(name,hex(address),dest)
     index=(dest[1]-offset)//52;assert dest[1]==offset+len(records)*52 and index==len(records),(name,hex(address),dest,len(records))
     pending.update(copy_call=hex(address),index=index,destination_offset=dest[1]);records.append(pending);pending=None
     if len(records)==count:break
    regs.update(eax=None,ecx=None,edx=None);pushed=[]
   elif ops and ops[0].type==capstone.x86.X86_OP_REG:
    target=insn.reg_name(ops[0].reg)
    if target not in ['eax','ebx','ecx','edx','esi','edi','ebp','esp']:
     # Partial register writes do not supply tuple arguments in these prefixes.
     if target in ['al','ah','ax','bl','bh','bx','cl','ch','cx','dl','dh','dx']:regs['e'+target[0]+'x']=None
     continue
    if insn.mnemonic=='mov':regs[target]=value(ops[1])
    elif insn.mnemonic=='lea':
     m=ops[1].mem;v=(0,m.disp&MASK)
     if m.base:v=add(v,regs.get(insn.reg_name(m.base)))
     if m.index:
      z=regs.get(insn.reg_name(m.index));v=add(v,None if z is None else ((z[0]*m.scale)&MASK,(z[1]*m.scale)&MASK))
     regs[target]=v
    elif insn.mnemonic=='xor' and ops[1].type==capstone.x86.X86_OP_REG and ops[0].reg==ops[1].reg:regs[target]=(0,0)
    elif insn.mnemonic in ['add','sub']:regs[target]=add(regs.get(target),value(ops[1]) if insn.mnemonic=='add' else neg(value(ops[1])))
    elif insn.mnemonic in ['inc','dec']:regs[target]=add(regs.get(target),(0,1 if insn.mnemonic=='inc' else MASK))
    elif insn.mnemonic=='or' and ops[1].type==capstone.x86.X86_OP_IMM and ops[1].imm&MASK==MASK:regs[target]=(0,MASK)
    elif insn.mnemonic not in ['cmp','test']:regs[target]=None
  assert len(records)==count,(name,len(records),count)
  result.append({'name':name,'constructor':hex(entry),'object':hex(base),'offset':offset,'count':count,'geometry':hex(geometry),'records':records,'prefix_instructions':prefix})
 return result

def make_vm(image):
 u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
 pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
 vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
 for k in range(u16(pe+6)):
  q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
 return vm

def native_recover(image,functions,static,fill=0xa5):
 vm=make_vm(image);vm.mem_map(0,4096);vm.mem_map(0x34000000,0x100000);vm.mem_map(0x33000000,4096)
 read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
 def write(a,v):vm.mem_write(a,(v&MASK).to_bytes(4,'little'))
 allowed={int(i['address'],16) for f in functions.values() for i in f['assembly']['items']};allowed.add(0x51f780)
 for f in functions.values():
  for i in f['assembly']['items']:
   b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
 assert bytes(vm.mem_read(0x51f780,1))==b'\xc3'
 state={};hits=set();answer=[]
 def hook(cpu,a,n,user):
  if a==state.get('stop'):cpu.emu_stop();return
  if a in [0x4e5210,0x545f80,0x561441]:
   sp=cpu.reg_read(UC_X86_REG_ESP);cleanup=20 if a==0x561441 else 0
   state['controlled'].add(a);cpu.reg_write(UC_X86_REG_EAX,cpu.reg_read(UC_X86_REG_ECX));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,read(sp));return
  assert a in allowed,'Unexpected native instruction '+hex(a)
  hits.add(a)
  if a==0x4e5120:
   sp=cpu.reg_read(UC_X86_REG_ESP);state['pending']=[S(read(sp+i*4)) for i in range(1,5)];state['temp']=cpu.reg_read(UC_X86_REG_ECX)
  if a==0x4e5140:
   sp=cpu.reg_read(UC_X86_REG_ESP);src=read(sp+4);dest=cpu.reg_read(UC_X86_REG_ECX);index=len(state['records'])
   assert src==state['temp'];assert [S(read(src+i*4)) for i in range(4)]==state['pending']
   assert dest==state['base']+state['offset']+index*52,(hex(dest),index)
   state['records'].append({'tuple':state['pending'],'source':src,'destination':dest,'return':read(sp)})
  if a==0x4e5189:
   e=state['records'][-1];assert [S(read(e['destination']+i*4)) for i in range(4)]==e['tuple']
   if len(state['records'])==state['count']:state['stop']=e['return']
 vm.hook_add(UC_HOOK_CODE,hook)
 for spec in static:
  entry=int(spec['constructor'],16);base=int(spec['object'],16);count=spec['count'];offset=spec['offset']
  vm.mem_write(base,bytes([fill])*(offset+count*52+32));vm.mem_write(0x34000000,bytes([fill])*0x100000)
  state.clear();state.update(base=base,offset=offset,count=count,records=[],controlled=set())
  sp=0x340f0000;write(sp,0x33000000);write(0,0x11223344)
  for reg,value in [(UC_X86_REG_ECX,base),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2),(UC_X86_REG_EBX,0x12345678),(UC_X86_REG_EBP,0x23456789),(UC_X86_REG_ESI,0x34567890),(UC_X86_REG_EDI,0x456789ab)]:vm.reg_write(reg,value)
  vm.emu_start(entry,0x33000000,count=1000000)
  assert vm.reg_read(UC_X86_REG_EIP)==state['stop'] and len(state['records'])==count
  rows=[[S(read(base+offset+i*52+j*4)) for j in range(4)] for i in range(count)]
  assert rows==[r['tuple'] for r in spec['records']],spec['name']
  answer.append({'name':spec['name'],'tuples':rows,'controlled_prefix_calls':[hex(a) for a in sorted(state['controlled'])],'native_copy_count':len(state['records'])})
 return answer,hits

def recover(native=None):
 image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert hashlib.sha256(image).hexdigest()==SHA
 path=native or PHASE/'metadata/native.json';d=json.loads(path.read_text());assert d['native_sha256']==SHA
 for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
 static=static_recover(image,d['functions']);native_rows,hits=native_recover(image,d['functions'],static)
 assert sum(s['count'] for s in static)==1327
 return image,d,static,native_rows,hits

def runtime_source(sets):
 lines=['/* Constant seed tuples recovered from pinned native constructors. */','#include "preview_footprint_seeds.h"','typedef struct {int32_t x,y,size,type;} PreviewSeed;']
 for spec in sets:
  lines.append('static const PreviewSeed '+spec['name']+'['+str(spec['count'])+']={')
  lines += ['    {'+','.join(map(str,r['tuple']))+'},' for r in spec['records']]
  lines.append('};')
 lines += ['typedef struct {uintptr_t target;uint32_t count;const PreviewSeed *seeds;} PreviewSeedSet;','static const PreviewSeedSet sets[8]={']
 for spec in sets:lines.append('    {0x%08xu,%du,%s},'%(int(spec['object'],16)+spec['offset'],spec['count'],spec['name']))
 lines += ['};','void restore_preview_footprint_seeds_initialize(void)','{','    uint32_t set,index;','    for(set=0;set<8;++set)for(index=0;index<sets[set].count;++index){','        volatile uint32_t *record=(volatile uint32_t *)(sets[set].target+(uintptr_t)index*52u);','        const PreviewSeed *seed=&sets[set].seeds[index];','        record[0]=(uint32_t)seed->x;record[1]=(uint32_t)seed->y;','        record[2]=(uint32_t)seed->size;record[3]=(uint32_t)seed->type;','    }','}','']
 return '\n'.join(lines)

if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,default=PHASE/'metadata/provenance.json');parser.add_argument('--write-c',type=Path,help='Regenerate only the constant C tables and seed initializer');parser.add_argument('--native',type=Path,help='Explicit complete native constructor dossier');args=parser.parse_args()
 image,d,static,native,hits=recover(args.native)
 report={'native_sha256':SHA,'record_count':1327,'sets':static,'native_prefix_confirmation':native,'native_instructions_executed':len(hits),'method':'Independent Capstone affine register/push propagation and Unicorn constructor prefixes executing real4e5120/4e5140. Both tuple values and every destination/count agree.','limitations':['Only seed fields0..3 are recovered. Base/vector constructors and empty destructors are controlled; execution stops immediately after the final real copy into each target array. SEH epilogues, RNG/post-seed tails and unrelated object fields are outside this proof.']}
 args.output.write_text(json.dumps(report,indent=2)+'\n')
 if args.write_c:args.write_c.write_text(runtime_source(static))
 print(json.dumps({'records':1327,'sets':[(s['name'],s['count']) for s in static],'native_prefix_instructions':len(hits)},indent=2))
