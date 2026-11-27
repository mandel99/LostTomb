import pathlib,os,struct,json,base64,urllib.request,hashlib
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison';cfg=json.loads((D/'ready.json').read_text());URL=f"http://127.0.0.1:{cfg['port']}/emulate";HEAD={'Content-Type':'application/json','X-LostTomb-Token':cfg['token']}
def call(v):
 req=urllib.request.Request(URL,data=json.dumps(v).encode(),headers=HEAD);return json.load(urllib.request.urlopen(req,timeout=65))
def chunk(a,b):return {'address':f'{a:x}','base64':base64.b64encode(b).decode()}
def dll():
 b=(D/'placement-fixture.dll').read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0];n=struct.unpack_from('<H',b,pe+6)[0];sz=struct.unpack_from('<H',b,pe+20)[0];op=pe+24;assert struct.unpack_from('<H',b,op)[0]==0x10b;base=struct.unpack_from('<I',b,op+28)[0];assert base==0x10000000;size,headers=struct.unpack_from('<II',b,op+56);memory=bytearray(size);memory[:headers]=b[:headers]
 for i in range(n):
  s=op+sz+i*40;vs,va,raw,off=struct.unpack_from('<IIII',b,s+8);memory[va:va+raw]=b[off:off+raw]
 exrva=struct.unpack_from('<I',b,op+96)[0];ed=struct.unpack_from('<IIHHIIIIIII',memory,exrva);num=ed[7];funcs,names,ords=ed[8:11];exports={}
 for i in range(num):
  x=struct.unpack_from('<I',memory,names+i*4)[0];end=memory.index(0,x);name=memory[x:end].decode();ordinal=struct.unpack_from('<H',memory,ords+i*2)[0];exports[name]=base+struct.unpack_from('<I',memory,funcs+ordinal*4)[0]
 return [chunk(base,memory)],exports
DLL,EXPORT=dll();assert 'fixture_run' in EXPORT,EXPORT
COMMON=[chunk(0xbca2d0,struct.pack('<12i',228,228,0,0,0,0,0,0,227,227,114,114)),chunk(0xbc9768,struct.pack('<I',0x11223344)),chunk(0x20000000,bytes(0x10000))]
REG={'ESP':'30010000','ECX':'20000000','EAX':'a1b2c3d4','EBX':'b2c3d4e5','ESI':'c3d4e5f6','EDI':'d4e5f607','EBP':'e5f60718'}
def request(entry,writes,args):
 return {'entry':f'{entry:x}','stop':'40000000','max_steps':100000,'timeout_ms':30000,'registers':REG,'writes':writes+[chunk(0x30010000,struct.pack('<I',0x40000000)+b''.join(struct.pack('<I',a&0xffffffff) for a in args))],'watch':[{'address':'bc9768','size':4}]}
if __name__=='__main__':
 results=[]
 for terrain in [0,1]:
  case=struct.pack('<8I',0,0,0x20000000,26106,0x2000004c,0,terrain,7);w=COMMON+[chunk(0x200f0000,case)];a=call(request(0x4e5310,w,[terrain,7]));b=call(request(EXPORT['fixture_run'],DLL+w,[0x200f0000]));passed=a.get('ok') and b.get('ok') and (int(a['registers']['EAX'],16)&255)==int(b['registers']['EAX'],16) and a['memory']==b['memory'];results.append({'terrain':terrain,'pass':passed,'native':a,'C':b});print({'terrain':terrain,'pass':passed,'native_steps':a.get('steps'),'C_steps':b.get('steps'),'C_error':b.get('error'),'native_EAX':a.get('registers',{}).get('EAX'),'C_EAX':b.get('registers',{}).get('EAX')})
 (D/'placement-smoke-pairs.json').write_text(json.dumps(results));(D/'placement-dll-exports.json').write_text(json.dumps(EXPORT));assert all(r['pass'] for r in results)
