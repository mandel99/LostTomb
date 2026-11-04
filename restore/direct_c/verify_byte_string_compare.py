"""Complete5631B0: all byte values, alignments, prefix exits and read faults."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/byte-string-compare-native.json').read_text());f=catalogue['functions'][0]
rea={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-sample-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
raw=bytes.fromhex(f['bytes_hex']);ev=rea[f['bytes_evidence_id']]
assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
assert raw==bytes(images[0].data[0x5631b0-images[0].base:0x5631b0-images[0].base+len(raw)])
instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,0x5631b0));padding={int(a,16) for a in catalogue['padding_instruction_addresses']}
points={int(a,16) for a in f['instruction_addresses'].split()};assert {i.address for i in instructions}-padding==points
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in instructions if i.mnemonic.startswith('j') and i.mnemonic!='jmp'}
hits=set();edges=set();previous=[None];checked=invocations=0
LPAGE,RPAGE=0x35000000,0x36000000
for c,u,state in machines:
 for at in [LPAGE,RPAGE]:u.mem_map(at,4096)
 state['collect']=False
 def write(uc,access,at,size,value,data):
  if data['collect'] and (LPAGE<=at<LPAGE+4096 or RPAGE<=at<RPAGE+4096):data['writes'].append((at,size,value))
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_WRITE,write,state);u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state,begin=0x5631b0,end=0x563233)

def exercise(left=b'abc',right=None,la=0,ra=0,lp=None,rp=None,fault=None):
 global checked,invocations
 right=left if right is None else right;lp=LPAGE+128+la if lp is None else lp;rp=RPAGE+128+ra if rp is None else rp;expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   state.update(collect=False,writes=[],faults=[]);u.mem_write(LPAGE,b'\xa5'*4096);u.mem_write(RPAGE,b'\x6b'*4096)
   if LPAGE<=lp<LPAGE+4096:u.mem_write(lp,(left+b'\0')[:LPAGE+4096-lp])
   if RPAGE<=rp<RPAGE+4096:u.mem_write(rp,(right+b'\0')[:RPAGE+4096-rp])
   for reg,value in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
   u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP);put(u,SP+4,lp);put(u,SP+8,rp)
   pc=0x5631b0 if not c else sym('restore_byte_string_compare' if route=='named' else 'FUN_005631b0');previous[0]=None;state['collect']=True;invocations+=1
   try:u.emu_start(pc,STOP,count=10000);value=u.reg_read(UC_X86_REG_EAX)
   except UcError:value='fault'
   state['collect']=False
   if fault:assert state['faults'] and state['faults'][0][1:]==fault,(left,right,la,ra,route,state['faults'])
   else:
    assert not state['faults'] and u.reg_read(UC_X86_REG_EIP)==STOP,(route,state['faults'])
    assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    a=left.split(b'\0')[0];b=right.split(b'\0')[0];assert value==(0 if a==b else 0xffffffff if a<b else 1),(left,right,route,value)
   current=(value,tuple(state['writes']),tuple(state['faults']),bytes(u.mem_read(LPAGE,4096)),bytes(u.mem_read(RPAGE,4096)))
   if expected is None:expected=current
   else:assert current==expected,(left,right,la,ra,route,'native/C result, writes, fault or guards differ')
 checked+=1
 if checked%1000==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 exercise(b'\xffabc');exercise(b'\x80abc',b'\x81abc');exercise(b'')
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for byte,la,ra in itertools.product(range(256),range(4),range(4)):exercise(bytes([byte])*12,la=la,ra=ra)
 for a,b,la,ra in itertools.product([0,1,2,127,128,254,255],[0,1,2,127,128,254,255],range(4),range(4)):exercise(bytes([a])+b'abcd',bytes([b])+b'abcd',la,ra)
 for length,la,ra in itertools.product(range(21),range(4),range(4)):exercise(b'a'*length,la=la,ra=ra)
 for at,la,ra in itertools.product(range(16),range(4),range(4)):
  exercise(b'a'*at+b'\xffrest',b'a'*at+b'\x80rest',la,ra)
  exercise(b'a'*at+b'\x80rest',b'a'*at+b'\xffrest',la,ra)
 exercise(b'',rp=0,fault=(0,1));exercise(lp=LPAGE+4096,fault=(LPAGE+4096,4))
 exercise(b'\xff',b'\xff',lp=LPAGE+4095,fault=(LPAGE+4096,4))
 exercise(b'\xff\xff',b'\xff\xff',lp=LPAGE+4094,fault=(LPAGE+4096,4))
 exercise(b'\xff'*4,b'\xff'*4,rp=RPAGE+4095,fault=(RPAGE+4096,1))
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared byte strings:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges and not hits&padding,(missing,missing_edges,hits&padding)
 sources=['CMakeLists.txt','src/unit_02848.c','include/byte_string_compare.h','direct_c/verify_byte_string_compare.py','direct_c/verify_options_startup.py','metadata/byte-string-compare-native.json','metadata/sound-sample-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={'0x5631b0':dict(instructions=len(points),executed=len(points&hits))},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],unreachable_padding=sorted(hex(a) for a in padding),source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[],controlled_backend_endpoints=[],limitations=['Complete function with54 analyzed instructions. Two unreachable post-RET padding instructions (3 bytes) excluded and observed unexecuted. No child or external endpoint is controlled.','All256 byte values and16 left/right alignments, signed-byte boundaries, exact-1/0/+1 results, NUL/prefix exits and five matched read-fault cases compared. Watched pages and guards remain unchanged; saved ABI compared on normal returns.','Native aligned left DWORD/word read widths retained. Raw caller registers after faults, flags, arbitrary malformed mappings and concurrency are not certified.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/byte-string-compare-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
