# Token-efficient reconstruction

- Work in one agent by default. Use a subagent only for a separate substantial
  task that cannot be handled by the active agent without blocking.
- Start every batch with `tools/migration_workflow.ps1 discover`, then use
  `plan` only for the selected source. Do not dump the binding inventory or the
  complete function map into context.
- Never print a complete large C file. Locate functions with `rg -n`, then read
  only the required body and directly used helpers in bounded excerpts.
- Keep ordinary tool output below roughly 2,500 tokens. Report counts, hashes
  and failures; store full manifests in files rather than displaying them.
- Reuse confirmed C before invoking REA or Ghidra. Reverse engineer only a
  function for which project-wide search found no relevant native C owner.
- Migrate connected groups of 20-30 functions. Compile once after the complete
  group. Do not repeat behavior tests for unchanged, previously confirmed C.
- Run `tools/migration_workflow.ps1 verify` after the build. Do not repeat
  equivalent ad-hoc map, SHA, CMake and source-policy scans.
- Keep progress messages and handoffs compact: changed paths, function count,
  build result, library hash and blockers only.
