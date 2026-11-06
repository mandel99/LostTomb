"""Connected environment-vector and callback-capture ABI fixtures."""
from cohort7_startup_cases import TEXT,HEAP,COPY,WIDE,put,imp,child
from cohort7_multibyte_cases import base as mbbase,CP,LOCALE,FLAGS,ENABLED
ENV,INITIAL,WENV,READY,TZ_ONCE=0x1266458,0x126645c,0x1266460,0x1266a80,0x1266568
VECTOR,WIDEV,OTHER=0x32008000,0x32009000,0x3200a000

def base(entry,label,args=None,result=0xffffffff):
 c=mbbase(entry,label,args,result);c['id']=c['id'].replace('c7-mb/','c7-env/')
 for p in [VECTOR,WIDEV,OTHER]:c['stores'].append((p,b'\xa5'*128));c['watched'].append((p,128));c['write_ranges'].append((p,128))
 c['watched'] += [(ENV,12),(READY,4),(TZ_ONCE,4)];c['write_ranges'] += [(ENV,12),(READY,4),(TZ_ONCE,4)]
 c['stores'].append((FLAGS,b'\0'*257))
 for a,v in [(ENV,VECTOR),(INITIAL,OTHER),(WENV,WIDEV),(READY,0x100),(TZ_ONCE,0),(ENABLED,0),(CP,932),(LOCALE,0x411),(VECTOR,0),(WIDEV,0),(OTHER,0)]:put(c,a,v)
 return c

def comparison(value=2):return child('comparison',0x56ac16,7,value)
def malloc(value=HEAP):return child('allocate',0x561b68,1,value)
def realloc(value=OTHER):return child('resize',0x5645a0,2,value)
def release():
 e=child('release',0x561bdc,1);e['result_kind']='void';return e

def getenv_cases():
 out=[]
 c=base(0x5696a2,'captured-vector-fresh-value',[COPY]);put(c,VECTOR,TEXT)
 c['stores'] += [(COPY,b'PATH\0'),(TEXT,b'PATH=value\0'),(HEAP,b'PATH=changed\0')]
 cmp=comparison();cmp['mutations']=[(VECTOR,4,HEAP),(ENV,4,OTHER)]
 c.update(endpoints=[cmp],expected_endpoint_counts={'comparison':1},expected_eax=HEAP+5);out.append(c)
 c=base(0x5696a2,'lazy-wide-converter-setter-chain',[TEXT]);put(c,ENV,0);put(c,INITIAL,0);put(c,WIDEV,WIDE);put(c,WIDEV+4,0)
 c['stores'] += [(TEXT,b'PATH\0'),(WIDE,'PATH=value\0'.encode('utf-16le'))]
 convert=imp('WideCharToMultiByte',8,11);convert['responses']=[dict(value=11),dict(value=11,mutations=[(HEAP+i,1,v)for i,v in enumerate(b'PATH=value\0')])]
 allocate=malloc();allocate['responses']=[dict(value=HEAP),dict(value=VECTOR)]
 c.update(endpoints=[convert,allocate,realloc(),comparison()],expected_endpoint_counts={'WideCharToMultiByte':2,'allocate':2,'resize':1,'comparison':1},expected_eax=HEAP+5);out.append(c)
 c=base(0x5696a2,'not-ready-no-vector-access',[COPY]);put(c,READY,0);put(c,ENV,0xdeadbeef)
 c.update(endpoints=[],expected_endpoint_counts={},expected_eax=0);out.append(c)
 c=base(0x5696a2,'null-name-after-lazy-conversion',[0]);put(c,ENV,0)
 convert=child('convert-wide-vector',0x56a896,0,0);convert['mutations']=[(ENV,4,VECTOR)]
 c.update(endpoints=[convert],expected_endpoint_counts={'convert-wide-vector':1},expected_eax=0);out.append(c)
 return out

