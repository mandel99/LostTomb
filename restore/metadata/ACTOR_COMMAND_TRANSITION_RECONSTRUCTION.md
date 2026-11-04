# Actor command transitions and tile search

This recovery replaces the five register/flags bodies at 4EC990, 4EC7C0,
4EC4A0, 4EC0F0 and the previously unidentified callback 4EAE20 with named C.
The command-table entry 41CD00 now also has its own named C replacement; see
[Hippo Stomp](ACTOR_SPAWN_HIPPOS_RECONSTRUCTION.md). The animal/civic bridge and
the three canonical recovered source files route into the new C bodies. All six
old models and the private register-frame/VM-stack invocation are removed.

REA provides the four analyzed procedure spans (603 instructions) and exact
153-byte callback input (53 instructions). Ghidra reports that the callback is
not contained in an analyzed procedure; its assembly is explicitly a Capstone
decode of those authenticated bytes. Original PE SHA-256 is
fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
The `next` evidence files remain immutable historical inputs. The new native
catalogue records its capture-stage status; current comparison results belong
to the separate diagnostic/game reports.

| Native entry | Named C body | Recovered behavior |
| --- | --- | --- |
| 4EAE20 | restore_actor_find_other_kind_on_tile | Follow signed WORD tile-list links; skip self, inactive actors, state 4, disabled kind records, the same kind and kinds selected by EADA0. Return the first remaining actor. |
| 4EC0F0 | restore_actor_spiral_search | Visit east/south/west/north segments, test bounds and the blocked-tile flag, and return the first nonzero callback value. Preserve fresh output/global reads and the original 228-round limit. |
| 4EC990 | restore_actor_seek_other_kind | Classify the actor, honor inactive/state guards, read scene/variant search radius, then copy the selected actor's coordinates and link. Fall back to weighted state selection on a miss. |
| 4EC7C0 | restore_actor_choose_command_state | Pick one of four table entries from state-dependent weights using the original paired 31-step RNG and history ring. |
| 4EC4A0 | restore_actor_apply_command_state | Store the low state byte, then dispatch on the full DWORD: timed state 18, local movement/nearest-search state 19, short timed state 26, or a target-search default. |

The decompiler's void interfaces lose observable return bits. In particular,
4EC4A0's default branch returns its child's full EAX. The new C interfaces retain
the classifier's upper 24 bits when only AL changes, the found actor's complete
value, the timed-state division quotient, and the movement branch's partial-AX
return after nearest search. No boolean normalization replaces these values.

Each RNG phase retains its own publication order. History index/sample reads
precede the state-19 animation store; the pair is loaded after the history write,
so history aliases of the random-state globals still take effect. Movement uses
all eight local candidates before calling the real nine-DWORD nearest-search
body. Its output locals start with the native argument-slot values, so a child
that leaves an output unwritten preserves that value.

The native caller audit checks exact original bytes and retained source models.
The animal callsites reload ECX and overwrite flags; their continuation replaces
EDX before use (the 49F108 path crosses the typed movement helper). The command
loop reloads its RNG registers before reuse. The bridge therefore carries the
full result, RET0 stack movement and nonvolatile registers without retaining the
five old models merely to manufacture unused scratch values.

Verification compares native bytes with the actual main-build providers, using
both named and canonical entry points. It checks full original data/private
memory, ordered CPU stores, meaningful child arguments, full EAX results and
normal-return nonvolatile ABI. Actual algorithm lanes, controlled child/callback
contracts and synthetic RAM mutations are counted separately. Output aliases,
blocked tiles, signed links, all weight rows, partial returns, history wrapping
and history/RNG overlap are explicit cases.

Three sign-correction instructions are analytically dead because their source
was masked to 15 bits before `AND 80000007`. Four nonpositive-span outcomes are
dead because the spiral's span is always 1..455. The weight-loop exhaustion
outcome is dead because every row totals 100 and the roll is 0..99. These three
instructions and six outcomes are separately qualified by the native audit;
they are not presented as executed coverage.

The fixture uses an exact page-based seed reset: every changed page, including
changed code, is restored. Unchanged pages require no write or Unicorn code-cache
flush. Neither the comparison scope nor the compared final memory is reduced.

Both Release executables pass 1,353 primary scenarios / 3,976 invocations each:
1,256 actual algorithm cases, 95 explicit child/callback contracts and two RAM
mutation cases. All 653 feasible instructions and 138 feasible conditional
outcomes execute. The three instructions and six outcomes proved dead above are
qualified separately. Another 67 cases / 201 invocations per binary check actual
seeker/search/callback compositions, including scene/variant/radius selection,
coordinate boundaries and fresh target-coordinate reads (66 actual cases and
one synthetic RAM mutation). These extras do not expand instruction ownership.
The strict source build passes `/W4 /WX /GS- /Ob0 /O2`.

Fourteen further cases / 28 invocations per binary execute the 41CD00
parent through the real action facade and its new C children. They compare all
data, ordered stores, meaningful child arguments and nonvolatile ABI with the
native parent, including disabled mode, full group/actor pools, terrain, history
aliases and RNG extremes. No child or platform result is replaced. The parent
has separate primary instruction ownership; these extra comparisons add none
and do not certify its incidental EAX/flags or arbitrary stack aliases.

No host game is launched. This finite verification does not prove all gameplay,
arbitrary caller-stack aliases, corrupted private locals, unknown indirect
callers, self-modifying code, OS exception dispatch or unused volatile outputs.
The remaining civic models and other unresolved game logic still need
reconstruction. Main-binary changes require all prior comparison reports to be
refreshed before a checkpoint is published.
