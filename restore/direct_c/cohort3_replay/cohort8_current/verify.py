"""Explicit C8 image/source overlay for unchanged C7 finite comparison recipes."""
from pathlib import Path
import argparse,copy,datetime,hashlib,importlib.util,inspect,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent
R=OUT.parents[2]
PUBLIC=R/'direct_c/cohort3_replay'
sys.path[:0]=[str(PUBLIC),str(R/'direct_c')]
from registry import load,save,digest,symbol


def read(path):return json.loads(Path(path).read_bytes())


def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    result=importlib.util.module_from_spec(spec)
    sys.path.insert(0,str(path.parent));spec.loader.exec_module(result)
    return result


def manifest(folder,name='manifest.json'):
    if not(folder/name).exists():
        assert folder.name=='quicktime_support'
        return # Every original packet file is independently pinned by epoch.json.
    for item in read(folder/name)['files']:
        assert digest(folder/item['path'])==item['sha256'],(folder.name,item['path'])


def checked_base():
    manifest(OUT)
    epoch=read(OUT/'epoch.json')
    for name,sha in epoch['source_guards'].items():assert digest(R/name)==sha,name
    for name,sha in epoch['packet_files'].items():assert digest(PUBLIC/name)==sha,name
    base=load(True)
    assert base['compiled_images']==epoch['compiled_images']
    assert digest(PUBLIC/'replay.py')==epoch['source_replay_sha256']
    return base


def rebind_guards(guards,packet):
    allowed=read(OUT/'epoch.json')['known_source_rebindings']
    result=copy.deepcopy(guards)
    for name,old in guards.items():
        now=digest(R/name)
        if now!=old:
            assert dict(path=name,before_sha256=old,sha256=now)in allowed,(packet,name,old,now)
        result[name]=now
    return result


def protocol(packet,base,changes=None):
    folder=PUBLIC/packet;manifest(folder)
    p=read(folder/'protocol.json')
    for key in ['strict_source_guards','source_guards','guarded_files']:
        if key in p:
            before=p[key];p[key]=rebind_guards(before,packet)
            if changes is not None:
                changes.extend(dict(packet=packet,path=n,before_sha256=s,sha256=p[key][n])for n,s in before.items()if p[key][n]!=s)
    for item in p.get('evidence_inputs',[]):assert digest(R/item['path'])==item['sha256'],item['path']
    if 'native'in p:assert p['native']==base['native']and digest(R/p['native']['path'])==p['native']['sha256']
    if 'compiled_images'in p:
        assert p['compiled_images']==read(OUT/'epoch.json')['original_packet_profiles'][packet],packet
        p['compiled_images']=base['compiled_images']
    if packet=='compiled_actor_tail':
        c=next(c for c in p['members']if int(c['address'],16)==0x542ee0)
        c['canonical_images']=dict(game=False,diagnostic=False,retained=True)
    return p


def compiled47(base):
    old=module('c8_reused_current_matrix',PUBLIC/'current_matrix/verify.py')
    manifest(PUBLIC/'current_matrix')
    old.OUT=OUT
    old.checked_protocol=lambda packet,current,changes:protocol(packet,current,changes)
    return old.bind()


def adapted_function(fn,old,new,extra):
    """Replace one protocol acquisition, preserving the checked binder body."""
    source=inspect.getsource(fn);assert source.count(old)==1
    scope=dict(fn.__globals__);scope.update(extra)
    exec(compile(source.replace(old,new),fn.__code__.co_filename+' [C8 binding]','exec'),scope)
    return scope[fn.__name__]


