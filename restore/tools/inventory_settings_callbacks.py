"""Read original settings descriptors; never infer implementation from a label.

The table addresses/counts come from the retained native main-input body and
its matching C translation. This inventories obligations, not completed pages.
"""
from pathlib import Path
import hashlib,json,re,struct
ROOT=Path(__file__).resolve().parents[1]
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
digest=hashlib.sha256(image).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20)
sections=[]
for i in range(u16(pe+6)):
    q=table+i*40;sections.append((base+u32(q+12),u32(q+16),u32(q+20)))
def read(address,size):
    for lo,n,raw in sections:
        if lo<=address and address+size<=lo+n:return image[raw+address-lo:raw+address-lo+size]
    raise ValueError(f'unbacked {address:x}/{size}')
spec=[(10,'display','rectangle',0x5c2e20,5),(86,'popup','rectangle',0x5c2eb0,14),(69,'difficulty','square',0x5c8ee8,2),(11,'sound','rectangle',0x5c3070,6),(11,'sound','square',0x5c8df8,8),(12,'speed','rectangle',0x5c3038,2),(12,'speed','square',0x5c8d98,4)]
registry=(ROOT/'runtime/menu_callbacks.c').read_text()
central={int(s,16) for s in re.findall(r'address==0x([0-9a-fA-F]{8})u',registry)}
central.update(int(s,16) for s in json.loads((ROOT/'metadata/dropdown-actions-emulation.json').read_text())['coverage'])
definitions={}
for directory in ['src','runtime']:
    for source in (ROOT/directory).glob('*.c'):
        for match in re.finditer(r'(?m)^[ \t]*(?:[A-Za-z_][A-Za-z0-9_]*[ \t]+)+\**FUN_([0-9a-f]{8})\s*\([^;{}]*\)\s*\{',source.read_text(errors='replace')):definitions.setdefault(int(match[1],16),[]).append(source.relative_to(ROOT).as_posix())
cache=[]
patterns=[re.compile(r'\b0x0*'+f'{address:x}'+r'\b',re.I) for _,_,_,address,_ in spec]
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
    batch=json.loads(path.read_text());assert batch['native_sha256']==digest
    for f in batch['entries']:
        matches=[x for x in f['assembly']['items'] if any(pattern.search(x['instruction']) for pattern in patterns)]
        if matches:cache.append({'entry':f['requested_address'],'cache':path.relative_to(ROOT).as_posix(),'references':matches})
tables=[];tokens=set()
for mode,label,kind,address,count in spec:
    width=28 if kind=='rectangle' else 24;raw=read(address,width*count);items=[]
    for i in range(count):
        b=raw[i*width:(i+1)*width]
        if kind=='rectangle':callbacks=struct.unpack_from('<II',b,8);arguments=struct.unpack_from('<ii',b,20)
        else:callbacks=(struct.unpack_from('<I',b,8)[0],);arguments=struct.unpack_from('<ii',b,16)
        tokens.update(token for token in callbacks if token)
        items.append({'address':f'{address+i*width:08x}','record_hex':b.hex(),'callbacks':[f'{token:08x}' for token in callbacks],'primary':arguments[0],'secondary':arguments[1]})
    tables.append({'mode':mode,'reference_label':label,'kind':kind,'address':f'{address:08x}','count':count,'record_size':width,'original_bytes_sha256':hashlib.sha256(raw).hexdigest(),'items':items})
out={'format':1,'native_sha256':digest,'complete_settings_pages':False,'method':'Exact file-backed native descriptor bytes and retained native instruction references; labels are orientation only, not a behavior specification.','tables':tables,'native_table_references':cache,'unique_callback_tokens':len(tokens),'callbacks':[{'token':f'{token:08x}','has_C_definition':token in definitions,'C_sources':definitions.get(token,[]),'routed_in_central_menu_router':token in central,'body_verified_by_this_inventory':False} for token in sorted(tokens)],'limitations':['No callbacks are counted as reconstructed from table membership or C source presence.','Global selected argument is published by the actual input controllers; individual callback ABI and argument use still require native verification.','Page drawing, keyboard/cancel handling and main-controller global widths remain separate obligations.']}
(ROOT/'metadata/settings-callback-inventory.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'tables':len(tables),'records':sum(t['count'] for t in tables),'unique_callbacks':len(tokens),'C_definitions':sum(t in definitions for t in tokens),'routed':sum(t in central for t in tokens)}))
