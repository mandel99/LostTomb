# Active reconstruction

- The active C reconstruction is in `restore/`. Preserve the other projects and
  do not include them in reconstruction commits.
- Reconstruct behavior from the pinned original executable and REA evidence.
  Compilation and finite tests do not establish complete gameplay equivalence.
- Do not launch the original or restored host game. Use offline native emulation
  and focused source-only verification with controlled external endpoints.
- Keep the original CD-music fallback as commented source. Startup must not
  require a CD or external music from a CD.

# Git checkpoints

- The user requests ongoing GitHub pushes. After each verified reconstruction
  checkpoint, create a focused commit and push the current branch to the
  configured GitHub remote.
- Use the existing `main` branch in `mandel99/LostTomb`. Do not create another
  branch for these checkpoints. Preserve the existing local remote in the
  separate `PharaohRewrite` repository.
- If the destination is not configured, request the repository URL. Do not pick
  a different repository merely because it has a related name.
- Never force-push or rewrite published history without explicit authorization.
- Keep original binaries/assets, raw extraction inputs, build outputs, downloaded
  tools, credentials and unrelated projects out of commits.
- Preserve exact source and evidence bytes, including line endings. The
  verification records bind SHA-256 hashes; retain the `.gitattributes` policy.

# Token-efficient workflow

- Use compact, explicit briefs for new subagents (`fork_turns="none"`), not
  full conversation forks. Avoid overlapping author/reviewer work.
- Translate connected batches of approximately100 functions. During writing,
  use shared strict compilation and only necessary ABI checks; defer detailed
  native replay to the batch milestone. Do not create per-function frameworks.
- Reuse existing correct C and evidence, including inactive source. Read only
  relevant excerpts and keep handoffs, progress reports and tool output short.
- `src/` also contains active generated units. Check CMake and symbol ownership
  before adding canonical wrappers; a missing runtime wrapper does not prove
  that its symbol is absent from the build.
- A full register result requires its live inputs. If a child writes only AL
  or AX, forward the caller's actual EAX instead of using a zeroed ABI frame.
- Macro-generated QUERY/METHOD entrypoints may already contain complete C. Read
  their shared algorithm before replacing or renaming a runtime owner.
- Reuse saved REA evidence for an unchanged pinned executable. Keep complete
  records in files; return only the needed excerpt or compact result to context.
- Check the result type before summarizing it: report string/array lengths,
  and bound object-key lists. Never enumerate a decompiled string's keys.
- Generated DAT pointer units and native load/store widths are separate facts.
  Reuse cached REA type observations; qualify unavailable/undefined types,
  signedness and overlay aliases against the native instructions. Do not replace
  every global with a DWORD or assume its pointer unit from one load width.
- Give each function one author. Do not start a second review agent or another
  round of discovery for a completed handoff without a concrete unresolved issue.
- Integrate completed handoffs before commissioning more analysis. Agent briefs
  and final handoffs should contain paths, counts, contracts and blockers; keep
  full evidence in files and do not paste complete manifests into tool output.
- Default to small structured tool summaries (about 1000 output tokens). Read
  function bodies only when needed for an edit or a specific unresolved contract.
- Prefer the root plus one implementation agent for routine batches. Additional
  agents need separate substantial work; idle agents and completed packets do
  not justify rediscovery, duplicate reviews or frequent status polling.
- At the approximately100-entry cohort milestone, perform the joint native/C
  comparison before expanding the cohort. Include compiled-only definitions
  in the verification plan; a successful build is not behavior equivalence.
  Batch build fixes, keep reports brief and do not rebuild or recheck an
  unchanged successful checkpoint.
