"""Complete flood-setting methods with real native/C RNG and classifier.

No substituted algorithms or platform endpoints. Native rand masks its result
to15 bits, making the two signed-remainder fixups unreachable. All99 annual
roll values and all8 effect magnitudes execute through actual generators.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import UC_HOOK_MEM_WRITE

Q,G=S+0x1000,0xd38360
NAMES={0x4bd330:'restore_flood_settings_initialize',0x4bd360:'restore_flood_settings_reset',
       0x4bd380:'restore_flood_settings_roll_quality',0x4bd550:'restore_flood_settings_prepare',
       0x4bd580:'restore_flood_settings_restore_timing',0x4bd590:'restore_flood_quality',
       0x4bd5d0:'restore_flood_settings_reset_quality',0x4bd5e0:'restore_god_settings_004bd5e0',
       0x4bed00:'restore_flood_quality_decrease',0x4bed40:'restore_flood_quality_increase',
       0x562348:'FUN_00562348',0x4c5120:'FUN_004c5120'}
SIZES={0x4bd330:46,0x4bd360:28,0x4bd380:451,0x4bd550:39,0x4bd580:12,0x4bd590:61,
       0x4bd5d0:7,0x4bd5e0:40,0x4bed00:49,0x4bed40:42,0x562348:30,0x4c5120:65}
EXPLICIT={0x4bd330,0x4bd360,0x4bd380,0x4bd550,0x4bd580}
PLAIN={0x562348,0x4c5120};RETURNS={0x4bd590,*PLAIN};WORLD={0x4bed00,0x4bed40}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
supported={a for a,n in NAMES.items() if available(n)}
legacy={a for a in NAMES if available('FUN_%08x'%a)}
if '--require-all' in sys.argv:assert supported==legacy==set(NAMES),(supported,legacy)
owned={};branches={};hits=set();edges=set();previous=[None];invocations=0
for a,n in SIZES.items():
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
    if a==0x562348:assert any(i.address==0x562360 and i.mnemonic=='and' and i.op_str=='eax, 0x7fff' for i in code)
points=set.union(*owned.values())
for c,u,state in machines:
    reset(u,state);state['writes']=[]
    def writes(cpu,access,at,size,value,data):
        if not 0x30000000<=at<0x30010000:data['writes'].append((at,size,value))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def visit(cpu,pc,size,data):
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,visit)

def invoke(c,u,token,obj,args,mode):
    global invocations
    invocations+=1;receiver=(obj-0x32c40)&0xffffffff if token in WORLD else obj
    if mode=='named':entry=sym(NAMES[token]);argv=args if token in PLAIN else [receiver,*args]
    else:entry=address(c,token);argv=[obj,*args] if c and token in EXPLICIT else args
    for reg,value in zip(registers,[0x12345678,receiver,0x789abcde,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=200000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode)
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+(4 if not c and token==0x4bd5e0 else 0)
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    return u.reg_read(UC_X86_REG_EAX) if token in RETURNS else None

def exercise(token,quality=50,base_quality=90,amount=0,a=0x12345678,b=0x87654321,index=0,
             seed=1,kind=60,flag=1,player=0,alignment=0,global_object=False,alias=None):
    if token not in supported:return 0
    obj=alias if alias is not None else G if global_object else Q+alignment
    expected=None
    for c,u,state in machines:
        modes=['legacy'] if not c else ['named']+(['legacy'] if token in legacy and token not in PLAIN and (token!=0x4bd590 or global_object) else [])
        for mode in modes:
            u.mem_write(0xd3b1c0,b'\x6b'*0x1cc)
            u.mem_write(obj-16,b'\xa5'*68)
            for i,v in enumerate([180,70,100,0x11111111,0x22222222,base_quality,0x44444444,quality,0x66666666]):put(u,obj+i*4,v)
            for at,value in [(0xd3b1c4,index),(0xd3b374,0x1234567f),(0xd3b384,a),(0xd3b380,b),
                             (0x604950,seed),(0xeaac28,player),(0xea5f9c+player*0x49d8,3)]:put(u,at,value)
            word(u,0xea5ce8+player*0x49d8,flag);word(u,0x7999e8+3*264,kind)
            state['writes']=[]
            args=[amount] if token==0x4bd5e0 else [3] if token==0x4c5120 else []
            value=invoke(c,u,token,obj,args,mode)
            observed=(value,tuple(state['writes']),bytes(u.mem_read(obj-16,68)),bytes(u.mem_read(0xd3b1c0,0x1cc)),
                      get(u,0x604950),get(u,0xeaac28),bytes(u.mem_read(0xea5ce8+player*0x49d8,2)),get(u,0xea5f9c+player*0x49d8),bytes(u.mem_read(0x7999e8+3*264,2)))
            if expected is None:expected=observed
            else:assert observed==expected,('flood settings',hex(token),quality,base_quality,amount,hex(a),hex(b),index,seed,kind,flag,player,alignment,global_object,alias,mode,observed[:2],expected[:2])
    return 1

def mix(value):
    for _ in range(31):value=(value>>1)|((((value>>4)^value)&1)<<30)
    return value
def annual_seeds():
    found={}
    for i in range(10000):
        value=(i*0x9e3779b9)&0xffffffff;roll=(mix(value)&0x7fff)%99+1;found.setdefault(roll,value)
        if len(found)==99:break
    assert len(found)==99
    return found
def effect_seeds():
    found={}
    for seed in range(1000):
        sample=(((seed*0x343fd+0x269ec3)&0xffffffff)>>16)&0x7fff;found.setdefault(sample%8,seed)
        if len(found)==8:break
    assert len(found)==8
    return found

if __name__=='__main__':
    total=0
    for token,alignment,quality in itertools.product([0x4bd330,0x4bd360,0x4bd580,0x4bd5d0],range(4),[-1000,0,50,0x7fffffff]):
        total+=exercise(token,alignment=alignment,quality=quality,base_quality=quality)
    for quality,amount in itertools.product([-2147483648,-1000,-1,0,50,100,101,2147483647],[-2147483648,-101,-1,0,1,101,2147483647]):
        total+=exercise(0x4bd5e0,quality=quality,amount=amount,alignment=3)
    for quality,kind in itertools.product([-2147483648,-1000,0,90,99,100,101,2147483647],[0,60,61,62,63,64,65,66,67,68,69,65535]):
        total+=exercise(0x4bd590,base_quality=quality,kind=kind,player=kind%4)
    for quality,flag,player,global_object in itertools.product([-1,95,2147483647],[0,0x100],[0,3],[False,True]):
        total+=exercise(0x4bd590,base_quality=quality,flag=flag,player=player,global_object=global_object)
    seeds=annual_seeds()
    for roll,a in sorted(seeds.items()):
        for token in [0x4bd380,0x4bd550]:total+=exercise(token,a=a,b=a^0xffffffff,index=[0,98,99][roll%3],kind=60+roll%10,flag=roll%2)
    for quality,a in itertools.product([-2147483648,-1000,0,100,2147483647],[0,1,0xffffffff,0x80000000]):
        total+=exercise(0x4bd380,quality=quality,a=a,b=a,index=99)
    for token,seed,quality,global_object in itertools.product([0x4bed00,0x4bed40],effect_seeds().values(),[-2147483648,-1000,0,50,100,2147483647],[False,True]):
        total+=exercise(token,seed=seed,quality=quality,global_object=global_object)
    for token,alias in itertools.product([0x4bd380,0x4bd550],[0xd3b364-28,0xd3b380-28,0xd3b1c4-28]):
        total+=exercise(token,alias=alias,index=99)
    for token in [0x4bed00,0x4bed40]:total+=exercise(token,alias=0x604950-28,seed=0xffffffff)
    for seed in [0,1,0x7fffffff,0x80000000,0xffffffff]:total+=exercise(0x562348,seed=seed)
    for kind in [0,59,*range(60,70),70,0x8000,0xffff]:total+=exercise(0x4c5120,kind=kind)
    # These paths are structurally impossible with the actual composed rand
    # (AND EAX,7FFF) and annual roll1..99 with cumulative weight100.
    impossible_instructions={0x4bed12,0x4bed13,0x4bed16,0x4bed4f,0x4bed50,0x4bed53}
    impossible_edges={(0x4bed10,0x4bed12),(0x4bed4d,0x4bed4f),(0x4bd515,0x4bd517)}
    expected_points=set.union(*(owned[a] for a in supported))-impossible_instructions
    checked_branches={a:b for a,b in branches.items() if a in expected_points}
    assert expected_points<=hits,('unexecuted reachable instructions',[hex(a) for a in sorted(expected_points-hits)])
    missing={(a,b) for a,targets in checked_branches.items() for b in targets if (a,b) not in edges}-impossible_edges
    assert not missing,('unexecuted reachable branches',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/flood_settings.h','runtime/flood_settings.c','runtime/flood_quality.c','runtime/flood_quality_effects.c','runtime/god_city.c',
             'src/unit_01602.c','src/unit_02825.c','src/unit_00768.c','src/unit_01821.c','CMakeLists.txt','direct_c/verify_flood_settings.py']
    sources += [f'src/unit_{i:05d}.c' for i in [1542,1543,1544,1545,1546,1547,1548,1549,1567,1568]]
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
        passed=True,checked=total,invocations=invocations,methods_compared=[hex(a) for a in sorted(supported)],legacy_entries_linked=[hex(a) for a in sorted(legacy)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        coverage={hex(a):dict(bytes=SIZES[a],instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
        unreachable_instructions=[hex(a) for a in sorted(impossible_instructions&set.union(*(owned[a] for a in supported)))],
        unreachable_edges=[(hex(a),hex(b)) for a,b in sorted(impossible_edges) if a in expected_points],
        controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        scope=['Ten whole settings/effect methods and actual rand/building classifier when linked',
               'Exact field/global write order, all99 annual rolls/all8 effect magnitudes, signed clamps/overflow and bonus classification',
               'Original field prefixes preserve ninth DWORD; explicit private/global/unaligned receivers and selected RNG/object aliases'],
        limitations=['Valid mapped settings/player/building records and history indices0..99; no concurrency.',
                     'Dead sign-fixups proved by genuine15-bit rand mask; impossible final table fallthrough proved by roll bound and cumulative100.',
                     'Does not approve the entire mission loader, event caller or full flood simulation.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/flood-settings-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS complete flood settings, real generators/classifier and field aliases:',total,flush=True)
    print('Native executed/structurally unreachable instructions:',len(hits),len(proof['unreachable_instructions']),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
