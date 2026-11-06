"""Offline signed percentage results, divide fault and continued context."""
from pathlib import Path
import hashlib,json,struct,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(R/'direct_c'),str(R/'direct_c/cohort3_replay'),str(R/'build/offline-verification-deps')]
from native_test_pe import PE
from registry import symbol,save
from replay import import_slots
import unicorn,unicorn.x86_const as x
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
STACK,SP,STOP,ABORT=0x2ff00000,0x30008000,0x31000000,0x31000200
registers=[x.UC_X86_REG_EAX,x.UC_X86_REG_ECX,x.UC_X86_REG_EDX,x.UC_X86_REG_EBX,x.UC_X86_REG_ESP,x.UC_X86_REG_EBP,x.UC_X86_REG_ESI,x.UC_X86_REG_EDI]
initial=[0xa5b6c7d8,0x89abcdef,0x33445566,0x12345678,SP,0x23456789,0x3456789a,0x456789ab]
native=PE(R/'build/smoke-game/Pharaoh.exe')
variants=[('native',native,0x4cde60,None)]
profile=json.loads((OUT/'image-profile.json').read_bytes())
for label,item in profile.items():
    assert hashlib.sha256((R/item['path']).read_bytes()).hexdigest()==item['sha256']
    assert hashlib.sha256((R/item['map_path']).read_bytes()).hexdigest()==item['map_sha256']
    image=PE(R/item['path']);map_text=(R/item['map_path']).read_text()
    provider=symbol(map_text,'FUN_004cde60');assert provider and 'unit_01731.obj'in provider['owner']
    variants.append((label+'_current',image,provider['va'],map_text))
cases=[('zero-zero',0,0,0),('zero-divisor',123,0,0),('positive',3,2,150),
    ('negative',-3,2,0xffffff6a),('negative-divisor',7,-4,0xffffff51),
    ('wrapped-half',0x20000000,2,0xc0000000),('wrapped-third',0x60000000,3,0xd5555556),
    ('wrapped-maximum',0x7fffffff,100,0xffffffff),('minus-one',-1,-1,100)]
cases += [(f'overflow-{v:08x}',v,-1,None)for v in [0x20000000,0x60000000,0xa0000000,0xe0000000]]
cases += [('continued-divisor',0x20000000,-1,0xc0000000),('continued-dividend',0x60000000,-1,0)]
assert hashlib.sha256((R/'src/unit_01731.c').read_bytes()).hexdigest()==json.loads((OUT/'protocol.json').read_bytes())['source_sha256']
for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
    assert hashlib.sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
