"""Complete tool-popup hit/selection and row enumerator versus pinned original x86."""
from pathlib import Path
import argparse,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path)
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
MASK=0xffffffff;STACK=0x34000000;STOP=0x33000000;sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
    q=table+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
functions={};packets=['tool-selector-input-native.json','tool-selector-next-native.json','tool-selector-input-helpers-native.json']
for name in packets:
    p=json.loads((ROOT/'metadata'/name).read_text());assert p['native_sha256']==digest
    for fn,f in p['functions'].items():
        body=f['assembly'];assert not body['has_more'] and len(body['items'])==body['total'];functions[int(fn,16)]=body['items']
instructions={int(i['address'],16):i for body in functions.values() for i in body}
for a,i in instructions.items():
    b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
for fn,f in json.loads((ROOT/'metadata/tool-selector-query-native.json').read_text())['functions'].items():
    for i in f['assembly']['items']:
        b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
fixed=[(0x5c2800,0x1d00),(0x5ce300,0x10000),(0x70a800,0x10000),(0x6059b0,192),(0x7056b0,96),(0xbc9700,0x3c0),(0xbca2b0,112),(0xd3b180,48),(0xe38dc0,0x100),(0x784b80,0x300),(0x5e6960,48),(0x576130,48),(0xeaac20,16),(0xe92e00,24),(0xf53f50,128),(0xe94da0,32)]
watched=[0x7056ec,0x7056f0,0x7056c8,0xd3b198,0x6059e8,0xeaac28,0xbca310,0x605a3c,0xf53f64,0xf53f60,0xf53f78,0x7056e8,0xe94dac,0xe38e34,0xe38e30,0x605a30,0x605a48,0xbca300,0x6059cc,0xbc9714]
argc={0x403e00:0,0x403f10:2,0x405200:0,0x404120:0,0x486060:1,0x53d850:0,0x426450:4,0x4d5ed0:0}
returning={0x403e00,0x403f10,0x404120,0x426450,0x4d5ed0}
controlled={0x403ef0:0,0x4f5470:1,0x4f5020:0,0x4f4ff0:0,0x4c52d0:2,0x53dd90:2}
params=None;regions=[];events=[];frames=[];seen=0;hits=set();edges=set();last=None;mutated=set();deep=set()
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def rows(category,kind,allowed):
    for j in range(31):write(0x5ce3b8+(category*30+j)*4,kind if j and j<=params[15] else 0);write(0x70a8dc+(category*30+j)*4,allowed)
def observe(fn,phase,v,result=0):
    global seen
    events.append([fn,phase]+list(v)+[0]*(4-len(v))+[result]+[read(a) for a in watched]+[read(0xe38e03,1),read(0xe38dda,1),read(0xe38dca,1)])
    p=params
    if fn==p[28] and phase==p[29]:
        seen+=1
        if seen==p[30]:
            bits=p[31];mutated.add((fn,phase))
            if bits&1:write(0x7056ec,p[32])
            if bits&2:write(0x7056f0,p[33])
            if bits&4:write(0xeaac28,p[34])
            if bits&8:write(0x6059e8,p[35])
            if bits&16:write(0xe38e34,read(0xe38e34)+p[36]);write(0xe38e30,read(0xe38e30)+p[37]);write(0xe38e03,p[45],1)
            if bits&32:write(0xf53f64,p[42]);write(0xe94dac,p[43]);write(0x7056e8,p[44])
            if bits&64:rows(p[2],p[53],p[17])
            if bits&128:write(0xd3b198,2 if fn==0x486060 or(fn==0x405200 and phase==0) else 0xfedcba98)
            if bits&256:write(0xbca310,2);write(0x7056ec,p[32])
            if bits&512:write(0x5c28a8+p[14]*28,p[38]+1,2)
