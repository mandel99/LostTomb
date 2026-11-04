# Actor update and the omitted prey dispatcher

The actor-stage owned C SAV confirmation passed actor updating and the restored
`0049bd40` callback, then exposed a missing animation-record receiver in
`0041fdb0` from `0041ff20`. That later stage is now reconstructed; see
[the animation update notes](ANIMATION_UPDATE_RECONSTRUCTION.md).
A completed city frame and independent gameplay
remain unproven. All C changes are in `restore`; the sibling C# project is a
read-only address/behavior hypothesis source.

## Actual C behavior

- `00489b70` uses the native DWORD player index and 0x49d8-byte profile stride.
  It clears the correct DWORD counters and WORD count, decrements a positive
  countdown, and walks actor IDs 1..1999 using a 388-byte stride.
- Paired actor IDs and back-links are signed WORDs; lifecycle and type fields
  are bytes. Invalid counterpart state/back-links clear the original forward
  link. The active count and current actor ID are DWORDs, including counts
  above 255. The current ID is reloaded after profiling/callback/deletion calls
  at the same points as in the original instructions.
- The numeric callback address from `005dc28c` is passed to a C registry.
  It is never cast to an executable pointer. The original shared `0051f780`
  callback is a bare RET and maps to an empty C function. `0049bd40` maps to
  its new handwritten C implementation. Other entries abort explicitly until
  their real C bodies are reconstructed.
- The omitted prey routine restores the complete state dispatcher, claimed
  building checks, WORD population count, byte animation frame, DWORD frame
  timing, signed WORD waits, death/walk/run/carcass/idle/spawn behavior, RNG
  history and sprite selection. Native wrap and signedness are retained.
- The destination offset table has an eight-byte stride, despite a misleading
  two-byte expression in the decompiler. Formation records have a 144-byte
  stride. Helper callbacks may change the current actor or formation data;
  subsequent reads follow the original reload order.
- The actor-table caller ignores EAX. The C callback has a void interface;
  no claim is made about the original routine's incidental scratch EAX values.

## Native authority and verification

The pinned original SHA-256 is
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`actor-update-native.json` retains the complete 74-instruction controller.
REA independently identifies it in `actor-update-rea-function.json`, evidence
`ev_185a6ac8d538d4bb4b5c8ae9b70277583ddd1a39b5bae0bdf013a64862a46717`.
`actor-callback-table-native.json` contains the original 110 table entries and
their exact bytes. Those entries identify 102 distinct callback bodies.

The normal function inventory omitted the prey entry. REA's declared analysis
seeds materialize the table-confirmed entrypoints only in its private analysis
database. Original file bytes are unchanged. `actor-prey-rea-function.json`
retains all 668 prey instructions, both typed jump tables and body range
`0049bd40..0049c6d4`, evidence
`ev_4b15b6d3c6fde71f3f90c7c5745c4651eda936d87e8eb5f13e647a8b160ecf31`.
The seed file, full snapshot and focused instruction dossier are retained.

`actor-update-emulation.json` passes 622 comparisons, with all six selected
probes reached. It compares full actor/profile state and guards, ordered
endpoint calls and intermediate state, native stack and saved registers.
Cases cover all 110 table entries, 1,999 active actors, paired links, full-width
counts, deletion and current-ID mutation during callbacks. Profiling, callbacks
and deletion are controlled endpoints; their deep bodies are not proved here.

`actor-prey-emulation.json` passes 3,048 comparisons, with all 26 selected
probes reached. Cases exercise every dispatcher state and sprite class, waits
at signed WORD limits, DWORD frame wrap, all claimed-building flag combinations,
RNG history wrapping, valid/failed wander destinations, callback mutation,
signed heading arithmetic and repeated ticks. Complete watched state/guards,
helper arguments/order/intermediate state and native ABI are compared.
The seven movement/death/route/wander/flee helpers are controlled. Their complete
algorithms are not established by the dispatcher proof.

Debug and Release pass all 69 source-only CTests. The aggregate is 96,442
native cases in 34 suites. Seven bounded menu scenarios are retained for the
current binary. Original instructions run only in the offline comparator;
no original game process or desktop automation is used.

## Remaining gap and owned C diagnostic

`actor-callback-inventory.json` records two implemented distinct bodies: the
shared RET and prey. **100 distinct callback bodies remain**, covering 106
table slots. A complete callback table is not an implemented actor system.
Static REA batch exports are reference evidence only and are not imported as C.
All 102 decompilations are retained successfully in four batches under
`actor-callbacks-rea/`; their original identity, exact address coverage and
per-item success are checked when recording the checkpoint.
`tools/record_actor_callback_inventory.py` checks the actual registry and current
prey proof instead of counting missing or aborting paths as implementations.

`city-confirm-prey` passes the former raw callback access violation and reaches
`0041fdb0` through `0041ff20`, `0041ff60` and the actual simulation tick. Its
Debug uninitialized-value check reports the lost record receiver. The complete
144-instruction update, nine-instruction registry walk and eight-instruction
ordered controller are in `animation-state-next-native.json`. These functions,
their record/table widths and all caller bindings are the next repair.

The C-created profile and temporary SAV are hash-identical after the diagnostic;
the SAV is removed. Other menu/city-entry branches, remaining callback bodies,
deep actor helpers, water/model algorithms and older migrations remain incomplete.
