# Changelog

## Unreleased

- Reconstruct the event scheduler, completed-event rearm, parameter count
  and dispatch-deferral predicate as named C providers. Replace four generated
  entries with explicit receiver/argument bridges and retire 434 duplicate
  frame-model lines. Preserve forward record copies, signed inheritance,
  full DWORD calendar inputs and the phase byte at record offset 0x32.
- Pass 915 native/C cases per diagnostic/game build, covering all 259 native
  instructions and 71 conditional edges, plus 12 complete event-iterator
  compositions and two recursive chains of 59 immediate follow-ups. Actual
  gameplay dependencies run; only the diagnostic-file sink is captured for
  event-pool exhaustion. Request/invasion simulation beyond the closed
  eligibility window and unrelated event behavior remain unfinished.
- Share one C parameter selector and event reset between mission loading and
  simulation. Replace two generated entry points with explicit ECX/EAX
  bridges, preserve native ECX outputs and the complete EAX result, and retire 590 duplicate
  register-frame lines. Keep signed WORD sentinels, RNG history and partial
  versus complete generator updates in their original write order.
- Pass 882 native/C cases per diagnostic/game build and 12 complete event
  iterator compositions, including 59 immediate follow-ups. Execute all 275
  reachable instructions; three signed-remainder fixup instructions are
  unreachable after the native 15-bit mask. No dependency algorithms or
  platform endpoints are substituted; unrelated mission/event behavior
  remains outside this comparison.
- Reconstruct seven event/resource methods with named C interfaces: signed
  record lookup, linked-event predicates, remaining months, message selection,
  the 30-byte message packet and flood-event execution. Correct the event
  record stride/count width and unsigned extension commodity IDs.
- Share the event graph stack for immediate flood follow-ups, preserving
  caller-owned packet fields and avoiding a fresh 64 KB arena at each step.
- Pass 3,182 native/C cases per diagnostic/game build, covering all 316
  instructions and 42 conditional edges of these seven methods, actual
  nonmodal message publication, and immediate/delayed chains of up to 59
  follow-ups through the complete event iterator. No dependency algorithms
  are substituted. Live gameplay and unrelated event kinds remain unverified.
- Restore ten flood-setting/effect methods with explicit receivers and shared
  C implementations. Preserve the ninth state DWORD, annual RNG/history write
  order, signed clamps and the world-to-settings offset used by random effects.
  Connect legacy save migration and event callers to the correct receivers.
- Pass 678 native/C cases per diagnostic/game build with actual random
  generators and building classification, all 99 annual rolls and eight
  effect magnitudes. Compile all five affected helper targets and pass normal
  diagnostic exit. Full mission/event callers and flood simulation remain
  outside this comparison.
- Reconstruct nine event/town record methods with explicit C receivers,
  inherited EAX inputs, signed WORD year/range fields and full DWORD calendar
  values. Share due-date/count implementations with existing gameplay facades.
- Pass 1,517 diagnostic cases covering all 151 instructions and 44 conditional
  edges, plus 545 cases for the five methods retained in the game build.
  Preserve the native raw lookup fault separately from the existing guarded
  global facade. Caller register-frame models and full event simulation
  remain outside this comparison.
- Correct the sprite-frame clock and F2/F5/F6 controls: keep full DWORD
  timestamps/flags, unsigned timer wrap and floating-point speed limits.
  Preserve original x87 rounding and unordered comparisons.
- Pass 160 native/C cases per diagnostic/game build, covering all 48 clock/key
  prefix instructions and 12 conditional edges, including five compositions
  with the actual animation tick. Optional object animation/render dispatch
  after this prefix remains unfinished; this is not a whole-function approval.
- Restore the final sprite-manager CRT startup entry and registered cleanup.
  Remove the extra blanket clear during graphics loading so fields preserved
  by the original constructors stay intact.
- Reconstruct both sprite-library array cleanup callbacks with an explicit
  parent-frame receiver and reverse destruction of all 30 library records.
- Pass 146 native/C lifetime cases per diagnostic/game build, covering all
  187 instructions across 11 entries, plus 22 graphics-startup orchestration
  cases covering all 180 caller instructions and four conditional edges.
  Loader/render backends are controlled in the orchestration comparison;
  generic C++ exception unwinding and virtual destruction remain unfinished.
- Restore file-list constructors, destructors and deleting destructors with
  explicit ECX/EBP inputs and named C interfaces. Connect save, mission and
  family globals to startup/exit in the original CRT order. Preserve pointer
  reloads after allocator callbacks and the defined return values.
- Pass 450 diagnostic cases covering all 20 lifetime entries, 91 native
  instructions and eight conditional edges, plus 170 cases in the game build
  and normal diagnostic exit. Heap/exit-list mechanisms are platform
  boundaries; general C++ exception dispatch/unwinding remains unverified.
- Restore construction and reverse destruction of all 50 global actor queues,
  their exit registration and four previously uncatalogued native entries.
  Replace original constructor/destructor code-address arguments with C
  calls; preserve capacity and the other untouched queue fields.
- Keep the standalone data arena alive until registered C destructors have
  finished. Pass 58 lifecycle cases using real native vector iterators, all 26
  lifecycle instructions, the 3,744 queue regressions and normal diagnostic
  exit. C++ exception unwinding and the full CRT startup catalogue remain
  outside this comparison.
