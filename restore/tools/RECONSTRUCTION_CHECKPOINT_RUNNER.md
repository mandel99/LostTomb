# Reconstruction checkpoint runner

`run_reconstruction_checkpoint.py` collects the repeated orchestration in one
standard-library Python command. It does not rewrite game code, commit, push,
or launch either host game. Its default action is a read-only plan.

From the repository root:

```powershell
python restore/tools/run_reconstruction_checkpoint.py --phase animal-next --plan
python restore/tools/run_reconstruction_checkpoint.py --phase animal-next --plan --json
python restore/tools/run_reconstruction_checkpoint.py --phase animal-next --run --jobs 4
```

Use a fresh phase name for each execution. Logs and `receipt.json` go under
`restore/build/checkpoints/PHASE/`; existing phases are never overwritten or
resumed. `--log-prefix NAME` changes log filenames within that new directory.

The current registry contains33 primary families,5 supplemental families and3
composition families. The complete plan has94 jobs:9 audits,66 primary runs,
10 supplemental runs,6 compositions and3 serial finalizers. The animal-following
audit runs the local typed cue contract and checks the retained legacy raw model;
the open global cue raw-drop audit is not represented as a passing gate.
Both main targets are Release builds:

| `--configuration` | Artifact |
| --- | --- |
| `diagnostic` | `build/c-only/Release/restore_link_check.exe` |
| `game` | `build/c-only/Release/PharaohRestoreC.exe` |

Omit `--configuration` to check both. Debug/Release source-only music fixtures
remain the responsibility of the existing host-binding finalizer.

For a partial rerun:

```powershell
python restore/tools/run_reconstruction_checkpoint.py --phase command-probe --suite actor_command_parent --configuration game --plan
python restore/tools/run_reconstruction_checkpoint.py --phase command-probe --suite actor_command_parent --configuration game --run
```

Repeat `--suite` or supply comma-separated names. `--list-suites` lists the
registry. Prerequisite primary families are added automatically. Partial runs
do not run the CMake audit, host binding or aggregate, and their receipts have
`complete_checkpoint: false`, even when their selected checks pass.

Every prerequisite must belong to a strictly earlier stage. The planner rejects
same-stage and later-stage prerequisites. Jobs within one oracle stage can run
in parallel; the runner does not implement a dependency graph within that stage.

The full plan runs these stages in order:

1. Required source/caller/startup audits, serially. The audio CRT startup audit
   completes before either `sound_device` run. Each suite declares its audits
   in the registry, including the actor command, Hippo Stomp and animal target
   audits.
2. All33 primary families for both main EXEs, with at most `--jobs` subprocesses.
   Music configuration is one primary family and runs only once per variant.
3. Allocation/music counterexample replays, actor queues, formation control and
   town record queries.
4. Scenario initializer compositions, command/search compositions and the
   command parent, after their primary reports exist.
5. Only after every previous subprocess has returned: the exact CMake scope
   audit, then music host binding, then the current aggregate. This last stage
   is serial. The CMake audit performs an actual incremental main build and
   requires both tested EXEs/MAPs to remain byte-identical. Host binding executes
   only the existing controlled source-only test fixtures.

`SUITES` is the runner's single family registry. A new entry supplies the name,
optional prerequisites/audits and any nonstandard report stem. The family's
verifier still owns its native coverage contract; the current aggregate must
also recognize its proof. A full run refuses to report a complete checkpoint
if that aggregate omits any registered primary report.

Execution requires already-built main EXEs/MAPs and the pinned original PE.
Finish edits and stop builds or verifiers that write the same reports first.
An independently launched new-family pair may overlap a partial runner for
older families only when both use the same frozen sources/binaries and their
report paths are disjoint. Reserve workers for that pair, keep source audit
outputs deterministic, and wait for both groups' actual exits before serial
finalization. The partial runner cannot establish that external barrier itself.
The runner snapshots C/header/assembly inputs, verification scripts and main
EXE/MAP bytes; it refuses later stages if those change. It adds no report-hash
compatibility exceptions. The existing audited CMake exception remains inside
its existing verifiers.

A literal `build/reconstruction-checkpoint.lock` prevents simultaneous uses of
this runner. It cannot detect an unrelated manually launched verifier or build.
If a process is forcibly killed, inspect a retained lock and its PID before
removing it. There is no automatic stale-lock deletion.

On failure, queued jobs and later stages are skipped, while already-running
verifiers are allowed to finish. Ctrl+C likewise stops new scheduling and drains
the actual child exits; it can therefore take time to return. A forced process
kill can leave a `running` receipt/lock. No finalizer runs after a failed or
interrupted stage.

The receipt records actual commands, child PIDs, exit codes, elapsed times,
script/log/report SHA-256 hashes, source and binary snapshots before/after,
errors and jobs not run. A child that exits0 but supplies an unpassed or
incorrectly bound report fails the run. Existing verifiers write their usual
reports; the runner never edits those reports or replaces their stored hashes.
A successful receipt describes this finite verification scope, not complete
gameplay equivalence.

To reduce repeated work further, convert a larger connected set of functions
before running one shared regression checkpoint. Agents can analyze separate
members while integration keeps a frozen tested source snapshot. This runner
automates those shared batches; it does not skip older tests merely because a
change appears unrelated. Skipping by narrow dependencies would require a new
audit of machine-code bodies, relocations, constants and transitive callees.
The current whole-binary SHA-256 bindings cannot simply be reassigned to a new
binary. That optimization is intentionally not implemented here.

Implementation validation used `--help`, full/partial plans, argument rejection
and temporary fake Python verifiers. The isolated harness checked stage barriers,
real child exits, digests, failure draining, incorrect report binding, source
mutation and Ctrl+C draining/exit130. The harness never runs the expensive corpus, an incremental build or host
fixtures; production verification remains a separate checkpoint.
The isolated review harness is tracked in
`tests/test_reconstruction_checkpoint_runner.py`:

```powershell
python restore/tests/test_reconstruction_checkpoint_runner.py
```

It also checks rejection of same-stage prerequisites. Each invocation retains
a fresh sandbox and result under that review directory; it performs no cleanup
and executes only its generated fake Python verifiers.
