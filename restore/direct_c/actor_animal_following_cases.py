"""Ported scenario definitions only; no imported candidate result or binary.

These functions merely enumerate inputs. Execution belongs to the current main
PE verifier. The originating source digests describe the definition port, not
proofs for any future binary.
"""
import itertools
S=0x32000000
terrain_deltas=(-228,-227,1,229,228,227,-1,-229)
SIBLING_ARITY={0x53b4a0:3,0x4b17d0:1,0x4b5050:1,0x4b1d50:1,0x4ecd40:1,0x4a5da0:2,0x4b3250:1,0x4eab10:6,0x4eaec0:1}
arity=SIBLING_ARITY
def advance(value):
    for _ in range(31):value=(value>>1)|((((value>>4)^value)&1)<<30)
    return value

SOURCE_SCENARIO_SHA256={'siblings/verify_candidate.py': '57c1c8b314d335e9265c7c30428de81614a2d56d14639331bad430ddffddad1f', 'siblings/verify_coverage_additions.py': '8cc0746697c29ccc7236dd7d059df07f1ef097313aae3f2866d2d203f495247f', 'siblings/verify_aliases.py': '051a861ebfa523217c0877a5336e39f91fade1dfc5e29aa6d646d13e452255c3', 'transition/verify_kind_cue.py': 'ec5133ca6efc5f7f2841426b8521b776850fff464f2dac8555277dce26d82850', 'crocodile_route/verify_candidate.py': '8ac6e836fe162cc96924dc2857f5372982628e7efdd2b1cb6e31274c90c63528', 'crocodile_route/verify_aliases.py': '95f4384a759c5a29a89c5083d4d0e4a6e49c3b700ff67ef48e8117567648f753', 'state_initializer/verify_candidate.py': '96e6925cf6f4f6e74ee392eb6ed29df0989139808b51f5625c1cef2561bb6f45', 'state_initializer/verify_aliases.py': '3915be31be7be8d9d4be7d46b089086b7b9276c84ff5b8b8910c2ab69e27cdf1'}

def sibling_baseline(exercise):
    for kind in [82, 83]:
        for state in range(256):
            exercise(kind, state=state)
        for (state, terrain, counter, direction) in itertools.product([4, 5, 8, 9, 10, 12, 13, 18, 19, 20, 21, 23, 24], [0, 4], [0, 10, 11], [0, 8, 9, 10]):
            exercise(kind, state=state, terrain=terrain, counter=counter, direction=direction)
        for (state, delay) in itertools.product([4, 12, 18, 19, 21, 24], [0, 1, 2, 6, 8, 18, 23, 500, 501, 32767, 32768, 65535]):
            exercise(kind, state=state, delay=delay, counter=5)
        for (state, results) in itertools.product([18, 23], [[0], [15], [-1], [16], [-1, 0], [-1, 15], [-1, -1], [2147483648], [2147483647]]):
            exercise(kind, state=state, delay=0, results={5155600: results})
        for (history, a, b) in itertools.product([0, 98, 99, 100, 110, 111, 4294967295], [0, 1, 4294967295, 305419896], [0, 4294967295]):
            exercise(kind, state=23, history=history, first=a, second=b, results={5155600: [-1, -1]})
        for state in [8, 9, 10]:
            for (a, b, history) in itertools.product([0, 1, 4294967295, 305419896], [0, 4294967295], [0, 99, 100, 110, 111, 4294967295]):
                exercise(kind, state=state, direction=8, history=history, first=a, second=b)
        for (speed, rotation) in itertools.product([0, 1, 2147483647, 2147483648, 4294967295], [0, 7, 255, 4294967295]):
            exercise(kind, state=19, speed=speed, rotation=rotation)
        for (counter, cycle) in itertools.product([0, 5, 10, 11, 12, 2147483647, 2147483648, 4294967295], [0, 1, 2, 11, 12, 2147483648, 4294967295]):
            exercise(kind, state=21, counter=counter, cycle=cycle)
        for state in [5, 4, 8, 9, 12, 13, 18, 19, 20, 21, 23]:
            for token in arity:
                exercise(kind, state=state, delay=0, counter=11, direction=8, child_mutations={(token, 1): [(13653752, 3)]}, child_bytes={(token, 1): [(12375600 + 3 * 388, 8), (12375561 + 3 * 388, 10)]}, results={5155600: [-1, 0]})
        exercise(kind, animation=11)
        for group in [-1, 0, 1, 49]:
            exercise(kind, state=23, group=group, results={5155600: [-1, -1]})

