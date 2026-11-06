"""Necessary signed record-score and descriptor-action fixtures."""
import struct
from cohort7_startup_cases import base as startup_base,put,child
REC=0x7999dc+2*264
SELECTION,CACHE,MODE,VALUES,MARKERS=0x70e488,0xd3b5c0,0xbca2e0,0xb7482c,0xb67d1c
GATES=[0x7057b4,0x7057b8,0x7057bc,0x7057c0,0x7057c4,0x7057c8,0x7057cc,0x7057d0,0x705ac4,
 0x705808,0x705810,0x705854,0x705a2c,0x705a30,0x705a28,0x705898,0x705894]
SIMPLE=[(0x4340e0,[0x1f,0x24],0x799a44,[0x7057cc,0x7057b8]),
 (0x434150,[0x1e,0x22,0x24],0x799a45,[0x7057cc,0x7057c4,0x7057b4]),
 (0x4341d0,[0x21,0x22,0x23],0x799a47,[0x7057c8,0x7057c4,0x7057c0]),
 (0x434250,[0x20,0x25],0x799a49,[0x7057d0,0x7057bc])]

def base(entry,label,index=2):
 c=startup_base(entry,label,0xffffffff);c['id']=c['id'].replace('c7-startup/','c7-record-lowio-actions/')
 c.update(arguments=[index],named_arguments=[index],endpoints=[],expected_endpoint_counts={})
 c['stores'] += [(REC,b'\0'*264),(REC+1,b'\1'),(REC+12,b'\x01\x01')]
 c['watched'] += [(REC,264),(CACHE,4)];c['write_ranges'].append((CACHE,4))
 for g in GATES:put(c,g,0)
 for a,v in [(SELECTION,2),(VALUES+8,0xabc01234),(CACHE,0xaabbccdd),(MODE,6)]:put(c,a,v)
 return c

def word(c,at,value):c['stores'].append((at,struct.pack('<H',value&0xffff)))

def simple_cases():
 out=[]
 for entry,types,metric,gates in SIMPLE:
  for mode in ['excluded-word','high-word-wrapped-index','inactive-byte','wrapped-gate-zero']:
   c=base(entry,mode,0x20000002 if mode=='high-word-wrapped-index'else 2)
   word(c,REC+12,types[0] if mode=='excluded-word'else types[0]+0x100)
   c['stores'].append((metric+528,b'\xff'));put(c,gates[0],0x100)
   if mode=='inactive-byte':c['stores'].append((REC+1,b'\0'))
   if mode=='wrapped-gate-zero':put(c,gates[0],0xffffff00);put(c,gates[1],0x100)
   c['expected_eax']=(-15)&0xffffffff if mode=='high-word-wrapped-index'else 0
   out.append(c)
 return out

def aggregate_cases():
 out=[]
 for mode in ['switch-excluded226','signed-negative-byte','high-word-type','wrapped-nine-gate-zero']:
  c=base(0x433ed0,mode);word(c,REC+12,226 if mode=='switch-excluded226'else 0x101 if mode=='signed-negative-byte'else 0x11e)
  c['stores'].append((REC+124,b'\x7f' if mode=='high-word-type'else b'\x80'));put(c,0x7057cc,0x100)
  if mode=='wrapped-nine-gate-zero':put(c,0x7057cc,0xffffff00);put(c,0x7057b4,0x100)
  c['expected_eax']=(22 if mode=='signed-negative-byte'else -2 if mode=='high-word-type'else 0)&0xffffffff
  out.append(c)
 return out

def rank_cases():
 out=[]
 for mode,rank,result in [('rank1',1,8),('rank2',2,5),('rank3',3,2),('unknown-byte',0x81,0),('excluded-word',1,0),('wrapped-gate-zero',1,0)]:
  c=base(0x434060,mode);word(c,REC+12,0x33 if mode=='excluded-word'else 0x133)
  c['stores'].append((REC+125,bytes([rank])));put(c,0x705808,0x100)
  if mode=='wrapped-gate-zero':put(c,0x705808,0xffffff00);put(c,0x705810,0x100)
  c['expected_eax']=result;out.append(c)
 return out

def word_score_cases():
 out=[]
 for entry,metric,excluded in [(0x433c90,0x799a20,0xa7),(0x433d50,0x799a1e,0x51)]:
  for mode in ['excluded-word','child-AL-zero','marker-rejected','callback-fresh-fields']:
   c=base(entry,mode);word(c,REC+12,excluded if mode=='excluded-word'else excluded+0x100);word(c,metric+528,-32768)
   eps=[];counts={};result=0
   if mode!='excluded-word':
    e=child('processing-kind',0x488030,1,0x12340000 if mode=='child-AL-zero'else 1)
    e.update(native_cleanup=4,native_receiver=0xbca350)
    if mode=='child-AL-zero':result=-327
    elif mode=='marker-rejected':put(c,MODE,0);c['stores'].append((MARKERS+2,b'X'))
    else:
     e['mutations']=[(SELECTION,4,0x100),(MODE,4,4),(MARKERS+0x100,1,0x42),(metric+528,2,0x4000),(REC+12,2,excluded)]
     c['write_ranges'] += [(SELECTION,4),(MODE,4),(MARKERS+0x100,1),(REC,264)]
     c['watched'] += [(SELECTION,4),(MODE,4),(MARKERS+0x100,1)];result=163
    eps=[e];counts['processing-kind']=1;c['expected_endpoint_arguments']={'processing-kind':[[2]]}
   c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=result&0xffffffff);out.append(c)
 return out

def percentage_cases():
 out=[]
 for label,amount,result in [('negative',-1,10),('zero',0,10),('positive99',99,5),('exact100',100,1),('exact200',200,1),('above200',201,0),('owner-disabled-still-eight-reads',1,0),('excluded-word',1,0)]:
  c=base(0x4342c0,label);word(c,REC+12,0x46 if label=='excluded-word'else 0x146)
  word(c,REC+14,-1);word(c,REC+24,100);put(c,0x70cdf4-88,0 if label=='owner-disabled-still-eight-reads'else 0x100)
  put(c,0x705854,0x100);c['stores'].append((REC+80,struct.pack('<8h',amount,0,0,0,0,0,0,0)))
  if label=='owner-disabled-still-eight-reads':
   c['read_ranges']=[(REC+80,16)];c['expected_reads']=[[REC+80+2*i,2]for i in range(8)]
  c['expected_eax']=result;out.append(c)
 return out

def amount_cases():
 out=[]
 for entry in [0x46f1c0,0x4347b0]:
  for mode,value in [('inactive',123456),('negative-value',0xfffd0000),('wrapped-rounding',0x7fffffff)]:
   c=base(entry,mode);put(c,REC+136,value);put(c,0x705a2c,0x100)
   if mode=='inactive':c['stores'].append((REC+1,b'\0'));result=0
   else:
    total=(value+5000)&0xffffffff;total=total if total<0x80000000 else total-0x100000000
    result=int(total/10000)
    if entry==0x4347b0:result=int(result/25)
   c['expected_eax']=result&0xffffffff;out.append(c)
 return out

def definitions(registry):
 from cohort7_lowio_record_action_cases import definitions as lowio_definitions
 present={int(m['address'],16)for m in registry['members']}
 root=simple_cases()+aggregate_cases()+rank_cases()+word_score_cases()+percentage_cases()+amount_cases()
 return [c for c in root if c['entry']in present]+lowio_definitions(registry)
