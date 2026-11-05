# Connected actor cohort: approximately 100 functions

Following the requested workflow, detailed native comparison and broad
regressions are deferred until a connected cohort of approximately 100
functions is transcribed. Cheap compilation and focused checks of genuinely
uncertain interfaces continue during writing. Source commits are pushed on
the existing main branch during this work.

The current cohort contains **63 transcribed ordinary C functions** after
accepted native checkpoint 1683857: 40 registry callbacks and 23 shared
helpers, covering 11,770 static native instructions. These are source scopes,
not execution coverage, a global game inventory or completed behavior proof.
The previous four source records describe 32 of them. Four initial additions
are listed below, followed by four connected fragments containing 27 more:

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
Five projectile/grid helpers add 322 instructions: path checking, projectile
creation, ordinal refresh and two tile-list searches. Four contact/formation
helpers add 544 instructions: neighboring contacts, list-state transitions,
damage and loss penalty. Their unknown physical returns remain outside the
ordinary void APIs, with complete original frame paths retained.
Ten Dock/trade helpers add 1,287 instructions for trade accounting, import
capacity, transactions, storage selection and docker transitions. Existing
storage import/export are reused and receive no new rewrite credit.

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

The current source module and both changed grid providers pass strict Win32
C11 `/W4 /WX /GS- /Ob0 /O2`. **The current main executables have not been
rebuilt or compared with native code.** Their last linked source checkpoint
is 1c63f52. Historical compile/link and accepted reports keep their original
hashes and are not rebound to this tree. No host game was launched.

The remaining 39 planned tasks are connected actor callback peers or
children of actor state, target, projectile, walking and trade paths. The
sentry, Dock/trade and human-state groups are reserved to separate agents. Planning
relationships come from existing source/models and are not a complete native
call graph: authenticate every native body before translating it. If an entry
already has ordinary main C, do not count it again; choose another connected
missing function. Three direct query dependencies are tracked as existing-C
repairs/refinements with zero new rewrite credit. Other already ordinary
functions were also excluded. The target is approximate, not a reason to pad
the count with duplicate implementations or repairs.

Complete frame compatibility remains for Map flag, the shared state handlers,
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
refinements, the remaining queue and explicit pending stages.
`actor-support-translation-rea-records.json` contains the exact authenticated
native windows for the four new functions and grid return closures.
`actor-cohort-100-new-rea-records.json` binds the 27 later bodies and their data.
