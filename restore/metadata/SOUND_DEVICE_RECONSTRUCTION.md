# PCM device, sound boot and audio global lifetimes

The recovered block has thirteen original entries and 474 owned native
instructions. Named C providers replace the PCM initializer, shutdown and
whole sound boot, and unify the sound/music constructors and their CRT
initialization/cleanup entry points. The original binary is bound by SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

| Original entry | C provider | Native instructions |
| --- | --- | ---: |
| 55E6C4 | restore_sound_initialize_digital | 130 |
| 55E92B | restore_sound_shutdown | 161 |
| 5394F0 | restore_sound_boot | 19 |
| 55E38B | restore_sound_construct | 107 |
| 539550 | restore_music_construct | 37 |
| 4CDF70/80/90/A0 | sound global initialize/construct/register_cleanup/cleanup | 10 |
| 4CDFB0/C0/D0/E0 | music global initialize/construct/register_cleanup/cleanup | 10 |

Four CRT/callback fragments (4CDF70/A0/B0/E0) are not defined as procedures
by the analysis provider. Their original CRT-table entries or callback
operands establish entry roles; exact REA bytes establish their instructions,
including the terminating tail jumps. Negative containing-procedure evidence
is retained. They are not counted as four newly discovered Ghidra procedures
or added to the structural catalogue denominator.

The PCM initializer preserves the unsigned platform/version gate, full version
result test, two Miles preferences, signed division of bits by eight and
wrapped format calculations. Original WORD/DWORD access widths matter.
Allocation and per-channel reset execute their actual C children. Each cache
store reloads the cache pointer, retaining alias and callback behavior. A
failed allocation leaves the original partial state; no rollback was added.

Shutdown preserves the disabled early return, signed fresh channel counts,
actual gated channel resets, 2D handle release and payload release, three
stream slots, optional 3D handle/provider cleanup, Miles shutdown and the
four 2D array frees. Heap frees occur in cache/sample/auxiliary/handle order.
Pointers are cleared after callbacks through freshly read fields. Original
untouched fields and unreleased 3D arrays remain untouched.

The native sound boot calls no constructors. It publishes the full PCM return
to F55B88 before resizing the file registry to 500, calls the actual music
setter/parser/range selection, stores -1 to F55B80 and runs building, ambient,
UI and actor bank initialization even when PCM initialization fails. The final
native transfer is a tail jump to actor-bank initialization. Primary and
drive-fallback fixtures exercise all actual file registration/ownership/array
and bank children, including 378 loads and 312 unique file IDs.

The constructors retain exact write order and leave unassigned fields
unchanged. In particular the sound constructor leaves sample counts alone;
the music constructor does not clear each category's ambient count. Its track
zeroing is performed as ordered DWORD stores, preserving the original REP
store effects without depending on host memset implementation choices.

The recovered constructor assignments include these exact values:

| Sound object offset | Meaning / native assignment |
| --- | --- |
| 000..008 | file table capacity/count/entries, initialized by actual55DBC0 |
| 00C..143 | 26 provider triples copied from604628 in DWORD order |
| 144 / 148 | enabled=1 / selected=-1 |
| 14C..158 | four volume values=100 |
| 164 | provider selector=5 (shutdown skips the 3D provider at this value) |
| 168 | zero; semantic name remains unresolved |
| 16C / 170 / 174 | raw float bits3F000000 /3D75C28F /3F000000 |
| 18C / 190 / 194 | rate22050 / bits16 / channels2 |
| 19C / 1A0 / 1A8 / 1AC | 2D handle/auxiliary/sample/cache pointers=0 |
| 1B0..1D7 | ten settings DWORDs=-1 |
| 1D8 / 1DC / 1E0 | three stream handles=0 |
| 1EC / 1F0 / 1F4 / 1F8 / 1FC / 200 | capability/3D arrays/environment/disabled fields=0 |

Unassigned counts, the digital handle, format bytes and remaining fields are
preserved. Names for unclassified fields are deliberately not guessed from a
zero assignment alone.

Native CRT table576008 places sound4CDF70 then music4CDFB0 between4BD300
and4D1340. The C-only main now invokes the once-owned adapters at that position
within its restored subset when entering the game path. They invoke actual
constructors and register actual C callback pointers. Arena cleanup is
registered first, so normal reverse cleanup runs music, sound, then arena.
The diagnostic path does not initialize these audio globals. The original
561953 atexit implementation is a declared CRT boundary, not a newly proved
allocator/registration algorithm.

Two retained pre-repair counterexamples are reproducible. A cache-array alias
faults on a DWORD write at address3 in the original and address11 in the old C.
At the first version-query entry, the original boot retains an existing handle
and cache array, while the old C's extra late constructor clears both and
increments the file-table allocation counter. The second comparison is an
intentional prefix and does not claim return ABI. The current paired oracle
contains the alias fault and a full boot with the populated prior object.

Both current Release binaries pass 348 scenarios / 1044 invocations each,
executing all 474 owned native instructions and all 44 conditional outcomes.
There are 260 scenarios with actual game-child compositions. The corpus
includes version/platform gates, format widths and signed division boundaries,
every pool allocation/handle failure, shutdown counts/providers/streams/cache
payloads, poisoned constructors, registration failures, reverse registered
cleanup, whole boot with missing/empty music configuration, full primary and
drive-fallback bank loading, callback/read-sensitive mutations and matching
null/alias faults.

At the device checkpoint the strict aggregate passed 50 paired reports across 25 families:
31,148 scenarios / 105,340 invocations. Its per-binary owned scope contains
9,682 native instructions: 9,678 executed and four separately proved dead
actor-message instructions. This is a finite recovered scope. The structural
inventory has 1,124 ordinary C catalogue entries of 4,402; that classification
does not itself prove behavior.

`verify_sound_device.py` compares actual native instructions with both named
and canonical C entries. It controls only OS queries/debug output, declared
CRT services and Miles endpoints. It compares all static/private/test-heap
bytes, ordered CPU stores, meaningful child/backend arguments, meaningful
full EAX, explicit fault addresses/widths and normal-return ABI/FS restoration.
Void incidental EAX and fault register/PC state are excluded. The paired
reports give exact corpus counts and instruction/branch coverage;
`audio-global-startup-audit.json` separately binds startup ownership and all
reached existing provider sources.

Physical audio, filesystem and allocator behavior, concurrency, complete native
CRT startup and OS exception dispatch remain unproved. Music.txt content
parsing beyond missing/empty files is outside this parent corpus. REA already
records the next parser/insertion/population-selection scope in
`music-configuration-next-native.json`; added bounds and path guards in those
existing providers need their own complete native audit. Full gameplay
equivalence remains unfinished.
