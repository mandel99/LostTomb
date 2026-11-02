"""Compare startup lookup C with isolated original instructions, never the game.

The original file is an optional analysis input. Default C builds/tests do not
read it. Only the listed instruction ranges can execute in this Unicorn VM.
"""
from pathlib import Path
import hashlib
import json
import struct
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'build/offline-verification-deps'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_BLOCK, __version__ as unicorn_version
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_EFLAGS)

image = (ROOT / 'build/smoke-game/Pharaoh.exe').read_bytes()
digest = hashlib.sha256(image).hexdigest()
assert digest == 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16 = lambda b, p: struct.unpack_from('<H', b, p)[0]
u32 = lambda b, p: struct.unpack_from('<I', b, p)[0]
pe = u32(image, 0x3c)
opt = pe + 24
table = opt + u16(image, pe + 20)
base, size = u32(image, opt + 28), u32(image, opt + 56)
vm = Uc(UC_ARCH_X86, UC_MODE_32)
vm.mem_map(base, (size + 4095) & ~4095)
vm.mem_write(base, image[:u32(image, opt + 60)])
for i in range(u16(image, pe + 6)):
    p = table + i * 40
    rva, raw, length = u32(image, p + 12), u32(image, p + 20), u32(image, p + 16)
    vm.mem_write(base + rva, image[raw:raw + length])
OBJECT = 0x30000000
STOP = 0x33000000
STACK = 0x34000000
for address, length in [(OBJECT, 0x10000), (STOP, 0x1000), (STACK, 0x100000)]:
    vm.mem_map(address, length)
RANGES = [(0x417dd0, 0x417e69), (0x41b920, 0x41b9cb),
          (0x41ba20, 0x41ba7a), (0x41bab0, 0x41bacf),
          (0x4cc110, 0x4cc15c), (0x4cc160, 0x4cc1ac),
          (0x4303a0, 0x43047f), (0x4ddec0, 0x4ddf12),
          (0x4ddf20, 0x4ddf6c), (0x4ddf70, 0x4ddf8e)]

def guard(cpu, address, size, user):
    if not any(start <= address and address + size - 1 <= end for start, end in RANGES):
        raise RuntimeError('Execution escaped audited ranges at ' + hex(address))

vm.hook_add(UC_HOOK_BLOCK, guard)

def write(address, value, width=4):
    vm.mem_write(address, (int(value) & ((1 << (width * 8)) - 1)).to_bytes(width, 'little'))

def read(address, width=4):
    return int.from_bytes(vm.mem_read(address, width), 'little')

def signed(value):
    return value - 0x100000000 if value & 0x80000000 else value

def invoke(entry, args=(), receiver=OBJECT, cleanup=0):
    sp = STACK + 0x80000
    registers = [UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
                 UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP]
    for i, register in enumerate(registers):
        vm.reg_write(register, (i + 1) * 0x11111111)
    vm.reg_write(UC_X86_REG_ECX, receiver)
    vm.reg_write(UC_X86_REG_ESP, sp)
    vm.reg_write(UC_X86_REG_EFLAGS, 0x202)
    for i, value in enumerate([STOP] + list(args)):
        write(sp + 4 * i, value)
    vm.emu_start(entry, STOP, count=1000000)
    assert vm.reg_read(UC_X86_REG_EIP) == STOP, 'Instruction budget exhausted'
    assert vm.reg_read(UC_X86_REG_ESP) == sp + 4 + cleanup, 'Stack cleanup differs'
    for register, value in [(UC_X86_REG_EBX, 0x22222222), (UC_X86_REG_ESI, 0x55555555),
                            (UC_X86_REG_EDI, 0x66666666), (UC_X86_REG_EBP, 0x77777777)]:
        assert vm.reg_read(register) == value, 'Callee-saved register differs'
    return vm.reg_read(UC_X86_REG_EAX)

def campaign_cases():
    for seed in range(6):
        for count in [0, 1, 12, 100, -1]:
            for category in [0, 1, 4, 5, 11, 255, 256, -1]:
                for previous in [-2147483648, -1, 0, count - 1, count, 2147483647]:
                    for choice in [0, 1]:
                        yield [0, seed, count, category, previous, choice, seed % 8, seed % 5]
                yield [1, seed, count, category, 0, 0, seed % 8, seed % 5]
            yield [2, seed, count, 0, 0, 0, seed % 8, seed % 5]
        for count in [0, 12, 100]:
            for category in [-128, -1, 0, 1, 4, 5, 11, 127, 128, 255]:
                yield [3, seed, count, category, 0, 0, seed % 8, seed % 5]

def campaign_native(header, c, case):
    op, seed, count, category, previous, choice, profile, option = case
    data = bytes.fromhex(c['input'])
    vm.mem_write(OBJECT, data)
    vm.mem_write(0x6fdd08, data)
    for (address, width), value in zip(header['fields'], c['before']):
        write(address, value, width)
    if op == 0:
        result = signed(invoke(0x41b920, [category, previous, choice], cleanup=12))
    elif op == 1:
        result = invoke(0x41ba20, [category], cleanup=4) & 255
    elif op == 2:
        result = invoke(0x41bab0) & 255
    else:
        invoke(0x417dd0, [category])
        result = 0  # Side effects only; the native return register is unspecified.
    return {'result': result,
            'after': [read(address, width) for address, width in header['fields']],
            'object_after': bytes(vm.mem_read(0x6fdd08 if op == 3 else OBJECT, len(data))).hex()}

