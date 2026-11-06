"""Focused scalar transfer, record stride and low-I/O layout checks."""
import struct
from cohort7_lowio_startup_cases import lowio_startup_cases
from cohort7_startup_cases import base as startup_base,put,TEXT,HEAP,COPY,WIDE,child,imp
SCORES=[(0x4343a0,0xb4,0x705a0c,0x799a4f),(0x434510,0x33,0x705808,0x799a4a),
 (0x434570,0x35,0x705810,0x799a4b),(0x4345d0,0x31,0x705800,0x799a4d),
 (0x434630,0x31,0x705a74,0x799a46),(0x434690,0x2e,0x7057f4,0x799a4e),
 (0x4346f0,0x2f,0x7057f8,0x799a50),(0x434750,0xb8,0x705a1c,0x799a48)]
TABLE,CAP,ERR,DOS=0x1266960,0x1266a60,0x1266430,0x1266434

def base(entry,label,args=None,result=0xffffffff):
 c=startup_base(entry,label,result);c['id']=c['id'].replace('c7-startup/','c7-scalar-lowio/')
 c.update(arguments=args or [],named_arguments=args or [],endpoints=[],expected_endpoint_counts={})
 return c

def score_cases():
 out=[]
 for entry,excluded,gate,metric in SCORES:
  for mode in ['excluded-word','high-word-wrap-index','inactive-byte','disabled-dword']:
   index=0x20000002 if mode=='high-word-wrap-index'else 2
   c=base(entry,mode,[index]);offset=2*264;record=0x7999dc+offset
   c['stores'] += [(record,b'\0'*264),(0x7999e8+offset,struct.pack('<H',excluded if mode=='excluded-word'else excluded+0x100)),
    (0x7999dd+offset,bytes([0 if mode=='inactive-byte'else 1])),(metric+offset,b'\xff')]
   put(c,gate,0 if mode=='disabled-dword'else 0x100)
   c['watched'] += [(record,264),(gate,4)]
   c['expected_eax']=(-15)&0xffffffff if mode=='high-word-wrap-index'else 0
   out.append(c)
 return out

def grid_cases():
 out=[]
 for mode in ['disabled-dword','inactive-byte','wide-coordinate-allowed','grid-bit-blocked']:
  c=base(0x4d6b00,mode,[2]);position=0x100+2*228+3;cell=0xaf598c+position*4
  for at,value in [(0xd05708,0 if mode=='disabled-dword'else 0x100),(0xbca2c8,0x100),(0xbca2cc,2),(0xbca2d8,3),(cell,0x104 if mode=='grid-bit-blocked'else 0x100)]:put(c,at,value)
  c['stores'].append((0xbcd728+2*388,bytes([0 if mode=='inactive-byte'else 1])))
  c['expected_eax']=1 if mode=='wide-coordinate-allowed'else 0
  out.append(c)
 return out

def handle_cases():
 out=[]
 for label,descriptor,flags in [('second-block',35,1),('closed-byte',35,0x80),('capacity-bound',64,1),('unsigned-invalid',0xffffffff,1)]:
  c=base(0x5669c6,label,[descriptor]);put(c,TABLE,HEAP);put(c,TABLE+4,TEXT);put(c,CAP,64)
  put(c,ERR,0xaabbccdd);put(c,DOS,0x12345678)
  c['stores'].append((TEXT+3*8,struct.pack('<IBBH',0xf1234567,flags,0x0a,0xcdef)))
  c['watched'] += [(TABLE,264),(ERR,8)];c['write_ranges'] += [(ERR,8)]
  c['expected_eax']=0xf1234567 if label=='second-block'else 0xffffffff
  out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in score_cases()+grid_cases()+handle_cases()+lowio_startup_cases()if c['entry']in present]
