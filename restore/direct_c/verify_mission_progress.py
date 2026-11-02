"""Whole native mission outcome evaluation and campaign advance, offline.

Outcome callbacks are controlled; real campaign indexing and typed callbacks
execute. No host game, profile writes, file IO, movies or save mutation.
"""
from verify_options_startup import *

CALLS = {0x4ea930: 0, 0x4f52f0: 1, 0x4ea280: 1, 0x417ae0: 1,
         0x51f780: 0, 0x4cf1c0: 0, 0x538dc0: 5, 0x4e0cf0: 5,
         0x4ea920: 0, 0x41aa20: 3}


def setup(u, state, player):
    reset(u, state)
    u.mem_write(0x784b60, bytes(0x100))
    for i in range(4):
        u.mem_write(0xea1380+i*0x49d8, bytes(0x49d8))
    for at, value in [(0xeaac28, player), (0xe38ebc, 0), (0x605a3c, 0),
                      (0x6059e8, 0x12345678), (0xe950dc, 0),
                      (0xe38db0, 1024), (0xe38dac, 768),
                      (0x78d7bc, -3500), (0x78d7b8, -3501), (0xe3907c, 0)]:
        put(u, at, value)
    for at, value in [(0xe38ea8, 0), (0xe38dd8, 0), (0xe38dd9, 0),
                      (0xe38dda, 1), (0xe3908c, 1), (0xe38dca, 0),
                      (0x7027c2, 0), (0x7027c3, 0)]:
        byte(u, at, value)
    put(u, 0xea1894+player*0x49d8, 1000)


def outcome(label, player=1, fields=(), houses=6, monument=1, mutate=0):
    states = []
    for c, u, state in machines:
        setup(u, state, player)
        for at, value, size in fields:
            {1: byte, 2: word, 4: put}[size](u, at, value)
        def action(uc, args, token):
            state['calls'].append((token, args))
            uc.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
            uc.reg_write(UC_X86_REG_EDX, 0xbaadf00d)
            if token == 0x4ea280:
                if mutate:
                    put(uc, 0xeaac28, mutate)
                return houses
            if token == 0x4ea930:
                return monument
            if token == 0x4f52f0 and mutate:
                put(uc, 0xeaac28, mutate)
            if token == 0x538dc0:
                byte(uc, 0xe38dda, args[4])
            return 0
        for token, count in CALLS.items():
            hook(c, state, token, count, lambda uc, args, t=token: action(uc, args, t))
        run(c, u, 0x4ea2c0)
        states.append((tuple(state['calls']), snapshot(u)))
    equal(states, label)
    return states[1][0]


def verify_outcomes():
    count = 0
    for player in [0, 1, 3]:
        off = player*0x49d8
        for required, have in [(6, 5), (6, 6), (256, 255), (256, 256)]:
            outcome(('houses', player, required, have), player,
                    [(0x784c34, required, 4), (0x784c38, 2, 4)], houses=have)
            count += 1
        for flag, target, current in [(0x784c3c, 0x784c24, 0xea5b3c),
                                       (0x784c3d, 0x784c28, 0xea5b40),
                                       (0x784c3e, 0x784c2c, 0xea5b44),
                                       (0x784c3f, 0x784c30, 0xea5b48),
                                       (0x784c58, 0x784c5c, 0xea1894)]:
            for have in [255, 256, 257]:
                outcome(('goal', player, hex(flag), have), player,
                        [(flag, 1, 4 if flag == 0x784c58 else 1),
                         (target, 256, 4), (current+off, have, 4)])
                count += 1
        for kind in ['none', 'editor', 'continued-wait', 'continued-done',
                     'forced', 'prior-win', 'final-win', 'alternate-movie', 'hostile-goals-met',
                     'loss-first', 'loss-again', 'loss-downgrade', 'military-loss']:
            f = [(0x784c34, 6, 4), (0x784c38, 2, 4)]
            if kind == 'none': f = []
            if kind == 'editor': f += [(0x605a3c, 1, 4)]
            if kind.startswith('continued'):
                f += [(0xea5d40+off, 1, 4), (0xea5d44+off, kind.endswith('wait'), 4)]
            if kind == 'forced': f += [(0xe38dd8, 1, 1)]
            if kind in ['prior-win', 'final-win', 'alternate-movie']:
                f += [(0xea5f88+off, 1, 4)]
            if kind == 'final-win': f += [(0xe38ea8, 10, 1)]
            if kind == 'alternate-movie': f += [(0xe3907c, 0x100, 4)]
            if kind.startswith('loss'):
                f += [(0x784c48, 1, 4), (0x78d7b8, -3501, 4),
                      (0x78d7bc, -3490, 4)]
            if kind == 'loss-again': f += [(0xea5f84+off, 1, 4)]
            if kind == 'loss-downgrade': f += [(0xea6018+off, 1, 4)]
            if kind == 'military-loss':
                f += [(0xea5eb4+off, 3, 4), (0xea5f0c+off, 5000, 4)]
            if kind == 'hostile-goals-met':
                f += [(0xea5eb4+off, 3, 4), (0xea5f0c+off, 4000, 4)]
            outcome((kind, player), player, f, houses=0 if kind.startswith('loss') else 6)
            count += 1
    for changing in [1, 3]:
        for initial in [0, 1]:
            outcome(('callback player reload', initial, changing), initial,
                    [(0x784c34, 6, 4), (0x784c38, 2, 4)], mutate=changing)
            count += 1
    print('PASS whole mission outcome, players, full goals and native callback reloads:', count, flush=True)
    return count


