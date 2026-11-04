"""Authenticate the observed native caller closure and narrow byte-return ABI.

Incoming scratch-register bits are tracked to their kills. The two exceptional
DX*5 sequences preserve unknown upper bits; their WORD stores observe known bits.
"""
from pathlib import Path
import json,hashlib,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_REG,X86_OP_MEM
native=ROOT/'build/smoke-game/Pharaoh.exe';sha=hashlib.sha256(native.read_bytes()).hexdigest()
assert sha=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
paths=['metadata/actor-civic-target-search-next-rea-records.json',
       'metadata/actor-civic-target-search-sibling-next-rea-records.json',
       'metadata/actor-civic-target-search-next-callers-rea-records.json',
       'metadata/actor-civic-target-search-next-joins-rea-records.json',
       'metadata/actor-civic-target-search-next-seed-callers-rea-record.json',
       'metadata/actor-animal-target-caller-rea-records.json',
       'metadata/actor-animal-target-additional-callers-rea-records.json',
       'metadata/actor-animal-target-closure-bytes-rea-records.json',
       'metadata/actor-animal-target-provider-rea-records.json',
       'metadata/actor-animal-target-direction-bytes-rea-record.json']
records=[];instructions={};windows={}
for path in paths:
    d=json.loads((ROOT/path).read_text());records+=d.get('records',[d])
for e in records:
    assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==sha
    if e['operation']!='read_bytes':continue
    z=e['normalized_result'];assert z['complete'];a=int(z['address'],16);raw=bytes.fromhex(z['bytes_hex'])
    assert raw==bytes(pe.data[a-pe.base:a-pe.base+len(raw)])
    code=list(decoder.disasm(raw,a));windows[a]=code
    for i in code:
        if i.address in instructions:assert bytes(instructions[i.address].bytes)==bytes(i.bytes)
        instructions[i.address]=i

def incoming(entry):
    e=next(e for e in records if e['operation']=='procedure_references' and int(e['parameters']['procedure'],16)==entry)
    return {int(r['source_address'],16) for r in e['normalized_result']['references'] if int(r['target_address'],16)==entry}

periodic={a for a,code in windows.items() if code and code[0].mnemonic=='call' and code[0].op_str=='0x4eaec0'}
assert len(periodic)==17 and incoming(0x4eaec0)==periodic
assert incoming(0x4eb000)=={0x49bb88}
assert incoming(0x4ebc90)=={0x4eb098,0x4eb295}
assert incoming(0x4eabf0)=={0x4eb43e}
assert incoming(0x56233e)=={0x4eaf72,0x4eb06a}
# Legacy byte results are consumed as AL. EB000's sole caller discards the
# whole result; the periodic native model needs its full reconstructed EAX.
for a in periodic|{0x49bb88}:
    code=windows[a]
    assert code[1].mnemonic=='mov' and code[1].op_str.endswith('dword ptr [0xd056f8]')
    assert (code[2].mnemonic,code[2].op_str)==('add','esp, 4')
group=windows[0x49bb88];assert (group[3].mnemonic,group[3].op_str)==('jmp','0x49bc4e')
catalogue=json.loads((ROOT/'metadata/actor-animal-target-native.json').read_text())
for f in catalogue['functions']:
    raw=bytes.fromhex(f['bytes_hex']);a=int(f['address'],16)
    for i in decoder.disasm(raw,a):instructions[i.address]=i
assert (instructions[0x4eb446].mnemonic,instructions[0x4eb446].op_str)==('test','al, al')

register_bits={}
for whole,half,low,high in [('eax','ax','al','ah'),('ecx','cx','cl','ch'),('edx','dx','dl','dh')]:
    for name,mask in [(whole,0xffffffff),(half,65535),(low,255),(high,0xff00)]:register_bits[name]=(whole,mask)
def reg_bit(i,reg):return register_bits.get(i.reg_name(reg))
def audit_flow(start,initial,allow_clear=False):
    pending=[(start,tuple(initial))];seen=set();kills=set();algebra=set();calls=set()
    while pending:
        pc,masks=pending.pop()
        if not any(masks):kills.add(pc);continue
        if (pc,masks) in seen:continue
        seen.add((pc,masks));assert pc in instructions,('caller window exhausted',hex(pc),masks)
        i=instructions[pc];taint=dict(zip(['eax','ecx','edx'],masks));next_pc=pc+i.size
        if i.mnemonic=='call':
            assert allow_clear and i.op_str=='0x4b3250' and not taint['eax'] and not taint['ecx'],('unknown call inputs',hex(pc),taint)
            audit_flow(0x4b3250,[0,0,taint['edx']])
            taint['edx']=0;calls.add(pc)
        else:
            zero=(i.mnemonic=='xor' and len(i.operands)==2 and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_REG and i.operands[0].reg==i.operands[1].reg)
            partial_lea=(i.mnemonic=='lea' and i.op_str=='edx, [edx + edx*4]' and taint['edx']==0xffff0000)
            if partial_lea:
                # Multiplication by five cannot move upper input bits into
                # the low WORD. Keep the upper taint until the later kill.
                algebra.add(pc)
            else:
                reads,writes=i.regs_access()
                for reg in reads:
                    bits=reg_bit(i,reg)
                    if bits and not zero:assert not taint[bits[0]]&bits[1],('incoming bits observed',hex(pc),i.mnemonic,i.op_str,taint)
                for reg in writes:
                    bits=reg_bit(i,reg)
                    if bits:taint[bits[0]]&=~bits[1]&0xffffffff
        masks=tuple(taint.values())
        if i.mnemonic=='jmp':pending.append((int(i.op_str,16),masks))
        elif i.mnemonic.startswith('j'):
            pending.extend([(int(i.op_str,16),masks),(next_pc,masks)])
        else:
            assert i.mnemonic!='ret',('scratch bits escape',hex(pc),masks)
            pending.append((next_pc,masks))
    return dict(states=len(seen),kill_addresses=[hex(x) for x in sorted(kills)],word_algebra=[hex(x) for x in sorted(algebra)],route_clear_calls=[hex(x) for x in sorted(calls)])