rows=[]
for label,pe,pc,map_text in variants:
    for name,a,b,wanted in cases:
        u=unicorn.Uc(unicorn.UC_ARCH_X86,unicorn.UC_MODE_32)
        u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
        u.mem_map(STACK,0x110000);u.mem_map(STOP,4096)
        if label!='native':
            imports=import_slots(pe)
            if 'abort'in imports:u.mem_write(imports['abort'],struct.pack('<I',ABORT))
        u.mem_write(SP-0x2000,b'\xcc'*0x4000)
        u.mem_write(SP,struct.pack('<III',STOP,a&0xffffffff,b&0xffffffff))
        for reg,value in zip(registers,initial):u.reg_write(reg,value)
        u.reg_write(x.UC_X86_REG_EFLAGS,0xad7)
        observed=dict(variant=label,case=name,entry=pc,arguments=[a&0xffffffff,b&0xffffffff],interrupts=[])
        abort_targets={ABORT}
        if map_text:
            for abort_name in ['abort','_abort']:
                provider=symbol(map_text,abort_name)
                if provider:abort_targets.add(provider['va'])
        def code(uc,address,size,data):
            if address in abort_targets:data['abort_boundary']=True;uc.emu_stop()
        def interrupt(uc,number,data):
            at=uc.reg_read(x.UC_X86_REG_EIP)
            ins=next(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(uc.mem_read(at,15)),at))
            data['interrupts'].append(dict(vector=number,pc=at,instruction=ins.mnemonic+' '+ins.op_str,
                eax=uc.reg_read(x.UC_X86_REG_EAX),edx=uc.reg_read(x.UC_X86_REG_EDX),ecx=uc.reg_read(x.UC_X86_REG_ECX),
                esp=uc.reg_read(x.UC_X86_REG_ESP),ebp=uc.reg_read(x.UC_X86_REG_EBP),eflags=uc.reg_read(x.UC_X86_REG_EFLAGS)))
            if name.startswith('continued-')and len(data['interrupts'])==1:
                assert number==0
                if name=='continued-divisor':uc.reg_write(x.UC_X86_REG_ECX,2)
                else:uc.reg_write(x.UC_X86_REG_EAX,0);uc.reg_write(x.UC_X86_REG_EDX,0)
                uc.reg_write(x.UC_X86_REG_EIP,at)
            else:uc.emu_stop()
        u.hook_add(unicorn.UC_HOOK_CODE,code,observed);u.hook_add(unicorn.UC_HOOK_INTR,interrupt,observed)
        try:u.emu_start(pc,STOP,count=10000)
        except Exception as e:observed['execution_error']=repr(e)
        observed.update(eax=u.reg_read(x.UC_X86_REG_EAX),esp=u.reg_read(x.UC_X86_REG_ESP),eip=u.reg_read(x.UC_X86_REG_EIP))
        observed['normal_return']=observed['eip']==STOP
        observed['nonvolatile_preserved']=[u.reg_read(registers[i])for i in [3,5,6,7]]==[initial[i]for i in [3,5,6,7]]
        rows.append(observed)
        if label in ['native','after_safe']or label.endswith('_current'):
            assert 'execution_error'not in observed,observed
            if wanted is None:
                assert len(observed['interrupts'])==1 and observed['interrupts'][0]['vector']==0 and not observed['normal_return'],observed
            else:
                assert observed['normal_return']and observed['eax']==wanted and observed['esp']==SP+4 and observed['nonvolatile_preserved'],observed
                assert len(observed['interrupts'])==int(name.startswith('continued-')),observed
        if 'before'in label and (wanted is None or name.startswith('continued-')):
            assert observed.get('abort_boundary')and not observed['interrupts'],observed
        if wanted is not None and not name.startswith('continued-'):
            assert observed['normal_return']and observed['eax']==wanted and observed['esp']==SP+4 and observed['nonvolatile_preserved'],observed
for name,a,b,wanted in cases:
    nr=next(r for r in rows if r['variant']=='native'and r['case']==name)
    for label in ['game_current','diagnostic_current','retained_current']:
        cr=next(r for r in rows if r['variant']==label and r['case']==name)
        if nr['interrupts']:
            for key in ['vector','eax','edx','ecx']:
                assert nr['interrupts'][0][key]==cr['interrupts'][0][key],(label,name,key)
            assert not ((nr['interrupts'][0]['eflags']^cr['interrupts'][0]['eflags'])&0xc5),(label,name)
result_path=R/'build/actor-animal-following/cohort7-arithmetic-fault-current-comparison.json'
save(result_path,dict(schema='percent-idiv-fault-probe-v1',cases=len(cases),invocations=len(rows),rows=rows,
    native_sha256=hashlib.sha256((R/'build/smoke-game/Pharaoh.exe').read_bytes()).hexdigest(),
    current_images=profile,
    native_and_safe_agree=True,before_abort_regression_is_separately_archived=True,normal_results_and_continuation_verified=True,
    physical_stack_layout_equal=False,scope='Full EAX/cdecl output plus actual #DE/vector0 arithmetic operands, defined arithmetic flags and two resumed contexts. C stack/code locations differ.',host_execution=False,whole_game_equivalence=False))
print(json.dumps(dict(cases=len(cases),invocations=len(rows),native_and_safe_agree=True,before_abort_regression_is_separately_archived=True,
    host_execution=False,result=str(result_path))))
