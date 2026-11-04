# Crocodile, hyena, cue and state initialization reconstruction

Both main Release comparisons passed **4,951 scenarios / 18,599 invocations per
EXE**. They cover1,907 executed instructions plus15 separately proved dead,
all296 feasible conditional outcomes and53 switch targets. Each binary checks
860 actual-child,4,085 protocol and6 standalone RAM cases; protocol cases also
contain explicit RAM mutation witnesses. All four newly converted raw entries
are checked. The complete old raw cue model remains unchanged.

The paired results are [game](actor-animal-following-game.json) and
[diagnostic](actor-animal-following-diagnostic.json). Compilation and these
finite comparisons do not establish complete gameplay equivalence.

| Native entry | Ordinary C name | Native bytes | Instructions | New routes |
| --- | --- | ---: | ---: | --- |
| 49D0F0 | restore_actor_update_crocodile | 2782 | 728 | named, canonical, RAW |
| 49DC50 | restore_actor_update_hyena | 3036 | 775 | named, canonical, RAW |
| 4B5050 | restore_actor_play_kind_cue | 290 | 119 | named, canonical; old RAW retained |
| 4ECD40 | restore_actor_choose_crocodile_state | 339 | 99 | named, canonical, RAW |
| 4ECA70 | restore_actor_enter_animal_state | 720 | 201 | named, canonical, RAW |

The family owns1,922 native instructions. Fifteen instructions and six branch
outcomes have separate mathematical infeasibility proofs; every other owned
instruction/outcome remains an executable primary-verifier obligation. Neither
compilation nor finite comparisons establish complete gameplay equivalence.

## Behavior and full-width contracts

The crocodile and hyena callbacks preserve their distinct cache/read/store
phases. The crocodile reads speed before publishing its incremented animation
counter and examines nine terrain cells for a flag-dependent cycle change. The
hyena reads speed after counter publication, reads direction before rotation,
and has different timer, movement and inline RNG phases. Their related state
machines have not been unified. The normal result is the full actor byte
offset; the crocodile's double-search failure returns a freshly loaded actor
ID, while hyena state24 returns full20 or8 according to its signed timer path.

Species names come from all six shipped Figure_model files: zero-based
ordinal82 is Crocodile and83 is Hyena. Every displayed figure ID agrees with
its row ordinal. Authenticated native loader bytes establish14DWORD rows
starting at7090CC, and callback/registry bytes use the same kind index. These
names are not guesses based on terrain, animation or sounds.

The weighted selector caches the original actor state before RNG/history
stores. Its two cumulative weights are50/50 for state8,90/10 for19,25/75
otherwise. A modulo100 draw selects full DWORD table state19 or18. The third
table value41 is dormant only under the stated normal-private-stack condition;
the original unvalidated history write must not corrupt the local weights or
return frame. The selector forwards the initializer's full EAX unchanged.

The initializer compares the complete state DWORD to19 while publishing only
its low byte. Full state0x10000013 therefore publishes byte19 but uses the non19 branch.
It preserves different RNG publication orders in its two branches. Non19 sets
a250..349 delay, cycle12 and counter0, returning the full A15/100 quotient.
State19 sets cycle11, tries eight circular terrain neighbors with a fresh actor
tile load per attempt, then falls back to the existing ordinary C provider
restore_building_nearest_search. Its arguments are signedWORD X/Y, type_count0,
typesNULL, mask4, private output pointers, rings5 and original actor ID as the
excluded actor. The outputs initially contain actor/state19 and are disjoint
from the provider's map globals.

Initializer results are preserved as complete DWORDs: neighbor success returns
zeroextended WORD targetY; failed search returns zeroextended fresh WORD actorX;
successful search returns `(search_EAX &FFFF0000) | WORD outputY`. Both search
output WORDs are cached before either target store on success. On failure,
targetX is stored before actorY is read afresh. The cached original actor is
used even if the active-actor global changes. The existing search provider
receives no new conversion credit.

