# Scenario actor and group initialization

`4BB3F0` and `4BBC10` now use named ordinary C providers instead of register/
flags frames. The mission-initialization frame dispatcher is removed. Its old
model translation unit remains a declaration-only input so the build definition
and historical CMake scope records remain stable. `4B6FF0` now has a named,
typed group allocator with volatile BYTE/WORD stores and explicit reservation
handling. The existing `4EC410` C terrain predicate remains unchanged and is
compared as a real child and as a complete standalone body.

| Original | Current provider | Native instructions |
|---|---|---:|
| `4BB3F0` | `restore_actor_initialize_scenario_groups` | 560 |
| `4BBC10` | `restore_actor_initialize_scenario_points` | 90 |
| `4B6FF0` | `restore_actor_group_allocate` | 45 |
| `4EC410` | existing `FUN_004ec410` | 45 |

The point initializer visits eight signed WORD coordinate pairs and creates
kind95 only when X is positive. It advances the paired scenario RNG, calls the
real actor allocator and derives a five-bit actor delay from the separate CRT
LCG. Kind95 is retained as a numeric kind; its name is not guessed.

The group initializer reads climate/variant kind and count tables before its
editor-mode guard. It processes four WORD and four DWORD coordinate pairs,
allocates groups10..49 and creates each configured member. Kind/count values
remain cached while group-mode selectors use the native climate reload points.
Group centers and actor coordinates are reread at their original boundaries.

Terrain search examines at most16 candidates. Exhaustion still allocates at the
resulting next candidate. The initial candidate is the member ordinal, without
an added modulo or range guard. Zero actor results still receive the original
actor-zero stores. BYTE/WORD truncation, signed comparisons and32-bit address
wrapping are preserved.

The scenario RNG has two31-step shift registers plus a100-entry history. Its
pre-search, pre-creation, post-creation and point-init store sequences differ.
Separate named publishers preserve those orders. The post-creation pair/history
index remain cached into the next member, including aliases where history or
actor-field writes touch RNG state. The WORD lane retains its extra zero-delay
store and multiplies the final modulo25 delay by four; the DWORD lane does not.

Both Release binaries pass547 scenarios /1352 invocations each:740/740 owned
instructions and126 conditional outcomes. Each corpus contains405 actual
algorithm scenarios (including primitive-root comparisons),137 explicit game-
child contract scenarios and five synthetic memory-mutation scenarios. Genuine
parent compositions execute actual group/actor allocation, profile/pose, terrain
and LCG providers. Twelve additional actual point-then-group sequences per
binary compare72 parent invocations, plus four DWORD-kind/cached-count cases
compare12 invocations each. No platform/device endpoint or host game is launched.

The oracle compares all original static/private bytes, ordered CPU stores,
meaningful normalized child arguments, defined results, normal-return ABI and
matching explicit memory faults. It covers history wrap in all six group RNG
phases, exhausted searches, zero/negative/wrapped results, state aliases, fresh
reads, cached climate/counts and both allocator reservation branches.

The exact observed caller has one direct call to each initializer. The retained
caller-prefix audit proves that each incoming EAX is overwritten before use,
supporting the existing void interfaces. Indirect/unrecognized callers, other
incidental volatile registers/flags, fault-time registers, arbitrary stack
aliases, OS exception dispatch and complete gameplay remain unproved.

Exactly two instruction models and one decompiler-style catalogue body are
converted. The structural inventory is1144 ordinary C entries,216 instruction
models and1106 decompiler-style entries among4402 catalogued procedures. Naming
helpers or retesting the already-C terrain predicate adds no catalogue entries.

The next pinned scope is the603-instruction command/state graph at
`4EC990`, `4EC7C0`, `4EC0F0` and `4EC4A0`, including its callback search and
command-transition cycle. Its remaining models are not approved here.
