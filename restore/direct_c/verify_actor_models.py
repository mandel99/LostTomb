"""Compare the four real model loaders and hunter/cart encounters offline.

Uses the pinned original and compiled C with identical actual model files.
Only file loading, file size and status logging are controlled endpoints.
Search, number parsing, table writes and encounter logic execute in full.
No game process, save writes or generated reports.
"""
from verify_options_startup import *


def execute(c, u, token, args=()):
    for r, v in zip(registers, [0x11111111, 0x22222222, 0x33333333,
                              saved[0], SP, *saved[1:]]):
        u.reg_write(r, v)
    u.reg_write(UC_X86_REG_EFLAGS, 2)
    put(u, SP, STOP)
    for i, v in enumerate(args):
        put(u, SP+4+i*4, v)
    u.emu_start(address(c, token), STOP, count=20000000)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == SP+4
    assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
    return u.reg_read(UC_X86_REG_EAX)


def verify():
    for difficulty, name in enumerate(['veryeasy', 'easy', 'normal', 'hard', 'impossible']):
        content = (ROOT/f'build/debug-startup-game/Figure_model_{name}.txt').read_bytes()
        states = []
        for c, u, state in machines:
            reset(u, state)
            put(u, 0xe92e1c, S+0x1000)
            put(u, 0xe39080, difficulty)

            def load_file(uc, args):
                assert args == (get(uc, 0x60191c+difficulty*4), S+0x1000, 0)
                uc.mem_write(args[1], content)
                return 1

            for token, n, fn in [(0x4ceb60, 3, load_file),
                                 (0x4ced50, 1, lambda uc, args: len(content)),
                                 (0x4ce6e0, 3, lambda uc, args: 0)]:
                hook(c, state, token, n, fn)
            assert execute(c, u, 0x540ed0) == 1
            assert get(u, 0x7090cc+4*56) == 1, 'Cart model overwritten by enemy data'
            assert get(u, 0x7090cc+9*56) == 1
            states.append(snapshot(u))
        assert states[0] == states[1], ('model tables', name)
        print('PASS complete figure/enemy model loader:', name, flush=True)

        # Changing only the cart's model category to its former corrupted
        # value reproduces the unwanted combat transition in both graphs.
        for enemy_profile in [False, True]:
            encounters = []
            for c, u, state in machines:
                u.mem_write(0xbcd5f8, bytes(3*388))
                for actor, kind in [(1, 73), (2, 4)]:
                    byte(u, 0xbcd602+actor*388, kind)
                    byte(u, 0xbcd606+actor*388, 1)
                    byte(u, 0xbcd630+actor*388, 8)
                    for field in [0xbcd60c, 0xbcd60e, 0xbcd61c, 0xbcd61e]:
                        word(u, field+actor*388, 100)
                for field, value in [(0xeaac28, 0), (0xea5ce8, 0),
                                     (0xd056f8, 1), (0xf55b88, 0)]:
                    put(u, field, value)
                if enemy_profile:
                    put(u, 0x7090cc+4*56, 3)
                execute(c, u, 0x4b1210, [1, 2])
                action = u.mem_read(0xbcd630+388, 1)[0]
                assert action == (5 if enemy_profile else 8)
                encounters.append(snapshot(u))
            assert encounters[0] == encounters[1], ('hunter/cart encounter', name, enemy_profile)
    print('PASS 10 real hunter/cart encounters; final C SHA256:', target_sha)


def verify_other_models():
    families = [(0x540c90, 0x601908, 'Pharaoh_Model'),
                (0x541160, 0x601930, 'Tax_Sentiment_Model'),
                (0x541310, 0, 'trade_recommends')]
    for token, paths, stem in families:
        levels = ['VeryEasy', 'Easy', 'Normal', 'Hard', 'Impossible'] if paths else ['']
        for difficulty, level in enumerate(levels):
            name = f'{stem}_{level}.txt' if paths else f'{stem}.txt'
            content = (ROOT/'build/debug-startup-game'/name).read_bytes()
            states = []
            for c, u, state in machines:
                reset(u, state)
                put(u, 0xe92e1c, S+0x1000)
                put(u, 0xe39080, difficulty)
                def load_file(uc, args):
                    expected_path = get(uc, paths+difficulty*4) if paths else 0x601dc0
                    assert args == (expected_path, S+0x1000, 0)
                    uc.mem_write(args[1], content)
                    return 1
                for entry, n, fn in [(0x4ceb60, 3, load_file),
                                     (0x4ced50, 1, lambda uc, args: len(content)),
                                     (0x4ce6e0, 3, lambda uc, args: 0)]:
                    hook(c, state, entry, n, fn)
                value = execute(c, u, token)
                states.append((value, snapshot(u)))
            assert states[0] == states[1], ('whole model load', name)
            assert states[1][0] == 1
        print('PASS complete model loader:', stem, len(levels), 'files', flush=True)


if __name__ == '__main__':
    verify()
    verify_other_models()
