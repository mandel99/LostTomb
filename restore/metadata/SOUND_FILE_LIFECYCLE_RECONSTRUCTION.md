# File-table, array and exception cleanup lifecycles, 2026-11-04

Fourteen canonical entries now use named C providers. Lost ECX/EBP values are
explicit object/frame parameters. Generic array callbacks receive the actual
object address, constructor traversal advances forward, and destruction and
rollback traverse backward. The formerly missing fifth constructor argument is
an explicit destruction callback. No assembler is added by this closure.

| Entry | Named provider | Instructions |
| --- | --- | ---: |
| 0055DBC0 | restore_sound_file_table_initialize | 17 |
| 0055DBF8 | restore_sound_file_table_destroy | 12 |
| 0055DC18 | restore_sound_file_table_clear | 25 |
| 0055DC63 | restore_sound_file_table_resize | 65 |
| 0055E58A | restore_sound_destroy | 20 |
| 00560210 | restore_owned_buffer_delete | 42 |
| 00561363 | restore_array_destroy | 34 |
| 005613CB | restore_array_destroy_cleanup_frame | 8 |
| 005613E3 | restore_array_unwind | 30 |
| 00561441 | restore_array_construct | 36 |
| 005614AB | restore_array_construct_cleanup_frame | 8 |
| 0056E140 | restore_sound_file_allocation_cleanup_frame | 5 |
| 0056E155 | restore_owned_buffer_cleanup_frame | 3 |
| 0056E168 | restore_sound_file_table_cleanup_frame | 3 |

The catalogue is authoritative for individual instruction counts. The14
analyzed bodies own308 instructions; a separate authenticated7-instruction
fragment at563C7F implements the unwind exception filter. Ghidra does not define
a procedure at that fragment. Five exception-only instructions in the contiguous
5613E3 byte range are retained separately, outside this normal-body coverage.
They are not relabeled as dead or claimed to have executed.

## Table state and allocation

55DBC0 adds12 to the shared DWORD counter at12663B0, then zeros entries, count
and capacity in that order and returns self.55DBF8 subtracts12 before clearing.
Arithmetic wraps32 bits, including when the object aliases that counter. A null
object can fault after the counter changes; no new null guard conceals it.

55DC18 preserves capacity/count. It tests entries, rereads and caches the entry
pointer, deletes that saved array, then publishes entries0 after callbacks.
A deterministic read-phase mutation covers the second-pointer-null path.

55DC63 rejects only signed count<1, leaving the previous object untouched in
that case. Otherwise it clears first and requests wrapped count*8+4 bytes.
A successful block receives its count cookie and actual owned-buffer
constructors. The resulting entries pointer, including0 on allocation failure,
is always published. A fresh entries read decides success; only then are
capacity and count updated. The old extra upper-size bound, cached pointer
assumptions and missed failed-allocation publication are removed.

Cookie/block aliases with the table and callbacks changing its fields are
retained. A post-publication mutation to0 causes failure while leaving
capacity/count unchanged. Constructor failure rolls back completed objects and
frees the raw allocation via a C finally. The successful path does not add
extra cleanup, clamping or overflow repair.

The existing sound constructor now invokes the same table constructor instead
of duplicating its counter/field logic. The sound destructor uses C finally to
clear/destroy files after the actual shutdown method returns or propagates an
exception. Its original frame cleanup reads the object atframe-16 explicitly.

## Arrays, callbacks and frames

Owned-buffer elements are8 bytes. Public void callbacks adapt the existing
explicit-self constructor/destructor to the generic C callback type. The
canonical iterators use ordinary C calls; no callback result is substituted.

Deleting flags&2 selects an array: read the cookie atself-4, destroy in reverse,
optionally free self-4 when flags&1, and return self-4. The scalar path destroys
self, optionally frees it and returns self. The pointer is not adjusted in the
scalar path.

