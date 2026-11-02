"""Compiled monitor bounds and dump contracts, with controlled Windows APIs.

No host thread, game, file or dump is created. Real Windows stack capture is
necessarily pending a live stall; this checks the compiled gating/error paths.
"""
from verify_options_startup import *

c, u, state = machines[1]
fields = ['journal', 'reporting', 'progress_stage', 'progress_time', 'save_time',
          'monitor_started', 'monitor_stopped', 'main_thread']


def invoke(name, args=(), cleanup=0):
    for reg, value in zip(registers, [0x11, 0x22, 0x33, saved[0], SP, *saved[1:]]):
        u.reg_write(reg, value)
    put(u, SP, STOP)
    for i, value in enumerate(args): put(u, SP+4+i*4, value)
    u.emu_start(sym(name), STOP, count=100000)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]] == saved


def prepare(now=100000, age=15000, window=1000, stage=2, busy=0,
            file_ok=True, dump_ok=True, library_ok=True, thread_ok=True):
    reset(u, state)
    for name in fields: put(u, sym(name), 0)
    for name, value in [('journal', 1), ('progress_stage', stage),
                        ('progress_time', now-age), ('save_time', now-window),
                        ('monitor_started', 1), ('reporting', busy), ('main_thread', 123)]:
        put(u, sym(name), value)
    u.mem_write(sym('hang_path'), 'X:\\PharaohRestoreC-hang.dmp\0'.encode('utf-16le'))
    events = []
    def sleep(cpu, args):
        assert args == (500,)
        put(cpu, sym('monitor_stopped'), 1)
        return 0
    def create(cpu, args):
        assert args == (sym('hang_path'), 0x40000000, 1, 0, 2, 0x80, 0)
        events.append('open')
        return 22 if file_ok else 0xffffffff
    def dump(cpu, args):
        assert args[:5] == (0xffffffff, 42, 22, 1, 0)
        assert args[6] == 0
        assert get(cpu, args[5]) == 2
        streams = get(cpu, args[5]+4)
        assert struct.unpack('<6I', bytes(cpu.mem_read(streams, 24))) == (
            0x10000, 228*228*2, 0xb350dc, 0x10001, 2000*388, 0xbcd5f8)
        events.append('dump')
        return int(dump_ok)
    def thread(cpu, args):
        assert args == (0, 0, sym('post_save_monitor@4'), 0, 0, 0)
        events.append('thread')
        return 77 if thread_ok else 0
    def close(cpu, args):
        events.append(('close', args[0]))
        return 1
    apis = {
        'GetTickCount': (0, lambda cpu, args: now),
        'Sleep': (1, sleep),
        'LoadLibraryExW': (3, lambda cpu, args: 77 if library_ok else 0),
        'GetProcAddress': (2, lambda cpu, args: STOP+0x800),
        'GetCurrentProcess': (0, lambda cpu, args: 0xffffffff),
        'GetCurrentProcessId': (0, lambda cpu, args: 42),
        'CreateFileW': (7, create), 'CloseHandle': (1, close),
        'CreateThread': (6, thread), 'GetLastError': (0, lambda cpu, args: 8)}
    data = images[1].data
    read = lambda p: struct.unpack_from('<I', data, p)[0]
    string = lambda p: bytes(data[p:data.index(0, p)]).decode('ascii')
    descriptor = read(read(60)+24+104)
    trampoline = STOP+0x100
    while read(descriptor+12):
        names, iat, i = read(descriptor) or read(descriptor+16), read(descriptor+16), 0
        while read(names+i*4):
            item = read(names+i*4)
            name = string(item+2) if item < 0x80000000 else ''
            if name in apis:
                count, action = apis[name]
                put(u, images[1].base+iat+i*4, trampoline)
                state['hooks'][trampoline] = (count, count*4, action)
                trampoline += 16
            i += 1
        descriptor += 20
    state['hooks'][STOP+0x800] = (7, 28, dump)
    state['hooks'][sym('restore_game_trace')] = (1, 0, lambda cpu, args: 0)
    return events


def verify():
    cases = 0
    for args, expected in [({}, 1), ({'stage': 0}, 0), ({'age': 14999}, 0),
            ({'window': 60000}, 1), ({'window': 60001}, 0), ({'busy': 1}, 0),
            ({'file_ok': False}, 0), ({'dump_ok': False}, 1),
            ({'library_ok': False}, 0), ({'now': 32, 'window': 48, 'age': 15000}, 1)]:
        events = prepare(**args)
        invoke('post_save_monitor@4', [0], cleanup=4)
        assert events.count('dump') == expected, (args, events)
        assert get(u, sym('reporting')) == args.get('busy', 0)
        cases += 1
    events = prepare()
    put(u, sym('monitor_started'), 0)
    invoke('restore_game_progress', [4])
    assert get(u, sym('progress_stage')) == 2
    put(u, sym('monitor_started'), 1)
    invoke('restore_game_progress', [4])
    assert get(u, sym('progress_stage')) == 4 and get(u, sym('progress_time')) == 100000
    invoke('restore_game_progress', [0])
    assert get(u, sym('progress_stage')) == 0
    cases += 3
    for thread_ok in [True, False]:
        events = prepare(thread_ok=thread_ok)
        put(u, sym('monitor_started'), 0)
        invoke('restore_game_trace_save_complete')
        invoke('restore_game_trace_save_complete')
        assert events.count('thread') == (1 if thread_ok else 2)
        assert get(u, sym('monitor_started')) == int(thread_ok)
        assert get(u, sym('save_time')) == 100000
        cases += 1
    events = prepare()
    put(u, sym('journal'), 0xffffffff)
    put(u, sym('monitor_started'), 0)
    invoke('restore_game_trace_save_complete')
    assert not events and get(u, sym('monitor_started')) == 0
    cases += 1
    print('PASS compiled post-save monitor gating, clock wrap, API failures, single thread and dump streams:', cases)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__': verify()
