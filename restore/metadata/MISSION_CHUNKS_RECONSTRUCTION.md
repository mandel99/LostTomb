# Mission chunk loading and decompression

The independent build now compiles the native chunk loader and its complete
decompression path from C. The original executable is used only as a static,
SHA-256-pinned verification oracle. The C test programs and game diagnostic
never map or execute its machine code. `../PharaohRewrite` remains a read-only
reference for addresses and format hypotheses.

## Recovered contracts

- `0055bdb0` and `0055be50`: omitted read/write callbacks with the native
  52-byte codec context, buffer limits, error propagation and CRC updates.
- `0055c0f0`: missing three-argument CRC32 implementation, verified against
  the original instructions and lookup table.
- `0055c000`: typed context and actual C callback pointers for the recovered
  DCL decoder. The old five-DWORD local was written as thirteen DWORDs.
- `0055bcf0`: full-width scratch pointer and signed marker, zero blocks,
  uncompressed fallback and compressed envelopes with output capacity.
- `004d0e10`: native 20-byte descriptor and 16-byte migration entries, full
  version/count fields, byte-addressed serialized-schema lookup, correct
  decompression lengths and explicit C dispatch for migration callbacks.
  Its return is AL; upper EAX bits are not a Boolean contract.
- `004d0c60` and `004d0d40`: omitted old-record conversions at version
  cutoffs 130 and 136. The former expands 150 town records from 104 to
  124 bytes, including constructor defaults. The complete town-list object
  is 18,628 bytes, including its 24-byte metadata tail.

Only two migration callbacks occur in the native Save/Map descriptor tables.
Their locations and complete tables are retained in `native-chunk-schemas.json`.
The new dispatcher calls compiled C implementations and rejects unknown addresses.

The C code defines unread stack padding as zero. The native oracle explicitly
normalizes only the fourteen proven uninitialized metadata tail bytes in the town conversion
and final two bytes of the 44-to-48-byte record conversion. This does not alter
initialized fields or ignore differences in game data. The precise hooks and
native instruction addresses are recorded in `chunk-io-emulation.json`.

## Verification

`compression-emulation.json` covers 212 cases, including 48 actual compressed
blocks from versions 147 and 160. Both the direct codec and file envelope are
compared: full output, guards, CRC, error paths, read requests and ABI.

`chunk-io-emulation.json` covers descriptor resizing, old/new shapes, raw and
compressed endpoints, full-width counts/versions, allocation failures and both
actual native migration functions. Its codec endpoint is controlled because
the codec has its separate instruction-level comparison.

`mission-chunks-emulation.json` then joins the real chunk loader and decoder.
It compares complete data streams from mission1.pak entries 0, 1 and 52 and the
existing isolated smoke-game autosave. These contain 712 chunks in total;
every chunk SHA-256, read count, file position, return, version/count fields
and native stack/saved registers agree. Every stream is consumed completely,
without codec errors. The original loader and decoder instructions run together
in the oracle, with no replacement of the decoder by a managed implementation.
`mission-chunks-corpus.json` records source and extracted fixture hashes.

REA evidence for `004d0e10` is retained in `mission-chunks-rea-function.json`
(evidence `ev_afb5b8fa35cef480c685f62c9dc89b68f80141b3e96e540a352c76c02b94bb18`).
Ghidra confirms the same executable hash. The migration disassembly is retained
in `../artifacts/chunk-migrations-native.txt`.

Reproduce the native checks after a Debug build:

```powershell
python tools/prepare_compression_corpus.py
python tools/verify_compression_emulation.py
python tools/inspect_migration_tables.py
python tools/verify_chunk_io_emulation.py
python tools/verify_mission_chunks_emulation.py
```

The native checks require the local original reference and verification assets;
ordinary Debug/Release builds and CTests do not. Source-only CTests exercise
the codec, chunk contracts and real-schema header pipeline with synthetic
in-memory data, without installed game files.

## Remaining work

The next verified layer restores route interpolation and the top-level
controllers/wrappers. Complete v160 MAP loading and a nonempty owned C preview
are covered in [the route and preview notes](MISSION_ROUTE_RECONSTRUCTION.md).
Complete v147/v160 SAV loading and reached compatibility helpers now have
separate native checks in [the legacy mission notes](LEGACY_MISSION_RECONSTRUCTION.md).
Builtin preview integration, city preparation, briefing and
playable independent gameplay remain. No original game process or desktop
automation was used for these checks.
