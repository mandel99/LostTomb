# Family actions and campaign selection

The C# project `../PharaohRewrite` is an address map and a source of hypotheses.
The original x86 instructions in Pharaoh.exe (SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`)
determine the restored behavior. No original game process is launched, and no
original machine code is linked or called by the independent C executable.

`runtime/family_options.c` restores the four missing family action callbacks at
004cb800, 004cb860, 004cb910 and 004cb940. The existing 004cb970 body is routed
through the C callback resolver. The campaign-completion predicate 0041ba80 now
has an explicit object receiver and the native record layout. Resume/history,
mission selection, load-save, custom missions and return-to-registry transitions
are compared with native instructions in 2,329 cases. Nested controllers/loaders
and the operating-system confirmation dialog are controlled endpoints here.

`runtime/campaign_menu.c` restores the missing selection/confirmation/tab
callbacks. The selector foreground/background, availability check and back
action retain full DWORD state, sprite/font IDs and signed coordinates. The
two bottom tab rectangles use the native 28-byte stride; the old DWORD-pointer
expression advanced 56 bytes. The native 0041bad0 predicate returns zero in AL
while retaining the parameter's upper EAX bytes. Consumers must test AL.

The selector comparator covers 5,310 cases: 640x480, 800x600 and 1024x768,
unlocked and locked categories, full-width values, hover states, both movie
variants, selection and confirmation, tab conditions and back navigation.
Actual native predicates execute in bounded Unicorn emulation. Final draw,
movie, mission-loading and nested-controller endpoints are controlled; these
checks establish their arguments, ordering and state, not complete gameplay.

REA analysis is retained in `campaign-selector-rea-function.json`. The separate
Ghidra session confirms the same original executable hash. Native assembly is
also retained under `../artifacts/`.

Reproduce verification from this directory:

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug *> artifacts/menu-routes-debug-build.log
.\build.ps1 -Configuration Release -Verify *> artifacts/menu-routes-release-build.log
python tools/refresh_menu_native_evidence.py
.\tools\Test-MenuIntegration.ps1 -Cases map,campaign,mission,duplicate,cancel,delete
python tools/record_campaign_menu.py
python tools/record_mission_menu.py
python tools/record_menu_dialogs.py
python tools/record_mission_map.py
python tools/record_menu_reconstruction.py
```

The owned-child integration uses isolated copied assets and the known C-created
Restore profile. It supplies input directly to the recovered C dispatcher and
captures immutable internal frame copies; it does not automate the desktop.
The debugger ends each run at its expected deadline. The harness restores the
profile and removes only its verified marker files after the deletion cases.

The next restoration covers the custom/individual mission selector (mode 61).
`0041dfc0` and `0041dfe0` require explicit list receivers. The latter returns
DWORD 0/1 for valid indices and 3 for invalid indices; it is not a Boolean-only
contract. All reviewed callers now pass `RESTORE_MISSION_FILES` explicitly.

The mode-61 foreground/input branches are restored in `runtime/mission_menu.c`.
Native stack arguments that the decompiler assigned to no-argument getter calls
are now passed to the actual draw/input routines. Row/tab rendering, scrolling,
dragging, mission activation and screen entry retain full counts/coordinates.
The foreground renderer uses signed WORD year/thumbnail fields, DWORD objectives,
76-byte results records and unsigned label IDs 220/221. The difficulty score
uses 124-byte event records and preserves the native scan of indices count-1
through 1. Missing toggle, confirm and difficulty callbacks are routed in C.

`mission-menu-emulation.json` compares 11,000 native cases, including original
inline dispatcher branches at 004dcfca and 004d801c. Final UI/text endpoints,
town-connectivity analysis, map/scenario loading and difficulty simulation
updates are controlled in this suite. These endpoints are not claimed to be restored merely
because the calling controller matches. REA evidence is retained separately in
`mission-menu-rea-function.json`.

A real C run then stopped in 0044bb90 because the old source used an
uninitialized `in_ECX`. The iterator now has an explicit implementation;
the existing two-argument C facade selects the global town list. All 24 native
call sites were reviewed and select that same receiver, including one with a
longer setup sequence. The caller evidence is in
`town-iterator-callers-native.json`.

`runtime/town_menu.c` and unit 00301 restore the actual connectivity walk and
unconnected-town count with 124-byte records, checked signed link IDs, native
type/mode predicates, recursion limit and tail-link depth behavior. A separate
`town-menu-emulation.json` compares 652 cases against the actual original
iterator, graph walker, predicates and checked lookup without endpoint stubs.
Full contents of both the global list and an alternate receiver, global side
effects and native return/stack contracts are compared, including cycles,
empty lists, sentinel creation and both recursion stopping paths.

The C# method tagged 0041f870 selects a different screen in the rewrite; the
restored C follows the original's explicit assignment of screen 0x3d. This is
one reason the C# project remains a reference instead of the behavioral oracle.

The owned C integration covers the isolated empty selector and its Back button,
and now a nonempty v160 MAP preview of The Battle of Actium. Its scenario hash
and four routes with 240 points match the native complete-load comparison.
The underlying chunk/codec pipeline also agrees for 712 chunks across four
v147/v160 mission/save streams. See [the chunk notes](MISSION_CHUNKS_RECONSTRUCTION.md)
and [the route/preview notes](MISSION_ROUTE_RECONSTRUCTION.md).

The complete SAV controller and reached v147 compatibility helpers now match
four native streams; see [the legacy mission notes](LEGACY_MISSION_RECONSTRUCTION.md).
The load/save selector also has native and owned C selection/cancel checks;
see [the file-menu notes](LOAD_SAVE_MENU_RECONSTRUCTION.md).
Remaining work includes builtin preview integration, confirmed saved-game city
loading, mission preparation and briefing (including unit 00222),
Go to city and the other root menu branches. A working preview does not
establish playable independent gameplay.
