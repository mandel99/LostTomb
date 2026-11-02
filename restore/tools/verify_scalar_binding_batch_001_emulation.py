"""Whole original WORD pixel leaves versus six-site transformed C, Debug/O2."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;HERE=ROOT/'artifacts/scalar_binding_batch_001'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_PROT_NONE
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--production',action='store_true')
parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path)
parser.add_argument('--limit',type=int);args=parser.parse_args()
MASK=0xffffffff;PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';sha=lambda b:hashlib.sha256(b).hexdigest()
frozen_manifest=HERE/'scalar_binding_batch_001_apply_map.json'
assert sha(frozen_manifest.read_bytes())=='6c63c87f78bf3cbc9e87c0fc5cc841f3991a097ceec1e1e4de19b3bd85e1ef23','frozen scalar manifest drift'
manifest=json.loads(frozen_manifest.read_text())
for name,want in manifest['proof_hashes'].items():assert sha((ROOT/name).read_bytes())==want,name
for mapping in manifest['mappings']:
    assert sha((ROOT/mapping['source']).read_bytes())==mapping['apply_sha256'],mapping['source']
    assert sha((ROOT/mapping['rollback_source']).read_bytes())==mapping['rollback_sha256'],mapping['rollback_source']
# Compare the complete emulator and corpus to their immutable original block.
# Changes below are executable routing, provenance and report plumbing only.
original_tool=(HERE/'verify_guarded_pixels.py').read_text();new_tool=Path(__file__).read_text()
oracle_start="u16=lambda p:struct.unpack_from('<H',image,p)[0]";oracle_end='if args.limit:cases=cases[:args.limit]'
oracle_core=original_tool.split('\n'+oracle_start,1)[1].split('\n'+oracle_end,1)[0]
assert new_tool.split('\n'+oracle_start,1)[1].split('\n'+oracle_end,1)[0]==oracle_core,'frozen native oracle/corpus drift'
product_files=[ROOT/m['target'] for m in manifest['mappings']]
if args.production:
    assert args.exe and args.optimized_exe,'production requires Debug --exe and Release --optimized-exe'
    for mapping,path in zip(manifest['mappings'],product_files):
        assert path.is_file() and sha(path.read_bytes())==mapping['apply_sha256'],'unapplied/changed product unit: '+str(path)
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
ALLOCATION=0x30000000;BUFFER=ALLOCATION+4096;CAP=65536;STACK=0x34000000;STOP=0x33000000
vm.mem_map(ALLOCATION,CAP+8192);vm.mem_protect(ALLOCATION,4096,UC_PROT_NONE);vm.mem_protect(BUFFER+CAP,4096,UC_PROT_NONE)
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
regions=[(BUFFER,CAP),(0xe38d84,36),(0xe92e08,36)]
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
data=json.loads((HERE/'scalar_binding_native.json').read_text());assert data['native_sha256']==PIN
entries=[0x40fa40,0x40fa70,0x40fae0];instructions={}
for key,e in data['functions'].items():
    assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
    assert e['info']['address']==key and int(e['info']['body_start'],16)==int(key,16)
    for item in e['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        instructions[a]=key
for sites in data['callers'].values():
    for site in sites:
        for i in site['following']:
            b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
hits=set();reads=[];writes=[];running=False
def code(cpu,a,n,user):
    if a in instructions:hits.add(a)
def memory_read(cpu,access,a,n,v,user):
    if running and not STACK<=a<STACK+0x100000:reads.append([a,n,read(a,n)])
def memory_write(cpu,access,a,n,v,user):
    if running and not STACK<=a<STACK+0x100000:writes.append([a,n,v])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory_read);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
patterns={seed:bytes((i*37+seed)&255 for i in range(256))*256 for seed in range(256)}
def digest():
    h=hashlib.sha256()
    for a,n in regions:h.update(vm.mem_read(a,n))
    return h.hexdigest()
def native(p):
    global running
    op,x,y,pitch,pointer,mask,offset,seed,initial=p;target=BUFFER+offset
    assert ((pointer+x*2+y*pitch)&MASK)==target
    vm.mem_write(BUFFER,patterns[seed]);vm.mem_write(0xe38d84,b'\xa5'*36);vm.mem_write(0xe92e08,b'\x5a'*36)
    write(target,initial,2);write(0xe38d94,pitch);write(0xe92e18,pointer)
    before=[bytes(vm.mem_read(a,n)) for a,n in regions];reads.clear();writes.clear();sp=STACK+0xf0000
    stack=[STOP,x,y,mask,0xbaadf00d]
    for k,v in enumerate(stack):write(sp+k*4,v)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_ECX,0xbadc0de);vm.reg_write(UC_X86_REG_EDX,0x9abcdef0);vm.reg_write(UC_X86_REG_EFLAGS,2)
    running=True
    try:vm.emu_start(entries[op],STOP,count=1000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(vm.reg_read(r)==v for r,v in saved)
    assert [read(sp+k*4) for k in range(len(stack))]==stack,'cdecl argument/stack canaries changed'
    result=(mask if op==0 else initial&mask if op==1 else initial|mask)&65535
    assert writes==[[target,2,result]],writes
    assert reads[:2]==[[0xe92e18,4,pointer],[0xe38d94,4,pitch]],reads
    assert reads[2:]==([] if op==0 else [[target,2,initial&65535]]),reads
    expected=bytearray(before[0]);expected[offset:offset+2]=result.to_bytes(2,'little')
    assert bytes(vm.mem_read(BUFFER,CAP))==expected
    assert all(bytes(vm.mem_read(a,n))==blob for (a,n),blob in zip(regions[1:],before[1:]))
    return {'memory':digest()}
cases=[]
def case(op,x=3,y=2,pitch=1024,mask=0xffff,offset=8192,seed=0,initial=0xa55a):
    x&=MASK;y&=MASK;pitch&=MASK;mask&=MASK;initial&=MASK
    # x*2 must be representable as signed int in the unchanged C body.
    signed_x=(x^0x80000000)-0x80000000;assert -0x40000000<=signed_x<=0x3fffffff
    pointer=(BUFFER+offset-x*2-y*pitch)&MASK
    cases.append([op,x,y,pitch,pointer,mask,offset,seed&255,initial])
masks=list(range(256))+[0xff00|n for n in range(256)]+[1<<k for k in range(16)]+[0x7fff,0x8000,0x5555,0xaaaa,0x55aa,0xaa55,0x80000000,0xffff0000,0xffffffff,0xa5a51234]
initials=[0,1,255,256,0x5555,0xaaaa,0x7fff,0x8000,0xffff]
for op,mask,initial in itertools.product(range(3),masks,initials):case(op,mask=mask,initial=initial,seed=(mask^initial)&255)
for op,x,y,pitch in itertools.product(range(3),[-0x40000000,-32768,-1,0,1,255,32767,0x3fffffff],[0,1,-1,0x40000000,0x80000000],[0,1,2,3,512,1024,4096,0x10000,0x80000000,0xfffffffe,0xffffffff]):
    case(op,x,y,pitch,mask=0xdead1234,offset=8193,seed=(x^y^pitch)&255,initial=0xface)
for op,offset in itertools.product(range(3),[0,1,2,4094,4095,4096,4097,8191,8192,65532,65533,65534]):
    for pitch in [0,511,512,1024,0xffffffff]:case(op,pitch=pitch,mask=0x7bef,offset=offset,initial=0xa55a,seed=offset&255)
rng=random.Random(0x40fa40)
for k in range(500):case(rng.randrange(3),rng.randrange(-100000,100000),rng.randrange(-2147483648,2147483648),rng.getrandbits(32),rng.getrandbits(32),rng.randrange(CAP-1),rng.randrange(256),rng.getrandbits(32))
if args.limit:cases=cases[:args.limit]
executables=[args.exe or HERE/'build/Debug/scalar_binding_pixels_verify.exe',args.optimized_exe or HERE/'build/Release/scalar_binding_pixels_verify.exe']
executables=[p.resolve() for p in executables]
if args.production:
    assert all(p.name.lower()=='restore_scalar_binding_batch_001_verify.exe' for p in executables),'production requires actual shared target basenames'
    assert executables[0]!=executables[1],'Debug and optimized executable paths must differ'
    assert all('scalar_binding_batch_001/build' not in p.as_posix() for p in executables),'private stage binaries are not production evidence'
processes=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
started=time.monotonic();counts={f'{a:08x}':0 for a in entries};p=None
try:
    for process in processes:assert json.loads(process.stdout.readline())=={'fields':9}
    for index,p in enumerate(cases):
        expected=native(p);counts[f'{entries[p[0]]:08x}']+=1;line=' '.join(map(str,p))+'\n'
        for k,process in enumerate(processes):
            process.stdin.write(line);process.stdin.flush();output=process.stdout.readline();assert output,'C exited: '+process.stderr.read()
            actual=json.loads(output);assert actual==expected,{'case':index,'p':p,'exe':str(executables[k]),'actual':actual,'expected':expected}
        if (index+1)%5000==0:print(f'checked {index+1}/{len(cases)}',flush=True)
finally:
    for process in processes:
        process.stdin.close();process.wait(timeout=20);assert process.returncode==0,process.stderr.read()
coverage={fn:[sum(a in hits for a,f in instructions.items() if f==fn),sum(f==fn for f in instructions.values())] for fn in counts}
if not args.limit:assert all(a==b for a,b in coverage.values())
plan=json.loads((HERE/'six_site_plan.json').read_text())
for e in plan['plan']:
    assert sha((ROOT/e['original_copy']).read_bytes())==e['before_sha256']
    assert sha((ROOT/e['patched_copy']).read_bytes())==e['after_sha256']
source_files=(product_files if args.production else [HERE/'patched'/f'unit_{n:05}.c' for n in [123,124,126]])+[HERE/'guarded_pixel_fixture.c',HERE/'shared_targets.cmake',HERE/'transform_scalar_bindings.py',HERE/'six_site_plan.json',HERE/'scalar_binding_native.json',HERE/'verify_guarded_pixels.py',frozen_manifest,Path(__file__),ROOT/'include/restore_types.h',ROOT/'include/restore_intrinsics.h',ROOT/'include/standalone_data.h']
report={'format':1,'passed':True,'checked':len(cases),'variant_comparisons':len(cases)*2,'per_function':counts,'native_sha256':PIN,'original_game_executed':False,'product_sources_applied':False,'algorithm_endpoints':[],'native_instruction_coverage':coverage,'native_instruction_total':sum(b for a,b in coverage.values()),'native_instruction_covered':sum(a for a,b in coverage.values()),'compiled_C_sha256':sha(executables[0].read_bytes()),'variants':{cfg:{'path':str(e.relative_to(ROOT)).replace('\\','/'),'sha256':sha(e.read_bytes()),'checked':len(cases)} for cfg,e in zip(['Debug','O2'],executables)},'elapsed_seconds':round(time.monotonic()-started,2),'method':'Complete authenticated native 18+18+20 instruction bodies in Unicorn versus exactly six mechanically transformed bindings, three separate C units, Debug and /O2. Complete64KiB framebuffer+globalguard SHA256, exact single native WORD mutation/read footprint, C/native expected WORD, inaccessible surrounding pages, cdecl stack/argument canaries and saved registers.','source_hashes':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in source_files},'native_exports':{str((HERE/'scalar_binding_native.json').relative_to(ROOT)).replace('\\','/'):sha((HERE/'scalar_binding_native.json').read_bytes())},'limitations':['The valid domain requires the final calculated WORD to be mapped and disjoint from the two globals, x*2 representable in signed32 bits, and Windows x86 support for unaligned WORD accesses. No clipping is invented.','y*pitch, pointer additions and coordinate cancellation are unsigned32 arithmetic after the explicit uint32_t binding changes; no floating point or mixed-width aliasing is used.','These are void source functions. Native caller-saved EAX/ECX/EDX scratch values are incidental and are not compared. Caller windows are retained for return-consumption review.','A binding-width scan alone does not establish equivalence; this report covers only the three complete algorithms under the documented valid memory/ABI domain. Whole-game completion is not established.']}
report.update({'production':args.production,'product_sources_applied':args.production,'frozen_manifest_sha256':sha(frozen_manifest.read_bytes()),'frozen_fixture_sha256':sha((HERE/'guarded_pixel_fixture.c').read_bytes()),'frozen_oracle_sha256':sha((HERE/'verify_guarded_pixels.py').read_bytes()),'oracle_core_sha256':sha(oracle_core.encode()),'frozen_mapping_verified':True,'scope':['0040fa40','0040fa70','0040fae0'],'canonical_cases':len(cases),'source_drift_at_completion':[str(p.relative_to(ROOT)).replace('\\','/') for p in source_files if sha(p.read_bytes())!=report['source_hashes'][str(p.relative_to(ROOT)).replace('\\','/')]]})
if not args.limit:assert len(cases)==16526 and report['native_instruction_covered']==report['native_instruction_total']==56
filename='scalar-binding-batch-001-emulation'+('-partial' if args.limit else '')+'.json'
output_directory=ROOT/'metadata' if args.production else ROOT/'artifacts/scalar_binding_batch_001_integration'
output_directory.mkdir(parents=True,exist_ok=True);path=output_directory/filename
path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','variant_comparisons','native_instruction_coverage','elapsed_seconds']}))