def sibling_additions(exercise):
    exercise(82, state=18, delay=0, history=99)
    for kind in [82, 83]:
        for counter in [0, 11]:
            for (tx, ty) in [(20, 31), (30, 20)]:
                exercise(kind, state=21, delay=5, counter=counter, direction=0, target_x=tx, target_y=ty)
        exercise(kind, state=21, delay=5, counter=0, direction=0, linked=0)
    exercise(82, state=21, delay=5, direction=8, linked=0)
    exercise(82, state=21, delay=5, direction=8, target_x=20, child_mutations={(4873632, 1): [(12375564 + 2 * 388, 3342386)]})
    exercise(82, state=21, delay=5, direction=8, target_x=20, child_mutations={(4873632, 1): [(12375566 + 2 * 388, 55)]})
    exercise(82, state=8, child_bytes={(5164352, 1): [(12375600 + 388, 255)]})
    exercise(82, state=4, terrain=0, delay=8)
    tile = 1000 + 11 * 228 + 10
    for delta in [-228, 1, 228, -1, -227, -229, 229, 227]:
        exercise(82, stores=[(11491724 + (tile + delta) * 4, 0)])
    exercise(83, state=8, history=98)
    exercise(83, state=18, delay=0, history=99)
    exercise(83, state=9, direction=8, history=98)
    exercise(83, state=4, child_bytes={(4922704, 1): [(12375600 + 388, 6)]})
    for kind in [82, 83]:
        for state in [4, 5, 6, 8, 9, 12, 13, 18, 19, 20, 21, 23, 24]:
            exercise(kind, state=state)
    exercise(82, state=21, delay=0)
    exercise(83, state=4, child_bytes={(4922704, 1): [(12375600 + 388, 6)]})

def sibling_aliases(exercise):
    inverse = pow(97, -1, 1 << 30)

    def actor_alias(field, target):
        assert (target - field) % 4 == 0
        return (target - field) // 4 * inverse % (1 << 30)
    for kind in [82, 83]:
        exercise(kind, actor=actor_alias(12375872, 7377132 + kind * 56), state=9, speed=12, direction=0)
        exercise(kind, actor=actor_alias(12375580, 13873604), state=23, stores=[(13873604, 0)], results={5155600: [0]})
        group_alias = actor_alias(12375580, 12368388 + 144)
        exercise(kind, actor=group_alias, state=23, stores=[(12375576 + group_alias * 388 & 4294967295, 1000 + 11 * 228 + 10)], results={5155600: [0]})
        vector_y = 6084988 if kind == 82 else 6085500
        exercise(kind, actor=actor_alias(12375580, vector_y), state=23, results={5155600: [0]})
        exercise(kind, state=23, read_mutations={12375640 + 388: [(1, [(12375545 + 388, 255)])]})
        exercise(kind, state=23, child_mutations={(5155600, 1): [(13653752, 3), (12368386 + 144, 3407923)], (4928080, 1): [(13653752, 2)]}, results={5155600: [0]})
        exercise(kind, state=13, counter=11, child_mutations={(4873632, 1): [(13653752, 3)], (5156544, 1): [(13653752, 2)]}, child_bytes={(4873632, 1): [(12375561 + 3 * 388, 8)], (5156544, 1): [(12375561 + 2 * 388, 9)]})
        exercise(kind, state=5, child_mutations={(5485728, 1): [(13653752, 3)], (4921296, 1): [(13653752, 2)]})
        exercise(kind, state=21, counter=11, target_x=20, child_mutations={(4928080, 1): [(12375564 + 2 * 388, 1441813)]})
        exercise(kind, state=18, delay=0, read_mutations={12375566 + 388: [(1, [(13874036, 2882400001), (13874024, 2882400002)])], 12375564 + 388: [(1, [(13874032, 2882400003)])]})
    tile = 1000 + 11 * 228 + 10
    last_terrain = 11491724 + (tile + 227) * 4
    exercise(82, state=18, delay=0, read_mutations={last_terrain: [(1, [(12375872 + 388, 99)])]})
    exercise(82, state=18, delay=0, counter=6, read_mutations={last_terrain: [(1, [(12375868 + 388, 5)])]})
    for state in [8, 23]:
        exercise(83, actor=actor_alias(12375868, 13873604), state=state)
    for state in [8, 9]:
        exercise(83, actor=actor_alias(12375561, 13874037), state=state, direction=8)
        exercise(83, actor=actor_alias(12375600, 13873604), state=state, direction=8)
    exercise(83, state=21, counter=0, delay=5, target_x=20, read_mutations={12375564 + 2 * 388: [(1, [(13874036, 2882400001)])]})
    exercise(83, state=23, results={5155600: [-1]}, child_mutations={(5155600, 1): [(13653752, 3)]})

