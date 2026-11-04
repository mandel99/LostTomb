"""Static evidence/CFG/caller audit for the1922-instruction animal family.

Portable from this preparation directory to restore/direct_c: all native
evidence/specifications are local metadata, with no import of build scripts.
This never executes original, restored, candidate or platform code.
"""
from pathlib import Path
import argparse,hashlib,json,re,struct,sys,subprocess
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in [HERE,*HERE.parents] if (p/'direct_c/native_test_pe.py').exists() and (p/'runtime').is_dir())
DATA=HERE if (HERE/'actor-animal-following-native.json').exists() else ROOT/'metadata'
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--require-production',action='store_true',help='Require five named/canonical entries, four new RAW bodies, exact retained RAW cue and the explicit five-site typed contract.')
options=parser.parse_args()
NATIVE_SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=ROOT/'build/smoke-game/Pharaoh.exe'
assert hashlib.sha256(native.read_bytes()).hexdigest()==NATIVE_SHA
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
catalogue_path=DATA/'actor-animal-following-native.json'
names_path=DATA/'actor-animal-following-names-evidence.json'
catalogue=json.loads(catalogue_path.read_text());assert catalogue['native_sha256']==NATIVE_SHA
assert catalogue['owned_instruction_count']==1922
evidence_paths=[DATA/name for name in catalogue['evidence_files']]
records=[];windows={}
for path in evidence_paths:
    for wrapped in json.loads(path.read_text())['records']:
        record=wrapped.get('result',wrapped)
        assert record['authority']=='shipped-artifact' and record['subject']['digest']['sha256']==NATIVE_SHA
        records.append(record)
        if record['operation']!='read_bytes':continue
        result=record['normalized_result'];assert result['complete']
        at=int(result['address'],16);raw=bytes.fromhex(result['bytes_hex'])
        assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])
        windows[at,len(raw)]=(raw,record)
def authenticated(at,length):
    matches=[(raw[at-start:at-start+length],record) for (start,size),(raw,record) in windows.items() if start<=at and at+length<=start+size]
    assert matches and all(value[0]==matches[0][0] for value in matches),('missing authenticated bytes',hex(at),length)
    return matches[0]
def exact(by,pc,mnemonic,operands):
    i=by[pc];assert (i.mnemonic,i.op_str)==(mnemonic,operands),(hex(pc),i.mnemonic,i.op_str)

specs={0x49d0f0:(2782,728,'restore_actor_update_crocodile'),
       0x49dc50:(3036,775,'restore_actor_update_hyena'),
       0x4b5050:(290,119,'restore_actor_play_kind_cue'),
       0x4ecd40:(339,99,'restore_actor_choose_crocodile_state'),
       0x4eca70:(720,201,'restore_actor_enter_animal_state')}
functions={};instruction_owner={};all_instructions={}
assert len(catalogue['functions'])==5
for function in catalogue['functions']:
    entry=int(function['address'],16);size,count,name=specs[entry]
    assert (function['byte_count'],function['instruction_count'],function['name'])==(size,count,name)
    raw,record=authenticated(entry,size)
    assert bytes.fromhex(function['bytes_hex'])==raw and function['bytes_evidence_id']==record['evidence_id']
    code=list(decoder.disasm(raw,entry));by={i.address:i for i in code}
    assert len(code)==count and sum(i.size for i in code)==size
    assert code[-1].address+code[-1].size==entry+size and code[-1].mnemonic=='ret'
    assert not set(by)&set(all_instructions)
    all_instructions.update(by);instruction_owner.update({a:entry for a in by})
    functions[entry]=dict(code=code,by=by,name=name)
assert len(all_instructions)==1922

switches=[];indirect={}
guards={0x49d23e:(0x49d230,0x49da54,'esi'),0x49daa2:(0x49da94,0x49db91,'esi'),
        0x49dd25:(0x49dd17,0x49e6c7,'eax'),0x49e73c:(0x49e72e,0x49e824,'esi'),
        0x4b507c:(0x4b506e,0x4b514d,'eax')}
