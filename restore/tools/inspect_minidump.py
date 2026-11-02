"""Read a bounded x86 minidump context/stack without launching the game."""
from pathlib import Path
import argparse,struct,json,re

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('dump',type=Path);p.add_argument('--map',type=Path);p.add_argument('--words',type=int,default=48)
a=p.parse_args();data=a.dump.read_bytes()
assert data[:4]==b'MDMP','Not a Windows minidump'
u32=lambda off:struct.unpack_from('<I',data,off)[0]
u64=lambda off:struct.unpack_from('<Q',data,off)[0]
streams={}
for i in range(u32(8)):
    typ,size,rva=struct.unpack_from('<III',data,u32(12)+i*12);streams[typ]=(rva,size)
ranges=[]
if 5 in streams:
    rva,_=streams[5]
    for i in range(u32(rva)):
        start,size,loc=struct.unpack_from('<QII',data,rva+4+i*16);ranges.append((start,size,loc))
if 9 in streams:
    rva,_=streams[9];loc=u64(rva+8)
    for i in range(u64(rva)):
        start,size=struct.unpack_from('<QQ',data,rva+16+i*16);ranges.append((start,size,loc));loc+=size
if 3 in streams:
    rva,_=streams[3]
    for i in range(u32(rva)):
        start,size,loc=struct.unpack_from('<QII',data,rva+4+i*48+24);ranges.append((start,size,loc))
def read(address,size):
    for start,length,rva in ranges:
        if start<=address and address+size<=start+length:return data[rva+address-start:rva+address-start+size]
    return None
symbols=[]
if a.map:
    for m in re.finditer(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+(\S+)\s+([0-9a-fA-F]{8})\s',a.map.read_text(),re.M):
        symbols.append((int(m[2],16),m[1]))
    symbols.sort()
def symbol(address):
    import bisect
    i=bisect.bisect_right(symbols,(address,'\uffff'))-1
    if i>=0 and address-symbols[i][0]<0x10000:return symbols[i][1]+'+0x%x'%(address-symbols[i][0])
    return None
rva,_=streams[6];context=u32(rva+164)
registers={name:u32(context+off) for name,off in [('eax',176),('ebx',164),('ecx',172),('edx',168),('esi',160),('edi',156),('ebp',180),('esp',196),('eip',184),('eflags',192)]}
print(json.dumps({'exception':{'code':hex(u32(rva+8)),'address':hex(u64(rva+24)),'parameters':[hex(u64(rva+40+i*8)) for i in range(min(u32(rva+32),15))]},'registers':{k:hex(v) for k,v in registers.items()}},indent=2))
if 4 in streams:
    rva,_=streams[4]
    for i in range(u32(rva)):
        off=rva+4+i*108;name_rva=u32(off+20);name=data[name_rva+4:name_rva+4+u32(name_rva)].decode('utf-16le',errors='replace')
        if 'PharaohRestoreC' in name:print(json.dumps({'module':name,'base':hex(u64(off)),'size':hex(u32(off+8)),'coff':hex(u32(off+16))}))
sp=registers['esp']
for i in range(min(max(a.words,0),256)):
    raw=read(sp+i*4,4)
    if raw is None:break
    value=struct.unpack('<I',raw)[0]
    print('%08x: %08x %s'%(sp+i*4,value,symbol(value) or ''))
