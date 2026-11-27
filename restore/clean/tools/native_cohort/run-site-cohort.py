import pathlib,os,json,struct,re,time,base64
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison'
ns={'__name__':'site_common'};p=D/'compare-placement.py';exec(compile(p.read_text().replace('placement-fixture.dll','sites-fixture.dll'),str(p),'exec'),ns)
call,chunk,request,DLL,EXPORT,REG=[ns[n] for n in ['call','chunk','request','DLL','EXPORT','REG']]
CASE=0x200f0000;SP=0x30010000;CELL=113*228+111
entries=[0x4bff60,0x4c3730,0x4def00,0x541ae0,0x542ee0,0x4e2e20,0x470900,0x44bb90,0x44d6d0,0x44d700]
m=(D/'sites-fixture.map').read_text()
def symbol(n):
 x=re.search(r'\s_'+re.escape(n)+r'\s+([0-9A-Fa-f]{8})\s',m);assert x,n;return int(x[1],16)
def pack(a,v,fmt='<i'):return chunk(a,struct.pack(fmt,v))
def words(a,v,n,fmt='i'):return chunk(a,struct.pack('<'+str(n)+fmt,*([v]*n)))
def event_data(records):
 b=bytearray(141*124)
 for i,r in enumerate(records):
  o=124*i;struct.pack_into('<hBB',b,o,r.get('id',99),r.get('type',2),r.get('month',0));struct.pack_into('<h',b,o+20,r.get('year',0));struct.pack_into('<Ih',b,o+40,r.get('flags',0),r.get('duration',200))
 return chunk(0x785d74,b)
