"""Read-only triage of reconstructed C; findings are leads, never patch approval."""
from pathlib import Path
import argparse,bisect,collections,datetime,functools,hashlib,json,re,struct,sys
ROOT=Path(__file__).resolve().parent.parent
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,default=ROOT/'metadata/reconstruction-pattern-audit.json')
args=parser.parse_args();sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_MEM,CS_OP_REG,CS_AC_READ,CS_AC_WRITE
sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(image)==PIN
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);sections=[]
for k in range(u16(pe+6)):
    q=table+k*40;sections.append((base+u32(q+12),u32(q+8),u32(q+20),u32(q+16),bool(u32(q+36)&0x20000000)))
def pe_bytes(a,n):
    for va,vs,raw,rs,x in sections:
        if va<=a and a+n<=va+rs:return image[raw+a-va:raw+a-va+n]
    return None
def code_address(a):return any(x and va<=a<va+vs for va,vs,raw,rs,x in sections)
def addr(value):
    if isinstance(value,int):return value
    if isinstance(value,str) and re.fullmatch(r'(?:0x)?[0-9a-fA-F]{6,8}',value):return int(value,16)
    return None
def pinned(obj):
    if isinstance(obj,dict):return any(v==PIN for v in obj.values() if isinstance(v,str)) or any(pinned(v) for k,v in obj.items() if k in ['program','target','binary','provenance'])
    return False
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
native=collections.defaultdict(list);native_inputs={};rejected=[];exports=0;authenticated=0
def walk_functions(obj,path,parent_address=None):
    global exports,authenticated
    if isinstance(obj,list):
        for item in obj:walk_functions(item,path,parent_address)
        return
    if not isinstance(obj,dict):return
    local=parent_address
    for key in ['requested_address','entry','function_address','address']:
        candidate=addr(obj.get(key))
        if candidate is not None:local=candidate;break
    for key in ['info','function']:
        if isinstance(obj.get(key),dict):local=addr(obj[key].get('address')) or local
    assembly=obj.get('assembly') or obj.get('disassembly')
    if assembly is None and isinstance(obj.get('items'),list) and obj['items'] and isinstance(obj['items'][0],dict) and 'instruction' in obj['items'][0]:assembly=obj
    if assembly is None and isinstance(obj.get('instructions'),list):assembly={'items':obj['instructions']}
    if isinstance(assembly,dict) and local is not None:
        items=assembly.get('items',[])
        if assembly.get('has_more') or obj.get('has_more'):
            rejected.append({'path':path,'function':f'{local:08x}','reason':'paginated/incomplete export'});return
        parsed=[]
        for item in items:
            if not isinstance(item,dict) or not item.get('bytes') or addr(item.get('address')) is None:continue
            try:b=bytes.fromhex(item['bytes'])
            except ValueError:continue
            a=addr(item['address'])
            if pe_bytes(a,len(b))!=b:
                rejected.append({'path':path,'function':f'{local:08x}','instruction':f'{a:08x}','reason':'instruction bytes do not match pinned PE'});return
            decoded=list(md.disasm(b,a))
            if len(decoded)!=1 or decoded[0].size!=len(b):continue
            ins=decoded[0];mem=[]
            for op in ins.operands:
                if op.type==CS_OP_MEM:
                    mem.append({'address':op.mem.disp&0xffffffff,'width':op.size,'indexed':bool(op.mem.index or op.mem.base),'scale':op.mem.scale,'read':bool(op.access&CS_AC_READ),'write':bool(op.access&CS_AC_WRITE)})
            partial=bool(ins.operands and ins.operands[0].type==CS_OP_REG and ins.reg_name(ins.operands[0].reg)=='al' and ins.operands[0].access&CS_AC_WRITE)
            parsed.append({'address':f'{a:08x}','bytes':b.hex(),'instruction':ins.mnemonic+' '+ins.op_str,'memory':mem,'writes_AL':partial})
        if parsed:
            info=obj.get('info') or obj.get('function') or {}
            if not isinstance(info,dict):info={}
            boundary=addr(info.get('body_start'))==local and addr(info.get('body_end')) is not None
            # A descriptor is retained as declared Ghidra scope, never inferred
            # by linearly decoding bytes beyond a known instruction list.
            record={'export':path,'function':f'{local:08x}','instructions':parsed,'declared_function_bounds':boundary,'body_start':info.get('body_start'),'body_end':info.get('body_end'),'instruction_count':len(parsed)}
            native[local].append(record);exports+=1;authenticated+=len(parsed)
        return
    for key,value in obj.items():
        child=addr(key)
        if isinstance(value,(dict,list)):walk_functions(value,path,child if child is not None else local)
