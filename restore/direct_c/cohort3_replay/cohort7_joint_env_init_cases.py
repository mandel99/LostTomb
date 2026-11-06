"""Expanded initializer domains; actual strlen/strcpy and MB initializer run."""
import struct
from cohort7_startup_cases import base, put, child, TEXT, HEAP, COPY, WIDE, MB, READY, ENV, ENVV

ENTRY = 0x567994
OLD = [b'=C:=old', b'A=one', b'LONGER=two']
NEW = [b'=D:=new', b'B=changed', b'C=3']


def block(items):
    return b'\0'.join(items) + (b'\0\0' if items else b'\0')


def make(label):
    c = base(ENTRY, label)
    c['id'] = c['id'].replace('c7-startup/', 'c7-joint-env-init/')
    c.update(compare_x87_values=True, compare_x87_control=True, initial_fpcw=0x27f,
             initial_x87=['0000000000000080ff3f', '000000000000008000c0'])
    items = [] if label in ('empty', 'empty-lazy-mb') else [b'=C:=x', b'=D:=y'] if label == 'only-drive-entries' else OLD
    c['stores'] += [(TEXT, block(items)), (WIDE, block(NEW))]
    put(c, MB, 0 if label == 'empty-lazy-mb' else 0x100)
    vector_failure = label == 'vector-allocation-failure'
    string_failure = label == 'string-allocation-failure'
    reload = label == 'reload-block-after-vector-allocation'
    used_items = NEW if reload else items
    visible = [s for s in used_items if not s.startswith(b'=')]
    pointers = [COPY + i * 64 for i in range(len(visible))]
    malloc = child('allocate', 0x561b68, 1)
    malloc['clobber_ecx'] = 0xdead1122
    malloc['clobber_edx'] = 0xbeef3344
    malloc['observe'] = [(ENV, 4), (ENVV, 4), (READY, 4), (HEAP, 12)]
    malloc['responses'] = [dict(value=0 if vector_failure else HEAP)]
    if reload:
        malloc['responses'][0]['mutations'] = [(ENV, 4, WIDE)]
    if not vector_failure:
        malloc['responses'] += [dict(value=0 if string_failure and i == 0 else at)
                                for i, at in enumerate(pointers[:1] if string_failure else pointers)]
    eps = [malloc]
    counts = {'allocate': len(malloc['responses'])}
    args = {'allocate': [[sum(not s.startswith(b'=') for s in items) * 4 + 4]]}
    if not vector_failure:
        args['allocate'] += [[len(s) + 1] for s in (visible[:1] if string_failure else visible)]
    final = [(TEXT, block(items).hex())]
    if vector_failure or string_failure:
        fatal = child('fatal-error', 0x5630e0, 1)
        fatal.update(result_kind='void', noreturn=True,
                     observe=[(ENV, 4), (ENVV, 4), (READY, 4), (HEAP, 12)])
        eps.append(fatal)
        counts['fatal-error'] = 1
        args['fatal-error'] = [[9]]
        c['noreturn_endpoint'] = 'fatal-error'
        final += [(ENVV, struct.pack('<I', 0 if vector_failure else HEAP).hex()),
                  (READY, struct.pack('<I', 0xaabbcc00).hex()),
                  (ENV, struct.pack('<I', TEXT).hex())]
        if string_failure:
            final.append((HEAP, '00000000'))
    else:
        release = child('release', 0x561bdc, 1)
        release.update(result_kind='void', observe=[(ENV, 4), (ENVV, 4), (READY, 4), (HEAP, 12)])
        eps.append(release)
        counts['release'] = 1
        args['release'] = [[WIDE if reload else TEXT]]
        final += [(ENV, '00000000'), (ENVV, struct.pack('<I', HEAP).hex()),
                  (READY, '01000000'),
                  (HEAP, b''.join(struct.pack('<I', x) for x in pointers + [0]).hex())]
        final += [(at, (s + b'\0').hex()) for at, s in zip(pointers, visible)]
        for at, s in zip(pointers, visible):
            final += [(at + len(s) + 1, 'a5' * (63 - len(s)))]
    if label == 'empty-lazy-mb':
        codepage = child('set-codepage', 0x569831, 1, 0x11223344)
        codepage.update(clobber_ecx=0xdead5566, clobber_edx=0xbeef7788)
        eps.append(codepage)
        counts['set-codepage'] = 1
        args['set-codepage'] = [[0xfffffffd]]
        final.append((MB, '01000000'))
    else:
        final.append((MB, '00010000'))
    c.update(endpoints=eps, expected_endpoint_counts=counts,
             expected_endpoint_arguments=args, authored_final_memory=final)
    return c


def definitions(registry):
    if not any(int(m['address'], 16) == ENTRY for m in registry['members']):
        return []
    return [make(label) for label in ['empty', 'empty-lazy-mb', 'only-drive-entries',
        'mixed-real-copy', 'reload-block-after-vector-allocation',
        'vector-allocation-failure', 'string-allocation-failure']]
