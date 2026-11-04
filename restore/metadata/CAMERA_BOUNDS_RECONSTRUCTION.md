# Camera bounds and clamps

The six restored C functions are 00430ca0, 00430cb0, their 004ddea0/004ddeb0
facades, 004ddf90 and 004de080. The old generated C narrowed lookup anchors,
camera coordinates, editor flags, viewport dimensions and all four limits to
bytes. Lookup entries are DWORDs and the camera uses signed DWORD comparisons.

The scan retains native endpoint traversal, load order and even-row masks.
The clamp retains editor margins, wrapped arithmetic, the one-byte redraw flag
and the final 0x0000fffe Y mask, including its upper-half clearing behavior.
Dimension facades now call the actual full-width getters.

PharaohRewrite/src/Pharaoh.Engine/Rendering/LegacyCityMapProjection.cs supplies
address and conceptual camera references. Its normal-mode model is a reference,
not the source of truth for the native editor branch or DWORD edge cases.
Ghidra exports and pinned original bytes define the C behavior.

The offline comparison composes the actual geometry predicate and tile helper,
all six restored functions, and original facade jumps. It compares every lookup
cell, all selected state regions and adjacent guards, with cdecl stack and saved
register checks. The 1,266-case corpus covers all 189 original instructions.
The private optimized build and the main Debug build are verified separately.
No original process, desktop automation or original text fallback is used.

This proof does not establish map initialization, scrolling input, drawing or
general gameplay correctness. Detailed current hashes and results are recorded
in camera-bounds-emulation.json; the main checkpoint binds the final binary.
