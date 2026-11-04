# Music configuration, track insertion and population selection

Five existing C providers are now unified behind named, native-derived C
algorithms. They cover494 original instructions.484 active instructions and86
conditional outcomes execute in the paired corpus; ten original CD-fallback
instructions are explicitly disabled at the user's request. This distinction
is preserved in the reports rather than counted as native equivalence.

| Entry | Named provider | Original instructions |
| --- | --- | ---: |
| 539250 | restore_music_load_configuration | 219 |
| 5395E0 | restore_music_add_track | 117 |
| 539730 | restore_music_select_population_range | 79 |
| 539A50 | restore_music_set_range | 5 |
| 561DB1 | restore_tokenize_byte_set | 74 |

The original binary SHA-256 is
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Exact bytes, Ghidra assembly and evidence IDs are retained in
`music-configuration-native.json` and `music-configuration-rea-evidence.json`.

## Recovered behavior

Insertion rejects a signed category greater than53 only. Category53 and
negative categories use the original wrapped row arithmetic. A normal/ambient
count is rejected only when exactly50/10, respectively. The source filename
is scanned before a forward DWORD copy and trailing BYTE copy, including its
NUL; no new path-length or null-pointer guard is introduced. Metadata stores
follow the copy in native order, and the final count is read afresh before
incrementing. This retains overlap, metadata overwrite, negative-index and
partial-fault behavior instead of the earlier added guards.

Population selection reads the full profile DWORD, signed category BYTE,
wrapped city/profile/track addresses and signed population fields. It retains
the conditional minimum/maximum reads, first matching track scan, subsequent
minimum scan, actual range-setter call and current/previous/waiting/phase stores.
No category, profile, count or current-track guards absent from the binary are
added. The range setter publishes first, then last, as separate DWORD stores.

The parser retains category/delay/minimum/maximum defaults across lines.
AMBIENT writes through its explicit receiver, while actual track insertion
always uses the globalE39180 manager. An AMBIENT marker at normal count50
overwrites the count itself with1. Missing type tokens cause the native byte
read fault; oversized paths retain original unbounded formatting/copy behavior.
The100-byte path and200-byte line buffers remain adjacent in that order.

The tokenizer builds all32 bytes of its delimiter bitset, including the NUL
bit, before reading the input/cursor. It skips delimiters, replaces a terminating
delimiter with zero and publishes the actual12663C8 cursor. Its earlier
null-cursor guard is removed. All256 byte values are exercised, including the
empty/NUL cases and high-byte delimiters.

## Requested CD exception

The user explicitly requested: “zachovej originální chování jen to nespouštěj
a zakomentuj, pro spuštění nebude vyžadováno CD ani externí skladby z CD”.
The original missing-local-file branch539470..53948C remains commented in
`runtime/music_configuration.c`. The compiled parser keeps the local relative
path and does not try a media-drive prefix, regardless of drive byte. Startup
does not require CD music. No original or restored host game was launched.

The ten instructions and incoming conditional outcome are recorded separately
as user-disabled. Three native/current-C cases (drive0,D,255) demonstrate the
authorized difference and assert matching named/canonical C behavior. They do
not contribute to equivalence instruction coverage. The compiled host fixture
also asserts that missing tracks retain a relative path even with driveD.

## Verification and limits

Each current Release EXE passes748 scenarios /2244 invocations, including203
actual child-composition scenarios and three CD-policy cases. All484 active
instructions and86 feasible active conditional outcomes execute. Three other
inline-strcmp outcomes are impossible for the immutable AMBIENT/A constants;
their exact edges and reasoning are retained. The owned bodies execute their
actual tokenizer, insertion and range-setter children. Only declared fopen,
fclose, fgets, sprintf and strtol services are controlled.

Comparison includes full static/private bytes, ordered CPU stores, meaningful
child/CRT inputs and results, tokenizer cursor, normal-return ABI and explicit
fault addresses/widths. Parser scratch pointers are normalized by their role
and offset. All four retained pre-repair insertion mismatches are reproduced
against the immutable previous EXE and replayed successfully against original,
current named C and current canonical C. Original report hashes are retained.

Debug and Release each pass294 host assertions in16 music scenarios and315
assertions in12 campaign scenarios. Their independent native corpora pass316
and333 cases, respectively. The music-tick fixture now compiles the real shared
volume provider in its own configuration and links the new population selector;
each configuration passes1668 original/C cases. Its clock, stream playback and
Miles endpoints remain controlled, so full real-stream integration is outside
that older corpus.

Two late CMake fixes affect only `restore_music_tick_verify`. The scope audit
retains the full earlier CMake/startup-audit records under their original hashes,
checks the exact whole-file delta and runs a current incremental main build.
Both tested main EXEs and their MAP files remain byte-identical. The aggregate
uses this explicit build-only compatibility evidence; it does not rehash old
test reports. Music-tick itself requires fresh strict current-source reports.
Any algorithm delta, unrelated CMake edit or changed production artifact fails
that exception. See `music-cmake-scope-audit.json`.

Physical CRT/files/audio, concurrency, arbitrary corrupted caller-stack aliases,
unbounded giant traversals, native exception dispatch and complete usable
gameplay remain unproved. The structural catalogue count is not behavioral
coverage. The next native scope targets the279-instruction4BB090 reserved-
walker initializer, which still uses a register/flags model, and its actual
actor-pool/town-link reset and allocator children.