def cue_cases(exercise):
    exercise(kind=26)
    exercise(kind=0)
    exercise(kind=83)
    exercise(kind=26, control_rng=True, control_sound=True, random_result=4294967295)
    for (kind, initial) in itertools.product(range(256), [0, 1, 4294967295]):
        exercise(kind=kind, seed=initial)
    for kind in range(256):
        exercise(kind=kind, control_rng=True, control_sound=True, random_result=32767)
    boundaries = sorted(set([0, 1, 32767, 32768, 4294967295, 4294967294, 2147483647, 2147483648, 268435456, 536870911, 536870912, 1073741823, 1073741824] + [n + delta for n in range(4096, 32769, 4096) for delta in [-1, 0, 1]]))
    for (kind, sample) in itertools.product([0, 26, 105], boundaries):
        exercise(kind=kind, control_rng=True, control_sound=True, random_result=sample)
    for (actor, kind) in itertools.product([0, 1, 1999, 2000, -1, -2000, 2147483647, 2147483648, 1073741825, 4294967295], [0, 26, 77, 83]):
        exercise(actor=actor, kind=kind)
    for kind in [0, 26, 77, 83]:
        exercise(kind=kind, sound_gate=1, sound_option=0)
    for (kind, result) in itertools.product([0, 26, 77, 83], [0, 1, 256, 4294967295, 2147483648]):
        exercise(kind=kind, control_sound=True, sound_result=result)
    for kind in [0, 26]:
        at = 12375554 + 388
        exercise(kind=kind, control_rng=True, control_sound=True, random_result=32767, rng_mutations=[(at, 1, 83), (S + 16, 4, 305419896)])
        exercise(kind=kind, seed_read_mutations=[(at, 1, 83), (S + 20, 4, 2864434397)])
    inverse = pow(97, -1, 1 << 30)
    actor = (6310226 - 12375554 & 4294967295) // 4 * inverse % (1 << 30)
    for kind in [0, 26, 77, 83]:
        exercise(actor=actor, kind=kind, seed=305419896, synthetic=True)

