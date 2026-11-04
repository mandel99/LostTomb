# Minimap scan and paired RGB565 pixels

The next stage after animation updating was the minimap, not the main terrain
renderer. The C# address annotations correctly identify `0043e810`, but that
reference deliberately omits the original four-cell padding, most overlays and
several invalid-input behaviors. The C reconstruction follows the original
instructions, including those differences.

## Reconstructed code

- `0053f4a0` uses the real sprite manager. Both original call sites bind ECX
  to `00f55bb0`; the three-argument C facade keeps that proven binding and
  delegates to an explicit-receiver implementation. It preserves the strict
  0..25499 image range, signed loaded-count fallback to image zero, DWORD
  cached pixel pointer and coordinates, and rejection of nonpositive pointers.
- `0040fbe0` and `0040fc10` perform the actual four-byte copy/fill for two
  adjacent RGB565 pixels. Frame pointer, pitch and coordinates are DWORDs;
  address arithmetic wraps at 32 bits, as in the native instructions.
- `0043e670` restores full-width viewport geometry, signed division by two,
  wrapped differences, conditional clamps and the original `0x0000fffe` row
  mask. The latter is more specific than simply clearing the low bit.
- `0043e810` scans all four padded rows/columns, alternates the horizontal
  origin by one pixel and preserves every map limit, clipping condition,
  DWORD tile/terrain lookup and BYTE/WORD side grid. Actor classification and
  color precedence, special overlays, outside cells, final camera rectangle
  arguments and clipping reset order follow all 444 original instructions.
- Terrain masks `0x2`, `0x2000000` and `0x200` are separate ordered tests.
  Dunes select the distinct `00f56ab4` palette; the other two select
  `00f568a4`. A native comparison caught an initially incorrect merged test.
- `00439980` restores all overlay selector cases, signed WORD building type,
  264-byte record stride, native category lookup, WORD tint, terrain override
  and all footprint branches. Footprint five draws four sprites in native
  order; values above five still return success and clear the tint.
- `0043ee10`, `0043eeb0` and `0043e790` follow signed WORD actor links using
  388-byte records, BYTE actor fields and 144-byte formations. The first
  classifier retains its actual four-valued result. The existing small
  `004ea9c0` classifier executes as actual C/native code, not a stub.
- `0043e770` uses the real four-byte table stride and native signed result.

## Evidence and comparisons

The pinned original SHA-256 remains
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`water-ground-render-next-native.json` retains the full 444-instruction scan,
393-instruction building dispatcher, 21-instruction cell helper and both
cell-helper cross references. Its historical filename predates identifying
the minimap. `minimap-helpers-native.json` retains the complete pixel, geometry,
actor and category helper bodies.

REA independently reports all 444 scan instructions in
`minimap-rea-function.json`, evidence
`ev_e79b032a4afde70b67264c0b2814e8e388bc8ac5aa8687807751d0a88459765d`.
The source C# reference is
`PharaohRewrite/src/Pharaoh.Engine/Rendering/LegacyCityMinimapProjection.cs`,
read without modification.

`tests/minimap.c` is a standalone C oracle. The original instruction emulator
in `tools/verify_minimap_emulation.py` compares full watched memory and a padded
two-MiB framebuffer with guards, ordered endpoint arguments/intermediate
state, consumed EAX values, stack cleanup and saved registers. Actual
two-pixel writes, actor loops, geometry, category lookup, cell helper, building
dispatcher and scan are composed in both implementations. The authoritative
accepted status and count are in `minimap-emulation.json`.

The 31 deeper overlay metrics, `004387d0` actor predicate, large sprite renderer
and camera-outline drawing remain controlled endpoints. Tests exercise their
permitted state changes and required subsequent reloads. The native Boolean
result domains are retained for `00434820` and `004387d0`. This does not prove
those deep algorithms or the complete main terrain renderer. Valid allocated
tables, acyclic positive links and a padded destination buffer are assumed.

The discard-return audit now accepts the rewritten scan only when the actual
file hash is covered by the passing native comparison. The original invocation
was superseded by complete reconstruction; its dropped guard was not restored.

## Runtime boundary

The minimap-stage Debug SAV confirmation passed the minimap and returned from
the `004d71c0` UI stage. It stopped later in `0053f250`, called from `00436b60` during
`004d75d0` main-map rendering. `city-draw-next-native.json` retains that helper's
165 instructions and all 123 references for the next reconstruction step.
`city-confirm-minimap.json` retains that historical checkpoint. Subsequent
[parallel restoration](PARALLEL_RECONSTRUCTION.md) repairs the sprite function
and reaches a completed mode5 framebuffer without a crash in45 seconds.
Its panel/object placement remains visibly corrupted; matching city visuals
and independent gameplay remain unproven. Another100 actor callback bodies
and the outstanding menu branches also remain.

All changes are in `restore`; original EXE bytes and the C# project are
read-only references. No original game process, computer UI automation or
original machine-code fallback is used. Hybrid stays OFF and both original
EXE build paths stay empty.