flows={}
for a in sorted(periodic):
    flows[hex(a)]=audit_flow(a+5,[0,0,0xffffffff],allow_clear=True)
flows['group_join']=audit_flow(0x49bc4e,[0xffffff00,0xffffffff,0xffffffff])
assert {p for f in flows.values() for p in f['word_algebra']}=={'0x49da02','0x49f3bf'}
assert {p for f in flows.values() for p in f['route_clear_calls']}=={'0x49d9d2','0x49f38f'}
# Every actual direction return sets AL to 0..7: literal 0/2/4/6,
# SETcc (0/1), SETcc+3/4, or an explicit full-register AND 7.
direction=windows[0x51cd80];returns=[]
for n,i in enumerate(direction):
    if i.mnemonic!='ret':continue
    writes=[q for q in direction[max(0,n-8):n] if any((reg_bit(q,r) or ('',0))[0]=='eax' for r in q.regs_access()[1])]
    last_write=writes[-1];m,op=last_write.mnemonic,last_write.op_str
    bounded=(m=='mov' and op in ['al, 2','al, 4','al, 6'] or m=='xor' and op=='al, al' or m=='and' and op=='eax, 7' or m in ['setg','setle'] and op=='al')
    if m=='add' and op in ['eax, 3','eax, 4']:
        bounded=len(writes)>=2 and writes[-2].mnemonic in ['setg','setle'] and writes[-2].op_str=='al'
    assert bounded,(hex(i.address),[(q.mnemonic,q.op_str) for q in writes])
    returns.append(hex(i.address))
assert len(returns)==12
dead_edges=[(0x4ebce8,0x4ebdcc),(0x4ebde8,0x4ebeac),(0x4ebed5,0x4ebf94),(0x4ebfb0,0x4ec073),(0x4eacd0,0x4ead83)]
for a,b in dead_edges:
    assert instructions[a].mnemonic in ['jle','je'] and int(instructions[a].op_str,16)==b
span_code=sorted((i for a,i in instructions.items() if 0x4ebc90<=a<=0x4ec0e2),key=lambda i:i.address)
edi_writes=[(i.mnemonic,i.op_str) for i in span_code if any(i.reg_name(reg)=='edi' for reg in i.regs_access()[1]) and i.mnemonic!='pop']
assert edi_writes==[('mov','edi, 1')]+[('mov','edi, dword ptr [esp + 0x28]'),('add','edi, 4'),('mov','edi, dword ptr [esp + 0x1c]')]*4+[('add','edi, 2')],edi_writes
assert [i.address for i in span_code if i.op_str=='dword ptr [esp + 0x1c], edi']==[0x4ebcd2,0x4ec0b0]
assert (instructions[0x4ec0bb].mnemonic,instructions[0x4ec0bb].op_str)==('cmp','eax, 0xe4')
# EDI temporarily iterates the type array, then restores the saved span at
# the end of each point. Only the prologue and round tail write that slot.
# Nonnegative span1+2*completed stays1..455 at all four tests. The other
# impossible edge compares d with (d-1) mod8 for a zero-extended direction.
assert all(d != ((d-1+8)%8) for d in range(8))
sources=[*paths,'metadata/actor-animal-target-native.json','direct_c/audit_actor_animal_target.py','runtime/actor_command.c','runtime/actor_civic.c','runtime/actor_civic_model.c','include/actor_command.h','include/actor_civic.h']
proof=dict(passed=True,native_sha256=sha,periodic_callers=len(periodic),scratch_register_flows=flows,direction_returns=returns,
    full_periodic_EAX_reconstructed=True,byte_group_and_step_return_supported=True,
    analytically_unreachable_instructions=[],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in dead_edges],
    unreachable_edge_proofs=['EBC90 span EDI starts at1, has only ADD EDI,2, and exits at228 rounds. Four JLE span<=0 outcomes are impossible at their tests.','EABF0 actual direction has AL in0..7. No direction equals its previous direction modulo8; the initial JE outcome is impossible.'],
    source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
    limitations=['Provider-observed direct callers only; unidentified indirect callers remain unknown.','AL is the supported group/step result. Their incidental upper EAX, private stack addresses and flags are not a C ABI guarantee.','The periodic entry preserves meaningful full EAX. Native ECX is saved/restored; caller EDX bits reach a full kill before meaningful use. Two DX*5 paths preserve upper taint while storing a known low WORD.','Civic compatibility frames retain unused scratch registers/flags. All observed callers overwrite flags with ADD ESP,4; source-level wrappers do not claim incidental native scratch-register values. No game is launched.'])
(ROOT/'metadata/actor-animal-target-caller-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS animal-target callers: 17 periodic paths, group byte result, two state searches, seed and 12 direction returns')
