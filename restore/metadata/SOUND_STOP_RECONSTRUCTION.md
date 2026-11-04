# Sample stopping and file-to-channel lookup, 2026-11-04

Four decompiler entries formerly read an uninitialized ECX. Their native object
receivers are now explicit C arguments. The existing stop-all parent delegates
to the real child members so their disabled checks and receiver forwarding run.

| Address | Provider | Native instructions |
| --- | --- | ---: |
| 0055EB99 | restore_sound_stop_sample | 19 |
| 0055EBD4 | restore_sound_stop_3d_sample | 19 |
| 0055EC0F | restore_sound_stop_looping_samples | 35 |
| 0055EC7C | restore_sound_stop_all_samples | 39 |
| 0055EFDC | restore_sound_find_channel_file | 27 |

The two single-channel stop members check the full disabled DWORD and return1,
including when no backend call occurs. Indices use native32-bit wrapped address
arithmetic. Stop-all checks disabled initially, then visits signed 2D and3D
counts and calls their respective members for every index. Each child rechecks
disabled. Counts and handle pointers reload after callbacks.

Loop stopping checks disabled only at entry. It queries each channel's full
backend loop-count result, and calls the 2D stop member only if the result is
zero. That child still performs a fresh disabled check. File-to-channel lookup
compares DWORD file IDs, returns the first matching index or-1 and has no
disabled gate. REA records no incoming references for the latter or the loop
stop member; this does not establish absence of indirect runtime calls.

A concrete previous linked binary is retained under `build/sound-stop-before`.
With three 2D and three 3D channels, the controlled first 2D end callback writes
disabled256. The original executes one 2D backend call and no3D calls. The old
typed stop-all parent directly called backend endpoints and executed all six.
The repaired parent executes exactly one call and still visits the native child
members. `sound-stop-before-counterexample.json` binds the old executable SHA,
native SHA, controlled mutation and observed counts; the old artifact is
historical counterexample evidence, not the current game build.

Each current Release binary passes383 scenarios /1149 invocations, reaching all
139 instructions and all20 conditional outcomes, with38 real compositions.
Only end-sample, end-3D-sample and loop-count Miles endpoints are controlled.
Tests include disabled high bits, negative/wrapped channels, signed counts,
negative/high-bit file IDs, mixed loop results, actual child forwarding, count
and handle-pointer mutations, the concrete disabled callback and matched null
object faults. Static/private bytes, ordered writes, backend and child inputs,
full meaningful EAX, stack cleanup and preserved registers are compared.

The tests do not certify physical audio, backend internals, concurrent behavior
or native OS exception dispatch. Broader usable gameplay remains unfinished.
The then-next186-instruction native 2D array-resize body is recorded separately
in `sound-allocation-next-native.json`: its native partial allocation state and
handle-allocation failure behavior differed from the older typed rewrite.
The historical probe retains a separate previous executable and
confirms eight concrete divergences: four heap-failure positions, two individual
handle failures and two upper-count/size-wrap paths. Three normal/invalid-count
controls match. Its report is `sound-allocation-before-counterexamples.json`;
it explicitly records `complete_reconstruction=false` and is not a passing
reconstruction oracle.
The subsequent [allocation repair](SOUND_ALLOCATION_RECONSTRUCTION.md) removes
these deviations and replays all eight settings against current C successfully.
