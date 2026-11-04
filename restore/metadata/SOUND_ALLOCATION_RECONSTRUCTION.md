# Native 2D sample-pool resizing, 2026-11-04

Canonical55DEA2 now delegates to `restore_sound_resize_samples` in
`runtime/sound_allocation.c`. The186-instruction native body is translated to
ordinary C with explicit signed tests, wrapped address/size arithmetic and
callback-sensitive DWORD accesses. It runs the actual stop-all and channel
reset members recovered in the preceding closure.

| Object offset | Array | Allocation size |
| --- | --- | --- |
| 19C | backend handles | count*4 |
| 1A0 | auxiliary DWORDs | count*4 |
| 1A8 | channel records | count*40 |
| 1AC | cache records | count*12 |

The original rejects only signed count <1. If an old handle array exists, it
stops channels through the real members, then resets/releases each old2D
channel and frees nonzero cache payloads. Cache pointers and counts reload
after callbacks; the cache data field is cleared after the free callback using
the fresh cache pointer. It subsequently frees the four arrays in the table's
order and clears each field after the free call.

New allocations publish into those fields in the same order. Each published
field is reread for its null check. On failure, the function immediately
returns0: previous successful allocations remain published, later fields stay
zero and the old sample count remains unchanged. No rollback is performed.
Only after all four allocations succeed is the new sample count stored.

Individual handle allocation walks the fresh signed sample count, publishes
through the fresh handle-array pointer and rereads the destination. A zero
handle ends the loop but still returns1. Already assigned handles and any
untouched tail bytes remain in place. The previous typed rewrite instead
released handles, freed all arrays and reset the count on failures, and added
an upper count guard absent from the binary. Those deviations are removed.

The retained previous executable and eight original/C mismatches remain in
`sound-allocation-before-counterexamples.json`. Both current Release binaries
now replay all eight failure settings successfully. Their separate regression
reports compare original and current C state, calls, EAX and ABI using matching
test-heap roles rather than historical physical pointer addresses.

Each current binary also passes361 differential scenarios /1083 invocations,
all186 owned instructions and all28 conditional outcomes, with91 actual
composition scenarios. Tests cover all16 combinations of old array fields,
each allocation-failure position, individual handle failures, old/new counts,
disabled high bits, raw wrapped sizes, pointer/count/backend mutations,
read-sensitive field publication, self aliases and explicit matching faults.
Only heap allocation/free and the five reached Miles endpoints are controlled.

The test allocator records full32-bit requests and exposes fixed guarded
mapped pages, so huge/wrapped requests can be tested through immediate failure
or a failed first handle without allocating gigabytes. Actual successful huge
traversals, physical allocator/audio semantics, concurrency and native OS
exception dispatch are not claimed.

The existing C initialization fixture also had assertions mirroring the old
rollback behavior. They now check retained allocation ownership and successful
partial handle allocation:265 assertions /18 scenarios pass in Debug and
Release. Its independent original/C corpus expands from355 to391 cases by
actually injecting all four heap failures and five handle failures through both
direct resize and the real PCM initializer, with/without an old pool.

Evidence: `sound-allocation-native.json`,
`sound-allocation-rea-evidence.json`, the two `sound-allocation-*.json` paired
reports, both `sound-allocation-regressions-*.json` reports and
`sound-allocation-host.json`. Device initialization/shutdown, their whole boot
parent and audio global lifetimes now have their own thirteen-entry, 474-native-
instruction audit in `SOUND_DEVICE_RECONSTRUCTION.md`. The three device/boot
bodies contain 310 instructions; the earlier preliminary count312 included
an incorrect boot instruction count. The rest of the game remains unfinished.
