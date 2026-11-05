"""Classify the saved native catalogue and emit proven i386 field-access C rules.

Default: read-only inventory. Candidates are written separately; installation,
strict compilation and native/C comparison remain explicit batch stages.
No original or restored host executable is launched.
"""
from __future__ import annotations
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
DEF = re.compile(r'\b(FUN_([0-9a-fA-F]{8}))\s*\([^;{}]*\)\s*\{')
REGS = {'eax':28,'ecx':24,'edx':20,'ebx':16,'ebp':8,'esi':4,'edi':0}
ALIASES = {a:r for r,aa in {'eax':['eax','ax','al'],'ecx':['ecx','cx','cl'],
 'edx':['edx','dx','dl'],'ebx':['ebx','bx','bl'],'ebp':['ebp','bp'],
 'esi':['esi','si'],'edi':['edi','di']}.items() for a in aa}

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes((json.dumps(value, indent=2)+'\n').encode())

def defined_symbols(path):
    """Read actual defined function symbols, including /bigobj COFF."""
    b=path.read_bytes();big=b[:4]==b'\0\0\xff\xff'
    if big: off,n=struct.unpack_from('<II',b,48);step=20
    else:
        if b[:2]!=b'\x4c\x01': return set()
        off,n=struct.unpack_from('<II',b,8);step=18
    strings=off+n*step;names=set();i=0
    while i<n:
        p=off+i*step;raw=b[p:p+8]
        if raw[:4]==b'\0'*4:
            q=strings+struct.unpack_from('<I',raw,4)[0]
            name=b[q:b.index(0,q)].decode('ascii')
        else: name=raw.rstrip(b'\0').decode('ascii')
        section=struct.unpack_from('<i' if big else '<h',b,p+12)[0]
        kind=struct.unpack_from('<H',b,p+(16 if big else 14))[0]
        if section>0 and kind&0x20: names.add(name)
        i+=1+b[p+step-1]
    return names

def catalogue():
    result={};conflicts=set()
    for path in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
        d=json.loads(path.read_bytes())
        if d['native_sha256']!=PIN: raise ValueError('Wrong native cache: '+str(path))
        for e in d['entries']:
            a=int(e['requested_address'],16)
            if 'assembly' not in e or 'items' not in e['assembly']: continue
            if a in result and result[a]['assembly']['items']!=e['assembly']['items']:
                conflicts.add(a)
            else: result[a]=e
    return result,conflicts

def native_bytes(entry):
    items=entry['assembly']['items']
    if not items or entry['assembly'].get('has_more'): return None
    a=int(entry['requested_address'],16);out=bytearray()
    for item in items:
        if int(item['address'],16)!=a+len(out): return None
        out.extend(bytes.fromhex(item['bytes']))
    if len(out)!=entry['info']['body_size']: return None
    return bytes(out)

def rule(ins):
    """Whole-EAX MOV/LEA rules and zeroed-EAX byte lookups; no calls/branches."""
    from capstone.x86_const import X86_OP_REG,X86_OP_IMM,X86_OP_MEM
    core=list(ins)
    if len(core)>=4 and [(x.mnemonic,x.op_str) for x in core[:2]]==[('push','ebp'),('mov','ebp, esp')]:
        if (core[-2].mnemonic,core[-2].op_str)!=('pop','ebp'): return None
        core=core[2:-2]+[core[-1]]
    if len(core)==4 and core[-1].mnemonic=='ret' and not core[-1].op_str:
        first,a,b=core[:3]
        if (first.mnemonic,first.op_str)==('xor','eax, eax') and a.mnemonic==b.mnemonic=='mov':
            aa,bb=a.operands,b.operands
            if (len(aa)==len(bb)==2 and aa[0].type==bb[0].type==X86_OP_REG and
                a.reg_name(aa[0].reg)==b.reg_name(bb[0].reg)=='al' and
                aa[1].type==bb[1].type==X86_OP_MEM and aa[1].size==bb[1].size==1 and
                a.reg_name(aa[1].mem.base)=='ecx' and b.reg_name(bb[1].mem.base)=='eax' and
                not(aa[1].mem.index or bb[1].mem.index or aa[1].mem.segment or bb[1].mem.segment)):
                return dict(kind='lookup',result='uint32_t',offset=aa[1].mem.disp,width=1,
                    table_base=bb[1].mem.disp&0xffffffff,native_flags='xor_eax_eax')
    if len(core)!=2 or core[-1].mnemonic!='ret' or core[-1].op_str: return None
    op=core[0];args=op.operands
    if len(args)!=2: return None
    def field(v):
        return v.type==X86_OP_MEM and op.reg_name(v.mem.base)=='ecx' and not v.mem.index and not v.mem.segment
    if op.mnemonic=='mov' and args[0].type==X86_OP_REG and op.reg_name(args[0].reg)=='eax' and args[1].type==X86_OP_REG and op.reg_name(args[1].reg)=='ecx':
        return dict(kind='self',result='uint32_t',offset=0)
    if args[0].type==X86_OP_REG and op.reg_name(args[0].reg)=='eax' and field(args[1]):
        if op.mnemonic=='lea': return dict(kind='offset',result='uint32_t',offset=args[1].mem.disp)
        if op.mnemonic not in ('mov','movzx','movsx'): return None
        width=args[1].size
        if width not in (1,2,4) or (op.mnemonic=='mov' and width!=4): return None
        return dict(kind='read',result='uint32_t',offset=args[1].mem.disp,width=width,signed=(op.mnemonic=='movsx'))
    if op.mnemonic=='mov' and field(args[0]) and args[0].size in (1,2,4):
        r=dict(kind='write',result='void',offset=args[0].mem.disp,width=args[0].size)
        if args[1].type==X86_OP_IMM:
            r['constant']=args[1].imm&((1<<(8*r['width']))-1)
        elif args[1].type==X86_OP_REG and op.reg_name(args[1].reg) in ALIASES:
            r['value_register']=ALIASES[op.reg_name(args[1].reg)]
        else: return None
        return r
    return None