metadata=ROOT/'metadata';cache=metadata/'native-function-audit-cache'
paths=sorted(set(metadata.glob('*.json')) | (set(cache.rglob('*.json')) if cache.exists() else set()))
for path in paths:
    if path.resolve()==args.output.resolve():continue
    try:data=json.loads(path.read_text(encoding='utf-8-sig'))
    except (ValueError,UnicodeError,OSError):continue
    if not pinned(data):continue
    previous=exports;walk_functions(data,str(path.relative_to(ROOT)).replace('\\','/'))
    if exports>previous:native_inputs[str(path.relative_to(ROOT)).replace('\\','/')]=sha(path.read_bytes())
cache_index_path=cache/'index.json';cache_index={}
if cache_index_path.exists():
    cache_index=json.loads(cache_index_path.read_text(encoding='utf-8-sig'));assert cache_index.get('native_sha256')==PIN
    native_inputs[str(cache_index_path.relative_to(ROOT)).replace('\\','/')]=sha(cache_index_path.read_bytes())
units=json.loads((ROOT/'units.json').read_text());assert len(units)==4402
type_width={'undefined':1,'undefined1':1,'byte':1,'char':1,'uchar':1,'bool':1,'uint8_t':1,'int8_t':1,'unsigned char':1,'signed char':1,'undefined2':2,'short':2,'ushort':2,'word':2,'wchar16':2,'uint16_t':2,'int16_t':2,'unsigned short':2,'undefined4':4,'int':4,'uint':4,'long':4,'ulong':4,'dword':4,'int32_t':4,'uint32_t':4,'unsigned int':4,'signed int':4,'pointer32':4,'float':4,'undefined8':8,'longlong':8,'ulonglong':8,'double':8,'int64_t':8,'uint64_t':8,'void':0}
def width(t):
    t=re.sub(r'\b(const|volatile|__cdecl|__stdcall|static|extern|inline|__inline)\b','',t);t=' '.join(t.split())
    return 4 if '*' in t else type_width.get(t)
token_re=re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+[uUlL]*|\d+[uUlL]*|==|!=|<=|>=|&&|\|\||<<|>>|\+\+|--|->|\+=|-=|\*=|/=|&=|\|=|\^=|\.\.\.|[^\s]')
macro_re=re.compile(r'^\s*#define\s+([A-Za-z_]\w*)\s+\(\*\(\s*(.*?)\s*\*\s*\)\s*(0x[0-9a-fA-F]+)[uUlL]*\s*\)',re.M)
findings=[];exclusions=[];file_summaries=[];binding_counts=collections.Counter();decls=collections.defaultdict(list);definitions={};calls=[]
def add(kind,file,line,context,confidence,**kw):
    findings.append({'kind':kind,'file':file,'line':line,'context':context.strip()[:360],'confidence':confidence,'function_address':current_address,**kw})
@functools.lru_cache(maxsize=1)
def source_lines(source):return source.splitlines()
def excerpt(source,line):
    lines=source_lines(source)
    return lines[line-1] if line and line<=len(lines) else ''
def family(source):
    result=[]
    for n,label in [(0x108,'building_264'),(0x184,'actor_388'),(0x49d8,'profile_18904')]:
        if re.search(r'\*\s*(?:0x0*'+f'{n:x}'+r'|'+str(n)+r')\b',source,re.I):result.append(label)
    vtable=re.search(r'\*in_ECX\s*=\s*&?(PTR_\w+)',source)
    return result,('vtable:'+vtable[1] if vtable else 'ECX_receiver_unclassified')
