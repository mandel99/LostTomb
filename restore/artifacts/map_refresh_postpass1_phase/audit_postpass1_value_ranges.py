"""Exhaust native BYTE/CW conversion ranges to justify dead signed-modulo fixups; not a C proof."""
from pathlib import Path
import hashlib,itertools,json,struct,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes();u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(max(u32(opt+56),0x1400000-base)+4095)&~4095)
for k in range(u16(pe+6)):
    q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
facts_path=ROOT/'artifacts/map_refresh_phase/map_refresh_native.json';facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN;allowed=set();decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for name in ['00479b70','0056133c']:
    for item in facts['functions'][name]['assembly']['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;decoded=list(decoder.disasm(b,a));assert b''.join(bytes(i.bytes) for i in decoded)==b;allowed.update(i.address for i in decoded)
STACK=0x34000000;vm.mem_map(STACK,4096);sp=STACK+0x800;hits=set();stop_current=0
def code(cpu,a,n,user):
    if a==stop_current:cpu.emu_stop();return
    assert a in allowed,hex(a);hits.add(a)
vm.hook_add(UC_HOOK_CODE,code)
cws=[0x7f|pc|rc|ic for pc,rc,ic in itertools.product([0,0x200,0x300],[0,0x400,0x800,0xc00],[0,0x1000])];results={};count=0
for cw in cws:
    first=[];second=[]
    for byte in range(256):
        for which in [0,1]:
            vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_FPCW,cw);vm.reg_write(UC_X86_REG_FPSW,0);vm.reg_write(UC_X86_REG_FPTAG,0xffff)
            if which==0:
                vm.mem_write(sp+0x14,struct.pack('<i',byte-1));start,end=0x479d1f,0x479d34
            else:
                vm.mem_write(0xa509bc,bytes([byte]));vm.reg_write(UC_X86_REG_EBX,0xa509bc);vm.reg_write(UC_X86_REG_EAX,0);start,end=0x479d34,0x479d6a
            stop_current=end
            try:vm.emu_start(start,0,count=100)
            except Exception as error:raise AssertionError((hex(cw),byte,which,hex(vm.reg_read(UC_X86_REG_EIP)),hex(vm.reg_read(UC_X86_REG_ESP)),hex(vm.reg_read(UC_X86_REG_EBX)))) from error
            assert vm.reg_read(UC_X86_REG_EIP)==end and vm.reg_read(UC_X86_REG_ESP)==sp and vm.reg_read(UC_X86_REG_FPCW)==cw and vm.reg_read(UC_X86_REG_FPTAG)==0xffff and vm.reg_read(UC_X86_REG_FPSW)&0x3800==0
            value=vm.reg_read(UC_X86_REG_EAX);assert vm.reg_read(UC_X86_REG_EDX)==0
            if which==0:assert 0<=value<=7;first.append(value)
            else:assert 0<=value<=12;second.append(value)
            count+=1
    results[f'{cw:04x}']={'first_by_A5_BYTE':first,'second_by_A509_BYTE':second}
report={'format':1,'passed':True,'native_sha256':PIN,'original_game_executed':False,'conversions':count,'control_words':cws,'byte_values_per_word':256,'first_range':[0,7],'second_range':[0,12],'results':results,'native_exports':{facts_path.relative_to(ROOT).as_posix():sha(facts_path)},'method':'Original exact FILD/FMUL conversion chain and original cap/divide/multiply chain, including real original56133C, executed under Unicorn for everyBYTE and24maskedCW. Used solely as numeric range evidence for unreachable signed-modulo correction paths; whole native/C algorithm proof remains separate.','status_traps_proved':False,'dead_path_reason':'The first two MOV AL fallback paths require EAX==0, giving high24zero; the third first XORs EAX,EAX. BYTE load cannot set signbit31. Thus JNS always skips all nine signed-remainder fixup instructions.'}
(HERE/'postpass1-value-ranges.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'passed':True,'conversions':count,'first_range':[0,7],'second_range':[0,12]}))
