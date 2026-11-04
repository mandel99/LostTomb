# Whole actor-message controller, 2026-11-04

53C100 now uses `restore_actor_update_message_result` in
`runtime/actor_message.c`. Its canonical entry in `src/unit_02503.c` delegates
to the named void API; the old decompiler body and the active976-instruction
register/flags interpreter are removed. `runtime/menu_actor_message.c` retains
only typed call/stack adapters and dispatch. Its53C100 frame preserves inherited
EAX explicitly. The original sole native caller discards EAX, so the canonical
and named gameplay APIs remain void.

The original actor stride is388 bytes. Kind is the BYTE atBCD602, state at
BCD630, cycling counter atBCD674, message atBCD675 and group atBCD729.
EADCDC is a shared DWORD result. These names describe observed behavior; exact
original identifiers and player-facing text meanings are not asserted.

The function returns immediately for nonpositive signed actor indices. Otherwise
it clears the actor message, sets the shared result toFFFFFFFF, scans the actual
town list for a44D700 record, clears the actor group and selects by the actor's
BYTE kind. All37 destinations in the first switch are represented by ordinary
C cases, including grouped kinds and the native invalid/default behavior.

The recovered cases cover cycling counters, actor state and cargo values,
city conditions, training/building predicates, trade capacity, linked actor
kinds and weighted message selection. Distance decisions sign-extend four WORD
coordinates, compute absolute axis differences, write the X and Y diagnostic
DWORDs in that order, and compare their maximum with25. Employment decisions
cache four city DWORDs, use wrapped32-bit sums/multiplication, then signed IDIV.
The remaining city reads use the original signed thresholds and cached city
offsets. Kind85 replaces the prior town-record result with its cached state
membership before calling4BF9D0 with the proven D05720 receiver.

Store order is part of the reconstruction. Some cases write the shared result
before the actor group, others do the reverse; terminal cases write the actor
message before the shared result. The common completion path consumes its
cached EAX value, which may differ from memory after an intervening write.
Export/import cases reload the linked building WORD, current city DWORD and
trader BYTE between their two child calls. Counter decisions compare the cached
incremented BYTE, then reload the counter where the native code does.

When the common result isFFFFFFFF, a second kind table decides whether to call
53C060. That selector's result is stored globally;8/9 select group13, and0..7
map to groups8/13/2/1/11/5/1/10. The kind is read again at this stage. Other
completion paths retain the group and only store the result's low BYTE as the
actor message. Invalid kinds store messageFF and sharedFFFFFFFF but retain
`zero_extended_kind-1` as the full native EAX return. This differs from returning
the shared result and is preserved by the result/frame APIs.

## Impossible native branches

Kind88's comparison at53CBB3 and kind89's comparison at53CCA7 each compare a
zero-extended BYTE with500. Their high branches cannot execute: instructions
53CBB5/53CBBA and53CCA9/53CCAE are consequently dead. The native comparisons
against600 for kind8 and800 for kind33 also have fixed false predicate results,
but their SET/ADD instructions remain executable and are covered. At53CD0C,
the same cached ECX has already passed the nonzero guard and is unmodified;
its zero edge is impossible. These constraints explain the finite coverage
qualification without substituting guessed wider fields or repairing native
behavior.

## Verification

`direct_c/verify_actor_message.py` authenticates all976 owned instruction
addresses and five switch tables against the original PE bytes and exported
REA evidence. The reference decompilation is retained separately; its generated
types do not override instruction widths. Reports compare native, named result,
named void, canonical void and raw frame routes. The result/frame routes check
full EAX; void routes check the effects expected by the native caller.

All256 BYTE kinds, signed/nonpositive and wrapped positive actor indices,
counter wrap/limits, states, signed coordinate boundaries, city thresholds,
linked kinds and all selector outcomes are exercised. Deterministic probes
check counter reloads, cached shared results and child-induced changes to the
owner/building/trader or actor kind/state. These are load-phase probes rather
than a claim about concurrent gameplay.

Town iterator and calendar children always execute. Other algorithm results
are controlled for isolated decision coverage; actual compositions execute
every reached direct child and its normal descendants, including the weighted
builder/list/calendar path. These child instructions are not included in the
parent's owned instruction count. Tests compare ordered owned static stores,
consumed child arguments, the entire original static-memory range, saved ABI
registers and stack cleanup. Private scratch and incidental flags/volatile
registers are excluded. Invalid IDIV exception correspondence, live UI and the
rest of the game remain unfinished.

```powershell
python direct_c/verify_actor_message.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_actor_message.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/audit_processing_receiver.py
python direct_c/verify_labor_access_current.py
```

Both Release executables pass1172 scenarios/5860 invocations each. All972
feasible owned instructions,205 conditional outcomes and49 targets across five
switch tables execute. The four other owned instructions and three impossible
edges have the native dataflow constraints above. Thirteen actual compositions
execute the complete reached child closure without substituted algorithm results.
All twelve earlier paired suites pass again against the new binaries; the
actor-message checkpoint checked26 reports; the voice follow-up expands the
voice checkpoint to28; the subsequent stream checkpoint reaches30.

The paired `actor-message-{diagnostic,game}.json` reports bind final sources,
executables, scenario counts and coverage qualifications by hash. The combined
manifest retains `complete_game_reconstruction=false`.
