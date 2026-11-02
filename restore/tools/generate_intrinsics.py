"""Generate integer p-code helpers with explicit widths and one evaluation."""
import re


def generate_intrinsics(source):
    helpers = sorted(set(re.findall(r'\b(?:CONCAT\d\d|SUB\d\d|SBORROW[1248]|CARRY[1248])\b',source)))
    output = ['/* Generated integer helpers; input and output widths are bytes. */']
    for helper in helpers:
        if helper.startswith('CONCAT'):
            upper, lower = map(int,helper[6:])
            if upper+lower>8:
                raise ValueError(f'Unsupported helper {helper}')
            dtype = 'unsigned int' if upper+lower<=4 else 'unsigned __int64'
            mask = (1<<(lower*8))-1
            upper_mask = (1<<(upper*8))-1
            output.append(f'static __inline {dtype} {helper}(unsigned __int64 a,unsigned __int64 b) '
                +'{ return ('+dtype+f')(((a & 0x{upper_mask:x}ui64) << {lower*8}) | (b & 0x{mask:x}ui64)); }}')
        elif helper.startswith('SUB'):
            input_size, output_size = map(int,helper[3:])
            mask = (1<<(output_size*8))-1
            dtype = 'unsigned int' if output_size<=4 else 'unsigned __int64'
            output.append(f'static __inline {dtype} {helper}(unsigned __int64 value,unsigned int offset) '
                +'{ return ('+dtype+f')((value >> (offset*8)) & 0x{mask:x}ui64); }}')
        else:
            size = int(helper[-1])
            if size == 8:
                raise ValueError(f'Unsupported helper {helper}')
            bits = size*8
            mask = (1<<bits)-1
            expression = (f'(((a^b)&(a^((a-b)&0x{mask:x}ui64)))>>{bits-1})&1'
                          if helper.startswith('SBORROW') else f'(a+b)>0x{mask:x}ui64')
            output.append(f'static __inline unsigned int {helper}(unsigned __int64 a,unsigned __int64 b) '
                +f'{{ a &= 0x{mask:x}ui64; b &= 0x{mask:x}ui64; return (unsigned int)({expression}); }}')
    return '\n'.join(output)+'\n',helpers
