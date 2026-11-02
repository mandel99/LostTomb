"""Actual remaining-menu central, dropdown and clicked-row composition, controlled deep callees."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--limit',type=int);args=parser.parse_args()
MASK=0xffffffff;PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(size,0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
FROZEN=ROOT/'artifacts/remaining_menu_actions_phase'
assert sha((FROZEN/'verify_remaining_menu_actions.py').read_bytes())=='3dca377d7560ef5ac880081ff4c24f87967f0ee15dcfe97c907b3647b9ffa6ba'
facts=json.loads((FROZEN/'remaining_menu_native.json').read_text());assert facts['native_sha256']==PIN
scope={0x406090,0x40a0e0,0x489030,0x4c7f30};core={};traced=set();abi_only=0
for name,f in facts['functions'].items():
    assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
    for i in f['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        if int(name,16) in scope:
            core[a]=name
            if name!='004c7f30':traced.add(a)
        else:abi_only+=1
context={};context_hits=set()
context_facts=json.loads((ROOT/'metadata/dropdown-menu-native.json').read_text());assert context_facts['native_sha256']==PIN
for name in ['00425f20','004260d0']:
    f=context_facts['functions'][name];assert not f['assembly']['has_more']
    for i in f['assembly']['items']:
        a=int(i['address'],16);b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;context[a]=name
boundaries={0x478420:(0,False,0),0x486e70:(0,False,0),0x4dea50:(0,False,0),0x4be160:(4,True,16),0x488e90:(5,False,20),0x4e2970:(1,False,0),0x40a160:(0,False,0),0x539170:(0,False,0)}
state=[0xe38dda,0xe38dca,0x605a3c,0xbca2d0,0xbca2d4,0xd3b198,0xeaac28,0x605a44,0xe38ea4,0xe38ebc,0xf53f48,0xd3b17c,0xd3b180]
events=[];hits=set();params=None;calls=0;running=False
def snapshot():return [read(a,1 if i<2 else 4) for i,a in enumerate(state)]
def mutate():
    if not params[10]&(1<<calls):return
    bits=params[11]
    if bits&1:put(0xbca2d0,params[12]);put(0xbca2d4,params[13])
    if bits&2:put(0xd3b198,params[14])
    if bits&4:put(0xeaac28,params[15])
    if bits&8:put(0xe38dda,params[16],1)
    if bits&16:put(0xe38ebc,params[17])
    if bits&32:put(0xd3b17c,params[18]);put(0xd3b180,params[19])
    if bits&64:put(0x605a3c,params[20])
    if bits&128:put(0xe38dca,0xa6,1);put(0xf53f48,0xa5a55a5a)
def code(cpu,a,n,user):
    global calls
    if a==STOP:return
    if a in boundaries:
        count,receiver,cleanup=boundaries[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(count)]
        events.append([2,a,cpu.reg_read(UC_X86_REG_ECX) if receiver else 0,count]+argv+snapshot())
        mutate();calls+=1
        cpu.reg_write(UC_X86_REG_EAX,0xdeadbeef);cpu.reg_write(UC_X86_REG_ECX,0xc0dec0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe)
        cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);return
    assert a in core or a in context,f'escaped native instruction {a:08x}'
    if a in core:hits.add(a)
    else:context_hits.add(a)
    if a==0x489030:events.append([3,a,cpu.reg_read(UC_X86_REG_ECX)])
def memread(cpu,access,a,n,value,user):
    if running and cpu.reg_read(UC_X86_REG_EIP) in traced and not STACK<=a<STACK+0x100000:events.append([0,a,n,read(a,n)])
def memwrite(cpu,access,a,n,value,user):
    if running and not STACK<=a<STACK+0x100000:
        assert any(lo<=a and a+n<=lo+size for lo,size in regions(params)),f'unwatched native write {a:08x}/{n} at{cpu.reg_read(UC_X86_REG_EIP):08x}'
        if cpu.reg_read(UC_X86_REG_EIP) in traced:events.append([1,a,n,value&((1<<(8*n))-1)])
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memread);vm.hook_add(UC_HOOK_MEM_WRITE,memwrite)
def regions(p):
    fixed=[(0x605a2c,48),(0xbca2c0,80),(0xd3a6d8,0xae0),(0xe38dc0,0x110),(0xeaac18,32),(0xf53f38,80),(0xd05710,48),(0xbca340,48)]
    return fixed+[((0x6fdc88+p[4]-16)&MASK,33),((0x6fdc88+p[14]-16)&MASK,33),((0xea623c+p[5]*0x49d8-16)&MASK,33),((0xea623c+p[15]*0x49d8-16)&MASK,33),(0x705700,64),(0x6f2000,64),((0x6fdc88+p[30]-16)&MASK,33)]
def native(p):
    global params,events,calls,running
    params=p;events=[];calls=0;rs=regions(p)
    for i,(a,n) in enumerate(rs):vm.mem_write(a,bytes([(p[9]+i*37)&255])*n)
    for a,v,n in [(0xe38dda,p[1],1),(0x605a3c,p[2],4),(0xe38ebc,p[3],4),(0xd3b198,p[4],4),(0xeaac28,p[5],4),(0x6fdc88+p[4],p[6],1),(0x6fdc88+p[14],p[28],1),(0xbca2d0,p[7],4),(0xbca2d4,p[8],4),(0xd3b17c,p[21],4),(0xd3b180,p[22],4),(0xe38dca,p[23],1),(0xf53f48,p[24],4),(0xea623c+p[5]*0x49d8,p[26],1),(0xea623c+p[15]*0x49d8,p[27],1)]:put(a,v,n)
    for a,v,n in [(0x6fdc88+p[30],p[32],1),(0x705728,p[31],4),(0x705724,0x6f2000,4),(0x70572c,1,4),(0x705730,1,4),(0x6f2008,0x6f2020,4),(0x6f200c,1,2),(0x6f2024,0x406090 if p[0]==0 else 0x40a0e0,4),(0x6f202c,p[30],4),(0xe38e03,1,1)]:put(a,v,n)
    sp=STACK+0xf0000;put(sp,STOP);put(sp+4,0x6f2000);put(sp+8,1);put(sp+12,0xbaadf00d)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EAX,p[25]);vm.reg_write(UC_X86_REG_ECX,p[25]^MASK);vm.reg_write(UC_X86_REG_EDX,p[25]^0x12345678);vm.reg_write(UC_X86_REG_EFLAGS,2)
    running=True
    try:vm.emu_start(0x425f20 if p[29]==4 else (0x406090 if p[0]==0 else 0x40a0e0),STOP,count=3000)
    finally:running=False
    assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(sp+4)==0x6f2000 and read(sp+8)==1 and read(sp+12)==0xbaadf00d
    assert all(vm.reg_read(r)==v for r,v in saved)
    h=hashlib.sha256()
    for a,n in rs:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'result':vm.reg_read(UC_X86_REG_EAX) if p[29]==4 else 0,'memory':h.hexdigest(),'events':events.copy()}
default=[0,1,1,1,7,2,0,228,228,17,0,0,123,124,6,3,0,0,3,1,0,3,0,0,6,0xdeadbeef,255,128,0,0,2,1,0]
cases=[]
def add(**kw):
    p=default.copy()
    for name,v in kw.items():p[int(name[1:])]=v&MASK
    cases.append(p)
# All five real C entry paths; ordinary disjoint prepared bar/item tables and valid indices.
for action,route,mode,restored,gate,flag,player in itertools.product(range(2),range(5),[0,1,3,5],[0,1,5],[0,1],[0,1],range(4)):
    add(p0=action,p29=route,p1=mode,p31=restored,p3=gate,p6=flag,p28=flag,p32=flag,p5=player)
# Captured fields versus post-endpoint reloads, including close539170, every mutation bit and combinations.
for action,route,mask,bits in itertools.product(range(2),range(5),[1,2,4,8,16,31],[1,2,4,8,16,32,64,128,255]):
    add(p0=action,p29=route,p1=5 if action else 1,p31=5 if action else 1,p3=0 if action else 1,p10=mask,p11=bits,p22=1)
# Published argument differs from prior selected value, every ordinary player and callback.
for action,arg,player,guard,saved,media in itertools.product(range(2),range(8),range(4),[0,1],[0,1,5],[0,1]):
    add(p0=action,p29=4,p30=arg,p5=player,p2=guard,p21=saved,p22=media,p1=3,p31=1)
rng=random.Random(0x425f20)
for i in range(400):
    add(p0=rng.randrange(2),p29=rng.randrange(5),p1=rng.choice([0,1,3,5]),p31=rng.choice([0,1,5]),p2=rng.randrange(2),p3=rng.randrange(2),p4=rng.randrange(8),p14=rng.randrange(8),p30=rng.randrange(8),p5=rng.randrange(4),p15=rng.randrange(4),p6=rng.randrange(2),p28=rng.randrange(2),p32=rng.randrange(2),p9=rng.randrange(256),p10=rng.randrange(32),p11=rng.randrange(256),p7=rng.randrange(1,229),p8=rng.randrange(1,229),p12=rng.randrange(1,229),p13=rng.randrange(1,229),p16=rng.choice([0,1,3,5]),p17=rng.randrange(2),p18=rng.choice([0,1,5]),p19=rng.randrange(2),p20=rng.randrange(2),p21=rng.choice([0,1,5]),p22=rng.randrange(2),p24=rng.randrange(7))
complete=len(cases)
if args.limit:cases=cases[:args.limit]
executables=[HERE/'build'/cfg/f'remaining_menu_click_{kind}.exe' for cfg in ['Debug','Release'] for kind in ['trace','plain']]
files=[HERE/'remaining_menu_click_fixture.c',HERE/'CMakeLists.txt',HERE/'shared_targets.cmake',HERE/'CONTRACT.md',Path(__file__),FROZEN/'remaining_menu_native.json',ROOT/'metadata/dropdown-menu-native.json']+[ROOT/n for n in ['src/unit_00049.c','src/unit_00077.c','runtime/remaining_menu_actions.c','include/remaining_menu_actions.h','src/unit_01628.c','runtime/menu_callbacks.c','runtime/dropdown_menu_boundary.c','src/unit_00410.c','src/unit_00413.c','include/dropdown_menu.h','include/menu_input.h']]
relative=lambda p:str(p.relative_to(ROOT)).replace('\\','/')
library=ROOT/'build/c-only-debug/Debug/pharaoh_restored.lib'
source_before={relative(p):sha(p.read_bytes()) for p in files}
library_before=sha(library.read_bytes())
map_bindings={}
expected_symbols={'_restore_menu_callback':'pharaoh_restored:menu_callbacks.obj','_restore_dropdown_callback':'pharaoh_restored:dropdown_menu_boundary.obj','_FUN_00425f20':'pharaoh_restored:unit_00410.obj','_FUN_004260d0':'pharaoh_restored:unit_00413.obj','_FUN_00406090':'unit_00049.obj','_FUN_0040a0e0':'unit_00077.obj','_restore_remaining_menu_action':'remaining_menu_actions.obj','_FUN_004c7f30':'unit_01628.obj'}
expected_symbols.update({f'_FUN_{a:08x}':'remaining_menu_click_fixture.obj' for a in boundaries})
for e in executables:
    mp=e.with_suffix('.map');lines=mp.read_text().splitlines();bindings={}
    for sym,owner in expected_symbols.items():
        matches=[ln for ln in lines if sym in ln.split()]
        assert len(matches)==1 and matches[0].split()[-1]==owner,(mp,sym,matches)
        bindings[sym]=owner
    map_bindings[relative(mp)]={'sha256':sha(mp.read_bytes()),'symbols':bindings}
procs=[subprocess.Popen([str(e),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for e in executables]
started=time.monotonic();counts={'00406090':0,'0040a0e0':0};endpoint_counts={f'{a:08x}':0 for a in boundaries}
try:
    for proc in procs:assert json.loads(proc.stdout.readline())=={'fields':33}
    for i,p in enumerate(cases):
        expected=native(p);counts['00406090' if p[0]==0 else '0040a0e0']+=1
        for e in expected['events']:
            if e[0]==2:endpoint_counts[f'{e[1]:08x}']+=1
        line=' '.join(map(str,p))+'\n'
        for k,proc in enumerate(procs):
            proc.stdin.write(line);proc.stdin.flush();out=proc.stdout.readline();assert out,'C exited: '+proc.stderr.read();actual=json.loads(out)
            if k in [1,3]:
                actual['events']=[e for e in actual['events'] if e[0]==2]
                want={'result':expected['result'],'memory':expected['memory'],'events':[e for e in expected['events'] if e[0]==2]}
            else:want=expected
            assert actual==want,json.dumps({'case':i,'p':p,'variant':str(executables[k]),'actual':actual,'expected':want})
        if (i+1)%3000==0:print(f'checked {i+1}/{len(cases)}',flush=True)
finally:
    for proc in procs:proc.stdin.close();proc.wait(timeout=20);assert proc.returncode==0,proc.stderr.read()
coverage={name:[sum(a in hits for a,f in core.items() if f==name),sum(f==name for f in core.values())] for name in facts['algorithm_scope']}
if not args.limit:
    assert len(cases)==3628 and all(a==b for a,b in coverage.values()) and sum(b for a,b in coverage.values())==70
    assert sum(a in context_hits for a in context)==35
    assert all(any(p[29]==route for p in cases) for route in range(5))
report={'format':1,'passed':True,'production':False,'product_applied':True,'checked':len(cases),'complete_cases':complete,'corpus_sha256':sha(json.dumps(cases,separators=(',',':')).encode()),'comparisons':len(cases)*4,'native_sha256':PIN,'original_game_executed':False,'method':'Complete native49/77+489030+4C7F30 versus applied C bodies, actual library central/dropdown dispatchers and425F20/4260D0 selected-row route. Debug/O2 callback units with stable Debug library. Controlled8 deep endpoints preserve identical local-proof mutations.','route_counts':{name:sum(p[29]==i for p in cases) for i,name in enumerate(['direct','remaining_resolver','central','dropdown','clicked_row'])},'per_controller':counts,'coverage':coverage,'native_algorithm_instructions':70,'click_context_coverage':{name:[sum(a in context_hits for a,f in context.items() if f==name),sum(f==name for f in context.values())] for name in ['00425f20','004260d0']},'abi_only_instruction_bytes_authenticated':abi_only,'controlled_boundaries':endpoint_counts,'compiled_C_sha256':sha(executables[0].read_bytes()),'variants':{label:{'path':relative(e),'sha256':sha(e.read_bytes()),'checked':len(cases)} for label,e in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],executables)},'source_hashes':source_before,'link_map_bindings':map_bindings,'native_exports':{relative(p):sha(p.read_bytes()) for p in [FROZEN/'remaining_menu_native.json',ROOT/'metadata/dropdown-menu-native.json']},'linked_library':{'path':relative(library),'sha256':library_before,'configuration':'Debug (stable root122 build); target-local callback/close/controller fixture units Debug or O2'},'elapsed_seconds':round(time.monotonic()-started,2),'limitations':['Eight deep algorithms are controlled boundaries, not proved implementations; actual production facades call existing real C functions. 488E90/FPU/width repair remains unfinished mandatory scope.','Actual C central/dropdown router paths are executed; original native callbacks are directly called by native indirect dispatch. Selected valid clicked-row branch only: no hover, cancel or keep-open branch claim.','Read/write trace covers callback49/77 and489030 facade, while context and close internal stores are checked through guarded final state and endpoint snapshots.','All physical indices ordinary valid; prepared bar/item tables disjoint from profile/player/global regions. Lookup guard windows may overlap as normal nearby array elements.','No IO, UI fidelity, text, full menu or whole-game completion claim.']}
report['source_drift']=[relative(p) for p in files if sha(p.read_bytes())!=source_before[relative(p)]]
report['library_drift']=sha(library.read_bytes())!=library_before
assert not report['source_drift'] and not report['library_drift']
out=HERE/('remaining-menu-click-emulation-partial.json' if args.limit else 'remaining-menu-click-emulation.json');out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','click_context_coverage','elapsed_seconds']}))
