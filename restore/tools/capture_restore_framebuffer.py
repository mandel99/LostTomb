"""Read only the internal RGB16 buffer of an explicitly identified restored C child.
The caller must supply the child and its owned debugger PID. No screen/UI capture,
input, attaching debugger, injection, suspension or process mutation is performed.
"""
from pathlib import Path
import argparse,ctypes as c,ctypes.wintypes as w,hashlib,json,re,struct
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser();parser.add_argument('--pid',type=int,required=True);parser.add_argument('--debugger-pid',type=int,required=True)
parser.add_argument('--output',type=Path,default=ROOT/'artifacts/startup-menu-frame.bmp')
parser.add_argument('--manifest',type=Path,default=ROOT/'metadata/startup-menu-frame.json')
parser.add_argument('--completed-probe',action='store_true')
parser.add_argument('--completed-mode',choices=['name','family','options','dialog','return','campaign','mission','load-save','load-save-selected','high-scores','high-scores-scrolled','high-scores-dragged','high-scores-return','city','city-gameplay'],default='name');args=parser.parse_args()
k=c.WinDLL('kernel32',use_last_error=True)
k.OpenProcess.argtypes=[w.DWORD,w.BOOL,w.DWORD];k.OpenProcess.restype=w.HANDLE
k.CloseHandle.argtypes=[w.HANDLE];k.CloseHandle.restype=w.BOOL
k.QueryFullProcessImageNameW.argtypes=[w.HANDLE,w.DWORD,w.LPWSTR,c.POINTER(w.DWORD)];k.QueryFullProcessImageNameW.restype=w.BOOL
k.ReadProcessMemory.argtypes=[w.HANDLE,c.c_void_p,c.c_void_p,c.c_size_t,c.POINTER(c.c_size_t)];k.ReadProcessMemory.restype=w.BOOL
class Entry(c.Structure):
 _fields_=[('size',w.DWORD),('usage',w.DWORD),('pid',w.DWORD),('heap',c.c_size_t),('module',w.DWORD),('threads',w.DWORD),('parent',w.DWORD),('priority',w.LONG),('flags',w.DWORD),('exe',w.WCHAR*260)]
k.CreateToolhelp32Snapshot.argtypes=[w.DWORD,w.DWORD];k.CreateToolhelp32Snapshot.restype=w.HANDLE
k.Process32FirstW.argtypes=[w.HANDLE,c.POINTER(Entry)];k.Process32FirstW.restype=w.BOOL
k.Process32NextW.argtypes=[w.HANDLE,c.POINTER(Entry)];k.Process32NextW.restype=w.BOOL
snapshot=k.CreateToolhelp32Snapshot(2,0);entry=Entry();entry.size=c.sizeof(entry);found=False
try:
 okay=k.Process32FirstW(snapshot,c.byref(entry))
 while okay:
  if entry.pid==args.pid:
   assert entry.parent==args.debugger_pid,'Child parent differs';found=True;break
  okay=k.Process32NextW(snapshot,c.byref(entry))
