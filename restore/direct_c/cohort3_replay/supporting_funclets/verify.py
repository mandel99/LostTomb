"""Observe actual supporting funclets reached by real parent trajectories."""
from pathlib import Path
import datetime,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
from registry import load,symbol,save
import replay
from cases import definitions
registry=load(True)
assert registry['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes())
for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
    assert __import__('hashlib').sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
from native_test_pe import PE
native_image=PE(R/registry['native']['path'])
terminal_fragment=bytes(native_image.data[0x563890-native_image.base:0x56389a-native_image.base])
assert terminal_fragment.hex()=='e800000000e954ffffff'
# Native CALL563895;563895 JMP5637EE. The preserved C parent invokes
# the same declared nullary/noreturn5637EE boundary directly.
assert 'f->registration.state=-1;\n FUN_005637ee();'in (R/'src/unit_02857.c').read_text()
contracts=[]
for name in ['cohort7-eh3-supporting-funclets.json','cohort7-exception-lifetime-supporting-funclets.json']:
    contracts+=json.loads((R/'metadata'/name).read_bytes())['entries']
ids={c['necessary_case']for c in contracts}
cases=[c for c in definitions(registry)if c['id']in ids]
assert {c['id']for c in cases}==ids
members={int(m['address'],16):m for m in registry['members']}
maps={label:(R/image['map_path']).read_text()for label,image in registry['compiled_images'].items()}
labels=['native',*registry['compiled_images']]
queues={label:[]for label in labels}
for c in cases:
    m=members[c['entry']]
    for label in labels:
        if label=='native':routes=[('native',c['entry'])]
        else:
            p=m['providers'][label];routes=[]
            if p['named']:routes.append(('named',p['named']['va']))
            canonical=p['canonical']
            if canonical and (Path(m['source']).stem+'.obj'in canonical['owner']or c.get('canonical_owner')and c['canonical_owner']in canonical['owner']):
                routes.append(('canonical',canonical['va']))
        if 'routes'in c:routes=[r for r in routes if r[0]in c['routes']]
        wanted=[f for f in contracts if int(f['necessary_case'].split('/')[1],16)==c['entry']]
        for route,pc in routes:
            targets={}
            for f in wanted:
                target=int(f['native_address'],16)if label=='native'else symbol(maps[label],f['compiled_symbol'])['va']
                targets.setdefault(target,[]).append(f)
            queues[label].append(dict(case=c['id'],route=route,start=pc,targets=targets))

import unicorn
import unicorn.x86_const as x
OriginalUc=unicorn.Uc
traces=[];created=[]


class ObservedUc:
    def __init__(self,*args,**kwargs):
        self.inner=OriginalUc(*args,**kwargs)
        self.image=labels[len(created)];created.append(self.image)
        self.position=0;self.current=None;self.returns=[]
        self.hook_add(unicorn.UC_HOOK_CODE,self.observe)

    def __getattr__(self,name):
        return getattr(self.inner,name)

    def observe(self,uc,address,size,user_data):
        if self.current is None:return
        sp=self.reg_read(x.UC_X86_REG_ESP)
        for pending in list(self.returns):
            if address==pending['return_pc']and sp==pending['entry_sp']+4:
                pending['return_eax']=self.reg_read(x.UC_X86_REG_EAX)
                pending['return_sp']=sp
                self.returns.remove(pending)
        for f in self.current['targets'].get(address,[]):
            event=dict(token=f['compiled_symbol'],native_address=f['native_address'],pc=address,
                entry_sp=sp,entry_bp=self.reg_read(x.UC_X86_REG_EBP),entry_eax=self.reg_read(x.UC_X86_REG_EAX))
            self.current['events'].append(event)
            if 'filter'in f['compiled_symbol']:
                event['return_pc']=int.from_bytes(self.mem_read(sp,4),'little')
                self.returns.append(event)

    def emu_start(self,begin,until,*args,**kwargs):
        # Shared replay invokes one emu_start per physical route; no fixture
        # creates an artificial funclet frame or jumps directly to a funclet.
        expected=queues[self.image][self.position]
        assert begin==expected['start'],(self.image,self.position,hex(begin),expected['case'])
        assert until==replay.STOP
        row={k:expected[k]for k in ['case','route','start']}
        row.update(image=self.image,events=[])
        self.current=dict(expected,events=row['events']);self.returns=[]
        traces.append(row);self.position+=1
        try:return self.inner.emu_start(begin,until,*args,**kwargs)
        finally:
            row['filters_without_observed_return']=[p['token']for p in self.returns]
            self.current=None


directory=R/'build/actor-animal-following/cohort7-funclets-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
directory.mkdir(parents=True,exist_ok=False)
unicorn.Uc=ObservedUc
try:result=replay.execute(registry,cases,directory)
finally:unicorn.Uc=OriginalUc
save(directory/'funclet-trace.json',dict(contracts=contracts,observations=traces,
    observation_only=True,synthetic_funclet_frames=False,host_execution=False))
assert len(traces)==result['invocations']
parent_routes={(c['id'],r['image'],r['route']):r for c in result['cases_detail']for r in c['routes']}
nonreturn_filters=[]
for t in traces:
    parent=parent_routes[t['case'],t['image'],t['route']]
    for e in t['events']:
        if 'filter'in e['token']and 'return_eax'not in e:
            assert parent['termination'],('Unexplained filter without return',t['case'],t['image'],e['token'])
            nonreturn_filters.append(dict(case=t['case'],image=t['image'],route=t['route'],
                token=e['token'],actual_parent_boundary=parent['termination']))
summary=[]
for c in cases:
    observations=[t for t in traces if t['case']==c['id']]
    native=next(t for t in observations if t['image']=='native')
    expected=[f['compiled_symbol']for f in contracts if f['necessary_case']==c['id']]
    native_tokens=[e['token']for e in native['events']]
    missing=[token for token in expected if token not in native_tokens]
    native_returns=[(e['token'],e.get('return_eax'))for e in native['events']if 'filter'in e['token']]
    mismatch=[];semantic_mismatch=[];edge_replacements=[]
    for t in observations:
        tokens=[e['token']for e in t['events']]
        if tokens!=native_tokens:
            difference=dict(image=t['image'],route=t['route'],kind='visit-order')
            mismatch.append(difference)
            allowed=(c['id']=='c7-eh3/00563844/dispatch'and
                native_tokens==['restore_crt_eh3_execute_terminate_filter','restore_crt_terminate_recover','restore_crt_terminate_finally']and
                tokens==['restore_crt_eh3_execute_terminate_filter','restore_crt_terminate_recover']and
                parent_routes[t['case'],t['image'],t['route']]['termination']=='terminate')
            if allowed:edge_replacements.append(dict(image=t['image'],route=t['route'],
                native='CALL563895 -> JMP5637EE',compiled='direct same nullary/noreturn5637EE boundary',
                native_bytes=terminal_fragment.hex(),source='src/unit_02857.c'))
            else:semantic_mismatch.append(difference)
        returns=[(e['token'],e.get('return_eax'))for e in t['events']if 'filter'in e['token']]
        if returns!=native_returns:
            difference=dict(image=t['image'],route=t['route'],kind='filter-result')
            mismatch.append(difference);semantic_mismatch.append(difference)
    summary.append(dict(case=c['id'],expected=expected,native_visits=native_tokens,
        native_filter_returns=native_returns,missing=missing,mismatches=mismatch,
        permitted_terminal_edge_replacements=edge_replacements,semantic_mismatches=semantic_mismatch))
checks=dict(parent_cases=len(cases),parent_invocations=result['invocations'],
    parent_failed_cases=result['failed_cases'],inputs_stable=result['inputs_stable'],
    supporting_funclets=len(contracts),summaries=summary,
    all_declared_funclets_observed=not any(s['missing']for s in summary),
    native_C_observations_match=not any(s['mismatches']for s in summary),
    semantic_observations_match=not any(s['semantic_mismatches']for s in summary),
    actual_parent_stack_frames_valid=all(replay.STACK<=e['entry_sp']<replay.STACK+replay.STACK_SIZE and
        replay.STACK<=e['entry_bp']<replay.STACK+replay.STACK_SIZE for t in traces for e in t['events']),
    filters_without_observed_return=sum(len(t['filters_without_observed_return'])for t in traces),
    nonreturn_filters=nonreturn_filters,
    new_source_credit=0,new_entry_credit=0,host_execution=False,whole_game_equivalence=False)
save(directory/'observation-checks.json',checks)
print(json.dumps(dict(result=str(directory/'result.json'),checks=str(directory/'observation-checks.json'),
    **{k:checks[k]for k in ['parent_cases','parent_invocations','parent_failed_cases','inputs_stable','all_declared_funclets_observed','native_C_observations_match','filters_without_observed_return']})),flush=True)
assert not result['failed_cases']and result['inputs_stable']
assert checks['semantic_observations_match']and checks['all_declared_funclets_observed']and checks['actual_parent_stack_frames_valid']
