"""Translate a bounded x86 function directly into C and one header; no reports or dumps."""
from pathlib import Path
import argparse,hashlib,re,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_REG,X86_OP_IMM,X86_OP_MEM
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('binary',type=Path)
ap.add_argument('--start',type=lambda s:int(s,0),required=True)
ap.add_argument('--end',type=lambda s:int(s,0),required=True,help='Inclusive final instruction byte address')
ap.add_argument('--output',type=Path,required=True)
ap.add_argument('--name')
ap.add_argument('--frame-name',default='restore_instruction_frame')
ap.add_argument('--expected-sha256')
ap.add_argument('--jump-table',action='append',default=[],metavar='SITE:COUNT',help='Explicit reviewed indirect JMP table entry count')
ap.add_argument('--rodata',action='append',default=[],metavar='ADDRESS:SIZE',help='Embed immutable original code-section data as C constants')
ap.add_argument('--tail-target',action='append',default=[],type=lambda s:int(s,0),metavar='ADDRESS',help='Reviewed external immediate JMP target; transfer the existing entry stack without pushing a return slot')
ap.add_argument('--indirect-call-site',action='append',default=[],type=lambda s:int(s,0),metavar='ADDRESS',help='Reviewed register/member CALL site; the boundary must bind its real targets')
ap.add_argument('--menu-capacity-fix',action='store_true',help='Compose the verified eight Pharaoh menu route-capacity hooks')
options=ap.parse_args()
if options.menu_capacity_fix:assert options.start==0x4d8310 and options.end==0x4dd8ca and options.frame_name=='restore_menu_input_frame'
if options.name is None:options.name=f'restore_C_{options.start:08x}'
assert all(re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*',s) for s in [options.name,options.frame_name])
raw=options.binary.read_bytes();digest=hashlib.sha256(raw).hexdigest()
if options.expected_sha256:assert digest==options.expected_sha256,'Unexpected source executable'
u16=lambda p:struct.unpack_from('<H',raw,p)[0]
u32=lambda p:struct.unpack_from('<I',raw,p)[0]
pe=u32(60);assert raw[pe:pe+4]==b'PE\0\0' and u16(pe+4)==0x14c
opt=pe+24;assert u16(opt)==0x10b
base=u32(opt+28);table=opt+u16(pe+20)
iat_start=base+u32(opt+96+12*8);iat_end=iat_start+u32(opt+96+12*8+4)
sections=[(base+u32(table+j*40+12),u32(table+j*40+16),u32(table+j*40+20),u32(table+j*40+36)) for j in range(u16(pe+6))]
def image_bytes(address,size,readonly=False):
 assert size>0
 for va,n,offset,flags in sections:
  if va<=address and address+size<=va+n:
   if readonly:assert not flags&0x80000000,'Cannot embed writable original memory'
   return raw[offset+address-va:offset+address-va+size]
 raise AssertionError(('Region is not file-backed',hex(address),size))
def pair(text):
 left,right=text.split(':');return int(left,0),int(right,0)
tables={}
for argument in options.jump_table:
 site,count=pair(argument);assert site not in tables and 0<count<=4096;tables[site]=count
rom={}
def add_rom(address,size):
 blob=image_bytes(address,size,True)
 for old,data in rom.items():assert address+size<=old or old+len(data)<=address,'Overlapping immutable regions'
 rom[address]=blob
for argument in options.rodata:add_rom(*pair(argument))
assert options.start<=options.end
block=None
for address,size,offset,flags in sections:
 if address<=options.start and options.end<address+size:
  assert flags&0x20000000,'Selected interval is not executable'
  begin=offset+options.start-address;block=raw[begin:begin+options.end-options.start+1];break
assert block is not None,'Selected interval is not file-backed'
decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
instructions=list(decoder.disasm(block,options.start))
assert instructions and b''.join(bytes(i.bytes) for i in instructions)==block,'Instruction decoding is incomplete'
addresses={i.address for i in instructions};labels=set();indirect={}
tail_targets=set(options.tail_target);used_tail_targets=set()
indirect_calls=set(options.indirect_call_site);used_indirect_calls=set()
for i in instructions:
 if i.mnemonic.startswith('j'):
  operand=i.operands[0]
  if operand.type==X86_OP_IMM:
   if operand.imm in addresses:labels.add(operand.imm)
   else:
    assert i.mnemonic=='jmp' and operand.imm in tail_targets,('External jump needs an explicit reviewed tail binding',hex(i.address))
    assert any(va<=operand.imm<va+n and flags&0x20000000 for va,n,offset,flags in sections),'Tail target is not executable'
    used_tail_targets.add(operand.imm)
  else:
   assert i.mnemonic=='jmp' and i.address in tables,('Indirect jump needs a reviewed table',hex(i.address))
   mem=operand.mem
   assert operand.type==X86_OP_MEM and operand.size==4 and not mem.segment and not mem.base and mem.index and mem.scale==4
   count=tables[i.address];data=image_bytes(mem.disp,count*4,True)
   targets=struct.unpack('<'+'I'*count,data)
   assert all(a in addresses for a in targets),'Jump target outside the selected instruction body'
   indirect[i.address]=targets;labels.update(targets);add_rom(mem.disp,count*4)
assert set(tables)==set(indirect),'Unused or incorrect jump-table site'
assert tail_targets==used_tail_targets,'Unused or incorrect tail target'
rows=[None]*len(instructions)
header=options.output.with_suffix('.h')

INTEGER_HELPERS="#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))\n#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))\n#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))\nenum {MI_CF=1,MI_PF=4,MI_AF=16,MI_ZF=64,MI_SF=128,MI_OF=2048};\nstatic __inline uint32_t mi_mask(uint32_t width){return width==32?UINT32_MAX:((1u<<width)-1u);}\nstatic __inline uint32_t mi_szp(uint32_t value,uint32_t width)\n{\n uint32_t parity=value&255u;parity^=parity>>4;parity&=15;\n return (value==0?MI_ZF:0)|((value>>(width-1u))&1u?MI_SF:0)|\n        (((0x6996u>>parity)&1u)?0:MI_PF);\n}\nstatic __inline uint32_t mi_alu(struct restore_menu_input_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)\n{\n uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,flags,carry;\n uint64_t wide;a&=mask;b&=mask;carry=operation==2?(f->flags&MI_CF):0;\n if(operation==0){wide=(uint64_t)a+b;result=(uint32_t)wide&mask;\n  flags=(wide>mask?MI_CF:0)|((~(a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);\n }else if(operation<=2){wide=(uint64_t)b+carry;result=(a-b-carry)&mask;\n  flags=((uint64_t)a<wide?MI_CF:0)|(((a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);\n }else{result=(operation==3?(a&b):operation==4?(a|b):(a^b))&mask;flags=f->flags&MI_AF;}\n f->flags=(f->flags&~(MI_CF|MI_PF|MI_AF|MI_ZF|MI_SF|MI_OF))|flags|mi_szp(result,width);\n return result;\n}\nstatic __inline uint32_t mi_shift(struct restore_menu_input_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)\n{\n uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,carry,flags;\n count&=31u;a&=mask;if(!count)return a;\n if(arithmetic){carry=count>=width?(a&sign)!=0:((a>>(count-1u))&1u);\n  result=count>=width?((a&sign)?mask:0):((a>>count)|((a&sign)?(mask^(mask>>count)):0));\n }else{carry=count>width?0:((a>>(width-count))&1u);result=(uint32_t)((uint64_t)a<<count)&mask;}\n flags=carry|mi_szp(result,width);\n if(count==1){if(!arithmetic&&(((result&sign)!=0)^carry))flags|=MI_OF;}\n else flags|=f->flags&MI_OF;\n f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|flags;return result;\n}\nstatic __inline uint32_t mi_imul(struct restore_menu_input_frame *f,uint32_t a,uint32_t b)\n{\n int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;\n f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;\n return result;\n}\nstatic __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}"

regs = {'eax': 0, 'ecx': 1, 'edx': 2, 'ebx': 3, 'esp': 4, 'ebp': 5, 'esi': 6, 'edi': 7}
aliases = {}
for name, index in regs.items():
    aliases[name] = (index, 32, 0)
    aliases[name[1:]] = (index, 16, 0)
for index, name in enumerate(['a', 'c', 'd', 'b']):
    aliases[name+'l'] = (index, 8, 0); aliases[name+'h'] = (index, 8, 8)

def effective(operand):
    m = operand.mem
    assert not m.segment
    parts = [f'0x{m.disp & 0xffffffff:08x}u']
    if m.base:
        parts.append(f'f->r[{regs[decoder.reg_name(m.base)]}]')
    if m.index:
        parts.append(f'f->r[{regs[decoder.reg_name(m.index)]}]*{m.scale}u')
    return '('+'+'.join(parts)+')'

def value(operand):
    if operand.type == X86_OP_IMM:
        return f'0x{operand.imm & 0xffffffff:08x}u'
    if operand.type == X86_OP_REG:
        index, width, shift = aliases[decoder.reg_name(operand.reg)]
        v = f'f->r[{index}]'
        if shift: v = '('+v+f'>>{shift})'
        return v if width == 32 else '('+v+f'&0x{(1<<width)-1:x}u)'
    assert operand.type == X86_OP_MEM and operand.size in [1, 2, 4]
    return f'mi_read({effective(operand)},{operand.size}u)' if rom else f'MI_U{operand.size*8}({effective(operand)})'

def store(operand, expression):
    assert operand.type == X86_OP_REG
    index, width, shift = aliases[decoder.reg_name(operand.reg)]
    if width == 32: return f'f->r[{index}]={expression};'
    mask = ((1 << width)-1) << shift
    return f'f->r[{index}]=(f->r[{index}]&0x{(~mask)&0xffffffff:08x}u)|((({expression})&0x{(1<<width)-1:x}u)<<{shift});'

# Direct C loads/stores deliberately keep the native access width. No inferred
# Ghidra global type, array stride or floating C expression is transplanted.
def put(operand, expression):
    if operand.type == X86_OP_MEM:
        kind = {1:'uint8_t', 2:'uint16_t', 4:'uint32_t'}[operand.size]
        return f'MI_U{operand.size*8}({effective(operand)})=({kind})({expression});'
    return store(operand, expression)

conditions = {'jne':'!(f->flags&MI_ZF)', 'je':'(f->flags&MI_ZF)',
              'jl':'mi_signed_less(f->flags)', 'jge':'!mi_signed_less(f->flags)',
              'jg':'!(f->flags&MI_ZF)&&!mi_signed_less(f->flags)',
              'jle':'(f->flags&MI_ZF)||mi_signed_less(f->flags)',
              'jbe':'(f->flags&(MI_CF|MI_ZF))', 'jae':'!(f->flags&MI_CF)',
              'ja':'!(f->flags&(MI_CF|MI_ZF))','jb':'(f->flags&MI_CF)',
              'js':'(f->flags&MI_SF)','jns':'!(f->flags&MI_SF)'}
lines=['/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */', '#include "'+header.name+'"', '#include <stdlib.h>', '', INTEGER_HELPERS.replace('restore_menu_input_frame',options.frame_name), '', 'void '+options.name+'(struct '+options.frame_name+' *f)', '{', '    uint32_t a,b,t,p; uint16_t sw;', '    (void)a;(void)b;(void)t;(void)p;(void)sw;']
calls = []
if rom:
 lines.insert(2,'#include <stdlib.h>')
 for address,data in sorted(rom.items()):
  lines.append(f'static const uint8_t mi_rom_{address:08x}[]={{'+','.join(f'0x{x:02x}' for x in data)+'};')
 # Immutable ROM is part of the C source, not an executable dependency.
 rom_lines=['static __inline uint32_t mi_read(uint32_t address,uint32_t width)','{',' uint32_t offset,k,result;']
 for address,data in sorted(rom.items()):
  rom_lines += [f' if(address>=0x{address:08x}u&&width<={len(data)}u&&(offset=address-0x{address:08x}u)<={len(data)}u-width){{',
                f'  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_{address:08x}[offset+k]<<(k*8u);return result;',' }']
 rom_lines += [' if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);','}']
 # Helpers must precede the translated function.
 insert=next(k for k,line in enumerate(lines) if line.startswith('void '+options.name+'('))
 arrays=lines[insert+4:];del lines[insert+4:]
 lines[insert:insert]=arrays+rom_lines+['']
if any(i.mnemonic=='idiv' for i in instructions):
 if not rom:lines.insert(2,'#include <stdlib.h>')
 helper='''static __inline void mi_idiv32(struct FRAME *f,uint32_t divisor_bits)
{
 int32_t divisor=(int32_t)divisor_bits;
 int64_t dividend=(int64_t)(int32_t)f->r[2]*INT64_C(4294967296)+(int64_t)f->r[0];
 int64_t quotient,remainder;
 /* Native #DE cases explicitly fail; no C division overflow or fake result. */
 if(!divisor||(dividend==INT64_MIN&&divisor==-1))abort();
 quotient=dividend/(int64_t)divisor;
 if(quotient<INT32_MIN||quotient>INT32_MAX)abort();
 remainder=dividend%(int64_t)divisor;
 f->r[0]=(uint32_t)(int32_t)quotient;f->r[2]=(uint32_t)(int32_t)remainder;
 /* IDIV leaves arithmetic flags undefined. Keep their previous frame bits. */
}'''.replace('FRAME',options.frame_name)
 insert=next(k for k,line in enumerate(lines) if line.startswith('void '+options.name+'('))
 lines[insert:insert]=[helper,'']
if options.menu_capacity_fix:lines.insert(2,'#include "menu_inline_capacity.h"')
for i, row in zip(instructions, rows):
    if i.address in labels:
        lines.append(f'mi_{i.address:08x}:;')
    lines += [f'    /* {i.address:08x} */']
    if options.menu_capacity_fix and i.address in [0x4da05a,0x4da424,0x4da8d3,0x4dacb8,0x4db446,0x4db6e6,0x4db98b,0x4dbc55]:
        lines.append(f'    restore_menu_inline_capacity_before(0x{i.address:08x}u,f);')
    op = i.mnemonic; args = i.operands
    if op == 'nop':
        assert bytes(i.bytes)==b'\x90' and not args, 'Only reviewed one-byte NOP is supported'
        code = '; /* Native NOP preserves registers, memory and flags. */'
    elif op == 'mov': code = put(args[0], value(args[1]))
    elif op in ['movsx','movzx']:
        code = put(args[0], value(args[1]) if op=='movzx' else f'(uint32_t)(int32_t)(int{args[1].size*8}_t){value(args[1])}')
    elif op == 'lea': code = put(args[0], effective(args[1]))
    elif op == 'not': code = put(args[0], f'(~({value(args[0])}))')
    elif op == 'push': code = f'a={value(args[0])};f->r[4]-=4;MI_U32(f->r[4])=a;'
    elif op == 'pop': code = 'a=MI_U32(f->r[4]);f->r[4]+=4;'+put(args[0], 'a')
    elif op in ['add','sub','cmp','sbb','and','or','xor','test']:
        width = args[0].size*8
        operation = {'add':0,'sub':1,'cmp':1,'sbb':2,'and':3,'test':3,'or':4,'xor':5}[op]
        code = f'a={value(args[0])};b={value(args[1])};t=mi_alu(f,a,b,{width},{operation});'
        if op not in ['cmp','test']: code += put(args[0], 't')
    elif op in ['inc','dec','neg']:
        code = f'a={value(args[0])};b=f->flags&MI_CF;'
        code += f't=mi_alu(f,{"0,a" if op=="neg" else "a,1"},{args[0].size*8},{0 if op=="inc" else 1});'
        if op != 'neg': code += 'f->flags=(f->flags&~MI_CF)|b;'
        code += put(args[0], 't')
    elif op in ['shl','sar']:
        code = f'a={value(args[0])};b={value(args[1])};t=mi_shift(f,a,b,{args[0].size*8},{1 if op=="sar" else 0});'+put(args[0], 't')
    elif op=='shr':
        assert args[0].size==4
        if args[1].type==X86_OP_REG:
            assert i.reg_name(args[1].reg)=='cl', 'Only the native CL shift-count register is supported'
            code=f'a={value(args[0])};b=({value(args[1])})&31u;if(b){{t=a>>b;'
            code+='f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>(b-1u))&1u)|mi_szp(t,32);'
            code+='if(b==1u)f->flags=(f->flags&~MI_OF)|((a&0x80000000u)?MI_OF:0);'
            code+=put(args[0],'t')+'}'
            code+=' /* AF and OF for count>1 are architecturally unspecified; retained bits are not a status-equivalence claim. */'
        else:
            assert args[1].type==X86_OP_IMM
            count=args[1].imm&31
            if not count:code='/* Masked shift count zero preserves the operand and flags. */'
            else:
                code=f'a={value(args[0])};t=a>>{count}u;'
                code+=f'f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF'+('|MI_OF' if count==1 else '')+f'))|((a>>{count-1}u)&1u)|mi_szp(t,32)'+('|((a&0x80000000u)?MI_OF:0)' if count==1 else '')+';'
                code+=put(args[0],'t')
    elif op == 'imul':
        assert len(args) in [1,2,3] and args[0].size in [2,4]
        if args[0].size==2:
            # Reviewed494ABF/494AD1 and468F47/468F84: preserve the
            # two/three-operand WORD result and its signed overflow flags.
            assert len(args) in [2,3]
            lhs,rhs=(value(args[0]),value(args[1])) if len(args)==2 else (value(args[1]),value(args[2]))
            code=f'a={lhs};b={rhs};'
            code+='{int32_t wide=(int32_t)(int16_t)a*(int16_t)b;uint16_t low=(uint16_t)wide;'
            code+='f->flags&=~(MI_CF|MI_OF);if(wide!=(int32_t)(int16_t)low)f->flags|=MI_CF|MI_OF;t=low;}'
            code+=put(args[0],'t')
        elif len(args)==1:
            code=f'a=f->r[0];b={value(args[0])};'
            code+='{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;'
            code+='f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;'
            code+='f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}'
        else:
            lhs, rhs = (value(args[0]),value(args[1])) if len(args)==2 else (value(args[1]),value(args[2]))
            code = f'a={lhs};b={rhs};t=mi_imul(f,a,b);'+put(args[0],'t')
    elif op == 'cdq': code = 'f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;'
    elif op == 'idiv':
        assert len(args)==1 and args[0].size==4,'Only reviewed 32-bit IDIV is supported'
        code=f'a={value(args[0])};mi_idiv32(f,a);'
    elif op == 'div':
        assert len(args)==1 and args[0].size==4,'Only reviewed 32-bit DIV is supported'
        code=f'a={value(args[0])};'
        code+='{uint64_t dividend=((uint64_t)f->r[2]<<32)|f->r[0],quotient;'
        code+='if(!a)abort();quotient=dividend/a;if(quotient>UINT32_MAX)abort();'
        code+='f->r[2]=(uint32_t)(dividend%a);f->r[0]=(uint32_t)quotient;}'
        code+=' /* Native #DE fails explicitly; arithmetic flags after DIV are undefined. */'
    elif op.startswith('set') and 'j'+op[3:] in conditions:
        assert len(args)==1 and args[0].size==1,'SETcc requires a byte destination'
        code='t=('+conditions['j'+op[3:]]+')?1u:0u;'+put(args[0],'t')
    elif op == 'jmp':
        if args[0].type==X86_OP_IMM and args[0].imm in tail_targets:
            target=args[0].imm
            code=f'f->boundary(0x{i.address:08x}u,0x{target:08x}u,f);return;'
            calls.append({'site':f'{i.address:08x}','target':f'{target:08x}','kind':'reviewed tail JMP boundary'})
        elif i.address not in indirect:code=f'goto mi_{args[0].imm:08x};'
        else:
            code='switch('+value(args[0])+'){'+''.join(f'case 0x{a:08x}u:goto mi_{a:08x};' for a in sorted(set(indirect[i.address])))+'default:abort();}'
    elif op in conditions: code = f'if({conditions[op]})goto mi_{args[0].imm:08x};'
    elif op == 'leave':
        assert not args,'Only reviewed 32-bit LEAVE is supported'
        code='f->r[4]=f->r[5];a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;'
    elif op == 'ret':
        cleanup=args[0].imm if args else 0
        code = f'f->r[4]+={4+cleanup};return;'
    elif op == 'call':
        if i.address in indirect_calls:
            assert args[0].type in [X86_OP_REG,X86_OP_MEM] and args[0].size==4
            used_indirect_calls.add(i.address)
            code=f'p={value(args[0])};f->r[4]-=4;MI_U32(f->r[4])=0x{i.address+i.size:08x}u;f->boundary(0x{i.address:08x}u,p,f);'
            calls.append({'site':f'{i.address:08x}','target':'reviewed dynamic target','kind':'explicit indirect boundary'})
        else:
            target = args[0].imm if args[0].type == X86_OP_IMM else args[0].mem.disp
            assert args[0].type == X86_OP_IMM or (args[0].type == X86_OP_MEM and not args[0].mem.base and not args[0].mem.index and iat_start<=args[0].mem.disp<iat_end)
            code = f'f->r[4]-=4;MI_U32(f->r[4])=0x{i.address+i.size:08x}u;f->boundary(0x{i.address:08x}u,0x{target:08x}u,f);'
            calls.append({'site':f'{i.address:08x}','target':f'{target:08x}','kind':'direct' if args[0].type==X86_OP_IMM else 'IAT slot boundary'})
    elif op == 'fnstsw':
        assert i.op_str=='ax'
        code = '__asm { fnstsw sw }\n    '+put(args[0],'sw')
    elif op=='rep movsb':
        assert bytes(i.bytes)==b'\xf3\xa4'
        code='while(f->r[1]){a=MI_U8(f->r[6]);MI_U8(f->r[7])=(uint8_t)a;f->r[6]+=(f->flags&0x400u)?0xffffffffu:1u;f->r[7]+=(f->flags&0x400u)?0xffffffffu:1u;--f->r[1];}'
    elif op=='rep movsd':
        assert bytes(i.bytes)==b'\xf3\xa5'
        code='while(f->r[1]){a=MI_U32(f->r[6]);MI_U32(f->r[7])=a;f->r[6]+=(f->flags&0x400u)?0xfffffffcu:4u;f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}'
    elif op=='stosw':
        assert bytes(i.bytes)==b'\x66\xab'
        code='MI_U16(f->r[7])=(uint16_t)f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffeu:2u;'
    elif op=='repne scasb':
        assert bytes(i.bytes)==b'\xf2\xae'
        code='while(f->r[1]){a=f->r[0]&255u;b=MI_U8(f->r[7]);t=mi_alu(f,a,b,8,1);f->r[7]+=(f->flags&0x400u)?0xffffffffu:1u;--f->r[1];if(f->flags&MI_ZF)break;}'
    elif op=='rep stosd':
        assert bytes(i.bytes)==b'\xf3\xab'
        code='while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}'
    elif op.startswith('f'):
        if any(operand.type == X86_OP_MEM for operand in args):
            assert len(args)==1 and args[0].type==X86_OP_MEM
            assert args[0].size==4 or (args[0].size==8 and op in ['fld','fadd','fdivr','fcom']), 'Only reviewed DWORD operations and QWORD FLD/FADD/FDIVR/FCOM are supported'
            stack_operand=(args[0].mem.base and decoder.reg_name(args[0].mem.base)=='esp'
                           and not args[0].mem.index and not args[0].mem.segment)
            # A reviewed ESP-relative scratch operand cannot alias immutable
            # code-section tables. Other floating ROM reads still need an
            # explicit lowering rather than dereferencing an original token.
            assert not rom or op=='fstp' or stack_operand,'Floating ROM reads require a separate reviewed lowering'
            code = f'p={effective(args[0])};\n    __asm {{ mov eax,p }}\n    __asm {{ {op} {"dword" if args[0].size==4 else "qword"} ptr [eax] }}'
        else:
            operands = re.sub(r'st\((\d)\)',r'st(\1)',i.op_str)
            if op in ['faddp','fmulp']: operands=(operands or 'st(1)')+',st(0)'
            if op in ['fmul','fadd','fdiv']: operands='st(0),'+operands
            assert op in ['fld','fmul','fadd','fdiv','fstp','faddp','fmulp','fsqrt']
            code = '__asm { '+op+(' '+operands if operands else '')+' }'
    else: raise AssertionError((hex(i.address),op,i.op_str))
    lines.append('    '+code)
assert indirect_calls==used_indirect_calls,'Unused or incorrect reviewed indirect CALL site'
lines.append('}')

guard='RESTORE_'+options.name.upper()+'_H'
frame_guard='RESTORE_'+options.frame_name.upper()+'_DEFINED'
header_text=f"""#ifndef {guard}
#define {guard}
#include <stdint.h>
#ifndef {frame_guard}
#define {frame_guard}
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct {options.frame_name};
typedef void (*{options.frame_name}_call)(uint32_t,uint32_t,struct {options.frame_name} *);
struct {options.frame_name} {{ uint32_t r[8],flags; {options.frame_name}_call boundary; }};
#endif
void {options.name}(struct {options.frame_name} *frame);
#endif
"""
options.output.parent.mkdir(parents=True,exist_ok=True)
for path,content in [(header,header_text),(options.output,'\n'.join(lines)+'\n')]:
 data=content.encode()
 if not path.exists() or path.read_bytes()!=data:path.write_bytes(data)
print(f'{len(instructions)} instructions -> {options.output} + {header.name}; {len(calls)} explicit boundary sites. No auxiliary data files.')