ret_addresses={int(i['address'],16) for body in functions.values() for i in body if i['instruction'].startswith('RET')}
def code(cpu,a,n,user):
    global last
    if last is not None:edges.add((last,a))
    last=a;sp=cpu.reg_read(UC_X86_REG_ESP)
    if a in controlled:
        v=[read(sp+4+j*4) for j in range(controlled[a])];cleanup=0
        if a==0x53dd90:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;v=[0xf55bb0]+v;cleanup=8
        observe(a,0,v);p=params
        if a==0x403ef0:r=p[19]|(p[18]&1)
        elif a==0x4f5470:r=p[19]|((p[18]>>(1 if v[0]==0xb3 else 2 if v[0]==0xb2 else 3 if v[0]==0xb1 else 4))&1)
        elif a==0x4f5020:r=p[20]
        elif a==0x4f4ff0:r=p[21]
        elif a==0x4c52d0:r=p[19]|((p[18]>>(5 if v[1]==1 else 6 if v[1]==2 else 7))&1)
        else:r=1
        observe(a,1,v,r);deep.add(a);cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);return
    assert a in instructions,'Native escaped '+hex(a);hits.add(a)
    if a in functions:
        v=[read(sp+4+j*4) for j in range(argc[a])];frames.append((a,sp,v));observe(a,0,v)
    if a in ret_addresses:
        assert frames and frames[-1][1]==sp,'Native return frame'
        fn,_,v=frames.pop();observe(fn,1,v,cpu.reg_read(UC_X86_REG_EAX) if fn in returning else 0)
def memory_write(cpu,access,a,n,value,user):
    if STACK<=a and a+n<=STACK+0x100000:return
    assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
def native(p):
    global params,regions,seen,last
    params=p;regions=[];events.clear();frames.clear();seen=0
    for a,n in fixed+[(0xea5ba0+j*0x49d8,0x500) for j in range(8)]+[(0x7999d0+p[26]*264,280)]:vm.mem_write(a,bytes([(p[1]+len(regions)*29)&255])*n);regions.append((a,n))
    for j in list(range(38))+[256,MASK,p[2],p[32]]:rows(j,p[16],p[17])
    if p[55]!=MASK:rows(p[55],p[53],p[17]^1)
    write(0x5ce3b8+p[2]*120,p[52]);write(0x70a8d8+p[2]*120,p[17])
    for j in range(256):
        a=0x5c28a8+j*28
        v=[20000,20000,20001,20001] if p[13]==0 and j!=p[14] else [p[38],p[39]+(j*20 if p[13]==2 else 0),p[40],p[41]+(j*20 if p[13]==2 else 0)]
        for k,x in enumerate(v):write(a+k*2,x,2)
    for j in range(31):write(0x5cf63c+j*8,0x10000+j);write(0x5cf640+j*8,0x20000+j);write(0x784dde+j*2,p[49],2)
    rec=0x5d19c8+(((p[49]&65535)^32768)-32768)*16
    for k,x in enumerate(p[46:49]):write(rec+k*2,x,2)
    for j in range(8):
        off=j*0x49d8;write(0xea5baa+off,p[50] if j==p[22] else 1,2);write(0xea5ce8+off,p[24] if j==p[22] else p[24]^1,2);write(0xea5f9c+off,1000+j)
        for k in range(30):write(0xea5fa0+off+k*4,p[23])
    for a,x in [(0x7056ec,p[2] if p[55]==MASK else p[55]),(0x7056f0,p[3]),(0xf53f64,p[4]),(0xf53f60,p[5]),(0xf53f78,p[6]),(0x605a3c,p[7]),(0xe94dac,p[8]),(0x7056e8,p[9]),(0xe38e34,p[10]),(0xe38e30,p[11]),(0xeaac28,p[22]),(0xbca310,p[25]),(0xbc9800,p[26]),(0x6059e8,p[51]),(0xd3b198,1),(0xe92e0c,0x12345678),(0xbca2e0,2)]:write(a,x)
    write(0xe38e03,p[12],1)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    result=0
    for repeat in range(p[54]):
        sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
        if p[0]==1:write(sp+4,p[2]);write(sp+8,p[27])
        for reg,x in saved:vm.reg_write(reg,x)
        vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);last=None
        vm.emu_start(0x403f10 if p[0]==1 else 0x403e00,STOP,count=1000000)
        assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native execution budget/frames'
        assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native cdecl stack';assert all(vm.reg_read(reg)==x for reg,x in saved),'Native saved registers'
        guard=sp+12 if p[0]==1 else sp+4;assert bytes(vm.mem_read(guard,128))==b'\xcc'*128,'Native caller guard';result=vm.reg_read(UC_X86_REG_EAX)
    h=hashlib.sha256()
    for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
    return {'return':result,'memory':h.hexdigest(),'events':events.copy()}
