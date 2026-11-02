"""Actual 53DD90 plus SG3 reader graph; filesystem/allocator endpoints only. Stdout only."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1]
frozen=ROOT/'tools/verify_sprite_reader_emulation.py'
assert hashlib.sha256(frozen.read_bytes()).hexdigest()=='faa54e77b440dc453775cb66fafdb227ff49bef5497daa3e543d08d5607490dd'
text=frozen.read_text();prefix=text.split("exe=ROOT/'build/c-only-debug/Debug/restore_sprites_verify.exe'",1)[0]
def replace_once(text,old,new):
    assert text.count(old)==1,old
    return text.replace(old,new)
prefix=replace_once(prefix,'NAME=0x30021000','NAME=0x32000000')
prefix=replace_once(prefix,'(OBJECT,0x30000)','(OBJECT,0x310000)')
prefix=replace_once(prefix,"assert path in ['data\\\\fixture.sg3','data\\\\fixture.555'],path", "assert path.lower() in ['data\\\\fixture.sg3','data\\\\fixture.555','data\\\\other.sg3','data\\\\other.555'],path\n  state['files'].append([1 if a==0x4ceb60 else 0,path.encode().hex(),args[2] if a==0x4ceb60 else args[1]])")
ns={'__file__':str(frozen),'__name__':'reader_oracle'};exec(compile(prefix,str(frozen),'exec'),ns)
vm=ns['vm'];vm.mem_map(ns['NAME'],0x1000)
ns['RANGES'] += [(0x53dd90,0x53dfc1),(0x562370,0x5623fb)]
# Reuse the frozen fixture construction, stopping before its standalone reader call.
fixture=text[text.index('def native('):text.index(' sp=STACK+0x180000')]
fixture=fixture.replace('def native(','def prepare_reader_fixture(',1)+' return state\n'
exec(compile(fixture,str(frozen),'exec'),ns)
u32=ns['u32'];read=ns['read'];write=ns['write'];OBJECT=ns['OBJECT'];NAME=ns['NAME'];HEAP=ns['HEAP']
lib_size=0xcc3c;manager_size=0x30f5d8;M=0x1f40;IM=0x180e48
hits=set();edges=set();last=None;parent={};conditional=set()
cache=ROOT/'metadata/native-function-audit-cache/batch-2624.json'
for entry in json.loads(cache.read_text())['entries']:
    if int(entry['requested_address'],16)!=0x53dd90:continue
    assert entry['status']=='exact_ghidra_function' and not entry['assembly']['has_more']
    for ins in entry['assembly']['items']:
        a=int(ins['address'],16);b=bytes.fromhex(ins['bytes']);assert bytes(vm.mem_read(a,len(b)))==b
        parent[a]=ins['instruction']
        if ins['instruction'].startswith('J') and not ins['instruction'].startswith('JMP'):
            conditional.update([(a,a+len(b)),(a,int(ins['instruction'].split()[-1],16))])
assert len(parent)==156 and parent[0x53dde9]=='MOV EAX,dword ptr [ESI + ECX*0x4 + 0xeb6c]'
def observe(cpu,a,n,unused):
    global last
    if a in parent:
        hits.add(a)
        if last is not None:edges.add((last,a))
        last=a
vm.hook_add(ns['UC_HOOK_CODE'],observe,begin=0x53dd90,end=0x53dfbf)
def original(slot,previous,old_bytes,name_mode,append):
    global last
    state=ns['prepare_reader_fixture'](slot!=0,0,2);state['files']=[]
    vm.mem_write(OBJECT,bytes(manager_size));write(0xd3b5f4,0);write(0x126648c,0)
    lib=OBJECT+M+slot*lib_size;ptr=HEAP+0x1f0000 if old_bytes else 0
    vm.mem_write(lib,b'fixture\0');vm.mem_write(ptr,b'\xa5'*old_bytes) if ptr else None
    for offset,value in [(0xcc24,ptr),(0xcc28,old_bytes),(0xcc2c,previous),(0xcc30,4),(0xcc34,11),(0xcc38,21)]:write(lib+offset,value)
    for offset,value in [(0x30f5c8,41),(0x30f5cc,31),(0x30f5d0,100+old_bytes),(0x30f5d4,slot),(0x30f548+slot*4,104 if slot else 5)]:write(OBJECT+offset,value)
    if slot:write(OBJECT+0x30f548+(slot-1)*4,100)
    vm.mem_write(NAME,[b'\0',b'fixture\0',b'FIXTURE\0',b'other\0'][name_mode])
    sp=ns['STACK']+0x180000
    for j,value in enumerate([ns['STOP'],NAME if name_mode else 0,0xffffffff if append else slot]):write(sp+j*4,value)
    saved=[(ns['UC_X86_REG_EBX'],0x12345678),(ns['UC_X86_REG_EBP'],0x87654321),(ns['UC_X86_REG_ESI'],0x11223344),(ns['UC_X86_REG_EDI'],0xaabbccdd)]
    for reg,value in saved:vm.reg_write(reg,value)
    write(0,0xffffffff);vm.reg_write(ns['UC_X86_REG_ECX'],OBJECT);vm.reg_write(ns['UC_X86_REG_ESP'],sp);last=None
    vm.emu_start(0x53dd90,ns['STOP'],count=2000000)
    assert vm.reg_read(ns['UC_X86_REG_EIP'])==ns['STOP'] and vm.reg_read(ns['UC_X86_REG_ESP'])==sp+12
    assert read(0)==0xffffffff and all(vm.reg_read(reg)==value for reg,value in saved)
    memory=bytearray(vm.mem_read(OBJECT,manager_size));base=u32(memory,M+slot*lib_size+0xcc24);size=u32(memory,M+slot*lib_size+0xcc28)
    struct.pack_into('<I',memory,M+slot*lib_size+0xcc24,int(bool(base)))
    for j in range(25500):
        offset=IM+j*64;address=u32(memory,offset)
        if base and not memory[offset+0x34] and base<=address<base+size:struct.pack_into('<I',memory,offset,address-base)
    cursor=read(0xd3b5f4)
    if base and base<=cursor<=base+size:cursor-=base
    return dict(result=vm.reg_read(ns['UC_X86_REG_EAX'])&255,manager_sha256=hashlib.sha256(memory).hexdigest(),pixels=bytes(vm.mem_read(base,size)).hex() if size else '',cursor=cursor,files=state['files'])
binary=Path(sys.argv[1]).resolve()
guard=[Path(__file__),frozen,binary,ROOT/'include/sprite_libraries.h',ROOT/'direct_c/sprite_manager_probe.c',ROOT/'direct_c/CMakeLists.txt',ROOT/'tests/sprite_libraries.c',ROOT/'runtime/sprite_libraries.c',ROOT/'runtime/host_allocator.c']
guard += [ROOT/f'src/unit_{n:05d}.c' for n in [2517,2518,2519,2520,2522,2528,2529,1733,1734,2804,2810]]
before=[hashlib.sha256(p.read_bytes()).hexdigest() for p in guard]
process=subprocess.Popen([str(binary)],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
checked=0
try:
    for case in itertools.product([0,1,7,29],[1,0,4,8],[0,16],[2,1,3,0],[0,1]):
        process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush()
        line=process.stdout.readline();assert line,('C probe stopped',process.poll(),process.stderr.read())
        actual=json.loads(line);expected=original(*case)
        assert actual==expected,(case,{k:(actual[k],expected[k]) for k in actual if actual[k]!=expected[k]})
        checked+=1
finally:
    process.stdin.close();process.wait(timeout=10)
assert process.returncode==0
assert [hashlib.sha256(p.read_bytes()).hexdigest() for p in guard]==before
print(f'PASS {checked} actual manager+reader cases;parent {len(hits)}/156 instructions;normalized whole-manager SHA256/pixels/cursor/exact ordered filesystem calls/AL/SP/nonvolatiles/SEH-chain equal')
print('Parent unexecuted:',','.join(hex(a) for a in sorted(set(parent)-hits)))
print('Warning/ostream/MessageBox and malformed-file/exception behavior remain unproved;allocator identity is checked separately by the canonical reader proof;ASCII filename/default-locale domain.')
