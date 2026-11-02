"""Inspect compiler-emitted scalar accesses in independently built C leaves."""
from pathlib import Path
import hashlib,json,re,struct,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_MEM,CS_OP_IMM,CS_AC_READ
sha=lambda b:hashlib.sha256(b).hexdigest();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
report={'format':1,'passed':True,'original_game_executed':False,'variants':{}}
for config in ['Debug','Release']:
    executable=HERE/'build'/config/'scalar_binding_pixels_verify.exe';mapfile=executable.with_suffix('.map');binary=executable.read_bytes();symbols=mapfile.read_text()
    u16=lambda p:struct.unpack_from('<H',binary,p)[0];u32=lambda p:struct.unpack_from('<I',binary,p)[0]
    pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);sections=[]
    for k in range(u16(pe+6)):
        q=table+k*40;sections.append((base+u32(q+12),u32(q+8),u32(q+20),u32(q+16)))
    def bytes_at(a,n):
        for va,vs,raw,rs in sections:
            if va<=a and a+n<=va+rs:return binary[raw+a-va:raw+a-va+n]
        raise AssertionError(hex(a))
    functions={}
    for function in ['FUN_0040fa40','FUN_0040fa70','FUN_0040fae0']:
        match=re.search(r'\b_'+function+r'\s+([0-9a-fA-F]{8})\b',symbols);assert match,function
        entry=int(match[1],16);ins=[];reads=[];calls=[]
        for i in md.disasm(bytes_at(entry,512),entry):
            ins.append({'address':f'{i.address:08x}','bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str})
            for op in i.operands:
                if op.type==CS_OP_MEM and not op.mem.base and not op.mem.index and op.mem.disp in [0xe38d94,0xe92e18]:
                    assert op.size==4 and op.access&CS_AC_READ
                    reads.append({'global':f'{op.mem.disp:08x}','instruction_address':f'{i.address:08x}','width':op.size})
            if i.mnemonic=='call':
                calls.append(i.op_str)
                if i.operands[0].type==CS_OP_IMM:assert not 0x400000<=i.operands[0].imm<0x56e100,'original text fallback'
            if i.mnemonic=='ret':break
        assert ins[-1]['instruction'].startswith('ret')
        assert {r['global'] for r in reads}=={'00e38d94','00e92e18'}
        functions[function]={'compiled_entry':f'{entry:08x}','scalar_read_widths':reads,'compiler_helper_calls':calls,'instructions_to_RET':ins}
    report['variants'][config]={'executable_sha256':sha(binary),'map_sha256':sha(mapfile.read_bytes()),'functions':functions}
report['limitations']=['This compiler inspection establishes actual DWORD scalar operands and absence of calls into original text in the C leaves. Full algorithm equivalence is separately measured by scalar_binding_emulation.json. Debug compiler runtime checks are not original algorithm endpoints.']
(HERE/'compiled_scalar_accesses.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'variants':list(report['variants']),'scalar_globals':['00e38d94','00e92e18'],'functions_per_variant':3}))