finally:k.CloseHandle(snapshot)
assert found,'Requested child no longer exists'
handles=[]
try:
 for pid,name in [(args.pid,'restore_link_check.exe'),(args.debugger_pid,'restore_debug.exe')]:
  handle=k.OpenProcess(0x410,False,pid);assert handle,c.get_last_error();handles.append(handle)
  text=c.create_unicode_buffer(32768);length=w.DWORD(len(text))
  assert k.QueryFullProcessImageNameW(handle,0,text,c.byref(length))
  assert Path(text.value).resolve()==(ROOT/'build/c-only-debug/Debug'/name).resolve(),'Unexpected process image'
 process=handles[0]
 def read(a,n):
  output=c.create_string_buffer(n);length=c.c_size_t()
  assert k.ReadProcessMemory(process,a,output,n,c.byref(length)) and length.value==n,(hex(a),n,c.get_last_error())
  return output.raw
 def word(a):return struct.unpack('<I',read(a,4))[0]
 if args.completed_probe:
  linkmap=(ROOT/'build/c-only-debug/Debug/restore_link_check.map').read_text(errors='replace')
  symbol={'name':'_restore_probe_frame','family':'_restore_probe_family_frame','options':'_restore_probe_menu_frame','dialog':'_restore_probe_dialog_frame','return':'_restore_probe_return_frame','campaign':'_restore_probe_campaign_frame','mission':'_restore_probe_mission_frame','load-save':'_restore_probe_load_save_frame','load-save-selected':'_restore_probe_load_save_selected_frame','high-scores':'_restore_probe_high_scores_frame','high-scores-scrolled':'_restore_probe_high_scores_scrolled_frame','high-scores-dragged':'_restore_probe_high_scores_dragged_frame','high-scores-return':'_restore_probe_high_scores_return_frame','city':'_restore_probe_city_frame','city-gameplay':'_restore_probe_gameplay_frame'}[args.completed_mode]
  found=re.findall(r'\S+\s+'+symbol+r'\s+([0-9a-fA-F]{8})\s+',linkmap);assert len(found)==1,'Expected one C probe frame symbol'
  magic,ready,width,height,pitch,format,mode,pixels=struct.unpack('<8I',read(int(found[0],16),32))
  assert magic==0x52504652 and ready==1,'Completed C probe frame is not ready'
 else:
  width,height,pitch,pixels,format=[word(a) for a in [0xe38d9c,0xe38d98,0xe38d94,0xe92e18,0xe3914c]]
  mode=read(0xe38dda,1)[0]
 assert 0<width<=4096 and 0<height<=4096 and width*2<=pitch<=16384 and pixels
 raw=read(pixels,pitch*height);live_mode=read(0xe38dda,1)[0]
 if not args.completed_probe:assert mode==live_mode,'Screen mode changed during read'
 name_entry=None
 if mode==0x36:
  name_entry={'text':read(0xe38e74,32).split(b'\0',1)[0].decode('cp1250',errors='replace'),'active_slot':word(0xe20990),'cursor':word(0xe207f0),'length':word(0xe208a4),'text_pointer':hex(word(0xe207a0)),'font':word(0xe20944)}
 output=bytearray(width*height*4);colors=set()
 for y in range(height):
  for x in range(width):
   v=struct.unpack_from('<H',raw,y*pitch+x*2)[0];colors.add(v)
   r=(v>>(10 if format==0x555 else 11))&31;g=(v>>5)&(31 if format==0x555 else 63);b=v&31
   off=(y*width+x)*4;output[off:off+4]=bytes([b*255//31,g*255//(31 if format==0x555 else 63),r*255//31,0])
 bmp=struct.pack('<2sIHHI',b'BM',54+len(output),0,0,54)+struct.pack('<IiiHHIIiiII',40,width,-height,1,32,0,len(output),0,0,0,0)+output
 path=args.output.resolve();path.write_bytes(bmp)
 report={'method':'ReadProcessMemory of the restored C internal framebuffer; no desktop capture or UI automation','child_pid':args.pid,'debugger_pid':args.debugger_pid,'mode':mode,'width':width,'height':height,'pitch':pitch,'pixels':hex(pixels),'format':hex(format),'unique_16bit_colors':len(colors),'raw_sha256':hashlib.sha256(raw).hexdigest(),'image':str(path),'child_exe_sha256':hashlib.sha256((ROOT/'build/c-only-debug/Debug/restore_link_check.exe').read_bytes()).hexdigest()}
 report['completed_frame_copy']=args.completed_probe
 report['current_mode']=live_mode
 if args.completed_probe and args.completed_mode in ['city','city-gameplay']:
  # These are live diagnostic values, not a reconstructed state at frame-copy time.
  watched=[0xe94e0c,0x6059e8,0xbca300,0x78d7d4,0x78d7d8,0x78d7dc,0xe38e94,0xf56f84,
           0xbca2bc,0xbca2c0,0xe94dc4,0xe94dc8,0xe94dcc,0xe94dd0,0x70e460,0x70e464,
           0xf53f60,0xf53f64,0xf53f78,0xf53f7c,
           0x6fd5d8,0x6de650,0x6fd368,0x6f9320,0x6f5648,0x6de9e0,0x6e9188,0x6de548,
           0x6f9370,0x6f5698,0x6dea30,0x6e91d8]
  report['live_render_state_at_capture']={hex(a):{'raw4':read(a,4).hex(),'u32':word(a)} for a in watched}
 if args.completed_probe and args.completed_mode.startswith('high-scores'):
  states=re.findall(r'\S+\s+_restore_probe_high_scores_state\s+([0-9a-fA-F]{8})\s+',linkmap);assert len(states)==1
  slot=['high-scores','high-scores-scrolled','high-scores-dragged','high-scores-return'].index(args.completed_mode)
  total,first=struct.unpack('<2I',read(int(states[0],16)+slot*8,8))
  report['high_scores_state']={'total':total,'first':first,'records_sha256':hashlib.sha256(read(0xd38708,7600)).hexdigest()}
 if name_entry is not None:report['name_entry']=name_entry
 if mode==13:
  report['live_save_state']={'count':word(0x6057d0),'total':word(0x6059f8),'first':word(0x6059f4),'selected':word(0x6059f0),
   'operation':read(0xd3b1c0,1)[0],'return_mode':word(0xd3b1bc),'filename_hex':read(0xe37100,64).hex()}
 if mode==61 and live_mode==61:
  info=read(0x7848d8,1592);route_lengths=[word(0x7770e4+i*324) for i in range(50)]
  report['live_mission_state']={'list_count':word(0x704db4),'selected_index':word(0x704dbc),'selector_kind':word(0x704db0),
   'scenario_info_sha256':hashlib.sha256(info).hexdigest(),'scenario_info_title_hex':info[60:124].hex(),
   'file_label_hex':read(0xe37078,64).hex(),'file_version':word(0xe945c0),'declared_chunks':word(0xe945c4),
   'nonempty_routes':sum(n>0 for n in route_lengths),'route_points':sum(route_lengths)}
 args.manifest.resolve().write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
finally:
 for handle in handles:k.CloseHandle(handle)