Reverse destruction computes wrapped base+stride*count and decrements the
DWORD count before testing its sign. In particular INT_MIN becomes INT_MAX:
while(count>0) would change the original. Normal negative counts and0 skip
callbacks; INT_MIN can require billions. Production code has no new iteration
limit. Tests stop after three real callbacks for that extreme path and for a
wrapped4-byte allocation with a huge positive constructor count. Readable
padding is a verification fixture, not a claim of safe original heap behavior.

Forward construction increments completed count and advances only after a
constructor returns. Cleanup starts at the current end and destroys only
completed predecessors. A failed reverse destructor similarly unwinds the
remaining predecessors, excluding the currently failing object.

Original finally thunks receive explicit original-frame views: completed flags
atframe-28/-32, constructed count atframe-28, arguments atframe+8 onward, and
cleanup pointers atframe-20/-24/-16. Their canonical C wrappers do not invent a
zero-valued frame or depend on an uninitialized EBP variable. New C iterators
express their own cleanup through compiler-generated SEH and typed locals.

The563C7F filter continues searching for non-C++ exception codes and reaches
terminate for E06D7363. Its terminate entry is compared as a stopped prefix;
5637EE's complete implementation and original OS dispatch remain outside this
proof. The C handler preserves that decision with an explicit code argument.

## Verification

Both Release executables pass312 scenarios/931 invocations. All315 owned
instructions and28 conditional outcomes execute.135 scenarios reach actual
array/ownership callbacks. Only allocation/free and the empty enabled-shutdown
Miles endpoint are controlled. Exact table/element/static bytes, ordered
stores, heap requests/free inputs, callback addresses/objects and normal saved
registers/stack/FS restoration are compared. Native callbacks use ECX; C
callbacks receive ordinary explicit parameters. Frame callback pointers are
normalized by semantic target, and temporary stack layouts are excluded.

Cases include counter wrap/fault ordering, capacity/count retention, callback
mutations, pointer publication timing, scalar/vector flags, zero/negative/signed
wrapped counts, forward/reverse strides, cookie/table aliases and explicit
normal/abnormal-marker frame cleanup. INT_MIN and huge-allocation runs stop
before the fourth callback. Their full extreme returns are not claimed.

Actual Windows C SEH also passes six contracts in both Debug and Release:
normal construction, partial constructor rollback, remaining-object destructor
rollback, propagated non-C++ exception, raw-allocation cleanup after a cookie
write access violation, and C++ unwind routing to a test-only nonreturning
exit77. This confirms C cleanup execution, not the original OS dispatcher or
5637EE termination internals. See `array-lifecycle-host.json`.

The pre-existing sound initialization corpus passes355 cases in Debug and
Release with real vector constructors/destructors; the old algorithm controls
for those iterators are removed. The separate5600-case file-registration
corpus passes in both configurations. Media/OS and allocator boundaries retain
their declared controlled scope. Full sound initialization failure/overflow
behavior beyond those finite cases is not inferred.

All new production units compile with /W4 /WX; Release uses /O2 and Debug keeps
its normal runtime checks. REA bytes are authenticated against original SHA256
fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
Paired reports bind final sources/executables. The combined manifest checks40
paired reports, separate full settings/ground/table/initialization corpora and
C exception contracts. Whole-game reconstruction remains unfinished; native
OS exception dispatch, the five retained exception-only instructions, other
legacy callback users and broader gameplay still need recovery/verification.

## Following retained scope

Building, UI and actor sample-bank loaders/initializers still use decompiler
bodies. The next seven entries include1456 native instructions.53B090 has a
concrete cached-ID/order issue: it accepts negative indices, which can alias
the request being written, while the old C reloads the ID after those writes.
Native code caches it and publishes volume, pan, file, owner and loop fields in
that exact order. See `sound-bank-next-native.json`.

```powershell
python direct_c/verify_sound_file_lifecycle.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_sound_file_lifecycle.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_array_lifecycle_host.py
python tools/verify_sound_initialization_emulation.py --exe build/c-only/Debug/restore_sound_verify.exe
python tools/verify_sound_initialization_emulation.py --exe build/c-only/Release/restore_sound_verify.exe
python direct_c/verify_labor_access_current.py
```
