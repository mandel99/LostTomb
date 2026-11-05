"""Pure, fresh cohort3 CRT formatter case recipes; this module executes no image.

Only ``definitions(registry)`` is public. All arguments are stack DWORDs, including
the two little-endian DWORDs of 568D88's single uint64_t argument. Real internal
children execute. Void routines deliberately use result_mask=None: their EAX is
diagnostic, while complete output bytes and nonvolatile/cleanup contracts apply.
"""
import struct

SCRATCH = 0x32000000
VALUE = SCRATCH + 0x800
BUFFER = SCRATCH + 0x1000
DESCRIPTOR = SCRATCH + 0x2000
DIGITS = SCRATCH + 0x2100
OUTPUT = SCRATCH + 0x3000
LEFT = SCRATCH + 0x4000
RIGHT = SCRATCH + 0x4100
CONTROL = 0x1266420
STATIC_DIGITS = 0x12666A8
STATIC_DESCRIPTOR = 0x12666C8
POINT = 0x604E68
TABLE = 0x605438
ENTRIES = (0x564263, 0x564367, 0x564445, 0x5644E0, 0x564507,
           0x56452A, 0x568D88, 0x56A47E, 0x56A904, 0x56AB24)


def _u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def _bits(value):
    return struct.unpack('<Q', struct.pack('<d', value))[0]


def _raw96(significand, exponent):
    return significand.to_bytes(10, 'little') + struct.pack('<H', exponent)


def _case(entry, label, arguments, stores, watched, writes, result=0xFFFFFFFF,
          expected=None, flags=0x202):
    out = dict(id=f'formatter/{entry:08x}/{label}', entry=entry,
               receiver=0xA1B2C3D4, arguments=arguments,
               named_arguments=list(arguments), eflags=flags, stores=stores,
               watched=watched, write_ranges=writes, result_mask=result,
               native_cleanup=0, named_cleanup=0, canonical_cleanup=0,
               canonical_registers_and_flags=False)
    if expected is not None:
        out['expected_eax'] = expected
    return out


def _control(reuse=0, exponent=0x13579BDF, rounded=0x7E, point=b'.'):
    # Each primitive width is intentional; gaps and adjacent bytes stay canaries.
    return [(CONTROL - 0x20, b'\xA5' * 0x60),
            (CONTROL, _u32(DESCRIPTOR)), (CONTROL + 4, bytes([reuse])),
            (CONTROL + 8, _u32(exponent)), (CONTROL + 12, bytes([rounded])),
            (POINT - 8, b'\xA5' * 17), (POINT, point)]


def _static():
    # Conversion may alter the 28-byte raw decimal structure and 16-byte result,
    # but neither the four-byte gap nor surrounding bytes are writable.
    return [(STATIC_DIGITS - 0x20, b'\xA5' * 0x70)]


def _float(entry, label, bits, precision, capitals=0, mode=None, point=b'.',
           alignment=0, flags=0x202):
    buffer = BUFFER + alignment
    args = [VALUE, buffer]
    if entry == 0x56452A:
        args += [ord(mode), precision, capitals]
    else:
        args += [precision]
        if entry != 0x564367:
            args += [capitals]
    general = entry == 0x564445 or (entry == 0x56452A and mode not in 'eEf')
    stores = (_control(point=point) + _static() +
              [(VALUE - 16, b'\xA5' * 40), (VALUE, struct.pack('<Q', bits)),
               (BUFFER - 32, b'\xA5' * 0x440)])
    watched = [(VALUE - 16, 40), (BUFFER - 32, 0x440),
               (CONTROL - 0x20, 0x60), (STATIC_DIGITS - 0x20, 0x70),
               (POINT - 8, 17)]
    writes = [(buffer, 0x400), (STATIC_DIGITS, 28), (STATIC_DESCRIPTOR, 16)]
    if general:
        writes += [(CONTROL, 4), (CONTROL + 4, 1),
                   (CONTROL + 8, 4), (CONTROL + 12, 1)]
    return _case(entry, label, args, stores, watched, writes,
                 expected=buffer, flags=flags)


