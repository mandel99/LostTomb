# Image collection reference audit

The older project at `PH Local Decompile/reused/restore` contains a substantial
`0x0053DFF0` translation in `generated/mixed/src/Unit02522.c`, but that body
still depends on absolute addresses, legacy `Original...` calls and 32-bit
pointer overlays. It is useful as a behavior map, not as a clean-C owner.

Tutankhamun's `src/game/graphics/image_collection.c` at commit
`a012b1a137b1d5307a2382bfea8efacbbc8dbe43` was inspected as an implementation
reference. Its SHA-256 is
`f3023cb3bde6c0596a652e43d3ff93d2c277110b248ebcb7cc562e091be7bfa7`.

## Behavior corroborated against the pinned Pharaoh.exe

REA/Ghidra pseudocode for `0x0053DFF0`
(`ev_0e3181a7ccd2c853d8294ed2c8bf8851c4761c4ad6ac8954e62014adbc38628a`)
confirms these matching behaviors in the reference:

- Metadata buffer capacity and record layout: `0xD8928` bytes, 300 signed
  group-image entries at offset `0x50`, 200-byte bitmap names at `0x2A8`, and
  64-byte image records at `0x9F28`.
- Nonzero library slots skip 200 system images and `0x992E` pixel bytes when
  the first bitmap name is `system.bmp`.
- Group records are sorted by group key; the ordered groups populate the tag
  table and write the corresponding group ID into the image header.
- Image data starts at pixel offset 4, external images keep a marker value when
  their data field is zero, and conversion dispatches on draw type, compressed
  state and isometric-top state.
- A successful fresh load updates the manager's image, group and loaded-slot
  counts; cached loads compare the name case-insensitively.

## Differences that must not be copied as native behavior

- Tutankhamun zeroes the metadata buffer and returns cleanly when reading fails;
  the original path continues using the stack buffer and has no initialized
  metadata guarantee after a failed read. The failed-read path is therefore not
  a safe equivalence target.
- Tutankhamun handles allocation failures with `malloc` checks. The original
  uses its allocator's non-returning allocation contract in this path.
- Tutankhamun logs group-count mismatches. The original builds a warning and
  calls `MessageBoxA` with title `Graphic file out of sync`.
- Tutankhamun keeps pixel offsets in `g_pixel_cursor`; the EXE updates a global
  cursor at `0x00D3B5F4`. The clean project must express this as typed state,
  without reproducing the absolute address.

The reference supports implementation and naming choices only. Exact field
widths, call results, warning behavior and control flow remain governed by the
pinned EXE evidence. No game or test executable was run for this audit.
