from pathlib import Path
import json,sys,hashlib,struct
H=Path('artifacts/map_refresh_postpass8_phase');R=Path.cwd();sys.path.insert(0,str(R/'build/offline-verification-deps'))
from unicorn import *
from unicorn.x86_const import *
f=json.loads((H/'postpass8-native.json').read_text());image=(R/'build/smoke-game/Pharaoh.exe').read_bytes();assert hashlib.sha256(image).hexdigest()==f['native_sha256'];u16=lambda a:struct.unpack_from('<H',image,a)[0];u32=lambda a:struct.unpack_from('<I',image,a)[0];o=u32(60)+24;t=o+u16(o-4);base=u32(o+28);v=Uc(UC_ARCH_X86,UC_MODE_32);v.mem_map(base,0x1000000)
for i in range(u16(o-18)):
 q=t+i*40;v.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for n in ['00484be0']:
 for it in f['functions'][n]['assembly']['items']:assert bytes(v.mem_read(int(it['address'],16),len(bytes.fromhex(it['bytes']))))==bytes.fromhex(it['bytes'])
v.mem_map(0x33000000,0x10000);sp=0x3300f000;put=lambda a,x:v.mem_write(a,struct.pack('<I',x));r=lambda a:int.from_bytes(v.mem_read(a,4),'little');rows=[];seen=set();variants={};total=0
for mask in range(256):
 for rot in [0,2,4,6]:
  for anim in [0,1,255]:
   raw=bytes.fromhex(f['real_record_inputs']['primary']['bytes']);v.mem_write(0x5ca658,raw);put(0xbca2e0,rot)
   for i in range(8):put(0xbc97c0+i*4,not(mask>>i&1))
   for i,x in enumerate([0x33000000,0x5ca658,48,anim,0]):put(sp+i*4,x)
   v.reg_write(UC_X86_REG_ESP,sp);v.emu_start(0x484be0,0x33000000,count=10000);ordinal=v.reg_read(UC_X86_REG_EAX);assert 1<=ordinal<=47 and raw[(ordinal-1)*16+13]>0
   seen.add(ordinal);total+=1
   if anim==0:rows.append([mask,rot,ordinal,r(0xbc9748),raw[(ordinal-1)*16+13]]);variants.setdefault(str(r(0xbc9748)),[mask,rot])
report={'native_sha256':f['native_sha256'],'original_game_executed':False,'native_only_cases':total,'C_cases':0,'all_256_physical_neighbor_masks':True,'rotations':[0,2,4,6],'animation_values':[0,1,255],'reachable_ordinals':sorted(seen),'zero_frame_sentinel_reachable':False,'rows':rows,'first_input_by_variant':variants,'scope':'Native-only reachability proof over actual primary PE records, not whole47A220/C equivalence.'}
(H/'primary-record-reachability.json').write_text(json.dumps(report,indent=2)+'\n')
with (H/'postpass8_records.h').open('w') as out:
 out.write('/* Exact pinned PE record bytes; fixture inputs only. */\n')
 for name,item in f['real_record_inputs'].items():
  data=bytes.fromhex(item['bytes']);out.write('static const unsigned char '+name+'_records['+str(len(data))+']={\n');out.write(','.join('0x%02x'%x for x in data));out.write('};\n')
print(total,len(seen),sorted(map(int,variants)))
