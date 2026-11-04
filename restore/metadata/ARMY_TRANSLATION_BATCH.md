# Nine functions: C-first army/state checkpoint

Eight registered callbacks and their shared enemy-group state machine now use
ordinary named C in `runtime/actor_command.c`. Their canonical entries are
connected to these bodies. This is another translation-first checkpoint;
complete-game reconstruction and joint main/native verification remain open.

| Entry | Kind / role | C name suffix | Native instructions |
| --- | --- | --- | ---: |
| 4986B0 | 49 Barbarian | barbarian_kind49 | 36 |
| 498750 | 50 Barbarian | barbarian_kind50 | 36 |
| 4987F0 | 51 Barbarian | barbarian_kind51 | 46 |
| 4988B0 | 52 Barbarian | barbarian_kind52 | 98 |
| 498A10 | 53 Enemy | enemy_kind53 | 45 |
| 498AD0 | 54 Egyptian Infantry | egyptian_infantry | 116 |
| 498C60 | 55 Egyptian Archers | egyptian_archers | 114 |
| 499010 | 57 Enemy | enemy_kind57 | 118 |
| 4991A0 | shared group state handler | enemy_group | 523 |

The 1,132 instructions are static scopes authenticated with exact REA bytes
and the pinned original, not execution coverage. Generic Enemy/Barbarian
labels retain numeric kinds instead of guessing a more specific identity.

Callback returns are explicit full `uint32_t` values. Kind49/50/51 and the
normal kind 53 path return the published heading without storing a primary
graphic. Infantry, Archers and Enemy57 preserve their distinct base, heading,
sprite and partial-AL return forms. Enemy57 keeps its temporary direction11
graphic store before rereading render state; its return prefix can derive
from actorID or a signed pose half-delay. Infantry reloads the actor after
sound, while Archers also reloads the global heading. Cached group decisions,
fresh actor reads, BYTE wrap and one-time negative heading correction remain
separate operations. These eight obsolete per-callback frame models are removed;
their new typed RAW adapters still await joint qualification.

The 523-instruction group handler is ordinary `void(void)` C, covering states
3/4/5/9/10/11/12/13/14 and default: formation offsets, saved targets, movement,
linked actors, ranged searches, aiming and projectile counters. It preserves
fresh ordinal reads, signed full coordinate comparisons before WORD stores,
cached speed, negative full target results and cached aiming actor versus
fresh projectile actor. Original 47D350 is an authenticated zero-result leaf,
so its unused signal arguments are omitted through the existing typed API.

The group handler promises no physical EAX/ECX/EDX/flags. Its original PUSH ECX
slot is overwritten by cached speed before the final POP; several exits also
inherit child-dependent EAX. Its complete RAW/model/prototype remain exact.
The same earlier compatibility remains for cue, Ostrich, Antelope and the
shared493610 service-state handler. No unknown physical value is replaced by
a fabricated zero or result bridge.

Enemy45 at498160 and Egyptian Chariots at498DF0 keep their entire original
canonical/RAW/model paths. Both index a native16-byte private stack table;
heading0..7 has not been proved globally. Chariots additionally reloads the
index after sound, so that post-sound invariant is also open. No clamp, modulo
or guessed stack value is introduced, and these drafts are not counted above.

The whole C module passes strict Win32 C11 `/W4 /WX /GS- /Ob0 /O2`, and both
main Release targets link. Private finite interface witnesses are not rebound
to these main images. **Combined main/native comparison, actual-child
compositions, registered-consumer/RAW qualification, expanded cue inventory
and broad regressions remain pending.** Neither host game was launched;
CMake and the commented, disabled original CD fallback are unchanged.

Across four source-first blocks after accepted checkpoint 1683857, 32 ordinary
C functions are transcribed and linked: 30 registry callbacks and two shared
handlers, totaling 8,493 static native instructions. Earlier accepted and
source-first records retain their original hashes. Current bindings and
pending states are in `actor-army-translation-batch.json`; exact REA records
are in `actor-army-translation-rea-records.json`.