def convert_cases():
 out=[]
 for mode in ['empty','query-failure','second-conversion-failure','ignored-setter-and-captured-wide-vector']:
  c=base(0x56a896,mode);eps=[];counts={};expected=0
  if mode!='empty':
   put(c,WIDEV,WIDE);put(c,WIDEV+4,0);c['stores'] += [(WIDE,'G=1\0'.encode('utf-16le')),(WIDE+32,'H=2\0'.encode('utf-16le'))]
   conv=imp('WideCharToMultiByte',8,0 if mode=='query-failure'else 4);eps=[conv];counts['WideCharToMultiByte']=1
   if mode=='query-failure':expected=0xffffffff
   else:
    allocate=malloc();allocate['mutations']=[(WIDEV,4,WIDE+32)]
    conv['responses']=[dict(value=4),dict(value=0 if mode=='second-conversion-failure'else 4,mutations=[(HEAP+i,1,v)for i,v in enumerate(b'H=2\0')])]
    eps.append(allocate);counts.update(WideCharToMultiByte=2,allocate=1)
    if mode=='second-conversion-failure':
     expected=0xffffffff;eps.append(release());counts['release']=0
    else:
     setter=child('set-entry',0x56aebe,2,0xffffffff);setter['mutations']=[(WENV,4,OTHER)]
     eps.append(setter);counts['set-entry']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def finder_cases():
 out=[]
 for mode in ['found-prefix','missing-fresh-global-base','empty-vector']:
  c=base(0x56b045,mode,[COPY,1]);c['stores'] += [(COPY,b'A\0'),(TEXT,b'A=1\0'),(HEAP,b'B=2\0')]
  eps=[];counts={};expected=0
  if mode!='empty-vector':
   put(c,VECTOR,TEXT);put(c,VECTOR+4,HEAP);put(c,VECTOR+8,0)
   e=comparison(2 if mode=='found-prefix'else 1)
   if mode=='missing-fresh-global-base':e['mutations']=[(ENV,4,VECTOR+4)];expected=0xffffffff
   eps=[e];counts['comparison']=1 if mode=='found-prefix'else 2
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=expected);out.append(c)
 return out

def timezone_once_cases():
 out=[]
 for initial in [0,0x100]:
  c=base(0x566bae,f'guard-{initial:x}',result=None);put(c,TZ_ONCE,initial);eps=[];counts={}
  if initial==0:
   e=child('timezone',0x566bc3,0);e.update(result_kind='void',mutations=[(TZ_ONCE,4,0xffffffff)]);eps=[e];counts['timezone']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def setter_cases():
 out=[]
 for mode in ['append','replace-captured-vector','delete-shrink-failure','OS-failure-still-success']:
  entry=b'X='if mode=='delete-shrink-failure'else b'NEW=2'if mode=='append'else b'X=new'
  c=base(0x56aebe,mode,[HEAP,int(mode=='OS-failure-still-success')]);put(c,WENV,0)
  c['stores'] += [(HEAP,entry+b'\0'),(TEXT,b'X=old\0'),(COPY,b'Y=2\0')];put(c,VECTOR,TEXT);put(c,VECTOR+4,0)
  eps=[];counts={}
  if mode=='append':
   cmp=comparison(1);resize=realloc();resize['mutations']=[(OTHER,4,TEXT)]
   eps=[cmp,resize];counts={'comparison':1,'resize':1}
  else:
   lookup=child('find-name',0x56b045,2,0);eps=[lookup];counts={'find-name':1}
   if mode=='replace-captured-vector':lookup['mutations']=[(ENV,4,OTHER)]
   elif mode=='delete-shrink-failure':
    put(c,VECTOR+4,COPY);put(c,VECTOR+8,0);eps += [release(),realloc(0)];counts.update(release=1,resize=1)
   else:
    allocate=malloc(COPY);os=imp('SetEnvironmentVariableA',2,0)
    eps += [allocate,os,release()];counts.update(allocate=1,SetEnvironmentVariableA=1,release=1)
    # Real native/C strlen, strchr and strcpy remain active in this path.
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=0);out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in getenv_cases()+convert_cases()+finder_cases()+timezone_once_cases()+setter_cases()if c['entry']in present]
