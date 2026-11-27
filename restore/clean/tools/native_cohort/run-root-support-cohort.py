import os,pathlib,json,struct,re,time,sys
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison';ns={'__name__':'root_common'};p=D/'compare-placement.py';exec(compile(p.read_text().replace('placement-fixture.dll','sites-fixture.dll'),str(p),'exec'),ns)
call,chunk,request,DLL,EXPORT,REG=[ns[n] for n in ['call','chunk','request','DLL','EXPORT','REG']];CASE=0x200f0000;SP=0x30010000
entries={10:0x4e2d30,11:0x4e3100,12:0x4e3140,13:0x4e3180,14:0x4e31c0,15:0x4e3200,16:0x4e3240,17:0x4e32e0,18:0x405b40,19:0x405b60,20:0x405b80,21:0x40f920,22:0x459fa0,23:0x46fde0,24:0x482720,25:0x485200,26:0x4b5830,27:0x4e9680,28:0x459ef0,29:0x43e670,36:0x4829b0,30:0x440700,31:0x440850,32:0x440b10,33:0x440ed0,34:0x43f410,35:0x441130,37:0x447530,38:0x4e3330,39:0x488140,40:0x489050,41:0x489110,42:0x4b9ee0,43:0x51ce90,44:0x457ac0,45:0x457a40}
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

# Projection x indexes rows; y indexes columns in the native lookup owner.
for bounds in [(167,289,84,145),(0,456,0,230),(100,140,80,120),(1,2,1,2),(-100,500,-30,240)]:
 for cam in [(114,218),(1,1),(220,450),(-3,-7)]:
  for width,height in [(73,111),(40,40),(0,0)]:
   lo_r,hi_r,lo_c,hi_c=bounds;cx,cy=cam
   add('origin:'+str((bounds,cam,width,height)),29,[chunk(0x70e420,struct.pack('<4i',hi_r,hi_c,lo_r,lo_c)),pack(0x70e464,14),pack(0x70e460,50),pack(0x70e478,cx),pack(0x70e474,cy)],x=width,y=height)
for x,y in [(-2,-1),(0,0),(111,113),(227,227),(229,3)]:
 for size,radius in [(1,0),(2,1),(0,0),(-1,0),(1,-1),(4,2)]:
  for required in [-1,0,1,4,-2147483648]:
   add('count:'+str((x,y,size,radius,required)),36,[pack(CASE+24,required)],x=x,y=y,size=size,extra=radius,tool=1,fill=1 if required%2 else 0,required=required)
for dims in [(0,0),(1,1),(3,4),(228,228)]:
 for bits,fill in [(0,1),(2,1),(0x80000000,0x80000000)]:
  add('count_dims:'+str((dims,bits)),36,[pack(CASE+24,2),chunk(0xbca2d0,struct.pack('<2i',*dims))],x=0,y=0,size=4,extra=0,tool=bits if bits<0x80000000 else -2147483648,fill=fill,required=2)
venue_contract=json.loads((D/'entertainment-fixture-contract.json').read_bytes())
for group in ['preview_success_cases','rejected_cases','site_cases','slot_cases','bounded_successful_commit_cases']:
 for i,raw in enumerate(venue_contract[group]):
  case=dict(raw.get('case',raw));case.update({k:v for k,v in raw.items() if k!='case'});case.update(label='venue:'+group+':'+str(i),writes=[]);case.update(festival_exists=True) if group=='rejected_cases' and case['op']==33 else None;cases.append(case)
for view in [2,4,6]:
 for op,kind,extent,cross in [(30,31,2,(1,1)),(31,30,3,(1,2)),(32,33,4,(1,1)),(33,209,5,(2,2))]:
  add('venue:commit_view:'+str((op,view)),op,tool=kind,x=90,y=80,size=0,view=view,roads='cross'+str(cross)+',extent'+str(extent))
if '--entertainment-only' in sys.argv:cases=[c for c in cases if 30<=c['op']<=35]
food_contract=json.loads((D/'food-supply-fixture-contract.json').read_bytes())
for raw in food_contract['priority_cases']:
 c=dict(raw['case']);c.update(label='food:'+raw['name'],writes=[],food_spec=raw);cases.append(c)
