"""Assemble retained REA pseudocode with reversible address-based names."""
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
inventory = json.loads((ROOT/'raw/rea/procedures.json').read_text())['normalized_result']
aliases = {}
for proc in inventory:
    address = proc['address']
    if address.startswith('0x') and not proc.get('procedure',{}).get('external'):
        aliases[proc['value']] = 'FUN_' + f'{int(address,16):08x}'
# Provider display names can differ from the emitted C identifier.
signature_pattern = re.compile(r'^([A-Za-z_][\w *]+?)(?:\s+|(?<=\*))([A-Za-z_?$@][\w?$@:~]*)\s*\([^;{}]*?\)\s*\n\s*\{',re.M)
records = []
for file in sorted((ROOT/'raw/rea').glob('batch-*.json')):
    data = json.loads(file.read_text())
    for item in data['normalized_result']['items']:
        code = item.get('pseudocode','').replace('\r\n','\n')
        match = signature_pattern.search(code)
        if match:
            emitted_name = match[2]
            proc = next((p for p in inventory if p['address']==item['address']),None)
            if not (proc and (proc.get('procedure',{}).get('thunk_target') or '').startswith('EXTERNAL:')):
                aliases[emitted_name] = 'FUN_'+f'{int(item["address"],16):08x}'
        records.append((item,data['evidence_id']))
pattern = re.compile(r'[A-Za-z_?$@][\w?$@]*(?:::[~A-Za-z_?$@][\w?$@]*)*')
items = []
failures = []
manifest = []
for item,evidence in records:
        if item['status'] != 'ok' or not item.get('pseudocode'):
            failures.append(item)
            fallback = ROOT/'metadata/fallbacks'/('FUN_'+f'{int(item["address"],16):08x}'+'.c')
            if not fallback.exists():
                manifest.append(dict(address=item['address'],status='unavailable',evidence=evidence))
                continue
            code = fallback.read_text()
            status = 'historical-ghidra-fallback'
        else:
            code = item['pseudocode'].replace('\r\n','\n')
            status = 'rea'
        code = pattern.sub(lambda m:aliases.get(m[0],m[0]),code)
        match = signature_pattern.search(code)
        if not match:
            raise ValueError(f'Cannot locate emitted function definition: {item["address"]}')
        canonical_name = 'FUN_'+f'{int(item["address"],16):08x}'
        code = code[:match.start(2)] + canonical_name + code[match.end(2):]
        items.append(code)
        manifest.append(dict(address=item['address'],status=status,evidence=evidence))
expected = sum(not p.get('procedure',{}).get('external') for p in inventory)
if len(manifest) != expected and '--partial' not in sys.argv:
    raise SystemExit(f'Export incomplete: {len(items)} successes, {len(failures)} failures, expected {expected}')
output = ROOT/'raw/Pharaoh.rea.c'
output.write_text('/* REA Ghidra export: see raw/rea for unchanged provider output. */\n\n'+'\n\n'.join(items),encoding='utf-8')
(ROOT/'raw/rea/decompile_failures.json').write_text(json.dumps(failures,indent=2))
(ROOT/'metadata/export_manifest.json').write_text(json.dumps(manifest,indent=2))
print(json.dumps(dict(exported=len(items),rea_failed=len(failures),
    fallback=sum(x['status']=='historical-ghidra-fallback' for x in manifest),
    unavailable=sum(x['status']=='unavailable' for x in manifest),expected=expected,path=str(output))))
