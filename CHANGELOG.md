# Changelog

## Unreleased

- Reconstruct five complete pyramid-worker functions: arrival/travel states,
  exact work-list sorting, linked-worker validation, paired-side queries and
  worker recruitment. Correct field widths, cached ownership and detach rules;
  retain the original sort's tie order through the existing C implementation.
- Pass 428 native/C cases, including 51 combined movement/work/claim paths,
  and all 355 Ferry Landing regressions on the same build. Live gameplay
  remains untested.
- Fix the Ferry Landing selection crash: a native switch table had been
  translated as a call through original code addresses. Recover every scan
  branch and the full first/second-bank preview, with exact field widths.
- Pass 355 native/C cases covering crossing scans, opposite-bank highlights,
  route-buffer setup and complete preview compositions. Reproduce the old
  null instruction fetch offline. Live gameplay remains untested.
- Reconstruct the complete monument footprint writer and both resource/refresh
  loops. Recover the explicit object receiver, cached footprint size and virtual
  image flags. Correct map field widths, shape masks and eight old caller arguments.
- Pass 376 native/C cases covering all shape/orientation branches, finished tiles,
  material selection and real image/activity/map-write compositions. Physical
  rendering and live gameplay remain untested.
- Reconstruct seven complete pyramid-work selection functions: worker-slot
  thresholds, claim cleanup, pending work, paired pieces/workers, route gaps
  and destination selection. Preserve native claim priorities, signed distance
  scoring, field widths and state transitions; add descriptive C names.
- Pass 766 focused native/C cases, including 85 complete selection/travel
  compositions. Fix the old public route-gap query accepting overlapping
  linked workers. Host gameplay remains untested.
- Reconstruct and name seven complete pyramid-worker functions: travel setup,
  movement, target snapping, work-list ranking, perimeter traversal and work
  reservations. Recover the missing distance expression and delayed arguments
  at the next-travel call; identify the previously unnamed assigned-worker field.
- Pass 687 focused native/C cases, including real work-selection and movement
  compositions. Actor-grid endpoints are controlled; live gameplay is untested.
- Remove three obsolete public C bodies for building allocation, repair slots
  and god effects. Their address entries now share the same complete typed C
  providers as simulation/construction callers.
- Pass 668 native/C cases covering all 241 building kinds, contiguous search,
  reservations, all repair slots and god-effect branches. Preserve unspecified
  bytes in the original 30-byte message scratch rather than inventing defaults.
- Reconstruct chief-advisor advice selection/render calls and temple-complex
  upgrade previews. Preserve full-width coordinates, player/table indices,
  linked building records and tint fields; add descriptive C entry names.
- Pass 448 focused native/C cases covering all twelve advice categories,
  the actual trade iterator, both upgrade branches and the upgrade-bit query.
  Physical rendering and live gameplay remain untested.
- Reconstruct and name seven complete pyramid functions: footprint stamping,
  linked part creation, construction/worker images, whole refresh, special
  parts, finished phases and entrance creation. Correct the missing third
  argument in the old image-refresh caller.
- Pass 368 focused native/C cases, including 35 complete seven-function
  placement compositions. Preserve BYTE/WORD/DWORD field widths and table
  reloads after callbacks. Physical rendering and live gameplay remain untested.
- Reconstruct five complete mausoleum functions: placement, tile/image update,
  part/worker repositioning, instant completion and whole-monument refresh.
- Preserve all 116 parts, linked allocation order, terrain map field widths and
  distinct worker retarget/reset/restart paths. Fix complete geometry buffers
  in the remaining public C entries.
- Pass 192 focused native/C emulation cases, including all 14 part image
  subtypes. Host gameplay and physical rendering were not exercised.
- These source updates will be built and released only after a new tag is pushed.

## v0.1.1-wip.20261102

- Fix the crash when previewing placement of the mausoleum. Use the complete
  52-byte geometry record, correct count-table indexing and DWORD coordinates.
- Check all 116 geometry pieces and all four rotations against the original
  native routine in 44 focused offline emulation cases. Drawing and projection
  endpoints are controlled; live gameplay confirmation remains pending.
- Fix corrupted dash characters in the README.

This remains an experimental work-in-progress prerelease.

## v0.1.0-wip.20261102

- Initial public Windows x86 runtime and C source snapshot.
- Include the simulation-event receiver fix and earlier reconstruction work.