def text_cases():
    for op in [0, 1]:
        for group in range(4):
            for entry in [-2147483648, -1, 0, 1, 2]:
                yield [op, group, entry]

def text_native(header, c, case):
    op, group, entry = case
    data = bytearray(b'Q' * 512)
    fixtures = [(16, b'Alpha\0Beta\0Gamma\0Tail\0'),
                (80, b'\1\0Delta\0Epsilon\0Tail\0'),
                (160, b'\0\0Oak\0 Pine\0Tail\0'),
                (256, b'\x80\xff \0last\0tail\0\0')]
    for i, (offset, content) in enumerate(fixtures):
        data[offset:offset + len(content)] = content
        data[offset - 1] = 0
        write(0xe34c38 + i * 8, offset)
        write(0xe34c3c + i * 8, 0xeeeeeeee)
    vm.mem_write(header['base'], bytes(data))
    write(0xe36c70, header['base'])
    write(0xe20768, 0xdeadbeef)
    write(0xe20764, 0xfeedface)
    invoke(0x4cc160 if op else 0x4cc110, [group, entry])
    return {'primary': read(0xe20768), 'secondary': read(0xe20764)}

def grid_cases():
    for op in range(3):
        for width in [0, 123, 256, 640, 800, 1024, 1280, -1]:
            yield [op, width]

def grid_native(header, c, case):
    op, width = case
    vm.mem_write(0x70e430, b'\x5a' * header['grid_bytes'])
    write(0xe38db0, width)
    invoke([0x4ddec0, 0x4ddf20, 0x4ddf70][op])
    return bytes(vm.mem_read(0x70e430, header['grid_bytes'])).hex()

def compare(name, cases, native):
    executable = ROOT / ('build/c-only-debug/Debug/restore_' + name + '_verify.exe')
    process = subprocess.Popen([str(executable), '--oracle-batch'], cwd=ROOT,
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    result = {'checked': 0, 'compiled_C_oracle_sha256': hashlib.sha256(executable.read_bytes()).hexdigest()}
    try:
        header = json.loads(process.stdout.readline())
        for case in cases():
            case = [v & 0xffffffff for v in case]
            process.stdin.write(' '.join(map(str, case)) + '\n')
            process.stdin.flush()
            line = process.stdout.readline()
            if not line:
                raise RuntimeError('C oracle stopped: ' + process.stderr.read())
            actual = json.loads(line)
            expected = native(header, actual, case)
            if name == 'campaign':
                actual = {key: actual[key] for key in expected}
            if actual != expected:
                raise AssertionError('Different C/native output: ' + repr(actual)[:700] + ' / ' + repr(expected)[:700])
            result['checked'] += 1
        result['passed'] = True
    except Exception as error:
        result.update(passed=False, case=case, error=str(error), native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
    finally:
        process.stdin.close()
        try:
            process.wait(timeout=10)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()
        result['oracle_exit_code'] = process.returncode
        if process.returncode != 0:
            result['passed'] = False
    print(name, json.dumps(result), flush=True)
    return result

started = time.monotonic()
report = {'format': 1, 'original_sha256': digest, 'unicorn_version': unicorn_version,
          'method': 'Compiled C versus isolated original x86: exact results, memory and ABI checks; no external endpoints',
          'original_game_executed': False, 'hybrid_executed': False,
          'native_instruction_ranges': [[hex(a), hex(b)] for a, b in RANGES]}
report['campaign'] = compare('campaign', campaign_cases, campaign_native)
report['text'] = compare('text', text_cases, text_native)
report['grid'] = compare('grid', grid_cases, grid_native)
sources = ['include/campaign_index.h', 'src/unit_00213.c', 'src/unit_00253.c',
    'src/unit_00256.c', 'src/unit_00258.c', 'tests/campaign_index.c',
    'include/text_selection.h', 'runtime/text_selection.c', 'src/unit_01672.c',
    'src/unit_01673.c', 'tests/text_selection.c', 'src/unit_00456.c',
    'src/unit_01882.c', 'src/unit_01883.c', 'src/unit_01884.c',
    'tests/startup_grid.c', 'tools/verify_startup_lookups_emulation.py']
report['source_hashes'] = {file: hashlib.sha256((ROOT / file).read_bytes()).hexdigest() for file in sources}
report['elapsed_seconds'] = round(time.monotonic() - started, 3)
report['passed'] = all(report[name]['passed'] for name in ['campaign', 'text', 'grid'])
(ROOT / 'metadata/startup-lookups-emulation.json').write_text(json.dumps(report, indent=2) + '\n')
sys.exit(0 if report['passed'] else 1)