def verify_next_mission(recover=False):
    from replay_saved_actor_tick import timers
    path = ROOT/'build/debug-startup-game/Save/Restore.dat'
    profile = path.read_bytes()
    assert len(profile) == 0x4a9c
    results = []
    for c, u, state in machines:
        setup(u, state, 1)
        u.mem_write(0x6fdd08, profile)
        u.mem_write(S+0x1000, b'save\\Restore.dat\0')
        for at, value in [(0xe38eab, 0), (0xe38ea9, 0), (0xe38eaa, 255),
                          (0xe39088, 0), (0x7027b4, 1), (0xe38df8, 0)]:
            byte(u, at, value)
        put(u, 0xe39080, 2)
        put(u, 0xea5b48+0x49d8, 50)
        put(u, 0xea5cc0+0x49d8, 12)
        if recover and c:
            from verify_mission_schema import call_named
            data=(ROOT/'build/debug-startup-game/Save/Restore/autosave.sav').read_bytes()
            u.mem_write(0xe92e50,data[12:6012])
            put(u,0xe945c0,160);put(u,0xe945c4,181);byte(u,0x7027b4,0)
            call_named(u,'restore_mission_schema_recover_loaded')
            assert u.mem_read(0x7027b4,1)==b'\1'
        def profile_write(uc, args):
            if c:
                assert args[0] == 0x6fdd08
                args = args[1:]
            else:
                assert uc.reg_read(UC_X86_REG_ECX) == 0x6fdd08
            state['calls'].append((0x41b8a0, args))
            return 1
        hook(c, state, 0x516a20, 0, lambda uc, args: S+0x1000)
        hook(c, state, 0x41b8a0, 2 if c else 1, profile_write, cleanup=0 if c else 4)
        hook(c, state, 0x4188f0, 0,
             lambda uc, args: state['calls'].append((0x4188f0, args)) or 0)
        timers(c, u, state)
        run(c, u, 0x418640)
        assert bytes(u.mem_read(0xe38eaa, 2)) == b'\0\1'
        assert u.mem_read(0xe38ea8, 1) == b'\1'
        assert get(u, 0xd3b124) == 201
        assert get(u, 0x6fdca4) == 53 and get(u, 0x6fdcb0) == 0
        assert u.mem_read(0xe38dda, 1) == b'\4'
        assert get(u, 0x7027a8) == 0 and get(u, 0xd3b138) == 0
        byte(u, 0xe38df8, 1)
        run(c, u, 0x41aa60)
        assert u.mem_read(0xe38dda, 1) == bytes([53])
        assert get(u, 0xd3b124) == 201
        results.append((tuple(state['calls']),
            bytes(u.mem_read(0xe38ea8, 20)), bytes(u.mem_read(0xd3b124, 24)),
            bytes(u.mem_read(0x6fdca4, 16)), bytes(u.mem_read(0x6fdd08, len(profile)))))
    assert results[0] == results[1], 'real campaign indexing and timed briefing transition'
    print('PASS current read-only family profile: real first-to-second selection, ID201 and briefing timer; recovery=',recover, flush=True)
    print('Profile SHA256:', hashlib.sha256(profile).hexdigest(), flush=True)


if __name__ == '__main__':
    verify_outcomes()
    verify_next_mission()
    verify_next_mission(recover=True)
    print('Verified target SHA256:', target_sha)
