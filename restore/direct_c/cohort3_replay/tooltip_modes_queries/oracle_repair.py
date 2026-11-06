"""Derived expected arguments only; original inputs/responses stay intact."""
import copy,hashlib,json

IDS={f'c7-tooltip-modes/mode2-six-live-DWORD-values-theme{theme}'for theme in(0,2)}
STAT_BASES=(0xea446c,0xea5b3c,0xea5b40,0xea5b44,0xea5b48,0xea1878)
FORMATS=(0x5e03fc,0x57c048,0x57c048,0x57c048,0x57c048,0x5e03fc)


def seed_u32(case,address):
    # Reconstruct the actual authored initial RAM, honoring store order.
    data={}
    for at,payload in case['stores']:
        for i,value in enumerate(payload):
            if address<=at+i<address+4:data[at+i]=value
    assert set(data)==set(range(address,address+4)),('unseeded native input',hex(address))
    return int.from_bytes(bytes(data[a]for a in range(address,address+4)),'little')


def signed(value):return value-0x100000000 if value&0x80000000 else value


def derive_arguments(case):
    assert case['id']in IDS and case['entry']==0x4cabd0
    player=seed_u32(case,0xeaac28);offset=(player*0x49d8)&0xffffffff
    values=[seed_u32(case,(base+offset)&0xffffffff)for base in STAT_BASES]
    font=seed_u32(case,0xf56f84);theme=seed_u32(case,0xe92dfc)
    mx=signed(seed_u32(case,0xe38e30));my=signed(seed_u32(case,0xe38e34))
    # Native4CB0D5 SETL and4CB0F1/0F6;4CB0E0 SETL and4CB104/109.
    y=my+20 if my<200 else my-200
    x=mx+10 if mx<200 else mx-32
    front,shadow=(0x3922,0xff3a)if theme==2 else(0x1c82,0x7f9a)
    arguments=[]
    for index,(value,fmt)in enumerate(zip(values,FORMATS)):
        dy=index*14
        #4CB28A loads VALUE into EAX;4CB28D loads FORMAT into EBP.
        #PUSH EBP;PUSH40;PUSH EAX => argv0=VALUE,argv2=FORMAT.
        arguments.append([value,0x40,fmt,(y+140)&0xffffffff,(x+dy+5)&0xffffffff,font,front])
        #4CB2D0 reloads the same VALUE; EBP still holds its FORMAT.
        arguments.append([value,0x40,fmt,(y+139)&0xffffffff,(x+dy+4)&0xffffffff,font,shadow])
    return arguments


def correct_expected_formatted(cases):
    result=copy.deepcopy(cases);changed=[]
    for original,case in zip(cases,result):
        if case['id']in IDS:
            expected=derive_arguments(original)
            before=original['expected_endpoint_arguments']['formatted']
            assert len(before)==len(expected)==12
            for old,new in zip(before,expected):
                assert old==[new[2],new[1],new[0],*new[3:]],'Only the proven0/2 reversal may be repaired'
            case['expected_endpoint_arguments']['formatted']=expected
            restored=copy.deepcopy(case)
            restored['expected_endpoint_arguments']['formatted']=copy.deepcopy(before)
            assert restored==original,'Input/response/order/memory oracle changed'
            changed.append(case['id'])
        else:assert case==original
    assert set(changed)==IDS and len(changed)==2
    return result


def oracle_digest(case):
    keys=('authored_final_memory','authored_endpoint_order','authored_endpoint_observations',
          'expected_endpoint_counts','expected_endpoint_arguments')
    return hashlib.sha256(json.dumps({k:case[k]for k in keys},sort_keys=True,separators=(',',':')).encode()).hexdigest()
