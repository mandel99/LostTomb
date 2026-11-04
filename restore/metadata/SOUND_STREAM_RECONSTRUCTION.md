# Sound-stream methods and genuine voice closure, 2026-11-04

Seven native methods now share named C in `runtime/sound_streams.c`. Canonical
02704/02705/02707/02708/02709/02710/02711 are typed aliases. This removes the
added channel guards, stale handle caching and the remaining implicit backend
declarations from these providers. The existing exact voice-volume body is
reused through the shared publication helper.

| Native entry | Named C algorithm | Instructions |
| --- | --- | ---: |
| 0055F449 | restore_sound_open_stream | 77 |
| 0055F639 | restore_sound_set_stream_volume | 23 |
| 0055F561 | restore_sound_close_stream | 19 |
| 0055F5A1 | restore_sound_stream_is_playing | 28 |
| 0055F5F4 | restore_sound_get_stream_volume | 23 |
| 0055F38D | restore_sound_set_music_volume | 30 |
| 0055F32F | restore_sound_set_voice_volume | 30 |

The legacy name `restore_sound_music_volume_0055f32f` is retained as an alias for
existing figure/settings callers and their evidence. The native offsets show
that55F32F publishes channel1/field154, while55F38D publishes channel0/field14C.
The new public names distinguish the two without changing existing callers.

## Recovered behavior

Streams reside at self+1D8+channel*4, the digital driver at178, disabled at200,
and the open-time channel volumes at14C/154/158. All addresses use unsigned
wrapped32-bit arithmetic rather than out-of-bounds C array subscripts. This
preserves mapped aliases for signed negative or wrapped channel values.

55F449 rejects only **signed channel>2**, before reading self. The earlier C
also rejected negative channels. Enabled calls close a nonnull old handle,
clear its slot after the close callback, open with the freshly read digital
driver, and publish the returned handle. The subsequent nonnull test rereads
that slot. It caches the chosen volume after open, before loop-count setup.
Loop count, volume and start each reload the handle independently. Backend
callbacks can therefore replace it between those calls; the cached volume
does not change with later volume-field writes. Return1 is preserved even if
open returns0 or the controller is disabled. Only signed channel>2 returns
FFFFFFFF.

55F639,55F561,55F5A1 and55F5F4 have no native channel-range guard. Their former
lower/upper checks are removed. They test the full disabled DWORD first.
Volume/status queries return0 for a null handle; status is true only for the
exact backend DWORD result4, and get-volume forwards all result bits. Set-volume
tests then rereads the handle. Close deliberately calls the backend even for a
null handle, then clears the slot after that callback.

Music and voice publication write their configured DWORD before testing
disabled. If the stream exists, they call status with a fresh handle read;
status4 triggers set-volume with another fresh handle and the original input
volume. A callback's replacement handle is used even when it also changes
disabled or the published volume. No new clamps, retries or error returns are
introduced.

## Verification

`direct_c/verify_sound_streams.py` authenticates all230 instruction addresses
and exact bytes through `sound-stream-native.json` and REA evidence for original
SHA256 `fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Both Release executables pass532 scenarios/1658 invocations each, covering all
230 instructions and40 conditional outcomes. Native ECX receivers and
RET4/8/12 are compared with explicit C receivers and cdecl cleanup. Full EAX is
compared for open/status/get-volume; void APIs compare their consumed effects
and saved ABI rather than incidental volatile registers.

Tests include signed and wrapped channels, aliased slots, disabled values1/256,
null/nonzero handles, open failure, exact status4, full DWORD volume results,
private/global receivers and argument/order faults at low addresses. Backend
callbacks replace handles, driver, disabled and volume fields. Read probes
change a handle between its guard and consumed load. Deferred store probes
replace a just-published handle before its new test; these distinguish native
memory reloads from cached open results. The selected open volume remains
cached across loop-count callbacks.

Thirty real compositions execute539A70,53BFE0 or53D0B0 with all reached voice
and stream algorithms. Only seven Miles endpoints are controlled: close,
open, loop-count, set-volume, start, status and get-volume. No algorithm result
is substituted. Filename assembly, voice-volume publication, close/open and
playback configuration execute together, including backend failure, disabled
controllers and callback-induced changes. Parent voice instruction coverage
remains in the separate300-instruction corpus; it is not counted twice here.

Ordered static/object writes, method/backend inputs, all original static memory,
the mapped private object fixture, matching fault addresses/sizes, saved
registers and cleanup are compared. Deterministic mutation probes are not a
concurrency guarantee. Arbitrary unmapped channels, post-overflow filename
returns, physical audio devices and live gameplay remain outside this finite
proof. Whole-game reconstruction remains unfinished.

```powershell
python direct_c/verify_sound_streams.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_sound_streams.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

Paired JSON reports bind final source/executable hashes. This historical stream
scope is followed by the five-body fade/receiver closure documented in
`SOUND_FADE_RECONSTRUCTION.md`. The current combined manifest checks32 reports
across sixteen suites plus the separate full settings corpus. The next retained
scope is ambient classification53B830/controller53B960; see
`sound-ambient-next-native.json`.
