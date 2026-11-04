This is a private typed-fact exporter pilot. It does not modify the existing Ghidra project, regenerate production files, or automatically accept functions.

Run a new private import and guarded candidate emission from the restore directory:

    python tools/bulk_type_export/run_pilot.py --name a-new-private-name --run

Without --run the command prints its exact private invocation. Existing run/project names are refused; failed private logs are retained. The wrapper imports the pinned PE as static data into a new project below build/bulk-type-pilot, disables automatic analysis, and seeds only verified pilot/support entries in that new project. The script checks both the program SHA and private project/output locations. No running original game or existing analysis server is opened.

The five exported entries are0040FA40/0040FA70/0040FAE0/00425F20/0053DB60. Each function's raw C, Clang-token HighVariable/HighGlobal type/size, varnode facts, linked symbol/listing advisory datatype, prototype/storage, native instruction/raw p-code and high p-code come from the same decompilation. Only the three scalar leaves can emit C. The pointer-projection and receiver cases remain quarantined with complete facts.

The decisive distinction is per-token HighGlobal versus its symbol/listing type. In the completed run, framebuffer/pitch Clang HighGlobal and varnode are4bytes/int, while the linked listing/global-symbol datatype is undefined/1byte. Pointer705724 is4bytes/short*, with2byte pointees. A GlobalSymbolMap-only exporter would repeat the old BYTE bug. The emitter uses per-token facts corroborated by pinned native read sizes; there is no undefined-to-BYTE, underscore-to-DWORD or index/address rewrite fallback.

C is emitted under an address-stable private candidate path, preserving raw body text and aliases after newline normalization. The proposed existing unit path comes from units.json. Production source/mapping hashes are captured before and after export/emission. All production src/runtime/include plus top-level tools/manifests are protected; no apply operation exists in this tool.

Completed evidence is below build/bulk-type-pilot/runs/guarded4. The initial exploratory successful import is retained separately at build/bulk-type-pilot/project. Guarded development attempts guarded/guarded2/guarded3 are retained as failures: WindowsPowerShell lacked Get-FileHash, an unsaved import had no DomainFile project locator, and Ghidra returned a /C:/-prefixed Windows project location. The final wrapper uses .NET SHA256 and the script resolves its active project state with explicit Windows-location normalization. No existing project lock was removed or bypassed.

CLI rejection evidence: build/bulk-type-pilot/cli-guard-detailed.json tests production output, existing run, existing private project, wrong input SHA and a nonprivate project. The existing private GPR remains unchanged and invalid output files are absent.

The private CMake project build/bulk-type-pilot/CMakeLists.txt compiles the completed guarded4 candidates into restore_bulk_type_pilot_verify. The independent comparison command is:

    python tools/bulk_type_export/verify_candidates.py --exe build/bulk-type-pilot/out/Debug/restore_bulk_type_pilot_verify.exe

Repeat with the Release executable. This proof checks all56 native instructions, full guarded byte changes/global slots and native stack/saved registers. It deliberately limits raw signed C arithmetic to valid bounded coordinates0..31, pitch260..4094 and pointer006F0000. It does not accept arbitrary32-bit wrap or callers automatically. The prototype sidecar's inferred __stdcall label is preserved as an observation; physical native RET without immediate cleanup is also retained, and the emitter does not impose that model label on raw default-C signatures.

For broader use, add reviewed per-use type/projection/ABI policies and staged cases; do not widen every byte macro, emit receiver functions as ordinary C or overwrite already accepted source. The earlier artifacts/bulk_export_investigation report remains frozen and historical; this pilot supplies the actual HighGlobal/listing observations it could not previously establish.
