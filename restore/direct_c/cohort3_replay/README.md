The third cohort has its own immutable comparison epochs. Historical cohort2
receipts remain unchanged. Build `restore_cohort3_retained` explicitly to retain
every selected C definition, including those discarded from the main images.
The verification image is mapped only by the offline emulator.

`python restore/direct_c/cohort3_replay/registry.py` checks native bytes, source
hashes and selected providers. `replay.py` plans cases; `replay.py --run` performs
the currently authored families and saves exact input/artifact hashes before
execution, with another check afterwards. The initial families cover table
lookups and96-bit carry/rounding, including canonical lookup flags/registers.
Other selected entries remain explicit pending work; this is a staged comparison,
not a claim that the whole cohort or game is verified. No host game is launched.
