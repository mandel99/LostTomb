# Pharaoh clean reconstruction

This is the new dependency-minimal C reconstruction. Original executable
functions live under `game/`; tests and evidence are kept outside production
source. The old `restore/` tree remains an evidence and migration source, but is
not linked by this project.

An implementation enters this project only when it has:

- an original address and executable-derived evidence;
- typed inputs and state instead of absolute process addresses;
- no generated instruction model, register frame, or legacy address macro;
- no call back into the old reconstruction;
- provenance and verification appropriate to the source: focused offline tests
  for newly reconstructed behavior, or strict integration compilation for an
  unchanged translation already confirmed in the previous project.

The project builds a static C library and a Windows bootstrap executable. Its
icon, cursor source files, and version description are compiled directly from
`resources/PharaohClean.rc`; no generated resource binary is linked. The
bootstrap displays a basic window, uses the project icon/cursor resources, and
checks for the expected game-data files. It does not yet initialize the game,
load its archives, play startup video, or enter the menu. Building it does not
launch it or the original game.

Configure and test with a normal CMake toolchain:

```powershell
cmake -S restore/clean -B restore/clean/build
cmake --build restore/clean/build --config Release
ctest --test-dir restore/clean/build -C Release --output-on-failure
```

To reproduce the Win32 Release bootstrap build with one command, run
`./restore/clean/build.ps1`. It uses the previous project's Visual Studio 2022
Win32 Release configuration and writes
`restore/clean/build/bootstrap-vs2022-win32/Release/pharaoh_clean_bootstrap.exe` and
prints the executable's size and SHA-256 without launching it.

Repeated migration discovery and structural checks are automated by
`tools/migration_workflow.ps1`. `discover` lists unmapped single-owner C
sources while filtering obvious legacy bridges, `plan` emits the exact address
and hash manifest for selected sources, and `verify` audits the map, source
hashes, CMake coverage and clean-source policy. Algorithm translation remains
manual because these structural checks cannot prove behavioral equivalence.

```powershell
./restore/clean/tools/migration_workflow.ps1 discover
./restore/clean/tools/migration_workflow.ps1 plan -Source Source/Level/EventManager/Actions.c
./restore/clean/tools/migration_workflow.ps1 verify
```

The current source/evidence mapping is recorded in
`evidence/function-map.json`. Passing tests establish only the covered logical
contracts, not complete gameplay equivalence.

## Migration status

The clean library currently owns 260 original-address functions. The second
batch added 20 functions covering ring queues, finance, population, religion
and small static objects. The third batch added 20 event reason, message,
resource and classification queries. Both batches reused translated and mapped
C from the old project. The fourth batch added 20 risk and house-service
overlay columns using typed building state and explicit rendering callbacks.
The fifth batch added 28 request, message and monument text-token functions
using explicit state, callbacks and caller-owned buffers. The sixth batch
added 24 event action processors with typed state and external effect
endpoints; its sole Restore wrapper was excluded. The seventh batch added 18
monument stage/work methods and two industry queries. The eighth batch added
20 clean level-editor event actions and rejected seven Restore-backed bodies
from the same previous source. The ninth batch added 30 functions covering
monument tile removal, overlay metrics and columns, military strength/target
queries and terrain panels. The tenth batch added 22 industry building panels behind a typed semantic UI
command endpoint, including farm, producer, workshop and guild state logic.
The eleventh batch added 20 market-selection, house-service, label-rendering
and military-transport functions. One market-storage candidate was rejected
because its old body still used absolute work-table addresses.
The twelfth batch added four gift-response and five political-overseer actions;
native callback addresses were replaced by named popup endpoint kinds.
The thirteenth batch migrated two ship status queries; the fourteenth added
enemy ship escape, sound and target queries with typed map callbacks.
The fifteenth batch added the enemy ship reroute timeout transition.
The thirteenth batch records two typed ship hull and crew status queries.
These batches
received strict integration compilation but no
duplicate behavioral tests. Their prior ownership and address evidence remains
in `metadata/GameCurrentReferenceBindings.json` in the old tree.
