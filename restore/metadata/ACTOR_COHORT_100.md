# Connected actor cohort: approximately 100 functions

Following the requested workflow, connected functions were transcribed
before the detailed comparison milestone at approximately 100 entrypoints.
Cheap compilation and focused checks of uncertain interfaces continued
during writing. Source commits are pushed on the existing main branch.

The current cohort contains **92 new ordinary C algorithms** after accepted
native checkpoint 1683857: 45 registry callbacks and 47 shared helpers,
covering 14,744 static native instructions. Another **13 entries reuse,
repair or adapt existing C**, giving a joint verification workload of105
entrypoints. These are source scopes, not execution coverage, a global game
inventory or completed behavior proof. Four initial additions are listed below:

| Entry | Name | Native instructions | Current contract |
| --- | --- | ---: | --- |
| 494800 | Map flag | 167 | Full named grid-link result; original frame path retained |
| 494B30 | Flotsam | 250 | Full actor offset result |
| 4998F0 | Soldier target search | 82 | Full actorID or zero; two-pass nearest/fallback logic |
| 499E50 | Priority target search | 206 | Full actorID or zero; priority and distance must both improve |

The shared service fragment adds Schoolchild, Teacher, Librarian, Dentist,
Physician, Herbalist, Embalmer and Worker: eight native functions /419
instructions. Six use one ordinary C service routine with their authenticated
sprite slots; the existing 493610 state handler is reused.
Three new projectile/grid helpers add 231 instructions: path checking,
projectile creation and ordinal refresh. The two tile-list searches reuse
existing source and receive zero new rewrite credit. Four contact/formation
helpers add 544 instructions: neighboring contacts, list-state transitions,
damage and loss penalty. Their unknown physical returns remain outside the
ordinary void APIs, with complete original frame paths retained.
Ten Dock/trade helpers add 1,287 instructions for trade accounting, import
capacity, transactions, storage selection and docker transitions. Existing
storage import/export are reused and receive no new rewrite credit.

The latest connected fragments add 33 new algorithms /3,850 instructions:
six sentry/target helpers, seven trade/human-state functions, eight path
helpers and twelve water-building/human-support functions. They include
trade ships, merchant caravans, protestors, fishing boats, standard bearers
and the entertainer callback shared by five actor kinds. Existing search,
resource-consumption and pixel-path algorithms are reused.

Independent provider audits searched the whole source tree, including
inactive files. They corrected four earlier source-only attributions:
4B56D0 and 4B5770 reuse their previous typed tile queries; 493610 reuses its
previous service-state algorithm with native read/publication phases fixed;
49BD40 is an adaptation of an earlier ordinary ostrich implementation.
The duplicate tile/service bodies were removed from `actor_command.c` and
the original owners were selected through CMake. The corrected count for
the previous 63-function checkpoint is 59 new algorithms. Earlier immutable
family reports keep their historical counts and hashes; the current ledger
records these corrections explicitly. Mere presence of generated C is not
enough for direct reuse: the remaining rejected providers have concrete
native width, address-stride or ordering defects. See
`actor-cohort-100-provider-audit.json`.

The target searches retain signed comparison and modulo subtraction,
first-wins ties, fresh best-distance reads, full Boolean child results and
the final ordered publication of target coordinates. Only their identical
pure difference helper is shared. Load/store phases remain separate.
Flotsam preserves WORD guard versus BYTE coordinate reads, movement toggle,
RNG bands and low-WORD delay stores. Map flag preserves fresh coordinate,
flag and actor reads, pixel wrapping and the final grid insertion.

Two already ordinary C grid providers now also expose result-bearing APIs:
`restore_actor_link_to_grid` and `restore_actor_unlink_from_grid`. Each uses
one algorithm core shared with its existing void facade, preserving the
original diagnostic caller site. Link returns the native tile/tail value;
unlink returns the cached actor offset. Map flag now returns the result of
the real typed link API. No physical C-void scratch result is sampled.
These refinements receive **zero new rewrite credit**.

The current source module and selected reused providers pass strict Win32
C11 `/W4 /WX /GS- /Ob0 /O2`. **Both current Release executables build
successfully.** All 7,868 build/proof inputs in the final prebuild snapshot
remained byte-identical after the build. The five mutable report outputs
are excluded from that input snapshot. The existing controlled
`standalone_actor_callbacks_small` fixture also builds and passes after
limiting provider renames to the main library. The selected API comparison
now agrees on all 1,649 scenarios for 105 entrypoints. It performed 8,237
route invocations: 1,649 native references and 6,588 compiled routes across
both main images. There were zero failures. The suite contains 1,608
controlled-protocol scenarios and 41 actual child compositions. It does
not close instruction/branch/loop coverage, RAW or global cue gates;
broad current-main regression remains pending.
Historical accepted reports are not rebound to these new PE hashes.
No host game was launched.

The wider comparisons found two phase errors. In 492D20 the monument
predicate had folded two native WORD reads of the piece index into one.
The first source correction matched constant-value cases, but mutation
cases showed that emitted MSVC code still skipped the second read when
the first was nonzero. Two real noinline read calls now preserve that path,
confirmed in both main images. The existing 492FC0 ordinary owner also now
reads stock before conditionally reading cost, as native does. Both repairs
receive zero new rewrite credit. All 17 current repair witnesses agree
across 85 routes. Historical runs keep their original source/PE bindings;
the broader current-main selected API suite also agrees. See
`actor-cohort-100-phase-repair.json`.

The actual main linker omits two unused symbols. For 492FC0 the available
ordinary void owner is `restore_monument_consume_resource`; its unused
`FUN_00492fc0` facade has no MAP entry. For 4B5420 only the canonical void
entry is present; the unused named full-result helper has no MAP entry.
Current-main comparison must respect these available lanes and cannot
claim a result from an absent symbol.

Original frame paths remain for Map flag, the shared state handlers,
cue and the earlier void animal callbacks. Laborer RAW delegation is unchanged.
Enemy45 and Egyptian Chariots also retain their original canonical/frame
paths because their stack-table index invariants are unproved. Global physical
register/flags closure and whole-game reconstruction remain unfinished.
The original CD fallback remains commented and disabled.

The creator's player/direction arguments have a bounded native BYTE-input
proof in `actor-cohort-100-creator-byte-contract.json`. Unused upper argument
slot bits remain diagnostic; identical raw DWORD slots are not claimed.
New contact cue site 4BC5F9 is a typed void scope with global cue closure false.
Unproved difficulty-array domains in the pending Criminal/Tomb Robber drafts
also remain outside the transcribed count until their invariant is closed.

`actor-cohort-100.json` binds current bodies, source hashes, syntax evidence,
both main builds, zero-credit reuse and explicit pending stages.
`actor-cohort-100-main-comparison.json` binds the finite comparison result,
all 105 per-entry scenario counts, exact source/PE/MAP hashes and remaining
qualifications. A corrupted cursor input remains a separate native-fault
appendix; its wrap behavior was not changed or clamped in C.
`actor-support-translation-rea-records.json` contains the exact authenticated
native windows for the four new functions and grid return closures.
`actor-cohort-100-new-rea-records.json` binds 60 later native body windows
and their data, including the two zero-credit tile-query scopes.
