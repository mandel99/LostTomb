"""Read-only native settings-block reference audit; never launches the game."""
from pathlib import Path
import hashlib
import json
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'build/offline-verification-deps'))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM

START, END = 0xe38e58, 0xe3908c
path = ROOT / 'build/smoke-game/Pharaoh.exe'
raw = path.read_bytes()
sha = hashlib.sha256(raw).hexdigest()
assert sha == 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16 = lambda p: struct.unpack_from('<H', raw, p)[0]
u32 = lambda p: struct.unpack_from('<I', raw, p)[0]
pe = u32(60)
opt = pe + 24
base = u32(opt + 28)
sections = []
for j in range(u16(pe + 6)):
    p = opt + u16(pe + 20) + j * 40
    sections.append(dict(name=raw[p:p+8].rstrip(b'\0').decode(),
                         start=base+u32(p+12), virtual_size=u32(p+8),
                         raw_size=u32(p+16), raw_offset=u32(p+20), flags=u32(p+36)))

def raw_at(address, size):
    for s in sections:
        off = address - s['start']
        if 0 <= off and off + size <= s['raw_size']:
            return raw[s['raw_offset']+off:s['raw_offset']+off+size]
    return None

decoder = Cs(CS_ARCH_X86, CS_MODE_32)
decoder.detail = True
refs, instructions, occupied, functions = [], {}, {}, set()
for cache in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
    doc = json.loads(cache.read_text(encoding='utf-8-sig'))
    assert doc['native_sha256'] == sha
    for entry in doc['entries']:
        proc = entry.get('info', {}).get('address')
        if not proc:
            continue
        functions.add(proc)
        for row in entry.get('assembly', {}).get('items', []):
            a, b = int(row['address'], 16), bytes.fromhex(row['bytes'])
            assert raw_at(a, len(b)) == b, (cache, hex(a))
            if a in instructions:
                continue
            instructions[a] = (b, proc)
            for p in range(a, a+len(b)):
                occupied[p] = a
            if not any(START <= struct.unpack_from('<I', b, k)[0] < END
                       for k in range(len(b)-3)):
                continue
            for ins in decoder.disasm(b, a):
                for op in ins.operands:
                    value = op.imm & 0xffffffff if op.type == X86_OP_IMM else op.mem.disp & 0xffffffff if op.type == X86_OP_MEM else -1
                    if START <= value < END:
                        refs.append(dict(address=hex(a), procedure='0x'+proc,
                                         instruction=ins.mnemonic+' '+ins.op_str,
                                         target=hex(value), offset=hex(value-START),
                                         size=op.size, memory=op.type == X86_OP_MEM,
                                         access=op.access,
                                         indexed=bool(op.type == X86_OP_MEM and (op.mem.base or op.mem.index))))

# Every possible unaligned DWORD window, not just compiler-aligned data.
# This catches pointer tables and literal references outside cached procedures.
literals = []
for off in range(len(raw)-3):
    value = u32(off)
    if not START <= value < END:
        continue
    section = next((s for s in sections if s['raw_offset'] <= off < s['raw_offset']+s['raw_size']), None)
    va = section['start'] + off - section['raw_offset'] if section else None
    owner = occupied.get(va)
    literals.append(dict(file_offset=hex(off), address=hex(va) if va else None,
                         value=hex(value), offset=hex(value-START),
                         section=section['name'] if section else None,
                         instruction=hex(owner) if owner else None))

text = (ROOT/'build/debug-startup-game/Pharaoh_Text.eng').read_bytes()
group_start = struct.unpack_from('<I', text, 28 + 52*8)[0]
# Native lookup ignores the second table DWORD and skips control bytes.
strings = text[8028+group_start:].split(b'\0')
labels = {str(i): strings[i].lstrip(bytes(range(32))).decode('cp1252')
          for i in [0, 1, 2, 3]}
descriptors = []
for index in range(181):
    at = 0x5e09a0 + index*20
    row = struct.unpack('<5I', raw_at(at, 20))
    if START <= row[1] < END:
        descriptors.append(dict(index=index, address=hex(at), codec=row[0],
                                target=hex(row[1]), offset=hex(row[1]-START),
                                element_size=row[2], count=row[3], extra=row[4]))
unknown_ranges = [(0x009,1),(0x00e,2),(0x040,12),(0x05c,4),(0x06a,1),
                  (0x07c,4),(0x088,4),(0x090,4),(0x09c,392),(0x232,2)]
