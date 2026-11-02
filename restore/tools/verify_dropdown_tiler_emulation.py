"""Whole55-instruction dropdown panel tiler against the pinned x86 body."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if Path(__file__).resolve().parent.parent.name=='dropdown_tiler_phase' else Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-dropdown-tiler/out/Debug/restore_dropdown_tiler_verify.exe');args=ap.parse_args();phase=Path('.') if args.production else Path('artifacts/dropdown_tiler_phase');exe=args.exe.resolve()
sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,60);opt=pe+24;base=u32(image,opt+28);tableoff=opt+u16(image,pe+20);size=u32(image,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 q=tableoff+i*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x200000)
def put(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def get(a):return int.from_bytes(vm.mem_read(a,4),'little')
evidence=['metadata/dropdown-menu-render-latches-native.json','metadata/world-render-sprite-core-native.json'];d=json.loads((ROOT/evidence[0]).read_text());a=d['functions']['00536990']['assembly'];assert not a['has_more'] and len(a['items'])==a['total']==55;instructions={int(q['address'],16):q for q in a['items']}
for address,q in instructions.items():assert bytes(vm.mem_read(address,len(bytes.fromhex(q['bytes']))))==bytes.fromhex(q['bytes'])
callee=json.loads((ROOT/evidence[1]).read_text())['functions']['0053e590']['assembly'];assert not callee['has_more']
for q in callee['items']:assert bytes(vm.mem_read(int(q['address'],16),len(bytes.fromhex(q['bytes']))))==bytes.fromhex(q['bytes'])
rets=[q for q in callee['items'] if q['instruction'].startswith('RET')];assert rets and all(q['instruction']=='RET 0xc' for q in rets)
regions=[(0xd3b5b8,56),(0xf56834,24)];p=[];events=[];executed=set();edges=set();last=None
cs=Cs(CS_ARCH_X86,CS_MODE_32)
def hook(cpu,a,n,user):
 global last
 if a==STOP:return
 if last is not None:edges.add((last,a))
 last=a;executed.add(a)
 if a==0x53e590:
  sp=cpu.reg_read(UC_X86_REG_ESP);receiver=cpu.reg_read(UC_X86_REG_ECX);assert receiver==0xf55bb0
  v=[get(sp+4+i*4) for i in range(3)];events.append([receiver]+v+[get(0xd3b5c0),get(0xd3b5e0),get(0xd3b5dc),get(0xf5683c),p[8]])
  if p[5]&1:put(0xf5683c,get(0xf5683c)+p[6])
  if p[5]&2:put(0xd3b5c0,p[9]);put(0xd3b5e0,p[10]);put(0xd3b5dc,p[11])
  if p[5]&4:put(0xf5683c,p[7])
  cpu.reg_write(UC_X86_REG_EAX,p[8]);cpu.reg_write(UC_X86_REG_ECX,0xdecafbad);cpu.reg_write(UC_X86_REG_EDX,0xabcdef01);cpu.reg_write(UC_X86_REG_EIP,get(sp));cpu.reg_write(UC_X86_REG_ESP,sp+16);return
 assert a in instructions,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,hook)
def native(case):
 global p,last
 p=case;events.clear();last=None
 for a,n in regions:vm.mem_write(a,bytes([(0xa5^p[12])&255])*n)
 put(0xf5683c,p[4]);sp=STACK+0x1e0000;put(sp,STOP)
 for i,v in enumerate(p[:4]):put(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(0x536990,STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee-saved registers differ'
 return {'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
fields=['x','y','width','height','artbase','mutation','increment','replacement','return_EAX','newimage','newx','newy','canary','reserved13','reserved14','reserved15'];default=[400,300,11,11,0x12345678,0,0x10203040,0xabcdef01,0xa1b2c3d4,0x12345678,0x87654321,0xfedcba98,0,0,0,0];cases=[]
def add(**kw):
 q=default.copy()
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(tuple(q))
for width,height in itertools.product([-2147483648,-1,0,1,2,9,10,11,19,20,21,31],[-2147483648,-1,0,1,2,9,10,11,19,20,21,31]):add(width=width,height=height)
for x,y,art in itertools.product([-2147483648,-65536,-32768,-1,0,255,256,400,0x7fffff80,2147483647],[-2147483648,-32768,-1,0,255,256,300,0x7fffff80,2147483647],[0,1,255,256,0x12345678,0x7fffffff,0x80000000,0xfffffff0,0xffffffff]):add(x=x,y=y,artbase=art,width=2,height=2)
for mutation,increment,replacement,ret in itertools.product(range(8),[0,1,256,0xffffffff],[0,0x12345678,0xffffffff],[0,1,256,0x80000000,0xffffffff]):add(mutation=mutation,increment=increment,replacement=replacement,return_EAX=ret,width=11,height=11)
for width,height,canary in itertools.product([-1,0,1,10,11,21],[-1,0,1,10,11,21],[0x5a,0xa5,0xff]):add(width=width,height=height,canary=canary,mutation=3)
for width,height in [(255,1),(256,1),(1,255),(1,256),(32,32),(2147483647,0),(-2147483648,1)]:add(width=width,height=height)
cases=list(dict.fromkeys(cases));smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':0,'stage':'production' if args.production else phase.as_posix(),'native_sha256':digest,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Whole55-instruction native536990 panel tiler executed and compared with handwritten C. FullDWORD imagebase/caches, exact10column/10row phases, signed count guards, explicit32bit coordinate/imagewrap, live base reload after each endpoint, ordered receiver/stackarguments, discarded fullEAX values, globals/canaries and native stack/callee-saved registers.','limitations':['Deep53E590 sprite render remains a typed controlled endpoint; its1076nativeinstructions are authenticated for entry/thiscall/RET12contract only, not executed or re-proven here. No pixel equivalence claimed.','The actual native caller explicitly bindsECX=F55BB0, pushes y/x/image and receives callee cleanup12. The accepted compiledC53E590facade takes image/x/y as cdecl and binds the same globalmanager internally. The fixture records that manager and those3values; no native pointer calls.','Positive tile extents tested through256 for individualaxes and32x32; nonpositive signedDWORDextents includeINT_MIN. Huge positive grids are outside finite fixture budget, but no count narrowing or new production bounds are introduced.','Caller-provided coordinates and spritebase are tested across all32bit wrap boundaries under the controlled-renderer contract. The actual renderer has independent image/clip preconditions; this suite does not relax them.']}
case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':16}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  assert actual==expected,'C/native mismatch '+json.dumps({'C':actual,'native':expected})[:2000]
  report['checked']+=1
 report['passed']=True
except Exception as e:report['error']=str(e);report['failed_case']=dict(zip(fields,case)) if case else None
finally:
 process.stdin.close();process.terminate();process.wait(timeout=10);conditional=[]
 for a,q in instructions.items():
  ins=next(cs.disasm(bytes.fromhex(q['bytes']),a,count=1))
  if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':conditional.extend([(a,int(ins.op_str,16)),(a,a+ins.size)])
 report['instruction_coverage']={'executed':len(set(instructions)&executed),'total':len(instructions),'missing':[f'{a:08x}' for a in sorted(set(instructions)-executed)]};report['conditional_coverage']={'executed':sum(e in edges for e in conditional),'total':len(conditional),'missing':[[f'{a:08x}',f'{b:08x}'] for a,b in conditional if (a,b) not in edges]};report['qualified_unreachable_edges']=[]
 if report['passed'] and (report['instruction_coverage']['missing'] or report['conditional_coverage']['missing']):report['passed']=False;report['error']='Native full coverage incomplete'
 paths=[phase/'include/dropdown_tiler.h',phase/'src/unit_02446.c',phase/'tests/dropdown_tiler.c',phase/'tools/verify_dropdown_tiler_emulation.py',Path('include/sprite_render.h'),Path('src/unit_02524.c')];report['source_hashes']={q.as_posix():sha((ROOT/q).read_bytes()) for q in paths};report['evidence_hashes']={q:sha((ROOT/q).read_bytes()) for q in evidence};report['native_instruction_count']=55;report['native_byte_count']=sum(len(bytes.fromhex(q['bytes'])) for q in instructions.values());report['deep_callee_contract']={'entry':'0053e590','receiver':'00f55bb0','arguments':['imageDWORD','xDWORD','yDWORD'],'cleanup_bytes':12,'native_instructions_authenticated_only':len(callee['items']),'native_return_sites':[q['address'] for q in rets]}
 out=ROOT/phase/'metadata/dropdown-tiler-emulation.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','instruction_coverage','conditional_coverage']}))
 if not report['passed']:print(report.get('error'),report.get('failed_case'));sys.exit(1)
