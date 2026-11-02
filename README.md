# LostTomb

**Work in progress â€” experimental C reconstruction of Pharaoh + Cleopatra 2.1.**

LostTomb reconstructs the behavior of the 1999â€“2000 game in C. The current
Windows x86 build contains a game launcher, menus, rendering and simulation
paths, but reconstruction and validation are ongoing. Crashes, graphical
issues and incomplete gameplay behavior are still possible.

## Download and run

Download the Windows x86 ZIP from [Releases](https://github.com/mandel99/LostTomb/releases).
Extract it into your own Pharaoh + Cleopatra 2.1 installation, beside
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

## Current status

This is a **WIP prerelease**, not a finished reconstruction. Recent work covers
campaign progression, terrain and overlay rendering, building previews,
monument previews, saved-state handling and event dispatch. The latest binary
includes the fix for the missing event-object receiver in the simulation path.

Passing a focused native/C comparison does not establish full-game fidelity.
Remaining work includes checking legacy function contracts and field widths,
finishing gameplay behavior, and broader visual and gameplay validation.
The latest event fix was checked in offline emulation, including a read-only
saved-state fixture; live gameplay confirmation remains pending.

Please report reproducible steps, the release tag, Windows version and the
mission involved. Logs and crash dumps can contain local or saved-game data;
review them before sharing publicly.
