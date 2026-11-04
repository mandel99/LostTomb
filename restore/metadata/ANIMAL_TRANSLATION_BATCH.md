# Six animal callbacks: C-first source checkpoint

The user requested larger C translation blocks before detailed verification.
These six callbacks are now concrete ordinary C in `runtime/actor_command.c`.
Both main Release images link, and the whole module passes strict Win32 C11
compilation with `/W4 /WX /GS- /Ob0 /O2`. No host game was launched.

| Native entry | Kind | Named C callback | Native instructions | Raw frame route |
| --- | ---: | --- | ---: | --- |
| 49B750 | 68 | restore_actor_update_birds | 376 | replaced by full-result C |
| 49BD40 | 69 | restore_actor_update_ostrich | 668 | original retained |
| 49C750 | 70 | restore_actor_update_antelope | 640 | original retained |
| 49F710 | 104 | restore_actor_update_scorpion | 866 | replaced by full-result C |
| 4A04D0 | 103 | restore_actor_update_lion | 777 | replaced by full-result C |
| 4A1130 | 102 | restore_actor_update_asp | 867 | replaced by full-result C |

The4,194 native instructions are static scope accounting, **not current-main
execution coverage**. Exact callback bodies and registry bytes were reread
through REA and checked against the pinned original SHA256. Kinds/names come
from the native registry and the six shipped model files. Provider procedure
ownership is not asserted for these byte-bounded callbacks.

The C retains distinct counter widths, signed branches, cached/fresh reads,
formation vectors, RNG history/publication order and rendering rules. Only
identical pure offset/31-step pair-shift helpers are shared. Ostrich keeps its
separate pointer-based private shift helper and scoped signed-WORD accessor.

Ostrich and Antelope have a native early return whose high16 EAX bits originate
in the movement child's incidental machine result. The existing movement API
is void. Their ordinary game callbacks are therefore natural void C, using
the actual existing child API and preserving known effects without fake zero
results. The registered callback consumer discards the result. Their complete
old RAW branches, models, readers, ROM tables and prototypes remain unchanged.
The source-only `actor_prey.c` test provider remains separate and unchanged.

This retained compatibility is an intermediate migration step. It does not
close broader raw-register contracts or finish whole-game reconstruction.
The complete old RAW cue path is also retained; its global CRT-handler gate
remains open. The expanded local cue inventory will be refreshed for the
whole connected block, including explicitly identified source-only copies.

Independent candidate checks were limited to strict builds and focused native
witnesses, or earlier historical protocol scopes. They are not relabelled as
current main-PE proofs. **Joint main/native comparison and broad regression
validation of this block are pending.** The reports accepted at checkpoint
`1683857` remain historical, with their original source/binary bindings.

Machine-readable source/build status and exact hashes:
`actor-animal-translation-batch.json`. Curated native observations:
`actor-animal-translation-rea-records.json`. Original binaries/assets, candidate
DLLs, build outputs and raw extraction inputs are not included in this commit.
