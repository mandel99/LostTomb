# What progress counts mean

There is no established percentage for completed reconstruction of the game.
Source presence, structural C classification and native behavior comparisons
measure different things and must be reported separately.

A previous figure of approximately 99% counted functions represented by C text,
including generated decompiler output with unresolved inputs and assignments.
Presenting that number as completed C reconstruction was incorrect.

The [structural snapshot](metadata/reconstruction-structural-snapshot-46dbf8e8.json)
at commit 46dbf8e8 classifies 1,317 of 4,402 original manifest functions as ordinary
C structure (29.92%). Excluding 1,372 compiler unwind functions, it classifies
1,314 of 3,030 as ordinary C (43.37%). This is a conservative syntax heuristic:
macros, model/frame implementations and unresolved provider ownership have
separate categories. It is neither a behavioral approval nor a whole-catalogue
count; later discovered native entries are outside this denominator.

The [finite native/C checkpoint](metadata/cohort8-player-ui35-current-finite-joint-checkpoint.json)
records 1,232 scenarios at 197 distinct native entries, with 7,693 physical
invocations and no failed cases on its three exact saved images. It proves
agreement only for those inputs and observed contracts. It does not prove all
paths of each function, whole-game equivalence or a percentage of completion.

For reconstruction updates, report changed existing functions, new algorithms,
resolved inputs/assignments and the exact comparison scope. Keep generated
source coverage separate. Reusing an already-correct algorithm earns no new
algorithm credit, and multiple routes or scenarios do not add functions.
