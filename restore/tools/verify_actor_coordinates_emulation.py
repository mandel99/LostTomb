"""Complete actor offset/inverse/interpolation chain with the actual accepted world helper."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
MASK=0xffffffff;OUT=0x31000000;STOP=0x33000000;STACK=0x34000000
sha=lambda b:hashlib.sha256(b).hexdigest();s16=lambda v:((v&65535)^32768)-32768
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(OUT,4096),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
packet=json.loads((ROOT/'metadata/actor-coordinates-next-native.json').read_text())['functions']
helper=json.loads((ROOT/'metadata/camera-sidebar-coordinate-native.json').read_text())['functions']['004f0ba0']
functions={int(fn,16):f['assembly']['items'] for fn,f in packet.items()};functions[0x4f0ba0]=helper['assembly']['items']
allowed=sorted(int(i['address'],16) for a in functions.values() for i in a);allowed_set=set(allowed)
for body in functions.values():
    for i in body:
        b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
hits=set();block_cache={};params=None;regions=[];events=[];callstack=[];mutation_seen=0;mutated=set();table_cache={};grid_cache={}
watched=[0x5e4ef4,0x70e44c,0x70e45c,0xbca2e0,0x70e414,0x70e418,0x70e474,0x70e478,0xbca2e8,0xbca2ec,0x70e490,0x70e48c,0xe94e04]
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def pointer_tag(a):return a if any(start<=a and a+4<=start+n for start,n in regions) else 0
def observe(address,phase,first,second,x,y):
    global mutation_seen
    r=params[2]*264&MASK;a=params[32]*388&MASK
    events.append([address,phase,first,second,pointer_tag(x),pointer_tag(y),int(x==y),read(x) if phase else 0,read(y) if phase else 0]+[read(z) for z in watched]+[read(0x7999ea+r,2),read(0x799a2c+r,1),read(0x799a40+r),read(0xbcd64a+a,2),read(0xbcd64c+a,2),read(0xbcd648+a,2)])
    if address==params[40] and phase==params[41]:
        mutation_seen+=1
        if mutation_seen==params[42]:
            bits=params[43];mutated.add((address,phase))
            if bits&1:write(0x5e4ef4,params[44])
            if bits&2:write(0xbca2e0,params[45])
            if bits&4:write(0xe94e04,params[46])
            if bits&8:write(0x7999ea+r,params[47],2);write(0x799a2c+r,params[49],1);write(0x799a40+r,params[48])
            if bits&16:write(0xbcd64a+a,params[50],2);write(0xbcd64c+a,params[51],2);write(0xbcd644+a,params[58],2);write(0xbcd648+a,params[58],2)
            if bits&32:write(0x70e45c,params[54]);write(0x70e44c,params[55]);write(0xbca2e8,params[56]);write(0xbca2ec,params[57])
            if bits&64:write(0x70e474,1);write(0x70e478,2)
            if bits&128 and phase:write(x,read(x)+params[52]);write(y,read(y)+params[53])
            if bits&256:
                r=params[25]*264&MASK;write(0x7999de+r,params[56],2);write(0x7999e0+r,params[57],2);r=params[26]*264&MASK;write(0x7999de+r,params[56]+1,2);write(0x7999e0+r,params[57]+1,2)
def entry(cpu,address,n,user):
    sp=cpu.reg_read(UC_X86_REG_ESP)
    if address==0x4f0d30:first=read(sp+4);second=0;x=read(sp+8);y=read(sp+12)
    else:first=read(sp+4);second=read(sp+8);x=read(sp+12);y=read(sp+16)
    callstack.append((address,first,second,x,y));observe(address,0,first,second,x,y)
def leave(cpu,address,n,user):
    fn,first,second,x,y=callstack.pop();assert address in returns[fn];observe(fn,1,first,second,x,y)
def block(cpu,address,n,user):
    assert address in allowed_set,'Unexpected native block '+hex(address)
    if(address,n) not in block_cache:block_cache[address,n]=allowed[bisect.bisect_left(allowed,address):bisect.bisect_left(allowed,address+n)]
    hits.update(block_cache[address,n])
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a and a+n<=STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
returns={fn:{int(i['address'],16) for i in body if i['instruction'].startswith('RET')} for fn,body in functions.items()}
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
for fn in functions:
    vm.hook_add(UC_HOOK_CODE,entry,begin=fn,end=fn)
    for a in returns[fn]:vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
def native(p):
    global params,regions,mutation_seen
    params=p;regions=[];events.clear();callstack.clear();mutation_seen=0
    def region(a,n):
        a&=MASK
        if any(start==a for start,length in regions):return
        vm.mem_write(a,bytes([(p[1]+len(regions)*29)&255])*n);regions.append((a,n))
    for a,n in [(0x5e3000,0x7000),(0x5f0000,0x8000),(0x70e400,0x775630-0x70e400),(0xbca2d0,64),(0xe94df8,32),(OUT,256)]:region(a,n)
    ids=[p[2],p[25],p[26],(p[26]-s16(p[8]))&MASK,(p[26]-s16(p[47]))&MASK]
    for value in ids:region(0x7999d0+value*264,288)
    region(0xbcd5f0+p[32]*388,412)
    if p[1] not in table_cache:
        table_cache[p[1]]=(b''.join(struct.pack('<I',(i//4+p[1])%11) for i in range(0,0x7000,4)),b''.join(struct.pack('<I',((i//4*37+p[1])%121-60)&MASK) for i in range(0,0x8000,4)))
    vm.mem_write(0x5e3000,table_cache[p[1]][0]);vm.mem_write(0x5f0000,table_cache[p[1]][1])
    for i,value in enumerate(ids):
        r=value*264&MASK
        for a,v,n in [(0x7999ea,p[8],2),(0x799a2c,p[7] if value==p[2] else(p[7]+1)%4,1),(0x799a2f,p[10],1),(0x799a30,p[11],1),(0x799a40,p[12] if value==p[2] else p[12]^1,4),(0x7999de,p[38]+i*3,2),(0x7999e0,p[39]-i*5,2),(0x7999fe,p[36]+i*7,2),(0x799a00,p[37]-i*11,2)]:write(a+r,v,n)
    index=s16(p[8]);write(0x5e4ef8+(p[7]*144+index)*8,p[9]);write(0x5e6578+(p[7]*36+index)*8,p[9])
    if p[20] not in grid_cache:grid_cache[p[20]]=struct.pack('<I',p[20])*(231*457)
    vm.mem_write(0x70e494,grid_cache[p[20]])
    for a,v in [(0x5e4ef4,p[6]),(0x70e45c,p[13]),(0x70e44c,p[14]),(0xbca2e0,p[15]),(0x70e414,p[16]),(0x70e418,p[17]),(0x70e474,p[18]),(0x70e478,p[19]),(0xbca2e8,p[21]),(0xbca2ec,p[22]),(0x70e490,p[23]),(0x70e48c,p[24]),(0xe94e04,p[31])]:write(a,v)
    r=p[32]*388&MASK
    for a,v in [(0xbcd642,p[25]),(0xbcd644,p[27]),(0xbcd646,p[26]),(0xbcd648,p[28]),(0xbcd64a,p[29]),(0xbcd64c,p[30])]:write(a+r,v,2)
    write(OUT,p[34]);write(OUT+4,p[35]);x,y=OUT,OUT+4
    if p[33]==1:y=x
    elif p[33]==2:y=OUT+2
    elif p[33]==3:x=0x70e44c
    elif p[33]==4:x=0xbca2e0
    elif p[33]==5:y=0xbca2e0
    elif p[33]==6:x=(0x799a40+p[2]*264)&MASK
    elif p[33]==7:y=(0x799a40+p[2]*264)&MASK
    elif p[33]==8:x=(0xbcd64a+p[32]*388)&MASK
    elif p[33]==9:y=0xe94e04
    for repeat in range(p[59]):
        sp=STACK+0x80000;vm.mem_write(sp-0x1000,b'\xcc'*0x1200)
        callargs=[p[2],p[3],x,y] if p[0]==0 else [p[32],x,y] if p[0]==2 else [p[4],p[5],x,y]
        for i,v in enumerate([STOP]+callargs):write(sp+i*4,v)
        saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
        for r,v in saved:vm.reg_write(r,v)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_ECX,0xbadc0de)
        vm.emu_start([0x4f08c0,0x4f0ab0,0x4f0d30,0x4f0ba0][p[0]],STOP,count=100000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP and not callstack,'Native budget/call stack'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack';assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
        assert bytes(vm.mem_read(sp+4+len(callargs)*4,0x100))==b'\xcc'*0x100,'Native caller guard'
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return{'memory':h.hexdigest(),'events':events.copy()}
default=[0]*60
for k,v in {2:1,3:1,4:228,5:230,6:3,9:1,13:29,14:15,18:10,19:5,20:0xfff0000,23:320,24:144,25:1,26:2,27:1,28:7,29:1,30:2,32:257,34:0x12345678,35:0xabcdef01,36:7,37:9,38:4,39:6,42:1,44:5,45:4,46:6,47:1,48:1,49:2,50:2,51:3,52:7,53:-9,54:61,55:31,56:13,57:17,58:13,59:1}.items():default[k]=v&MASK
cases=[]
def case(**kw):
    p=default.copy()
    for k,v in kw.items():p[int(k[1:])]=v&MASK
    cases.append(tuple(p))
case()
for layout,orientation,rotation,active,forced in itertools.product([0,1,3,5,0x100,0x105],range(11),[0,2,4,6],[0,1],[0,1]):case(p6=layout,p9=orientation,p15=rotation,p12=active,p10=0 if forced else 2,p11=1)
for step,layout,variant,part in itertools.product([-1,0,1,6,7,12,13,30,36],[0,1,3,5],[0,1,3],[-1,0,1,35,143]):case(p3=step,p6=layout,p7=variant,p8=part)
for op,alias,layout,rotation in itertools.product([0,1,2,3],range(10),[0,3,5],[0,2,4,6]):case(p0=op,p33=alias,p6=layout,p15=rotation)
for rotation,origin,row,column in itertools.product([0,2,4,6,1,0x100],[0,1,255,256,65535,0xffffffff],[-5000,0,231,457,10000],[-5000,0,230,456,10000]):case(p0=1,p15=rotation,p16=origin,p17=origin,p4=row,p5=column)
for actor,source,target,mode,fraction,total,tile in itertools.product([1,257,1000],[0,1,2],[1,2],[0,2,4,6,1],[0,1,3],[1,2],[12345,0xfff0000]):case(p0=2,p32=actor,p25=source,p26=target,p31=mode,p29=fraction,p30=total,p20=tile)
for op,rotation,tile in itertools.product([2,3],[0,2,4,6],[0,227,228,0x12345,0xfff0000,0x7fffffff,0xffffffff]):case(p0=op,p15=rotation,p20=tile)
for op,value in itertools.product([0,1,2,3],[0,1,255,256,32767,0x7fffffff,0x80000000,0xffffffff]):case(p0=op,p13=value,p14=value,p23=value,p24=value)
for fraction,total in itertools.product([-32768,-3,-1,0,1,2,32767],[1,2,32767]):case(p0=2,p29=fraction,p30=total)
for step in [37,255,256,65535,0x7fffffff]:case(p3=step,p6=3)
mutation_fixtures={0x4f08c0:{0:2},0x4f0ab0:{0:2},0x4f0ba0:{0:2},0x4f0d30:{0:2}}
for address,fixture in mutation_fixtures.items():
    for phase,occurrence,bits in itertools.product([0,1],[1,2],[1,2,4,8,16,32,64,128,256,511]):
        kw={'p'+str(k):v for k,v in fixture.items()};kw.update(p40=address,p41=phase,p42=occurrence,p43=bits);case(**kw)
for repeats in [1,2]:case(p0=2,p59=repeats,p40=0x4f08c0,p41=1,p43=511)
cases=list(dict.fromkeys(cases))
for rotation,orientation,layout in itertools.product([-3,-2,-1,1,3,7,8,255,256],range(11),[0,1,3,5]):case(p15=rotation,p9=orientation,p6=layout)
for building,layout,part in itertools.product([-1,0,257,1000],[1,3,5],[-1,0,35,143]):case(p2=building,p6=layout,p8=part)
for source,target,actor in [(257,258,1),(1000,1000,257),(0,1000,1000),(-1,2,1)]:case(p0=2,p25=source,p26=target,p2=source or target,p32=actor)
for fraction,total in [(0,0),(1,0),(32767,0),(-3,-1),(-32768,-3),(0,-1)]:case(p0=2,p29=fraction,p30=total)
for step in [0x80000000,0xffffffff]:case(p3=step,p33=1)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-actor-coordinates/Debug/actor_coordinates_verify.exe'
optimized_exe=args.optimized_exe or(None if args.exe else ROOT/'build/agent-actor-coordinates/Release/actor_coordinates_verify.exe')
oracles=[subprocess.Popen([str(p),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in [exe]+([optimized_exe] if optimized_exe else [])]
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'start':args.start,'limit':args.limit,'single_case':bool(args.case)},
 'method':'Complete403 native instructions of actor offset/inverse/interpolation plus actual113-instruction accepted world conversion against compiled C. Real composed fallbacks, ordered calls/output stores as observed at real helper boundaries, full watched memory/canaries, aliases/overlap, mutations and native/C stack/saved registers compared.',
 'limitations':['Fixtures use mapped table/record/output memory and nonfaulting signed divisions. No null-pointer or arbitrary malformed table-index contract is claimed.','Direct world-conversion and actor-composition tests use supported camera rotations0/2/4/6. Direct inverse and offset tests separately cover noncanonical rotation paths; the world helper default return-without-output path is not a separate fixture. Void incidental EAX is not asserted.']}
checked=0;p=None;started=time.monotonic()
try:
    assert all(json.loads(o.stdout.readline())=={'fields':60} for o in oracles)
    for p in cases:
        expected=native(p)
        for number,o in enumerate(oracles):
            o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
            if actual!=expected:(ROOT/'artifacts/actor_coordinates_last_difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
            assert actual==expected,repr({'oracle':number,'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):assert hits==allowed_set,'Uncovered '+repr([hex(a) for a in sorted(allowed_set-hits)]);assert mutated==set(itertools.product(mutation_fixtures,[0,1])),'Unreached helper phases'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='src' if args.production else 'artifacts/actor_coordinates_phase'
sources=[prefix+'/unit_'+n+'.c' for n in ['02113','02114','02116']]+['src/unit_02115.c','include/actor_coordinates.h','include/placement_preview.h','include/restore_types.h','include/standalone_data.h','tests/actor_coordinates.c','tests/actor_coordinates_real_offset.c','tests/actor_coordinates_real_inverse.c','tests/actor_coordinates_real_world.c','tests/actor_coordinates_real_actor.c','tools/actor_coordinates_project/CMakeLists.txt','tools/verify_actor_coordinates_emulation.py','metadata/actor-coordinates-next-native.json','metadata/camera-sidebar-coordinate-native.json']
report.update(comparison_checked=checked if optimized_exe else 0,comparison_C_sha256=sha(optimized_exe.read_bytes()) if optimized_exe else None,elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits&{int(i['address'],16) for body in packet.values() for i in body['assembly']['items']}),403],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_uncovered_instructions=[hex(a) for a in sorted(allowed_set-hits)],controlled_algorithm_endpoints=[],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/actor-coordinates-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