for label,records,query in [
 ('empty',[],(100,100)),('first',[(1,1,0,1,1,100,100)],(100,100)),('last',[(49,1,0,1,1,106,94)],(100,100)),
 ('state2',[(1,2,0,1,1,100,100)],(100,100)),('skip42',[(1,1,1,1,1,100,100)],(100,100)),('gate69',[(1,1,0,0,1,100,100)],(100,100)),('members0',[(1,1,0,1,0,100,100)],(100,100)),
 ('far7',[(1,1,0,1,1,107,100)],(100,100)),('negativeXY',[(1,1,0,1,1,-6,6)],(0,0)),('two_live_scratch',[(1,1,0,1,1,101,99),(2,1,0,1,1,120,113)],(100,100)),
 ('boundary50',[(50,1,0,1,1,100,100)],(100,100)),('INTMIN',[(1,1,0,1,1,32767,-32768)],(-2147483648,-2147483648)),('INTMAX',[(1,1,0,1,1,-32768,32767)],(2147483647,2147483647))
 ]+[(str((dx,dy)),[(1,1,0,1,1,100+dx,100+dy)],(100,100)) for dx in [-6,0,6] for dy in [-6,0,6]]:
 b=bytearray(51*144);b[:144]=bytes([0xa5])*144
 b[0]=1;b[66]=0;b[105]=1;b[40]=1;struct.pack_into('<hh',b,46,query[0] if -32768<=query[0]<=32767 else 100,query[1] if -32768<=query[1]<=32767 else 100);struct.pack_into('<H',b,78,0xbcbc);b[118]=0xbc
 for bid,state,skip42,gate69,members,gx,gy in records:
  off=bid*144;b[off]=state;b[off+66]=skip42;b[off+105]=gate69;b[off+40]=members;struct.pack_into('<hh',b,off+46,gx,gy);struct.pack_into('<H',b,off+78,0xeeee);b[off+118]=0xdd
 add('alert:'+label,42,[chunk(0xbcb9c8,b),chunk(0xd3b4f8,bytes.fromhex('78563412efcdab09'))],x=query[0],y=query[1])
for label,tiles in [('closed',[]),('blocked_start',[(100,100,0xfc)]),('isolated',[(100,100,0)]),('cross',[(100,100,0),(100,99,0),(101,100,0),(100,101,0),(99,100,0)]),('deferred_start',[(100,100,0xfe),(101,100,0)]),('deferred_neighbor',[(100,100,0),(101,100,0xfe),(102,100,0)]),('entry_FC_neighbor',[(100,100,0),(101,100,0xfc),(102,100,0)]),('FD_barrier',[(100,100,0),(101,100,0xfd),(102,100,0)])]:
 add('water:'+label,43,water_spec={'tiles':tiles},x=100,y=100)
for step in [-228,1,228,-1]:
 tiles=[(100+(i if abs(step)==1 else 0)*(1 if step>0 else -1),100+(i if abs(step)==228 else 0)*(1 if step>0 else -1),0xfe if i==3 else 0) for i in range(8)]
 add('water:corridor:'+str(step),43,water_spec={'tiles':tiles},x=100,y=100)
for label,query,blocked,water in [('true',(100,100),False,True),('far',(110,110),False,True),('no_terrain',(100,100),False,False),('blocked_entry',(100,100),True,True)]:
 add('dock:'+label,44,water_spec={'square':4,'blocked':blocked,'terrain_water':water},x=query[0],y=query[1],expected_eax=1 if label=='true' else 0)
for highest in [-1,0,7]:add('dock_refresh:'+str(highest),45,water_spec={'square':4,'terrain_water':True,'highest':highest},x=100,y=100)
if '--water-only' in sys.argv:cases=[c for c in cases if 43<=c['op']<=45]
if '--alert-only' in sys.argv:cases=[c for c in cases if c['op']==42]
if '--food-only' in sys.argv:cases=[c for c in cases if 37<=c['op']<=41 and c['food_spec']['expected_C_status']==0]
if '--food-fault-only' in sys.argv:cases=[c for c in cases if 37<=c['op']<=41 and c['food_spec']['expected_C_status']!=0]
if '--navigation-only' in sys.argv:cases=[c for c in cases if c['op']==29]
if '--region-only' in sys.argv:cases=[c for c in cases if c['op']==36]
case_label=next((v.split('=',1)[1] for v in sys.argv if v.startswith('--case-label=')),None)
if case_label is not None:cases=[c for c in cases if c['label']==case_label]
output_stem='water' if '--water-only' in sys.argv else 'alert' if '--alert-only' in sys.argv else 'food-fault' if '--food-fault-only' in sys.argv else 'food' if '--food-only' in sys.argv else 'entertainment-extra' if case_label is not None else 'entertainment' if '--entertainment-only' in sys.argv else 'region' if '--region-only' in sys.argv else 'navigation' if '--navigation-only' in sys.argv else 'root-support'


