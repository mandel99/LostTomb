"""Independent bounded native comparison of private typed scalar candidates."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[2];P=ROOT/'build/bulk-type-pilot';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ap=argparse.ArgumentParser();ap.add_argument('--exe',type=Path,required=True);ap.add_argument('--facts',type=Path,default=P/'runs/guarded4/typed-facts.json');a=ap.parse_args();exe=a.exe.resolve();facts=json.loads(a.facts.read_text());sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();pin='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert hashlib.sha256(image).hexdigest()==facts['native_sha256']==pin
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];pe=u32(image,60);opt=pe+24;base=u32(image,opt+28);table=opt+u16(image,pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(image,opt+56)+4095)&~4095)
for i in range(u16(image,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x20000);N=0x22020;START=0x6efff0
instructions=set()
for entry in ['0040fa40','0040fa70','0040fae0']:
 for q in facts['functions'][entry]['native_instructions']:
  pc=int(q['address'].split(':')[1],16);b=bytes.fromhex(q['bytes']);assert bytes(vm.mem_read(pc,len(b)))==b;instructions.add(pc)
seen=set()
def hook(cpu,pc,n,user):
 if pc==STOP:return
 assert pc in instructions;seen.add(pc)
vm.hook_add(UC_HOOK_CODE,hook)
def put(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
initial={s:bytes((i*37+s)&255 for i in range(N)) for s in [0,0x5a]}
def native(p):
 op,x,y,pitch,value,seed=p;buf=initial[seed];vm.mem_write(START,buf);vm.mem_write(0xe92e10,b'\x5a'*24);vm.mem_write(0xe38d8c,b'\xa5'*24);put(0xe92e18,0x6f0000);put(0xe38d94,pitch);sp=STACK+0x10000;put(sp,STOP);put(sp+4,x);put(sp+8,y);put(sp+12,value)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x40fa40,0x40fa70,0x40fae0][op],STOP,count=1000);assert vm.reg_read(UC_X86_REG_ESP)==sp+4 and all(vm.reg_read(r)==v for r,v in saved)
 actual=bytes(vm.mem_read(START,N));return {'globals':(bytes(vm.mem_read(0xe92e10,24))+bytes(vm.mem_read(0xe38d8c,24))).hex(),'changed':[[i,v] for i,v in enumerate(actual) if v!=buf[i]]}
cases=list(itertools.product(range(3),[0,1,31],[0,1,31],[260,1026,2048,4094],[0,1,0x55aa,0x8000,0xffff],[0,0x5a]));smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True);assert smoke.returncode==0,smoke.stdout+smoke.stderr;proc=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE);report={'passed':False,'checked':0,'native_sha256':pin,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe),'facts_path':str(a.facts),'facts_sha256':sha(a.facts),'native_instruction_coverage':[0,56],'limitations':['Raw signed C bodies and aliases retained. Valid test domain:x/y0..31,pitch260..4094,pointer006F0000,fullWORDvalues; no signed overflow occurs. Arbitrary32bit wrap outside this domain is not accepted by this candidate proof.','No product application, automatic function acceptance or caller graph reconstruction is claimed. The two quarantined receiver/pointer functions are not emitted or accepted by this pilot.','Native RET without immediate, threeargument slots, EBX/EBP/ESI/EDI preservation and native stack checked. Ghidra model label__stdcall is retained as metadata and not imposed on generated C against physicalRET behavior.'], 'original_game_executed':False,'product_written':False}
try:
 assert json.loads(proc.stdout.readline())=={'fields':6}
 for case in cases:
  proc.stdin.write((' '.join(map(str,case))+'\n').encode());proc.stdin.flush();line=proc.stdout.readline();assert line,proc.stderr.read().decode();actual=json.loads(line);expected=native(case);assert actual==expected,str(case)+' '+str(actual)+' != '+str(expected);report['checked']+=1
 assert seen==instructions;report['passed']=True
except Exception as e:report['error']=str(e)
finally:
 proc.stdin.close();proc.terminate();proc.wait(timeout=10);report['native_instruction_coverage']=[len(seen&instructions),len(instructions)];report['source_hashes']={p.relative_to(ROOT).as_posix():sha(p) for p in [P/'verify_candidates.c',ROOT/'include/restore_types.h',*sorted((a.facts.parent/'candidates').glob('*.c')),*sorted((ROOT/'tools/bulk_type_export').glob('*.py')),*sorted((ROOT/'tools/bulk_type_export').glob('*.java')),*sorted((ROOT/'tools/bulk_type_export').glob('*.ps1'))]};out=P/('candidate-proof-'+exe.parent.name.lower()+'.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','native_instruction_coverage']}));print(report.get('error',''));sys.exit(0 if report['passed'] else 1)
