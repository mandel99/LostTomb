import os,pathlib,json,struct,re,time
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison';ns={'__name__':'root_common'};p=D/'compare-placement.py';exec(compile(p.read_text().replace('placement-fixture.dll','sites-fixture.dll'),str(p),'exec'),ns)
call,chunk,request,DLL,EXPORT,REG=[ns[n] for n in ['call','chunk','request','DLL','EXPORT','REG']];CASE=0x200f0000;SP=0x30010000
entries={10:0x4e2d30,11:0x4e3100,12:0x4e3140,13:0x4e3180,14:0x4e31c0,15:0x4e3200,16:0x4e3240,17:0x4e32e0,18:0x405b40,19:0x405b60,20:0x405b80,21:0x40f920,22:0x459fa0,23:0x46fde0,24:0x482720,25:0x485200,26:0x4b5830,27:0x4e9680,28:0x459ef0}
maptext=(D/'sites-fixture.map').read_text()
def symbol(n):
 m=re.search(r'\s_'+re.escape(n)+r'\s+([0-9A-Fa-f]{8})\s',maptext);assert m,n;return int(m[1],16)
def pack(a,v,fmt='<i'):return chunk(a,struct.pack(fmt,v))
results=[];cases=[]
def add(label,op,writes=None,**kw):cases.append({'label':label,'op':op,'writes':writes or [],**kw})
countaddr={11:0x705854,12:0x7057cc,13:0x7057c4,14:0x7057c8,15:0x7058b8}
for op,tool in [(10,110),(11,71),(12,31),(13,30),(14,33),(15,40),(16,95),(17,92)]:
 variants=[('trigger',[]),('editor1',[pack(0x605a3c,1)]),('editor257',[pack(0x605a3c,257)]),('shown',[pack(0xe94e20,1)])]
 if op in countaddr:
  variants += [('count1',[pack(countaddr[op],1)]),('count_negative',[pack(countaddr[op],-1)])]
 if op==10:
  variants += [('workers9',[pack(0xea1870+0x2c04,9)]),('no_labor',[pack(0x705eb8+tool*40,0)]),('negative_labor',[pack(0x705eb8+tool*40,-1)]),('player1',[pack(0xeaac28,1),pack(0xea1870+0x49d8+0x2c04,10)])]
 if op==16:
  variants += [('stock1',[pack(0xea1870+0x4918,1)]),('stock_negative',[pack(0xea1870+0x4918,-1)]),('player1_stock',[pack(0xeaac28,1),pack(0xea1870+0x49d8+0x4918,1)])]
 if op==17:variants += [('water_bit',[pack(0xaf598c+(113*228+111)*4,0x80)]),('other_bits',[pack(0xaf598c+(113*228+111)*4,0x80000000,'<I')])]
 for label,w in variants:add(f'warning{op}:{label}',op,w,tool=tool)
 add(f'warning{op}:other_tool',op,tool=0)
for tool in [44,45,92]:add(f'warning_alternate{tool}',15 if tool!=92 else 10,tool=tool)
for op in [18,19,20]:
 for bits in range(16):
  value=sum(1<<bit for i,bit in enumerate([1,20,21,29]) if bits&(1<<i));add(f'rule{op}:{value:x}',op,tool=value)
 for v in [-1,0x40000002]:add(f'rule{op}:extra{v}',op,tool=v)
for length in [-1,0,1,63,64,65,127,128]:
 for mask in [0,0x5a,0xff]:
  for offset in [0,1]:add(f'bytes:{length}:{mask}:{offset}',21,x=length,y=offset,tool=mask)
for t in list(range(0,12))+[89,90,91,99,100,101,102,103,104,105,106,195,196,197,223,224,225,-1,0x7fffffff,-0x80000000]:add('type:'+str(t),23,tool=t)
for t in [0,83,84,85,86,87,88,183,184,185,186,187,188,189,190,-1]:
 b=bytearray(4000*264);b[264]=1;struct.pack_into('<h',b,264+16,t);add('treasury_leaf:'+str(t),28,[chunk(0x7999d8,b)],x=1)
