# Reserved actors and town links

The original `4BB090` register/flags model is replaced by
`restore_actor_initialize_reserved`. Its three observed direct callers discard
incoming EAX before meaningful use; the retained caller windows and the offline
EAX/AX/AL/AH liveness audit support the existing void interface.

The initializer clears all 2,000 actor records and town actor links, then makes
41 allocations with `(player=1, kind=36, x=-1, y=-1, direction=0)`. The tags are
`1,2,3`, an intentionally untagged fourth allocation, `12..32`, then `50..65`.
There is no added failure guard: a zero allocation result still addresses actor
zero for each original tag store. Return values and address arithmetic retain
their original 32-bit wrapping behavior. The meaning of kind36 is not inferred
from the number alone.

The named closure contains seven whole native bodies and 502 instructions:

| Original | C provider | Instructions |
|---|---|---:|
| `4BB090` | `restore_actor_initialize_reserved` | 279 |
| `4B4760` | `restore_actor_reset_pool` | 14 |
| `4B8C90` | `restore_actor_reset_town_links` | 27 |
| `4B4800` | `restore_actor_create` | 94 |
| `40F8F0` | `restore_memory_fill_bytes` | 22 |
| `448A60` | `restore_town_actor_links_00448a60` | 10 |
| `44BB90` | `restore_town_next` | 56 |

The allocator preserves the first available actor in slots1..1999, reservation
checks, 97 ordered DWORD clears, serial/coordinate truncation, actual RNG,
profile/pose children, optional history and the fresh signed highest-index read.
Existing C allocator behavior is given a named entry; naming is not counted as
an additional catalogue conversion.

The byte fill now uses forward DWORD stores followed by trailing BYTE stores,
instead of host `memset`. This retains store widths, order, wrapped addresses
and partial writes before a fault under the native C ABI's cleared direction
flag. `restore_town_next` caches its signed count once and uses wrapped addresses
rather than out-of-bounds C array indexing. Its fallback deliberately writes the
exhausted index's record but returns record140. The constructor's native loop
initializes all150 record IDs and makes record140 eligible; the fixture mirrors
that prestate. A malformed reserved ID can make the original caller loop; no
production guard was added to hide that behavior.

`direct_c/verify_actor_reserved.py` compares original native code with both the
named and canonical C entries in each Release executable. Private-receiver
iterator cases use its named entry because the old C facade selects the global
town list. It compares complete original static/private/guarded memory, ordered
CPU stores, logical child inputs, allocator/iterator results, normal return ABI
and explicit matching memory faults. The JSON reports record the final corpus
size, instruction/outcome coverage and exact source/binary hashes.

Parent protocol cases control only pool reset, town reset and allocation.
Allocator protocol cases control only glyph, pose and history children. Actual
composition cases execute every reached game algorithm. One separate town-reset
protocol case controls a null iterator result to exercise the initial null
guard; ordinary bounded lists return the reserved record on that first call.
Read-mutated states are
labeled synthetic RAM scenarios; they do not prove physical concurrency. No
platform or device endpoints are invoked, and no host game is launched.

Coverage does not certify incidental void return registers/flags, fault-time
registers, arbitrary caller-stack aliases, the OS exception dispatcher, host
allocation/device behavior or complete gameplay. Two remaining mission-spawn
models (`4BB3F0`, `4BBC10`) are retained as the next scope with native evidence.

The structural inventory now recognizes `__declspec` before return types. Its
reclassification of existing attributed bodies must not be counted as new
algorithm conversions. Exactly one register/flags model is removed here.
