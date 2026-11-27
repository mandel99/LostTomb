import pathlib,os,json,struct,re,time
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison'
root=pathlib.Path(__file__).resolve().parent
source=(root/'compare-placement.py').read_text().replace('placement-fixture.dll','warnings-fixture.dll')
ns={'__name__':'cohort_common'};exec(compile(source,str(root/'compare-placement.py'),'exec'),ns)
call,chunk,request,DLL,EXPORT,REG=[ns[n] for n in ['call','chunk','request','DLL','EXPORT','REG']]
CASE=0x200f0000;SP=0x30010000
m=(D/'warnings-fixture.map').read_text()
def symbol(name):
 x=re.search(r'\s_'+re.escape(name)+r'\s+([0-9A-Fa-f]{8})\s',m);assert x,name;return int(x[1],16)
POST=symbol('pharaoh_city_warning_post')
owners=json.loads((D/'warnings-spec.json').read_text())
entries=[int(a,16) for a,n in owners]+[0x443900,0x443000,0x480770]
def pack(a,v,fmt='<i'):return chunk(a,struct.pack(fmt,v))
def base():
 return [pack(0x605a3c,0),pack(0xeaac28,0),pack(0xe38ec1,0,'<B'),pack(0xe38dda,1,'<B'),pack(0xe94e20,0),pack(0xf53f34,0),chunk(0xe94e28,bytes(5*120)),chunk(0xea1870,bytes(2*0x49d8)),chunk(0x78d64c,bytes(288)),chunk(0x705af0,bytes(237*4)),chunk(0x77de00,bytes(61*106)),chunk(0x77f748,bytes(200*98)),chunk(0x77646c,bytes(2*144)),chunk(0x784ce0,bytes(86)),chunk(0x784de0,bytes(6)),chunk(0x5d19c8,bytes(16*8)),chunk(0xbca2d0,struct.pack('<3i',228,228,0)),pack(0xd3b4e0,0x12345678),pack(0xd3b4e4,0x23456789),pack(0xe950d4,0)]
RESOURCE_WATCH=[(0xea1870,2*0x49d8),(0x78d64c,288),(0x705af0,237*4),(0x77de00,61*106),(0x77f748,200*98),(0x77646c,2*144),(0x784ce0,86),(0x784de0,6),(0x5d19c8,16*8),(0xe94e20,8+5*120),(0xf53f34,4)]
def run_case(c):
 op=c['op'];args=c.get('args',[c.get('tool',0)]);w=base()+c.get('writes',[])+[chunk(CASE,struct.pack('<I5i',op,c.get('tool',0),c.get('x',111),c.get('y',113),c.get('size',1),c.get('extra',0)))]
 if op in (19,20):args=[c['tool'],c.get('x',111),c.get('y',113),c.get('size',1)]
 elif op in (21,22):args=[]
 elif op==24:args=[c.get('x',0),c['tool']]
 elif op==25:args=[c.get('x',111),c.get('y',113),c.get('size',1)]
 rn=request(entries[op],w,args);rc=request(EXPORT['fixture_run'],DLL+w,[CASE]);watch=c.get('watch',RESOURCE_WATCH)
 rn['watch']=rc['watch']=[{'address':f'{a:x}','size':n} for a,n in watch]
 rn['max_steps']=rc['max_steps']=300000
 rn['trace']=[{'address':'4e2970','tag':'warning','stack_offsets':[4]}]
 rc['trace']=[{'address':f'{POST:x}','tag':'warning','stack_offsets':[8]}]
 a=call(rn);b=call(rc);issues=[]
 if not a.get('ok') or not b.get('ok'):issues.append(['execution',a.get('error'),b.get('error'),a.get('pc'),b.get('pc')])
 else:
  if op>=23 and (int(a['registers']['EAX'],16)&255)!=(int(b['registers']['EAX'],16)&255):issues.append(['AL',a['registers']['EAX'],b['registers']['EAX']])
  if a['memory']!=b['memory']:issues.append(['memory',[(x['address'],x['sha256'],y['sha256']) for x,y in zip(a['memory'],b['memory']) if x!=y]])
  if [(t['tag'],t['args']) for t in a['trace']]!=[(t['tag'],t['args']) for t in b['trace']]:issues.append(['ordered_calls',a['trace'],b['trace']])
  for n in ['EBX','ESI','EDI','EBP']:
   if a['registers'][n]!=REG[n] or b['registers'][n]!=REG[n]:issues.append(['nonvolatile',n,a['registers'][n],b['registers'][n]])
  if int(a['registers']['ESP'],16)!=SP+4 or int(b['registers']['ESP'],16)!=SP+4:issues.append(['cleanup',a['registers']['ESP'],b['registers']['ESP']])
 return {'case':c,'native_entry':f'{entries[op]:08x}','passed':not issues,'issues':issues,'native':a,'C':b}
if __name__=='__main__':
 r=run_case({'label':'brewery_no_supply','op':0,'tool':110});(D/'warnings-smoke.json').write_text(json.dumps(r));print({'passed':r['passed'],'issues':r['issues'],'native_steps':r['native'].get('steps'),'C_steps':r['C'].get('steps')})
