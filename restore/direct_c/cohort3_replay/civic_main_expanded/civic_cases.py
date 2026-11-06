"""Civic main states and manager timer domains; existing C is preserved."""
from compiled_callbacks.callback_cases import Case,actor,sprite_value
ENTRY=0x48e9e0;MGR=0xbcb9c8+144;PLAYER=2;COUNTER=0xea5ee8+PLAYER*0x49d8

def one(label,frame=10,busy=0,timer=0,counter=11,reset=0,direction=1,state=18,actor_timer=0x4567,callback=None,nav=8,arrived=False,hold=False,kind=0):
 c=Case(ENTRY,label);c.c['id']='c7-civic-main/'+label;a=actor(1)
 c.block(MGR,144,0);c.block(COUNTER,4);c.put(COUNTER,counter)
 c.block(0x7090ec+kind*56,4);c.put(0x7090ec+kind*56,0x76543210)
 c.block(0x784cbd,1,0)
 for address in(0xf55ed4,0xf55eec,0xf55f04,0xf56074):c.put(address,sprite_value(address))
 c.block(0xbca2d8,4);c.put(0xbca2d8,256)
 for off,width,value in [(1,1,frame),(10,1,kind),(17,1,direction),(18,1,2),(19,1,4),(54,2,actor_timer),
  (56,1,state),(96,2,1),(98,1,0),(99,1,reset),(114,1,0x77),(384,4,0xcafe0001)]:c.put(a+off,value,width)
 for off,width,value in [(0,1,1),(42,2,0),(54,2,20),(56,2,30),(92,2,0),(126,2,busy),(128,2,timer)]:c.put(MGR+off,value,width)
 c.put(MGR+132,int(hold))
 if arrived:c.put(a+20,20,2);c.put(a+22,30,2)
 if state==7:
  for obj in range(0xeaaf68,0xeadb28,0xe0):c.block(obj,4,0)
 c.begin()
 c.put(COUNTER,(counter+1)&0xffffffff);c.put(a+114,9,1)
 frame=(frame+1)&255;c.put(a+1,frame if frame<12 else 0,1)
 c.put(a+384,0)
 if busy:
  c.put(MGR+128,(timer+1)&0xffff,2)
  signed=timer-65536 if timer&0x8000 else timer
  if signed>2000:c.put(MGR+126,0,2)
 # Native table pair at5CD6F8 (mode0) /5CDA78 (mode7), direction0,
 # is eight zero bytes in the pinned original. These are actual ROM inputs.
 c.put(a+48,20,2);c.put(a+50,30,2)
 target=1
 if state==18:c.put(a+54,0,2);c.put(a+99,0,1)
 elif state in(4,5):
  address=0x4b1d50 if state==4 else 0x4b17d0;c.endpoint('state-child',address,1)
  changes=[]
  if callback=='switch':changes=[(0xd056f8,4,2)];target=2
  elif callback=='state18':changes=[(a+56,1,18)]
  c.event('state-child',[1],mutations=changes)
 elif state==17:c.put(a+101,1,1);c.put(a+99,1,1)
 elif state==8:
  c.endpoint('state8-child',0x4b53b0,1)
  c.eps['state8-child'].update(compiled_frame_symbols=['restore_civic_C_004b53b0'],
   compiled_frame_dispatcher='restore_actor_civic_dispatch',compiled_frame_return_sites=[0x48ec4a])
  c.event('state8-child',[1]);c.put(a+54,0,2);c.put(a+99,1,1);c.put(a+1,0,1)
  if not arrived:c.put(a+56,9,1)
 elif state in(3,9,16,22,23):
  if state!=3 or not hold:
   c.put(a+54,0,2);c.put(a+99,1,1)
   if state==16:c.put(a+101,0,1)
   if state in(22,23):c.put(a+114,10,1)
   c.put(a+36,20,2);c.put(a+38,30,2);c.put(a+40,256+20+228*30)
   c.endpoint('navigation',0x4a5da0,2);c.event('navigation',[1,0x76543210],mutations=[(a+17,1,nav)])
   if nav==8:c.put(a+56,8,1)
   elif nav==9:c.endpoint('reset-route',0x4b3250,1);c.event('reset-route',[1])
   elif nav==10:c.put(a+14,2,1)
 elif state==6:c.put(a+114,10,1);c.put(a+4,0)
 elif state==7:
  c.put(a+114,10,1);c.put(a+1,0,1);value=(actor_timer+1)&0xffff;c.put(a+54,value,2)
  if (value-65536 if value&0x8000 else value)>100:c.put(a+54,0,2);c.put(a+14,2,1)
 elif state==0:c.put(a+56,8,1)
 else:raise AssertionError(('unauthored state',state))
 final_actor=actor(target);final_state=c.get(final_actor+56,1)
 if final_state==5:selected_direction=c.get(final_actor+19,1)
 else:
  selected_direction=c.get(final_actor+17,1);signed=selected_direction-256 if selected_direction>=128 else selected_direction
  if signed>=8:selected_direction=c.get(final_actor+18,1)
 signed_direction=selected_direction-256 if selected_direction>=128 else selected_direction
 orientation=signed_direction-3
 if orientation<0:orientation+=8
 if orientation>=8:orientation-=8
 c.put(0xe94e00,orientation)
 if kind in(11,12,13)and target==1:
  assert final_state not in(4,5,12,20),'Special render branch not authored'
  image={11:0xf55ed4,12:0xf55eec,13:0xf55f04}[kind]
  displacement=c.get(a+1,1)*8+orientation
  c.put(a+4,sprite_value(image)+displacement)
  if kind==13:
   pairs=((22,238),(27,253),(23,17),(0,11),(235,17),(231,255),(233,239),(0,244))
   c.put(a+384,sprite_value(0xf56074)+displacement)
   for off,value in zip((108,109),pairs[orientation]):c.put(a+off,value,1)
 if final_state==18:c.put(final_actor+4,0);c.put(final_actor+384,0)
 c.c['actual_children']=[]
 c.c['focal_domain']='Main civic graph with a live manager: state transitions, signed timer/WORD widths, frame/counter wrapping, goal tables, callbacks, orientation and logical void result.'
 return c.end()

