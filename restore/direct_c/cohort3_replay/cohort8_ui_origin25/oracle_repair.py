"""Expected center colors only; original recipes and actual runs stay immutable."""
import copy
CASE='c8-ui-origin-joint25/00532fe0/full-dword-x-negative-y'
def correct_cases(cases):
 result=copy.deepcopy(cases)
 for c in result:
  if c['id']!=CASE:continue
  assert c['entry']==0x532fe0
  rows=c['expected_endpoint_arguments']['center']
  assert len(rows)==3 and rows[1][0:2]==[0x43,1]and rows[2][0:2]==[0x43,2]
  assert rows[1][5]==0x3922 and rows[2][5]==0x1c82
  rows[1][5]=0x1c82;rows[2][5]=0x3922
 return result
