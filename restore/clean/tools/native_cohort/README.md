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
