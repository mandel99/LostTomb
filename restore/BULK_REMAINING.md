# Remaining-function inventory

Run from the repository root. The default scan reads the pinned native image,
saved REA catalogue, source owners and the main CMake-generated project together
with its referenced object libraries. It does not launch a game or change C,
evidence, selected cohorts or build configuration.

```powershell
python restore/tools/bulk_remaining.py
```

The complete per-function inventory is written to the ignored file
`restore/build/bulk-reconstruction/remaining-inventory.json`. The console prints
only counts. The catalogue is the union of saved requested addresses, native
cache entries, `units.json` and the export manifest; incomplete, conflicting and
uncached functions remain in the inventory. A saved static catalogue is not a
claim that every native function has been discovered.

`existing_selected` reserves existing cohorts. `existing_C_preserved` preserves
primary C without a recognized automatic repair marker. `existing_C_reuse_review`
records other canonical, macro, frame or model definitions, including inactive
source and artifact trials. These are reuse leads, with correctness unproved;
none of these categories establishes completed reconstruction or gameplay
equivalence. `review_needed`, `unsupported_rule` and `uncached` state the specific
remaining obstacle for each function.
Repair markers are only triage heuristics. A separate definition remains reserved
even when it uses names such as `in_EAX` or `in_ECX` for explicit live frame inputs.

`batches` contains only safe candidates accepted by the existing
`bulk_reconstruct.py` native-template and ABI contract. `review_batches` separately
groups unresolved and reuse-review work for manual reconstruction. Cached direct
calls determine connected groups of approximately 100 entries; oversized
components are split explicitly. Cross-batch dependencies, incomplete native
scopes and indirect-call gaps are recorded. These categories are not counts of
missing implementations.

Candidate emission is opt-in and always separate:

```powershell
python restore/tools/bulk_remaining.py --batch-size 100 --write-candidates restore/build/bulk-reconstruction/remaining-candidates
```

Each nonempty candidate batch gets `src/` files and a SHA-bound `members.json`;
the directory also gets `index.json`. A differing existing packet is refused.
Use a fresh output directory after source or ownership changes. Reports and
candidate output must stay under `restore/build`. Review batches never emit C.

There is no installation or selected-cohort advancement mode. When the progress
ledger declares `source_expansion_suspended: true`, planning and separate candidate
emission remain possible, and selected-cohort advancement remains disabled.
Strict compilation and joint offline native/C comparison are separate required
checkpoint stages. The measured scan currently has **0 safe automatic candidates**;
unsupported semantics are reported explicitly rather than replaced with stubs.
The compact published queue is
[metadata/bulk-reconstruction-next-queue.json](metadata/bulk-reconstruction-next-queue.json).

The latest source scan covers4530 saved addresses and leaves28 native/source review leads in one connected packet. It finds no new exact-rule C candidate. These are not28 newly missing algorithms. Native/source boundaries still need review, and source presence elsewhere does not prove faithful behavior. The driver defaults to the highest-numbered cohort progress manifest.

Reviewed native scope exceptions are deliberately narrow. `native_scope_policy.py` accepts only curated findings supported by agreeing pinned REA entry and containment records. It checks every saved instruction in every declared body range against the native PE, retaining the actual entry separately from the lowest address. The reviewed516630 preceding tail and56B670 shared unwind RET now retain their existing C. These ranges cannot enter the flat automatic template generator. Other scope gaps, including the qualified48A720 foreign epilogue, retain review. This establishes coverage of saved bytes, not native/C behavior equivalence.

Reviewed logical-input dispositions also preserve ordinary typed canonical C while its exact reviewed source and current active owner still match. Twelve history/teardown paths use this rule; inactive owners and changed sources retain review. This proves source presence, not register/flag or gameplay equivalence.

Cohort7 now contains66 selected source entries. Fourteen related score/low-I/O action repairs restore native signed BYTE/WORD metrics, DWORD wrapping gates,264-byte records and88-byte owner entries, signed rounding and callback snapshots. Allocator/set/free retain64-slot scans and captured-cell reloads. All three images pass strict compilation. Necessary current-image checks cover14 entries with58 cases/406 invocations. Earlier receipts retain exact archived images. Detailed joint validation remains pending at66/100 nominal entries.

The catalogue gained three exact pinned CFG records through metadata/*-native-function-discovery.json. Ghidra had not declared these procedures; complete instruction bytes, recovered control flow, data/call references and finite native/C execution support the entries. This discovery does not close the whole native catalogue.

Known EH3, startup, multibyte, environment, calendar, low-I/O startup/descriptor actions and related record-score paths are migrated. Twenty-two historical scalar-pilot candidates are now repaired, plus the directly connected rounding callee. The read-only pilot remains a review-lead snapshot, not a source emitter or count of missing algorithms. Remaining leads require native narrowing, signedness, pointer-stride and caller ABI review. This queue is not a complete inventory of semantic gaps.

MAP lookup indexes each image once. All earlier finite receipts stay with their exact archived source/image epochs; they are not rebound to the current layout.