def parts(n,size=1):
 b=bytearray(n*52)
 for i in range(n):struct.pack_into('<5i',b,i*52,i%10,i//10,size,0,0)
 return b
cases=[]
def add(label,op,writes=None,**kw):cases.append({'label':label,'op':op,'writes':writes or [],**kw})
for mode in [0,1,2,-1]:
 for mask in [0,1,0x40,8,0x1000000]:add(f'gate{mode}:terrain{mask}',0,[words(0xaf598c,mask,51984,'I')],size=mode)
for x in [2,3,224,225]:add(f'gate_bound{x}',0,x=x,y=113)
origins=[[(0,-2),(4,2),(2,4),(-2,0)],[(-2,0),(0,-2),(4,2),(2,4)],[(2,4),(-2,0),(0,-2),(4,2)],[(4,2),(2,4),(-2,0),(0,-2)]]
for view in range(4):
 for turn in range(4):
  dx,dy=origins[view][turn];tile=(113+dy)*228+111+dx;w=[pack(0xbca2e0,view*2),pack(0xbc9714,turn)]
  add(f'temple{view}:{turn}:free',1,w)
  add(f'temple{view}:{turn}:terrain',1,w+[pack(0xaf598c+tile*4,8)])
  add(f'temple{view}:{turn}:entry',1,w+[pack(0x784b64,111+dx,'<h'),pack(0x784b6c,113+dy,'<h')])
add('temple_walker_precedes_entry',1,[pack(0x784b64,111,'<h'),pack(0x784b6c,111,'<h'),pack(0xb350dc+(111*228+111)*2,-1,'<h')])
for op,n,address in [(2,116,0x5e2ad8),(3,100,0x601dd8)]:
 for view in [0,2,4,6]:add(f'monument{op}:view{view}',op,[pack(0xbca2e0,view),chunk(address,parts(n))])
 for label,mask,idx in [('bit0',1,0),('blocked_first',8,0),('blocked_last',8,n-1)]:
  tile=(113+idx//10)*228+111+idx%10;add(f'monument{op}:{label}',op,[chunk(address,parts(n)),pack(0xaf598c+tile*4,mask)])
 for label,mask in [('6x6_clear',0),('6x6_last_blocked',8)]:
  b=parts(n,0);struct.pack_into('<i',b,8,6);add(f'monument{op}:{label}',op,[chunk(address,b),pack(0xaf598c+(CELL+5*228+5)*4,mask)])
 add(f'monument{op}:outside',op,[chunk(address,parts(n))],extra=2*228+2)
for kind in ['none','wrong_state','complete','incomplete','skip99','next101']:
 b=bytearray(4000*264)
 if kind!='none':
  b[264]=2 if kind=='wrong_state' else 1;struct.pack_into('<h',b,264+16,213)
  if kind in ['complete','skip99','next101']:
   root=1 if kind=='complete' else 100 if kind=='skip99' else 101;b[root*264]=1;struct.pack_into('<h',b,root*264+16,213)
   for i in range(root,root+100):struct.pack_into('<i',b,i*264+88,13)
 add('completed:'+kind,4,[chunk(0x7999d8,b),chunk(0x601dd8,bytes(100*52)),chunk(0xbcd5f8,bytes(388))])
# The complete native BYTE mapper is also compared statically in the receipt.
for tool in [0,29,30,31,32,33,34,35,36,37,54,55,56,59,60,64,65,69,70,76,77,78,79,80,81,82,85,86,87,137,138,139,140,149,150,166,167,168,183,184,185,186,187,188,189,190,225,226,227]:add('military_tool:'+str(tool),5,tool=tool)
for label,recs in [('running',[{}]),('future',[{'year':5}]),('expired',[{'duration':1}]),('disabled',[{'flags':0x10}]),('other_type',[{'type':1}]),('second',[{'year':5},{}]),('upper_flags',[{'flags':0x100}])]:
 add('military:'+label,5,[pack(0x785d70,len(recs)),event_data(recs)],tool=30)
for label,w in [('editor1',[pack(0x605a3c,1)]),('editor2',[pack(0x605a3c,2)]),('shown',[pack(0xbcb9b4,1)]),('troops',[pack(0xea1870+0x4668,1,'<B')]),('player1',[pack(0xeaac28,1),pack(0xea1870+0x49d8+0x4668,1,'<B')])]:add('military:'+label,5,[pack(0x785d70,1),event_data([{}])]+w,tool=30)
for i in range(4):
 for second in [False,True]:
  a,b=(0x784cc0+i*4,0x784cd0+i*4) if second else (0x784b64+i*2,0x784b6c+i*2);fmt='<i' if second else '<h';add(f'entrypoint:{i}:{second}',6,[pack(a,111,fmt),pack(b,113,fmt)],size=111,extra=113)
add('entrypoint_reverse_bounds',6,[pack(0x784b64,111,'<h'),pack(0x784b6c,113,'<h')],x=112,y=114,size=110,extra=112)
for label,recs,previous,create in [('none',[],-1,0),('first',[{}],-1,0),('second',[{'type':1},{}],-1,0),('continue',[{'id':0},{}],0,0),('exhausted',[{'id':0}],0,0),('create',[],-1,1),('create_after_noninvasion',[{'type':1}],-1,1)]:add('invasion:'+label,7,[pack(0x785d70,len(recs)),event_data(recs)],x=previous,y=create)
for op in [8,9]:
 for year in [-2,0,1,2,256]:
  for month in [0,11]:add(f'calendar{op}:{year}:{month}',op,[event_data([{'year':year,'month':month}])],x=0)
 for flags in [1,0x10,0x100]:add(f'calendar{op}:flags{flags}',op,[event_data([{'year':1,'flags':flags}])],x=0)

def execute(c):
 op=c['op'];x,y,size,extra=c.get('x',111),c.get('y',113),c.get('size',1),c.get('extra',CELL);tool=c.get('tool',0)
 w=[chunk(0xbca2d0,struct.pack('<12i',228,228,0,0,0,0,0,0,227,227,114,114)),pack(0xbc9768,0x12345678),pack(0xbc9714,0),words(0x784b64,1000,8,'h'),words(0x784cc0,1000,8),pack(0x605a3c,0),pack(0xeaac28,0),chunk(0xea1870,bytes(2*0x49d8)),pack(0xbcb9b4,0),pack(0x785d70,0),event_data([]),pack(0x78a634,0,'<B'),pack(0x5d9a40,7),pack(0x7848d8,0,'<h'),pack(0x78d7bc,1),pack(0x78d7c0,0),pack(0xe38ec1,0,'<B'),pack(0xe38dda,1,'<B'),chunk(0xe94e20,bytes(8+600)),pack(0xf53f34,0)]
 watch=[(0xbc9768,4)]
 if op<=3:w += [chunk(0xaf598c,bytes(51984*4)),chunk(0xb350dc,bytes(51984*2)),chunk(0xd05720,bytes(51984*4))];watch += [(0xaf598c,51984*4),(0xb350dc,51984*2)]
 if op==4:watch += [(0x7999d8,4000*264)]
 if op>=5:watch += [(0x785d70,4+141*124),(0x78a634,1),(0x5d9a40,4),(0xea1870,2*0x49d8),(0xe94e20,608),(0xf53f34,4)]
 w += c['writes']+[chunk(CASE,struct.pack('<I5i',op,tool,x,y,size,extra))]
 args=([x,y,size] if op==0 else [x,y] if op==1 else [extra] if op in (2,3) else [tool] if op==5 else [x,y,size,extra] if op==6 else [0 if x<0 else 0x785d74+x*124,y] if op==7 else [])
 rn=request(entries[op],w,args);rc=request(EXPORT['fixture_run'],DLL+w,[CASE]);rn['watch']=rc['watch']=[{'address':f'{a:x}','size':n} for a,n in watch];rn['max_steps']=rc['max_steps']=500000
 rn['registers']=dict(REG)
 if op==7:rn['registers']['ECX']='785d70'
 if op in (8,9):rn['registers']['ECX']=f'{0x785d74+x*124:x}'
 trace=[]
 if op==0:trace=[(0x47c880,symbol('pharaoh_map_empty_rectangle_fits'),'rectangle',[4,8,12,16,20,24,28],[8,12,16,20,24,28,32])]
 if op in (2,3):trace=[(0x46f9a0,symbol('pharaoh_map_grid_size_reaches_outside'),'bounds',[4,8,12],[8,12,16])]
 if op==4:trace=[(0x542ad0,symbol('pharaoh_sun_temple_progress'),'progress',[4,8],[8,12])]
 if op==5:trace=[(0x4e2970,symbol('pharaoh_city_warning_post'),'warning',[4],[8])]
 rn['trace']=[{'address':f'{a:x}','tag':tag,'stack_offsets':n} for a,b,tag,n,z in trace];rc['trace']=[{'address':f'{b:x}','tag':tag,'stack_offsets':z} for a,b,tag,n,z in trace]
 a=call(rn);b=call(rc);issues=[]
 if not a.get('ok') or not b.get('ok'):issues.append(['execution',a.get('error'),b.get('error'),a.get('pc'),b.get('pc')])
 else:
  if op!=5:
   av,bv=[int(t['registers']['EAX'],16) for t in [a,b]]
   if op in (2,3,4,6,9):av&=255;bv&=255
   if av!=bv:issues.append(['result',a['registers']['EAX'],b['registers']['EAX']])
  if a['memory']!=b['memory']:issues.append(['memory',[(x['address'],x['sha256'],y['sha256']) for x,y in zip(a['memory'],b['memory']) if x!=y]])
  if [(t['tag'],t['args']) for t in a['trace']]!=[(t['tag'],t['args']) for t in b['trace']]:issues.append(['logical_calls',a['trace'],b['trace']])
  for n in ['EBX','ESI','EDI','EBP']:
   if a['registers'][n]!=REG[n] or b['registers'][n]!=REG[n]:issues.append(['nonvolatile',n])
  if int(a['registers']['ESP'],16)!=SP+4+(8 if op==7 else 0) or int(b['registers']['ESP'],16)!=SP+4:issues.append(['cleanup',a['registers']['ESP'],b['registers']['ESP']])
 return {'case':c,'native_entry':f'{entries[op]:08x}','passed':not issues,'issues':issues,'native':a,'C':b}
results=[];began=time.monotonic()
for i,c in enumerate(cases):
 r=execute(c);results.append(r)
 if not r['passed']:print(json.dumps({'failed':c['label'],'issues':r['issues']})[:1800],flush=True);break
 if i%30==0:print(f'Compared {i+1}/{len(cases)} cases in {time.monotonic()-began:.1f}s',flush=True)
(D/'site-cohort-results.json').write_text(json.dumps(results));summary={'cases':len(cases),'executed':len(results),'passed':sum(x['passed'] for x in results),'entries':sorted({x['native_entry'] for x in results}),'seconds':time.monotonic()-began};(D/'site-cohort-summary.json').write_text(json.dumps(summary));print(json.dumps(summary));assert len(results)==len(cases) and all(x['passed'] for x in results)