def weighted_baseline(exercise):
    seeds = {}
    value = 439041101
    while len(seeds) < 100:
        value = value * 1664525 + 1013904223 & 4294967295
        seeds.setdefault((advance(value) & 32767) % 100, value)
    for (state, sample) in itertools.product([8, 19, 0], range(100)):
        exercise(state=state, a=seeds[sample], b=seeds[99 - sample], label='all100 percentages')
    for state in range(256):
        exercise(state=state, a=seeds[40], b=4294967295, label='all entry state bytes')
    for (a, b) in itertools.product([0, 1, 2, 16, 17, 2147483648, 2147483647, 4294967295, 305419896], repeat=2):
        exercise(state=19, a=a, b=b, label='RNG state bits')
    for (state, history) in itertools.product([8, 19, 0], [0, 98, 99, 100, 103, 107, 108, 109, 110, 111, 4294967295, 2147483646, 2147483647, 2147483648, 1073741824]):
        exercise(state=state, history=history, previous=2309737967, a=seeds[50], label='history signed wrap and alias')
    for (actor, state, result) in itertools.product([0, 1, 100, 1073741825, 2147483649, 4294967295], [8, 19, 0], [0, 1, 2147483647, 2147483648, 4294967295]):
        exercise(actor=actor, state=state, result=result, child_mutations=[(13653752, 3, 4), (13874052, 3735928559, 4)], label='fullDWORD arguments and child EAX')
    for state in [8, 19, 0]:
        exercise(state=state, a=seeds[99], table=[2147483667, 1985229336, 4294967295], label='dynamic fullDWORD table')
        exercise(state=state, a=seeds[0], table=[4294967295, 305397778, 41], label='dynamic fullDWORD table')
    return seeds

def weighted_aliases(exercise):
    seeds = {}
    x = 439041101
    while len(seeds) < 100:
        x = x * 1664525 + 1013904223 & 4294967295
        seeds.setdefault((advance(x) & 32767) % 100, x)

    def actor_for(address):
        difference = address - 12375600 & 4294967295
        assert difference % 4 == 0
        actor = difference // 4 * pow(97, -1, 1 << 30) & 1073741823
        assert 12375600 + actor * 388 & 4294967295 == address
        return actor

    def witness(label, selected=None, expected_memory=(), **cfg):
        exercise(label=label, selected=selected, expected_memory=expected_memory, **cfg)
    witness('actor state aliases history index:8 cached before index becomes9', 19, actor=actor_for(13873604), history=8, state=8, a=seeds[40])
    witness('actor state aliases history index:19 cached before index becomes20', 19, actor=actor_for(13873604), history=19, state=19, a=seeds[50])
    witness('actor state aliases A:history111 overwrites it before fresh A load', 19, actor=actor_for(13874052), history=111, state=8, a=0, previous=seeds[40])
    witness('actor state aliases B:history110 overwrites it after weights cached', 19, actor=actor_for(13874048), history=110, state=8, a=seeds[40], previous=286331153)
    witness('actor state aliases old sample:weights cached before new sample publication', 18, actor=actor_for(13874036), history=0, state=8, a=seeds[60], previous=8)
    witness('actor state aliases history slot0:old8 weights survive history write19', 18, actor=actor_for(13873608), history=0, state=8, a=seeds[60], previous=19)
    witness('history index read precedes old sample read', 19, a=seeds[40], history=0, read_mutations={13873604: [(1, [(13874036, 3735928559, 4)])]}, expected_memory=[(13873608, 3735928559)])
    witness('history index cached before old sample read mutates it', 19, a=seeds[40], history=0, read_mutations={13874036: [(1, [(13873604, 111, 4)])]}, expected_memory=[(13873604, 1)])
    witness('history store cannot change cached entry weights', 18, a=seeds[60], state=8, history=0, write_mutations={13873608: [(1, [(12375600 + 388, 19, 1)])]})
    witness('history store precedes fresh A load', 19, a=seeds[60], state=8, history=0, write_mutations={13873608: [(1, [(13874052, seeds[40], 4)])]})
    witness('history index publication precedes fresh B load', 19, a=seeds[40], b=0, history=0, write_mutations={13873604: [(1, [(13874048, 305419896, 4)])]}, expected_memory=[(13874048, advance(305419896))])
    witness('A is loaded before B and B observes mutation at A read', 19, a=seeds[40], b=0, read_mutations={13874052: [(1, [(13874048, 2309737967, 4)])]}, expected_memory=[(13874048, advance(2309737967))])
    witness('A remains cached when B read changes its global', 19, a=seeds[40], read_mutations={13874048: [(1, [(13874052, seeds[60], 4)])]}, expected_memory=[(13874052, advance(seeds[40]))])
    witness('B remains cached across A publication', 19, a=seeds[40], b=0, write_mutations={13874052: [(1, [(13874048, 2309737967, 4)])]}, expected_memory=[(13874048, 0)])
    witness('sample A remains cached when B publication changes A global', 19, a=seeds[40], write_mutations={13874048: [(1, [(13874052, 0, 4)])]}, expected_memory=[(13874052, 0), (13874032, advance(seeds[40]) & 32767)])
    witness('A15 publication follows B32 publication', 19, a=seeds[40], write_mutations={13874048: [(1, [(13874032, 2882400001, 4)])]}, expected_memory=[(13874032, advance(seeds[40]) & 32767)])
    witness('A15 publication cannot change cached entry weights', 18, a=seeds[60], state=8, write_mutations={13874032: [(1, [(12375600 + 388, 19, 1)])]})
    witness('state table is read after final sample publication', 4276944915, a=seeds[40], state=8, write_mutations={13874036: [(1, [(6180576, 4276944915, 4)])]})
    witness('fullDWORD selected table value is read freshly', 2882338834, a=seeds[60], state=8, read_mutations={6180580: [(1, [(6180580, 2882338834, 4)])]})
    witness('initial state read precedes later RNG A load', 19, a=seeds[60], state=8, read_mutations={12375600 + 388: [(1, [(13874052, seeds[40], 4)])]})