assert len(catalogue['switches'])==5
for spec in catalogue['switches']:
    entry=int(spec['owner'],16);by=functions[entry]['by'];site=int(spec['site'],16)
    selector=int(spec['selector_address'],16);table=int(spec['pointer_table'],16)
    values=bytes(spec['selector_values']);targets=tuple(int(t,16) for t in spec['targets'])
    first,last=spec['input_range'];assert len(values)==last-first+1
    assert authenticated(selector,len(values))[0]==values
    assert struct.unpack('<'+'I'*len(targets),authenticated(table,len(targets)*4)[0])==targets
    assert all(value<len(targets) for value in values) and all(target in by for target in targets)
    guard,default,index=guards[site]
    exact(by,guard,'ja',hex(default))
    exact(by,guard-3,'cmp',index+', '+hex(len(values)-1))
    exact(by,site-8,'xor','edx, edx')
    exact(by,site-6,'mov','dl, byte ptr ['+index+' + '+hex(selector)+']')
    exact(by,site,'jmp','dword ptr [edx*4 + '+hex(table)+']')
    # No branch may enter the selector block after its unsigned bounds guard.
    for i in by.values():
        if (i.mnemonic.startswith('j') and i.mnemonic!='jmp') or (i.mnemonic=='jmp' and i.op_str.startswith('0x')):
            target=int(i.op_str,16)
            assert not guard+by[guard].size<=target<=site,('bounds bypass',hex(i.address),hex(target))
    assert {str(k+first):hex(targets[v]) for k,v in enumerate(values)}==spec['state_to_target']
    indirect[site]=set(targets)
    switches.append(dict(spec,guard=hex(guard),default=hex(default)))
for entry,function in functions.items():
    by=function['by'];todo=[entry];seen=set();returns=set()
    while todo:
        at=todo.pop()
        if at in seen:continue
        assert at in by,('CFG leaves owned body',hex(entry),hex(at))
        seen.add(at);i=by[at]
        if i.mnemonic=='ret':returns.add(at);continue
        if i.mnemonic=='jmp':todo.extend(indirect[at] if at in indirect else [int(i.op_str,16)])
        elif i.mnemonic.startswith('j'):todo.extend([int(i.op_str,16),at+i.size])
        else:todo.append(at+i.size)
    assert seen==set(by)
    function['returns']=[hex(a) for a in sorted(returns)]

dead=[];dead_edges=[];qualifications=[]
for qualification in catalogue['outcome_qualifications']:
    entry=int(qualification['owner'],16);by=functions[entry]['by']
    if qualification['kind']=='signed_remainder_sign_bit':
        first=int(qualification['first_mask'],16);second=int(qualification['second_mask'],16)
        branch=int(qualification['branch'],16);target=int(qualification['always_taken_target'],16)
        exact(by,first,'and','edi, 0x7fff');exact(by,second,'and','edi, 0x8000000f');exact(by,branch,'jns',hex(target))
        for i in by.values():
            if first<i.address<second:assert 'edi' not in [i.reg_name(r) for r in i.regs_access()[1]]
            if second<i.address<branch:assert 'eflags' not in [i.reg_name(r) for r in i.regs_access()[1]]
        negative=[by[a] for a in sorted(by) if branch+by[branch].size<=a<target]
        assert [(i.mnemonic,i.op_str) for i in negative]==[('dec','edi'),('or','edi, 0xfffffff0'),('inc','edi')]
        dead.extend(i.address for i in negative);dead_edges.append((branch,negative[0].address))
    elif qualification['kind']=='initializer_signed_remainder_sign_bit':
        assert entry==0x4eca70
        exact(by,0x4ecc2e,'and','edx, 0x7fff')
        exact(by,0x4ecc39,'mov','eax, edx')
        exact(by,0x4ecc41,'and','eax, 0x80000007')
        exact(by,0x4ecc58,'jns','0x4ecc5f')
        for i in by.values():
            if 0x4ecc2e<i.address<0x4ecc39:assert 'edx' not in [i.reg_name(r) for r in i.regs_access()[1]]
            if 0x4ecc39<i.address<0x4ecc41:assert 'eax' not in [i.reg_name(r) for r in i.regs_access()[1]]
            if 0x4ecc41<i.address<0x4ecc58:assert 'eflags' not in [i.reg_name(r) for r in i.regs_access()[1]]
        negative=[by[a] for a in [0x4ecc5a,0x4ecc5b,0x4ecc5e]]
        assert [(i.mnemonic,i.op_str) for i in negative]==[('dec','eax'),('or','eax, 0xfffffff8'),('inc','eax')]
        dead.extend(i.address for i in negative);dead_edges.append((0x4ecc58,0x4ecc5a))
    else:
        assert qualification['kind']=='bounded_cumulative_weights' and entry==0x4ecd40
        for a,m,o in [(0x4ecd5e,'mov','dword ptr [esp + 4], 0x19'),(0x4ecd66,'mov','dword ptr [esp + 8], 0x4b'),
                      (0x4ecd70,'mov','dword ptr [esp + 4], 0x5a'),(0x4ecd78,'mov','dword ptr [esp + 8], 0xa'),
                      (0x4ecd82,'mov','eax, 0x32'),(0x4ecd87,'mov','dword ptr [esp + 4], eax'),(0x4ecd8b,'mov','dword ptr [esp + 8], eax'),
                      (0x4ece35,'and','eax, 0x7fff'),(0x4ece51,'cdq',''),(0x4ece52,'mov','ecx, 0x64'),(0x4ece5a,'idiv','ecx'),
                      (0x4ece62,'xor','eax, eax'),(0x4ece68,'add','edi, dword ptr [ecx]'),(0x4ece6a,'cmp','edx, edi'),
                      (0x4ece6c,'jl','0x4ece77'),(0x4ece72,'cmp','eax, 2'),(0x4ece75,'jl','0x4ece68')]:exact(by,a,m,o)
        assert all(sum(pair)==100 for pair in qualification['weights'].values())
        dead_edges.append((0x4ece75,0x4ece77))
    qualifications.append(qualification)
