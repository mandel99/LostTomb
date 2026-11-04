# Six functions: C-first source checkpoint

Five Enemy callbacks and the shared service-state handler now have ordinary
named C bodies in `runtime/actor_command.c`. This follows the requested order:
translate connected blocks first, compile and inspect uncertain interfaces
immediately, then compare the combined main images with the native program at
a larger milestone. Complete-game reconstruction remains unfinished.

| Entry | Registry kind / role | C name suffix | Native instructions |
| --- | --- | --- | ---: |
| 493610 | shared service-state handler | service_state | 117 |
| 497DC0 | 43 Enemy | enemy_kind43 | 157 |
| 497FC0 | 44 Enemy | enemy_kind44 | 122 |
| 498370 | 46 Enemy | enemy_kind46 | 87 |
| 4984A0 | 47 Enemy | enemy_kind47 | 64 |
| 498590 | 48 Enemy | enemy_kind48 | 80 |

The627 native instructions are static byte-bounded scopes authenticated
against the pinned original and REA observations. They are not execution
coverage or a claim of provider-owned procedures. Enemy names retain the
generic shipped model label and use the kind number to distinguish entries.

The five Enemy bodies return full `uint32_t` values. Their canonical void
callbacks discard those values, while the civic dispatcher uses a typed result
route. Their obsolete individual frame models are removed. The new RAW adapter
and actual-child/main compositions still require the joint verification;
private controlled-endpoint witnesses are not current-main evidence.

Return values differ from the displayed graphic on several Enemy43/44 paths:
the short attack can retain actorID upper bits with the timer replacing AL,
death can return the signed half-delay, and other paths can return only the
selected base. Enemy46/47/48 return the full stored graphic. The early terrain
paths derive their actorID return from the exact native4BD250 helper while
calling its existing void C provider; no physical C-void scratch result is
assumed. Cached group/state/base reads, fresh actor/direction reads, wrapping
BYTE counters and signed WORD division toward zero remain explicit.

The shared493610 API stays `void(void)`. A caller's pushed literal is unused;
it does not become a fabricated parameter. State, speed and home are read in
native order. Successful destination search caches X before publishing state,
then reads Y after storing X. Child-dependent physical EAX is unpromised, so
the complete old RAW/model/prototype remain byte-exact for compatibility even
though the canonical game call now uses the ordinary C body.

Enemy45 at498160 is deliberately excluded from replacement. Its native
renderer indexes a16-byte table on its private stack. Outside heading0..7,
the lookup can read saved registers, a return address or other caller stack
data. The heading invariant has not been proved. Its original canonical,
RAW/model/prototype remain exact; no clamp, modulo or invented zero value is
introduced. This open issue does not prevent the independent callbacks above
from being translated.

The full module passes strict Win32 C11 `/W4 /WX /GS- /Ob0 /O2`, and both
main Release targets link. The host games were not launched. **Whole-block
native/main comparison, current registered-consumer qualification, expanded
cue inventory and broad regression validation remain pending.** CMake and
the commented, disabled original CD fallback are unchanged.

Together with the earlier six-animal and eleven-service source blocks, this
adds23 ordinary C functions (22 registry callbacks and one shared handler),
covering7,361 static native instructions after accepted checkpoint1683857.
Earlier accepted reports and source-first records retain their original
hashes and remain historical. Current source/build bindings are in
`actor-combat-translation-batch.json`; exact REA observations are in
`actor-combat-translation-rea-records.json`.
