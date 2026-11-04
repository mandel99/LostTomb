# Periodic and group target reconstruction

Five native bodies now have ordinary named C implementations in
`runtime/actor_command.c`. Their five civic register/flags models are removed;
the remaining civic models use typed compatibility dispatch entries. Native
addresses remain available through the existing public aliases.

| Native address | Named implementation | Native instructions |
| --- | --- | ---: |
| 4EAEC0 | restore_actor_try_periodic_target | 86 |
| 4EB000 | restore_actor_update_group_target | 364 |
| 4EBC90 | restore_actor_find_state_nearby | 325 |
| 4EABF0 | restore_actor_choose_clear_step | 127 |
| 56233E | restore_actor_random_seed | 3 |

The names describe observed behavior. The type lists remain literal kind IDs;
no species or undocumented game-state label is inferred from their numbers.
REA records and the exact pinned original PE authenticate all 905 instructions.
The original SHA-256 is
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

The periodic check preserves the classifier's upper return bits on early
failure, byte counter wrap, the signed threshold and wrapped 48-byte table
offset. Deferred returns preserve the table offset's upper bytes. It seeds the
actual game seed cell after the clock call, reloads coordinates, searches with
the undoubled radius, copies the target coordinates and publishes state21,
the radius-times18 delay and the low-word linked actor. The named and raw
entries return full EAX; the legacy byte interface exposes AL.

The group update searches the literal peer lists and preserves fresh state
reads after child calls. When following a peer, the two 31-step paired random
advances continue a local pair without an intermediate state publication.
The second history write, target-X store, history-index publication and final
target-Y read retain their native order, including aliases with random state
and history fields. The alternative search, clear-step selection and byte
progress timeout execute their existing game dependencies. The seed operation
stores its argument at604950; a host CRT seed function is not substituted.

The state-filtered spiral deliberately retains the native asymmetry. Each
point caches its tile-list head and each traversed chain caches its requested
kind; an empty head does not read the kind array. Kind and state comparisons
use the full requested DWORD against the actor's byte. Terrain fallback exists
only on the east segment. South correction decrements D3B4E0 instead of the
output Y coordinate. A terrain result of count+1 may itself be zero for
count=-1, and still terminates the search. Zero rings performs a full first
round; the original 228-round cap remains.

Clear-step selection checks seven neighbors and excludes the previous
direction. Its probe and output-X tables intentionally have different signs.
It tests the terrain predicate's AL, then the line predicate's full DWORD.
Output-X is read freshly after output-Y is stored; target-Y remains cached.
Consequently aliased outputs and a failed final line test retain native
observable writes.

`direct_c/audit_actor_animal_target.py` checks the seventeen observed periodic
callers and the group, step, state-search and seed callers. It tracks incoming
scratch-register bits through the observed control flow. Two DX*5 sequences
retain unknown upper bits while their WORD stores consume known low bits;
the remaining bits reach a full kill before meaningful use. The group caller
discards the result, and the clear-step caller tests AL. Their incidental upper
EAX values, including private stack addresses, are outside the byte C APIs.
The actual direction provider's twelve returns constrain AL to0..7.

`direct_c/verify_actor_animal_target.py` compares the original, named C, legacy
alias and civic raw adapter for both main Release executables. The corpus has
789 scenarios /3156 invocations per binary:317 ordinary algorithms,463 explicit
child protocols and9 synthetic RAM/clock mutation scenarios. It compares
original data/private bytes, ordered stores, meaningful child arguments,
supported byte/full return values and normal-return nonvolatile ABI. The only
controlled platform endpoint is the zero-argument clock. Ordinary lanes execute
the actual game seed, classifiers, searches, direction, route clearing,
terrain and line algorithms; protocol lanes identify substituted child results.

Both binaries execute all905 instructions and199 conditional outcomes. Four span<=0 outcomes are analytically
impossible because the saved span starts at1 and reaches at most455 at the
four tests. The initial direction==previous-direction outcome is impossible
for a direction in0..7. These five authenticated outcome qualifications are
listed separately from executed coverage; no missing instruction is accepted.
The strict `/W4 /WX` object build and both main Release targets also pass.

The current structural inventory has1153 ordinary C bodies of4402 catalogued,
214 instruction/frame models and1100 decompiler-style bodies. Five additional
catalogued entries are linked, for3999 linked entries. Structural categories
are not a gameplay proof. The actor-command, Hippo Stomp and earlier primary
suites are refreshed against these exact source and binary hashes.

The main CMake file remains unchanged. CD fallback source remains commented
and disabled. No host game is launched. Arbitrary private/caller-stack
corruption, unobserved indirect callers, OS exception dispatch and complete
gameplay remain unproved. The next pinned helpers are4EAAC0/4EAB10. Callback49E8B0 is authenticated
through its table pointer and935 decoded instructions, but Ghidra has no
procedure extent for it. Its two bounded selector/jump tables are now
authenticated; its behavior, children and ABI still need reconstruction. Many
other civic callbacks and catalogue bodies still need reconstruction.
