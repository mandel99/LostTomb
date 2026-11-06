"""Necessary native-unit and parent-stack recipes for the codepage cluster."""
import struct
from cohort7_startup_cases import base as startup_base,put,imp,child
SP=0x30008000
CP,DEFAULT,ENABLED,LOCALE,LOCAL_CP,SYMBOLS,FLAGS,MAP=0x1266710,0x12666e4,0x126672c,0x1266944,0x126649c,0x1266720,0x1266840,0x1266740
BLOCK=0x12666d0

def base(entry,label,args=None,result=None):
 c=startup_base(entry,label,result);c['id']=c['id'].replace('c7-startup/','c7-mb/')
 c['arguments']=c['named_arguments']=args or []
 c['stores'].append((BLOCK,b'\xa5'*0x500));c['watched'] += [(BLOCK,0x500),(LOCAL_CP,4)]
 c['write_ranges'] += [(BLOCK,0x500),(LOCAL_CP,4)]
 put(c,CP,0x12345678);put(c,LOCAL_CP,65001)
 return c

def classify_cases():
 out=[]
 for label,args,result in [('word-stride',[0x141,0x100,0],1),('low-byte-mask',[0xff41,0,0x108],1),('disjoint-word-mask',[0x41,0x200,0],0)]:
  c=base(0x569800,label,args,0xffffffff)
  c['stores'] += [(0x604c62,b'\0'*512),(0x604c62+2*0x41,struct.pack('<H',0x100)),(0x604c62+8*0x41,b'\0\0'),(FLAGS+0x42,b'\x08')]
  c.update(endpoints=[],expected_endpoint_counts={},expected_eax=result);out.append(c)
 return out

def resolve_cases():
 out=[]
 for requested in [932,-4,-2]:
  c=base(0x5699ca,f'request-{requested}',[requested&0xffffffff],0xffffffff)
  eps=[];counts={};result=65001 if requested==-4 else 932
  if requested==-2:
   e=imp('GetOEMCP',0,932);e['observe']=[(DEFAULT,4)];e['mutations']=[(DEFAULT,4,0x87654321)]
   eps=[e];counts['GetOEMCP']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=result);out.append(c)
 return out

def reset_cases():
 c=base(0x569a47,'exact-byte-and-dword-ranges');c.update(endpoints=[],expected_endpoint_counts={});return [c]

def cpinfo():return struct.pack('<I',2)+b'?\0'+bytes([0x81,0x82])+b'\0'*10+b'\0'*2

def case_buffers():
 attributes=bytearray(512)
 for at,value in [(0x41,1),(0x61,2),(0x81,3)]:struct.pack_into('<H',attributes,at*2,value)
 lower=bytes((i+0x20)&255 for i in range(256));upper=bytes((i-0x20)&255 for i in range(256))
 return bytes(attributes),lower,upper

def case_table_cases():
 out=[]
 for mode in ['fallback-bool2','zero-return-written-outputs','zero-return-preexisting-stack']:
  c=base(0x569a70,mode);put(c,CP,932);put(c,LOCALE,0x411)
  info=imp('GetCPInfo',2,2 if mode=='fallback-bool2'else 1);info['pointer_args']={1:'CPINFO'}
  eps=[info];counts={'GetCPInfo':1}
  if mode!='fallback-bool2':
   info['writes_arg_bytes']=[(1,0,cpinfo().hex())]
   attributes,lower,upper=case_buffers()
   types=child('character-types',0x569335,7,0);types.update(pointer_args={3:'character-types'},private_input_args={1:dict(role='sanitized-256-byte-input',width=256)})
   mapping=child('case-map',0x56947e,8,0);mapping.update(pointer_args={4:'case-bytes'},private_input_args={2:dict(role='sanitized-256-byte-input',width=256)})
   if mode=='zero-return-written-outputs':
    types['writes_arg_bytes']=[(3,0,attributes[:256].hex()),(3,256,attributes[256:].hex())]
    mapping['responses']=[dict(writes_arg_bytes=[(4,0,lower.hex())]),dict(writes_arg_bytes=[(4,0,upper.hex())])]
   else:
    # Exact native parent offsets; ignored failing outputs retain these bytes.
    frame=SP-4-0x514
    c['stores'] += [(frame,attributes),(frame+512,upper),(frame+768,lower)]
   eps += [types,mapping];counts.update({'character-types':1,'case-map':2})
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def setter_cases():
 out=[]
 for mode in ['builtin-932','generic-dbcs','default-failure-high-dword']:
  c=base(0x569831,mode,[932 if mode=='builtin-932'else 1200],0xffffffff)
  info=imp('GetCPInfo',2,2);info['pointer_args']={1:'CPINFO'}
  info['observe']=[(CP,4),(DEFAULT,4),(ENABLED,4),(LOCALE,4),(SYMBOLS,4),(SYMBOLS+4,4),(SYMBOLS+8,4)]
  counts={'GetCPInfo':1}
  if mode=='generic-dbcs':
   info['responses']=[dict(value=1,writes_arg_bytes=[(1,0,cpinfo().hex())]),dict(value=2)];counts['GetCPInfo']=2
  elif mode=='default-failure-high-dword':
   info['responses']=[dict(value=0,mutations=[(DEFAULT,4,0x100)]),dict(value=0)];counts['GetCPInfo']=2
  c.update(endpoints=[info],expected_endpoint_counts=counts,expected_eax=0);out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in classify_cases()+resolve_cases()+reset_cases()+case_table_cases()+setter_cases()if c['entry']in present]