def initializer_baseline(exercise):
    seeds = {}
    value = 439041101
    while len(seeds) < 100:
        value = value * 1664525 + 1013904223 & 4294967295
        seeds.setdefault((advance(value) & 32767) % 100, value)
    for state in range(256):
        exercise(state=state, a=seeds[40], label='all state low bytes')
    for state in [275, 268435475, 2147483667, 4294967059, 4294967295]:
        exercise(state=state, label='fullDWORD comparison versus stored byte')
    for remainder in range(100):
        exercise(state=18, a=seeds[remainder], label='all delay remainders and quotient returns')
    for (a, b, state) in itertools.product([0, 1, 16, 17, 2147483648, 2147483647, 4294967295, 305419896], [0, 1, 2147483648, 4294967295], [18, 19]):
        exercise(state=state, a=a, b=b, label='RNG state bits')
    by_direction = {}
    for (sample, seed) in seeds.items():
        by_direction.setdefault(advance(seed) & 7, seed)
    assert len(by_direction) == 8
    for (direction, attempt, blocked) in itertools.product(range(8), range(8), [0, 524292]):
        flags = [blocked] * 8
        flags[(direction + attempt) % 8] = 4
        exercise(a=by_direction[direction], terrain=flags, label='each circular neighbor hit and blocker')
    for (direction, result) in itertools.product(range(8), [0, 1, 256, 2147483648, 305397761, 4294967295]):
        exercise(a=by_direction[direction], result=result, outputs=[('out_x', 2882400002, 4), ('out_y', 4275863553, 4)], label='all neighbors blocked full search EAX')
    for (state, history) in itertools.product([18, 19], [0, 98, 99, 100, 107, 108, 109, 110, 111, 4294967295, 2147483646, 2147483647, 2147483648, 1073741824]):
        exercise(state=state, history=history, previous=2309737967, label='history signed wrap and alias')
    for (actor, result) in itertools.product([0, 1, 100, 1073741825, 2147483649, 4294967295], [0, 1, 1985216512, 4294967295]):
        exercise(actor=actor, result=result, outputs=[], child_mutations=[(13653752, 3, 4)], label='cached actor and seeded untouched output slots')
    for (x, y) in itertools.product([0, 1, 32767, 32768, 65535], repeat=2):
        exercise(x=x, y=y, result=0, label='fresh signed coordinates on failed search')
        exercise(x=x, y=y, terrain=[4] * 8, label='neighbor coordinate WORD wrapping')
    for result in [0, 1, 256, 4294901760, 4294967295]:
        exercise(result=result, outputs=[('out_x', 32770, 2), ('out_y', 65535, 2)], label='partial WORD output writes')
    return (seeds, by_direction)

