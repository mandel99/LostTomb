# Figure codes, selection and voice filenames, 2026-11-04

53D280's remaining overlay instruction model now delegates to ordinary C
`restore_actor_figure_code`. Canonical02505 calls the same provider. The older
02504/02501 decompiler bodies now call the existing selection and voice-record
algorithms, which have shared result/void interfaces in
`runtime/figure_menu_actions.c`. No duplicate replacement algorithm is introduced
for the existing130-instruction selection body or44-instruction filename body.

| Native entry | Shared C provider | Owned instructions |
| --- | --- | ---: |
| 0053D280 | restore_actor_figure_code | 65 |
| 0053D0B0 | restore_figure_selection_result_0053d0b0 | 130 |
| 0053BFE0 | restore_actor_voice_variant_result | 44 |
| 00539A70 | restore_figure_voice_00539a70 | 61 |

These are descriptive names. The raw6012EC table code is also used by the figure
UI, while selection subtracts201 to derive its voice bank. Exact original
identifiers and all table-entry meanings are not asserted.

## Native field and sentinel behavior

53D280 caches the signed WORD building atBCD652+actor*388 before reading the
primary6012EC DWORD table entry. It loads kind fromBCD602. Positive primary
codes return directly. Primary-2 selects scenario DWORD types68/69/70 as
210/209/208 using the low BYTE of784CB0 and48-byte records. Primary-3 maps
4C5120 results0..4 to230..234; other results become-1. Primary-4 reads the
secondary kind atBCD603: secondary-3 uses training, secondary-4/-2/-1 become-1,
and values outside that four-value interval return unchanged, including0 and
negative values below-4. Other nonpositive primary codes normalize to-1.
Unsigned wrapped address arithmetic replaces signed-overflow-prone decompiler
index expressions. The query's full EAX and RET0 frame behavior are retained.

53D0B0 caches the selected actor from EADD24+EADCE4*4. Nonpositive signed actor
values return immediately. Nine special kinds first issue sound cues0..8, then
the actor kind is reloaded without rereading the selected actor. It publishes
the primary raw table code in EADCE0. Training failures reload EADCE0; this can
lead into the secondary-kind branch. Secondary-2 is not scenario-expanded or
normalized as in the query: it remains-2, then subtracts201. Primary-1 returns
before subtraction. Other accepted codes subtract201 with32-bit wrap, publish
that value, and use its sign bit to decide whether to play. The actor message
BYTE atBCD675 is sign-extended into EADCDC and the variant argument. Selection
and query must therefore not be collapsed into a generic normalized lookup.

53BFE0 accepts a signed positive bank and nonnegative variant. It zeroes32
scratch bytes, then copies the full terminating-NUL-inclusive filename in DWORD
groups followed by BYTEs. The general source is5FB16C+
`(variant-20+bank*20)*24`; bank108 instead uses60110C+variant*24. Wrapped32-bit
arithmetic and the original unbounded copy extent remain. The result interface
preserves inherited EAX on the two early-return paths; normal returns propagate
539A70's EAX. Canonical/named void interfaces match callers that discard it.

The physical data contains51 regular banks followed by the zookeeper records.
The generic bank52 formula aliases that last physical set;108 uses its explicit
special branch. Pinned primary mappings for figure kinds0..108 and the known
scenario/training expansions use positive banks1..51 or108. A continuous1..108 filename-bank
range would read unrelated tables/text. The retained bank56 native fault at
EIP20646574 documents this distinction. No filename clamping or invented error
return is added. Separate probes copy33 and41 bytes into the32-byte scratch and
stop at the next child before the corrupted return. They verify the original
untruncated copy extent, not post-overflow host stack-layout equivalence.

## Corrected first-volume read phase

539A70 tests the sound-enabled DWORD and voice-gate BYTE, copies the prefix to
F55A20, then appends the supplied name. Native539AD4 loads the first volume
DWORD after filename DWORD copies but before539ADF copies its remaining BYTEs.
The former C implementation loaded it after the entire copy. The shared provider
now caches the first volume before the BYTE tail. It passes that value to
55F639, reloads the volume for55F32F after the first call, and passes the fixed
F55A20 buffer to55F449. All receivers are E92BF8. Tests mutate volume during
the word/tail copies and between calls to verify these phases independently.

## Verification and limits

`direct_c/verify_actor_voice.py` authenticates all300 owned instruction addresses
and five switch tables against the original PE and REA evidence. It compares
native, named result, canonical and appropriate named void/frame routes. Full
EAX is checked for result/query/frame interfaces; void interfaces compare effects.
Ordered owned static stores, consumed child inputs, actual filename bytes and
zero padding, complete static memory, saved registers and stack cleanup are
checked. The four owned functions execute together in genuine compositions;
training and incidental-sound children also execute there. Enabled stream
controller methods55F639/55F32F/55F449 remain controlled algorithm endpoints.
This does not claim physical sound or full controller equivalence.

Coverage includes BYTE kinds, signed table sentinels, both training switches,
scenario masking, cached-building versus late-building reads, sound/selection
mutations, signed variants, wrapped actor/bank indices, filename offsets and
copy remainders, prefix/input overlap, gating, return values and volume reloads.
Both Release executables pass624 scenarios/2440 invocations each, including
16 genuine compositions and two stopped overflow-copy probes. All300 owned
instructions,72 conditional outcomes and22 switch targets execute. The voice checkpoint checked28 reports across fourteen suites; the stream
follow-up expands the combined current manifest to30.

Private addresses are normalized. Deterministic probes are not a concurrency
guarantee; post-overflow returns and live UI remain unverified. The paired JSON
reports bind final counts, sources and executables by hash. Whole-game
reconstruction remains unfinished.

```powershell
python direct_c/verify_actor_voice.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_actor_voice.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

Stream-method recovery and genuine voice/stream compositions are documented in
[SOUND_STREAM_RECONSTRUCTION.md](SOUND_STREAM_RECONSTRUCTION.md). The separate
actor-voice corpus retains its controlled stream-method scope described above.
