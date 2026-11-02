# LostTomb

**Work in progress - experimental C reconstruction of Pharaoh + Cleopatra 2.1.**

LostTomb reconstructs the behavior of the 1999-2000 game in C. The current
Windows x86 build contains a game launcher, menus, rendering and simulation
paths, but reconstruction and validation are ongoing. Crashes, graphical
issues and incomplete gameplay behavior are still possible.

## Download and run

New tagged builds publish `PharaohRestoreC.exe` directly in
[Releases](https://github.com/mandel99/LostTomb/releases), without a runtime ZIP.
Place it into your own Pharaoh + Cleopatra 2.1 installation, beside
`Pharaoh.inf`, `Language.inf` and the `Data` directory, and run
`PharaohRestoreC.exe`. Keep the original installation's supporting files and
media DLLs available there. The launcher applies 16-bit color compatibility
when needed.

The runtime uses the original game's data files. Game data archives, music,
videos, original executables and third-party media DLLs are not distributed
with this project. The reconstructed launcher does not need the original EXE.
Use a separate copy of your installation and saves while testing this WIP.

## Tools used

- **Rea and Ghidra:** examination of the original binary, function boundaries,
  instructions, references, calling conventions and data layouts.
- **Capstone:** instruction decoding and checks against the reference bytes.
- **Unicorn and Python:** offline comparisons between original x86 routines
  and compiled C, including memory writes, arguments and return behavior.
- **Microsoft Visual C++, CMake and MSBuild:** Windows x86 compilation and
  linking; **PowerShell** for build and packaging scripts.
- **OpenAI Codex:** assistance with reconstruction, fixes and automation.
  **Ollama** was also used in earlier drafting experiments; drafts require
  review against native evidence.

## Build the C sources

Requirements: Windows, Visual Studio 2022 or 2026 with the Desktop development
with C++ workload, a Windows SDK, and CMake 3.25 or newer. Build an **x86/Win32**
target even on 64-bit Windows.

```powershell
.\restore\build.ps1 -Configuration Release -BuildGame
```

The output is `restore/build/c-only/Release/PharaohRestoreC.exe`.
The default C build uses the checked-in C sources, initialized data and Windows
resources. It does not need an original EXE or game installation to compile.
Running the game requires the supporting game data described above.

The default diagnostic is `restore_link_check.exe`; running it without
arguments checks linking and data initialization and does not start the game.
`-BuildVerification` builds the broader standalone verification targets.
Native comparison scripts need their separate local reference binary and
Python tooling; those inputs are not included in the release.

## Automatic tagged releases

GitHub Actions builds a Windows x86 Release **only when a Git tag is pushed**.
Branch pushes and pull requests do not trigger a build. Create each release
from a commit that includes the release workflow:

```powershell
git tag -a v0.1.2-wip.20261102 -m "Next WIP build"
git push origin v0.1.2-wip.20261102
```

The workflow builds the C sources, runs the link/data diagnostic and publishes
the EXE, run instructions, build identification and SHA-256 checksums directly
as release assets. Game data and third-party DLLs are not included.
Releases remain marked as work in progress / prerelease.

After successful publication, only the **three most recently published
releases** are kept, including prereleases. Older releases and their assets are
deleted; Git tags and source history are preserved. Drafts are not pruned.
The workflow uses the repository's automatic `GITHUB_TOKEN`; no personal token
is required. GitHub's automatic source ZIP/TAR links remain available.

## Current status

This is a **WIP prerelease**, not a finished reconstruction. Recent work covers
campaign progression, terrain and overlay rendering, building previews,
monument previews, saved-state handling and event dispatch. The current sources fix
the mausoleum placement-preview crash caused by an undersized geometry buffer,
incorrect table indexing and truncated coordinates. It also retains the fix
for the missing event-object receiver in the simulation path.

Passing a focused native/C comparison does not establish full-game fidelity.
Remaining work includes checking legacy function contracts and field widths,
finishing gameplay behavior, and broader visual and gameplay validation.
The mausoleum fix passed 44 focused native/C emulation cases covering all four
rotations and placement acceptance. Live gameplay confirmation remains pending.

Please report reproducible steps, the release tag, Windows version and the
mission involved. Logs and crash dumps can contain local or saved-game data;
review them before sharing publicly.
