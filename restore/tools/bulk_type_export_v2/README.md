V2 is a separate private typed-fact batch. The frozen tools/bulk_type_export pilot and all production sources remain untouched.

Run one new known-cache project:

    python tools/bulk_type_export_v2/run_batch.py --name NEW_NAME --run

An optional --snapshot-cache path may reuse SHA256 values only when the prior read was stable and size/mtime are unchanged. The first completed all-root snapshot is retained under build/bulk-type-batch/runs/known4527/protected-before.json. That preflight stopped on a list-shaped metadata report before any Ghidra project was created. The guarded project/run is known4527_guarded. Snapshot hashing now uses8workers; root changes observed between snapshots are retained separately, never overwritten or reclassified as exporter changes. The snapshot excludes only the new V2 paths and Git/Python transient cache directories.

The seed is the exact4527-function cache, with every374525 native instruction record authenticated against the pinned PE. Unit identity comes from units.json address mapping, not enumeration order. No callback address is discovered/materialized from a table in this run. The91 qualified actor seeds remain deferred to another explicitly scoped private analysis. Native-boundary conflicts supplied by the actor investigation are retained as native-facts-only records:48A720 and its foreign17byte epilogue48AD90 receive no independent private function/ABI/C materialization here.

ExportTypedBatch.java seeds known cache bodies in a NEW project only. It streams one JSON file per function, retaining exact raw C, same-token HighVariable/HighGlobal type/size/storage, varnode and p-code facts, logical parameter/return storage, native callsites, physical register read/write observations and native RET instructions. Listing/global-symbol types and inferred calling-model labels are advisory. The same mismatch proven by the earlier pilot remains relevant: a symbol/listing undefined1 can coexist with a token HighGlobal int/4 or short*/4. No function-signature/type setter is applied to the shared project, and no existing project is opened or unlocked. Decompiler timeouts/errors are explicit per-function availability records with complete native evidence, not generated stubs.

The staged candidate selector allows at most the first50 conservative leaves in address order. Current accepted sources, active phase sources, source changes after the initial snapshot, explicit coordinator exclusions and metadata approvals are protected. The overrides file includes concurrent menu/settings work and the actor boundary conflict. Protection is rechecked after the potentially long export.

Eligibility requires an existing unit identity, complete matching native body facts, no native CALL, no computed/out-of-function jump, no floating/vector/system ABI, no receiver/register parameter, canonical stack parameter slots, no unresolved register/stack markers, no pointer/array/field projection or ambiguous token widths, only supported primitive integer types, exact token/native global widths, fullEAX definition along every syntactic return path and native RET without immediate cleanup. Native nonvolatile scratch use, noncanonical frames, stack shims and unused logical parameter/stack-slot gaps are also quarantined. Bare undefined is not assigned a BYTE fallback. Pointer and ABI cases remain facts-only. Known initialized code-pointer slots/direct indirect-call slots are excluded. These conservative gates are not a proof of whole-program calling semantics.

Each candidate preserves raw body text and aliases after newline normalization. Only type-bound fixed-address lvalues and an explicit private primitive header are added. There is no old baseline-type hint input, underscore-width guess, index/address rewrite, calling-convention erasure, direct original-code pointer or production apply operation. Native arithmetic overflow, caller contracts and runtime validity still require independent equivalence proofs.

Policy/CLI checks:

    python tools/bulk_type_export_v2/test_policy.py

After a complete export and selection, compile the candidates privately:

    python tools/bulk_type_export_v2/compile_candidates.py --run-root build/bulk-type-batch/runs/NEW_NAME

For an additional fresh diagnostic build, --tag NEW_TAG chooses new private build/log paths. Existing paths are refused. The completed guarded run retains the initial23-candidate compile diagnostics and the final21-candidate -final compilation; no existing build needs to be overwritten.

The private Win32 object targets use Debug/Od and Release/O2, with /W4 /WX. Syntax/type/unused-value failures are retained, never patched to silence diagnostics. Compilation does not set verified=true, update review counts, route an action or apply C. The root coordinator owns later whole-function native comparisons and any production integration.

Completed guarded-run results: 4,527 records, 4,518 with HighFunction facts, seven explicit 30-second timeouts and two qualified boundary-conflict native-only records. The final 21 staged leaves cover 189 native instruction records and 16 typed global bindings; all 21 compile in Debug and Release. None is automatically verified or applied. Full provenance, source/root drifts, compiler logs and freeze hashes are in that run's report.json and manifest.json.
