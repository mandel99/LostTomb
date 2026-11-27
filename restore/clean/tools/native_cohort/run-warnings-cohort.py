import pathlib,runpy,json,struct,time,os
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison'
ns=runpy.run_path(str(D/'compare-warnings.py'));run_case,chunk,pack,RESOURCE_WATCH=[ns[n] for n in ['run_case','chunk','pack','RESOURCE_WATCH']]
cases=[]
def add(label,op,tool=0,writes=None,**kw):cases.append({'label':label,'op':op,'tool':tool,'writes':writes or [],**kw})
def all_words(a,v,n=36):return chunk(a,struct.pack('<'+str(n)+'i',*([v]*n)))
def stocks(v,p=0):return all_words(0xea1870+p*0x49d8+0x48f0,v)
def trades(v,p=0):return chunk(0xea1870+p*0x49d8+0x2826,struct.pack('<36h',*([v]*36)))
def cities(kind=1,opened=1,quota=10,last=False):
 b=bytearray(61*106);i=60 if last else 0;off=i*106;b[off]=1;b[off+2]=kind;b[off+4]=1;b[off+5]=opened;b[off+42:off+78]=bytes([1]*36)
 return [chunk(0x77de00,b),all_words(0x77646c+144,quota)]
variants=[('none',[]),('editor1',[pack(0x605a3c,1)]),('editor2',[pack(0x605a3c,2)]),('stock1',[stocks(1)]),('stock_negative',[stocks(-1)]),('active1',[all_words(0x78d64c,1)]),('active_negative',[all_words(0x78d64c,-1)]),('total1',[all_words(0x78d6dc,1)]),('local',cities(0)),('open_import_disabled',cities()),('open_import1',cities()+[trades(1)]),('open_import3_player1',cities(last=True)+[pack(0xeaac28,1),trades(3,1)]),('closed_import',cities(opened=0)),('quota0',cities(quota=0)),('quota_negative',cities(quota=-1)),('stock_player1',[pack(0xeaac28,1),stocks(1,1)])]
tools=[110,111,113,114,203,233,194,177,213,204,232,32,226,51,47,231,183,228,33]
for op,tool in enumerate(tools):
 for label,w in variants:add(f'{op}:{label}',op,tool,w)
 add(f'{op}:wrong_tool',op,0)
# Native resources and signed-width/recipe boundaries.
inputs=[(14,15),(16,17),(18,19),(11,13),(22,23),(32,33),(1,9),(20,20),(30,30),(11,12),(13,34),(14,15),(8,8),(22,23),(16,17),(32,33),(12,24),(23,24)]
for op,(a,b) in enumerate(inputs):
 for r in dict.fromkeys([a,b]):
  add(f'{op}:stock{r}',op,tools[op],[pack(0xea1870+0x48f0+4*r,1)])
  add(f'{op}:active{r}',op,tools[op],[pack(0x78d64c+4*r,1)])
  add(f'{op}:total{r}',op,tools[op],[pack(0x78d6dc+4*r,1)])
# Alternate tools and exactly matching school counters.
for op,tool in [(8,222),(13,53),(17,225),(17,227),(18,30),(18,31)]:
 for label,w in variants[:12]:add(f'alt{op}:{tool}:{label}',op,tool,w)
for tool in [30,31,33]:
 for r in [34,35,36]:add(f'school{tool}:count{r}',18,tool,[pack(0x705af0+r*4,1)])
for style in range(1,7):
 for goal_slot in [0,2]:
  definition=struct.pack('<8h',1,style,0,0,0,0,0,0)
  add(f'pyramid_style{style}:slot{goal_slot}',16,183,[pack(0x784de0+2*goal_slot,1,'<h'),chunk(0x5d19d8,definition)])
# Map state shared by native and C. Tables remain the original image's data.
mapwatch=RESOURCE_WATCH+[(0xd3b4e0,8),(0xaf598c,51984*4)]
empty_map=[chunk(0xaf598c,bytes(51984*4)),chunk(0xb4e6fc,bytes(51984*2)),chunk(0xa76aec,bytes(51984))]
roadmap=bytearray(51984*4)
for y in range(100,125):
 for x in range(100,125):struct.pack_into('<I',roadmap,(228*y+x)*4,0x40)
for tool in [41,42,43,92,57,58,170,171,200,201,202,56,10,40,44,45,211,212,183,210,227,225,230,228,222,207,208,213,229,234,236,235,110,72,193,65,69,140,149]:
 add(f'road_none:{tool}',19,tool,empty_map,watch=mapwatch)