## Explicit cue scope and retained compatibility

The entire old `RAW(004b5050)`, restore_civic_C_004b5050 body, prototype, two ROM
arrays and mi_read_004b5050 reader remain unchanged. Six exact source-fragment
hashes enforce that retention. This is source compatibility preservation;
**no new full RAW cue comparison is claimed**.

The new ordinary named/canonical cue is authorized by these five individual
typed caller proofs:

| Native call site | C caller | Scope |
| --- | --- | --- |
| 49DA20 | restore_actor_update_crocodile | current-batch |
| 49E692 | restore_actor_update_hyena | current-batch |
| 49EA25 | restore_actor_update_hippo | current-batch |
| 49F3DD | restore_actor_update_hippo | current-batch |
| 49C50C | actor_prey.c / FUN_0049bd40 | separately explicit existing C caller |

The CLI `current-batch` scope remains exactly four sites. The fifth is a
separate typed contract, not an implicit extension of that CLI scope. Source
inventory verifies four ordinary calls in actor_command.c and one in
actor_prey.c, plus the named call inside the canonical adapter. Any additional
runtime C caller fails the release audit.

The portable local reproducer reanalyzes all incoming EAX/ECX/EDX/arithmetic
flag bits from authenticated bytes. One path enters the authenticated4B1D50
prefix until all prior cue scratch is discarded; this does not prove that
callee's outputs match another C provider. No opaque API summary or branch
assumption is used in the local gate. Two additional REA envelopes supplement
the family and existing tracked parent evidence; the full global diagnostic
dossier remains private.

The global cue audit remains **OPEN** at5653DD CALL EAX through the mutable CRT
new-handler slot1266480. There is no null-forever assumption and no global
raw-drop authorization. The public cue result deliberately keeps `passed=false`
and `production_raw_adapter_ready=false`, while `selected_scope=current-batch`,
`selected_scope_passed=true` and `diagnostic_allow_open=false`. The typed-five
contract has `local_typed_contract=true` and `global_raw_drop=false`.

## Reproduction and remaining limits

Run `python restore/direct_c/audit_actor_animal_following.py --require-production`
after refreshing the tracked actor-animal-parent audit. The main audit reruns
the portable local cue reproducer and typed-five generator, verifies the
current production source inventory, five named/canonical routes, four new
RAW routes, exact retained cue fragments, native CFGs and source hashes.
Its `production_ready=true` is scoped to that retained-RAW policy. The report
has `raw_scope.entries=[49D0F0,49DC50,4ECD40,4ECA70]`,
`cue_raw_retained=true`, `cue_raw_verified=false`.

Catalogue, REA, species, caller, typed and compatibility records use the
actor-animal-following stem in this directory. Public scripts depend on those
metadata files and existing tracked dependencies, not private build scripts.
Original PE/model assets remain local untracked inputs. Candidate proofs are
historical isolated protocol/RAM lanes; their EXE bindings are not relabeled
as current production results. The current main-C verifier executes actual current C children in its
actual-child lanes and labels protocols separately.

Normal32-bit cdecl/private-stack/nonvolatile ABI and valid disjoint output
buffers are explicit preconditions. Arbitrary code/stack corruption,
unidentified indirect/runtime-generated callers, physical sound and complete
gameplay equivalence remain outside the proof. Neither original nor restored
host game is launched. The full reconstruction goal remains unfinished.

The shared checkpoint retains68 successful checks from its first selected run.
That run stopped after a missing `snapshot` test helper caused one early
failure. After restoring the identical helper, a fresh20-job recovery ran only
that failed job and the19 jobs not started. Passed reports were checked against
their original source/binary hashes and were never rebound. Both receipts keep
their actual statuses. Serial CMake scope and controlled music-host checks run
after both groups finish; the current aggregate validates66 primary reports.
The source-only prey provider is not the canonical49BD40 object in either main
MAP; its fifth typed cue proof is an explicit conservative source contract.