def generate(address, canonical, r):
    width=r.get('width',4);tag=('i' if r.get('signed') else 'u')+str(width*8)
    name=f'restore_object_{r["kind"]}_{tag}_{address:08x}'
    offset=r['offset']&0xffffffff;pointer=f'(uintptr_t)(self+0x{offset:08x}u)'
    value_arg=',uint32_t value' if 'value_register' in r else ''
    if r['kind']=='lookup':
        body=f'uint32_t index=*(volatile uint8_t *){pointer};\n return (uint32_t)*(volatile uint8_t *)(uintptr_t)(0x{r["table_base"]:08x}u+index);'
    elif r['kind']=='read':
        typ=('int' if r['signed'] else 'uint')+str(width*8)+'_t'
        body=f'return (uint32_t)*(volatile {typ} *){pointer};'
    elif r['kind'] in ('self','offset'): body=f'return self+0x{offset:08x}u;'
    else:
        typ='uint'+str(width*8)+'_t';value='value' if value_arg else f'0x{r["constant"]:x}u'
        body=f'*(volatile {typ} *){pointer}=({typ}){value};'
    lines=['#include <stdint.h>','#if !defined(_MSC_VER) || !defined(_M_IX86)',
      '#error This entry preserves the original MSVC i386 register ABI.','#endif',
      '/* Generated from a complete native MOV/LEA rule; nonfaulting accesses.',
      '   Logical result, access width and canonical register/flag preservation',
      '   are explicit. Whole-game/exception-context equivalence is unproved. */',
      f'{r["result"]} {name}(uint32_t self{value_arg})','{',' '+body,'}',
      f'__declspec(naked) void {canonical}(void)','{',' __asm {','  pushfd']
    if r['result']=='void':
        lines+=['  pushad','  mov ecx,[esp+24]']
        if value_arg: lines+= [f'  mov eax,[esp+{REGS[r["value_register"]]}]','  push eax']
        lines+=['  push ecx',f'  call {name}',f'  add esp,{8 if value_arg else 4}','  popad']
    else:
        lines+=['  push ecx','  push edx','  push ecx',f'  call {name}','  add esp,4','  pop edx','  pop ecx']
    lines+=['  popfd']
    if r.get('native_flags')=='xor_eax_eax':
        lines+=['  push eax','  xor eax,eax','  pop eax']
    lines+=['  ret',' }','}','']
    return name,'\n'.join(lines).encode()

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--report',type=Path,default=ROOT/'build/bulk-reconstruction/plan.json')
    ap.add_argument('--write-candidates',type=Path)
    ap.add_argument('--limit',type=int,default=100)
    ap.add_argument('--exclude-manifest',type=Path,action='append',default=[])
    args=ap.parse_args()
    if args.limit<1: ap.error('--limit must be positive')
    sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    from native_test_pe import PE
    binary=ROOT/'build/smoke-game/Pharaoh.exe'
    if sha(binary)!=PIN: raise ValueError('Pinned original SHA256 mismatch')
    pe=PE(binary);dis=Cs(CS_ARCH_X86,CS_MODE_32);dis.detail=True
    entries,conflicts=catalogue();sources={};excluded=set()
    for path in (ROOT/'src').glob('*.c'):
        text=path.read_text(errors='strict');found=list(DEF.finditer(text))
        if len(found)==1: sources[int(found[0][2],16)]=(path,found[0][1],text)
    for path in [ROOT/'metadata/city-active-buildings-reconstruction.json',ROOT/'metadata/actor-cohort-2.json',*args.exclude_manifest]:
        d=json.loads(path.read_bytes());excluded.update(int(x['address'],16) for x in d.get('members',[]))
    owners=set()
    for folder in ['runtime','include']:
        for path in (ROOT/folder).rglob('*'):
            if path.suffix not in ('.c','.h','.asm'): continue
            content=path.read_text(errors='replace')
            owners.update(int(m[2],16) for m in DEF.finditer(content))
            # Conservatively reserve macro-generated entrypoints as existing C.
            # An ordinary call/prototype alone is not an algorithm owner.
            for line in content.splitlines():
                if re.match(r'^\s*[A-Z_][A-Z_0-9]*\s*\(',line):
                    owners.update(int(x,16) for x in re.findall(r'(?:FUN_|0x|\b)(00[0-9a-fA-F]{6})(?:u|\b)',line))
    project=ROOT/'build/c-only/pharaoh_restored.vcxproj';ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
    active={Path(x.attrib['Include']).resolve() for x in ET.parse(project).findall('.//m:ClCompile[@Include]',ns)}
    rows=[];members=[];candidate_hashes={};original_hashes={}
    for address,e in sorted(entries.items()):
        row=dict(address=f'0x{address:08x}',status='requires_semantic_analysis')
        item=sources.get(address)
        if address in excluded: row['status']='excluded_existing_cohort_or_assigned_owner'
        elif address in conflicts: row['status']='conflicting_cached_extents_require_audit'
        elif not item: row['status']='source_owner_requires_audit'
        else:
            path,canonical,text=item;row['source']=path.relative_to(ROOT).as_posix()
            marker=re.search(r'\b(?:u?int|undefined[124]|uint32_t|int32_t|char|byte|short|ushort|bool)\s+(?:\*\s*)*in_ECX\s*;',text)
            if not marker or len(re.findall(r'\bin_ECX\b',text))<2 or re.search(r'\bin_ECX\s*=(?!=)',text): row['status']='outside_current_automatic_rules'
            elif address in owners: row['status']='existing_C_definition_requires_reuse_audit'
            elif path.resolve() not in active: row['status']='outside_primary_active_library'
            else:
                obj=ROOT/'build/c-only/pharaoh_restored.dir/Release'/(path.stem+'.obj')
                if not obj.is_file() or obj.stat().st_mtime_ns<path.stat().st_mtime_ns or '_'+canonical not in defined_symbols(obj): row['status']='canonical_owner_requires_audit'
                else:
                    raw=native_bytes(e)
                    ins=list(dis.disasm(raw,address)) if raw else []
                    r=rule(ins)
                    if not r: row['status']='native_pattern_requires_analysis'
                    elif bytes(pe.data[address-pe.base:address-pe.base+len(raw)])!=raw: raise ValueError('Native bytes mismatch: '+hex(address))
                    else:
                        row.update(status='eligible_exact_native_rule',rule=r,native_bytes=len(raw),native_instructions=len(ins))
                        if args.write_candidates and len(members)<args.limit:
                            name,code=generate(address,canonical,r);dest=args.write_candidates/row['source'];dest.parent.mkdir(parents=True,exist_ok=True)
                            if dest.exists() and dest.read_bytes()!=code: raise ValueError('Candidate already differs: '+str(dest))
                            dest.write_bytes(code);candidate_hashes[row['source']]=sha(dest);original_hashes[row['source']]=sha(path)
                            signature=r['result']+f' cdecl{2 if "value_register"in r else 1}(uint32_t self'+(',uint32_t value'if'value_register'in r else '')+')'
                            members.append(dict(address=row['address'],source=row['source'],name=name,canonical_symbol=canonical,logical_signature=signature,native_bytes=len(raw),native_instructions=len(ins),native_final_mnemonic='ret',native_byte_sha256=hashlib.sha256(raw).hexdigest(),repair_kind='Unbound ECX replaced by an exact native memory/LEA C rule',rule=r,ownership=dict(object_path=obj.relative_to(ROOT).as_posix(),object_sha256=sha(obj),original_source_sha256=sha(path))))
                            row['status']='candidate_emitted_comparison_pending'
        rows.append(row)
    report=dict(schema='bulk-native-C-rule-plan-v1',native_sha256=PIN,catalogue_entries=len(entries),native_catalogue_closed=False,complete_game_reconstruction=False,behavior_comparison_complete=False,status_counts=dict(Counter(x['status']for x in rows)),functions=rows)
    save(args.report,report)
    if args.write_candidates:
        save(args.write_candidates/'members.json',dict(members=members))
        save(args.write_candidates/'hashes.json',dict(candidate_sha256=candidate_hashes,original_source_sha256=original_hashes))
    print(json.dumps(dict(catalogue_entries=len(entries),status_counts=report['status_counts'],emitted=len(members),report=str(args.report))))

if __name__=='__main__': main()
