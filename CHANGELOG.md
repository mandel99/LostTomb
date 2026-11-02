# Changelog

## Unreleased

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