assert len(dead)==15 and len(dead_edges)==6
assert sorted(hex(a) for a in dead)==sorted(catalogue['analytically_unreachable_instructions'])
assert sorted([[hex(a),hex(b)] for a,b in dead_edges])==sorted(catalogue['analytically_unreachable_edges'])

# Names are original model labels, tied to the exact native loader and kind
# indexing. Assets are raw original inputs, never imports of preparation code.
names=json.loads(names_path.read_text());assert names['native_sha256']==NATIVE_SHA
assert names['established_names']=={'82':'Crocodile','83':'Hyena','84':'Hippo'}
for window in names['loader_windows']:
    raw,_=authenticated(int(window['address'],16),window['byte_count'])
    assert hashlib.sha256(raw).hexdigest()==window['sha256']
for field in ['loader_selection_instructions','loader_fill_instructions']:
    specification=names['loader_windows'][0 if field=='loader_selection_instructions' else 1]
    start=int(specification['address'],16)
    data,_=authenticated(start,specification['byte_count'])
    loader_by={i.address:i for i in decoder.disasm(data,start)}
    for address,mnemonic,operands in names[field]:
        # Decode from the exact instruction start, using an authenticated
        # containing window, rather than accepting asset labels as an index map.
        exact(loader_by,int(address,16),mnemonic,operands)
assert authenticated(0x601c54,12)[0]==b'ALL FIGURES\0'
assert struct.unpack('<III',authenticated(0x5dc3d4,12)[0])==(0x49d0f0,0x49dc50,0x49e8b0)
asset_hashes={}
for asset in names['asset_files']:
    path=ROOT/'build/smoke-game'/asset['name'];raw=path.read_bytes()
    assert hashlib.sha256(raw).hexdigest()==asset['sha256'];asset_hashes[path.relative_to(ROOT).as_posix()]=asset['sha256']
    section=raw.decode('cp1252').split('ALL FIGURES',1)[1].split('ALL ENEMIES',1)[0]
    rows=list(re.finditer(r'^\s*(\d+),([^,]+),\{([^}]*)\}',section,re.M))
    assert len(rows)==110 and all(int(row.group(1))==n for n,row in enumerate(rows))
    for kind,name in [(82,'Crocodile'),(83,'Hyena'),(84,'Hippo')]:assert rows[kind].group(2)==name
for entry,kind in [(0x49d0f0,82),(0x49dc50,83)]:
    by=functions[entry]['by']
    for address,mnemonic,operands in names['callback_kind_to_model_instructions'][str(kind)]:exact(by,int(address,16),mnemonic,operands)
assert 0x7090cc+8*4==0x7090ec and (0x70a8dc-0x7090cc)//56==110

