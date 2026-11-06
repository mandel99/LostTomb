"""Repair only pre-query observations from native caller STORE ordering."""
import copy,hashlib,json

OBSERVED_PUBLICATIONS={(0xd3a6d0,4),(0xd3b51c,2)}
QUERY_TOKENS={'available-a','available-b','eligible','hit'}


def seed(case,address,width):
    result={}
    for at,payload in case['stores']:
        for index,value in enumerate(payload):
            if address<=at+index<address+width:result[at+index]=value
    assert set(result)==set(range(address,address+width)),('unseeded input',hex(address),width)
    return int.from_bytes(bytes(result[a]for a in range(address,address+width)),'little')


def publications(case):
    assert seed(case,0xd3a6c0,4)==0
    assert seed(case,0xe38dda,1)in(0x14,0x15,0x4e)
    theme=seed(case,0xe92dfc,4);font=seed(case,0xf56f84,4)
    return {(0xd3b168,4):font,(0xd3b164,4):font,(0xd3a6d0,4):200,
            (0xd3b130,4):0,(0xd3b13c,4):30,(0xd3b51c,2):0x3922 if theme==2 else 0x1c82}


def correct_query_snapshots(cases):
    result=copy.deepcopy(cases);changed_cases=0;changed_snapshots=0;changed_cells=0
    for original,case in zip(cases,result):
        if not case['id'].startswith('c7-tooltip-modes/query'):
            assert case==original;continue
        expected=publications(original);order=original['authored_endpoint_order']
        assert order.count('prepare')==1
        boundary=order.index('prepare');visits={}
        endpoints={e['token']:e for e in original['endpoints']}
        for event_index,token in enumerate(order[:boundary]):
            assert token in QUERY_TOKENS
            visit=visits.get(token,0);visits[token]=visit+1
            ep=endpoints[token];response=ep['responses'][visit]
            for field in('mutations','writes_args','writes_arg_bytes'):
                assert not ep.get(field)and not response.get(field),'Unproven helper RAM output'
            touched=set()
            for cell in case['authored_endpoint_observations'][event_index]:
                address,width,old=cell;key=(address,width)
                if key in OBSERVED_PUBLICATIONS:
                    assert old==seed(original,address,width),'Not the original delayed-publication oracle'
                    assert old!=expected[key]
                    cell[2]=expected[key];touched.add(key);changed_cells+=1
            assert touched==OBSERVED_PUBLICATIONS
            changed_snapshots+=1
        if boundary:changed_cases+=1
        restored=copy.deepcopy(case)
        restored['authored_endpoint_observations']=copy.deepcopy(original['authored_endpoint_observations'])
        assert restored==original,'Only authored observations may change'
    assert(changed_cases,changed_snapshots,changed_cells)==(10,331,662)
    return result


def oracle_digest(case):
    keys=('authored_final_memory','authored_endpoint_order','authored_endpoint_observations',
          'expected_endpoint_counts','expected_endpoint_arguments')
    return hashlib.sha256(json.dumps({k:case[k]for k in keys},sort_keys=True,separators=(',',':')).encode()).hexdigest()