for unit in units:
    file=unit['file'];path=ROOT/file;source=path.read_text(encoding='utf-8-sig');newlines=[-1]+[m.start() for m in re.finditer('\n',source)]
    line_of=lambda pos:bisect.bisect_left(newlines,pos)
    current_address=f"{int(unit['name'].split('_')[-1],16):08x}" if re.fullmatch(r'FUN_[0-9a-fA-F]+',unit['name']) else unit['provenance'].get('address')
    source_address=int(current_address,16);strides,receiver=family(source)
    clean=re.sub(r'^\s*#.*$',lambda m:' '*len(m[0]),source,flags=re.M)
    tokens=[(m[0],m.start(),m.end()) for m in token_re.finditer(clean) if not m[0].startswith(('/*','//','"',"'"))]
    names=[x[0] for x in tokens];positions=collections.defaultdict(list)
    for i,t in enumerate(tokens):positions[t[0]].append(i)
    start_count=len(findings);bindings=[];code_macro_uses=[];table_macro_uses=[]
    def address_of_use(i):
        if not i or names[i-1]!='&':return False
        before=names[i-2] if i>1 else ''
        if not before or before in ['(',',','=','+','-','*','return',':','?','[',';','sizeof']:return True
        if before==')':
            j=i-3;depth=1
            while j>=0:
                if names[j]==')':depth+=1
                elif names[j]=='(':
                    depth-=1
                    if depth==0:break
                j-=1
            inside=names[j+1:i-2] if j>=0 else []
            return bool(inside) and all(t in ['*','const','volatile','signed','unsigned','struct','void','int','char','short','long','float','double','byte','uint','ushort','dword','word'] or t.startswith('undefined') or t.endswith('_t') for t in inside)
        return False
    evidence=[]
    for export in native.get(source_address,[]):
        evidence.extend((export,ins,mem) for ins in export['instructions'] for mem in ins['memory'])
    for match in macro_re.finditer(source):
        name,t,a=match.groups();a=int(a,16);w=width(t);uses=[];classes=collections.Counter()
        for i in positions.get(name,[]):
            prev=names[i-1] if i else '';before=names[i-2] if i>1 else ''
            address_only=address_of_use(i)
            projection=any(names[j].startswith('RESTORE_SLICE') for j in range(max(0,i-3),i))
            indexed_value=address_only and i+2<len(names) and names[i+1]==')' and names[i+2]=='['
            cls='indexed_value' if indexed_value else 'address_only' if address_only else 'projection_address' if projection else 'scalar_value'
            classes[cls]+=1
            if sum(u['class']==cls for u in uses)<3:uses.append({'line':line_of(tokens[i][1]),'class':cls,'context':excerpt(source,line_of(tokens[i][1])).strip()[:360]})
        binding_counts['all_definitions']+=1
        binding_counts['unused_definitions' if not classes else 'used_definitions']+=1
        if not classes:continue
        if code_address(a):code_macro_uses.append({'macro':name,'address':f'{a:08x}','actual_use_counts':dict(classes),'uses':uses,'class':'native_code_address_macro_identity' if not classes['scalar_value'] else 'native_code_memory_macro_value'})
        elif name.startswith('PTR_FUN_'):table_macro_uses.append({'macro':name,'address':f'{a:08x}','actual_use_counts':dict(classes),'uses':uses,'class':'original_callback_table_data_symbol'})
        binding_counts['definitions_with_scalar_value_uses']+=int(classes['scalar_value']>0)
        binding_counts['address_only_definitions']+=int(not classes['scalar_value'] and not classes['projection_address'] and not classes['indexed_value'])
        binding_counts['scalar_value_occurrences']+=classes['scalar_value'];binding_counts['address_only_occurrences']+=classes['address_only'];binding_counts['projection_occurrences']+=classes['projection_address'];binding_counts['indexed_value_occurrences']+=classes['indexed_value']
        direct=[(e,ins,m) for e,ins,m in evidence if m['address']==a and not m['indexed'] and (m['read'] or m['write'])]
        native_widths=sorted({m['width'] for e,ins,m in direct});examples=[];seen=set()
        for e,ins,m in direct:
            key=(ins['address'],m['width'])
            if key in seen:continue
            seen.add(key);examples.append({'export':e['export'],'address':ins['address'],'instruction':ins['instruction'],'width':m['width'],'authenticated':True})
        if w==1 and classes['scalar_value'] and any(n in [2,4] for n in native_widths):
            mixed=1 in native_widths
            add('scalar_byte_native_wider',file,next(u['line'] for u in uses if u['class']=='scalar_value'),next(u['context'] for u in uses if u['class']=='scalar_value'),'native_backed_candidate' if not mixed else 'native_backed_mixed_width_review',global_name=name,global_address=f'{a:08x}',binding_type=t,binding_width=w,actual_use_counts=dict(classes),uses=uses,native_widths=native_widths,evidence=examples[:8],proof_limit='Same function/global native reads are wider; source-to-instruction and mixed-path matching still need review.',record_stride_family=strides,receiver_family=receiver)
        elif w==1 and classes['scalar_value'] and not direct:
            add('scalar_byte_without_native_width_evidence',file,next(u['line'] for u in uses if u['class']=='scalar_value'),next(u['context'] for u in uses if u['class']=='scalar_value'),'heuristic_only',global_name=name,global_address=f'{a:08x}',binding_type=t,actual_use_counts=dict(classes),uses=uses,record_stride_family=strides,receiver_family=receiver)
        indexed_native=[(e,ins,m) for e,ins,m in evidence if m['address']==a and m['indexed'] and (m['read'] or m['write'])]
        indexed_widths=sorted({m['width'] for e,ins,m in indexed_native})
        if classes['indexed_value'] and w in [1,2,4] and any(n!=w for n in indexed_widths):
            sample=next(u for u in uses if u['class']=='indexed_value')
            add('indexed_base_native_width_mismatch',file,sample['line'],sample['context'],'native_indexed_lead',global_name=name,global_address=f'{a:08x}',binding_type=t,binding_width=w,native_widths=indexed_widths,uses=uses,evidence=[{'export':e['export'],'address':ins['address'],'instruction':ins['instruction'],'memory_width':m['width']} for e,ins,m in indexed_native][:5],proof_limit='(&base)[index] dereferences its declared element type; compare index expression and explicit outer conversions to native before changing width/stride.',record_stride_family=strides,receiver_family=receiver)
        if (classes['address_only'] or classes['indexed_value']) and w and w>1:
            scaled=[u for u in uses if u['class'] in ['address_only','indexed_value'] and (re.search(r'&\s*'+re.escape(name)+r'\s*\+[^;)\n]*\*\s*(?:0x[0-9a-f]+|\d+)',u['context'],re.I) or re.search(r'&\s*'+re.escape(name)+r'\s*\)\s*\[[^\]\n]*\*',u['context']))]
            indexed=[{'export':e['export'],'address':ins['address'],'instruction':ins['instruction'],'native_index_scale':m['scale'],'memory_width':m['width']} for e,ins,m in evidence if m['address']==a and m['indexed']]
            if scaled:add('scaled_address_of_array_base',file,scaled[0]['line'],scaled[0]['context'],'native_indexed_lead' if indexed else 'heuristic_only',global_name=name,global_address=f'{a:08x}',binding_type=t,pointer_element_width=w,uses=scaled,actual_use_counts=dict(classes),evidence=indexed[:5],proof_limit='C address-of arithmetic scales by element width; native register expression equivalence must be checked before repair.',record_stride_family=strides,receiver_family=receiver)
        if w==1 and classes['address_only'] and not classes['scalar_value'] and not classes['projection_address'] and not classes['indexed_value']:
            exclusions.append({'file':file,'function_address':current_address,'global_name':name,'global_address':f'{a:08x}','binding_type':t,'reason':'Byte binding is only an address base, not a scalar byte load. Explicit outer casts/index scale define access width. Do not widen the macro automatically.','actual_use_counts':dict(classes),'uses':uses})
        bindings.append({'name':name,'address':f'{a:08x}','type':t,'counts':dict(classes)})
    # Local register variables: a dereferenced store still reads the pointer.
    # Only a direct assignment to the variable counts as initialization.
    for reg in ['in_ECX','in_EAX']:
        ix=positions.get(reg,[])
        if not ix:continue
        declarations=[];assignments=[];reads_reg=[];escapes=[]
        for i in ix:
            line=line_of(tokens[i][1]);text=excerpt(source,line);prev=names[i-1] if i else '';nxt=names[i+1] if i+1<len(names) else ''
            declared=bool(re.match(r'^\s*(?:[A-Za-z_]\w*\s+)+(?:\*\s*)*'+reg+r'\s*(?:;|=)',text))
            if declared:declarations.append({'line':line,'initialized':nxt=='='})
            direct=nxt=='=' and prev not in ['*','->','.']
            if direct:assignments.append(line)
            if prev=='&':escapes.append(line)
            if not declared and not direct:reads_reg.append(line)
        if declarations and reads_reg and not any(d['initialized'] for d in declarations):
            no_assignment=not assignments and not escapes and '__asm' not in clean
            first=min(reads_reg)
            if no_assignment or not assignments or first<min(assignments):
                add('uninitialized_register_input' if no_assignment else 'register_read_before_syntactic_assignment',file,first,excerpt(source,first),'source_definite_uninitialized_local' if no_assignment else 'heuristic_control_flow_review',register=reg,declarations=declarations,assignment_lines=assignments,address_escape_lines=escapes,read_lines=sorted(set(reads_reg))[:12],proof_limit='No local initialization/assignment exists in this C function.' if no_assignment else 'Token order is not a control-flow proof; address escapes/branches require review.',record_stride_family=strides,receiver_family=receiver)
    # Balanced call/prototype argument lists, excluding comments and strings.
    for i,name in enumerate(names):
        if not re.fullmatch(r'FUN_[0-9a-fA-F]{8}',name) or i+1>=len(names) or names[i+1]!='(':continue
        depth=0;j=i+1;commas=0
        while j<len(names):
            if names[j]=='(':depth+=1
            elif names[j]==')':
                depth-=1
                if depth==0:break
            elif names[j]==',' and depth==1:commas+=1
            j+=1
        if j==len(names):continue
        inside=names[i+2:j];arity=0 if not inside or inside==['void'] else commas+1
        begin=i-1
        while begin>=0 and names[begin] not in [';','{','}']:begin-=1
        prefix=' '.join(names[begin+1:i]);next_token=names[j+1] if j+1<len(names) else ''
        definition=next_token=='{' and not any(x in prefix.split() for x in ['if','while','switch','return','='])
        prototype='extern' in prefix.split() and next_token==';'
        record={'file':file,'line':line_of(tokens[i][1]),'function_address':current_address,'name':name,'arity':arity,'arguments':' '.join(inside)[:300],'context':excerpt(source,line_of(tokens[i][1])).strip()[:360]}
        if definition or prototype:
            record.update({'return_type':prefix,'return_width':width(prefix),'unspecified_arguments':not inside,'variadic':'...' in inside,'definition':definition})
            decls[name].append(record)
            if definition:definitions[name]=record
        else:calls.append(record)
    partial=[]
    for e in native.get(source_address,[]):
        if any(ins['writes_AL'] for ins in e['instructions']):partial.append({'export':e['export'],'declared_function_bounds':e['declared_function_bounds'],'instructions':[ins for ins in e['instructions'] if ins['writes_AL']][:5]})
    if partial:
        canonical=definitions.get(unit['name'])
        if canonical and canonical['return_width'] in [2,4,8]:
            add('partial_AL_vs_full_C_return',file,canonical['line'],canonical['context'],'native_backed_ABI_review',C_return_type=canonical['return_type'],evidence=partial[:3],proof_limit='AL writes alone do not prove noncanonical EAX at every return. Audit complete dataflow and every caller before changing the interface.',record_stride_family=strides,receiver_family=receiver)
    # Code-address tokens are not all calls: switch labels and callback identity
    # values are recorded separately from typed executable-pointer uses.
    token_classes=collections.Counter();examples=[]
    for i,value in enumerate(names):
        if not re.fullmatch(r'0x[0-9a-fA-F]+[uUlL]*',value):continue
        a=int(value.rstrip('uUlL'),16)
        if not code_address(a):continue
        line=line_of(tokens[i][1]);context=excerpt(source,line)
        cls='dispatch_case_token' if i and names[i-1]=='case' else 'explicit_function_pointer_context' if re.search(r'\b(code|__cdecl|__stdcall)\b.*\*',context) else 'code_address_literal_identity_or_lookup'
        token_classes[cls]+=1
        if len(examples)<8:examples.append({'line':line,'address':f'{a:08x}','class':cls,'context':context.strip()[:360]})
    callback_symbols=[]
    for name,ix in positions.items():
        if re.fullmatch(r'FUN_[0-9a-fA-F]{8}',name):
            for i in ix:
                if i+1<len(names) and names[i+1]=='(':continue
                callback_symbols.append({'line':line_of(tokens[i][1]),'symbol':name,'context':excerpt(source,line_of(tokens[i][1])).strip()[:360]})
    if token_classes or callback_symbols or code_macro_uses or table_macro_uses:
        first=(examples or callback_symbols or [u for m in code_macro_uses+table_macro_uses for u in m['uses']])[0]
        add('original_callback_or_code_token_use',file,first['line'],first['context'],'classification_only',token_class_counts=dict(token_classes),literal_examples=examples,native_code_macro_uses=code_macro_uses,callback_table_data_macros=table_macro_uses,C_function_symbol_value_uses=callback_symbols[:12],C_function_symbol_value_use_count=len(callback_symbols),proof_limit='Code identity constants, callback-table data addresses, dispatch cases and C callback pointers are different. These tokens alone do not prove execution of original code.',record_stride_family=strides,receiver_family=receiver)
    file_summaries.append({'file':file,'function_address':current_address,'sha256':sha(path.read_bytes()),'findings':len(findings)-start_count,'used_macro_bindings':len(bindings),'record_stride_family':strides,'receiver_family':receiver,'native_export_count':len(native.get(source_address,[]))})
