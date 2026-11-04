# Ambient/sample receivers and stream fades, 2026-11-04

Five canonical entries now use named C.02706/02712 no longer contain
uninitialized `in_ECX` variables: they take an explicit `RestoreSound *`.
The existing exact immediate-fade algorithm is moved out of the unrelated
campaign unit into `runtime/sound_fades.c`. Both incremental fade bodies replace
decompiler field types, pointer scaling and signed-overflow expressions.

| Native entry | Named provider | Instructions |
| --- | --- | ---: |
| 0055F3EB | restore_sound_set_ambient_volume | 30 |
| 0055F67E | restore_sound_set_sample_volume | 10 |
| 0053B690 | restore_sound_fade_to_stop | 33 |
| 0053B6F0 | restore_sound_fade_out_step | 62 |
| 0053B790 | restore_sound_fade_in_step | 54 |

## Receivers and publication

55F3EB publishes the full input DWORD at self+158 before checking disabled.
If channel2's stream at self+1E0 exists, status4 leads to set-volume using a
fresh handle load and the original input volume. Its canonical entry and
settings alias share the same publication helper as music/voice.55F67E stores
the input DWORD at self+150 unconditionally, including when disabled is nonzero.
Neither method clamps volume. Native RET4 becomes explicit-self cdecl in C.
REA found no analyzed direct incoming references to these two canonical entries;
this does not establish that all indirect references are absent.

## Exact fade behavior

All fades read the sound-enabled **DWORD** atF55B88 and the stream DWORD at
E92DD0+channel*4. The old decompiler macros read BYTEs and scaled a DWORD pointer
by another4, yielding16-byte channel steps. The new unsigned32-bit address
arithmetic preserves negative/wrapped channel aliases. Native signed channel
comparisons are kept separately from effective addresses.

53B6F0 checks playing status, obtains the backend volume and subtracts5 with
32-bit wrap. **JNS tests the result's sign bit**, so INT_MIN-5 yields7FFFFFFB
and continues with that large positive volume. A conventional `volume>=5`
check would change this behavior. A negative wrapped result closes the stream.
Inactive/missing/closed channel0 clears the whole DWORD atF55B8C; channel1
clears the whole DWORD atF55B90. Other channels leave those flags alone.
Continuing with a volume update returns without clearing them.

53B790 adds5 with32-bit wrap. Exact channels0/1/2 compare that signed result
against full signed configured DWORDs atE38EC8/E38ECC/E38ED0 and cap only when
greater. Other channel values are uncapped, even when their wrapped address
aliases a standard slot. Configuration is read after get-volume returns,
preserving callback changes. No lower clamp or overflow repair is introduced.

53B690 obtains volume once, repeatedly subtracts5 while the cached signed value
is positive, sends every intermediate value and then closes. Final values-1
through-4 are possible and preserved. It does not reread backend volume after
each step, and it does not clear the incremental-fade flags. Positive INT_MAX
would require429496730 iterations; the production code has no new iteration
limit. Tests compare its first three actual set-volume calls as a stopped
prefix, without claiming that the full extreme run completed.

## Verification

Both Release executables pass732 scenarios/2310 invocations each. All189 owned
instructions and52 conditional outcomes execute.284 cases invoke actual
stream-method children. Only the seven Miles endpoints are controlled; no
algorithm result is replaced. Named APIs, canonical aliases and the retained
ambient settings alias compare ordered writes, method/backend inputs, all
original static memory, private receivers, matching memory-fault addresses and
saved ABI. All original APIs here are void, so incidental EAX is excluded.

The corpus includes DWORD enabled256, high flag bytes, exact channel strides,
negative/wrapped channels, signed caps above255, signed wraparound, all gate
paths, small repeated fades and callback-induced handle/configuration changes.
Five low-page/unmapped cases compare corresponding faults. The INT_MAX case
stops after three backend updates; other successful cases return normally.
The seven already-reviewed stream methods execute as dependencies and are not
counted again in the189 owned instructions.

`sound-fade-native.json` and REA evidence authenticate the original PE bytes
against SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Paired JSON reports bind final C sources and executables by hash. Physical
sound, arbitrary invalid pointers, post-corruption returns and concurrent
execution remain outside this finite proof. Whole-game reconstruction remains
unfinished.

## Settings trace/plain integration

The settings source-only fixture now aliases all shared public sound providers
and their header prototypes in both translation units. Unused Miles endpoints
fail explicitly if unexpectedly reached. The production publication macro
defaults to the same volatile DWORD store; the trace fixture overrides it to
observe the154/158 publications owned by its native comparison. This preserves
one production algorithm while retaining exact owned-write order in the oracle.

Debug and Release settings executables build and pass their smoke checks. The
full production settings comparison passes8625 cases against Debug/Release,
trace/plain C variants, with531 native instructions and108 conditional outcomes
covered (458 instructions belong to that corpus's owned methods). Four
difficulty recalculation children,4CDE40, PostMessage delivery and media calls
retain the explicitly controlled scope in that separate settings report.

```powershell
python direct_c/verify_sound_fades.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_sound_fades.py build/c-only/Release/PharaohRestoreC.exe
python tools/verify_settings_callbacks_emulation.py --production --exe build/c-only/Debug/restore_settings_callbacks_verify.exe --optimized-exe build/c-only/Release/restore_settings_callbacks_verify.exe
python direct_c/verify_labor_access_current.py
```

The combined current manifest checks32 paired reports and separately validates
the full settings corpus's source/executable bindings. Those results are finite
behavioral proofs, not completion of all catalogued procedures or gameplay.

The following retained scope is ambient classification53B830 and the225-instruction
ambient controller53B960. REA exposes the original dispatch/remap tables, DWORD
priority/cache/owner fields, city stride49D8 and the industrial x87 comparison.
The latter has two branch outcomes reaching the same already-pushed industrial
filename; neither an alternate track nor the semantic identity of metric5F9058
is inferred. See `sound-ambient-next-native.json`.

The ambient profile/controller scope above is now completed in
`SOUND_AMBIENT_RECONSTRUCTION.md`. Its current combined manifest has34 paired
reports across seventeen suites, and separately checks full settings and
production ground-render corpora. Sample-bank/file-registration recovery is
the next retained scope; see `sound-sample-next-native.json`.