unknowns = []
for off, size in unknown_ranges:
    unknowns.append(dict(offset=hex(off), size=size,
                         references=[r for r in refs if r['memory'] and
                                     int(r['offset'],16) < off+size and
                                     int(r['offset'],16)+r['size'] > off],
                         literals=[r for r in literals if off <= int(r['offset'],16) < off+size]))

# Separate ranges with no observed field consumers from recovered active fields.
# A default write alone does not establish a historical option name.
passive_ranges = [(0x009, 1, 0, '0x414819'), (0x00e, 2, 0, None),
                  (0x040, 12, 0, None), (0x07c, 4, 2, '0x414820'),
                  (0x088, 4, 0, '0x41483a'), (0x090, 4, 0, None),
                  (0x09c, 392, 0, None), (0x232, 2, 0, None)]
origins = []
for off, size, initial, writer in passive_ranges:
    field = next(r for r in unknowns if int(r['offset'], 16) == off)
    reads = [r for r in field['references'] if r['access'] & 1]
    writes = [r for r in field['references'] if r['access'] & 2]
    assert not reads, (hex(off), reads)
    assert [r['address'] for r in writes] == ([writer] if writer else [])
    # The full-file literal scan also covers code absent from the cached bodies.
    expected_literals = [writer] if writer else []
    actual_literals = [r['instruction'] for r in field['literals']]
    assert actual_literals == (expected_literals if off != 0x09c else [None])
    if off == 0x09c:
        assert field['literals'][0]['address'] == '0x5e141c'
        assert any(r['index'] == 134 and r['target'] == '0xe38ef4'
                   and r['element_size'] == 4 and r['count'] == 99
                   for r in descriptors)
    origins.append(dict(offset=hex(off), size=size, initial_value=initial,
                        initial_source='explicit initializer store' if writer else 'PE virtual zero-fill',
                        default_writer=writer,
                        direct_field_reads=reads, direct_field_writes=writes,
                        later_sources=['existing Pharaoh.inf'] +
                                      (['SAV/PAK descriptor134, first392 of396 bytes'] if off == 0x09c else []),
                        persisted_by='414750 writes the entire564-byte block',
                        historical_semantic_name=None))
assert sum(r['size'] for r in origins) == 421
uncached_passive_literals = [r for r in literals if r['section'] == '.text'
                            and not r['instruction']
                            and any(off <= int(r['offset'], 16) < off+size
                                    for off, size, _, _ in passive_ranges)]
assert not uncached_passive_literals, uncached_passive_literals
settings_section = next(s for s in sections if s['start'] <= START < s['start']+s['virtual_size'])
assert START >= settings_section['start']+settings_section['raw_size']
report = dict(native_sha256=sha, settings_start=hex(START), size=END-START,
              sections=sections, authenticated_functions=len(functions),
              authenticated_instructions=len(instructions),
              reference_count=len(refs), literal_count=len(literals),
              literal_targets={hex(v): sum(int(r['value'],16)==v for r in literals)
                               for v in sorted({int(r['value'],16) for r in literals})},
              escaped_pointers=[r for r in refs if not r['memory']],
              pointers_outside_text=[r for r in literals if r['section'] != '.text'],
              indexed_operands=[r for r in refs if r['indexed']],
              initial_storage='PE virtual zero-fill beyond .data raw bytes',
              language_group_52=labels,
              save_descriptors=descriptors, previously_unknown_fields=unknowns,
              passive_field_origins=origins, passive_bytes=421,
              uncached_passive_code_literals=uncached_passive_literals,
              limitations=['Static references are not a proof against all computed addresses.',
                           'Raw byte matches can also be accidental non-pointer constants.',
                           'Runtime writes by generic file loading are separate from field consumers.'])
out = ROOT/'metadata/pharaoh-inf-native-audit.json'
out.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
print('Authenticated functions/instructions:', len(functions), len(instructions))
print('References:', len(refs), 'literal occurrences:', len(literals))
print('Unowned literals:', sum(not x['instruction'] for x in literals))
print('Pointers in non-code sections:', json.dumps([x for x in literals if x['section'] != '.text']))
print('Indexed operands:', json.dumps([x for x in refs if x['indexed']]))
print('Previously unknown fields:', [(r['offset'], len(r['references']), len(r['literals'])) for r in unknowns])
print('Save descriptors:', json.dumps(descriptors))
print('Language group52:', labels)
print('Settings section:', [s for s in sections if s['start'] <= START < s['start']+s['virtual_size']])
print('Report:', out)
