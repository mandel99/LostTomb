"""Offline original/C actor release, reservations and linked memberships.

Execute actual children; compare ordered owned stores, child inputs, all original
static data, FIFO memory and the saved ABI. No child results are replaced and
no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/actor-teardown-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x4b4980:'restore_actor_release'}
owned={};branches={};hits=set();edges=set();last=[None];switches={0x4b4cb8:set(),0x4b4cf7:set()};checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue['tables']:
 a=int(t['address'],16);b=bytes.fromhex(t['bytes_hex']);assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
points=set.union(*owned.values());machines=[]
calls={0x488050:1,0x4a43a0:1,0x4b3250:1,0x4b5420:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0] in switches:switches[last[0]].add(pc)
   last[0]=pc
  for p0 in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p0),end=max(p0))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  data['events'].append((t,args))
  if not data['compiled'] and t==0x488050:assert uc.reg_read(UC_X86_REG_ECX)==0xbca350
  if data['mutation']=='home' and t==0x488050:
   word(uc,0xbcd652+data['actor']*388,3);word(uc,0xbcd656+data['actor']*388,3)
  if data['mutation']=='before_clear' and t==0x4b5420:put(uc,0xbcd5f8+data['actor']*388+380,0x12345678)
  if t not in owned:
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 state['entries']={(sym('restore_actor_building_gate' if t==0x488050 else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,cfg):
 def setv(a,v,n=4):u.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 actor=cfg.get('actor',1);record=actor*388;home=cfg.get('home',1);target=cfg.get('target',2);player=cfg.get('player',0);current=cfg.get('current',actor)
 for a,n in [(0xbcd5f8,2000*388),(0x7999d8,4000*264),(0xb350dc,0xcb10*2),(0xaf598c,0xcb10*4),(0xd05720,0xcb10*4),(0x9783a4,10*28)]:
  u.mem_write(a,bytes(n))
 u.mem_write(0xbcd5f8+record,b'\xa5'*388)
 for a,v,n in [(0xeaac28,player,4),(0xd056f8,current,4),(0xbca2d8,912,4),(0xbcd602+record,cfg.get('kind',0),1),
  (0xbcd603+record,cfg.get('resource',16),1),(0xbcd600+record,0,2),(0xbcd618+record,1512,4),(0xbcd632+record,cfg.get('route',0),2),
  (0xbcd652+record,home,2),(0xbcd654+record,cfg.get('aux',0),2),(0xbcd656+record,target,2),(0xbcd666+record,cfg.get('formation',0),1),
  (0xbcd66c+record,cfg.get('amount',0),2),(0xbcd68a+record,cfg.get('group',0),1),(0xbcd714+record,cfg.get('slot',0),4),
  (0xea5f04+player*0x49d8,cfg.get('counter',10),4),(0xea6210+player*0x49d8,cfg.get('counter',10),4),(0xea61f0+player*0x49d8,cfg.get('counter',10),4)]:
  setv(a,v,n)
 h=(home*264)&0xffffffff;t=(target*264)&0xffffffff
 for a,v,n in [(0x799a02+h,actor if cfg.get('primary',True) else 99,2),(0x799a8e+h,actor if cfg.get('secondary',False) else 98,2),
  (0x799a04+h,123,2),(0x799a06+h,124,2),(0x799a08+h,125,2),(0x799a76+h,126,2),(0x799a78+h,127,2),(0x799a7a+h,128,2),
  (0x799a7e+h,cfg.get('workers',2),1),(0x799a8c+t,456,2),(0x7999de+t,24,2),(0x7999e0+t,24,2),
  (0x7999e8+t,cfg.get('building_kind',20),2),(0x799aa6+t,cfg.get('building_resource',1),2),
  (0x799adc+t,cfg.get('reservations',7),2),(0x799ade+t,cfg.get('reserved_amount',20),2),(0x799aa3+t,cfg.get('list',0),1)]:
  setv(a,v,n)
 if cfg.get('actors') is not None:
  for i,v in enumerate(cfg['actors']):setv(0x799a86+h+i*2,v,2)
 else:
  for i in range(3):setv(0x799a86+h+i*2,actor if i%2==0 else 99,2)
 if current!=actor:
  setv(0xbcd656+current*388,cfg.get('current_target',3),2);setv(0xbcd66c+current*388,cfg.get('current_amount',5),2)
  ct=cfg.get('current_target',3)*264
  for a,v,n in [(0x7999e8+ct,cfg.get('current_building_kind',71),2),(0x799aa6+ct,cfg.get('current_building_resource',1),2),
   (0x799adc+ct,10,2),(0x799ade+ct,30,2)]:setv(a,v,n)
 if cfg.get('route',0)>0:setv(0xd04e58+cfg['route']*2,actor if cfg.get('route_match',True) else 99,2)
 group=cfg.get('group',0)
 if group:setv(0xbcba4c+group*144,0x12345678)
 group=cfg.get('formation',0)
 if group:
  setv(0x77de54+group*106,77,2)
  for i in range(3):setv(0x77de5a+group*106+i*2,actor if i in cfg.get('members',[0]) else 99,2)
 slot=cfg.get('slot',0)
 if slot:setv(0x9784d8+slot*44,1,1)
 listing=cfg.get('list',0)
 if 0<listing<10:
  q=0x9783a4+listing*28;values=cfg.get('queue',[99,actor,77]);head=cfg.get('head',0);cap=8
  for a,v in [(q+4,head),(q+8,(head+len(values)-1)%cap),(q+12,len(values)),(q+16,cap),(q+20,S+0x10000)]:setv(a,v)
  for i,v in enumerate(values):setv(S+0x10000+((head+i)%cap)*4,v)
 mode=cfg.get('walk','empty')
 if mode=='head':setv(0xb350dc+1512*2,actor,2);setv(0xbcd600+record,2,2)
 elif mode=='middle':
  setv(0xb350dc+1512*2,2,2);setv(0xbcd600+2*388,actor,2);setv(0xbcd600+record,3,2)
 elif mode=='negative':setv(0xbcd618+record,-1)
 if cfg.get('gate'):
  cell=912+24*228+24;setv(0xaf598c+cell*4,0x10000)
 if cfg.get('object') is not None:
  cell=912+24*228+24;setv(0xd05720+cell*4,S+0x12000);setv(S+0x12018,cfg['object'],1)
 return [actor]

def exercise(**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),mutation=cfg.get('mutation'),actor=cfg.get('actor',1))
   args=prepare(u,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=0x4b4980 if not c else sym('restore_actor_release' if route=='named' else 'FUN_004b4980')
   try:run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S+0x10000,0x3000)))
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'queue',expected[3]==answer[3]))
 checked+=1
 if checked%50==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for cfg in [{},dict(kind=4,amount=1,building_kind=204),dict(kind=9,current=4,current_building_kind=71),
  dict(kind=85,building_kind=100,gate=True),dict(kind=77,list=1,head=7),dict(kind=38,formation=2,slot=3,aux=1,route=5,walk='middle')]:exercise(**cfg)
 if not quick:
  for kind in range(256):exercise(kind=kind)
  for kind in [5,39,62,15,16,17,38,75,90,73]:
   exercise(kind=kind,home=0);exercise(kind=kind,home=-1)
  for kind in [92,101,77]:
   for group in [1,255]:exercise(kind=kind,group=group)
  for kind in [75,90,73]:
   for workers in [0,1,255]:exercise(kind=kind,workers=workers)
  for counter in [-2147483648,-1,0,1,2147483647]:
   for kind in [57,93,100,98]:exercise(kind=kind,counter=counter,player=3)
  for primary,secondary in itertools.product([False,True],repeat=2):exercise(primary=primary,secondary=secondary)
  for kind in [4,9]:
   for building_kind in [-1,0,31,32,47,51,53,74,110,111,112,113,114,115,177,178,187,188,189,194,203,204,205,226,231,232,233,234,32767]:
    exercise(kind=kind,building_kind=building_kind,amount=1)
   for descriptor in [-32768,-1,0,*range(1,28),32767]:
    exercise(kind=kind,building_kind=204,amount=1,building_resource=descriptor,resource=16)
  for kind in [4,9]:
   exercise(kind=kind,building_kind=204,current=4,current_building_kind=71,current_building_resource=26,resource=34,current_amount=5)
   exercise(kind=kind,home=0,building_kind=204,amount=1)
   exercise(kind=kind,building_kind=204,amount=0)
   exercise(kind=kind,current=4,current_building_kind=71,current_amount=0)
   exercise(kind=kind,current=4,current_building_kind=71,current_amount=-1,reservations=0)
   exercise(kind=kind,building_kind=204,amount=1,resource=16,reservations=0,reserved_amount=0)
   exercise(kind=kind,building_kind=71,amount=-1,reservations=0,reserved_amount=0)
  for building_kind in [20,100,224]:
   for gate in [False,True]:exercise(kind=85,building_kind=building_kind,gate=gate)
  for obj in [0,1]:exercise(kind=85,building_kind=100,object=obj)
  exercise(kind=85,building_kind=100,gate=True,aux=1,mutation='home')
  for kind in [77,78]:
   for listing in [0,1,9,10,255]:exercise(kind=kind,list=listing)
   for values in [[],[1],[99,1,77],[99,77,1],[99,77,66]]:
    for head in [0,7]:exercise(kind=kind,list=1,queue=values,head=head)
  for members in [[0],[1],[2],[],[0,1,2]]:exercise(formation=255,members=members)
  for route,match in [(0,False),(-1,False),(1,False),(1,True)]:exercise(route=route,route_match=match)
  for walk in ['empty','head','middle','negative']:exercise(walk=walk,aux=1,slot=5,mutation='before_clear')
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared actor release:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges; switch counts',[len(v) for v in switches.values()],flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 table=bytes.fromhex(catalogue['tables'][0]['bytes_hex'])
 expected_switches={0x4b4cb8:set(struct.unpack('<2I',table[:8])),0x4b4cf7:set(struct.unpack('<26I',table[212:316]))}
 assert switches==expected_switches,('switch targets',switches,expected_switches)
 print('PASS complete actor-release comparison and coverage',flush=True)
 sources=['CMakeLists.txt','include/actor_teardown_model.h','runtime/menu_drag_actor_teardown.c','direct_c/verify_actor_teardown.py','metadata/actor-teardown-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets={hex(a):[hex(p) for p in sorted(v)] for a,v in switches.items()},
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual building-gate closure, FIFO removal, route and map-list unlink children. Owned ordered stores, helper inputs, all final original static data, FIFO memory and saved ABI compared.',
  'Native 488050 has fixed incoming ECX BCA350 and RET4; its typed adapter has an explicit building argument. Authenticated closure overwrites or does not consume the receiver. Incidental void registers/flags and private stack scratch excluded. No host game execution, concurrent access or UI fidelity claim.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/actor-teardown-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
