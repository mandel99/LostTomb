# Native/C cohort comparison in the open Ghidra JVM

This diagnostic helper runs pinned original x86 instructions and linked Release C
through Ghidra p-code. It uses isolated emulator memory and a transient import;
it does not mutate the live program or close/restart Ghidra. Native predicates
and the actual C providers execute without replacement endpoints.

The placement cohort covers23 entries in113 authored cases. It compares logical
AL (the observed native consumer width), watched buffers, ordered predicate and
progress calls, callee stack cleanup and nonvolatile registers. It does not
establish whole-game, native-OS, rendering, arbitrary-domain or complete indirect
coverage. Raw EAX is retained in results; unspecified upper bits are not equated
to a C Boolean.

Prerequisites: the existing Ghidra JVM, Ghidra12.1.4, JDK21 and VS2022 Win32,
the pinned original game and a previously built Release pharaoh_game.lib.
Run prepare.ps1 with the existing Ghidra process ID, then execute the two cohort
scripts copied to $env:TEMP/losttomb-ghidra-native-comparison. The comparison
records are saved there. Source/binary hashes are bound in the receipt.

Only the diagnostic agent's /stop endpoint disposes its own transient program;
it never closes the main MCP analysis. The local token is read from ready.json
and is neither printed nor committed. All compiled outputs stay outside Git.

The warnings cohort covers26 entries in632 cases. Use -Cohort warnings with
prepare.ps1, then run run-warnings-cohort.py in the same temporary directory.
It checks resource/venue/access routing, actual empire queries, free image
restoration and ferry/undo state. The original warning-enabled byte is zero:
ordered calls to the real post routines are compared, but text rendering and
mutations caused by enabled posting remain outside this finite proof. The
fixture links actual static CRT memory routines; it supplies no emulation
endpoint replacements. Void entries do not expose incidental native EAX.

The sites cohort covers10 entries in202 cases (six new complete implementations
and four unchanged C dependency adoptions). Use -Cohort sites, then run
run-site-cohort.py. It compares genuine rectangle/bounds/progress/post calls,
entry-point precedence, all temple rotations, monument cursors, fixed4000-ID
scans and their99-ID skip, invasion iteration and signed calendar results.
The prior gatehouse C scan order was corrected to native columns before rows.
Military text posting remains disabled; its real routing and event mutations
are compared. Original thiscall invasion cleanup is checked separately from
the C context ABI. No native endpoint substitutes are supplied.

run-root-support-cohort.py uses the same sites fixture for19 construction
root dependencies in259 cases. Eight warnings call the real post routine
through live state; three actual terrain rules serve the indirect region
query. Seven root algorithms and a treasury child reuse existing C. Finance
uses the real editor flag (the old argument label was unrelated). Disabled
warning text, arbitrary callback domains and whole-game equivalence remain
outside the comparison. Original raw rule entries have no function objects.
