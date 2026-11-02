"""Read-only saved-state actor replay in Unicorn, without launching a game.

This is a diagnostic fixture, not a substitute for the real save-load pipeline.
Only saved fixed-address regions are restored; unsaved runtime state is seeded
from the native image. Sound/profiling endpoints and OS time are controlled.
Unsaved map classification/storage indices are not reconstructed here, so a
divergence is a review lead, not proof of the reported live hang's cause.
"""
from verify_options_startup import *
from collections import deque


def timers(c, u, state):
    data = images[c].data
    read = lambda p: struct.unpack_from('<I', data, p)[0]
    string = lambda p: bytes(data[p:data.index(0, p)]).decode('ascii')
    descriptor = read(read(60)+24+104)
    trampoline = STOP+256
    while read(descriptor+12):
        names = read(descriptor) or read(descriptor+16)
        iat, i = read(descriptor+16), 0
        while read(names+i*4):
            item = read(names+i*4)
            name = string(item+2) if item < 0x80000000 else ''
            if name in ('timeGetTime', 'GetTickCount'):
                put(u, images[c].base+iat+i*4, trampoline)
                state['hooks'][trampoline] = (0, 0, lambda cpu, args: 348592050)
                trampoline += 16
            i += 1
        descriptor += 20


def saved_regions(path):
    source = ROOT/'tools/verify_compression_emulation.py'
    decoder = {'__file__': str(source), '__name__': 'saved_actor_decoder'}
    exec(compile(source.read_text().split('sample=bytes.fromhex', 1)[0],
                 str(source), 'exec'), decoder)
    data = path.read_bytes()
    descriptors = [struct.unpack_from('<5I', data, 12+i*20)
                   for i in range(struct.unpack_from('<I', data, 8)[0])]
    schema = next(i for i, d in enumerate(descriptors)
                  if d[0] == 0 and d[2:4] == (20, 300))
    position, regions = 6012, []
    for codec, at, size, count, _ in descriptors[schema+1:]:
        length = size*count
        marker = struct.unpack_from('<I', data, position)[0] if codec else None
        position += 4 if codec else 0
        stored = length if marker in (None, 0x80000000) else marker
        payload = data[position:position+stored]
        position += stored
        if not length:
            continue
        if codec and marker != 0x80000000:
            result = decoder['native'](0, length, 0xffffffff, payload)
            assert result['result'] == 1, (hex(at), result['result'])
            payload = bytes.fromhex(result['output'])[:length]
        assert len(payload) == length and 0x560000 <= at < at+length <= 0x126d000
        regions.append((at, payload))
    assert position == len(data)
    print('Save SHA256:', hashlib.sha256(data).hexdigest(), flush=True)
    return regions


def replay():
    regions = saved_regions(ROOT/'build/debug-startup-game/Save/Restore/autosave.sav')
    histories = []
    for c, u, state in machines:
        reset(u, state)
        u.mem_map(0x2fe00000, 0x200000)
        for at, data in regions:
            u.mem_write(at, data)
        hook(c, state, 0x47d350, 0, lambda cpu, args: 0)
        # Profiling timers are external OS observations, outside actor state.
        hook(c, state, 0x489a50, 1, lambda cpu, args: 0)
        hook(c, state, 0x489a80, 0, lambda cpu, args: 0)
        timers(c, u, state)
        history = deque(maxlen=20)
        entries = {(linked[name] if c else int(name[4:], 16)): name
                   for name in json_names}
        def observe(cpu, pc, size, unused, entries=entries, history=history):
            if pc in entries:
                history.append((entries[pc], get(cpu, 0xd056f8)))
        u.hook_add(UC_HOOK_CODE, observe)
        histories.append(history)
        print('Actor replay:', 'C' if c else 'native',
              'phase', get(u, 0x78d7cc), 'year', get(u, 0x78d7bc), flush=True)
    for tick in range(8):
        results = []
        for c, u, state in machines:
            try:
                try:
                    run(c, u, 0x489b70)
                except AssertionError:
                    # A whole actor pass includes two108000-DWORD scratch
                    # clears. Give the compiled scalar loops a larger finite
                    # instruction budget before diagnosing nontermination.
                    if u.reg_read(UC_X86_REG_EIP) == STOP:
                        raise
                    u.emu_start(u.reg_read(UC_X86_REG_EIP), STOP, count=18000000)
                    assert u.reg_read(UC_X86_REG_EIP) == STOP
                    assert u.reg_read(UC_X86_REG_ESP) == SP+4
                    assert [u.reg_read(registers[i]) for i in [3,5,6,7]] == saved
            except Exception:
                print('Tick', tick, 'C', bool(c), 'last entries:', list(histories[c]),
                      'PC', hex(u.reg_read(UC_X86_REG_EIP)), flush=True)
                raise
            results.append([(at,bytes(u.mem_read(at,len(data)))) for at,data in regions])
        differences=[]
        for (at,left),(_,right) in zip(*results):
            if left!=right:
                differences.append((hex(at),[(i,a,b) for i,(a,b) in enumerate(zip(left,right)) if a!=b][:8]))
        assert not differences, ('Serialized state differs',tick,differences[:4],list(histories[1]))
    print('PASS eight saved actor ticks, all serialized city regions including stock/actors/grid; target SHA256:', target_sha)


import json
linked = {name: int(at, 16) for name, at in re.findall(
    r'\s_(FUN_[0-9a-fA-F]{8})\s+([0-9a-fA-F]{8})\s', symbols)}
json_names = [row['name'] for row in json.loads((ROOT/'units.json').read_text())
              if row['name'] in linked]
if __name__ == '__main__':
    replay()
