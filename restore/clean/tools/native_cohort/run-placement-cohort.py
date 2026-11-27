import pathlib,os,json,struct,runpy,time
D=pathlib.Path(os.environ['TEMP'])/'losttomb-ghidra-native-comparison';g=runpy.run_path(str(D/'compare-placement.py'));chunk,call,request=[g[n] for n in ['chunk','call','request']];DLL,EXPORT,REG=[g[n] for n in ['DLL','EXPORT','REG']];traceaddr=json.loads((D/'placement-trace-addresses.json').read_text());spec=json.loads((D/'placement-spec.json').read_text());native_pred=[0x4e5310,0x401440,0x547de0,0x4e6620];Cpred=[traceaddr[n] for n in ['pharaoh_monument_tile_is_free','pharaoh_abu_simbel_tile_is_free','pharaoh_royal_tomb_terrain_allowed','pharaoh_lighthouse_tile_is_free']];SELF=0x20000000;VT=0x20020000;CASE=0x200f0000;SP=0x30010000;CELL=114*228+114;BLD=0x7999d8;STOP=0x40000000;cases=[]
def add(label,op,sel=0,terrain=0,part=0,count=2,mode='clear',building=None):cases.append(dict(label=label,op=op,sel=sel,terrain=terrain,part=part,count=count,mode=mode,building=building))
for p in range(4):
 for terrain in [0,2,0x20000002,0x40]:
  for part in [0,1]:add(f'predicate-{p}-{terrain:08x}-part{part}',0,p,terrain,part)
for count in [-1,0,1,2]:
 for mode in ['clear','blocked']:add(f'shared-{count}-{mode}',4,count=count,mode=mode)
for mode in ['clear','blocked','no-adjacency','north-only']:add('royal-'+mode,5,count=2,mode=mode)
for i,s in enumerate(spec):
 for mode in ['clear','blocked']:add(s[0]+'-check-'+mode,6,i,count=s[1],mode=mode)
 for case in ['none','first','wrong-state','outside-count']:add(s[0]+'-existing-'+case,7,i,count=s[1],building=case)
for count in [-1,0,1,2]:
 for case in ['none','first']:add(f'generic-existing-{count}-{case}',8,count=count,terrain=230,building=case)
