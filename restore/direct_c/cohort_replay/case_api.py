"""Pure shared scenario API. Importing this module never loads a PE or Unicorn."""
from pathlib import Path
import ast,importlib.util

SP=0x30008000
STOP=0x31000000
S=0x32000000
DATA_START,DATA_END=0x560000,0x126d000
STACK_START,STACK_END=0x2ff00000,0x30010000
CONTROL_KEYS={'controlled','results','child_mutations','child_bytes','child_outputs','read_mutations','write_mutations','clock_mutations'}

def integer(value):
    if isinstance(value,str):return int(value,0)
    assert isinstance(value,int),value
    return value

def validate_child(token,spec):
    token=integer(token);value=dict(spec)
    assert 0<=value['arity']<=16,(hex(token),value)
    value.setdefault('name','FUN_%08x'%token)
    value.setdefault('native_cleanup',0);value.setdefault('compiled_cleanup',0)
    value.setdefault('result','full32');value.setdefault('arg_masks',{})
    value.setdefault('private_output_args',[])
    assert value['result'] in {'full32','al8','void'}
    assert all(integer(i)<value['arity'] for i in value['private_output_args'])
    value['arg_masks']={integer(k):integer(v)for k,v in value['arg_masks'].items()}
    if value['arg_masks']:
        # Qualified consumed BYTE arguments only. 469E40 reads its alternative
        # with MOV AL,[ESP+14h]; see the saved migration byte-boundary evidence.
        qualified={0x4b4800:{0:255,4:255},0x469e40:{1:255}}
        assert value['arg_masks']==qualified.get(token),('Unqualified argument normalization',hex(token),value['arg_masks'])
    assert value['native_cleanup']%4==0 and value['compiled_cleanup']%4==0
    value['aliases']=sorted(set(value.get('aliases',[])+[value['name']]))
    value['result_views']=sorted(set(value.get('result_views',[])+[value['result']]))
    return token,value

def validate_case(case,children):
    assert set(case)>={'id','fn','origin','cfg'},case
    assert isinstance(case['id'],str)and'/'in case['id']
    assert case['origin'] in {'protocol','actual-composition'}
    result=dict(case);result['fn']=integer(case['fn']);cfg=dict(case['cfg']);result['cfg']=cfg
    assert cfg.get('setup','actor')in{'actor','minimal'}
    assert all(isinstance(a,int)for a in cfg.get('arguments',[]))
    if'saved'in cfg:assert len(cfg['saved'])==4 and all(isinstance(v,int)for v in cfg['saved']),(case['id'],'EBX/EBP/ESI/EDI initial values')
    if case['origin']=='protocol':assert 'controlled'in cfg,('Protocol must declare its controlled children',case['id'])
    else:assert not CONTROL_KEYS.intersection(cfg),('Actual composition contains protocol substitutions',case['id'],CONTROL_KEYS.intersection(cfg))
    assert all(integer(t)in children for t in cfg.get('controlled',[])),case['id']
    for key in ['child_mutations','child_bytes','child_outputs']:
        for (token,nth),changes in cfg.get(key,{}).items():
            assert token in children and nth>=1,(case['id'],key,token,nth)
            if key=='child_outputs':
                for argument,width,value in changes:
                    assert argument in children[token]['private_output_args']and width in{1,2,4},(case['id'],token,argument,width)
    for at,raw in cfg.get('regions',[]):
        assert isinstance(at,int)and isinstance(raw,(str,bytes,bytearray))
        if isinstance(raw,str):bytes.fromhex(raw)
    for at,width,value in cfg.get('expect_memory',[]):assert isinstance(at,int)and width in{1,2,4}and isinstance(value,int)
    return result

def load_modules(paths):
    """Load author-owned pure definitions, never an old executable fixture runner."""
    children={};cases=[];sources=[];groups=[]
    forbidden={'unicorn','native_test_pe','offline_callback','subprocess','os','ctypes'}
    for index,path in enumerate(paths):
        path=Path(path).resolve();tree=ast.parse(path.read_bytes(),filename=str(path))
        for node in ast.walk(tree):
            if isinstance(node,(ast.Import,ast.ImportFrom)):
                names=[a.name for a in node.names]if isinstance(node,ast.Import)else[node.module or'']
                assert not any(n.split('.')[0]in forbidden or n.startswith(('verify_','offline_'))for n in names),('Executable fixture import forbidden',path,names)
        spec=importlib.util.spec_from_file_location('cohort100_cases_%d'%index,path)
        module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
        assert isinstance(module.GROUP,str)and callable(module.cases)
        groups.append(module.GROUP);sources.append(path)
        local_views={}
        for token,child in module.CHILDREN.items():
            token,child=validate_child(token,child)
            local_views[token]=list(child['result_views'])
            if token in children:
                previous=children[token]
                for key in ['arity','native_cleanup','compiled_cleanup','arg_masks','private_output_args']:
                    assert previous[key]==child[key],('Conflicting child contract',hex(token),key,previous[key],child[key])
                previous['aliases']=sorted(set(previous['aliases']+child['aliases']))
                previous['result_views']=sorted(set(previous['result_views']+child['result_views']))
            else:children[token]=child
        for case in module.cases():
            assert '_endpoint_result_views'not in case,('Result context is loader-owned',case['id'])
            case=dict(case);case['_endpoint_result_views']=local_views.copy()
            assert all(t in local_views for t in case['cfg'].get('controlled',[])),('Controlled endpoint missing local declaration',case['id'])
            cases.append(case)
    assert len({c['id']for c in cases})==len(cases),'Duplicate scenario IDs'
    return [validate_case(c,children)for c in cases],children,sources,groups