for tool in [110,72,193,140,149]:add(f'road_present:{tool}',19,tool,empty_map+[chunk(0xaf598c,roadmap)],watch=mapwatch)
for count in [2,3,4]:add(f'road_cap:{count}',19,110,empty_map+[pack(0xe950d4,count)],watch=mapwatch)
add('road_editor',19,110,empty_map+[pack(0x605a3c,1)],watch=mapwatch)
for op,tool in [(20,59),(20,172),(20,173),(25,0)]:
 for layout in range(4):
  for mask in [0,0x4000]:add(f'pattern{op}:{tool}:{layout}:{mask}',op,tool,empty_map+[all_words(0xaf598c,mask,51984)],size=layout,watch=mapwatch)
add('tower_warning_shown',20,59,empty_map+[pack(0xe94e20,1)],watch=mapwatch)
add('tower_other_tool',20,58,empty_map,watch=mapwatch)
# Full image arrays: occupied words (including negative) must retain images.
image=struct.pack('<51984I',*[(i*11+7)&0xffffffff for i in range(51984)])
backup=struct.pack('<51984I',*[(i*17+9)&0xffffffff for i in range(51984)])
ids=bytearray(51984*2);struct.pack_into('<h',ids,251*2,2);struct.pack_into('<h',ids,802*2,-1)
rows=[chunk(0xbc9f24,struct.pack('<3i',3,7,9)),chunk(0xbc9b94,struct.pack('<3i',8,7,12)),chunk(0xbc9804,struct.pack('<3i',250,500,800))]
for height in [-1,0,1,3]:add(f'restore_height{height}',21,writes=rows+[pack(0xbca2d4,height),chunk(0xb7482c,image),chunk(0xa1126c,backup),chunk(0xb4e6fc,ids)],watch=RESOURCE_WATCH+[(0xb7482c,len(image)),(0xb4e6fc,len(ids)),(0xbc9768,4)])
undowatch=RESOURCE_WATCH+[(0xbca310,4),(0xbc95f0,4),(0xbc9694,4),(0xba746c,525*2),(0xba7888,525*264),(0x7999d8,4000*264)]
for mode,bid,ids_mode in [(0,1,'empty'),(1,1,'empty'),(2,1,'empty'),(4,1,'empty'),(-1,1,'empty'),(3,0,'empty'),(3,-1,'empty'),(3,1,'empty'),(3,1,'duplicate'),(3,3999,'full'),(3,17,'late')]:
 ids=bytearray(525*2)
 if ids_mode=='duplicate':struct.pack_into('<h',ids,5*2,bid)
 if ids_mode=='full':ids=bytearray(struct.pack('<525h',*range(1,526)))
 if ids_mode=='late':ids=bytearray(struct.pack('<525h',*list(range(1,525))+[0]))
 buildings=bytearray(4000*264)
 if 0<bid<4000:buildings[bid*264:(bid+1)*264]=bytes((i*7+3)%256 for i in range(264))
 w=[pack(0xbca310,mode),pack(0xbc9800,bid),pack(0xbc95f0,123),pack(0xbc9694,1),chunk(0xba746c,ids),chunk(0xba7888,bytes([0xcd])*(525*264)),chunk(0x7999d8,buildings)]
 add(f'ferry{mode}:{bid}:{ids_mode}',22,writes=w,watch=undowatch)
# Actual prior C leaves, including the last native object and last sold slot.
for op in [23,24]:
 for index in [0,199]:
  for slot in [0,13]:
   for city_type in [0,1]:
    b=bytearray(200*98);b[index*98]=1;b[index*98+1]=1;b[index*98+24]=city_type;b[index*98+30+slot]=32
    add(f'sales{op}:{index}:{slot}:{city_type}',op,32,[chunk(0x77f748,b)],x=index)
 for resource in [0,32]:add(f'sales{op}:absent{resource}',op,resource)
results=[];began=time.monotonic()
for i,c in enumerate(cases):
 r=run_case(c);results.append(r)
 if not r['passed']:
  print(json.dumps({'failed':c['label'],'issues':r['issues']})[:1800],flush=True);break
 if i%40==0:print(f'Compared {i+1}/{len(cases)} cases in {time.monotonic()-began:.1f}s',flush=True)
(D/'warnings-cohort-cases.json').write_text(json.dumps(cases));(D/'warnings-cohort-results.json').write_text(json.dumps(results))
summary={'cases':len(cases),'executed':len(results),'passed':sum(r['passed'] for r in results),'entries':sorted({r['native_entry'] for r in results}),'seconds':time.monotonic()-began,'scope':'Actual original and linked Release C; warning posting disabled by original enabled byte; ordered real post entries, full watched state, preserved EBX/ESI/EDI/EBP and cdecl cleanup. No stub children.'}
(D/'warnings-cohort-summary.json').write_text(json.dumps(summary));print(json.dumps(summary));assert len(results)==len(cases) and all(r['passed'] for r in results)
