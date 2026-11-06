QuickTime support reconstruction
=================================

The nine existing support entries now use recovered selector, loader and component
parameter contracts. The window callback supplies the original MSG and a separate
16-byte event output, then reloads the controller and forwards the same event.
Already-correct loader algorithms are reused. This packet adds no selected source
entries and makes no claim about external SDK bodies or complete gameplay.

`python restore/direct_c/cohort3_replay/quicktime_support/verify.py` checks exact
native/source/image bindings and plans 51 cases / 204 offline invocations.
Add `--run` to compare the pinned native PE and three compiled images in Unicorn.
No executable is launched and no media DLL is loaded. `--scope core` selects 46
recipes; `--scope window` selects five capture/filtered-default recipes.

The named QuickTime provider is the canonical FUN export itself. Each physical
provider is executed once, without a duplicate route to the identical address.
The real Win32 callback adapter is tested through its decorated stdcall symbol.
The window native extent is historical pinned raw evidence including branch data;
it is not represented as a fabricated Ghidra function/instruction inventory.

Cases independently specify import order, exact arguments, selector EAX, mutable
export slots, component bytes and results. Window cases verify the identity and
contents of the converted event, including controller mutation by the converter.
They use the original mode T filter; general message handling is outside scope.
An initial attempt unintentionally entered general handling / 41D110 and failed
five fixture cases. Its report is retained in protocol.json; correcting fixture
mode selection required no further C changes. No failed run is labeled passing.

SDK argument arities, long or unterminated paths, concurrent module races and
other window branches remain unproved. See protocol.json for exact limitations.