default=[0]*56
for k,v in {1:1,2:1,3:1,4:1024,5:800,10:907,11:110,12:1,15:3,16:0x6a,17:256,20:1,21:2,26:257,27:-1,30:1,32:5,33:2,34:1,35:0x6a,36:1,37:-1,40:20,41:16,42:1280,43:50,44:200,45:1,49:1,50:1,51:0x12345678,53:0x61,54:1,55:MASK}.items():default[k]=v&MASK
cases=[]
def case(**kw):
    p=default.copy()
    for k,x in kw.items():p[int(k[1:])]=x&MASK
    cases.append(tuple(p))
case()
for side,mode,screen,offset,menu in itertools.product([0,1,2,256],[0,1,256],[256,1024,1280,0x7fffffff,0x80000000,0xffffffff],[0,1,-21,0x7fffffff],[0,256]):
    x=((screen-258)+161-((offset+20)&MASK))&MASK;y=(menu+110)&MASK
    if side==1 and mode!=1:x=(800-258+161-((offset+20)&MASK))&MASK
    case(p6=side,p7=mode,p4=screen,p8=offset,p9=menu,p10=x,p11=y)
for left,top,right,bottom in [(0,0,20,16),(-32768,-32768,32767,32767),(-1,-2,1,2),(10,10,0,0)]:
    for dx,dy,click in itertools.product([left-1,left,right-1,right],[top-1,top,bottom-1,bottom],[0,1,128,256]):case(p38=left,p39=top,p40=right,p41=bottom,p10=(907+dx),p11=(110+dy),p12=click)
for count,index in [(0,0),(-1,0),(1,0),(2,1),(30,29),(31,30),(256,255)]:case(p3=count,p14=index)
for category,kind,bits,allowed,built,temple in itertools.product([0,6,7,10,22,28,36,37,256],[1,4,0x30,0x61],[0,1,3,5,9,17,33,65,129,255],[0,256],[0,1],[0,256]):
    case(p0=1,p2=category,p16=kind,p18=bits,p17=allowed,p23=built,p24=temple,p15=2)
for category,kind in itertools.product(range(38),[1,0x6a,0x30,0x61]):case(p2=category,p16=kind,p18=255,p15=3,p24=256)
for previous,rows_count,kind,allowed in itertools.product([-2,-1,0,1,28,29,30],[0,1,2,30],[0,-1,1,0x100],[0,256]):case(p0=1,p27=previous,p15=rows_count,p16=kind,p17=allowed)
for category in [6,10,22,28,36]:
    for high,bits in itertools.product([0,0x100,0x80000000,0xffffff00],[0,1,255]):case(p0=1,p2=category,p16=0x30 if category==6 else 4,p19=high,p18=bits,p24=1)
for global_category,kind,allowed in itertools.product([5,6,7,256],[0x30,0x61],[0,256]):case(p0=1,p2=6,p55=global_category,p53=0x61,p16=kind,p17=allowed,p18=255,p24=0)
for fn in list(functions)+list(controlled):
    for phase,bits in itertools.product([0,1],[1,2,4,8,16,32,64,128,256,512,1023]):
        category=28 if fn in [0x53d850,0x53dd90] else 6 if fn in [0x4f5020,0x4f4ff0] else 36 if fn==0x4c52d0 else 22 if fn in [0x403ef0,0x4f5470] else 1
        kind=0x30 if category==6 or fn==0x404120 else 0x6a
        case(p2=category,p16=kind,p18=1,p28=fn,p29=phase,p31=bits,p25=1 if fn==0x486060 else 0,p47=1 if category==28 else 0)
for category in [6,10,22,28,36]:
    for target in [0x403ef0,0x4f5470,0x4c52d0]:case(p0=1,p2=category,p16=0x61,p24=1,p28=target,p29=1,p31=69,p18=255)
