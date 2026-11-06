"""Connected scalar, caller-slot and live-ECX ABI recipes."""
import struct
from cohort7_startup_cases import TEXT,COPY,HEAP,put,imp,child
from cohort7_multibyte_cases import base as mbbase,CP,LOCALE,FLAGS,MAP,ENABLED,LOCAL_CP
SP=0x30008000
CTYPE,MAX_BYTES,ERRNO=0x126648c,0x604e64,0x1266430
NAME1,NAME2,NAME3=0x32005000,0x32006000,0x32007000
STDPTR,DSTPTR,CACHED,TZI,FROM_OS=0x604f04,0x604f08,0x1266564,0x12664b8,0x12664b0

def base(entry,label,args=None,result=0xffffffff):
 c=mbbase(entry,label,args,result);c['id']=c['id'].replace('c7-mb/','c7-consumers/')
 c['stores'].append((FLAGS,b'\0'*257));c['watched'] += [(SP+4,16),(CTYPE,4),(MAX_BYTES,4),(ERRNO,4)]
 c['write_ranges'] += [(CTYPE,4),(MAX_BYTES,4),(ERRNO,4),(SP,24)]
 for a,v in [(CP,932),(LOCALE,0x411),(ENABLED,1),(LOCAL_CP,932),(CTYPE,0),(MAX_BYTES,0x102),(ERRNO,0x12345678)]:put(c,a,v)
 return c

def uppercase_cases():
 out=[]
 for mode in ['single-byte','pair-two-output-bytes','pair-one-output-byte','pair-without-lead']:
  arg=0x61 if mode=='single-byte'else 0xaabb3344 if mode=='pair-without-lead'else 0x817a
  c=base(0x56b363,mode,[arg]);eps=[];counts={};expected=arg
  if mode=='single-byte':
   c['stores'] += [(FLAGS+0x62,b'\x20'),(MAP+0x61,b'A')];expected=0x41
  elif mode!='pair-without-lead':
   c['stores'].append((FLAGS+0x82,b'\x04'))
   e=child('uppercase-map',0x56947e,8,1)
   e.update(private_input_args={2:dict(role='incoming-argument-pair',width=2)},pointer_args={4:'native-ECX-scratch'})
   payload=b'\x81A'if mode=='pair-two-output-bytes'else b'A'
   e['writes_arg_bytes']=[(4,0,payload.hex())];eps=[e];counts['uppercase-map']=1
   c['receiver']=0x12345678;expected=0x8141 if len(payload)==2 else 0x4156
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def find_cases():
 out=[]
 for mode in ['single-byte-delegate','high-dword-multibyte-flag','pair-match']:
  character=0x817a if mode=='pair-match'else 0x12340041
  c=base(0x56b104,mode,[TEXT,character]);put(c,ENABLED,0 if mode=='single-byte-delegate'else 0x100)
  c['stores'].append((TEXT,b'\x81zx\0'if mode=='pair-match'else b'A\0'))
  if mode=='pair-match':c['stores'].append((FLAGS+0x82,b'\x04'))
  # Existing real native/C strchr handles lowBYTE only on the disabled path.
  c.update(endpoints=[],expected_endpoint_counts={},expected_eax=0 if mode=='high-dword-multibyte-flag'else TEXT)
  out.append(c)
 return out

def collate_cases():
 out=[]
 for value in [None,0,3]:
  c=base(0x56a857,'zero-count'if value is None else f'compare-{value}',[TEXT,COPY,0 if value is None else 0x80000001])
  eps=[];counts={}
  if value is not None:eps=[child('comparison',0x56ac16,7,value)];counts['comparison']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=0 if value is None else 0x7fffffff if value==0 else 1);out.append(c)
 return out