for label,highest,state,typ,bid in [('none',0,1,84,1),('state2',1,2,84,1),('wrongtype',1,1,1,1),('first',1,1,84,1),('last',3999,1,189,3999),('beyond',1,1,84,2),('negative',-1,1,84,1)]:
 b=bytearray(4000*264);b[bid*264]=state;struct.pack_into('<h',b,bid*264+16,typ);add('treasury_scan:'+label,22,[pack(0x78d790,highest),chunk(0x7999d8,b)])
for selector in range(3):
 for label,xy,size,radius,hit in [('none',(111,113),2,1,None),('first',(111,113),2,1,(-1,-1)),('last',(111,113),2,1,(2,2)),('clipped',(0,0),2,1,(0,0)),('empty',(229,229),2,0,None)]:
  v=0x100002 if selector==0 else 2;terrain=bytearray(51984*4)
  if selector==0:struct.pack_into('<51984I',terrain,0,*([0xffffffff]*51984))
  if hit:struct.pack_into('<I',terrain,((xy[1]+hit[1])*228+xy[0]+hit[0])*4,v)
  add(f'region{selector}:{label}',24,[chunk(0xaf598c,terrain)],tool=selector,x=xy[0],y=xy[1],size=size,extra=radius)
for v in [0,123,-1,-0x80000000]:add('undo:'+str(v),25,[pack(0xbc9728,v)])
for typ,state,dx,dy,second in [(43,1,12,12,False),(57,1,12,12,False),(99,1,12,12,False),(42,1,0,0,False),(58,1,0,0,False),(43,2,0,0,False),(43,1,13,0,False),(43,1,0,13,False),(43,1,12,12,True)]:
 b=bytearray(2000*388);o=1999*388;b[o+10]=typ;b[o+14]=state;struct.pack_into('<hh',b,o+20,(150 if second else 111)+dx,(150 if second else 113)+dy);add(f'enemy:{typ}:{state}:{dx}:{dy}:{second}',26,[chunk(0xbcd5f8,b)],size=150,extra=150)
for player in [0,1]:
 for active,editor,amount in [(1,0,100),(1,1,100),(1,2,-100),(2,0,100),(0,0,100),(1,0,-0x80000000)]:
  base=0xea1870+player*0x49d8;add(f'finance:{player}:{active}:{editor}:{amount}',27,[pack(base+2,active,'<B'),pack(base+12,0x7ffffff0),pack(base+0x2c8c,0xfffffff0,'<I'),pack(0x605a3c,editor)],tool=player,x=amount)

