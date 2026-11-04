# Sound sample banks, registration, ownership and byte comparison, 2026-11-04

Six sample/ownership entries and the shared byte comparator now delegate to named C providers. The two ownership
methods take explicit self pointers; no uninitialized ECX surrogate remains in
those definitions. Game logic uses wrapped32-bit addresses and volatile DWORD
accesses. The file-registration provider retains cleanup using MSVC C
`__try/__finally`; there is no new assembler in this closure.

| Native entry | Named provider | Owned instructions |
| --- | --- | ---: |
| 00539C60 | restore_sound_load_file_id | 48 |
| 0053BC80 | restore_sound_load_ambient_sample | 39 |
| 0053BCF0 | restore_sound_initialize_ambient_samples | 213 |
| 0055DD3F | restore_sound_register_file | 105 |
| 00560280 | restore_owned_buffer_initialize | 12 |
| 005602B0 | restore_owned_buffer_release | 20 |
| 005631B0 | restore_byte_string_compare | 54 |

## File ID and bank loading

The loader tries fopen(filename,"rb"). On failure it uses the native sprintf
ABI with format `%c:\%s`, drive BYTE E38E62 and a260-byte stack buffer, then
retries. Both failures returnFFFFFFFF. A successful original path is borrowed;
a successful fallback is registered with copy=1. The actual global file-table
method runs before fclose, and its result survives fclose's return value.
No length bound/truncation is invented for the original unbounded sprintf.
Long overflowed stack returns are outside this finite proof.

An ambient bank is48 bytes: ten variant IDs, a default ID at+40 and signed count
at+44. Mode0 loads and stores the default. Mode1 checks count<10 before loading,
then rereads count for the ID destination. The ID store precedes a fresh count
read/increment, preserving aliases when the destination is the count itself.
Other modes do nothing. Failed loads still publishFFFFFFFF and consume a
variant slot. Neither negative bank/count nor wrapped aliases gain new guards.

Initialization resets all six banks before the first load. It then performs48
loads in exact native order: default plus seven variants for desert, water,
rock, farm, flood and forest. Remaining variant IDs stayFFFFFFFF; ordinary
completed banks have count7 even when media paths were unavailable.

## Exact registration and cleanup

55DD3F checks signed count>=capacity before searching, so even an existing name
fails when the table is full. A null entries-array pointer rejects registration;
individual null-name slots are skipped. The loop reads
fresh count/table pointers and executes the actual case-sensitive strcmp child.
An append uses the cached original index, but reloads the table pointer for
each name/ownership store and reads count again for its final increment.

Removed deviations from the former C implementation:

- It no longer rejects negative counts or null paths before the native gates.
- Duplication failure stores a null pointer, stores the exact raw copy flag,
  increments the fresh count and returns the cached index.
- It no longer caches loop bounds/entry pointers across reached calls.
- Count/index arithmetic preserves native32-bit wrapping and field aliases.

560280 writes ownership0 first, then pointer0, and returns self.5602B0 checks
ownership and pointer, reloads the pointer for free, and clears only pointer
after free returns. Ownership is retained, including callback changes.

The file-table guard is initialized and destroyed on normal paths. A C finally
also expresses cleanup on exceptional exits. Native/compiler frame layouts and
handler addresses differ. Both FS chains restore on all verified normal
returns; arbitrary host exception dispatch, corrupted caller frames and full
C++ unwind equivalence remain separate obligations.

## Verification

Both Release executables pass473 scenarios/1419 invocations each. All437 owned
instructions and28 conditional outcomes execute.370 cases reach actual child
methods. No game algorithm result is substituted: loaders, registry, ownership
helpers and case-sensitive strcmp execute; only fopen/fclose/sprintf/strdup/free
are controlled CRT boundaries. The native ABI fixtures enter with DF clear.
Real filesystem/device behavior is not claimed.

The oracle compares all original static bytes, guarded private table/heap bytes,
ordered stores, meaningful child/CRT inputs, consumed full EAX, normal-return
saved registers/cleanup and corresponding explicit memory faults. Temporary
stack addresses are normalized by role. Cases cover signed/full DWORD gates,
case and alignment differences, cached negative indices, null input, failed
copy insertion, live pointer/count publication, bank/count aliases and callback
changes during file opening, duplication, close and free.

The older table-only corpus is extended to5600 cases and now executes native
strcmp as well. Debug and Release use the same production source; only strdup
is controlled in that separate corpus. The old source-only assertions for
failed allocation and negative counts have been corrected to native behavior.
Debug is also buildable through the main CMake target: optimization for the new
sources is selected for Release, avoiding an /O2 versus /RTC1 conflict in Debug.
The separate `tools/sound_file_project` remains a reproducible standalone Debug
fixture. Its smoke checks preserve an explicit failure if the unused temporary
guard unexpectedly owns an allocation.

The caller audit authenticates the48 direct bank-loader calls and the sole
analyzed initializer call. The latter is followed by53AF90, which replaces EAX
withFFFFFFFF before value-dependent use. Incidental EAX from the void APIs is
excluded; indirect-reference completeness is not claimed.

`sound-sample-native.json` and exported REA evidence authenticate the original
PE SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
The paired reports bind exact current sources/executables. The combined manifest
checks38 paired reports plus separate complete settings, ground and Debug/Release
table corpora. These are finite proofs; full-game reconstruction is unfinished.

## Unsigned-byte comparison regression

A direct counterexample exposed a separate decompiler bug in the shared
strcmp provider: two equal `FF 61 62 63 00` strings returned1 in the earlier C,
while original5631B0 returned0. Several equality tests promoted the right byte
through signed char. The new named C comparator uses unsigned bytes throughout,
returns exactly-1/0/+1 and retains native left-byte/word/aligned-DWORD reads,
right-byte reads and comparison-before-NUL ordering.

Its separate paired corpus passes5736 scenarios/17208 invocations per Release
binary, covering all54 analyzed instructions and34 conditional outcomes. All256
byte values run under all16 left/right alignments, plus mismatch/prefix/NUL and
five matched read-fault cases. Two post-RET padding instructions (3 bytes) in the
contiguous native range are excluded and remain unexecuted. No endpoint or child
is controlled in this pure comparator suite. The sample corpus also includes
actual duplicate registration for filenames beginning with127/128/129/254/255.
These54 instructions are counted once in the combined manifest, separately
from the six sample/ownership bodies'437 instructions.

## Next native scope

Constructor55DBC0, destructor55DBF8, clear55DC18, resize55DC63, deleting destructor
560210 and generic array callbacks still contain implicit receivers or behavior
changes. The existing resize C adds a nonnative upper bound and differs in
failed-allocation publication. Native array destruction decrements count before
testing its sign; INT_MIN wraps and cannot be safely replaced by while(count>0).
Frame-based cleanup thunks need explicit frame contracts. See
`sound-file-lifecycle-next-native.json`; this closure does not claim those
remaining algorithms or exception paths are already reconstructed.

```powershell
python direct_c/verify_sound_samples.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_sound_samples.py build/c-only/Release/PharaohRestoreC.exe
python tools/verify_sound_file_lookup_emulation.py --exe build/c-only/Debug/restore_sound_files_verify.exe
python tools/verify_sound_file_lookup_emulation.py --exe build/c-only/Release/restore_sound_files_verify.exe
python direct_c/verify_labor_access_current.py
```

The following file-table/array lifecycle scope is now documented in
`SOUND_FILE_LIFECYCLE_RECONSTRUCTION.md`. The combined current manifest has40
paired reports across twenty suites and separately validates initialization and
C SEH contracts. The next retained scope is building/UI/actor sound banks.
