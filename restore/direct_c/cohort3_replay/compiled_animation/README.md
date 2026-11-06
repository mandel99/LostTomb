Existing C animation comparison

25 finite cases /160 applicable routes were replayed before publication.
Use run.py --run for offline replay; omitting --run only checks the plan.
The original executable and exact recorded compiled images are required.

41FD80 executes the existing complete C register model, supplied with every
live CPU input and its genuine native argument stack. The physical C stack
is separate. Defined flags and all eight architectural registers are compared
with the native return. Logical memory checks cover the DWORD actor input,
WORD field truncation, BYTE state, wraparound and a source/store alias.
The unretained typed carpenter helper is not substituted or rewritten.

41FDB0 executes its actual existing explicit-self C function. Cases cover
linked state, signed bounds, frame wrapping/clamping, real fallback children,
transition choices, table reloads after RNG and a frame/max read alias.
Unvisited transition rows, arbitrary callback mutations and fault inputs
remain outside this finite matrix. Complete gameplay remains unproved.
