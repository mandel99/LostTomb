# Animation tick, registries and temple selector

At the animation-stage checkpoint, the owned C SAV confirmation passed the
animation controller. The next Debug trap was an uninitialized receiver in
`0053f4a0`, reached through `0043e810` after `004d7090` has returned.
That stage and the sprite dispatcher are subsequently restored; see
[the parallel restoration notes](PARALLEL_RECONSTRUCTION.md). A completed
mode5 framebuffer now exists; matching visuals and gameplay remain unproven.

## Reconstructed behavior

- `0041fdb0` takes an explicit animation record receiver. It preserves the
  native signed actor link and mode WORDs, action/direction BYTEs, full-width
  kind/sequence/frame fields, 40-byte catalog/transition rows and 48-byte frame
  descriptors. Actor action mismatches clear the link and stop the animation.
- The frame step uses 32-bit wrap. Modes 2/3 return early; mode 1 clamps in the
  native order. Other active modes search for an exact sequence/direction row,
  count up to four nonnegative alternatives, run the actual RNG and choose a
  new sequence/direction/frame. Missing or invalid transitions call the actual
  reconstructed `0041fca0` state initializer with state 3.
- All 32 native calls to `0041fdb0` now pass their actual receiver. Six simple
  16-byte registries contain 22, 13, 27, 56, 27 and 15 records. `004216d0`
  preserves the unusual native order of its 25 groups of three records.
- `004e59c0` returns the real building ID. It reads the DWORD player index,
  three DWORD candidates, 264-byte building records, a WORD type, and BYTE
  lifecycle/completion fields. The actual return is retained at all four C
  consumers; their other algorithms are not thereby proved.
- `004e5a80` uses the selector result, reloads the building type on each
  iteration and updates the ten or eighteen records at slot offset eight.
  The complete `0041ff60` order is retained, covering 235 ordinary records
  plus zero, ten or eighteen temple records.

## Native evidence

The original static reference has SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`animation-update-native.json` retains all 144 update instructions, all eight
registry bodies, the complete eight-instruction controller and all 32 update
cross references. `animation-building-selector-native.json` retains the
35-instruction selector and all four consumed-return cross references.
REA independently produced the complete update body in
`animation-update-rea-function.json`, evidence
`ev_2928a07874785fd46a3661f2f93654e9dd84c47e71a014f523ca6aa5727a17c4`.

`tests/animation_update.c` and `tools/verify_animation_update_emulation.py`
compare the actual compiled C against original instructions in Unicorn.
The complete RNG, state setter, update, registries, selector and controller
execute in both implementations. No deep algorithm endpoint is substituted.
The verifier compares all watched memory with guards, ordered RNG input
states, the selector's full EAX, native stack cleanup and saved registers.
All 4,709 comparisons pass and all 22 selected native probes are reached.
The accepted result is recorded in `animation-update-emulation.json`.

Cases include invalid kinds/sequences, WORD modes, exact BYTE directions,
signed frame overflow and clamping, zero to four choices, absent rows,
invalid selected sequences, actor action changes, player 0/1, all three
candidate slots, every registry and repeated ticks. Valid allocated tables
and actor links are assumed; dangling pointers are outside the tested domain.

## Integration boundary and next work

`city-confirm-animation.json` records a real, bounded Debug child from the
owned C build and copied game assets. It reaches mode 1, passes flood, actor
and animation updates, then stops at `_RTC_UninitUse` in `0053f4a0`.
`water-ground-render-next-native.json` retains its 21 instructions and both
complete callers. Both native call sites load ECX with `00f55bb0`; the C
receiver and the helper's DWORD drawing globals still need reconstruction
and comparison. This dossier is evidence for subsequent work, not a claim
that the renderer is repaired.

Debug and Release both pass 70 standalone CTests. All seven owned C menu
integrations pass on this same Debug binary: saved-file selection/cancel,
nonempty map preview, campaign, empty mission selector/back, duplicate name,
dialog cancel and delete. All 20 retained BMP files match their earlier
accepted hashes, including the 15 recaptured menu views. The isolated profile
retains its baseline hash and the temporary SAV/MAP fixtures were removed.

The C# project remains a read-only address/hypothesis reference. No original
game process, computer UI automation or original-code execution fallback was
used. Build configuration keeps hybrid OFF and both original EXE paths empty.
Another 100 actor callback bodies and later city rendering/simulation remain.
