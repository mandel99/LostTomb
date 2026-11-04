# Small-fragment Ollama batch

The batch processes every procedure listed in `units.json`, sequentially, using
the installed local `pharaoh-coder:7b-16k` model. It uses Ollama's local HTTP
API with a JSON response schema. No cloud fallback or model download is used.
The initial plan contains 4,402 procedures and 7,743 fragments. Each editable
fragment is at most 40 lines and 2,500 characters; read-only source context is
limited to another 800 characters. Function signatures, declarations, and
address-based global definitions remain fixed.

From the restore directory, start or continue in the foreground:

```powershell
.\ollama-batch.cmd
```

Or run the worker without an open console:

```powershell
.\ollama-batch.ps1 -Action Start
.\ollama-batch.ps1 -Action Status
.\ollama-batch.ps1 -Action Stop
.\ollama-batch.ps1 -Action Resume
```

`Stop` finishes the current bounded model request/check before stopping. Resume
uses the existing plan and skips completed work. A Windows OS file lock prevents
concurrent workers/reviews of the same batch and releases if a process exits.
`Resume` clears the stop request only after obtaining the worker lock, so it
cannot accidentally cancel a stop while another worker is still finishing.
An Ollama connection failure stops the worker with an explicit error; resuming
retries the unfinished request. Failed code proposals are retained for Codex,
not sent back to Ollama for an automatic repair loop. Changed model digests and
changed input sources are detected.

The default output is `artifacts/ollama-batch/`. A different batch can use:

```powershell
python .\tools\ollama_batch.py --output .\artifacts\another-batch plan --model pharaoh-coder:7b-16k
python .\tools\ollama_batch.py --output .\artifacts\another-batch run --limit 5
```

The plan and immutable input snapshots are retained. Each fragment has the
exact request/response, proposed replacement, compiler log, and trial source.
Responses that remove or change an
existing unresolved-return guard are rejected and queued for Codex ABI review.
Each procedure has a reconstructed candidate and result JSON. `worker.json` reports the current
procedure/fragment and actual worker PID. `status` refreshes `summary.json` and
`REVIEW.md`, including the queue of failures for Codex. Worker stdout/stderr are
stored separately when launched using PowerShell.

## Validation and repairs

Each proposed edit is applied only to a temporary copy of its whole procedure
and compiled as Win32 C. A failed edit is excluded from the combined candidate;
its failed source/diagnostic remains in its fragment directory. Processing then
continues with other fragments. Final candidates are compiled again.

The five scalar functions with existing independent original-x86 fixtures also
run the differential suite, substituting the candidate's object for that
function. Each candidate is exercised on 100,021 boundary/random inputs; the
whole five-function suite performs 500,105 comparisons. Inputs and library
digests are retained. An executable test runs in a bounded child process.

Statuses deliberately distinguish:

- `needs_codex`: invalid output, failed compilation, or failed behavior test.
- `compiled_unverified`: compiles, but has no independent behavior oracle yet.
- `differential_passed_pending_codex_review`: native comparisons passed.
- `codex_reviewed_compiled_unverified`: Codex repaired/reviewed and compiled it,
  but no independent behavior oracle exists, so it is not approved for activation.
- `codex_reviewed_differential_passed`: Codex reviewed/repaired and reran tests.

Compilation alone does not prove ABI, global layout, call contracts, or game
behavior. Most procedures still need an independent harness and analyst work.
The worker never edits `src/`, installs hooks, or publishes a new game build.
Those steps are performed separately by Codex after reviewing evidence.

Codex corrections do not contact Ollama:

```powershell
python .\tools\ollama_batch.py review --unit FUN_00451bc0 --file .\path\corrected.c --notes "Describe the original behavior and correction."
```

Run a review between worker runs; request `Stop` and wait for the current
request/check to finish first. The corrected full procedure must preserve its
original preamble/signature. Review output and test logs have a separate
directory so the initial failing diagnostic remains intact. There is no claim
that Codex reviews or repairs while no Codex turn is running: the worker queues
that work for the next active turn.

## Initial real pilot

Ollama returned five compilable proposals. Four passed their native oracle;
one failed: `FUN_00451bc0` swapped `0x1b7` and `0x133` in a ternary expression.
The original returns `0x1b7` for input 25 and `0x133` otherwise. Codex corrected
the two branches and reran the original-code test successfully. All five then
passed manual review and differential testing. Four changed source bodies were
integrated separately into the project; the fifth remained unchanged.

The failed proposal and first mismatch are retained, along with the corrected
source and passing test. `metadata/ollama-pilot.json` records this pilot. The
number of active game hooks remains 23: these are revisions of existing hooks.
Regenerating the mechanical C sources can replace reviewed source edits; keep
the retained reviewed files/evidence and rerun verification when reapplying.

The implementation follows the official [chat API](https://docs.ollama.com/api/chat)
and [structured output documentation](https://docs.ollama.com/capabilities/structured-outputs).