def initializer_aliases(exercise):

    def actor_for(field, address):
        delta = address - field & 4294967295
        assert delta % 4 == 0
        actor = delta // 4 * pow(97, -1, 1 << 30) & 1073741823
        assert field + actor * 388 & 4294967295 == address
        return actor

    def witness(label, expected_eax=None, expected_memory=(), expected_calls=None, **cfg):
        exercise(label=label, expected_eax=expected_eax, expected_memory=expected_memory, expected_calls=expected_calls, **cfg)
    actor = 1
    o = 388
    cycle = 12375868 + o
    tile = 12375576 + o
    ax = 12375564 + o
    ay = 12375566 + o
    tx = 12375580 + o
    ty = 12375582 + o
    witness('cycle aliases history index; cached old index survives cycle11 store', actor=actor_for(12375868, 13873604), state=19, a=0, initial=[(13873604, 0, 4)], expected_memory=[(13873604, 1, 4)])
    witness('cycle aliases old sample; history keeps sample cached before cycle11', actor=actor_for(12375868, 13874036), state=19, a=0, initial=[(13874036, 2882400001, 4)], expected_memory=[(13873608, 2882400001, 4)])
    witness('cycle aliases A; fresh A load observes11', actor=actor_for(12375868, 13874052), state=19, a=0, expected_memory=[(13874052, advance(11), 4)])
    witness('cycle aliases B; B sees11 while cached A survives final counter overwrite', actor=actor_for(12375868, 13874048), state=19, a=0, b=0, expected_memory=[(13874048, advance(11), 4), (13874052, 0, 4)])
    witness('state byte aliases history index before its first load', actor=actor_for(12375600, 13873604), state=19, history=0, a=0, expected_memory=[(13873604, 20, 4)])
    witness('full state113 chooses delay although published byte19 aliases RNG A', actor=actor_for(12375600, 13874052), state=275, a=0, expected_eax=(advance(19) & 32767) // 100, expected_memory=[(13874052, advance(19), 4)])
    witness('cycle store cannot change cached history index', state=19, a=0, write_mutations={cycle: [(1, [(13873604, 99, 4)])]}, expected_memory=[(13873604, 1, 4)])
    witness('cycle store cannot change cached previous sample', state=19, a=0, previous=305419896, write_mutations={cycle: [(1, [(13874036, 2309737967, 4)])]}, expected_memory=[(13873608, 305419896, 4)])
    witness('cycle store precedes fresh A load', state=19, a=0, write_mutations={cycle: [(1, [(13874052, 305419896, 4)])]}, expected_memory=[(13874052, advance(305419896), 4)])
    for state in [18, 19]:
        witness('state%d index read precedes old sample read' % state, state=state, a=0, read_mutations={13873604: [(1, [(13874036, 3735928559, 4)])]}, expected_memory=[(13873608, 3735928559, 4)])
        witness('state%d A read precedes B read' % state, state=state, a=0, b=0, read_mutations={13874052: [(1, [(13874048, 305419896, 4)])]}, expected_memory=[(13874048, advance(305419896), 4)])
        witness('state%d cached A survives B-read mutation' % state, state=state, a=0, read_mutations={13874048: [(1, [(13874052, 4294967295, 4)])]}, expected_memory=[(13874052, 0, 4)])
        witness('state%d B3 versus A15 publication order' % state, state=state, a=0, write_mutations={13874028: [(1, [(13874032, 305419896, 4)])]}, expected_memory=[(13874032, 0 if state == 19 else 305419896, 4)])
        witness('state%d previous sample versus B7 publication order' % state, state=state, a=0, b=0, write_mutations={13874036: [(1, [(13874024, 2309737967, 4)])]}, expected_memory=[(13874024, 0 if state == 19 else 2309737967, 4)])
    extra_tiles = [(11491724 + (20000 + delta) * 4, 0, 4) for delta in terrain_deltas]
    extra_tiles.append((11491724 + (20000 + terrain_deltas[1]) * 4, 4, 4))
    witness('each neighbor attempt reloads original actor tile', state=19, a=0, initial=extra_tiles, read_mutations={tile: [(2, [(tile, 20000, 4)])]}, expected_eax=10, expected_calls=0)
    first_flags = 11491724 + (10000 + terrain_deltas[0]) * 4
    new_first = 11491724 + (20000 + terrain_deltas[0]) * 4
    witness('delta load precedes fresh actor tile load in same attempt', state=19, a=0, initial=[(new_first, 4, 4)], read_mutations={6180480: [(1, [(tile, 20000, 4)])]}, expected_eax=10, expected_calls=0)
    witness('tile load cannot retroactively change cached delta', state=19, a=0, terrain=[4] + [0] * 7, read_mutations={tile: [(1, [(6180480, 1234, 4)])]}, expected_eax=10, expected_calls=0)
    witness('one terrain DWORD supplies both flag tests', state=19, a=0, terrain=[4] + [0] * 7, read_mutations={first_flags: [(2, [(first_flags, 524292, 4)])]}, expected_eax=10, expected_calls=0)
    witness('vectorY is read after vectorX', state=19, a=0, terrain=[4] + [0] * 7, read_mutations={6180512: [(1, [(6180516, 2, 4)])]}, expected_eax=13, expected_memory=[(ty, 13, 2)], expected_calls=0)
    witness('vectorY cached before actorX is read', state=19, a=0, terrain=[4] + [0] * 7, read_mutations={ax: [(1, [(6180516, 99, 4)])]}, expected_eax=10, expected_calls=0)
    witness('neighbor targetX store precedes fresh actorY read', state=19, a=0, terrain=[4] + [0] * 7, write_mutations={tx: [(1, [(ay, 100, 2)])]}, expected_eax=99, expected_memory=[(ty, 99, 2)], expected_calls=0)
    witness('neighbor vectorY stays cached across targetX store', state=19, a=0, terrain=[4] + [0] * 7, write_mutations={tx: [(1, [(6180516, 99, 4)])]}, expected_eax=10, expected_calls=0)
    witness('vector entries consume only lowWORD', state=19, a=0, terrain=[4] + [0] * 7, initial=[(6180512, 2147418114, 4), (6180516, 2147549183, 4)], expected_eax=10, expected_memory=[(tx, 12, 2), (ty, 10, 2)], expected_calls=0)
    witness('search success caches both outputs before targetX store', state=19, a=0, result=305397761, outputs=[('out_x', 7, 4), ('out_y', 8, 4)], write_mutations={tx: [(1, [('out_y', 65535, 4)])]}, expected_eax=305397768, expected_memory=[(tx, 7, 2), (ty, 8, 2)], expected_calls=1)
    witness('search failure reads changed original actor not active actor global', state=19, a=0, result=0, child_mutations=[(ax, 32768, 2), (ay, 65535, 2), (13653752, 3, 4)], expected_eax=32768, expected_memory=[(tx, 32768, 2), (ty, 65535, 2)], expected_calls=1)
    witness('search failure targetX store precedes fresh actorY read', state=19, a=0, result=0, write_mutations={tx: [(1, [(ay, 123, 2)])]}, expected_eax=10, expected_memory=[(ty, 123, 2)], expected_calls=1)
    witness('full search result100 is nonzero despite AL0', state=19, a=0, result=256, outputs=[('out_x', 7, 4), ('out_y', 32769, 4)], expected_eax=32769, expected_memory=[(tx, 7, 2), (ty, 32769, 2)], expected_calls=1)

def enumerate_ported(sibling,cue,weighted):
    sibling_baseline(sibling);sibling_additions(sibling);sibling_aliases(sibling)
    cue_cases(cue)
    weighted_baseline(weighted);weighted_aliases(weighted)
def enumerate_initializer(exercise):
    initializer_baseline(exercise);initializer_aliases(exercise)