def execute(c):
 op=c['op'];tool=c.get('tool',0);x,y,size,extra=c.get('x',111),c.get('y',113),c.get('size',1),c.get('extra',0)
 w=[chunk(0xbca2d0,struct.pack('<12i',228,228,0,0,0,0,0,0,227,227,114,114)),chunk(0x784b64,struct.pack('<8h',*([1000]*8))),chunk(0x784cc0,struct.pack('<8i',*([1000]*8))),pack(0x605a3c,0),pack(0xeaac28,0),chunk(0xea1870,bytes(2*0x49d8)),pack(0xe38ec1,0,'<B'),pack(0xe38dda,1,'<B'),chunk(0xe94e20,bytes(608)),pack(0xf53f34,0),pack(0xd3b4e0,0x12345678),pack(0xd3b4e4,0x23456789)]
 watch=[]
 if 10<=op<=17:
  w += [chunk(0x7057c4,bytes(0xf8)),chunk(0x705ea4,bytes(237*40)),pack(0x705eb8+tool*40,1),pack(0xea1870+0x2c04,10),chunk(0xaf598c,bytes(51984*4))];watch=[(0xea1870,2*0x49d8),(0xe94e20,608),(0xf53f34,4),(0x7057c4,0xf8)]
 if op==21:w += [chunk(0x20000000,bytes((i*17+3)%256 for i in range(256)))];watch=[(0x20000000,256)]
 if op in (22,28):watch=[(0x7999d8,4000*264)]
 if op==24:watch=[(0xd3b4e0,8),(0xaf598c,51984*4)]
 if op==25:w += [chunk(0xbc9684,struct.pack('<3i',13,17,19)),pack(0xe38dca,0,'<B')];watch=[(0xbc9684,12),(0xe38dca,1)]
 if op==26:watch=[(0xbcd5f8,2000*388)]
 if op==27:watch=[(0xea1870,2*0x49d8)]
 w += c['writes']+[chunk(CASE,struct.pack('<I5i',op,tool,x,y,size,extra))]
 args=[tool] if op in list(range(10,17))+[18,19,20,23] else [tool,x,y] if op==17 else [0x20000000+y,x,tool] if op==21 else [x,y,size,extra,[0x405b40,0x405b60,0x405b80][tool]] if op==24 else [x,y,size,extra] if op==26 else [tool,x] if op==27 else [x] if op==28 else []
 rn=request(entries[op],w,args);rc=request(EXPORT['fixture_run'],DLL+w,[CASE]);rn['watch']=rc['watch']=[{'address':f'{a:x}','size':n} for a,n in watch];rn['max_steps']=rc['max_steps']=500000
 trace=[]
 if 10<=op<=17:trace=[(0x4e2970,symbol('pharaoh_city_warning_post'),'warning',[4],[8])]
 if op==24:trace=[([0x405b40,0x405b60,0x405b80][tool],symbol(['pharaoh_construction_terrain_rule_00405b40','pharaoh_construction_terrain_rule_00405b60','pharaoh_construction_terrain_rule_00405b80'][tool]),'real_rule',[4],[4])]
 rn['trace']=[{'address':f'{a:x}','tag':tag,'stack_offsets':z} for a,b,tag,z,q in trace];rc['trace']=[{'address':f'{b:x}','tag':tag,'stack_offsets':q} for a,b,tag,z,q in trace]
 a=call(rn);b=call(rc);issues=[]
 if not a.get('ok') or not b.get('ok'):issues.append(['execution',a.get('error'),b.get('error'),a.get('pc'),b.get('pc')])
 else:
  if op in [18,19,20,22,23,24,26,28]:
   av,bv=[int(t['registers']['EAX'],16) for t in [a,b]]
   if op in [18,19,20,22,23]:av&=255;bv&=255
   if av!=bv:issues.append(['result',a['registers']['EAX'],b['registers']['EAX']])
  if a['memory']!=b['memory']:issues.append(['memory',[(x['address'],x['sha256'],y['sha256']) for x,y in zip(a['memory'],b['memory']) if x!=y]])
  if [(t['tag'],t['args']) for t in a['trace']]!=[(t['tag'],t['args']) for t in b['trace']]:issues.append(['real_calls',a['trace'],b['trace']])
  for n in ['EBX','ESI','EDI','EBP']:
   if a['registers'][n]!=REG[n] or b['registers'][n]!=REG[n]:issues.append(['nonvolatile',n])
  if int(a['registers']['ESP'],16)!=SP+4 or int(b['registers']['ESP'],16)!=SP+4:issues.append(['cleanup',a['registers']['ESP'],b['registers']['ESP']])
 return {'case':c,'native_entry':f'{entries[op]:08x}','passed':not issues,'issues':issues,'native':a,'C':b}
began=time.monotonic()
for i,c in enumerate(cases):
 r=execute(c);results.append(r)
 if not r['passed']:print(json.dumps({'failed':c['label'],'issues':r['issues']})[:1500],flush=True);break
 if i%40==0:print(f'Compared {i+1}/{len(cases)} cases in {time.monotonic()-began:.1f}s',flush=True)
(D/'root-support-results.json').write_text(json.dumps(results));summary={'cases':len(cases),'executed':len(results),'passed':sum(r['passed'] for r in results),'entries':sorted({r['native_entry'] for r in results}),'seconds':time.monotonic()-began};(D/'root-support-summary.json').write_text(json.dumps(summary));print(json.dumps(summary));assert len(results)==len(cases) and all(x['passed'] for x in results)
