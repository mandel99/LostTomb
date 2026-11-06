"""Independent bounded production contracts; existing C and real frames reused."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case

ENTRY=0x488400;BUILDING=2;HOME=0x7999d8+BUILDING*264
MANAGER=0x32018010;ORIGIN=256+10+10*228
TILES=tuple(ORIGIN+x+y*228 for y in range(3)for x in range(3))
INDEX={100:0,101:1,102:9,103:2,104:3,105:4,196:5,197:7,224:8}
WORKER=3;ACTOR=0xbcd5f8+WORKER*388

def quotient(a,b):return 0 if b==0 else (abs(a*100)//abs(b))*(-1 if (a<0)!=(b<0)else 1)

def one(label,**o):
 kind=o.get('kind',100);mode=o.get('mode',0);special=o.get('special',False)
 manager=0xbca350 if o.get('global_receiver')else MANAGER
 worker=o.get('worker',0);abase=0xbcd5f8+worker*388
 c=Case(ENTRY,label,manager,[BUILDING],cleanup=4,result=0xffffffff)
 c.c['id']='c7-city-production/'+label
 c.block(HOME-2,268);c.block(MANAGER-16,64)
 c.block(0xbca2d8,0xbca370-0xbca2d8);c.put(0xbca2d8,256)
 c.put(0xbca34c,o.get('divisor',300));c.put(0xbca348,0x7e,1)
 if not o.get('global_receiver'):
  for i in range(-1,11):c.put(manager+i,0,1)
  c.put(manager+INDEX.get(kind,-1),o.get('flag',1),1)
 c.block(0x78d790,0x68);c.put(0x78d790,0);c.put(0x78d7c4,mode);c.put(0x78d7f0,0x76543210)
 for at,v in [(0xd386d4,o.get('allowed',1)),(0xbc9768,0x76543210),(0xeaac28,2)]:c.block(at,4);c.put(at,v)
 for player in (2,3):
  at=0xea6204+player*0x49d8;c.block(at,4);c.put(at,o.get('bonus',0)if player==2 else 1)
 for off,width,value in [(6,2,10),(8,2,10),(16,2,kind),(18,2,1),(60,2,o.get('stock',250)),
  (84,2,o.get('quantity',137)),(206,2,1),(204,2,o.get('stock2',180)),
  (53,1,o.get('used',0)),(64,2,0x6f32),(167,1,0x99),(180,2,worker)]:c.put(HOME+off,value,width)
 c.block(abase-2,392)
 for off,v in [(14,o.get('active',1)),(10,o.get('worker_kind',0x55)),(56,o.get('state',5))]:c.put(abase+off,v,1)
 # Real4BEEB0 reads D05720+32FB4, the SAME allowed DWORD D386D4.
 pointer=0x3201a000;c.block(pointer,32,0xa5);c.put(pointer+24,0,1)
 for row in range(3):c.block(0xa5d4cc+TILES[row*3]-1,5)
 for i,tile in enumerate(TILES):
  for base,width,value in [(0xd05720,4,pointer if o.get('pointer_special')and i==0 else 0),
   (0xaf598c,4,0x80000008|(0x10000 if special and not o.get('pointer_special')and i==0 else 0))]:
   at=base+tile*4;c.block(at,4);c.put(at,value)
  c.put(0xa5d4cc+tile,0 if i==0 else 12+i*29,1)
 c.start([(HOME+84,2),(HOME+60,2),(HOME+204,2),(HOME+180,2),(0xeaac28,4),(0xbca348,1)])
 c.put(0xbc9768,ORIGIN)
 flag=c.get(manager+INDEX.get(kind,-1),1)
 enabled=(c.get(0xd386d4)==1)if special else(bool(flag)and mode==0)
 if not enabled:return c.end(0)
 result=quotient(o.get('quantity',137),o.get('divisor',300))*8
 c.put(HOME+84,0,2)
 if c.get(HOME+18,2)and c.get(HOME+60,2):c.put(HOME+60,(c.get(HOME+60,2)-100)&0xffff,2)
 if c.get(HOME+206,2)and c.get(HOME+204,2):c.put(HOME+204,(c.get(HOME+204,2)-(25 if kind==204 else 100))&0xffff,2)
 c.endpoint('redraw',0x488e60,2,native_cleanup=8)
 ep=c.eps['redraw'];ep.update(compiled_frame_symbols=['restore_building_refresh_C_00488e60'],
  compiled_frame_dispatcher='restore_building_refresh_dispatch',compiled_frame_return_sites=[0x4884dd],registers=['ecx'])
 changes=[]
 if o.get('late_owner'):
  changes=[(HOME+84,2,999),(0xeaac28,4,3),(HOME+53,1,1)]
 if o.get('remove_terrain'):changes=[(0xaf598c+TILES[0]*4,4,0x80000008)]
 c.event('redraw',[BUILDING,0],mutations=changes)
 now_special=special and not o.get('remove_terrain')
 if now_special:
  amount=50 if c.get(HOME+53,1)else 80
  for tile in TILES:
   at=0xa5d4cc+tile;c.put(at,max(1,c.get(at,1)*(100-amount)//100),1)
 if special:
  player=c.get(0xeaac28)
  if c.get(0xea6204+player*0x49d8):result*=2;c.put(0xbca348,1,1)
  c.put(HOME+167,0,1);c.put(HOME+64,0,2)
  if worker:
   eligible=c.get(abase+14,1)!=2 and c.get(abase+10,1)==0x55 and c.get(abase+56,1)not in(6,7)
   if eligible:c.put(0x78d7f0,0);c.put(abase+14,2,1)
   c.put(HOME+180,0,2)
 c.c['actual_children']=['488030/487f80 classifier','4894a0/489470/4893c0 terrain query',
  '4883d0/4cde60 signed percentage','455c30 stock reduction','4890f0 BYTE query','489180 nine tile updates']
 c.c['controlled_boundary']='488e60 render only; observed from the actual live building-refresh C frame'
 c.c['expected_endpoint_registers']={'redraw':[{'ecx':manager}]}
 return c.end(result)

def cases(registry=None):
 specs=[('enabled-kind100',{}),('enabled-kind101-high-BYTE-flag',dict(kind=101,flag=0x80)),
  ('default-index-minus1-kind99',dict(kind=99)),('kind204-quarter-stock',dict(kind=204)),
  ('signed-negative-quantity',dict(quantity=-137)),('signed-negative-divisor',dict(divisor=-300)),
  ('zero-divisor-still-releases-stock',dict(divisor=0)),('WORD-stock-underflow',dict(stock=50,stock2=30)),
  ('disabled-BYTE-resource-flag',dict(flag=0)),('mode7-ordinary-rejected',dict(mode=7)),
  ('terrain-allowed-DWORD2-rejected',dict(special=True,allowed=2)),
  ('terrain-allowed-high-DWORD-rejected',dict(special=True,allowed=0x1000001)),
  ('terrain-mode7-production',dict(special=True,mode=7)),('terrain-owner-bonus',dict(special=True,bonus=1)),
  ('terrain-owner-high-DWORD-bonus',dict(special=True,bonus=0x100)),
  ('worker-wrong-kind',dict(special=True,worker=3,worker_kind=0x54)),
  ('worker-inactive2',dict(special=True,worker=3,active=2)),
  ('worker-state6',dict(special=True,worker=3,state=6)),('worker-state7',dict(special=True,worker=3,state=7)),
  ('worker-eligible-actual-count0-search',dict(special=True,worker=3)),
  ('signed-worker-minus1',dict(special=True,worker=-1,active=2)),
  ('redraw-mutates-owner-and-cached-quantity',dict(special=True,late_owner=True)),
  ('redraw-removes-terrain-before-refresh',dict(special=True,remove_terrain=True)),
  ('actual-pointer-map-special',dict(special=True,pointer_special=True)),
  ('global-receiver-default-divisor-high-BYTE-enabled',dict(global_receiver=True,kind=99,divisor=0x1000000)),
  ('global-receiver-default-divisor-high-BYTE-disabled',dict(global_receiver=True,kind=99,divisor=300))]
 out=[one(name,**options)for name,options in specs]
 return out
