# Processing-site classification and permission scan, 2026-11-04

Four native entry points now share named ordinary C in
`runtime/building_processing_query.c`. This replaces the remaining4BF9D0
decompiler register input,487F80's decompiler-style partial return construction
and duplicate actor-message frame algorithms.459240's existing collapsed C
behavior is retained in the shared provider with its exact native load phases.

| Entry | Named algorithm | Owned instructions |
| --- | --- | ---: |
| 00487F80 | restore_kind_is_processing_type | 11 |
| 00488030 | restore_building_is_processing_type | 8 |
| 00459240 | restore_building_resource_is_permitted | 44 |
| 004BF9D0 | restore_city_has_unpermitted_occupied_building | 42 |

The descriptive names do not recover original source identifiers. The exact
processing membership is100..105,196 and224. Permission means the selected city
resource WORD is nonzero; this does not assign a guessed Boolean representation
or resource identity to the underlying storage.

487F80 retains the high24 bits of wrapped `kind-100` and sets AL from membership.
488030 first sign-extends the building's WORD type. Thus a direct kind65535
and a stored WORDFFFF have different full-EAX high bits, while both have AL0.
The former signed-overflow-prone decompiler expression is now unsigned32-bit
arithmetic. Native RET4 is normalized by typed C aliases and retained by the
existing raw frame dispatcher where required.

459240 reads the type, always calls488030, then reloads the type. Its initial
signed comparisons always clear BL: if kind>=106 the first edge clears it;
otherwise kind<106 makes the subsequent kind>114 condition impossible. The
classifier child neither changes this saved register nor can restore BL1.
Both native switch targets consequently reach the same lookup. The algorithm
reads city EAAC28 before the resource BYTE at799A1B+building*264, then tests the
WORD at EA4126+city*49D8+resource*2. It returns full-EAX0/1. Both type reads and
the child call are preserved, including observable child mutations. The original
selector/jump bytes are retained and verified even though the convergent C
algorithm needs no runtime code-table lookup.

4BF9D0 reads its explicit receiver's DWORD at32FB4 once. Exact values1/2/3
disable the scan; a matching low byte alone does not. It scans all indices1..3999
without consulting the building high-water mark. Status must be exactly1,
488030's AL and4894A0's AL must be nonzero, the WORD at799A8C must be zero,
and459240's **whole EAX** must be zero. That path returns1. Otherwise it clears
AL of the last native EAX value, retaining its high24 bits. Children can change
the record before subsequent reads; the mode and scan bounds stay cached.

## Explicit receiver repair

At4AB48C and53C383 the original bytes load ECX=D05720 immediately before CALL
4BF9D0, followed by TEST AL. The actor-laborer frame caller retains this receiver. The53C100 call now lives
in `runtime/actor_message.c`, whose typed call supplies D05720 directly. Its
canonical source02503 delegates to the named controller. The earlier generated
caller repair is retained as a narrow idempotent regeneration rule for historical
exports; its positive and negative samples remain audited. The native windows,
current typed/frame wiring and canonical delegation pass
`direct_c/audit_processing_receiver.py`. The whole controller follow-up is in
[ACTOR_MESSAGE_RECONSTRUCTION.md](ACTOR_MESSAGE_RECONSTRUCTION.md).

## Verification

Both Release executables pass260 scenarios/887 invocations each. All105 owned
native instructions,30 feasible conditional outcomes and four switch targets
execute. The two analytically impossible outcomes are459263→459267 and
459298→4592C2; the native source-value constraints above explain both. No native
instruction is omitted. Byte catalogues and REA records authenticate the corpus
against original SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

Tests compare full EAX, ordered owned stores, consumed child inputs, full
original static memory, saved registers and native cleanup through frames.
They cover all candidate kinds90..234 and signed32-bit boundaries, signed WORD
types, AL versus DWORD predicates, city/resource/record mutations, resource255,
WORD8000/FFFF, wrapped city offsets, full scan index3999, fixed bounds, explicit
and cached receiver state, and both switch arms. The kind leaf always executes.
Eleven parent/permission compositions run every reached real child without
substituted results. Other controller cases isolate callers with controlled
children; they do not certify every deeper-child instruction.

```powershell
python direct_c/verify_building_processing.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_processing.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/audit_processing_receiver.py
python direct_c/verify_labor_access_current.py
```

`building-processing-{diagnostic,game}.json` binds sources and binaries by hash.
The processing checkpoint checked22 paired reports; the category follow-up
expands the current combined manifest to24. Private stack,
incidental flags/volatile registers, concurrency and live UI remain outside
this finite proof. Full-game reconstruction remains unfinished. The calendar and category follow-up is documented in
[MESSAGE_CATEGORY_RECONSTRUCTION.md](MESSAGE_CATEGORY_RECONSTRUCTION.md). The
parent53C100 follow-up is documented in ACTOR_MESSAGE_RECONSTRUCTION.md.
