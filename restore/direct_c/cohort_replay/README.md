# Shared offline cohort replay

This reuses the historical shared replay driver, scenario API and setup. The
second cohort supplies three pure case modules. `--plan` checks their structure
without loading an executable. `--run` uses Unicorn and an immutable local epoch
manifest; it never launches either host game.

The recorded epoch03 compares 285 cases for all100 entries against the pinned
original. It uses the actual main game/diagnostic images where symbols remain,
plus a separate image retaining all98 distinct named definitions. The fixed
legacy data descriptors and initializer bytes are checked independently.

Example from the repository root (with the local verified artifacts present):

```powershell
python restore/direct_c/cohort_replay/replay.py --run --freeze restore/build/actor-animal-following/cohort2-verification/freeze-03.json --go epoch-cohort2-03 --cases restore/direct_c/cohort_replay/actor_cases.py --cases restore/direct_c/cohort_replay/acceptance_route_cases.py --cases restore/direct_c/cohort_replay/city_cases.py
```

The epoch binds exact PE/MAP, source snapshot and input bytes before and after
execution. Local manifests, binaries, RAM archives and complete receipts stay
under `restore/build/`. The curated result and per-entry cases are recorded in
`metadata/actor-cohort-2-comparison.json`. Earlier failed receipts are historical
and are never rebound to later source, driver or image hashes.

Comparisons keep ordered parent-owned reads, writes and calls, final fixed and
scratch RAM, declared result bits, native/C stack cleanup and saved registers.
Explicit receiver/query adapters preserve actual inputs. Natural void EAX is
diagnostic. Designated private output pointers retain alias identity; consumed
BYTE arguments are qualified individually, with native evidence for469E40 in
`metadata/actor-cohort-2-byte-argument-boundary.json`. Full physical child DWORD
arguments remain in diagnostics. Unknown dynamic targets are not admitted.

The existing worker-query RAW bridge is recognized as a child boundary for its
five known query tokens. Its real compiled C model still executes; its internal
reads are excluded from the parent trace just as the native child's are. The
bridge's explicit zero incoming EAX remains observable and is not masked.

These are finite comparisons. Instruction/branch completeness, all input
domains, primary RAW register/flag contracts and whole-game equivalence remain
unproven. Receiver acceptance inside the approach scans is one unvisited domain.
