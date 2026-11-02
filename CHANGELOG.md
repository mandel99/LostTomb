# Changelog

## Unreleased

- Connect eight legacy world-route entries to the editor's existing complete
  C methods, with explicit receivers and the original six-byte point layout.
  Share exact numeric primitives, city endpoints and diagonal-capacity
  planning with the mission-load route rebuild. Remove duplicate methods.
- Check 383 offline native/C route cases, including hit selection, city
  binding, removal, loading and x87 rounding/stack preservation, plus three
  C-only loader capacity/failure cases. Allocation endpoints are controlled;
  physical world-map editing remains untested.
- Repair missing source-only route/history fixture link dependencies.
- Reconstruct four complete earthquake routines: event scanning, terrain
  eligibility, tile destruction and all sixteen expansion choices. Restore
  full map offsets, one-byte building state writes, callback-sensitive
  reloads and explicit event/message receivers. Pass 338 offline native/C
  cases; final demolition/refresh and message publication are controlled.
- Restore the complete settings-save/shutdown routine, including full
  allocation pointers and ordered cleanup. Pass 117 offline cases through
  the real binary writer, startup/save/restart and family-profile loading.
  Normal is the no-config default; existing Pharaoh.inf choices persist.
  The family DAT loader restores campaign progress without changing that
  selected difficulty. Host shutdown and physical gameplay remain untested.
- Fix the new-campaign name picker terminating in the unresolved-callback
  handler. Restore its original open, choose and close actions, including
  text-editor reset, and reconstruct the full 42-row suggestion panel.
- Pass 315 offline native/C cases covering real catalog names, every
  original button identity, right/no-click behavior, all resolution modes
  and full font/hover selection. Final drawing endpoints are controlled.
- Reconstruct the complete event-detail renderer and three rectangle-text
  helpers. Preserve explicit event receivers, cached ranges and records,
  delayed arguments, dependency rows and full 32-bit positions/fonts.
- Pass 406 native/C cases, including actual query graphs, record erasure
  and rectangle helpers. Physical rendering remains untested.
- Reconstruct the complete event-description formatter and its twelve-row
  editor list, with explicit event receivers, signed ranges and IDs,
  dependency indices, native text-selection restoration and 40-character
  display truncation. Restore the kind-label and quantity-table queries.
- Pass 642 offline native/C cases, including real catalog/member-query
  composition, all 256 quantity-table indices, callback-sensitive list
  state and 40-character truncation. Reproduce the old formatter read
  fault offline. Final drawing/scrollbar endpoints remain controlled.
- Add bounded difficulty diagnostics at settings load and mission setup,
  including asset-directory selection and selected versus serialized PAK
  difficulty. This investigates the reported Normal-to-Very-Easy change;
  it does not force a difficulty or claim the underlying issue is fixed.
- Pass 60 offline native/C cases for the real persisted-settings reader,
  PAK difficulty preservation and actual language-catalog labels. Full
  startup and live gameplay reproduction remain outside these checks.
- Reconstruct the complete temple-complex placement preview and library-kind
  query. Recover all 32 missing paint arms as a shared C traversal, preserving
  rotation, 91-cell terrain checks, sprite order and 16-bit tint.
- Pass 328 native/C cases covering every paint arm, dynamic library selection,
  key cycling, blocked footprints and callback-sensitive values. Reproduce
  the old null instruction fetch offline. Live rendering remains untested.
- Remove ten obsolete duplicate unit-control bodies. Their public address
  entries now share the same complete C providers as button/keyboard actions,
  including building selection, actor return and formation changes.
- Eliminate the old unit-command jump through native code addresses. Pass
  309 native/C cases, including real search, assignment, route release and
  map-neighbor compositions. Live gameplay remains untested.
- Fix garbled mission countdown text: preserve full font IDs, calendar values,
  viewport origins and mode flags. Reconstruct the complete status renderer,
  including both timer modes, outlines, victory/tomb-placement notices and pause.
- Pass 417 native/C cases, including actual language-catalog and number
  composition producing the original mission label and 144/143/133 months.
  Physical glyph rendering and live gameplay remain untested.
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
