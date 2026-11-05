"""Shared native/named-C replay adapted from the historical cohort driver. Emulation requires an admitted frozen GO."""
from pathlib import Path
from collections import Counter
import argparse,ast,datetime,hashlib,json,re,shutil,struct,sys,zlib
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if(p/'runtime/actor_civic.c').is_file())
sys.dont_write_bytecode=True
from case_api import load_modules,DATA_START,DATA_END,STACK_START,STACK_END,SP,STOP,S
from freeze_gate import admit,digest
from fixture_setup import setup,put,get,byte,word

def plan(paths):
    cases,children,sources,groups=load_modules(paths)
    unprovided=[dict(id=c['id'],endpoint=hex(t),local_result_views=c['_endpoint_result_views'][t])for c in cases for t in c['cfg'].get('controlled',[])if t not in c['cfg'].get('results',{})and c['_endpoint_result_views'][t]!=['void']]
    return cases,children,sources,dict(status='input-plan-only',cases=len(cases),by_group=dict(Counter(c['id'].split('/')[0]for c in cases)),by_origin=dict(Counter(c['origin']for c in cases)),entries=len({c['fn']for c in cases}),child_contracts=len(children),unprovided_nonvoid_protocol_results=unprovided,unprovided_result_policy='Planned controlled set can include unreached endpoints. Reaching a listed endpoint fails closed; no return value is invented.',execution_performed=False,full_cohort_validation_complete=False)

def sections(path,base):
    b=Path(path).read_bytes();pe=struct.unpack_from('<I',b,60)[0];n=struct.unpack_from('<H',b,pe+6)[0];table=pe+24+struct.unpack_from('<H',b,pe+20)[0];out=[]
    for i in range(n):
        at=table+i*40;size,rva=struct.unpack_from('<II',b,at+8);flags=struct.unpack_from('<I',b,at+36)[0]
        out.append((base+rva,size,flags))
    return out

def snapshot_differences(expected,actual):
    """Keep the comparison exact; diagnostics do not alter any trace or RAM mask."""
    detail=[]
    for n,name in enumerate(['reads','writes','events']):
        if actual[n]!=expected[n]:
            first=next(((i,a,b)for i,(a,b)in enumerate(zip(expected[n],actual[n]))if a!=b),None)
            if first is None:
                i=min(len(expected[n]),len(actual[n]));first=(i,expected[n][i]if i<len(expected[n])else None,actual[n][i]if i<len(actual[n])else None)
            detail.append(dict(component=name,native_length=len(expected[n]),compiled_length=len(actual[n]),first_difference=first))
    for n,base in [(3,DATA_START),(4,S)]:
        if actual[n]!=expected[n]:
            first=next((dict(address=hex(base+i),native=a,compiled=b)for i,(a,b)in enumerate(zip(expected[n],actual[n]))if a!=b),None)
            detail.append(dict(component='RAM',base=hex(base),first_difference=first,native_sha256=hashlib.sha256(expected[n]).hexdigest(),compiled_sha256=hashlib.sha256(actual[n]).hexdigest()))
    return detail