def canonical_registry(base,addresses):
    """Bind existing C8 canonical procedures, without inventing a named ABI."""
    from native_test_pe import PE
    native=PE(R/base['native']['path']);result=copy.deepcopy(base)
    source=read(R/'metadata/cohort8-source100-local-value-repairs.json')
    maps={label:(R/image['map_path']).read_text()for label,image in base['compiled_images'].items()}
    existing={int(m['address'],16)for m in base['members']}
    for address in addresses:
        assert address not in existing
        m=next(m for m in source['members']if int(m['address'],16)==address)
        assert digest(R/m['source'])==m['source_sha256']
        entries=[]
        for path in m['native_cache_inputs']:
            cache=read(R/path);assert cache['native_sha256']==base['native']['sha256']
            entries += [e for e in cache['entries']if int(e['requested_address'],16)==address]
        assert len(entries)==1 and entries[0]['status']=='exact_ghidra_function'
        entry=entries[0];assert not entry['assembly']['has_more']
        ranges=[]
        for i in entry['assembly']['items']:
            at=int(i['address'],16);raw=bytes.fromhex(i['bytes'])
            assert native.data[at-native.base:at-native.base+len(raw)]==raw
            if ranges and ranges[-1][1]==at:ranges[-1][1]+=len(raw)
            else:ranges.append([at,at+len(raw)])
        providers={}
        for label,text in maps.items():
            canonical=symbol(text,m['name']);assert canonical==m['providers'][label]
            if canonical:assert Path(m['source']).stem+'.obj'in canonical['owner']
            providers[label]=dict(named=None,canonical=canonical)
        assert providers['retained']['canonical']
        result['members'].append(dict(address=hex(address),name=m['name'],source=m['source'],
            algorithm_source=m['source'],source_definition_symbol=m['name'],source_sha256=m['source_sha256'],
            body_sha256=m['source_sha256'],body_hash_scope='complete existing source unit, including macro-generated entry',
            native_ranges=[dict(start=hex(start),end=hex(end-1))for start,end in ranges],native_ret_immediates=[0],
            logical_signature='Native canonical entry; exact logical contract is required in each recipe',providers=providers,
            rule=None,supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
    result['comparison_entries']=len(result['members'])
    return result


def recipes(scope,base):
    matrix=module('c8_authored_audit',PUBLIC/'current_matrix/verify.py')
    if scope=='tooltip-support':
        authored=module('c8_tooltip_support_cases',OUT/'tooltip_support_cases.py')
        registry=canonical_registry(base,[0x52da40])
        return registry,authored.cases(registry),matrix.audit,12,48
    if scope=='source100':
        from cases import definitions
        addresses={int(m['address'],16)for m in read(R/'metadata/cohort7-source-progress.json')['members']}
        cases=[c for c in definitions(base)if c['entry']in addresses]
        return base,cases,matrix.audit,606,4233
    if scope=='compiled47':
        registry,cases=compiled47(base)
        return registry,cases,matrix.audit,299,1757
    if scope=='quicktime':
        folder=PUBLIC/'quicktime_support';manifest(folder)
        old=module('c8_quicktime',folder/'verify.py')
        p=protocol('quicktime_support',base)
        binder=adapted_function(old.bind,"p = json.loads((OUT/'protocol.json').read_bytes())",'p = copy.deepcopy(C8_PROTOCOL)',dict(C8_PROTOCOL=p))
        registry=binder(base)
        cases=old.core_cases.cases(registry)+old.window_cases.definitions(registry)
        for c in cases:
            m=next(m for m in registry['members']if int(m['address'],16)==c['entry'])
            c['expected_invocations']=1+sum(bool(p['named'])for p in m['providers'].values())
        return registry,cases,lambda cases,report:old.inspect(registry,cases,report),51,204
    packet={'production':'production_expanded','tooltip':'tooltip_modes_queries','civic':'civic_main_expanded','hunter':'hunter_states_expanded'}[scope]
    folder=PUBLIC/packet
    manifest(folder,'publication-manifest.json'if scope in ['tooltip','hunter']else'manifest.json')
    if scope=='tooltip':
        old=module('c8_tooltip',folder/'current_verify.py')
        binding=old.binding
        p=protocol(packet,base)
        # Preserve the author's explicit model-endpoint extension qualification.
        rebound=read(folder/'replay-rebind.json')
        assert digest(folder/'protocol.json')==rebound['authoring_protocol_sha256']
        p['explicit_replay_source_rebind']=rebound
        binding.checked_inputs=lambda require_extension=False:copy.deepcopy(p)
        registry=binding.extend_registry(base);cases=old.corrected_cases(registry)
        return registry,cases,matrix.audit,31,217
    old=module('c8_'+scope,folder/('verify_current.py'if scope=='hunter'else'verify.py'))
    p=protocol(packet,base)
    binding=old.binding
    binding.checked_inputs=lambda:protocol('compiled_final6',base)
    registry=binding.extend_registry(base)
    if scope=='production':cases=old.production_cases.cases(registry);checker=old.checks;count=26;invocations=182
    elif scope=='civic':cases=old.civic_cases.cases(registry);checker=old.checks;count=43;invocations=301
    else:
        checked=adapted_function(old.guard.checked_cases,"protocol=json.loads((OUT/'protocol.json').read_bytes())",'protocol=copy.deepcopy(C8_PROTOCOL)',dict(C8_PROTOCOL=p,copy=copy))
        checked()
        cases=old.hunter_cases.cases(registry);count=30;invocations=210
        def checker(cases,report):
            result=matrix.audit(cases,report)
            result['detailed_checks']=old.guard.audit(cases,report)
            return result
    # Validate native bytes and ROM inputs independently of any image rebinding.
    from native_test_pe import PE
    native=PE(R/base['native']['path'])
    evidence_names={'production':['expanded-native-evidence.json'],'civic':[],'hunter':[]}[scope]
    for name in evidence_names:
        for record in read(folder/name)['records']:
            for i in record['member']['assembly']['items']:
                at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes'])
                assert native.data[at:at+len(raw)]==raw
    if scope=='civic':
        assert digest(folder/'civic_cases.py')==p['authored_recipe_sha256']
        for key in ['native_evidence','predicate_evidence']:
            ev=p[key];assert digest(R/ev['path'])==ev['sha256']
            data=read(R/ev['path']);member=next(m for m in data.get('members',data.get('entries',[]))if m['requested_address']==ev['address'])
            for i in member['assembly']['items']:
                at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes'])
                assert native.data[at:at+len(raw)]==raw
        for address,payload in p['ROM_inputs'].items():
            at=int(address,16)-native.base;raw=bytes.fromhex(payload)
            assert native.data[at:at+len(raw)]==raw
    return registry,cases,checker,count,invocations


def run_script(scope,base):
    """Only replace a pinned profile read and output path in old observers."""
    packet={'funclets':'supporting_funclets','arithmetic':'arithmetic_faults'}[scope]
    folder=PUBLIC/packet;manifest(folder)
    path=folder/'verify.py';source=path.read_text()
    before="json.loads((OUT/'image-profile.json').read_bytes())"
    assert source.count(before)==1
    source=source.replace(before,'C8_COMPILED_IMAGES')
    old_directory='cohort7-funclets-public-runs'if scope=='funclets'else'cohort7-arithmetic-fault-public-runs'
    # Arithmetic has a fixed report filename; see the separately recorded adapter.
    if scope=='funclets':
        assert source.count(old_directory)==1
        source=source.replace(old_directory,'cohort8-funclets-current-runs')
    else:
        old_directory='cohort7-arithmetic-fault-current-comparison'
        assert source.count(old_directory)==1
        source=source.replace(old_directory,'cohort8-arithmetic-fault-current-comparison')
    scope_globals=dict(__file__=str(path),__name__='__main__',C8_COMPILED_IMAGES=base['compiled_images'])
    exec(compile(source,str(path)+' [C8 image binding]','exec'),scope_globals)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--scope',required=True,choices=['source100','compiled47','quicktime','production','tooltip','civic','hunter','funclets','arithmetic','tooltip-support'])
    ap.add_argument('--run',action='store_true');args=ap.parse_args()
    base=checked_base()
    if args.scope in ['funclets','arithmetic']:
        if not args.run:
            print(json.dumps(dict(scope=args.scope,execution=False,compiled_images=3)));return
        run_script(args.scope,base);return
    registry,cases,checker,count,invocations=recipes(args.scope,base)
    assert len(cases)==count and len({c['id']for c in cases})==count
    if not args.run:
        print(json.dumps(dict(scope=args.scope,cases=count,expected_invocations=invocations,execution=False)));return
    import replay
    directory=R/'build/actor-animal-following/cohort8-current-matrix-runs'/(args.scope+'-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f'))
    directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
    save(directory/'epoch.json',read(OUT/'epoch.json'))
    original=replay.load
    # Reconstruct and revalidate the same registry at the end of every run.
    replay.load=lambda require_retained=True:recipes(args.scope,checked_base())[0]
    try:report=replay.execute(registry,cases,directory)
    finally:replay.load=original
    print(json.dumps(dict(result=str(directory/'result.json'),**{k:report[k]for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']})),flush=True)
    assert report['invocations']==invocations
    save(directory/'authored-checks.json',checker(cases,report))


if __name__=='__main__':main()
