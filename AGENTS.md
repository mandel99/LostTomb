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