results=[];began=time.monotonic()
for index,c in enumerate(cases):
 op,sel,count=c['op'],c['sel'],c['count'];offset=0x1e5c if op==5 else (spec[sel][3] if op in [6,7] else 0x4c);pred=2 if op==5 else ([1,0,0,3,2,2,2,2][sel] if op in [6,7] else sel);size=spec[sel][1] if op in [6,7] else max(count,2);size=max(size,2);blob=bytearray(0x10000);struct.pack_into('<I',blob,0,VT);struct.pack_into('<I',blob,0x50,0)
 for i in range(size):struct.pack_into('<4i',blob,offset+i*52,0,0,1 if i==0 else 0,9 if i==0 else 1)
 if size>1:struct.pack_into('<4i',blob,offset+(size-1)*52,7,5,0,1)
 fields=(op,sel,SELF,CELL,SELF+offset,count&0xffffffff,c['terrain'],c['part']);writes=[chunk(0xbca2d0,struct.pack('<12i',228,228,0,0,0,0,0,0,227,227,114,114)),chunk(0xbc9768,struct.pack('<I',0x11223344)),chunk(SELF,blob),chunk(VT,struct.pack('<5I',0,0,0,0x4e52d0,native_pred[pred])),chunk(CASE,struct.pack('<8I',*fields))]
 terrain=bytearray(51984*4)
 if c['mode']=='blocked':struct.pack_into('<I',terrain,CELL*4,8)
 elif c['mode'] in ['no-adjacency','north-only']:
  for delta in [-228,228,-1,1]:struct.pack_into('<I',terrain,(CELL+delta)*4,0x20000002)
  if c['mode']=='north-only':struct.pack_into('<I',terrain,(CELL-228)*4,0)
 writes.append(chunk(0xaf598c,terrain));watch=[{'address':'bc9768','size':4},{'address':f'{SELF:x}','size':len(blob)}]
 if op in [7,8]:
  buildings=bytearray(4000*264);kind=spec[sel][2] if op==7 else c['terrain'];which=c['building'];bid=count if which=='outside-count' else 1
  if which not in [None,'none'] and 0<bid<4000:buildings[bid*264]=2 if which=='wrong-state' else 1;struct.pack_into('<h',buildings,bid*264+16,kind)
  writes.append(chunk(BLD,buildings));watch.append({'address':f'{BLD:x}','size':len(buildings)})
 if op==0:entry=native_pred[sel];args=[c['terrain'],c['part']];pop=8
 elif op in [4,5]:entry=0x4e5330 if op==4 else 0x5463e0;args=[CELL,SELF+offset,count];pop=12
 elif op in [6,7]:entry=int(spec[sel][4 if op==6 else 5],16);args=[CELL] if op==6 else [];pop=4 if op==6 else 0
 else:entry=0x4e5790;args=[SELF,c['terrain'],count];pop=12
 rn=request(entry,writes,args);rc=request(EXPORT['fixture_run'],DLL+writes,[CASE]);rn['watch']=watch;rc['watch']=watch
 if op in [7,8]:rn['trace']=[{'address':'4e52d0','tag':'progress','stack_offsets':[4,8]}];rc['trace']=[{'address':f"{traceaddr['pharaoh_monument_no_progress']:x}",'tag':'progress','stack_offsets':[8,12]}]
 elif op in [4,5,6]:rn['trace']=[{'address':f'{native_pred[pred]:x}','tag':'predicate','stack_offsets':[4,8]}];rc['trace']=[{'address':f'{Cpred[pred]:x}','tag':'predicate','stack_offsets':[8,12]}]
 a=call(rn);b=call(rc);issues=[]
 if not a.get('ok') or not b.get('ok'):issues.append(['execution',a.get('error'),b.get('error')])
 else:
  if (int(a['registers']['EAX'],16)&255)!=int(b['registers']['EAX'],16):issues.append(['AL',a['registers']['EAX'],b['registers']['EAX']])
  if a['memory']!=b['memory']:issues.append(['memory',a['memory'],b['memory']])
  if [(t['tag'],t['args']) for t in a['trace']]!=[(t['tag'],t['args']) for t in b['trace']]:issues.append(['logical calls',a['trace'],b['trace']])
  for n in ['EBX','ESI','EDI','EBP']:
   if a['registers'][n]!=REG[n] or b['registers'][n]!=REG[n]:issues.append(['nonvolatile',n,a['registers'][n],b['registers'][n]])
  if int(a['registers']['ESP'],16)!=SP+4+pop or int(b['registers']['ESP'],16)!=SP+4:issues.append(['cleanup',a['registers']['ESP'],b['registers']['ESP'],pop])
 results.append({'case':c,'native_entry':f'{entry:08x}','passed':not issues,'issues':issues,'native':a,'C':b});(D/'placement-cohort-results.json').write_text(json.dumps(results))
 if issues:print(json.dumps({'failed':c['label'],'issues':issues[:2]})[:1800]);break
 if index%12==0:print(f'Compared{index+1}/{len(cases)} cases in{time.monotonic()-began:.1f}s',flush=True)
summary={'total_cases':len(cases),'executed':len(results),'passed':sum(r['passed'] for r in results),'elapsed_seconds':time.monotonic()-began,'native_entries':sorted({r['native_entry'] for r in results}),'all_selected_entries_compared':len({r['native_entry'] for r in results})==23,'scope':'Original x86 p-code and linked Release C in isolated memory in same existing Ghidra JVM; logical AL, full watched buffers, ordered genuine predicate/progress calls, stack cleanup and nonvolatile registers. No endpoint stubs.'};(D/'placement-cohort-summary.json').write_text(json.dumps(summary));print(json.dumps(summary));assert len(results)==len(cases) and all(r['passed'] for r in results)