for call in calls:
    definition=definitions.get(call['name'])
    if definition and not definition['variadic'] and call['arity']<definition['arity']:
        current_address=call['function_address']
        add('omitted_C_call_arguments',call['file'],call['line'],call['context'],'source_contract_conflict',callee=call['name'],supplied_arguments=call['arity'],defined_arguments=definition['arity'],definition=definition,proof_limit='Proves a current C declaration/call discrepancy, not the correct native calling convention or argument count.')
prototype_conflicts=[]
for name,records in decls.items():
    widths={r['return_width'] for r in records if r['return_width'] is not None};arities={r['arity'] for r in records if not r['unspecified_arguments'] and not r['variadic']}
    if len(widths)>1 or len(arities)>1:
        prototype_conflicts.append({'callee':name,'kind':'prototype_contract_conflict','confidence':'source_contract_conflict','return_widths':sorted(widths),'explicit_argument_counts':sorted(arities),'declarations':records,'proof_limit':'Unspecified C argument lists are excluded from arity conflicts. ABI adapters and discarded returns may be intentional; compare complete native callers before a patch.'})
groups={}
for label,keyfun in [('shared_global',lambda f:f.get('global_address')),('record_stride',lambda f:','.join(f.get('record_stride_family',[])) or None),('receiver_family',lambda f:f.get('receiver_family') if f['kind'].startswith(('uninitialized_register','register_read')) else None)]:
    collected=collections.defaultdict(list)
    for index,f in enumerate(findings):
        key=keyfun(f)
        if key:collected[key].append(index)
    groups[label]=[{'key':key,'finding_count':len(ix),'files':len({findings[i]['file'] for i in ix}),'kinds':dict(collections.Counter(findings[i]['kind'] for i in ix)),'finding_indices':ix} for key,ix in sorted(collected.items(),key=lambda kv:(-len(kv[1]),kv[0]))]
