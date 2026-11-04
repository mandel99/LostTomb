# City-price and music-update reconstruction

All implementation changes are in `restore`. The sibling `PharaohRewrite`
is a read-only address/structure reference. Original executable instructions
determine behavior; no C# implementation or ZIP draft is substituted.

## Trade-price preparation

- `0044baf0` now has an explicit town-list implementation and a global facade.
  All 26 original call sites select global list `00785d70`. Records are 124
  bytes; the list includes its native metadata tail.
- `0044d4c0` takes the selected town record explicitly. All ten C call sites
  now bind that receiver. Native upper EAX bits are retained where defined;
  disabled-record paths define only AL and are compared as such.
- `00443330` uses its real 36-element accumulator, signed quantities,
  population thresholds/category tables, wrapped 32-bit arithmetic and the
  correct 0x49d8-byte player stride. `00443080` counts buildings using a
  full-width count and word-sized building type. The real `00559c60` clamp
  executes in the comparison.

The 1,691-case `city-prices-emulation.json` compares complete watched memory,
return values, ordered helper arguments, stack/register preservation and all
eight selected branch probes. Fixtures cover all population bands, price
categories, signed trade quantities, arithmetic wrap and counts above 255.
Only the complex resource-demand helper `004f57b0` is controlled. Valid town
counts are 0..150 and building counts are bounded to 3999.

Complete bodies and caller windows are retained in `city-price-next-native.json`,
`city-prices-helpers-native.json` and `town-trade-callers-native.json`.
REA independently identifies `00443330` in `city-price-rea-function.json`,
including all 136 instructions, evidence
`ev_d51fa7b1399a9ed9ec19fa1741391b16ba955b7eacd733ccad4925cd15400a41`.
The pinned original SHA-256 is
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

## Music and sound receivers

- `00539820` takes the real music-manager receiver from `004d6b60` and
  restores all four phases: main track, ambient track, delay start and delay
  expiry. It retains the native unsigned clock comparison and wrapped delay.
  The index-50 marker lookahead reads the adjacent count field as the native
  instructions do.
- `0055f38d` takes the sound object explicitly. It writes stream volume,
  checks disabled/stream/status state and reloads the stream handle after the
  status callback before setting volume.
- `00562348` uses a DWORD RNG state. Its former byte declaration discarded
  the high state bits and made the returned random number always zero.
  The actual native multiply/add wrap and 15-bit result are restored.
- A clock seam supplies `GetTickCount` in the real runtime and deterministic
  time in the focused source verification.

The 1,668-case `music-tick-emulation.json` executes actual C/native selection,
volume and RNG helpers. It compares both global and alternate objects,
complete memory/guards, intermediate state and audio/time call order.
All five selected state-machine probes are reached. Clock, playback and Miles
API endpoints are controlled; this does not prove physical audio or codecs.
Valid categories are 0..52, main-track counts 0..50 and ambient counts 0..10.
Complete instruction bodies and the actual caller are in `music-tick-native.json`.
The existing 324-case music-manager constructor/parser/selection suite is
also refreshed and included in the aggregate; it is not a new set of repairs.

## Owned C integration and remaining failure

Debug and Release pass all 69 source-only CTests. The current native aggregate
is 96,442 cases in 34 suites. Seven bounded owned-C menu scenarios cover
load, custom map, campaign, mission, duplicate name, cancellation and deletion.
These tests use isolated copied assets, actual C callback dispatch and owned
framebuffer reads. They do not launch the original game or automate the desktop.

`city-confirm-prices` passes the town/price defect and stops in the missing
music receiver. The later `city-confirm-music` passes that boundary, completes
post-load preparation and reports `STARTUP frame mode=1`. The next Debug
uninitialized-value trap is `0056133c` from `004be770`, called by simulation
tick `004d7090`. `PharaohRewrite` identifies this address in
`src/Pharaoh.Engine/Environment/FloodManager.cs`; its approximate simulation
must still be checked against the retained 303-instruction original body in
`city-simulation-next-native.json`.

That boundary is historical: the later flood-simulation checkpoint restores
the controller and its reviewed region/quality helpers. `city-confirm-flood`
passes it and reaches the raw `0049bd40` actor callback in `00489b70`. See
FLOOD_SIMULATION_RECONSTRUCTION.md for the current evidence and limits.
The still later actor checkpoint passes the prey callback and reaches the
missing animation-record receiver in `0041fdb0`; see ACTOR_UPDATE_RECONSTRUCTION.md.

No completed city frame or playable standalone gameplay is established.
The profile and temporary SAV remain hash-identical; the copied SAV is
removed after the owned diagnostic. Full river/water/model algorithms,
remaining root menus and unreviewed deep endpoints still require reconstruction.

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug
.\build.ps1 -Configuration Release -Verify
python tools/refresh_menu_native_evidence.py
.\tools\Test-MenuIntegration.ps1 -Cases load,map,campaign,mission,duplicate,cancel,delete
.\tools\Test-CityConfirm.ps1 -RunName city-confirm-next
```
