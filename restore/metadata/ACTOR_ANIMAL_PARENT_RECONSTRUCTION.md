# Hippo callback and route-vector reconstruction

The next connected C recovery replaces the kind 84 callback and its two direct
route helpers. Their civic register/flags bodies are removed, along with the
callback's dedicated switch-ROM reader. Existing native-address aliases and
civic raw entry points remain available.

| Native entry | Named C body | Instructions |
| --- | --- | ---: |
| 4EAAC0 | restore_actor_route_line_mode | 17 |
| 4EAB10 | restore_actor_find_clear_vector | 84 |
| 49E8B0 | restore_actor_update_hippo | 935 |

All 1,036 owned instructions derive from the pinned original PE with SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Ghidra does not define 49E8B0 as a procedure. The authenticated 3,544-byte body,
both bounded selector/jump maps and the decoded local control-flow graph
provide its scope: 935 statically reachable instructions and 17 normal returns.
The registry at 5DC28C selects this callback for kind 84, whose exact pointer
at 5DC3DC also agrees with the previously decoded Hippo Stomp allocation kind.

The line-mode selector consumes the low byte of its input, reads scene and
signed variant in native order and replaces only AL of its wrapped table
offset. Named/raw interfaces retain the full result; the existing byte alias
exposes AL. The vector helper tries sixteen offsets in native order, caching
coordinates and tile index before calling the selector and line algorithms.
It tests full line returns, requires an empty actor-list head, then tests the
terrain result's AL. It returns the full selected index or -1. The helper calls
the existing byte selector alias, which naturally retains that public entry.

The hippo update keeps both state switches, animation selection and exact
counter widths. It preserves the double byte-animation increment at cycle
completion, cached speed/terrain/group values, fresh active-actor reloads after
specified child calls, state-specific paired RNG publication and target-copy
load/store ordering. Seventeen normal returns preserve native EAX: sixteen
return an actor offset; the double failed group search returns the freshly
loaded actor ID after marking its status2. State numbers remain literal where
their gameplay names are not independently established.

`audit_actor_animal_parent.py` authenticates all 13 direct vector-helper callers
and the selector's single caller. It follows undefined scratch bits through
subsequent calls instead of assuming that a CALL kills them. Two helper EDX
paths reach callback RETs. Both registered callbacks use the 489C52 invoker,
which immediately calls the profiler 489A80. Exact native bytes, pinned import
names and installed Win32 declarations establish the zero/one-stack-argument
clock boundaries. Explicit full ECX/EDX definitions and the full clock return
discard incoming scratch values before use; callback flags are overwritten by
the profiler's prologue. This supports the existing void callback alias and
unused civic-frame scratch retention. It does not assert equality of incidental
C-void register values or execute the physical OS APIs.

The combined production verifier passes 1,929 scenarios / 7,716 invocations per
main Release EXE through original, named C, legacy alias and civic raw paths.
The 576 actual-child, 1,312 protocol and 41 synthetic RAM scenarios are counted
separately. Its ordinary lanes execute current C descendants. Explicit child protocols and synthetic RAM mutations
are counted separately. All 1,036 owned instructions, 198 conditional outcomes
and 21 parent switch targets must execute; this family accepts no missing or
analytically excluded owned instruction/outcome.

Its native/C comparison covers fixed data/private bytes, ordered writes,
meaningful direct child arguments, supported full/byte returns and normal
nonvolatile ABI. Direct-call traces are scoped to the owned bodies and their
compiler helpers, avoiding incidental nested VM-dispatch calls. The sole
controlled platform endpoint is timeGetTime. Extreme negative selector variants
use explicitly seeded per-image bytes outside the fixed data arena, restored
after each case; this verifies wrapped RAM arithmetic without claiming valid
gameplay inputs.

The verifier uses bounded memory hooks and explicit endpoint/code hooks.
Separate probes compare their event capture with the former global filters,
including cross-boundary writes and overlapping read-mutation anchors. The
shared startup verifier is unchanged. Actual route compositions use bounded
representative inputs; complete reconstruction of their still-unowned path
algorithms remains a later task.

The strict `/W4 /WX` object build and both main Release builds pass. The new
paired primary runs finish against the frozen source/binary snapshot before
the shared runner checks the previous 31 families once. That selected runner
receipt is deliberately partial: it omits the already fresh parent family and
all three finalizers. After every selected process exits, finalization runs the
exact CMake scope audit, music host binding and aggregate in that order.

The resulting 64 primary reports contain 43,864 scenarios / 148,514 invocations.
Each binary owns 14,210 native instructions: 14,193 executed, seven proved dead
in older families and ten user-disabled CD instructions. No host game is
launched, the main CMake file is unchanged and original CD fallback remains
commented and disabled.

The structural inventory classifies 1,155 ordinary C bodies and 4,001 linked
entries out of 4,402 catalogued functions. The uncatalogued 935-instruction
callback is included in this native comparison scope but does not inflate that
catalogue count. The catalogue still contains 214 instruction/frame models and
1098 decompiler-style bodies. Complete gameplay, arbitrary private-stack
corruption, unobserved indirect callers and OS exception dispatch remain
unproved. The next connected candidates are the kind 82/83 sibling callbacks
49D0F0/49DC50 and the kind-dependent cue wrapper4B5050.