def venue_seed(c):
 w=[];seed=venue_contract['minimal_valid_seed']
 for a,v in seed['dword'].items():w.append(pack(int(a,16),v,'<I'))
 terrain=bytearray(51984*4);zero_byte=bytes(51984);images=bytearray(51984*4);buildings=bytearray(4000*264)
 kind=c.get('tool',31);extent={31:2,30:3,33:4,209:5}.get(kind,5)
 if c.get('roads','none')!='none':
  rx,ry=(1,2) if kind==30 else (2,2) if kind==209 else (1,1)
  for dx in range(-5,extent+5):
   for dy in range(-5,extent+5):
    if dx==rx or dy==ry:struct.pack_into('<I',terrain,(1000+(80+dy)*228+90+dx)*4,0x40)
 if c.get('free_records') is not None:
  for bid in range(c['free_records']+1,4000):buildings[bid*264]=1
 for a,b in [(0xaf598c,terrain),(0xb4e6fc,bytes(51984*2)),(0xd05720,bytes(51984*4)),(0xacf85c,zero_byte),(0xae8e7c,zero_byte),(0xb285cc,zero_byte),(0xb67d1c,zero_byte),(0xb7482c,images),(0x7999d8,buildings),(0xba746c,bytes(0x41a)),(0xba7888,bytes(0x21d68)),(0x9de62c,terrain),(0x9d1b1c,zero_byte),(0x9b84fc,zero_byte),(0x9ab9ec,zero_byte)]:w.append(chunk(a,b))
 backup=bytearray(51984*4)
 for dx in range(extent):
  for dy in range(extent):struct.pack_into('<I',backup,(1000+(80+dy)*228+90+dx)*4,0x123456)
 w.append(chunk(0xa1126c,backup))
 starts=[0]*228;ends=[0]*228;offset=[0]*228
 for y in range(200):offset[y]=1000+y*228
 for y in range(80,80+extent):starts[y]=90;ends[y]=90+extent;offset[y]=1000+y*228+90
 for a,v in [(0xbc9f24,starts),(0xbc9b94,ends),(0xbc9804,offset)]:w.append(chunk(a,struct.pack('<228i',*v)))
 w.append(pack(0xe38ec1,0,'<B'));w.append(pack(0xe38dda,0,'<B'))
 w.append(pack(0xbca2e0,c.get('view',0)))
 if c.get('festival_exists'):w += [pack(0xea6010,90),pack(0xea6014,80)]
 for group in range(1000):w.append(pack(0xf55bb4+group*8,100000+group*16,'<I'))
 occupancy=[0]*25
 if c.get('occupancy','')=='all25 one':occupancy=[1]*25
 elif 'except' in c.get('occupancy',''):occupancy=[1]*25;occupancy[0]=occupancy[5]=0
 w.append(chunk(0x775614,struct.pack('<25I',*occupancy)));w.append(pack(0xbc971c,0x12345678))
 w.append(chunk(0x200f0100,struct.pack('<2i',-77,-88)));w.append(pack(0xea6084,7,'<H'))
 return w

def food_seed(c):
 seed=food_contract['base_seed'];spec=c['food_spec'];profile=food_contract['profiles'][spec['profile']];w=[]
 for r in seed['clear_regions']:w.append(chunk(int(r['address'],16),bytes(r['bytes'])))
 w += [chunk(0xea1870,bytes(2*0x49d8)),chunk(0x705ea4,bytes(237*40)),chunk(0x77646c,bytes(4*36*61))]
 for kind,fmt in [('dword','<I'),('word','<H'),('byte','<B')]:
  for a,v in seed[kind].items():w.append(pack(int(a,16),v,fmt))
 w.append(chunk(0xea4096,bytes(36*2)));w.append(chunk(0xe94e20,bytes(608)));w.append(chunk(0xf53f34,bytes(20)))
 soil=spec.get('soil_override',spec.get('fill_soil',profile.get('fill_soil')))
 if soil is not None:w.append(chunk(0xa5d4cc,bytes([soil])*51984))
 for r in profile['patches']+spec['patches']:
  width=r['width'];w.append(chunk(int(r['address'],16),(r['value']&((1<<(8*width))-1)).to_bytes(width,'little')))
 return w

