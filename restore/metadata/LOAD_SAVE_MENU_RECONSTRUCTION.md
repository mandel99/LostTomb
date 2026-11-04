# Load/save file menu reconstruction

All implementation changes are in the independent `restore` C project.
`PharaohRewrite/src/Pharaoh.Engine/UI/LegacyFileMenu.cs` supplies address and
structure hypotheses. The original x86 instructions, including omitted local
procedures read through Ghidra, determine the behavior.

## Recovered behavior

- `00402450` enters saved-game loading. `00402540` and `00402630` are omitted
  local save/delete entries. They initialize screen 13, its operation byte,
  return mode, filename and text-input registration in native order. The load
  entry additionally clears DWORD `006059e8`; the other entries preserve it.
- `004cdff0` enumerates the actual global file list, strips extensions, sorts
  and resets scroll/selection state. Its previously restored explicit receivers
  are retained. The current tests execute this controller and the real C/native
  extension transformations, with enumeration and sorting controlled.
- The omitted `00402a80` row callback reads the native selected row plus scroll,
  copies the name to the correct filename field, then resets the input/caret.
  The native test covers both fields (screens 13 and 16), full-width IDs and
  negative relative row values when their resulting file index is valid.
- `00402720` follows the original cancel/load/save/delete/overwrite branches,
  sound stop, extension removal/restoration and profile-update order. The
  existing explicit profile and sound receivers are retained. Cursor/hover
  resets and action IDs now use DWORD fields.
- `00402980`, `004029d0` and `00532830` restore scrolling, bounded drag geometry
  and both scrollbar variants with full counts, coordinates and sprite IDs.
- `00532340` and `005325f0` draw the native file-menu panels, twelve rows,
  highlight font, titles, error countdown, filename and controls. Pointer fields,
  offsets and font IDs previously misdeclared as bytes are now full width.
- `runtime/load_save_menu.c` reconstructs the original mode-13 input branch
  inside `004d8310`, including continuing the current branch after confirmation
  changes mode, Escape, complete hover return values and callback order.
  The map-file branch retains its original common tail after extraction.
- Actual C callback routing includes all six recovered entry/selection/scroll/
  confirm addresses; no jump into the original executable is used.

## Native evidence

`load-save-menu-emulation.json` records 1,968 native comparisons, including
empty and large lists up to 300 files, viewport offsets for 640x480, 800x600 and
1024x768, all operation modes, error/countdown cases, hover IDs, scroll/drag
boundaries, filename fields and overwrite decisions. Watched state includes
complete filename storage and list strings with guards. Ordered endpoint calls,
their arguments and intermediate global state, plus stack and callee-saved
register behavior, agree.

The raw native selector and local entry bodies are retained in
`load-save-native.json`. The exact inline branch is in
`load-save-input-native.json`. REA independently analyzes the pinned executable
at `00402450` in `load-save-rea-function.json`, evidence
`ev_d21be4d119cbbb8b9047f321e20d6e575cf61acab3b3e2aa7442c120c8112b06`.

Filesystem enumeration/sort, final text/pixel operations, text-input registry
and deep city load/save/delete endpoints are controlled in this focused native
suite. Malformed selections that would dereference NULL in the original are
excluded; the C row callback returns after clearing the destination instead.
The controller still uses the native filename/path capacities for valid names.

## Owned C integration

`tools/Test-MenuIntegration.ps1 -Cases load` copies only the hash-verified
`mission-00.sav` fixture into the isolated `Save/Restore/restore-native.sav`.
The ordinary family options detect that file. Internal diagnostic input then
uses the actual C button dispatch to open loading, select its first row, and
cancel back to family options. No desktop input or original game process is
used. The fixture is verified unchanged and removed after the run; the C-created
profile remains byte-for-byte intact.

Immutable completed frames are captured before selection, after the selected
name has actually been rendered, and after returning. The diagnostic must skip
the render immediately preceding input dispatch before retaining the selected
frame. `tools/record_load_save_menu.py` requires different entry/selected frame
hashes, the complete live filename and list state, the actual callback trace,
fresh native evidence, normal bounded timeout and fixture cleanup.

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug
.\build.ps1 -Configuration Release -Verify
python tools/refresh_menu_native_evidence.py
.\tools\Test-MenuIntegration.ps1 -Cases load,map,campaign,mission,duplicate,cancel,delete
python tools/record_campaign_menu.py
python tools/record_mission_menu.py
python tools/record_menu_dialogs.py
python tools/record_mission_map.py
python tools/record_load_save_menu.py
python tools/record_menu_reconstruction.py
```

Ordinary CTests use synthetic data and require neither original EXE nor assets.
Real C rendering is currently captured at 1024x768; the other viewport offsets
have native draw-call coverage. Confirmed saved-game loading through `004d5100`
and subsequent city preparation now pass terrain graphics, trade-price
preparation, music and the repaired river/flood simulation in the owned C run.
Gameplay mode 1 and actor/prey updating are reached; the missing animation-record
receiver in `0041fdb0` prevents a completed city frame. See CITY_PREPARATION_RECONSTRUCTION.md
and ACTOR_UPDATE_RECONSTRUCTION.md for the contracts and diagnostic limits. Actual save and
delete filesystem actions, the rest of map/editor input and other root menus
remain to be reconstructed. Complete independent gameplay is not established.
