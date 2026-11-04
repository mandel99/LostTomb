# Local cleanup, 2026-11-04

Removed 61,359 obsolete generated files, totaling 10.728 GiB. The measured disk
free-space increase was approximately10.857 GiB, including filesystem allocation
overhead. The remaining local tree is approximately2.823 GiB, excluding the four
retained junctions to original AUDIO, BINKS, Data and Missions assets. The first
coarse inventory followed those junctions; it is not a comparable before size.

The cleanup removes unreferenced compiler/linker outputs, old CMake intermediate
trees and caches, Python bytecode and the unused typed-export V2 fact/project
cache. Four referenced fact files remain protected. Unapplied candidate sources,
provenance summaries and compiler diagnostics remain available. A fresh optional
V2 export can regenerate removed facts; its previous project is no longer local.

Current build/c-only, original assets/PE, offline verification dependencies,
CMake scope snapshots, tracked source/evidence and referenced historical EXE/MAP
pairs are retained. Legacy projects whose generated intermediates were removed
need reconfiguration/recompilation if used again.

The plan checks tracked dependencies and local phase JSON reports, protects MAP
companions of retained EXEs, skips junctions and hashes10,202 protected files.
The PowerShell executor validates every absolute target stays inside restore,
rejects active/protected paths and reparse points, then verifies protected hashes
again after removal. No tracked reconstruction source or evidence was deleted.

An actual incremental build passes after cleanup and preserves both tested
Release EXEs and MAPs byte for byte. The current54-report aggregate is checked
again after this documentation update; no host game is launched.

Local detailed plan, receipt, inventories and logs are retained in
build/cleanup-20261104/. They are generated local records, excluded from Git.
The compact public record is local-cleanup-20261104.json.

To prepare another reviewed plan, run tools/plan_restore_cleanup.py. Inspect it
with tools/Cleanup-Restore.ps1 -PlanPath PATH; apply that exact plan by adding
-Apply. The planner treats current project paths as Windows paths.