def water_seed(c):
 spec=c['water_spec'];tiles=bytearray([0xff])*51984
 if spec.get('full_grid'):
  for row in range(1,227):tiles[row*228+1:row*228+227]=bytes(226)
 for x,y,v in spec.get('tiles',[]):tiles[y*228+x]=v
 if 'square' in spec:
  for y in range(100-spec['square'],101+spec['square']):
   for x in range(100-spec['square'],101+spec['square']):tiles[y*228+x]=0
  if spec.get('blocked'):tiles[100*228+100]=0xfc
 b=bytearray(4000*264)
 for bid,state,flag,kind,x,y in [(0,1,0,75,100,100),(1,1,0,75,100,100),(2,1,0,192,110,110),(3,1,0,76,100,100),(4,2,0,75,100,100),(5,1,1,75,100,100),(6,1,0,192,100,100),(7,1,0,75,110,110),(8,1,0,75,100,100)]:
  off=bid*264;b[off]=state;b[off+5]=flag;struct.pack_into('<hh',b,off+6,x,y);struct.pack_into('<h',b,off+16,kind);b[off+0x35]=0x93
 return [chunk(0xed4140,tiles),chunk(0xeed760,bytes.fromhex('a3a4a5a6')*51984),chunk(0xeae010,bytes([0x7b])*51984),chunk(0xf203a0,bytes.fromhex('c1c2c3c4')*51984),chunk(0xeae004,struct.pack('<3I',0x12345678,0x4567,0x2345)),chunk(0x784c8c,struct.pack('<hh',100,100)),pack(0x78d790,spec.get('highest',7)),chunk(0x7999d8,b),chunk(0xaf598c,struct.pack('<I',4 if spec.get('terrain_water') else 0)*51984)]

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
 if op==29:watch=[(0x70e3d8,8)]
 if op==42:watch=[(0xbcb9c8,51*144),(0xd3b4f8,8)]+[(0xbcb9c8+i*144+78,2) for i in [0,1,49,50]]+[(0xbcb9c8+i*144+118,1) for i in [0,1,49,50]]
 if op==36:w += [chunk(0xaf598c,struct.pack("<I",c.get("fill",0))*51984)];watch=[(0xd3b4e0,8)]
 if 30<=op<=35:
  w+=venue_seed(c);watch=[(int(v['address'],16),v['bytes']) for v in venue_contract['watch_regions']]
 if 37<=op<=41:
  w+=food_seed(c);watch=[(int(v['address'],16),v['bytes']) for v in food_contract['observe']['write_watch']]
 if 43<=op<=45:
  w+=water_seed(c);watch=[(0xeae004,12),(0xeed760,51984*4),(0xeae010,51984),(0xf203a0,51984*4),(0x7999d8,4000*264),(0xeed760+(100*228+100)*4,16),(0xeae010+100*228+100,4),(0x7999d8,9*264)]+[(0x7999d8+i*264+0x35,1) for i in range(9)]
 w += c['writes']+[chunk(CASE,struct.pack('<I5i',op,tool,x,y,size,extra))]
 args=[tool] if op in list(range(10,17))+[18,19,20,23] else [tool,x,y] if op==17 else [0x20000000+y,x,tool] if op==21 else [x,y,size,extra,[0x405b40,0x405b60,0x405b80][tool]] if op==24 else [x,y,size,extra] if op==26 else [tool,x] if op==27 else [x] if op==28 else [x,y] if op==29 else [x,y,size,extra,tool,c["required"]] if op==36 else []
 if 30<=op<=33:args=[size,0x12345678,0x87654321,x,y]
 if op==34:args=[tool]
 if op==35:args=[x,y,0x200f0100,0x200f0104,tool,extra]
 if op==37:args=[]
 if op==38:args=[tool]
 if op==39:args=[tool,size]
 if op==40:args=[x,y,size,extra]
 if op==41:args=[tool]
 if op in [43,44]:args=[x,y]
 if op==42:args=[x,y]
 rn=request(entries[op],w,args);rc=request(EXPORT['fixture_run'],DLL+w,[CASE]);rn['watch']=rc['watch']=[{'address':f'{a:x}','size':n} for a,n in watch];rn['max_steps']=rc['max_steps']=2000000 if 30<=op<=35 else 4000000 if 43<=op<=45 else 500000
 if c.get('water_spec',{}).get('full_grid'):
  rn['max_steps']=rc['max_steps']=12000000;rn['timeout_ms']=rc['timeout_ms']=60000
 if 37<=op<=41:
  rn['registers']['ECX']='00bca350'
  for q in [rn,rc]:q['registers'].update(FPUControlWord='0000037f',FPUStatusWord='00000000',FPUTagWord='0000ffff')
 if 37<=op<=41:rc['watch']=rc['watch']+[{'address':'200f0200','size':4}]
 trace=[]
 if 10<=op<=17:trace=[(0x4e2970,symbol('pharaoh_city_warning_post'),'warning',[4],[8])]
 if op==24:trace=[([0x405b40,0x405b60,0x405b80][tool],symbol(['pharaoh_construction_terrain_rule_00405b40','pharaoh_construction_terrain_rule_00405b60','pharaoh_construction_terrain_rule_00405b80'][tool]),'real_rule',[4],[4])]
 if op==38:trace=[(0x4e2970,symbol('pharaoh_city_warning_post'),'warning',[4],[8])]
 rn['trace']=[{'address':f'{a:x}','tag':tag,'stack_offsets':z} for a,b,tag,z,q in trace];rc['trace']=[{'address':f'{b:x}','tag':tag,'stack_offsets':q} for a,b,tag,z,q in trace]
 a=call(rn);b=call(rc);issues=[]
 if 37<=op<=41:
  status_memory=b['memory'].pop();b['checked_status']=int.from_bytes(bytes.fromhex(status_memory['hex']),'little')
  if b['checked_status']!=c['food_spec']['expected_C_status']:issues.append(['C_status',b['checked_status']])
  if c['food_spec']['expected_C_status']!=0:return {'case':c,'native_entry':f'{entries[op]:08x}','passed':not issues,'qualification':'C checked-IDIV status only; pcode emulator may omit hardware divide overflow','native':a,'C':b,'issues':issues}
 if not a.get('ok') or not b.get('ok'):issues.append(['execution',a.get('error'),b.get('error'),a.get('pc'),b.get('pc')])
 else:
  if op in [18,19,20,22,23,24,26,28,30,31,32,33,34,35,36,37,39,40,41,43,44]:
   av,bv=[int(t['registers']['EAX'],16) for t in [a,b]]
   if op in [18,19,20,22,23]:av&=255;bv&=255
   if av!=bv:issues.append(['result',a['registers']['EAX'],b['registers']['EAX']])
  if 'expected_eax' in c and int(a['registers']['EAX'],16)!=c['expected_eax']:issues.append(['intended_native_result',a['registers']['EAX'],c['expected_eax']])
  if 37<=op<=41:
   spec=c['food_spec'];expected=spec.get('expected_normal_value')
   if expected is not None and int(a['registers']['EAX'],16)!=(expected&0xffffffff):issues.append(['expected_food_value',a['registers']['EAX'],expected])
   if 'expected_post94_calls' in spec and sum(t['tag']=='warning' and t['args']==[94] for t in a['trace'])!=spec['expected_post94_calls']:issues.append(['expected_post94_calls',a['trace']])
  if a['memory']!=b['memory']:issues.append(['memory',[(x['address'],x['sha256'],y['sha256']) for x,y in zip(a['memory'],b['memory']) if x!=y]])
  if [(t['tag'],t['args']) for t in a['trace']]!=[(t['tag'],t['args']) for t in b['trace']]:issues.append(['real_calls',a['trace'],b['trace']])
  for n in ['EBX','ESI','EDI','EBP']:
   if a['registers'][n]!=REG[n] or b['registers'][n]!=REG[n]:issues.append(['nonvolatile',n])
  if int(a['registers']['ESP'],16)!=SP+4+({39:8,40:16,41:4}.get(op,0)) or int(b['registers']['ESP'],16)!=SP+4:issues.append(['cleanup',a['registers']['ESP'],b['registers']['ESP']])
 return {'case':c,'native_entry':f'{entries[op]:08x}','passed':not issues,'issues':issues,'native':a,'C':b}
began=time.monotonic()
for i,c in enumerate(cases):
 r=execute(c);results.append(r)
 if not r['passed']:print(json.dumps({'failed':c['label'],'issues':r['issues']})[:1500],flush=True);break
 if i%40==0:print(f'Compared {i+1}/{len(cases)} cases in {time.monotonic()-began:.1f}s',flush=True)
(D/(output_stem+'-results.json')).write_text(json.dumps(results));summary={'cases':len(cases),'executed':len(results),'passed':sum(r['passed'] for r in results),'entries':sorted({r['native_entry'] for r in results}),'seconds':time.monotonic()-began};(D/(output_stem+'-summary.json')).write_text(json.dumps(summary));print(json.dumps(summary));assert len(results)==len(cases) and all(x['passed'] for x in results)
