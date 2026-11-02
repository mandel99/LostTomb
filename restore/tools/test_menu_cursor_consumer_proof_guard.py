"""Ensure incomplete/stale actor proofs cannot retire the historical cursor gap."""
from pathlib import Path
from copy import deepcopy
import json
from menu_cursor_consumer_proof import PROOF,_validate_consumer
ROOT=Path(__file__).resolve().parents[1]
proof=json.loads((ROOT/PROOF).read_text());_validate_consumer(proof,ROOT)
def flags(p):p['production']=False
def partial(p):p['complete_case_set']=False
def cases(p):p['checked']=2634
def instruction(p):p['qualified_controller']['executed']=237
def edge(p):p['qualified_controller']['edges_executed']=31
def helper(p):p['actual_helper_coverage']['0051cd80']['executed']=138
def width(p):p['outcomes']['direct_lookup_full_DWORD_returns']=[0,1,5,127,255]
def overclaim(p):p['complete_actor_order_gameplay_claimed']=True
def source(p):p['source_hashes']['src/unit_00085.c']='0'*64
def missing_source(p):del p['source_hashes']['src/unit_01455.c']
def optimized(p):p['variants']['O2']['sha256']='0'*64
checks=[flags,partial,cases,instruction,edge,helper,width,overclaim,source,missing_source,optimized]
passed=[]
for change in checks:
 altered=deepcopy(proof);change(altered)
 try:_validate_consumer(altered,ROOT)
 except (AssertionError,KeyError):passed.append(change.__name__)
 else:raise AssertionError(('Invalid proof accepted',change.__name__))
out={'format':1,'passed':True,'checked':len(passed),'rejected_cases':passed,'product_files_modified':False,'original_game_executed':False}
(ROOT/'metadata/menu-cursor-consumer-proof-negative.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out))