def cases(registry=None):
 return [one(name,**options)for name,options in [
  ('state18-manager-main',{}),('frame11-wrap',dict(frame=11)),('frame255-wrap',dict(frame=255)),
  ('DWORD-counter-wrap',dict(counter=0xffffffff)),('busy-timer2000-kept',dict(busy=1,timer=2000)),
  ('busy-timer2001-cleared',dict(busy=1,timer=2001)),('busy-signed32767-overflow-cleared',dict(busy=0x100,timer=32767)),
  ('busy-negative-timer-kept',dict(busy=0x100,timer=0xfffe)),('zero-busy-does-not-touch-timer',dict(timer=2001)),
  ('mode7-goal-input-cleared',dict(reset=1)),('signed-direction-minus1',dict(direction=255)),
  ('direction8-fallback',dict(direction=8)),
  ('state0-default-to8',dict(state=0)),('state17-publishes-duty-BYTES',dict(state=17)),
  ('state6-clears-image',dict(state=6)),('state7-timer99-no-query',dict(state=7,actor_timer=99)),
  ('state7-timer100-actual-empty-selector-query',dict(state=7,actor_timer=100)),
  ('state7-signed-WORD-timer-wrap',dict(state=7,actor_timer=32767)),
  ('state4-child',dict(state=4)),('state5-child',dict(state=5)),
  ('state4-child-reloads-current-actor',dict(state=4,callback='switch')),
  ('state5-child-reloads-current-actor',dict(state=5,callback='switch')),
  ('state5-child-changes-state-before-render',dict(state=5,callback='state18')),
  ('state8-arrived-stays8',dict(state=8,arrived=True)),('state8-away-becomes9',dict(state=8)),
  ('state9-navigation8',dict(state=9,nav=8)),('state9-navigation9-reset',dict(state=9,nav=9)),
  ('state9-navigation10-delete',dict(state=9,nav=10)),('state9-navigation1-continues',dict(state=9,nav=1)),
  ('state16-navigation8',dict(state=16,nav=8)),('state22-navigation8-rate10',dict(state=22,nav=8)),
  ('state23-navigation10-delete-rate10',dict(state=23,nav=10)),('state3-held-manager',dict(state=3,hold=True)),
  ('state3-navigation8',dict(state=3,nav=8)),
  *[(f'kind{kind}-state{state}',dict(kind=kind,state=state))for kind in(11,12,13)for state in(18,17,9)]]]