for have,need in [(0,0),(2,1),(0x80000000,0x7fffffff),(0x7fffffff,0x80000000),(-1,0)]:case(p0=1,p2=6,p16=0x30,p18=1,p20=have,p21=need)
for category,kind in itertools.product([256,-1],[0x6a,0x100]):case(p2=category,p16=kind)
case(p3=30,p14=29,p15=30);case(p16=0x100)
case(p0=1,p27=-2,p52=1);case(p54=2,p16=0x30);case(p3=2,p14=1,p13=2,p11=130)
cases=list(dict.fromkeys(cases))
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-tool-selector-input/Debug/tool_selector_input_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-tool-selector-input/Release/tool_selector_input_verify.exe')
oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in [exe]+([optimized] if optimized else [])]
owned={int(i['address'],16) for fn in [0x403e00,0x403f10] for i in functions[fn]}
cap=Cs(CS_ARCH_X86,CS_MODE_32);branches={}
for a in owned:
    i=next(cap.disasm(bytes.fromhex(instructions[a]['bytes']),a))
    if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]=(a+i.size,int(i.op_str,16))
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'passed':False,'checked':0,'case_selection':{'limit':args.limit,'start':args.start,'single_case':bool(args.case)},'method':'Complete native tool input and row enumeration with actual hit-test, geometry offset, selector, count, release and media controller against compiled C. Ordered boundary arguments/returns/state, full watched memory/canaries, callback mutations, cdecl stack and saved registers compared.','limitations':['Availability queries403EF0/4F5470/4F5020/4F4FF0/4C52D0 and sprite-library loader53DD90 remain explicitly controlled deepest endpoints. Their full algorithms are outside this proof.','The actual unchanged media controller is composed for players0..7 and valid saved variants0..2. Native loader cleanup8 and ECX manager receiver are asserted; the C adapter passes that receiver explicitly.','Mapped bounded category/row/table fixtures and nonfaulting memory are used. No UI event replay, actual asset loading, or complete sidebar pixels are claimed.']}
checked=0;p=None;started=time.monotonic()
try:
    assert all(json.loads(o.stdout.readline())=={'fields':56} for o in oracles)
    for p in cases:
        expected=native(p)
        for number,o in enumerate(oracles):
            o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
            if actual!=expected:(ROOT/'artifacts/tool_selector_input_last_difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
            assert actual==expected,repr({'oracle':number,'return':[actual['return'],expected['return']],'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
        checked+=1
        if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
    if not(args.limit or args.start or args.case):
        assert owned<=hits,'Uncovered owned instructions '+repr([hex(a) for a in sorted(owned-hits)])
        assert set(itertools.product(list(functions)+list(controlled),[0,1]))<=mutated,'Unreached mutation phases '+repr(set(itertools.product(list(functions)+list(controlled),[0,1]))-mutated)
        assert set(controlled)<=deep,'Unreached controlled endpoints'
        assert all((a,target) in edges for a,targets in branches.items() for target in targets),'Unreached owned conditional outcome'
        assert {b for a,b in edges if a==0x403f59}==set(struct.unpack('<6I',vm.mem_read(0x4040e8,24))),'Unreached enumerator dispatch target'
    report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
    for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='src' if args.production else 'artifacts/tool_selector_input_phase'
sources=[prefix+'/unit_'+n+'.c' for n in ['00034','00036']]+['src/unit_'+n+'.c' for n in ['00042','00037','01248','02508','00417','01840']]+['include/'+n+'.h' for n in ['tool_selector_input','tool_selector','menu_input','sprite_libraries','restore_types','standalone_data']]+['tests/tool_selector_input.c','tests/tool_selector_input_real.c','tools/tool_selector_input_project/CMakeLists.txt','tools/verify_tool_selector_input_emulation.py']+['metadata/'+name for name in packets+['tool-selector-query-native.json','tool_selector_input_callers_native.json']]
missing_edges=[(a,target) for a,targets in branches.items() for target in targets if(a,target) not in edges]
dispatch=bytes(vm.mem_read(0x4040e8,24));mapping=bytes(vm.mem_read(0x404100,31));targets=set(struct.unpack('<6I',dispatch));observed_dispatch={b for a,b in edges if a==0x403f59}
report.update(comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits&owned),len(owned)],composed_instruction_coverage=[len(hits-owned),len(set(instructions)-owned)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],owned_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],missing_owned_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],computed_dispatch={'targets_covered':len(observed_dispatch&targets),'targets_total':len(targets),'table_sha256':sha(dispatch),'map_sha256':sha(mapping)},controlled_algorithm_endpoints=[hex(a) for a in controlled],reached_controlled_endpoints=[hex(a) for a in sorted(deep)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/tool-selector-input-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions']},indent=2));sys.exit(0 if report['passed'] else 1)