def _reuse(entry, label, exponent, sign, text, precision, capitals=0,
           descriptor_digits=None, saved_exponent=None, initial_reuse=0x7F,
           point=b'.', alignment=0):
    buffer = BUFFER + alignment
    if descriptor_digits is None:
        descriptor_digits = text
    if saved_exponent is None:
        saved_exponent = exponent - 1
    args = [VALUE, buffer, precision]
    if entry in (0x564263, 0x5644E0):
        args += [capitals]
    payload = (b'?' if sign == ord('-') else b'') + text + b'\0'
    stores = (_control(initial_reuse, saved_exponent, point=point) + _static() +
              [(VALUE - 16, b'\xA5' * 40),
               (VALUE, struct.pack('<Q', 0x7FF0000000000001)),
               (DESCRIPTOR - 16, b'\xA5' * 64),
               (DESCRIPTOR, struct.pack('<iiII', sign, exponent, 0x87654321, DIGITS)),
               (DIGITS - 16, b'\xA5' * 96),
               (DIGITS, descriptor_digits + b'\0'),
               (BUFFER - 32, b'\xA5' * 0x440), (buffer, payload)])
    watched = [(VALUE - 16, 40), (BUFFER - 32, 0x440),
               (DESCRIPTOR - 16, 64), (DIGITS - 16, 96),
               (CONTROL - 0x20, 0x60), (STATIC_DIGITS - 0x20, 0x70),
               (POINT - 8, 17)]
    writes = [(buffer, 0x400)]
    if entry in (0x5644E0, 0x564507):
        writes += [(CONTROL + 4, 1)]
    return _case(entry, 'reuse-' + label, args, stores, watched, writes,
                 expected=buffer)


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    missing = set(ENTRIES) - present
    assert not missing, f'Missing formatter members: {sorted(missing)}'
    cases = []

    # Direct scientific/fixed/general paths, including actual nested conversion.
    scientific = [
        ('zero-p0', 0, 0, 0), ('negative-zero', 0x8000000000000000, 4, 1),
        ('negative', _bits(-12.345), 3, 0), ('half-up', _bits(1.25), 1, 1),
        ('carry-exponent', _bits(9.9995), 2, 0),
        ('tiny-exponent', _bits(1.0e-100), 7, 1),
        ('large-exponent', _bits(1.0e100), 7, 0),
        ('min-subnormal', 1, 17, 1), ('max-finite', 0x7FEFFFFFFFFFFFFF, 17, 0),
        ('infinity', 0x7FF0000000000000, 6, 0),
        ('negative-infinity', 0xFFF0000000000000, 6, 1),
        ('quiet-nan', 0x7FF8123456789ABC, 8, 0),
        ('signaling-nan-spill', 0x7FF0000000000001, 8, 1),
    ]
    fixed = [
        ('zero-p0', 0, 0), ('negative-zero', 0x8000000000000000, 4),
        ('negative', _bits(-12.345), 3), ('half-up', _bits(1.25), 1),
        ('carry-integer', _bits(99.995), 2), ('round-to-zero', _bits(0.0001), 2),
        ('negative-leading-zeros', _bits(-0.0012345), 7),
        ('leading-zeros-cut', _bits(0.0012345), 2),
        ('integer-trailing-zeros', _bits(1234.0), 5),
        ('max-finite', 0x7FEFFFFFFFFFFFFF, 2),
        ('min-subnormal-p340', 1, 340),
        ('infinity', 0x7FF0000000000000, 3),
        ('quiet-nan', 0xFFF8123456789ABC, 4),
    ]
    general = [
        ('zero', 0, 6, 0), ('negative-zero', 0x8000000000000000, 3, 1),
        ('negative', _bits(-123.456), 5, 0),
        ('round-carry-to-scientific', _bits(999.5), 3, 1),
        ('round-carry-fixed-trim', _bits(9.999), 3, 0),
        ('exponent-minus4-fixed', _bits(0.00012345), 5, 0),
        ('exponent-minus5-scientific', _bits(0.000012345), 5, 1),
        ('exponent-at-precision', _bits(12345.0), 4, 0),
        ('exponent-below-precision', _bits(1234.5), 5, 1),
        ('precision-one-carry', _bits(9.5), 1, 0),
        ('min-subnormal', 1, 17, 1), ('max-finite', 0x7FEFFFFFFFFFFFFF, 17, 0),
        ('infinity', 0x7FF0000000000000, 6, 0),
        ('quiet-nan', 0x7FF8123456789ABC, 6, 1),
    ]
    for i, (label, bits, precision, capitals) in enumerate(scientific):
        cases.append(_float(0x564263, label, bits, precision, capitals,
                            point=b',' if i == 2 else b'.', alignment=i % 4,
                            flags=0xAD7 if i % 3 == 1 else 0x202))
    for i, (label, bits, precision) in enumerate(fixed):
        cases.append(_float(0x564367, label, bits, precision,
                            point=b',' if i == 2 else b'.', alignment=i % 4))
    for i, (label, bits, precision, capitals) in enumerate(general):
        cases.append(_float(0x564445, label, bits, precision, capitals,
                            point=b',' if i == 2 else b'.', alignment=i % 4))

    # Each dispatcher arm is executed with real child composition. Capitalization
    # is its own fifth argument; uppercase format E alone is not that argument.
    dispatch = [
        ('e-zero', 'e', 0, 0, 0), ('E-lower', 'E', _bits(-12.5), 3, 0),
        ('e-capitals', 'e', _bits(1.0e-100), 6, 1),
        ('f-round', 'f', _bits(9.995), 2, 0),
        ('f-leading-zeros', 'f', _bits(-0.0012345), 7, 1),
        ('f-max-finite', 'f', 0x7FEFFFFFFFFFFFFF, 1, 0),
        ('g-fixed', 'g', _bits(123.45), 5, 0),
        ('G-scientific', 'G', _bits(0.000012345), 5, 1),
        ('unknown-fallback', '?', _bits(999.5), 3, 1),
        ('F-fallback', 'F', _bits(1.2345), 4, 0),
        ('nan', 'g', 0x7FF8123456789ABC, 7, 0),
        ('min-subnormal', 'e', 1, 17, 1),
    ]
    for i, (label, mode, bits, precision, capitals) in enumerate(dispatch):
        cases.append(_float(0x56452A, label, bits, precision, capitals, mode,
                            point=b',' if i == 6 else b'.', alignment=i % 4))

    # Reuse inputs are the existing descriptor plus already rounded significant
    # digits that general conversion normally supplies. The poisoned double must
    # remain unused. Direct functions also exercise arbitrary nonzero reuse bytes.
    reuse_scientific = [
        ('positive', 5, ord(' '), b'123', 3, 0, b'12345'),
        ('negative', -6, ord('-'), b'12345', 5, 1, b'12345'),
        ('zero', 0, ord(' '), b'000', 3, 0, b'0'),
        ('exponent-three-digits', 309, ord(' '), b'1797693', 7, 1, b'1797693'),
        ('negative-exponent-three-digits', -323, ord('-'), b'4940656', 7, 0, b'4940656'),
        ('precision-zero', 2, ord(' '), b'1', 0, 1, b'1'),
    ]
    for entry in (0x564263, 0x5644E0):
        for i, (label, exponent, sign, text, precision, capitals, digits) in enumerate(reuse_scientific):
            cases.append(_reuse(entry, label, exponent, sign, text, precision,
                                capitals, digits, point=b',' if i == 1 else b'.',
                                alignment=i % 4))
    reuse_fixed = [
        ('positive', 3, ord(' '), b'12345', 5, 2),
        ('negative', 3, ord('-'), b'12345', 5, 2),
        ('negative-exponent', -2, ord(' '), b'12345', 5, -3),
        ('negative-sign-and-exponent', -2, ord('-'), b'12345', 5, -3),
        ('zero', 0, ord(' '), b'000', 3, -1),
        ('saved-exponent-equals-precision', 6, ord(' '), b'12345', 5, 5),
        ('precision-zero', 1, ord(' '), b'1', 0, 0),
    ]
    for entry in (0x564367, 0x564507):
        for i, (label, exponent, sign, text, precision, saved_exponent) in enumerate(reuse_fixed):
            cases.append(_reuse(entry, label, exponent, sign, text, precision,
                                saved_exponent=saved_exponent,
                                point=b',' if i == 1 else b'.', alignment=i % 4))

    # Integer-bit input conversion never routes through host Python formatting.
    binary64 = [
        ('positive-zero', 0), ('negative-zero', 0x8000000000000000),
        ('one', 0x3FF0000000000000), ('negative-one', 0xBFF0000000000000),
        ('one-tenth', 0x3FB999999999999A), ('half', 0x3FE0000000000000),
        ('one-minus-ulp', 0x3FEFFFFFFFFFFFFF), ('one-plus-ulp', 0x3FF0000000000001),
        ('min-subnormal', 1), ('max-subnormal', 0x000FFFFFFFFFFFFF),
        ('min-normal', 0x0010000000000000),
        ('negative-min-subnormal', 0x8000000000000001),
        ('max-finite', 0x7FEFFFFFFFFFFFFF), ('negative-max-finite', 0xFFEFFFFFFFFFFFFF),
        ('positive-infinity', 0x7FF0000000000000), ('negative-infinity', 0xFFF0000000000000),
        ('quiet-nan', 0x7FF8000000000000), ('negative-indefinite', 0xFFF8000000000000),
        ('quiet-nan-payload', 0x7FF8123456789ABC), ('signaling-nan', 0x7FF0000000000001),
    ]
    for i, (label, bits) in enumerate(binary64):
        cases.append(_case(0x568D88, label, [bits & 0xFFFFFFFF, bits >> 32],
                           _control() + _static(),
                           [(CONTROL - 0x20, 0x60), (STATIC_DIGITS - 0x20, 0x70),
                            (TABLE, 0x400)],
                           [(STATIC_DIGITS, 28), (STATIC_DESCRIPTOR, 16)],
                           expected=STATIC_DESCRIPTOR, flags=0xAD7 if i % 2 else 0x202))

    # Native raw80 input is exactly low DWORD, high DWORD, sign/exponent WORD;
    # selected high padding bits demonstrate they are ignored, not API state.
    raw80 = [
        ('zero', 0, 0, 0, 17, 0), ('negative-zero', 0, 0, 0x8000, 17, 0),
        ('one', 0, 0x80000000, 0x3FFF, 17, 0),
        ('negative-one-pad', 0, 0x80000000, 0xA5B6BFFF, 17, 0),
        ('one-pad', 0, 0x80000000, 0xA5B63FFF, 17, 0),
        ('one-precision0', 0, 0x80000000, 0x3FFF, 0, 0),
        ('one-precision1', 0, 0x80000000, 0x3FFF, 1, 0),
        ('one-precision21', 0, 0x80000000, 0x3FFF, 21, 0),
        ('one-precision-clamp', 0, 0x80000000, 0x3FFF, 27, 0),
        ('half', 0, 0x80000000, 0x3FFE, 17, 0),
        ('one-quarter', 0, 0x80000000, 0x3FFD, 17, 0),
        ('half-round-digit', 0, 0xA0000000, 0x3FFF, 2, 0),
        ('carry-nine', 0, 0x98000000, 0x4002, 1, 0),
        ('ten', 0, 0xA0000000, 0x4002, 17, 0),
        ('one-plus-lsb', 1, 0x80000000, 0x3FFF, 21, 0),
        ('max-significand', 0xFFFFFFFF, 0xFFFFFFFF, 0x3FFF, 21, 0),
        ('min-denormal', 1, 0, 0, 17, 0),
        ('min-normal', 0, 0x80000000, 1, 17, 0),
        ('max-finite', 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFE, 21, 0),
        ('negative-max-finite', 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFE, 21, 0),
        ('fixed-one-p0', 0, 0x80000000, 0x3FFF, 0, 1),
        ('fixed-one-p2', 0, 0x80000000, 0x3FFF, 2, 1),
        ('fixed-small-zero', 0, 0x80000000, 0x3FF0, 2, 1),
        ('fixed-small-nonzero', 0, 0x80000000, 0x3FF0, 8, 1),
        ('fixed-flags-other-bits', 0, 0xA0000000, 0x3FFF, 2, 0xA5A50001),
        ('ignored-flags', 0, 0xA0000000, 0x3FFF, 2, 0xA5A50000),
        ('infinity', 0, 0x80000000, 0x7FFF, 17, 0),
        ('negative-infinity', 0, 0x80000000, 0xFFFF, 17, 0),
        ('quiet-nan', 0, 0xC0000000, 0x7FFF, 17, 0),
        ('indefinite', 0, 0xC0000000, 0xFFFF, 17, 0),
        ('quiet-nan-payload', 0x12345678, 0xC1234567, 0x7FFF, 17, 0),
        ('signaling-nan', 1, 0x80000000, 0x7FFF, 17, 0),
        ('unsupported', 0x12345678, 0x01234567, 0x7FFF, 17, 0),
    ]
    for i, (label, low, high, sign_exponent, requested, flags) in enumerate(raw80):
        output = OUTPUT + i % 4
        args = [low, high, sign_exponent, requested, flags, output]
        cases.append(_case(0x56A47E, label, args,
                           [(OUTPUT - 32, b'\xA5' * 128)],
                           [(OUTPUT - 32, 128), (TABLE, 0x400)], [(output, 28)],
                           expected=0 if sign_exponent & 0x7FFF == 0x7FFF else 1,
                           flags=0xAD7 if i % 2 else 0x202))

    one = 1 << 79
    multiply = [
        ('one-by-one', one, 0x3FFF, one, 0x3FFF),
        ('negative-left', one, 0xBFFF, one, 0x3FFF),
        ('negative-right', one, 0x3FFF, one, 0xBFFF),
        ('two-negatives', one, 0xBFFF, one, 0xBFFF),
        ('left-zero', 0, 0, one, 0x4000),
        ('left-negative-zero', 0, 0x8000, one, 0x4000),
        ('right-zero', one, 0x4000, 0, 0),
        ('right-negative-zero', one, 0x4000, 0, 0x8000),
        ('immediate-underflow', one, 1, one, 0x3FBE),
        ('subnormal-sticky-63', one + 1, 1, one + (1 << 32), 0x3FBF),
        ('subnormal-sticky-1', one + 1, 1, one + (1 << 32), 0x3FFD),
        ('denormal-left', one - 1, 0, one, 0x4000),
        ('denormal-right', one, 0x4000, one - 1, 0),
        ('normalization', 1 << 78, 0x4000, one, 0x3FFF),
        ('tie-even', one + (1 << 32), 0x3FFF, one + (0x4000 << 32), 0x3FFF),
        ('tie-odd', one + (1 << 32), 0x3FFF, one + (0xC000 << 32), 0x3FFF),
        ('above-half', one + (1 << 32), 0x3FFF, one + (0x4001 << 32), 0x3FFF),
        ('below-half', one + (1 << 32), 0x3FFF, one + (0x3FFF << 32), 0x3FFF),
        ('round-carry-dword', one + (1 << 32), 0x3FFF, 0x800000004001FFFFFFFF, 0x3FFF),
        ('round-carry-two-dwords', one + (1 << 32), 0x3FFF, 0x8000FFFFFFFEFFFE0000, 0x3FFF),
        ('round-carry-exponent', one + (1 << 32), 0x3FFF, 0xFFFFFFFFFFFE00000000, 0x3FFF),
        ('all-ones-diagonals', (1 << 80) - 1, 0x3FFF, (1 << 80) - 1, 0x3FFF),
        ('overflow', one, 0x7FFE, one, 0x7FFE),
        ('negative-overflow', one, 0xFFFE, one, 0x7FFE),
        ('exceptional-left', one, 0x7FFF, one, 0x3FFF),
        ('exceptional-right', one, 0x3FFF, one, 0xFFFF),
    ]
    for i, (label, left, left_exp, right, right_exp) in enumerate(multiply):
        cases.append(_case(0x56A904, label, [LEFT, RIGHT],
                           [(LEFT - 16, b'\xA5' * 44), (LEFT, _raw96(left, left_exp)),
                            (RIGHT - 16, b'\xA5' * 44), (RIGHT, _raw96(right, right_exp))],
                           [(LEFT - 16, 44), (RIGHT - 16, 44)], [(LEFT, 12)],
                           result=None, flags=0xAD7 if i % 2 else 0x202))
    cases.append(_case(0x56A904, 'aliased-square', [LEFT, LEFT],
                       [(LEFT - 16, b'\xA5' * 44),
                        (LEFT, _raw96(one + (0x4001 << 32), 0x3FFF))],
                       [(LEFT - 16, 44)], [(LEFT, 12)], result=None))

    # Magnitudes stay inside the original finite power table. INT_MIN's SAR loop
    # and powers outside that table are explicitly unresolved domains.
    scales = [
        ('zero-preserve', 0, 1, one + 0xBEEF, 0x3FFF),
        ('zero-clear-mode-noop', 0, 0, one + 0xBEEF, 0x3FFF),
        ('ten-preserve', 1, 1, one + 0xBEEF, 0x3FFF),
        ('ten-clear-tail', 1, 0, one + 0xBEEF, 0x3FFF),
        ('tenth-preserve', -1, 1, one + 0xBEEF, 0x3FFF),
        ('tenth-clear-tail', -1, 0, one + 0xBEEF, 0x3FFF),
        ('table-digit7', 7, 1, one, 0x3FFF),
        ('table-digit8', 8, 1, one, 0x3FFF),
        ('table-two-digits', 9, 1, one, 0x3FFF),
        ('table-digit63', 63, 1, one + 1, 0x3FFF),
        ('table-digit64', 64, 1, one + 1, 0x3FFF),
        ('negative-digit7', -7, 1, one, 0x3FFF),
        ('negative-digit8', -8, 1, one, 0x3FFF),
        ('negative-two-digits', -9, 1, one, 0x3FFF),
        ('negative-digit63', -63, 1, one + 1, 0x3FFF),
        ('negative-digit64', -64, 1, one + 1, 0x3FFF),
        ('binary64-large', 308, 1, one, 0x3FFF),
        ('binary64-small', -324, 1, one, 0x3FFF),
        ('fourth-table-level', 512, 1, one, 0x3FFF),
        ('negative-fourth-level', -512, 1, one, 0x3FFF),
        ('fifth-table-level', 4096, 1, one, 0x3FFF),
        ('negative-fifth-level', -4096, 1, one, 0x3FFF),
        ('large-finite', 4932, 1, one, 0x3FFF),
        ('small-subnormal', -4932, 1, one, 0x3FFF),
        ('negative-value', 28, -1, one + 0xCAFE, 0xBFFF),
        ('zero-value', -28, 0, 0, 0),
        ('overflow', 308, 1, one, 0x7FFE),
        ('underflow', -308, 1, one, 1),
    ]
    for i, (label, power, preserve, significand, exponent) in enumerate(scales):
        writes = [] if power == 0 else [(LEFT, 12)]
        cases.append(_case(0x56AB24, label, [LEFT, power, preserve],
                           [(LEFT - 16, b'\xA5' * 44),
                            (LEFT, _raw96(significand, exponent))],
                           [(LEFT - 16, 44), (TABLE, 0x400)], writes,
                           result=None, flags=0xAD7 if i % 2 else 0x202))

    assert {case['entry'] for case in cases} == set(ENTRIES)
    assert len({case['id'] for case in cases}) == len(cases)
    return cases
