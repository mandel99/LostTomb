# REA and decompilation cache

All 207 REA tools available in this workspace are cataloged in `rea-tool-catalog.md`.

For the pinned Pharaoh executable (`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`), REA's Ghidra provider listed 4,596 analyzed procedures: 4,402 internal procedures (including 12 thunks) and 194 external entries. The complete inventory is retained in `rea-procedure-inventory-049.json`.

The saved typed Ghidra export contains 3,210 pseudocode bodies. The local REA bundles currently retain 55 pseudocode observations. Those are distinct coverage measures: the procedure inventory does not contain bodies, and the Ghidra export does not represent every procedure in the inventory.

Use `python tools/decompilation_cache.py` to refresh `decompilation-cache-index.json`. Look up an address before requesting REA again:

```powershell
python tools/decompilation_cache.py --address 0x00540B50
python tools/decompilation_cache.py --address 0x00540B50 --show rea
python tools/decompilation_cache.py --address 0x004CC560 --show ghidra
```

The index reports known functions even when no pseudocode body is saved. A cache miss or inventory-only result is the point to request one REA decompilation; export its Evidence bundle under `evidence/rea-*.json`, then rebuild the index. This avoids repeating analysis while keeping the original Evidence records intact.

The inventory is complete for procedures exposed by this provider analysis of the pinned EXE. It is not proof of whole-program behavioral equivalence or complete runtime call coverage.
