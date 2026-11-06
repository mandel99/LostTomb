Use the shared runner with `--scope player10`.

These51 cases cover ten existing canonical C procedures. Source repairs concern
DWORD scalars, BYTE/WORD record fields, byte address bases, formation/descriptor
strides, statistics indexing and explicit32-bit wrap.420550 and420600 already
match their bounded contracts and are preserved. Return comparisons use AL for
420550, full EAX for446260/44D520, and memory/callback effects for the others;
native caller-consumer qualifications are stored in the checkpoint metadata.

The current applicability is164 routes, including native.420550,4269B0 and
446260 have canonical entries only in the retained comparison image. All input
frames are actual physical calls with declared native/C conventions.

Finite cases, controlled external boundaries and native-width facts do not
establish unrestricted function or whole-game equivalence.