rank={'native_backed_candidate':0,'source_definite_uninitialized_local':1,'source_contract_conflict':2,'native_backed_mixed_width_review':3,'native_backed_ABI_review':4,'native_indexed_lead':5,'heuristic_control_flow_review':6,'heuristic_only':7,'classification_only':8}
ranked=sorted(range(len(findings)),key=lambda i:(rank.get(findings[i]['confidence'],9),findings[i]['file'],findings[i]['line']))
exemplar=[e for e in exclusions if e['file'] in ['src/unit_01680.c','src/unit_01681.c']]
assert len(exemplar)==3 and all(e['actual_use_counts'].get('scalar_value',0)==0 for e in exemplar),'address-only false-positive exclusion regression'
report={'format':1,'generated_at':datetime.datetime.now(datetime.timezone.utc).isoformat(),'native_sha256':PIN,'read_only_audit':True,'C_sources_modified':False,'original_game_executed':False,'automatic_patch_safe':False,'all_functions_verified':False,'units_scanned':len(units),'summary':{'findings':len(findings),'by_kind':dict(collections.Counter(f['kind'] for f in findings)),'by_confidence':dict(collections.Counter(f['confidence'] for f in findings)),'bindings':dict(binding_counts),'prototype_conflict_families':len(prototype_conflicts),'calls_examined':len(calls),'definitions_parsed':len(definitions),'native_functions_with_evidence':len(native),'native_exports':exports,'authenticated_instruction_records':authenticated,'rejected_exports':len(rejected),'address_only_exclusions':len(exclusions)},'method':['Tokenized C use sites exclude comments, strings, and macro definitions. Unused macros and address-only/projection uses are not scalar-byte findings.','Native records come only from function-associated exported instruction lists in pinned dossiers/cache. Every instruction byte is authenticated against the PE; no linear disassembly over gaps creates a function.','Matching source/global width and native read width is candidate evidence, not proof of exact source-to-instruction equivalence or mechanical patch safety.','Register-variable scans distinguish dereference stores (pointer reads), direct assignment, initializer and address escape. Source-definite missing initialization is separate from control-flow heuristic leads.','C prototypes/calls are balanced-token parsed. Unspecified argument lists do not imply zero native arguments.','Partial AL writes require whole native return dataflow and caller-consumer audits. Original code identity tokens do not by themselves indicate original execution.'],'limitations':['This is a static triage tool, not a C AST/CFG verifier or function acceptance suite. Conditional initialization, macros, adapters and noncanonical native ABIs need manual/native review.','Absence of native evidence is reported explicitly and never filled by disassembling until the next guessed entry.','Counts describe the source hashes captured here; concurrent source changes require rerunning the audit.'],'native_exports':native_inputs,'source_hashes':{f['file']:f['sha256'] for f in file_summaries},'cache_directory_present':cache.exists(),'cache_json_files':len(list(cache.rglob('*.json'))) if cache.exists() else 0,'files':file_summaries,'findings':findings,'ranked_finding_indices':ranked,'prototype_conflicts':prototype_conflicts,'groups':groups,'false_positive_exclusion_exemplars':exemplar,'address_only_exclusions':exclusions,'rejected_exports':rejected}
report['source_hashes']['tools/audit_reconstruction_patterns.py']=sha(Path(__file__).read_bytes())
report['source_hashes']['units.json']=sha((ROOT/'units.json').read_bytes())
report['bulk_cache']={'index':str(cache_index_path.relative_to(ROOT)).replace('\\','/'),'completed':cache_index.get('completed',False),'requested':cache_index.get('requested'),'exact_functions_declared_by_index':cache_index.get('exact_functions'),'batches_declared_by_index':len(cache_index.get('batches',[])),'batch_hashes':{p:native_inputs[p] for p in cache_index.get('batches',[]) if p in native_inputs},'missing_batch_ingestion':[p for p in cache_index.get('batches',[]) if p not in native_inputs],'native_catalogue_closed':False}
report['automatic_patch_policy']={'eligible_findings':0,'all_findings_require_review':True,'excluded_from_mechanical_eligibility':['mixed native BYTE/WORD/DWORD access','floating point or bit-cast aliases','signedness not established from complete native control flow','address-only byte bases and explicit casts','indexed expressions without matched effective-address dataflow','noncanonical register/stack ABI and partial-register returns'],'reason':'Regex/token classification plus same-global native instructions is triage evidence, never sufficient patch authorization or function verification.'}
report['source_drift_at_completion']=[name for name,want in report['source_hashes'].items() if sha((ROOT/name).read_bytes())!=want]
report['native_export_drift_at_completion']=[name for name,want in native_inputs.items() if sha((ROOT/name).read_bytes())!=want]
report['proposed_first_batch']={'applied':False,'functions':[{'address':a,'file':f'src/unit_{n:05}.c','source_sha256':report['source_hashes'][f'src/unit_{n:05}.c']} for a,n in [('0040fa40',123),('0040fa70',124),('0040fae0',126)]],'shared_actual_scalar_globals':['00e38d94','00e92e18'],'proposal':'Review/reconstruct three complete WORD framebuffer leaves (store, AND, OR) with full-DWORD pitch and pointer loads; six repeated binding repairs. Do not apply this proposal automatically.','excluded_neighbor':'0040faa0/unit00125 carries EDX:EAX and is outside this simple void-return batch.','required_proof':['Complete function/argument/return review from authenticated cache bodies','Debug and /O2 C versus complete native instructions','Full pointer/pitch values above255, WORD color masks, row padding, surrounding canaries and no writes outside selected WORD','cdecl stack and saved registers; no invented clipping','Explicit valid mapped-buffer coordinates and signedness/overflow-domain review']}
args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'output':str(args.output),'units_scanned':len(units),**report['summary']},indent=2))