def wide_character_cases():
 out=[]
 for mode in ['null','single-byte','too-wide','locale-success','locale-default']:
  character=0x100 if mode=='too-wide'else 0x3042 if mode=='locale-success'else 0x17e if mode=='locale-default'else 0xcafe0041
  c=base(0x5691e5,mode,[0 if mode=='null'else TEXT,character]);eps=[];counts={};expected=0 if mode=='null'else 1
  if mode.startswith('locale'):
   put(c,CTYPE,0x100);default=int(mode=='locale-default');payload=b'?'if default else b'\x82\xa0'
   e=imp('WideCharToMultiByte',8,len(payload));e.update(pointer_args={7:'first-incoming-argument-BOOL'},private_input_args={2:dict(role='incoming-WCHAR',width=2)},writes_args=[(7,4,default)],mutations=[(TEXT+i,1,v)for i,v in enumerate(payload)],observe=[(SP+4,4)])
   eps=[e];counts['WideCharToMultiByte']=1;expected=0xffffffff if default else len(payload)
  elif mode=='too-wide':expected=0xffffffff
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def timezone_cases():
 out=[]
 for mode in ['OS-reloaded-name-pointer','OS-live-ECX-BOOL','TZ-negative-hms']:
  c=base(0x566bc3,mode,[],None)
  for p in [NAME1,NAME2,NAME3]:c['stores'].append((p,b'\xa5'*80));c['watched'].append((p,80));c['write_ranges'].append((p,80))
  c['stores'] += [(0x604e60,b'\xa5'*0x1c0),(FROM_OS,b'\xa5'*184)]
  c['watched'] += [(0x604e60,0x1c0),(FROM_OS,184)];c['write_ranges'] += [(0x604e60,0x1c0),(FROM_OS,184)]
  for a,v in [(STDPTR,NAME1),(DSTPTR,NAME2),(CACHED,0)]:put(c,a,v)
  env=child('getenv',0x5696a2,1,TEXT if mode.startswith('TZ')else 0)
  eps=[env];counts={'getenv':1}
  if mode.startswith('OS'):
   data=bytearray(172);struct.pack_into('<i',data,0,60);struct.pack_into('<H',data,70,0x100)
   struct.pack_into('<i',data,84,0);struct.pack_into('<H',data,154,3);struct.pack_into('<i',data,168,-60)
   data[4:12]='CET\0'.encode('utf-16le');data[88:98]='CEST\0'.encode('utf-16le')
   os=imp('GetTimeZoneInformation',1,2);os['mutations']=[(TZI+i,1,v)for i,v in enumerate(data)]
   convert=imp('WideCharToMultiByte',8,3);convert['pointer_args']={7:'entry-ECX-BOOL-cell'}
   first=dict(mutations=[(NAME1+i,1,v)for i,v in enumerate(b'CET')]);second=dict(mutations=[(NAME2+i,1,v)for i,v in enumerate(b'CES')])
   if mode=='OS-reloaded-name-pointer':
    first['writes_args']=[(7,4,0)];first['mutations'].append((STDPTR,4,NAME3));second['writes_args']=[(7,4,0)]
   else:c['receiver']=0x100
   convert['responses']=[first,second];eps += [os,convert];counts.update(GetTimeZoneInformation=1,WideCharToMultiByte=2)
  else:
   text=b'CET-1:30:15CEST\0';c['stores'].append((TEXT,text))
   free=child('release',0x561bdc,1);free['result_kind']='void'
   length=child('length',0x563380,1,len(text)-1);allocate=child('allocate',0x561b68,1,COPY)
   copy=child('cache-copy',0x566750,2);copy['result_kind']='void'
   prefix=child('prefix-copy',0x562240,3);prefix['responses']=[dict(mutations=[(NAME1+i,1,v)for i,v in enumerate(b'CET')]),dict(mutations=[(NAME2+i,1,v)for i,v in enumerate(b'CES')])]
   number=child('parse-number',0x561c0b,1);number['responses']=[dict(value=v)for v in [1,30,15]]
   eps += [free,length,allocate,copy,prefix,number];counts.update(release=1,length=1,allocate=1,**{'cache-copy':1,'prefix-copy':2,'parse-number':3})
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in uppercase_cases()+find_cases()+collate_cases()+wide_character_cases()+timezone_cases()if c['entry']in present]
