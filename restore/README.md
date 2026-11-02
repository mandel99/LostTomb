# C reconstruction workspace

See the [project README](../README.md) for WIP status, runtime requirements and tools.

Build from the repository root:

```powershell
.\restore\build.ps1 -Configuration Release -BuildGame
```

`src/`, `runtime/`, `include/` and `generated/` form the current C build.
`tests/`, `direct_c/`, `tools/` and the selected source fixtures under
`artifacts/` support compilation and verification. Historical build outputs,
full analysis exports, original game files and local saved games are excluded.

Code files preserve their source bytes because some CMake recipes verify
frozen source hashes. Avoid automatic line-ending normalization of C sources.
