# Ambient profiling and stream selection, 2026-11-04

The remaining canonical classifier/controller no longer use decompiler field
macros or implicit pointer scaling. The already exact reset implementation is
shared through the named profile provider. The main controller is ordinary C
except for a narrow native x87 calculation in `industrial_comparison`; that
numeric boundary remains explicit and is not presented as portable pure C.

| Native entry | Named provider | Owned instructions |
| --- | --- | ---: |
| 0053B670 | restore_sound_reset_ambient_weights | 8 |
| 0053B830 | restore_sound_building_ambient_class | 17 |
| 0053B960 | restore_sound_update_ambient | 225 |

`runtime/sound_ambient_profile.c` contains the reset and building classifier;
`runtime/sound_ambient.c` contains the selection controller. Canonical entries
are typed facades. These sources compile with /W4 /WX /O2. Classification keeps
all cases of the existing correct switch, authenticated against all226 native
remap bytes and the five-target table. Out-of-range unsigned inputs return0.
The tables are treated as immutable; arbitrary executable-table patching is not
part of this C lowering.

## Fields and choice

F55B50 contains12 signed DWORD weights. A scan starts with best0/index-1, uses
strict greater-than and keeps the first positive maximum. Zero/negative weights
are never candidates. F55B80 is the full DWORD cached index; F55B84 is a separate
DWORD total. Reset clears the12 weights and total, leaving the cache untouched.
Sound-enabledF55B88 is also a DWORD; ambient-enabledE38EC0 is a BYTE.

When the cached index matches and the real channel2 status method reports
playing, the controller obtains the signed backend volume. Volume>=100 returns;
a smaller value invokes the actual53B790 fade-in step. That child rereads its
own gates, stream status, volume and configured cap. The fixed100 test in the
parent is retained even if the configured cap is lower or higher.

Changed/stopped selections publish the cached DWORD before opening/closing.
The local choice survives callback changes to priorities/cache/owner. Every
new open (including backend failure) or default close is followed by the actual
channel2 set-volume method with0. Existing stream methods perform their own
disabled/handle checks; the parent does not invent success/failure handling.

| Selected index | Filename/behavior |
| --- | --- |
| 1 | housing1/2/3/4/5, signed population thresholds500/2000/5000/8000 |
| 2 | industrial, including both outcomes of the original x87 comparison |
| 3 or7 | meadow |
| 4 | monument_b below signed metric1000, otherwise monument_f |
| 6 | desert |
| 8 | floodplain |
| 9 | rock |
| 10 | water |
| 11 | vegetation |
| -1,0,5 | close channel2, then set its volume0 |

Population is the signed DWORD atEA1894+DWORDownerEAAC28*49D8. Raw unsigned
address arithmetic preserves native32-bit wrapping and does not add an owner
range check. Metric5F9058 is a signed DWORD; REA found only the analyzed read
at53BBDD, and its higher-level identity remains unproved. The former BYTE
macros and extra DWORD-pointer scaling were incorrect.

## Industrial numeric boundary

Original code performs signed FILD ofF55B58 andF55B84, multiplication by binary64
at570158, and FCOMPP. Its subsequent status branch reaches either53BAF0 or53BBF7.
Both targets call55F449 with the same industrial filename already pushed by
53BAD3. No alternate industrial track is inferred. The C helper retains the
FILD/FILD/FMUL/FCOMPP sequence, without a C double spill or precision/control
reset. The discarded status transfer and convergent branch need no C result.

## Verification and remaining scope

The offline oracle authenticates native procedure bytes and all tables from REA
against the original PE SHA256
fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
It compares named and canonical C paths against native execution. Only seven
Miles backend endpoints are controlled; all reached stream and fade algorithms
execute. Full static bytes, an extra guarded readable city-field page for owner256,
ordered stores/method/backend inputs, classification EAX, normal-return saved
registers/cleanup and corresponding population-read faults are compared.

The corpus covers all classification entries, strict ties/nonpositive weights,
full DWORD gate/cache/owner values, population/metric boundaries, wrapped owner
aliases, signed volume thresholds/caps and deterministic backend mutations.
Industrial cases compare complete x87 CW/SW/tags and live FP80 registers across
PC24/53/64, all four rounding modes, inherited live depths0/2/6 and masked stack
overflow at depths7/8. Relocated FIP/FDP and unmasked host trap delivery are not
claimed. The direction flag is clear at the native ABI boundary.

Both Release executables pass989 scenarios/2967 invocations each, with all250
owned instructions,30 conditional outcomes and15 switch targets covered.
629 scenarios reach actual child-method compositions. The report's233
`fp_scenarios` counts industrial-selection fixtures: it includes the216-case
precision/rounding/live-depth matrix, masked-overflow/status fixtures and other
controller inputs; one cached same-index fixture bypasses the numeric block.
This counter is not a claim of233 executions of the industrial calculation.

The ground renderer fixture shares the exact production profile source, with
local symbol aliases. Its existing pixel spies now match the current const-void
signatures of40FED0/40FF50; a corrupt-grid diagnostic fails explicitly if reached.
No game algorithm is replaced by these fixture integrations. The source-only
full production ground corpus passes5077 cases and checks940 main instructions and43 helper
instructions. Only246 of1076 sprite-dispatch instructions are reached: file
loading and final pixel consumers remain outside that composition's scope.

Full-game reconstruction, physical playback, concurrency and arbitrary corrupted
inputs remain unfinished. The next retained scope is sample-bank loading and
file registration:539C60/53BC80/53BCF0/55DD3F plus560280/5602B0 ownership helpers.
REA already shows nonnative safety checks in the older55DD3F C. See
`sound-sample-next-native.json`; strcmp is authenticated as a CRT reference,
including explicit unreachable padding, rather than counted as a new game body.

```powershell
python direct_c/verify_sound_ambient.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_sound_ambient.py build/c-only/Release/PharaohRestoreC.exe
python tools/verify_ground_render_emulation.py --production --exe build/c-only/Release/restore_ground_render_verify.exe
python direct_c/verify_labor_access_current.py
```

The sample loading/registration closure is now documented in
`SOUND_SAMPLE_RECONSTRUCTION.md`. The current combined manifest has38 paired
reports across nineteen suites plus separate full settings, ground and
Debug/Release table corpora. The next scope is the remaining file-table
lifecycle and array/frame callback contracts.