# Inherit the tracked profiler consumer proof, including all of its source and
# SDK hashes. It is a dependency; no second interpretation of Win32 ABI is made.
profiler_path=ROOT/'metadata/actor-animal-parent-caller-audit.json'
profiler=json.loads(profiler_path.read_text());assert profiler['passed'] and profiler['native_sha256']==NATIVE_SHA
for name,sha in profiler['source_hashes'].items():assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==sha,('stale profiler dependency',name)
for name,sha in profiler['installed_SDK_contract_sources'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==sha
consumer=profiler['registered_callback_consumer']
assert consumer['passed'] and consumer['callback_table']=='0x5dc28c' and consumer['callback_site']=='0x489c52'
assert consumer['all_scratch_killed_before']=='0x489ab9'

# Qualify the two easily lost full-width callback results independently of
# their legacy void facades: fresh actorID versus byte offset, and20/8.
croc=functions[0x49d0f0]['by'];hyena=functions[0x49dc50]['by']
exact(croc,0x49d7b1,'mov','eax, dword ptr [0xd056f8]')
for at in [a for a in croc if 0x49d7b6<=a<=0x49d7cd]:
    assert 'eax' not in [croc[at].reg_name(r) for r in croc[at].regs_access()[1]]
exact(croc,0x49d7c2,'mov','byte ptr [ecx*4 + 0xbcd606], 2')
exact(croc,0x49d7cd,'ret','')
assert next(s for s in switches if s['site']=='0x49dd25')['state_to_target']['24']=='0x49defa'
for at,m,o in [(0x49dd09,'xor','eax, eax'),(0x49dd0b,'mov','al, byte ptr [ecx + 0xbcd630]'),
               (0x49dd11,'add','eax, -4'),(0x49defa,'dec','word ptr [ecx + 0xbcd62e]'),
               (0x49df01,'xor','edx, edx'),(0x49df03,'cmp','word ptr [ecx + 0xbcd62e], dx'),
               (0x49df0a,'jg','0x49df23'),(0x49df0c,'mov','eax, 8'),(0x49df30,'ret','')]:exact(hyena,at,m,o)
for at in [a for a in hyena if 0x49df23<=a<=0x49df30]:
    assert 'eax' not in [hyena[at].reg_name(r) for r in hyena[at].regs_access()[1]]

# Weighted state: preserve complete child EAX, and independently close the
# observed parent scratch outputs rather than inferring ABI from a void facade.
weighted=functions[0x4ecd40]['by']
refs=next(r['normalized_result'] for r in records if r['operation']=='procedure_references' and r['parameters']['procedure'].lower()=='0x4ecd40')
assert [r['source_address'] for r in refs['references'] if r['target_address']=='0x4ecd40']==['0x49d828']
for at,m,o in [(0x49d827,'push','ecx'),(0x49d828,'call','0x4ecd40'),(0x49d82d,'mov','ecx, dword ptr [0xd056f8]'),
               (0x49d833,'add','esp, 4'),(0x49d836,'jmp','0x49da5b'),(0x49da5b,'lea','eax, [ecx + ecx*2]'),
               (0x49da5e,'mov','edx, dword ptr [0xbca2e0]')]:exact(croc,at,m,o)
tail=[weighted[a] for a in sorted(weighted) if a>=0x4ece89]
assert [(i.mnemonic,i.op_str) for i in tail]==[('add','esp, 8'),('pop','edi'),('pop','esi'),('pop','ebx'),('add','esp, 0xc'),('ret','')]
assert struct.unpack('<III',authenticated(0x5e4ee0,12)[0])==(19,18,41)
child=functions[0x4eca70]['by']
aliases={'eax':'eax','ax':'eax','al':'eax','ah':'eax','ecx':'ecx','cx':'ecx','cl':'ecx','ch':'ecx','edx':'edx','dx':'edx','dl':'edx','dh':'edx','eflags':'eflags'}
todo=[(0x4eca70,frozenset(['eax','ecx','edx','eflags']))];seen=set();child_terminals=set()
while todo:
    at,pending=todo.pop()
    if(at,pending) in seen:continue
    seen.add((at,pending));i=child[at];reads,writes=i.regs_access()
    assert not {aliases[i.reg_name(r)] for r in reads if i.reg_name(r) in aliases}&pending
    after=set(pending)
    for r in writes:
        if i.reg_name(r) in ['eax','ecx','edx']:after.discard(i.reg_name(r))
    if i.mnemonic=='cmp':after.discard('eflags')
    if not after:child_terminals.add(at);continue
    assert i.mnemonic not in ['call','ret']
    targets=[int(i.op_str,16)] if i.mnemonic=='jmp' else [int(i.op_str,16),at+i.size] if i.mnemonic.startswith('j') else [at+i.size]
    todo.extend((target,frozenset(after)) for target in targets)
assert child_terminals=={0x4ecabb,0x4ecbb9}

# Newly owned initializer: explicit full state comparison, dynamic neighbor
# tables, meaningful search arguments and partial-register return semantics.
initializer_refs=next(r['normalized_result'] for r in records if r['operation']=='procedure_references' and r['parameters']['procedure'].lower()=='0x4eca70')
assert [r['source_address'] for r in initializer_refs['references'] if r['target_address']=='0x4eca70']==['0x4ece84']
assert {(i.address,int(i.op_str,16)) for i in child.values() if i.mnemonic=='call'}=={(0x4eccea,0x4eb800)}
for at,m,o in [(0x4eca70,'mov','eax, dword ptr [esp + 4]'),(0x4eca80,'mov','eax, dword ptr [esp + 0x18]'),
               (0x4eca87,'cmp','eax, 0x13'),(0x4eca8a,'mov','byte ptr [edi + 0xbcd630], al'),
               (0x4ecb55,'cdq',''),(0x4ecb56,'mov','ecx, 0x64'),(0x4ecb5e,'idiv','ecx'),
               (0x4ecc61,'mov','ecx, dword ptr [eax*4 + 0x5e4e80]'),(0x4ecc68,'mov','ebx, dword ptr [edi + 0xbcd618]'),
               (0x4ecc77,'test','cl, 4'),(0x4ecc7c,'test','ecx, 0x80000'),
               (0x4ecc94,'mov','cx, word ptr [eax*8 + 0x5e4ea0]'),(0x4ecc9c,'mov','ax, word ptr [eax*8 + 0x5e4ea4]'),
               (0x4ecca4,'add','cx, word ptr [edi + 0xbcd60c]'),(0x4eccab,'mov','word ptr [edi + 0xbcd61c], cx'),
               (0x4eccb2,'add','ax, word ptr [edi + 0xbcd60e]'),(0x4eccb9,'mov','word ptr [edi + 0xbcd61e], ax'),
               (0x4eccc5,'mov','ecx, dword ptr [esp + 0x14]'),(0x4eccc9,'lea','edx, [esp + 0x18]'),
               (0x4ecccd,'push','ecx'),(0x4eccce,'push','5'),(0x4eccd0,'movsx','ecx, word ptr [edi + 0xbcd60e]'),
               (0x4eccd7,'lea','eax, [esp + 0x1c]'),(0x4eccdb,'push','edx'),(0x4eccdc,'movsx','edx, word ptr [edi + 0xbcd60c]'),
               (0x4ecce3,'push','eax'),(0x4ecce4,'push','4'),(0x4ecce6,'push','ebp'),(0x4ecce7,'push','ebp'),
               (0x4ecce8,'push','ecx'),(0x4ecce9,'push','edx'),(0x4eccea,'call','0x4eb800'),
               (0x4eccf2,'test','eax, eax'),(0x4eccf4,'jne','0x4ecd1d'),
               (0x4eccf6,'mov','ax, word ptr [edi + 0xbcd60c]'),(0x4eccfd,'mov','word ptr [edi + 0xbcd61c], ax'),
               (0x4ecd04,'mov','cx, word ptr [edi + 0xbcd60e]'),(0x4ecd0b,'mov','word ptr [edi + 0xbcd61e], cx'),
               (0x4ecd1d,'mov','dx, word ptr [esp + 0x14]'),(0x4ecd22,'mov','ax, word ptr [esp + 0x18]'),
               (0x4ecd27,'mov','word ptr [edi + 0xbcd61c], dx'),(0x4ecd2e,'mov','word ptr [edi + 0xbcd61e], ax')]:exact(child,at,m,o)
for at,m,o in [(0x4ecb88,'mov','eax, dword ptr [0xd3b1c4]'),(0x4ecb8d,'mov','edx, dword ptr [0xd3b374]'),
               (0x4ecb93,'mov','dword ptr [edi + 0xbcd73c], 0xb'),(0x4ecb9d,'mov','dword ptr [eax*4 + 0xd3b1c8], edx'),
               (0x4ecbb9,'mov','ecx, dword ptr [0xd3b384]'),(0x4ecbbf,'mov','eax, dword ptr [0xd3b380]'),
               (0x4ecc90,'jl','0x4ecc61')]:exact(child,at,m,o)
for at,address in [(0x4ecb10,0xd3b384),(0x4ecb1d,0xd3b378),(0x4ecb2b,0xd3b380),(0x4ecb31,0xd3b364),
                   (0x4ecb44,0xd3b370),(0x4ecb49,0xd3b368),(0x4ecb4f,0xd3b36c),(0x4ecb60,0xd3b374),
                   (0x4ecc03,0xd3b384),(0x4ecc10,0xd3b378),(0x4ecc1e,0xd3b380),(0x4ecc23,0xd3b364),
                   (0x4ecc34,0xd3b36c),(0x4ecc46,0xd3b370),(0x4ecc4c,0xd3b374),(0x4ecc52,0xd3b368)]:
    assert child[at].mnemonic=='mov' and child[at].op_str.startswith('dword ptr ['+hex(address)+'], ')
table_raw,_=authenticated(0x5e4e80,96)
assert struct.unpack_from('<8i',table_raw)==(-228,-227,1,229,228,227,-1,-229)
assert struct.unpack_from('<16i',table_raw,32)==(0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0,-1,-1)
search_header=ROOT/'include/building_nearest_search.h';search_source=ROOT/'runtime/building_nearest_search.c'
search_signature='int32_t restore_building_nearest_search(int32_t x,int32_t y,int32_t type_count,const uint32_t *types,uint32_t mask,int32_t *output_x,int32_t *output_y,int32_t rings,int32_t excluded_actor)'
assert search_signature in search_header.read_text() and search_signature in search_source.read_text()

def c_function_span(raw,name):
    match=re.search(rb'(?m)^[^\r\n;]*\b'+name.encode()+rb'\([^\r\n]*\)\s*\r?\n?\s*\{',raw)
    assert match,('missing C function',name)
    opening=raw.index(b'{',match.start());depth=1;end=opening+1
    while depth:
        if raw[end:end+1]==b'{':depth+=1
        elif raw[end:end+1]==b'}':depth-=1
        end+=1
    if raw[end:end+2]==b'\r\n':end+=2
    elif raw[end:end+1]==b'\n':end+=1
    return match.start(),end
def compatibility_fragment(raw,kind,name):
    if kind=='function':
        start,end=c_function_span(raw,name);return raw[start:end]
    if kind=='rom':pattern=rb'(?m)^static const uint8_t '+name.encode()+rb'\[\]=\{[^\r\n]*\};(?:\r?\n|$)'
    elif kind=='prototype':pattern=rb'(?m)^void '+name.encode()+rb'\(struct restore_civic_frame \*\);(?:\r?\n|$)'
    else:pattern=rb'(?m)^    RAW\(004b5050\);(?:\r?\n|$)'
    match=re.search(pattern,raw);assert match,('missing RAW compatibility fragment',kind,name)
    return match[0]

# Preserve the whole legacy cue RAW implementation. It is deliberately not
# substituted or included in the new executable RAW comparison scope.
compat_path=DATA/'actor-animal-following-cue-raw-compatibility.json'
compat=json.loads(compat_path.read_text());assert compat['cue_raw_retained'] is True and compat['cue_raw_verified'] is False
for part in compat['fragments']:
    value=compatibility_fragment((ROOT/part['path']).read_bytes(),part['kind'],part['name'])
    assert len(value)==part['byte_count'] and hashlib.sha256(value).hexdigest()==part['sha256'],('changed retained cue RAW',part['name'])

# Recompute local cue liveness and the separate typed-five receipt from the
# portable production scripts. These children perform static analysis only.
cue_script=HERE/'audit_actor_animal_following_cue.py'
typed_script=HERE/'audit_actor_animal_following_cue_typed.py'
child_runs=[]
for script,extra in [(cue_script,['--scope','current-batch']),(typed_script,[])]:
    assert script.exists(),('missing portable static child',str(script))
    run=subprocess.run([sys.executable,str(script),*extra],capture_output=True,text=True)
    assert run.returncode==0,('static child failed',script.name,run.returncode,run.stdout[-2500:],run.stderr[-2500:])
    child_runs.append(dict(script=script.relative_to(ROOT).as_posix(),arguments=extra,exit_code=0,
                           source_sha256=hashlib.sha256(script.read_bytes()).hexdigest(),stdout_sha256=hashlib.sha256(run.stdout.encode()).hexdigest()))
cue_path=DATA/'actor-animal-following-cue-caller-audit.json'
typed_path=DATA/'actor-animal-following-cue-typed-contract.json'
cue=json.loads(cue_path.read_text());typed=json.loads(typed_path.read_text())
current=['0x49da20','0x49e692','0x49ea25','0x49f3dd'];supported=[*current,'0x49c50c']
assert cue['native_sha256']==NATIVE_SHA
assert cue['selected_scope']=='current-batch' and cue['selected_scope_passed'] is True
assert cue['current_batch_prior_cue_scratch_dead'] is True and cue['current_batch_callers']==current
assert cue['diagnostic_allow_open'] is False and cue['passed'] is False and cue['production_raw_adapter_ready'] is False
assert typed['schema_version']==1 and typed['passed'] is True and typed['local_typed_contract'] is True
assert typed['global_raw_drop'] is False and typed['production_raw_adapter_ready'] is False
assert typed['supported_typed_callers']==supported and typed['current_batch_scope_count']==4
for site in supported:assert not typed['caller_results'][site]['unknown'] and not typed['caller_results'][site]['unresolved']
assert typed['caller_results']['0x49c50c']['states_checked']==17
assert any(item['pc']=='0x49c55f' for item in typed['caller_results']['0x49c50c']['kill_addresses'])
for document in [cue,typed]:
    for name,sha in document['source_hashes'].items():assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==sha,('stale scoped cue dependency',name)
for contract in cue['additional_import_contracts']:
    assert hashlib.sha256(Path(contract['SDK_source']).read_bytes()).hexdigest()==contract['SDK_sha256']
cue_dependency=dict(path=cue_path.relative_to(ROOT).as_posix(),sha256=hashlib.sha256(cue_path.read_bytes()).hexdigest(),
                    selected_scope='current-batch',selected_scope_passed=True,global_passed=False,production_raw_adapter_ready=False)

production_paths=[ROOT/'runtime/actor_command.c',ROOT/'runtime/actor_civic.c',ROOT/'include/actor_command.h',ROOT/'include/actor_civic.h',ROOT/'runtime/actor_civic_model.c',ROOT/'runtime/actor_prey.c']
command_raw=production_paths[0].read_bytes();civic_raw=production_paths[1].read_bytes()
command_text=command_raw.decode();civic_text=civic_raw.decode()
header_text=production_paths[2].read_text();civic_header=production_paths[3].read_text();model_text=production_paths[4].read_text()
named_integrated=True
for size,count,name in specs.values():
    c_function_span(command_raw,name)
    assert re.search(r'\b'+re.escape(name)+r'\s*\(',header_text),('missing named prototype',name)
raw_entries=['0x49d0f0','0x49dc50','0x4ecd40','0x4eca70']
expected_raw=['case0x49d0f0u:f->r[0]=restore_actor_update_crocodile();break;',
              'case0x49dc50u:f->r[0]=restore_actor_update_hyena();break;',
              'case0x4ecd40u:f->r[0]=restore_actor_choose_crocodile_state((int32_t)A(1));break;',
              'case0x4eca70u:f->r[0]=restore_actor_enter_animal_state((int32_t)A(1),A(2));break;']
compact=re.sub(r'\s+','',civic_text)
for expected in expected_raw:assert expected in compact,('missing new RAW named route',expected)
for entry in raw_entries:
    token=entry[2:].zfill(8)
    assert 'restore_civic_C_'+token not in model_text and 'restore_civic_C_'+token not in civic_header
    assert 'RAW('+token+');' not in compact
canonical={'FUN_0049d0f0':'(void)restore_actor_update_crocodile();',
           'FUN_0049dc50':'(void)restore_actor_update_hyena();',
           'FUN_004ecd40':'(void)restore_actor_choose_crocodile_state(p0);',
           'FUN_004eca70':'(void)restore_actor_enter_animal_state(p0,(uint32_t)p1);',
           'FUN_004b5050':'(void)restore_actor_play_kind_cue(p0);'}
for name,expected in canonical.items():
    first,last=c_function_span(civic_raw,name);body=re.sub(rb'\s+',b'',civic_raw[first:last]).decode()
    assert expected in body and 'invoke(' not in body,('wrong canonical named route',name)

# Full runtime C-source inventory, including the separately justified existing
# prey caller. Declarations, definitions and the canonical adapter are not
# counted as native semantic callers; any additional source caller fails.
owner_specs={'runtime/actor_command.c':{'restore_actor_update_hippo':['0x49ea25','0x49f3dd'],
              'restore_actor_update_crocodile':['0x49da20'],'restore_actor_update_hyena':['0x49e692']},
             'runtime/actor_prey.c':{'FUN_0049bd40':['0x49c50c']}}
source_inventory=[];canonical_named_calls=0
for path in sorted((ROOT/'runtime').glob('*.c')):
    original=path.read_bytes()
    clean=re.sub(rb'//[^\r\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                 lambda m:re.sub(rb'[^\r\n]',b' ',m[0]),original,flags=re.S)
    relative=path.relative_to(ROOT).as_posix();owners={}
    for name,sites in owner_specs.get(relative,{}).items():owners[name]=(*c_function_span(original,name),sites)
    alias_span=c_function_span(original,'FUN_004b5050') if path==production_paths[1] else None
    found_by_owner={name:[] for name in owners}
    for match in re.finditer(rb'\b(FUN_004b5050|restore_actor_play_kind_cue)\s*\(',clean):
        line_start=clean.rfind(b'\n',0,match.start())+1;prefix=clean[line_start:match.start()]
        if re.fullmatch(rb'\s*(?:__declspec\([^)]*\)\s*)?(?:extern\s+)?void\s+',prefix):continue
        if alias_span and alias_span[0]<=match.start()<alias_span[1]:canonical_named_calls+=1;continue
        owner=next((name for name,(first,last,sites) in owners.items() if first<=match.start()<last),None)
        assert owner is not None,('unapproved ordinary cue caller',relative,original[:match.start()].count(b'\n')+1)
        found_by_owner[owner].append(match.start())
    for name,positions in found_by_owner.items():
        sites=owners[name][2];assert len(positions)==len(sites),('wrong source cue call count',relative,name,len(positions),sites)
        for position,site in zip(positions,sites):source_inventory.append(dict(source=relative,function=name,line=original[:position].count(b'\n')+1,native_site=site))
assert canonical_named_calls==1 and len(source_inventory)==5
assert sorted(item['native_site'] for item in source_inventory)==sorted(supported)

pending=[]
sources=[Path(__file__),catalogue_path,names_path,*evidence_paths,profiler_path,ROOT/'direct_c/native_test_pe.py',*production_paths,search_header,search_source,
         compat_path,cue_script,typed_script,cue_path,typed_path,DATA/'actor-animal-following-cue-local-rea-records.json']
proof=dict(passed=True,native_static_passed=True,production_ready=not pending,pending_gates=pending,native_sha256=NATIVE_SHA,
    owned_instruction_count=1922,functions={hex(a):dict(name=f['name'],instructions=len(f['by']),statically_reachable=len(f['by']),normal_returns=f['returns']) for a,f in functions.items()},
    switches=switches,analytically_unreachable_instructions=[hex(a) for a in sorted(dead)],
    analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(dead_edges)],outcome_qualifications=qualifications,
    native_stack_preconditions=['Valid32-bit cdecl DWORD arguments and ordinary private stack; normal child returns preserve nonvolatile GPRs/SP; DF=0.',
        'The original history index is unvalidated. Weight-sum qualification excludes indexed history writes corrupting this function private weight array or return/stack words. Native stack/code corruption remains unknown.'],
    callback_consumer_dependency=dict(path=profiler_path.relative_to(ROOT).as_posix(),sha256=hashlib.sha256(profiler_path.read_bytes()).hexdigest(),registered_callback_consumer=consumer),
    weighted_state_contract=dict(actor='fullDWORD argument, not active actor global or byte offset',child='restore_actor_enter_animal_state(actor,fullDWORD selected_state)',
        result='preserve full ordinary C initializer EAX without byte/BOOL/void normalization',callee_entry_scratch_independent_by=[hex(a) for a in sorted(child_terminals)],
        sole_observed_caller='0x49d828',caller_kills={'EAX':'0x49da5b','ECX':'0x49d82d','EDX':'0x49da5e','flags':'0x49d833'},child_owned_in_this_family=201),
    state_initializer_contract=dict(catalogue['state_initializer'],sole_observed_native_caller='0x4ece84',
        outgoing_scratch='201 forwards through99. The observed49D828 native consumer kills all result/scratch before use; full named/raw EAX remains preserved.',
        search_provider_sources={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in [search_header,search_source]}),
    cue_raw_gate=dict(status='retained; global replacement not authorized',dependency=cue_dependency,global_raw_drop=False),
    raw_scope=dict(entries=raw_entries),cue_raw_retained=True,cue_raw_verified=False,
    typed_cue_contract=dict(path=typed_path.relative_to(ROOT).as_posix(),sha256=hashlib.sha256(typed_path.read_bytes()).hexdigest(),
        local_typed_contract=True,current_batch_scope_count=4,supported_typed_callers=supported,source_inventory=source_inventory),
    retained_raw_cue_source_fragments=compat['fragments'],static_child_runs=child_runs,
    callback_return_contracts=catalogue['callback_return_contracts'],
    names=dict(established=names['established_names'],asset_hashes=asset_hashes,source=names_path.relative_to(ROOT).as_posix()),
    source_hashes={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},
    limitations=['Static byte authority, bounded CFG, mathematical outcome qualifications and caller liveness are separate from finite executable equivalence.',
        'No executable candidate corpus is rerun by this audit. Unknown indirect/runtime-generated callers and arbitrary corruption remain unknown.',
        'A native scratch kill inside a later callee proves independence from an earlier result, not correctness of that later C provider. Actual current-C child composition remains a production verifier obligation.',
        'The existing ordinary C search provider and shared profiler are dependencies without new instruction ownership.'])
(DATA/'actor-animal-following-caller-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS native static1922 instructions;15 proved dead instructions;6 impossible outcomes; production gates:',','.join(pending) or 'none')
if options.require_production:assert proof['production_ready'],('production gates pending',pending)
