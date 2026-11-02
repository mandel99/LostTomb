"""Whole native map-cell functions versus independent manually written C."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent;STAGE=ROOT/'artifacts/map_cells_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--production',action='store_true')
parser.add_argument('--exe',type=Path);parser.add_argument('--plain-exe',type=Path)
parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--optimized-plain-exe',type=Path)
parser.add_argument('--limit',type=int);args=parser.parse_args()
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest()
frozen_manifest=STAGE/'map_cells_phase_apply_map.json'
assert sha(frozen_manifest.read_bytes())=='8d441528556fbfc31b90f4acaa46c5b1ed87f192832adf44647b897e3711df66','frozen manifest drift'
manifest=json.loads(frozen_manifest.read_text())
for mapping in manifest['mappings']:
    assert sha((ROOT/mapping['staged']).read_bytes())==mapping['staged_sha256'],mapping['staged']
for name,want in manifest['proof_hashes'].items():assert sha((ROOT/name).read_bytes())==want,name
assert sha((ROOT/'tests/map_cells_phase.c').read_bytes())==sha((STAGE/'map_cells_phase_test.c').read_bytes()),'shared test differs from frozen test'
# Freeze the actual algorithm and corpus, rather than accepting a renamed
# unrelated verifier. Infrastructure and report routing below are independent.
original_tool=(STAGE/'verify_map_cells_phase.py').read_text()
new_tool=Path(__file__).read_text()
oracle_start="u16=lambda p:struct.unpack_from('<H',image,p)[0]"
oracle_end="if args.limit:cases=cases[:args.limit]"
oracle_core=original_tool.split('\n'+oracle_start,1)[1].split('\n'+oracle_end,1)[0]
assert new_tool.split('\n'+oracle_start,1)[1].split('\n'+oracle_end,1)[0]==oracle_core,'native oracle/corpus drift'
source_paths=[ROOT/m['target' if args.production else 'staged'] for m in manifest['mappings']]
if args.production:
    for mapping,path in zip(manifest['mappings'],source_paths):
        assert path.is_file() and sha(path.read_bytes())==mapping['staged_sha256'],'unapplied or changed product source: '+str(path)
    assert all([args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]),'production requires all four shared executables'
source_paths.extend([ROOT/'tests/map_cells_phase.c',ROOT/'tools/map_cells_shared_targets.cmake',Path(__file__),frozen_manifest,ROOT/'metadata/map-cell-neighborhood-native.json',ROOT/'metadata/map-cell-rings-native.json'])
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(size,0x1100000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    s=table+k*40;vm.mem_write(base+u32(s+12),image[u32(s+20):u32(s+20)+u32(s+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
functions={}
for path in ['metadata/map-cell-neighborhood-native.json','metadata/map-cell-rings-native.json']:
    dossier=json.loads((ROOT/path).read_text());assert dossier['native_sha256']==digest;functions.update(dossier['functions'])
instructions={}
for fn,body in functions.items():
    assert not body['assembly'].get('has_more',False)
    for i in body['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        instructions[a]=fn
entries=[0x486d30,0x487260,0x4872c0,0x487300,0x487390]
hits=set();reads=[];writes=[];running=False
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,n,v):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def code(cpu,a,n,user):
    if a in instructions:hits.add(a)
def memread(cpu,access,a,n,value,user):
    if running and 0x700000<=a<STACK:reads.append([a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:writes.append([a,n,value])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def regions(p):
    op,a,b,c,d=p;t=(a+b)&MASK if op==4 else a
    if op==0:return [(0xaf598c-16,228*228*4+32),(0xbca2c0,48)]
    return [(0xaf598c+t*4-928,1860),(0xd05720+t*4-928,1860),(0xa509bc+t-16,36),(0xa509bc+t*4-16,36),(0xe00000,256)]
def memory_hash(rs):
    h=hashlib.sha256()
    for a,n in rs:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return h.hexdigest()
def native(p,fixture):
    global running
    rs=regions(p)
    for a,n in rs:vm.mem_write(a,bytes(n))
    for a,n,v in fixture:write(a,n,v)
    before=memory_hash(rs);reads.clear();writes.clear();sp=STACK+0xf0000
    for k,v in enumerate([STOP]+p[1:]):write(sp+k*4,4,v)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0x9abcdef0);vm.reg_write(UC_X86_REG_EFLAGS,2)
    running=True
    try:vm.emu_start(entries[p[0]],STOP,count=1000000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP,'instruction budget'
    assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'cdecl stack'
    assert all(vm.reg_read(r)==v for r,v in saved),'saved registers'
    assert not writes,writes
    after=memory_hash(rs);assert before==after
    raw=vm.reg_read(UC_X86_REG_EAX)
    return {'result':raw&255 if p[0]==1 else raw,'memory':after,'reads':reads.copy()},raw
cases=[]
def leaf(op=3,tile=10000,offset=0,mask=4,terr=(0,0,0,0),nodes=(0,0,0,0,0),growth=0,current=0):
    t=(tile+offset)&MASK if op==4 else tile;p=[op,tile&MASK,(offset if op==4 else mask)&MASK,0,0];f=[]
    for off,v in zip([-228,228,-1,1],terr):f.append((0xaf598c+(t+off)*4,4,v))
    f.extend([(0xaf598c+t*4,4,current),(0xa509bc+t,1,growth),(0xa509bc+t*4,4,0xdeadbeef)])
    for k,(off,kind) in enumerate(zip([-228,228,-1,1,0],nodes)):
        ptr=0xe00000+k*40 if kind else 0;f.append((0xd05720+(t+off)*4,4,ptr))
        if ptr:f.append((ptr+24,1,[0,0,1,128,255][kind]))
    # Distinct padding canaries and adjacent bytes expose mistaken DWORD strides.
    for a,n in regions(p):f.extend([(a,1,0xa5),(a+n-1,1,0x5a)])
    cases.append((p,f))
# Exhaustive opposing terrain states/mask widths and object pointer+byte states.
for op in [1,3,4]:
    for states in itertools.product(range(5),repeat=4):leaf(op,nodes=states+(0,),growth=173)
for mask in [0,1,2,4,128,256,65536,0x40004,0x80000000,MASK]:
    for terr in itertools.product([0,mask],repeat=4):leaf(2,mask=mask,terr=terr)
for op,water,banks in itertools.product([3,4],range(16),range(16)):
    leaf(op,terr=tuple(4 if water>>i&1 else 0 for i in range(4)),nodes=tuple(1 if banks>>i&1 else 0 for i in range(4))+(0,),growth=37)
for op,growth,current,node in itertools.product([3,4],range(256),[0,4,0x40000,0x40004,0x80000,0x80000000],[0,1,2]):
    leaf(op,growth=growth,current=current,nodes=(0,0,0,0,node))
# Physical array corners/edges/padding; leaves have no internal clipping.
for tile,off in itertools.product([229,230,455,456,10000,51754,51755],[-229,-228,-227,-1,0,1,227,228,229]):
    for op in [1,2,3,4]:leaf(op,tile,off,growth=(tile+off)&255,terr=(0,4,4,0),nodes=(2,1,0,2,0))
def ring(x,y,mx,my,limit,mask=4,marks=(),baseoffset=0):
    p=[0,x&MASK,y&MASK,mask&MASK,limit&MASK]
    f=[(0xbca2d0,4,mx),(0xbca2d4,4,my),(0xbca2d8,4,baseoffset),(0xaf598c-16,1,0xa5),(0xaf598c+228*228*4+15,1,0x5a)]
    for tx,ty,flag in marks:f.append((0xaf598c+(baseoffset+ty*228+tx)*4,4,flag))
    cases.append((p,f))
for limit in [-2147483648,-1,0,1,2,3,5,9,17]:
    for x,y in [(0,0),(0,4),(4,0),(4,4),(227,227),(-1,0),(0,-1),(228,227),(227,228)]:ring(x,y,227,227,limit)
for x,y,mx,my in [(8,8,20,20),(0,0,20,20),(20,20,20,20),(0,20,20,20),(20,0,20,20),(-1,10,20,20),(21,10,20,20),(4,4,4,4),(3,3,256,256),(3,3,0x80000000,20)]:
    for r in [1,2,3,5]:
        coords=[(x+dx,y+dy) for dy in range(-r,r+1) for dx in range(-r,r+1) if max(abs(dx),abs(dy))==r and 0<=x+dx<228 and 0<=y+dy<228]
        for tx,ty in coords:
            ring(x,y,mx,my,r+2,marks=[(tx,ty,4)]);ring(x,y,mx,my,r+1,marks=[(tx,ty,0x80004)])
for mask in [0,1,4,128,0x10000,0x50000,0x80000,0x80000000,MASK]:
    ring(8,8,20,20,7,mask,[(9,7,0x80000|mask),(10,7,mask),(8,8,mask)])
for offset in [0,228,456]:ring(6,6,12,12,8,marks=[(4,7,4)],baseoffset=offset)
rng=random.Random(0x486d30)
for k in range(300):
    x,y=rng.randrange(30),rng.randrange(30);marks=[(rng.randrange(30),rng.randrange(30),rng.choice([4,0x80004,128,0x40000])) for _ in range(15)]
    ring(x,y,29,29,rng.randrange(2,15),rng.choice([4,128,0x50000]),marks)
canonical_cases=len(cases);assert canonical_cases==13654
# Additional actual caller limit46 and modular offset probes retain the complete
# canonical corpus above unchanged and are counted separately in the report.
for x,y in [(0,0),(0,227),(227,0),(227,227),(114,114)]:
    ring(x,y,227,227,46)
    for r in [15,16,44,45,46]:
        tx=x+r if x+r<228 else x-r
        ring(x,y,227,227,46,marks=[(tx,y,4)])
for a in [0,1,0x7fffffff,0x80000000,0xffffffff,0x12345678]:
    for growth in [0,100,127,128,255]:leaf(4,a,(10000-a)&MASK,growth=growth)
if args.limit:cases=cases[:args.limit]
executables=[args.exe or STAGE/'build/Debug/map_cells_trace.exe',args.plain_exe or STAGE/'build/Debug/map_cells_plain.exe',args.optimized_exe or STAGE/'build/Release/map_cells_trace.exe',args.optimized_plain_exe or STAGE/'build/Release/map_cells_plain.exe']
executables=[e.resolve() for e in executables]
if args.production:
    assert [e.name.lower() for e in executables]==['restore_map_cells_verify.exe','restore_map_cells_plain_verify.exe','restore_map_cells_verify.exe','restore_map_cells_plain_verify.exe'],'production requires shared target names'
    assert executables[0]!=executables[2] and executables[1]!=executables[3],'Debug and optimized executables must differ'
    assert all('map_cells_phase/build' not in e.as_posix() for e in executables),'private stage binaries are not production proof'
variant_labels=['Debug_trace','Debug_plain','O2_trace','O2_plain']
processes=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
started=time.monotonic();counts={f:0 for f in functions};raw_native={op:set() for op in range(5)};raw_c={op:set() for op in range(5)};ordered_reads=0;ordered_read_cases={f:0 for f in functions}
try:
    for process in processes:assert json.loads(process.stdout.readline())=={'fields':5}
    for index,(p,fixture) in enumerate(cases):
        expected,nraw=native(p,fixture);raw_native[p[0]].add(nraw);counts[f'{entries[p[0]]:08x}']+=1
        ordered_reads+=len(expected['reads']);ordered_read_cases[f'{entries[p[0]]:08x}']+=int(bool(expected['reads']))
        line=' '.join(map(str,p+[len(fixture)]+[v&MASK for entry in fixture for v in entry]))+'\n'
        for k,process in enumerate(processes):
            process.stdin.write(line);process.stdin.flush();output=process.stdout.readline();assert output,'C exited: '+process.stderr.read()
            actual=json.loads(output);raw=actual.pop('raw');raw_c[p[0]].add(raw)
            if k in [1,3]:actual.pop('reads');compare={k:v for k,v in expected.items() if k!='reads'}
            else:compare=expected
            assert actual==compare,json.dumps({'case':index,'params':p,'fixture':fixture,'exe':str(executables[k]),'actual':actual,'expected':compare})
        if (index+1)%5000==0:print(f'checked {index+1}/{len(cases)}',flush=True)
finally:
    for process in processes:
        process.stdin.close();process.wait(timeout=20)
        assert process.returncode==0,process.stderr.read()
coverage={fn:[sum(a in hits for a,f in instructions.items() if f==fn),sum(f==fn for f in instructions.values())] for fn in functions}
if not args.limit:assert all(a==b for a,b in coverage.values()),coverage
# Retain all actual caller windows and instruction-authenticated return consumers.
md=Cs(CS_ARCH_X86,CS_MODE_32);callers={}
for fn,body in functions.items():
    callers[fn]=[]
    for xref in body['xrefs']['items']:
        a=int(xref['from'],16);window=[{'address':f'{i.address:08x}','bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str} for i in md.disasm(bytes(vm.mem_read(a,80)),a)]
        callers[fn].append(window)
assert len(callers['00487260'])==5
assert all(any(i['instruction']=='test al, al' for i in window[1:4]) for window in callers['00487260'])
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':True,'checked':len(cases),'variant_comparisons':len(cases)*len(executables),'per_function':counts,'native_instruction_coverage':coverage,'elapsed_seconds':round(time.monotonic()-started,2),'algorithm_endpoints':[],
 'canonical_cases':canonical_cases,'extended_cases':max(0,len(cases)-canonical_cases),'production_shape':'five separate native translation units and unique inline header; no unity compilation',
 'method':'Complete authenticated original functions in Unicorn versus handwritten C, Debug and /O2, both traced and plain; full EAX except AL-only487260, ordered data reads, full fixture bytes plus padding canaries, zero data writes, cdecl stack and EBX/EBP/ESI/EDI.',
 'raw_native_result_samples':{str(k):sorted(v)[:30] for k,v in raw_native.items()},'raw_C_result_samples':{str(k):sorted(v)[:30] for k,v in raw_c.items()},
 'limitations':['Finite fixtures cover mapped physical cells and caller-supplied padding; these leaves do not validate arbitrary pointers or rotated map shape.','Radius loop proof includes actual caller limits8/16/46, no-match and edge cases; enormous overflowing radius loops are outside valid caller domain.','No rendering, camera, actor movement, whole-game completion or gameplay visual fidelity is established.'],
 'source_hashes':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in source_paths},'executable_hashes':{str(e.relative_to(ROOT)).replace('\\','/'):sha(e.read_bytes()) for e in executables},'caller_windows':callers}
report.update({'production':args.production,'compiled_C_sha256':sha(executables[0].read_bytes()),'native_exports':{name:sha((ROOT/name).read_bytes()) for name in ['metadata/map-cell-neighborhood-native.json','metadata/map-cell-rings-native.json']},'oracle_core_sha256':sha(oracle_core.encode()),'ordered_read_coverage':{'data_reads_per_trace_variant':ordered_reads,'trace_variant_comparisons':len(cases)*2,'cases_with_reads_by_function':ordered_read_cases,'native_total_instructions':sum(b for a,b in coverage.values()),'native_covered_instructions':sum(a for a,b in coverage.values())},'variants':{label:{'path':str(e.relative_to(ROOT)).replace('\\','/'),'sha256':sha(e.read_bytes()),'checked':len(cases),'instrumented_reads':k in [0,2]} for k,(label,e) in enumerate(zip(variant_labels,executables))},'frozen_manifest_sha256':sha(frozen_manifest.read_bytes()),'frozen_mapping_verified':True})
filename='map-cells-emulation'+('-partial' if args.limit else '')+'.json'
path=(ROOT/'metadata' if args.production else ROOT/'artifacts/map_cells_integration')/filename
path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','variant_comparisons','native_instruction_coverage','elapsed_seconds']}))