- Reconstruct all eight legacy actor-ring queue entries with explicit object
  receivers and named C methods. Share one implementation across building,
  map and walker callers, removing duplicate queue algorithms and undefined
  receiver locals. Preserve cursor aliases and full-queue source refusal.
- Pass 3,744 offline native/C queue cases in the game and diagnostic builds.
  Cover all 104 original instructions and all 20 conditional edges; the
  diagnostic also retains all eight legacy adapters and ten caller facades.
  Global queue construction and complete actor simulation are outside this
  comparison; gameplay reconstruction remains ongoing.
- Reconstruct the complete angle-to-sprite direction mapper with a named C
  interface, correct return pointer and mirrored/unmirrored sector rules.
  Preserve extended precision, binary32 spills, inherited output padding and
  native writes to by-value argument slots without uninitialized C locals.
- Pass 9,419 offline native/C cases covering all 256 direction counts,
  adjacent sector edges, precision/rounding modes, descriptor/output aliases
  and inherited x87 values, plus the link check. Higher action dispatch and
  its virtual angle getter remain unfinished; full playback is not certified.
- Reconstruct complete animation timing, completion, stop and last-frame
  methods. Replace unknown return values and receivers; retain unsigned
  elapsed time, full-width timing mode, extended precision and signed FTOL
  conversion before integer division.
- Pass 347 offline native/C timing compositions without substituted
  endpoints, and 375 lower cycle regressions. Test all precision/rounding
  modes and inherited x87 stack values. Action/direction selection and
  complete animation playback remain unfinished.
- Reconstruct seven animation frame/variant/cycle methods with named C
  interfaces and explicit object receivers. Preserve signed frame wrapping,
  repeat counts, x87 precision and comparison before binary32 rounding.
- Pass 375 offline native/C cases through the C interfaces and legacy
  receiver adapters, plus the link check. The higher animation controller,
  direction selection and complete playback remain unfinished.
- Fix queued messages overwriting their month and body-template index when
  marked as opened. Restore byte writes, 48-byte event addressing and full
  UI fields throughout the queue opener. Population milestones retain their
  own text instead of becoming an unrelated attack notice.
- Pass 220 offline native/C queue cases, including actual event publication
  and panel setup for all ten population milestones. Retain the original
  999-slot scan boundary. Already damaged saved history is not repaired.
- Fix the public farm terrain query reading Y from another building record.
  Floodplain farms now skip the surplus generic building animation.
- Pass 280 offline native/C farm cases, including 24 actual city framebuffer
  comparisons with installed sprites. Fig and barley crops remain distinct
  across six growth stages; live gameplay is not certified.
- Reconstruct four auxiliary sprite/cache lifetime methods with explicit
  owner receivers, reverse record destruction and callback-sensitive
  pointer reloads. Fix a shared scratch pointer truncated to one byte.
  Allow non-null auxiliary slots to reach their restored cleanup logic.
- Pass 145 offline native/C cases, including actual global sprite cleanup
  composition, plus the 227-check compiled manager fixture and link check.
  Free and child virtual-delete endpoints are controlled. The child virtual
  destructor remains unreconstructed; full non-null child cleanup, physical
  rendering and injected exception unwinding are not certified.
- Reconstruct all eight monument-class constructors, four shared record/base
  methods, eight static receiver thunks and a previously unlisted record
  initializer. Restore complete costs, phases, flags, links and animation
  variants. Use the existing 1,327 footprint tuples without duplicating data.
- Run these constructors during standalone data initialization, before
  restored WinMain and geometry, in the original CRT-table order. Replace
  the production path's partial seed/vtable setup. The eight constructor
  sources shrink from 8,451 to 1,686 lines plus shared C helpers.
- Pass 122 offline native/C constructor/helper cases, including real RNG
  and geometry compositions, and the existing footprint/ABI fixture.
  Explicit body comparisons preserve native private input; public wrappers
  zero only native-uninitialized temporary words. Those words are masked
  in wrapper comparisons until geometry defines each piece's derived fields.
  Physical rendering, full startup and gameplay remain untested.
- Reconstruct eleven complete spatial-sound methods in C: provider selection,
  channel defaults, cache lookup/reference counts/loading, EAX preference,
  playback, position, orientation and active-voice updates. Replace missing
  controller receivers, retain original signed-byte index conversions,
  raw coordinate bits and callback-sensitive reloads.
- Compare provider private-frame prestate explicitly where the original
  leaves absent categories or failed attribute outputs uninitialized.
  Public-wrapper comparisons define those outputs; physical audio and
  hardware-provider behavior remain untested.
- Pass 379 offline native/C cases for the new spatial-sound bodies and
  97 sound-lifecycle regressions, with Miles/file/allocation boundaries
  controlled, plus the link check and 60 multimedia forwarding checks.
- Fix media DLL loading when the launcher selects game data separately from
  the EXE. Check executable-local DLLs first, then the selected data folder;
  this addresses the observed Preview abort resolving mss32.dll startup.
  Media DLLs remain external installation files.
- Restore complete 3D sound-pool resizing and sound shutdown with explicit
  controller receivers, original failure/partial-handle behavior and ordered,
  callback-sensitive cleanup. Pass 97 offline native/C cases with audio and
  allocation endpoints controlled, plus 74 compiled media-loader cases.
  Live audio, provider selection and full startup remain untested.
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
