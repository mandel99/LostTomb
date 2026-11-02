"""Whole ordered map-neighbor algorithms versus manual C, private or production."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--production',action='store_true');parser.add_argument('--manifest-sha256')
parser.add_argument('--exe',type=Path);parser.add_argument('--plain-exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--optimized-plain-exe',type=Path);parser.add_argument('--limit',type=int);args=parser.parse_args()
MASK=0xffffffff;PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';sha=lambda b:hashlib.sha256(b).hexdigest()
assert sha((ROOT/'src/unit_01137.c').read_bytes())=='fb8e7ee47f65bd86d17672b738f85a3069a312c12baec01ac159a06e4edf9159','unchanged real7DB00 drift'
manifest_path=HERE/'map_neighbors_phase_apply_map.json';manifest=None
if args.production:
    assert args.exe and args.plain_exe and args.optimized_exe and args.optimized_plain_exe,'all four shared binaries required'
    manifest=json.loads(manifest_path.read_text());assert manifest['frozen']
    if args.manifest_sha256:assert sha(manifest_path.read_bytes())==args.manifest_sha256
    for p,want in manifest['proof_hashes'].items():assert sha((ROOT/p).read_bytes())==want,p
    for m in manifest['mappings']:
        assert sha((ROOT/m['staged']).read_bytes())==m['staged_sha256'],m['staged']
        assert sha((ROOT/m['target']).read_bytes())==m['staged_sha256'],m['target']
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
native_data=json.loads((HERE/'map_neighbors_native.json').read_text());assert native_data['native_sha256']==PIN
entries=[0x47dbc0,0x47dc80,0x47ddd0,0x47dee0,0x47e000,0x47ddd0];all_instructions={}
for name,e in native_data['functions'].items():
    assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more'] and e['info']['address']==name
    for i in e['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        all_instructions[a]=name
for sites in native_data['callers'].values():
    for site in sites:
        for i in site['preceding']+site['following']:
            a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
offsets=[-228,-227,1,229,228,227,-1,-229]
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,n,v):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False
def code(cpu,a,n,user):
    if a in all_instructions:hits.add(a)
def on_read(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:events.append([0,a,n,read(a,n)])
def on_write(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert 0xbc97c0<=a<0xbc97e0 and n==4,'write outside eight slot DWORDs'
        events.append([1,a,n,value&MASK])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,on_read);vm.hook_add(UC_HOOK_MEM_WRITE,on_write)
def regions(p):
    t=p[1];return [((0xaf598c+t*4-932)&MASK,1868),((0xb285cc+t-245)&MASK,492),(0xbc9758,36),(0xbc97b0,64)]
def memory_hash(rs):
    h=hashlib.sha256()
    for a,n in rs:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return h.hexdigest()
def execute(entry,a,b):
    global running
    sp=STACK+0xf0000;stack=[STOP,a,b,0xbaadf00d]
    for k,v in enumerate(stack):write(sp+k*4,4,v)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0x9abcdef0);vm.reg_write(UC_X86_REG_EFLAGS,2)
    running=True
    try:vm.emu_start(entry,STOP,count=2000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'cdecl/budget'
    assert all(vm.reg_read(r)==v for r,v in saved),'saved registers'
    assert [read(sp+k*4) for k in range(4)]==stack,'argument/stack canaries'
    return vm.reg_read(UC_X86_REG_EAX)
def native(p):
    rs=regions(p);t=p[1]
    for i,(a,n) in enumerate(rs):vm.mem_write(a,bytes([(p[4]+i*41)&255])*n)
    write(0xbc9768,4,t)
    for i,off in enumerate(offsets):
        write(0xaf598c+(t+off)*4,4,p[6+i]);write(0xbc97c0+i*4,4,p[14+i]);write(0xb285cc+t+off,1,p[22+i])
    immutable=[bytes(vm.mem_read(a,n)) for a,n in rs[:3]]
    if p[0]==5:
        events.clear();execute(0x47db00,p[5],0)
    before_slots=[read(0xbc97c0+i*4) for i in range(8)];events.clear()
    raw=execute(entries[p[0]],p[2],p[3])
    assert all(bytes(vm.mem_read(a,n))==blob for (a,n),blob in zip(rs[:3],immutable)),'input/current-tile mutation'
    if p[0]==4:assert raw in [0,1],'full EAX Boolean'
    return {'result':raw if p[0]==4 else 0,'memory':memory_hash(rs),'before_slots':before_slots,'events':events.copy()}
cases=[]
def case(op=0,tile=10000,mask=4,arg=0,seed=17,base_mask=1,flags=None,old=None,secondary=None):
    if flags is None:flags=[0]*8
    if old is None:old=[((0xdeadbeef+i*0x1234567)^(seed*0x1020304))&MASK for i in range(8)]
    if secondary is None:secondary=[(seed+i*37)&255 for i in range(8)]
    cases.append([op,tile&MASK,mask&MASK,arg&MASK,seed&MASK,base_mask&MASK]+[v&MASK for v in flags]+[v&MASK for v in old]+[v&255 for v in secondary])
masks=[0,1,2,0x40,0x100,0x8000,0x10000,0x80000000,MASK]
for mask,pattern in itertools.product(masks,range(256)):
    case(0,mask=mask,flags=[mask if pattern>>i&1 else (~mask&MASK) for i in range(8)],seed=pattern)
filter_pairs=[(0,0),(4,0),(4,4),(4,1),(0x100,4),(0x80000000,1),(MASK,0),(MASK,MASK),(0x50000,0x80000)]
for (include,exclude),pattern in itertools.product(filter_pairs,range(256)):
    case(1,mask=include,arg=exclude,flags=[(include if pattern>>i&1 else 0)|(exclude if ((pattern*37+19)&255)>>i&1 else 0) for i in range(8)],seed=pattern^include)
shifts=list(range(32))+[32,33,63,64,127,128,255,256,0x80000000,MASK]
patterns=[0,255]+[1<<i for i in range(8)]+[255^(1<<i) for i in range(8)]
for mask,shift,pattern in itertools.product(masks,shifts,patterns):
    case(2,mask=mask,arg=shift,flags=[mask if pattern>>i&1 else 0 for i in range(8)],seed=(mask^shift^pattern)&255)
for value,direction,rock in itertools.product(range(256),range(8),[0,2]):
    flags=[0]*8;flags[direction]=rock|0x81000000;secondary=[0]*8;secondary[direction]=value
    case(3,flags=flags,secondary=secondary,seed=value)
for cardinal,diagonal in itertools.product(range(16),range(16)):
    flags=[(0x80008000 if ((cardinal>>(i//2)&1) if i%2==0 else (diagonal>>(i//2)&1)) else 0x80000001) for i in range(8)]
    case(4,flags=flags,seed=cardinal*16+diagonal)
for base_mask,merge_mask,shift,pattern in itertools.product([1,0x40,0x80000000],[0,4,0x10000],[0,1,8,31,32,255],patterns):
    flags=[(base_mask if pattern>>i&1 else 0)|(merge_mask if ((pattern*37)&255)>>i&1 else 0) for i in range(8)]
    case(5,mask=merge_mask,arg=shift,base_mask=base_mask,flags=flags,seed=pattern)
for op,tile in itertools.product(range(6),[-1,0,1,226,227,228,229,456,10000,51754,51983,51984]):
    case(op,tile,mask=0x80000000,arg=31,flags=[0xffffffff^(i*0x11111111) for i in range(8)],secondary=[0,127,128,255,1,129,64,192],seed=tile&255)
rng=random.Random(0x47dbc0)
for k in range(500):case(rng.randrange(6),rng.randrange(51984),rng.getrandbits(32),rng.getrandbits(32),rng.randrange(256),rng.getrandbits(32),[rng.getrandbits(32) for _ in range(8)],[rng.getrandbits(32) for _ in range(8)],[rng.randrange(256) for _ in range(8)])
complete_case_count=len(cases)
if args.limit:cases=cases[:args.limit]
executables=[args.exe or HERE/'build/Debug/map_neighbors_trace.exe',args.plain_exe or HERE/'build/Debug/map_neighbors_plain.exe',args.optimized_exe or HERE/'build/Release/map_neighbors_trace.exe',args.optimized_plain_exe or HERE/'build/Release/map_neighbors_plain.exe']
executables=[p.resolve() for p in executables]
if args.production:
    assert [p.name.lower() for p in executables]==['restore_map_neighbors_verify.exe','restore_map_neighbors_plain_verify.exe']*2
    assert executables[0]!=executables[2] and executables[1]!=executables[3]
    assert all('map_neighbors_phase/build' not in p.as_posix() for p in executables)
processes=[subprocess.Popen([str(p),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in executables]
counts={f'{a:08x}':0 for a in entries};ordered_reads=ordered_writes=transient_pairs=composition_cases=0;started=time.monotonic()
try:
    for process in processes:assert json.loads(process.stdout.readline())=={'fields':30}
    for index,p in enumerate(cases):
        expected=native(p);counts[f'{entries[p[0]]:08x}']+=1;composition_cases+=int(p[0]==5)
        ordered_reads+=sum(e[0]==0 for e in expected['events']);ordered_writes+=sum(e[0]==1 for e in expected['events'])
        transient_pairs+=sum(a[0]==b[0]==1 and a[1]==b[1] and a[3]==1 and b[3]==0 for a,b in zip(expected['events'],expected['events'][1:]))
        line=' '.join(map(str,p))+'\n'
        for variant,process in enumerate(processes):
            process.stdin.write(line);process.stdin.flush();output=process.stdout.readline();assert output,'C exited: '+process.stderr.read()
            actual=json.loads(output)
            if variant in [1,3]:actual.pop('events');compare={k:v for k,v in expected.items() if k!='events'}
            else:compare=expected
            assert actual==compare,json.dumps({'case':index,'p':p,'variant':str(executables[variant]),'actual':actual,'expected':compare})
        if (index+1)%5000==0:print(f'checked {index+1}/{len(cases)}',flush=True)
finally:
    for process in processes:process.stdin.close();process.wait(timeout=20);assert process.returncode==0,process.stderr.read()
coverage={fn:[sum(a in hits for a,f in all_instructions.items() if f==fn),sum(f==fn for f in all_instructions.values())] for fn in native_data['functions']}
phase_coverage={fn:v for fn,v in coverage.items() if fn!='0047db00'}
if not args.limit:assert all(a==b for a,b in coverage.values()) and sum(b for a,b in phase_coverage.values())==348
unit_paths=[ROOT/'src'/f'unit_{n:05}.c' if args.production else HERE/f'unit_{n:05}.c' for n in range(1138,1143)]
header=ROOT/'include/map_neighbors_phase.h' if args.production else HERE/'map_neighbors_phase.h'
source_paths=unit_paths+[header,HERE/'map_neighbors_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'map_neighbors_native.json',Path(__file__),ROOT/'src/unit_01137.c',ROOT/'include/terrain_redraw.h']
if args.production:source_paths.append(manifest_path)
relative=lambda p:str(p.relative_to(ROOT)).replace('\\','/')
report={'format':1,'passed':True,'checked':len(cases),'complete_case_count':complete_case_count,'variant_comparisons':len(cases)*4,'production':args.production,'per_function':counts,'composition_cases':composition_cases,'native_sha256':PIN,'original_game_executed':False,'algorithm_endpoints':[],'compiled_C_sha256':sha(executables[0].read_bytes()),'native_instruction_coverage':phase_coverage,'phase_instructions':[sum(a for a,b in phase_coverage.values()),sum(b for a,b in phase_coverage.values())],'real_helper_instruction_coverage':coverage['0047db00'],'ordered_events':{'reads_per_trace_variant':ordered_reads,'writes_per_trace_variant':ordered_writes,'transient_one_then_zero_pairs_per_trace_variant':transient_pairs},'elapsed_seconds':round(time.monotonic()-started,2),'method':'Complete original5functions versus5manualseparateCunits, Debug/O2 trace+plain; exactdataread/writeorder/width/value includingprefetch/transientstores, fullinput/state/outputbytes+canaries, read-onlyCinputpages, cdecl/savedregisters/fullEAXE000 andunchangedreal7DB00composition.','source_hashes':{relative(p):sha(p.read_bytes()) for p in source_paths},'native_exports':{relative(HERE/'map_neighbors_native.json'):sha((HERE/'map_neighbors_native.json').read_bytes())},'variants':{label:{'path':relative(p),'sha256':sha(p.read_bytes()),'checked':len(cases),'trace':i in [0,2]} for i,(label,p) in enumerate(zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables))},'limitations':['These leaves usephysical228stride andcaller-ownedmappedpadding; no bounds, orientation or sentinel checks are invented. Arbitraryunmappedinvalidaddresses are outside the valid domain.','Only ordered events inside the new5functions are compared to tracedC; the unchanged7DB00helper is actuallyexecuted and its before/after slotstate and finalmemory are compared.','VoidEAX/ECX/EDXscratch is not a returncontract. E000fullEAX0/1 is compared. ABIrequires clearDF.','This bounded proof doesnotapprove higherterrain/renderingcallers, actorwalking, visualfidelity orwholegamecompletion.']}
report['source_drift_at_completion']=[relative(p) for p in source_paths if sha(p.read_bytes())!=report['source_hashes'][relative(p)]]
if args.production:report['frozen_manifest_sha256']=sha(manifest_path.read_bytes())
filename='map-neighbors-emulation'+('-partial' if args.limit else '')+'.json'
output=(ROOT/'metadata'/filename) if args.production else HERE/filename
output.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','variant_comparisons','phase_instructions','real_helper_instruction_coverage','elapsed_seconds']}))
