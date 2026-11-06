"""Add only the proven live model boundary; retain every original oracle."""
import copy,hashlib,json

MODEL_FIELDS=dict(compiled_frame_symbols=['restore_resource_render_C_004cd510'],
 compiled_frame_dispatcher='restore_resource_render_dispatch',compiled_frame_return_sites=[0x4cda31])
ORACLE_KEYS=('authored_final_memory','authored_endpoint_order','authored_endpoint_observations',
 'expected_endpoint_counts','expected_endpoint_arguments','expected_masked_result')


def oracle_digest(case):
 oracle={k:case[k]for k in ORACLE_KEYS if k in case}
 return hashlib.sha256(json.dumps(oracle,sort_keys=True,separators=(',',':')).encode()).hexdigest()


def repair_boundary(cases):
 result=copy.deepcopy(cases)
 for original,case in zip(cases,result):
  if case['entry']==0x4cabd0:
   matches=[e for e in case['endpoints']if e['token']=='line'and e['native']==0x4cd510]
   assert len(matches)==1 and matches[0]['result_kind']=='void'
   assert matches[0]['compiled_symbols']==['FUN_004cd510']
   matches[0].update(copy.deepcopy(MODEL_FIELDS))
  assert oracle_digest(original)==oracle_digest(case),'Original authored oracle changed'
  restored=copy.deepcopy(case)
  for ep in restored['endpoints']:
   for key in MODEL_FIELDS:ep.pop(key,None)
  assert restored==original,'Only the endpoint binding may change'
 return result
