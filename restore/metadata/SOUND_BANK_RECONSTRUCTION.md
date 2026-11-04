# Building, UI and actor sample banks, 2026-11-04

Seven native entries now have named, typed C providers in
`runtime/sound_banks.c`. Six replace decompiler bodies; UI playback was already
ordinary C in the linked `mission_unlock.c`. Its canonical entry now delegates
to the shared provider. The unused `unit_02489.c` facade delegates to the same
provider. The active binary never used that old decompiler's late file-ID read.
Link-map ownership must be checked before treating a legacy source as active.

| Entry | Provider | Native instructions |
| --- | --- | ---: |
| 00539CF0 | restore_sound_load_building_sample | 35 |
| 00539D60 | restore_sound_initialize_building_samples | 1062 |
| 0053AF70 | restore_sound_load_ui_sample | 7 |
| 0053AF90 | restore_sound_initialize_ui_samples | 67 |
| 0053B090 | restore_sound_play_ui_sample | 22 |
| 0053B100 | restore_sound_load_actor_sample | 10 |
| 0053B130 | restore_sound_initialize_actor_samples | 253 |

The bank layouts and traversal rules are recovered from REA assembly and bytes:

- Building storage starts at F54670: 237 rows of 20 bytes, with three variant
  IDs, one default ID and a signed variant count. All rows reset before the
  first load. The 252 ordered loads address 131 distinct banks and 197 path
  addresses. Mode zero sets the default; mode one checks count < 3 before the
  loader, rereads count for the destination, then rereads it again to increment.
  A callback or a destination alias can therefore change the final count.
  Failed file loading still stores -1 and increments the variant count.
- UI storage starts at F543B8: 100 IDs reset, followed by 19 loads in the
  original order. Slots 7/8 share build.wav; slots 17/18 load before slots
  11..16. Reordering changes registration IDs when files are duplicated or the
  table fills.
- Actor storage starts at F54120: 165 IDs reset, followed by 59 loads into
  33 banks of five IDs. The destination is `(bank*5 + variant)*4`, with native
  32-bit wraparound and no invented bounds checks.
- UI playback checks a DWORD global enable, signed index < 19, a BYTE effects
  flag and signed nonnegative ID. It caches the ID before writes to volume,
  pan, file ID, owner and loop count, in exactly that order. Negative indices
  are accepted, including indices that overlap those five request fields.
  The old unused decompiler read the ID again after stores; that copy is gone.

`sound_bank_loads.inc` retains all 330 calls in their original order. The new
oracle checks these tables independently against actual execution of the
original parents and their real registration children. Path names remain at
their original addresses in the mapped game data. C uses unsigned arithmetic
for address wraparound, signed tests where the native instructions do, and
volatile accesses for ordered global reads/stores.
`sound-bank-load-order.json` also lists the decoded filename for every call,
so addresses such as 5FAB40 are visible as `audio\wavs\hyena_r.wav` rather
than unexplained constants. Variant zero is the `_r` path in this example;
variants one, two and three load roam, attack and death sounds respectively.

`direct_c/verify_sound_banks.py` compares the original, named C provider and
canonical C entry for both Release binaries. It checks all static bytes,
guarded private table/heap bytes, ordered stores, child arguments, CRT/Miles
inputs, preserved registers, stack cleanup and restored FS chain. Only CRT
file/allocation and Miles endpoints are controlled. Native import-slot patches
are restored after execution before comparing static snapshots. Incidental
EAX from void APIs is excluded.

Each binary passes 799 scenarios / 2,397 invocations, reaching all 1,456 owned
instructions and all 16 conditional outcomes, with 366 real composition
scenarios. This includes all initializer calls, primary/fallback/missing/mixed
file access, capacity exhaustion, duplicate-copy failure, count mutations,
wrapped and negative indices, all five UI request aliases, DWORD/BYTE gates,
cached playback, duplicate suppression and an asserted actual failed backend
file read into an empty cache entry.

UI compositions execute the actual duplicate scan, channel search, cache
reference refresh, cache lookup and cached playback. Cold cache is exercised
at a null backend file-read failure. This does not certify the full cold-cache
replacement algorithm, all signed-byte channel/cache states, physical audio,
filesystem contents, concurrency or native OS exception dispatch.

Verification results are retained in `sound-bank-diagnostic.json` and
`sound-bank-game.json`; the current aggregate checks their source and binary
hashes alongside the preceding recovery suites. Full-game equivalence remains
unproved. The next five native playback/cache bodies (453 instructions) have
their own REA byte/assembly evidence in `sound-playback-next-native.json`.