def execution(args,cases,children,source_paths,inventory):
    frozen,binding=admit(args.freeze,args.go)
    sys.path[:0]=[str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
    from native_test_pe import PE
    from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID
    from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EIP,UC_X86_REG_EFLAGS,UC_X86_REG_FPCW,UC_X86_REG_FPSW,UC_X86_REG_FPTAG,UC_X86_REG_MXCSR
    from scope_inputs import scopes
    scope_map,scope_paths=scopes()
    native=PE(ROOT/frozen['native']['path']);seed=bytes(native.data[DATA_START-native.base:DATA_END-native.base])
    regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI];saved=[0x12345678,0x23456789,0x3456789a,0x456789ab]
    machines=[]
    for label,item in [('native',frozen['native']),*frozen['compiled_images'].items()]:
        pe=native if label=='native'else PE(ROOT/item['path']);u=Uc(UC_ARCH_X86,UC_MODE_32)
        u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
        mapped_end=pe.base+((len(pe.data)+4095)&~4095)
        if DATA_START<pe.base:u.mem_map(DATA_START,pe.base-DATA_START)
        if DATA_END>mapped_end:u.mem_map(mapped_end,((DATA_END-mapped_end+4095)&~4095))
        u.mem_map(STACK_START,STACK_END-STACK_START);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
        writable=[]
        if label!='native':
            for at,n,flags in sections(ROOT/item['path'],pe.base):
                if flags&0x80000000 and n:writable.append((at,bytes(pe.data[at-pe.base:at-pe.base+n])))
                if flags&0x20000000:assert at+n<=DATA_START or at>=DATA_END,('Compiled code overlaps fixed-data reset',label,hex(at),n)
        state=dict(label=label,collect=False,reads=[],writes=[],events=[],faults=[],return_hooks=set())
        def mutate(uc,data,key,at,n):
            for nth,changes in data['cfg'].get(key,{}).get(at,[]):
                if nth!=n:continue
                data['events'].append(('mutation',key,at,n,tuple(tuple(x)for x in changes)))
                for target,value in changes:put(uc,target,value)
        def read(uc,access,at,size,value,data):
            if data['collect']and data.get('owner',True):
                if at in data['cfg'].get('read_mutations',{}):
                    n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n;mutate(uc,data,'read_mutations',at,n)
                row=(at,size,int.from_bytes(uc.mem_read(at,size),'little'));data['reads'].append(row);data['events'].append(('R',*row))
        def write(uc,access,at,size,value,data):
            if data['collect']:
                row=(at,size,value&((1<<(8*size))-1));data['writes'].append(row);data['events'].append(('W',*row))
                if at in data['cfg'].get('write_mutations',{}):
                    n=data['write_counts'].get(at,0)+1;data['write_counts'][at]=n;mutate(uc,data,'write_mutations',at,n)
        def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
        u.hook_add(UC_HOOK_MEM_READ,read,state,begin=DATA_START,end=DATA_END-1);u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=DATA_START,end=DATA_END-1)
        u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S,end=S+0x47fff)
        u.hook_add(UC_HOOK_MEM_READ,read,state,begin=S,end=S+0x47fff)
        u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
        if label!='native':
            def forbid_native_code(uc,pc,size,data):
                if data['collect']:raise AssertionError(('Original native code entered by compiled lane',data['case']['id'],hex(pc)))
            for at,n,flags in sections(ROOT/frozen['native']['path'],native.base):
                begin,end=max(at,DATA_START),min(at+n,DATA_END)
                if flags&0x20000000 and begin<end:u.hook_add(UC_HOOK_CODE,forbid_native_code,state,begin=begin,end=end-1)
        mapping=''if label=='native'else(ROOT/item['map_path']).read_text(errors='replace')
        def lookup(name,mapping=mapping):
            values=re.findall(r'\s_'+re.escape(name)+r'\s+([0-9a-fA-F]{8})\s',mapping)
            values=sorted({int(v,16)for v in values});return values[0]if len(values)==1 else None
        entries={}
        for token,spec in children.items():
            addresses=[token]if label=='native'else[]
            if label!='native':
                names=set(spec['aliases']+([]if spec.get('receiver')else['FUN_%08x'%token]))
                provider=frozen['provider_map'].get(hex(token))
                if provider:names.update([provider['named_symbol'],provider.get('canonical_effective_symbol',provider['named_symbol'])])
                addresses=[pc for name in names if(pc:=lookup(name))is not None]
            for pc in set(addresses):entries.setdefault(pc,set()).add(token)
        base_query_pc=None if label=='native'else lookup('restore_monument_base_worker_result')
        raw_query_pc=None if label=='native'else lookup('restore_acceptance_member_check_raw')
        if raw_query_pc is not None:
            # This existing bridge takes(entry,receiver,building), then executes
            # the C RAW child with an explicit zero incoming EAX. Other dynamic
            # targets are not admitted as these one-argument worker queries.
            queries={0x417590,0x4e6b20,0x40ec20,0x401b40,0x5477e0}
            entries.setdefault(raw_query_pc,set()).update(queries.intersection(children))
        explicit_pcs={lookup(n)for t,spc in children.items()if spc.get('receiver')for n in spc['aliases']if not n.startswith('FUN_')}
        explicit_pcs.discard(None)
        state['entries']=entries
        def returned(uc,pc,size,data):
            if data['collect']and not data['owner']and data.get('pending_return')==(pc,uc.reg_read(UC_X86_REG_ESP)):
                data['owner']=True;data['pending_return']=None
        def child(uc,pc,size,data,base_query_pc=base_query_pc,raw_query_pc=raw_query_pc,explicit_pcs=explicit_pcs):
            if not data['collect']or not data['owner']:return
            tokens=data['entries'][pc]-{data['case']['fn']}
            direct=set(scope_map.get(data['case']['fn'],{}).get('direct_children',[]))
            if pc==base_query_pc or pc==raw_query_pc:
                tokens&={get(uc,uc.reg_read(UC_X86_REG_ESP)+4)}
            if len(tokens)>1 and direct:tokens&=direct
            if not tokens:return
            assert len(tokens)==1,('Ambiguous folded child aliases',data['case']['id'],hex(pc),tokens)
            token=next(iter(tokens));spec=children[token];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
            offset=8 if pc==raw_query_pc else 4 if pc==base_query_pc or pc in explicit_pcs else 0
            arguments=tuple(get(uc,sp+4+offset+i*4)for i in range(spec['arity']));logical=[]
            if spec.get('receiver'):
                receiver=uc.reg_read(UC_X86_REG_ECX)if data['label']=='native'else get(uc,sp+4)
                logical.append(('receiver',receiver))
            if spec.get('incoming_eax'):
                eax=0 if pc==raw_query_pc else uc.reg_read(UC_X86_REG_EAX)if data['label']=='native'else get(uc,sp+4+offset+4*spec['arity'])
                logical.append(('incoming_eax',eax))
            for i,value in enumerate(arguments):
                value&=spec['arg_masks'].get(i,0xffffffff)
                if i in spec['private_output_args']and STACK_START<=value<STACK_END:
                    if value not in data['private_ids']:data['private_ids'][value]=len(data['private_ids'])
                    logical.append(('private-output',data['private_ids'][value]))
                else:logical.append(value)
            n=data['counts'].get(token,0)+1;data['counts'][token]=n
            data['events'].append(('C',token,tuple(logical)));data['physical_child_arguments'].append((token,n,arguments))
            for at,value in data['cfg'].get('child_mutations',{}).get((token,n),[]):put(uc,at,value)
            for at,value in data['cfg'].get('child_bytes',{}).get((token,n),[]):byte(uc,at,value)
            for index,width,value in data['cfg'].get('child_outputs',{}).get((token,n),[]):
                target=arguments[index];data['events'].append(('O',token,n,index,width,value&((1<<(width*8))-1)))
                uc.mem_write(target,(value&((1<<(width*8))-1)).to_bytes(width,'little'))
                if DATA_START<=target<DATA_END or S<=target<S+0x48000:
                    row=(target,width,value&((1<<(width*8))-1));data['writes'].append(row);data['events'].append(('W',*row))
            if token not in data['cfg'].get('controlled',[]):
                cleanup=spec['native_cleanup']if data['label']=='native'else spec['compiled_cleanup']
                data['pending_return']=(ret,sp+4+cleanup);data['owner']=False
                if ret not in data['return_hooks']:
                    uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['return_hooks'].add(ret)
                return
            values=data['cfg'].get('results',{}).get(token)
            if values is not None:
                assert values,(data['case']['id'],hex(token),'empty result queue')
                uc.reg_write(UC_X86_REG_EAX,values[min(n-1,len(values)-1)]&0xffffffff)
            else:assert data['case']['_endpoint_result_views'].get(token)==['void'],(data['case']['id'],hex(token),'No explicit protocol result for nonvoid endpoint',data['case']['_endpoint_result_views'].get(token))
            uc.reg_write(UC_X86_REG_ECX,0x6a6b6c6d);uc.reg_write(UC_X86_REG_EDX,0x7a7b7c7d)
            cleanup=spec['native_cleanup']if data['label']=='native'else spec['compiled_cleanup']
            uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
        for pc in entries:u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)
        clock_pc=S+0x49000
        def clock(uc,pc,size,data):
            if not data['collect']:return
            if data['owner']:data['events'].append(('clock',))
            for at,value in data['cfg'].get('clock_mutations',[]):put(uc,at,value)
            sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_EAX,data['cfg'].get('clock',0x12345678));uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
        u.hook_add(UC_HOOK_CODE,clock,state,begin=clock_pc,end=clock_pc)
        state['clock_slot']=0x56f22c if label=='native'else lookup('_imp__timeGetTime@0')
        machines.append((label,u,state,writable))
    stamp=datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f');out=args.output_dir/stamp;out.mkdir(parents=True)
    inputs=[Path(__file__),HERE/'case_api.py',HERE/'freeze_gate.py',HERE/'fixture_setup.py',HERE/'scope_inputs.py',*source_paths,*sorted(scope_paths),args.freeze.resolve()]
    input_hashes={str(p):digest(p)for p in inputs}
    archived={}
    for n,p in enumerate(inputs):
        name='%02d-%s'%(n,p.name);target=out/'inputs'/name;target.parent.mkdir(exist_ok=True);target.write_bytes(p.read_bytes());archived[name]=digest(target)
    observations=[];failures=[];attempts=0;invocations=0
    def journal(name,row):
        with(out/name).open('a',encoding='utf-8')as stream:stream.write(json.dumps(row)+'\n')
    def archive_route(prefix,record,snapshot):
        folder=out/'failures';folder.mkdir(exist_ok=True)
        trace=dict(record,reads=snapshot[0],writes=snapshot[1],events=snapshot[2])
        (folder/(prefix+'.json')).write_text(json.dumps(trace,indent=2)+'\n')
        for index,name in [(3,'fixed-ram'),(4,'scratch-ram')]:
            raw=zlib.compress(snapshot[index],9);path=folder/(prefix+'-'+name+'.zlib');path.write_bytes(raw)
            trace[name]=dict(path=str(path.relative_to(out)),sha256=digest(path),uncompressed_sha256=hashlib.sha256(snapshot[index]).hexdigest(),uncompressed_bytes=len(snapshot[index]))
        (folder/(prefix+'.json')).write_text(json.dumps(trace,indent=2)+'\n')
        return str((folder/(prefix+'.json')).relative_to(out))
    def invoke(case,provider,label,u,state,writable,route):
        nonlocal attempts,invocations
        cfg=case['cfg'];case_saved=[v&0xffffffff for v in cfg.get('saved',saved)];state['collect']=False
        state.update(cfg=cfg,case=case,owner=True,pending_return=None,reads=[],writes=[],events=[],faults=[],counts={},read_counts={},write_counts={},private_ids={},physical_child_arguments=[])
        mode=provider['named_result_comparison']if route=='native'else provider[route+'_result_comparison']
        record=dict(target=label,route=route,result_contract=mode,execution_started=False,returned_normally=False,initial_nonvolatile_registers=dict(zip(['EBX','EBP','ESI','EDI'],case_saved)))
        clock_patched=False;errors=[]
        try:
            for at,raw in writable:u.mem_write(at,raw)
            setup(u,seed,cfg,dict(kind=scope_map.get(case['fn'],{}).get('kind'),animation_bases=[]))
            if state['clock_slot']is not None:state['clock_saved']=get(u,state['clock_slot']);put(u,state['clock_slot'],S+0x49000);clock_patched=True
            for r,v in zip(regs,[cfg.get('incoming_eax',0xa5b6c7d8),cfg.get('receiver',0x22222222),0x33333333,case_saved[0],SP,*case_saved[1:]]):u.reg_write(r,v)
            for r,v in [(UC_X86_REG_EFLAGS,0x202),(UC_X86_REG_FPCW,cfg.get('fpcw',0x37f)),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff),(UC_X86_REG_MXCSR,0x1f80)]:u.reg_write(r,v)
            put(u,SP,STOP)
            assert len(cfg.get('arguments',[]))==provider['native_argument_count'],('argument count',len(cfg.get('arguments',[])),provider['native_argument_count'])
            arguments=cfg.get('arguments',[])
            if route!='native':
                arguments=[cfg.get('receiver',0x22222222)if x=='receiver'else cfg.get('incoming_eax',0xa5b6c7d8)if x=='incoming_eax'else x['literal']if isinstance(x,dict)else arguments[x]for x in provider['named_arguments']]
            for i,value in enumerate(arguments):put(u,SP+4+i*4,value)
            pc=case['fn']if route=='native'else provider['symbols'][label][route]['va']
            record['entry_pc']=hex(pc);attempts+=1;record['execution_started']=True
            state['collect']=True;u.emu_start(pc,STOP,count=args.budget);state['collect']=False
            invocations+=1
            if u.reg_read(UC_X86_REG_EIP)!=STOP or state['faults']:errors.append(dict(kind='execution/fault',reason='return/fault-or-budget',faults=state['faults']))
            else:
                record['returned_normally']=True
                pop=provider['native_ret_cleanup_bytes']if route=='native'else provider[route+'_C_ret_cleanup_bytes']
                category='execution/fault'if route=='native'else'native-vs-C'
                if u.reg_read(UC_X86_REG_ESP)!=SP+4+pop:errors.append(dict(kind=category,reason='stack-cleanup',expected_SP=hex(SP+4+pop),actual_SP=hex(u.reg_read(UC_X86_REG_ESP))))
                nonvolatile=[u.reg_read(regs[i])for i in [3,5,6,7]]
                if nonvolatile!=case_saved:errors.append(dict(kind=category,reason='nonvolatile-registers',expected=case_saved,actual=nonvolatile))
        except Exception as exc:errors.append(dict(kind='execution/fault',reason='exception',error=repr(exc),exception_class=type(exc).__name__,exception_message=str(exc)))
        finally:
            state['collect']=False
            if clock_patched:put(u,state['clock_slot'],state['clock_saved'])
        record.update(physical_EAX=hex(u.reg_read(UC_X86_REG_EAX)),EIP=hex(u.reg_read(UC_X86_REG_EIP)),ESP=hex(u.reg_read(UC_X86_REG_ESP)),faults=state['faults'],child_counts={hex(k):v for k,v in state['counts'].items()},local_child_result_views={hex(k):case['_endpoint_result_views'].get(k)for k in state['counts']},physical_child_arguments=state['physical_child_arguments'])
        snapshot=(tuple(state['reads']),tuple(state['writes']),tuple(state['events']),bytes(u.mem_read(DATA_START,len(seed))),bytes(u.mem_read(S,0x48000)))
        return record,snapshot,errors
    try:
        for case in cases:
            cfg=case['cfg'];provider=frozen['provider_map'][hex(case['fn'])];reference=None;native_record=None;routes_done=[];case_failures=[];reference_artifact=None
            for label,u,state,writable in machines:
                routes=['native']if label=='native'else(['named']if provider['symbols'][label].get('named')else[])
                if not routes:routes_done.append(dict(target=label,route='named',not_executed='definition-linker-eliminated-in-this-image'))
                for route in routes:
                    if route!='native'and reference is None:
                        record=dict(target=label,route=route,not_executed='native-reference-unavailable');routes_done.append(record);journal('routes.jsonl',dict(id=case['id'],**record));continue
                    record,snapshot,errors=invoke(case,provider,label,u,state,writable,route)
                    r0=int(record['physical_EAX'],16);mode=record['result_contract'];mask=255 if mode=='AL-only'else 0xffffffff
                    if route=='native'and not errors:
                        native_record=record;reference=snapshot
                        if mode!='discard-physical-EAX'and'expect_eax'in cfg and(r0&mask)!=(cfg['expect_eax']&mask):errors.append(dict(kind='native-authored-expectation',reason='expected-result',expected=hex(cfg['expect_eax']&mask),actual=hex(r0&mask),physical_EAX=hex(r0),mask=hex(mask)))
                        for token,count in cfg.get('expect_calls',{}).items():
                            actual=state['counts'].get(token,0)
                            if actual!=count:errors.append(dict(kind='native-authored-expectation',reason='expected-calls',token=hex(token),expected=count,actual=actual))
                        for at,width,value in cfg.get('expect_memory',[]):
                            actual=int.from_bytes(u.mem_read(at,width),'little');expected=value&((1<<(width*8))-1)
                            if actual!=expected:errors.append(dict(kind='native-authored-expectation',reason='expected-memory',address=hex(at),width=width,expected=expected,actual=actual))
                    elif route!='native'and not errors:
                        if mode!='discard-physical-EAX'and(r0&mask)!=(int(native_record['physical_EAX'],16)&mask):errors.append(dict(kind='native-vs-C',reason='result',native=native_record['physical_EAX'],compiled=hex(r0),mask=hex(mask)))
                        detail=snapshot_differences(reference,snapshot)
                        if detail:errors.append(dict(kind='native-vs-C',reason='trace/RAM-mismatch',differences=detail))
                    record['comparisons_agree']=not errors and route!='native';record['errors']=errors
                    if errors:
                        prefix='%04d-%s-%s'%(len(failures)+1,label,route)
                        artifact=archive_route(prefix,dict(case_id=case['id'],entry=hex(case['fn']),origin=case['origin'],cfg_repr=repr(cfg),**record),snapshot)
                        if reference is not None and reference_artifact is None:
                            reference_artifact=artifact if route=='native'else archive_route(prefix+'-native-reference',dict(case_id=case['id'],entry=hex(case['fn']),origin=case['origin'],cfg_repr=repr(cfg),**native_record),reference)
                        for error in errors:
                            failure=dict(id=case['id'],entry=hex(case['fn']),origin=case['origin'],target=label,route=route,artifact=artifact,native_reference_artifact=reference_artifact,**error)
                            failures.append(failure);case_failures.append(failure);journal('failures.jsonl',failure)
                    routes_done.append(record);journal('routes.jsonl',dict(id=case['id'],**record))
            row=dict(id=case['id'],entry=hex(case['fn']),origin=case['origin'],routes=routes_done,comparisons_agree=not case_failures,native_C_comparisons_agree=reference is not None and not any(f['kind']in{'execution/fault','native-vs-C'}for f in case_failures),authored_expectations_satisfied=reference is not None and not any(f['kind']=='native-authored-expectation'for f in case_failures),failure_count=len(case_failures),native_reference_available=reference is not None)
            observations.append(row);journal('cases.jsonl',row);print('failed'if case_failures else'compared',case['id'],dict(Counter(f['kind']for f in case_failures))if case_failures else'',flush=True)
        admit(args.freeze,args.go)
        assert all(digest(Path(p))==v for p,v in input_hashes.items()),'Replay/case input changed during execution'
        receipt=dict(schema=3,status='failed-partial-comparisons'if failures else'partial-comparisons-completed',freeze=binding,classification_counts=frozen['classification_counts'],case_inventory=inventory,checked_cases=len(observations),agreeing_cases=sum(o['comparisons_agree']for o in observations),failed_cases=sum(not o['comparisons_agree']for o in observations),native_C_agreeing_cases=sum(o['native_C_comparisons_agree']for o in observations),native_C_differing_cases=len({f['id']for f in failures if f['kind']=='native-vs-C'}),native_reference_unavailable_cases=sum(not o['native_reference_available']for o in observations),authored_expectation_failure_cases=len({f['id']for f in failures if f['kind']=='native-authored-expectation'}),route_attempts=attempts,route_emulation_returns=invocations,route_invocations=attempts,observations=observations,failures=failures,failures_by_kind=dict(Counter(f['kind']for f in failures)),RAW_lanes_admitted=False,full_cohort_validation_complete=False,coverage_complete=False,actual_composition_cases_attempted=sum(o['origin']=='actual-composition'for o in observations),actual_child_entry_observations=sum(sum(r.get('child_counts',{}).values())for o in observations if o['origin']=='actual-composition'for r in o['routes']),host_game_launched=False,input_hashes=input_hashes,archived_input_hashes=archived,limitations=['Finite named-C comparisons. No complete cohort coverage, raw frame or full gameplay equivalence claim. Child results are controlled only for explicitly listed protocol tokens. Independent cases reset writable data, stack, general registers, x87 controls/status/tag and MXCSR. Native authored expectations are checked separately and never replace native/C comparisons.'])
        (out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print('FAILED PARTIAL'if failures else'PARTIAL','comparison receipt',out/'receipt.json',flush=True)
        return not failures
    except Exception as exc:
        (out/'failure.json').write_text(json.dumps(dict(status='failed-infrastructure',error=repr(exc),freeze=binding,completed_observations=observations,failures=failures,input_hashes=input_hashes,archived_input_hashes=archived,full_cohort_validation_complete=False),indent=2)+'\n');raise

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--cases',action='append',type=Path,default=[]);p.add_argument('--plan',action='store_true');p.add_argument('--run',action='store_true');p.add_argument('--freeze',type=Path);p.add_argument('--go');p.add_argument('--budget',type=int,default=50000000);p.add_argument('--case-id',action='append',default=[]);p.add_argument('--output-dir',type=Path,default=ROOT/'build/actor-animal-following/cohort2-verification/runs')
    args=p.parse_args();paths=args.cases or[HERE/'smoke_cases.py'];cases,children,sources,inventory=plan(paths)
    if args.case_id:
        cases=[c for c in cases if c['id']in args.case_id];assert len(cases)==len(set(args.case_id))
        inventory['selected_cases']=len(cases);inventory['selected_case_ids']=args.case_id
    if not args.run:print(json.dumps(inventory,indent=2));return
    assert args.freeze and args.go,'Explicit admitted freeze and root GO epoch required'
    if not execution(args,cases,children,sources,inventory):raise SystemExit(1)
if __name__=='__main__':main()
