# Timed records, actor work state and category weights, 2026-11-04

Three former decompiler/frame algorithms now use named ordinary C in
`runtime/message_category.c`. The two existing exact calendar providers in
`runtime/town_record_queries.c` also replace duplicate instruction models in
the actor-message dispatcher. All five old frame entries delegate to these
shared algorithms. The public495B60/53C060 aliases call the named functions
directly;02506 now wraps the named weight builder with explicit pointer inputs.

| Native entry | C provider | Owned instructions |
| --- | --- | ---: |
| 0044D6D0 | restore_event_is_future_result | 17 |
| 0044D700 | restore_event_within_window_result | 31 |
| 00495B60 | restore_actor_building_work_state | 59 |
| 0053D3A0 | restore_message_build_category_weights | 263 |
| 0053C060 | restore_message_choose_weighted_category | 63 |

These descriptive names are inferred from behavior. Exact message/category
meanings and original source identifiers are not asserted.

44D6D0 adds two signed WORD years, compares the signed current DWORD year,
then compares the DWORD current month with the record's zero-extended BYTE
month if the years match. Its nonmatching-year returns retain the summed
year's high24 bits.44D700 rejects either record flag01 or10, preserving the
incoming EAX high24 bits on that early path. Future dates are rejected. Its
other path computes wrapped32-bit elapsed months and compares them, signed,
with the record's signed WORD duration. The raw frame passes inherited EAX
explicitly; no uninitialized C register variable is introduced.

495B60 sign-extends the actor's linked building WORD. Buildings outside status1
or types75/192 return1. It checks exactly three signed WORD actor links, accepts
only actor status1, and returns1 for states9/11/15/14 or2 for10/12/13/16.
Other links/states are skipped; exhausting all three slots returns0. Names
such as "working" or "idle" are deliberately not assigned to these numeric
states without evidence from their producers.

53D3A0 first performs two DWORD zero stores to the eight output bytes. It
walks the town list twice. The first pass stops at the first flags10 record
and reads its flags04; a later flags14 record cannot override that first
match. The second pass stops at a record accepted by44D700. Its iterator and
date receiver are explicit. Each category reloads the current city as in the
original; category3 caches one city for its two ten-row sums.

The eight categories derive weights from EA1888, EA4388, the timed-record
result and suppression fields, EA43A0/EA439C labor sums, five signed BYTEs at
EA5C0A, EA5B48, EA4468/EA1894, and EA5BF4. Thresholds, signed comparisons,
wrapped multiplication and signed division follow the native instructions.
In category2, a current record plus either suppression condition leaves EBX
unchanged: it inherits category1's weight. It does not reset that weight to1.
The output-default flag is cleared before each applicable weight store;
category5's blocked result0 does not clear it. The cached return sum is not
recomputed from output memory, which matters when that flag aliases a weight.
Afterward, categories3 and6 exclude the category whose weight is1 when the
other has a positive weight; the cached sum decreases by1.

53C060 calls the builder, sums eight separately truncated percentages, then
computes `selector = D3B374 & percent_sum`. It neither advances that state nor
uses modulo. The default flag is tested after this first percentage pass;
a nonzero flag returns9. Otherwise a second pass reloads the weights and
selects the first signed cumulative percentage at least equal to the selector.
The fallback is category7; a selected weight exactly1 returns8 instead.
The explicit invalid-division path aborts rather than supplying a fabricated
percentage. Correspondence of this abort with the original processor's divide
exception is outside the normal-return verification.

## Verification scope

`direct_c/verify_message_category.py` authenticates all433 instruction addresses,
the switch table and bytes with `message-category-native.json` and exported
REA evidence for original SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
It compares native, named C, canonical alias and raw frame routes. Full EAX,
ordered owned static writes, consumed child inputs, the complete original
static-memory range, saved registers and stack cleanup are checked. Iterator
and calendar children execute. Selected weight-builder results are controlled
only in isolated selector cases; actual compositions execute the entire reached
builder/list/calendar chain.

Deterministic memory hooks change city after an output write or weights at the
random-state read to check live reloads and the fallback. These are load-phase
probes, not a concurrency guarantee. Three analytically unreachable edges are
53D3F3→53D404 (the matched pointer is already nonnull),53D445→53D6BF (category
index stays0..7), and53C0DD→53C0E4 (the same final signed comparison already
failed before loop exit). The instruction at53D6F5 is reachable when the output
flag aliases weights and clears category6 after it was stored.

```powershell
python direct_c/verify_message_category.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_message_category.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

Both Release executables pass671 scenarios/2684 invocations each, with all433
owned instructions,127 feasible conditional outcomes and eight switch targets.
Seventeen compositions execute the actual reached builder/list/calendar chain.
The paired JSON reports bind the sources and binaries by hash. The combined
current manifest checks24 reports across twelve suites.
Private scratch, incidental flags/volatile registers, invalid division paths,
and complete gameplay remain outside this finite verification. The parent
53C100 controller is now reconstructed separately; see
[ACTOR_MESSAGE_RECONSTRUCTION.md](ACTOR_MESSAGE_RECONSTRUCTION.md). Other
city/input/actor models remain unfinished.
