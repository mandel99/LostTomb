# Region image updates, 2026-11-04

475720 and47BAE0 now execute ordinary named C in `runtime/map_region_repair.c`.
Their numbered APIs and existing frame adapters route to those implementations.
The former large frame bodies are replaced by small ABI bridges. The default
library and both Release executables compile and link successfully.

| Entry | Named algorithm | Instructions |
| --- | --- | ---: |
| 00475720 | restore_map_refresh_paved_region | 205 |
| 0047BAE0 | restore_map_refresh_channel_region | 173 |

The native byte catalogue and REA evidence bundle retain all378 instruction
addresses, authenticated against original executable SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
The names describe inferred visual roles; exact masks and load phases below
define the recovered contracts without claiming original source identifiers.

475720 sorts each pair of rectangle endpoints before clamping. A nonzero
DWORD clear flag enables the first pass over terrain bit40 with access bit80.
It skips a nonzero signed WORD building index whose record type is138. Other
eligible tiles receive image0, the cached access high nibble, and a freshly
read sprite byte OR40. The original first-pass row tail remains cached even
when map width is reloaded after each nonempty row. The second pass recomputes
its row tail from the last cached width and reloads the map base.

The second pass selects bit40 tiles with image0/access80. It caches the low
variation bit before calling475670. A nonzero full-DWORD result publishes a2x2
footprint using F5695C plus6/7. Otherwise it publishes a single image using
the two coordinate parities (odd/odd:+1, even/even:+0, mixed:+2), then marks
the sprite. The type138 exception belongs only to the optional clear pass;
a protected record with an already-zero image can still be processed later.
Both passes retain live padding reads but fixed clamped rectangle bounds.

47BAE0 sorts and expands the rectangle by one cell before clamping. Its tile
predicate is terrain100 without bitsC and an unsigned pattern byte <=15.
The signed image-minus-F56B84 difference selects offset0 only for0..47;
negative or >=48 selects48. A nonzero full-DWORD refresh flag forces48 while
preserving the original image/base reads.

It computes neighbor patterns through47E870/484BE0, then reloads the shared
cell cursor. Bit40 tiles clear access bit80 and may reset the secondary
offset from the northern tile's bit40.47B920 adds42 or15 to the live offset,
with another cursor reload before publication. The final image combines the
live frame, base, secondary offset and cached0/48 variant. Terrain10000 applies
the real51C620 transform and reloads the cursor afterward. Access, image,
sprite and pattern-byte stores preserve their native order, including the
pattern byte read between the access and sprite stores. Shared cursors retain
their distinct reload phases and fixed228-cell row stride. BCA328 is cleared
on normal and empty-rectangle returns.

Both Release executables pass128 scenarios/512 invocations each, with all378
owned instructions and all94 conditional outcomes executed. Comparisons cover
ordered owned writes, child arguments, complete original static memory, saved
registers and stack cleanup across named, canonical and frame routes. Tests
exercise sorted/clipped/empty rectangles, nonpositive dimensions, full-DWORD
flags, signed building indices, type138, both image passes, neighbor outputs,
BYTE pattern bounds, palette/coordinate mutations and live versus cached fields.

Eight cases execute every reached direct child without substituted results.
They include actual paving publication, special-building preservation,
single-tile image output, the47B920 branch and51C620 transformation. Assertions
check the protected image123, single image5000, required child paths and the
final BCA328 reset. The access-controller suite additionally includes a fifth
complete real16x16 repair composition with active region tiles, bringing that
suite to193 scenarios/772 invocations per executable.

```powershell
python direct_c/verify_map_region.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_map_region.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

`map-region-{diagnostic,game}.json` binds the tests to exact source and binary
hashes. These finite checks establish the owned algorithms and selected actual
compositions, not all deeper-child paths or full playable gameplay. Remaining
work includes city/input/actor models and unknown decompiler fields. Next
work includes timed-record predicates and remaining actor/city/input models.
The4BF9D0 parent now executes ordinary C; see
[processing queries](BUILDING_PROCESSING_QUERY_RECONSTRUCTION.md).
The4894A0/489470/4893C0 closure now executes ordinary C, and gameplay ring
growth now uses the literal canonical copy chain. See
[occupancy and ring growth](BUILDING_OCCUPANCY_AND_RING_RECONSTRUCTION.md)
for verified normal-return scope and remaining heap/exception limitations.
