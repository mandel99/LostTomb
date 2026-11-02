# Changelog

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
