"""Calendar native-unit, mutable-argument and partial-tm ABI fixtures."""
import struct,datetime
from cohort7_startup_cases import TEXT,COPY,put,child
from cohort7_multibyte_cases import base as mbbase
SP=0x30008000
TZ,DAYLIGHT,DST,START,END,FROM_OS,TZI,UTC=0x604e78,0x604e7c,0x604e80,0x604f10,0x604f20,0x12664b0,0x12664b8,0x1266570

def base(entry,label,args=None,result=0xffffffff):
 c=mbbase(entry,label,args,result);c['id']=c['id'].replace('c7-mb/','c7-calendar/')
 c['stores'] += [(TZI,b'\0'*172),(UTC,b'\xa5'*36)]
 for a,n in [(TZ,12),(START,12),(END,12),(FROM_OS,4),(TZI,172),(UTC,36),(SP+4,48)]:c['watched'].append((a,n));c['write_ranges'].append((a,n))
 for a,v in [(TZ,3600),(DAYLIGHT,1),(DST,-3600),(FROM_OS,0),(START,124),(START+4,90),(START+8,7200000),(END,124),(END+4,300),(END+8,3600000)]:put(c,a,v)
 return c

def tm(c,at=TEXT,year=124,day=150,hour=0,minute=0,second=0):
 c['stores'].append((at,struct.pack('<9i',second,minute,hour,1,4,year,0,day,0)))

def timezone():
 e=child('timezone-once',0x566bae,0);e['result_kind']='void';return e

def builder_cases():
 out=[]
 specs=[('relative-leap-start',[1,1,124,4,1,0,0,2,0,0,0],-3600),('relative-last-week',[1,1,125,10,5,0,0,2,0,0,0],-3600),('end-previous-day',[0,0,124,1,0,0,1,0,0,0,0],-3600),('end-next-day',[0,0,124,1,0,0,1,23,0,0,0],7200)]
 for label,args,bias in specs:
  c=base(0x566fcd,label,args,None);put(c,DST,bias);c.update(endpoints=[],expected_endpoint_counts={});out.append(c)
 return out

def daylight_cases():
 out=[]
 for mode in ['disabled','cached-start-boundary','cached-end-boundary','southern-outside-gap','rebuild-US-rules','rebuild-OS-WORD-rules']:
  c=base(0x566e21,mode,[0 if mode=='disabled'else TEXT]);tm(c)
  expected=1
  if mode=='disabled':put(c,DAYLIGHT,0);expected=0
  elif mode=='cached-start-boundary':tm(c,day=90,hour=2)
  elif mode=='cached-end-boundary':tm(c,day=300,hour=1);expected=0
  elif mode=='southern-outside-gap':put(c,START+4,300);put(c,END+4,90);tm(c,day=40)
  elif mode.startswith('rebuild'):
   put(c,START,-1);put(c,END,-1)
   if mode=='rebuild-OS-WORD-rules':
    put(c,FROM_OS,0x100)
    c['stores'] += [(TZI+152,struct.pack('<8H',0,3,0,2,2,0,0,258)),(TZI+68,struct.pack('<8H',2024,11,0,3,2,0,0,0))]
  c.update(endpoints=[],expected_endpoint_counts={},expected_eax=expected);out.append(c)
 return out

def local_time_cases():
 out=[]
 for mode,seconds,bias,daylight in [('negative',-1,3600,0),('fast-standard',1711929600,3600,0),('fast-daylight',1711929600,3600,1),('epoch-previous-year',0,3600,0),('upper-edge-next-hour',0x7fffffff,-3600,0)]:
  c=base(0x562644,mode,[COPY]);put(c,COPY,seconds);put(c,TZ,bias);put(c,DAYLIGHT,daylight)
  eps=[];counts={};expected=0
  if mode!='negative':
   eps=[timezone()];counts['timezone-once']=1;expected=UTC
   if daylight:put(c,START+4,0);put(c,END+4,365)
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def make_time_cases():
 out=[]
 for mode in ['invalid-year','forced-daylight','partial-frame-and-year-alias']:
  args=[1969 if mode=='invalid-year'else 2024,7,1,12,34,56,-1 if mode=='partial-frame-and-year-alias'else 1]
  c=base(0x567206,mode,args);frame=SP-40
  original=[17,23,0x11223344,0x22334455,0x33445566,0x44556677,0x55667711,0x66771122,0x77112233]
  c['stores'].append((frame,struct.pack('<9i',*original)));c['watched'].append((frame,36))
  eps=[];counts={};expected=0xffffffff
  if mode!='invalid-year':
   tz=timezone();eps=[tz];counts['timezone-once']=1
   expected=int(datetime.datetime(2024,7,1,12,34,56,tzinfo=datetime.timezone.utc).timestamp())&0xffffffff
   if mode=='partial-frame-and-year-alias':
    tz['mutations']=[(SP+4,4,2050),(SP+8,4,12),(SP+12,4,31),(SP+16,4,5),(SP+20,4,6),(SP+24,4,7)]
    pred=child('daylight-predicate',0x566e21,1,1)
    pred.update(private_input_args={0:dict(role='native-partial-tm',width=36)},mutations=[(SP+4,4,0x10203040)],observe=[(SP+4,4)])
    eps.append(pred);counts['daylight-predicate']=1;expected=(0x10203040-3600)&0xffffffff
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in builder_cases()+daylight_cases()+local_time_cases()+make_time_cases()if c['entry']in present]
