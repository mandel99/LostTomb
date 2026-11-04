# Hippo Stomp action (41CD00)

The final instruction model in actor_command.c is replaced by the ordinary
nullary C function `restore_actor_command_spawn_hippos`. Native table 57C3C8
row 3 points to this entry; its stored string decodes with XOR 3 to `Hippo Stomp`.
The legacy C alias and actual menu action facade share that implementation.
The register-frame type, model dispatch and private 512-DWORD VM stack are gone.
The model translation unit remains declaration-only to preserve the production
CMake definition and its existing fixture-scope evidence.

REA authenticates the exact 701-byte span, command row, vector table and caller
windows against original SHA-256
fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
Ghidra has no procedure at 41CD00; Capstone decodes the raw span into 195
instructions. This fact is explicit rather than manufacturing a provider-owned
function dossier. The old `next` records/notes are historical capture inputs;
current verification belongs to actor-spawn-hippos diagnostic/game reports.

The action runs only when BYTE E38DDA is 1. It allocates a kind-84 group with
mode 5 and direction 0, and continues only when the full signed group result is
positive. The three group stores retain BYTE/WORD widths and ordering. It then
creates eight actors, using two separate paired 31-step RNG advances per actor.
Each phase keeps its native history loads, history-index update and publication
order; the existing state-transition publisher has a different order and is not
substituted for these phases.

The terrain probe starts at the actor ordinal and advances its vector index
after every check, including success. Allocation uses the already advanced
index, including after all eight attempts fail. The probe's center-Y addition
is unscaled in the original index calculation. C preserves it, while actor
creation uses the actual vector Y plus the separately cached center Y. The
second phase loads center Y between its first state publication and masked
outputs; vector/center X are loaded later. These freshness boundaries matter
when RAM aliases or a child changes the values.

Actor allocation is not guarded against zero: native behavior writes actor-zero
fields and invokes target search after an allocation miss. Full DWORD actor
results still drive wrapped addressing. The state, group link, delay, ordinal
and animation stores use their native widths and order before the actual C
target-search graph runs.

The return audit follows the table call through 41D010, 41D3F0, the tail jump in
41D310 and input handler 4D8310. The table invoker overwrites only AL, so upper
EAX bits can propagate through these returns. Both observed input-loop callers
reach 4CF1F0, whose first instruction zeroes EAX before any use. The intervening
4D0280 either returns without consuming the incoming value or replaces it first.
This supports void for the existing nullary UI callback. Ghidra's reference
metadata labels 41D390 as a call; authenticated E9 bytes prove a tail jump and
are used by the audit. Unknown indirect consumers remain outside that proof.

The primary oracle owns only the 195 action instructions. It compares original,
named C, legacy C alias and actual handled-action facade. Actual algorithm
compositions, explicit group/actor/terrain/search child contracts and synthetic
RAM changes are counted separately. Full original data/private bytes, ordered
stores, meaningful child arguments, normal-return nonvolatile ABI and matching
explicit memory faults are compared. It covers lower-byte terrain returns,
signed group guards, vector advance/exhaustion, allocator zero/wrapped results,
both RNG phases, history/RNG overlap, center freshness and invalid addresses.

Both Release executables pass 169 scenarios / 676 invocations each: 35 actual
algorithm cases, 131 explicit child contracts and three synthetic RAM cases.
All 195 native instructions and all 28 conditional outcomes execute; no dead or
unexecuted instruction/edge qualification is needed for this action. The same
strict actor-command project passes `/W4 /WX /GS- /Ob0 /O2`. This entry was not
in the original 4,402-function catalogue, so its conversion does not inflate that
catalogue's structural ordinary-C count.

No host game is launched. Incidental callback EAX/flags, fault CPU state,
arbitrary private/caller-stack corruption, OS exception dispatch and full
gameplay are not certified by these finite tests. The remaining civic/search
callers and broader unresolved catalogue still need reconstruction.
