"""Narrow retained-export repair for the proven53C388 member-call receiver."""

def repair(source,function_name):
    if function_name!='FUN_0053c100':return source
    old='cVar1 = FUN_004bf9d0();'
    if old not in source:return source
    if source.count(old)!=1:raise ValueError('Ambiguous53C388 member-call site')
    source=source.replace('extern uint FUN_004bf9d0();','extern uint32_t FUN_004bf9d0(uint32_t self);')
    return source.replace(old,'cVar1 = FUN_004bf9d0(0x00d05720u);')
