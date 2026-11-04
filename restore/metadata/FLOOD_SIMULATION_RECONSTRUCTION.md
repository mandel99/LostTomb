# Flood simulation and post-menu city-entry checkpoint

The owned C SAV confirmation passes `004be770` and the later actor/prey repair.
The current stop is the missing animation-record receiver in `0041fdb0`;
see ACTOR_UPDATE_RECONSTRUCTION.md. No completed city frame or playable
standalone game is established.
All implementation changes are in `restore`; `PharaohRewrite` is read-only.

## Recovered native behavior

- The omitted CRT initializer `004bd310` sets the double at `00d05718` to
  `9792.0 / 25.0`. The standalone entry invokes it after opening owned data
  and before entering the game. Original bytes are retained in
  `flood-initializer-native.json`; the game build reads no original executable.
- `004be770` restores all floating-point inputs that the decompiler dropped
  before `0056133c`. It computes the absolute subcycle, previous subcycle,
  seasonal start/end, inundation span, previous-year correction, six phases,
  row limits, notices, growth reset/progress and row-update percentage.
  Scenario base year is signed 16-bit; current year and the other clock values
  are DWORDs. Wrapped integer operations and truncation toward zero are explicit.
- Region updates use the actual 28-byte object at
  `self + 0x32c64 + row * 28`. `004bd850` restores the cursor/list traversal,
  signed percentage skip, partial/complete budgets, active-payload byte and
  cursor reload after callbacks. `004be0d0` increments growth below five and
  uses that same explicit region receiver.
- `004be490` and `004be580` pass `self + 0x32c40` to the seasonal settings
  helpers. The latter retains the original actor-type/status filtering and
  388-byte actor stride. The `004be4a0` relocation function consumes its stack
  actor ID; its incoming ECX is unused in the retained native body.
- `004bd550`, `004bd580` and the explicit `restore_flood_quality` helper keep
  their real settings receiver. The native quality helper reads a WORD flag,
  a DWORD player index and the 0x49d8-byte player stride, then applies the
  conditional ten-point bonus and upper limit. `004bd590` remains a global
  C facade for the unchanged global callers; the season helper calls the
  explicit implementation. Caller windows are retained separately.
- `004bd380` restores the complete weighted quality-change table, including
  its lost first weight. Both 31-bit generators, history index and history
  values use DWORDs. The original strict threshold comparison and 0..100
  quality clamp are retained.
- `004bd950` now receives its payload explicitly. Its remaining deep tile,
  building and actor algorithm is not claimed restored by this checkpoint.

## Evidence and comparison

Original SHA-256:
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`city-simulation-next-native.json` retains all 303 controller instructions;
`flood-simulation-helpers-native.json` retains complete helper bodies and
exact floating-point constants. REA independently analyzes the same complete
controller in `flood-simulation-rea-function.json`, evidence
`ev_cf7771be18909b4750ce07da6efae096edb969ad5c256c13f44cbaad08c43724`.

`flood-simulation-emulation.json` passes 4,564 comparisons. All 21 selected
native branch probes must be reached, including actor relocation, payload
updates, quality bonuses, year wrapping, all six phases and growth clearing.
The original x87 instructions and original `0056133c` execute in Unicorn;
the original game process is not launched. Comparisons check complete watched
memory/guards, both global and alternate worlds, ordered endpoint arguments,
intermediate state, value returns, stack cleanup and saved registers.

Cases include a complete seasonal cycle, signed base years and current years
above 32767, normal month/day/tick combinations, every quality from 0 to 100,
widths through 30, list counts through 257, byte-truncated flags, existing
cursors, callback cursor mutation, repeated ticks and cached growth progress.
One early mismatch came from unsigned overflow in the Python fixture setup;
the corrected fixture is required by the retained final evidence.

Warning delivery `00489510`, payload mutation `004bd950`, actor relocation
`004be4a0` and person predicate `004c5120` are controlled in this focused
suite. Their caller contracts are checked; their deep behavior is not proved.
Valid list storage, region indices, widths 0..30, quality 0..100 and history
indices 0..99 are required. Invalid native division/pointers are excluded.
Native floating-point comparisons use control word 0x37f and the actual constants.

Both Debug and Release pass all 69 source-only CTests. The aggregate is
96,442 native cases in 34 suites. Seven bounded owned-C menu scenarios are
retained for the current binary, with actual callback dispatch and framebuffer
reads from isolated copied assets. No desktop automation is used.

## Remaining concrete failure

`city-confirm-flood` completes post-load preparation, reaches mode 1 and
passes the former flood uninitialized-input trap. The next exception is
`c0000005`: `00489b70` calls raw address `0049bd40` through table `005dc28c`.
`actor-update-next-native.json` retains the complete 74-instruction controller.
`actor-prey-next-native.json` contains only a marked partial byte excerpt;
the omitted callback's complete body is still to be recovered.

This is the historical flood diagnostic. The subsequent actor checkpoint
restores `00489b70`, the prey dispatcher and their actual C registry entries.
`city-confirm-prey` passes them and stops at `0041fdb0` from `0041ff20`.
Another 100 distinct callback bodies and the deep actor helpers remain.

The C# reference identifies `0049bd40` as the type-69 prey dispatcher in
`src/Pharaoh.Engine/Walkers/Walkers.Hunting.cs`. Its behavior is a hypothesis
until checked against the complete original function. The next work must
restore the actor controller widths/strides and route callbacks to actual C
implementations. Skipping actor updates or executing original code would not
establish the requested independent game.

The C-created profile and copied SAV remain byte-identical after the diagnostic;
the temporary SAV is removed. Full actor/water/model algorithms, mission entry
branches and remaining root menus still require reconstruction.
