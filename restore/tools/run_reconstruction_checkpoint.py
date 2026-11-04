"""Plan or run a finite reconstruction checkpoint without launching the game.

The default is a read-only plan. --run creates a new log directory, runs the
registered offline verifiers, and records their actual results and hashes.
Reports are never rebound, edited, resumed, or accepted merely from old hashes.
"""
from __future__ import annotations

import argparse
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait
from dataclasses import dataclass
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
STAGES = ("audit", "primary", "supplemental", "composition", "final")
CONFIGURATIONS = {"diagnostic": "restore_link_check.exe", "game": "PharaohRestoreC.exe"}
NATIVE_SHA256 = "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"


@dataclass(frozen=True)
class Suite:
    name: str
    stage: str = "primary"
    prerequisites: tuple[str, ...] = ()
    audits: tuple[str, ...] = ()
    report_stem: str | None = None
    report_overrides: tuple[tuple[str, str], ...] = ()

    def report(self, configuration: str) -> str:
        override = dict(self.report_overrides).get(configuration)
        if override:
            return override
        return f"metadata/{self.report_stem or self.name.replace('_', '-')}-{configuration}.json"


# Add each new family here once. Dependencies are included in partial plans.
# Verification/coverage contracts remain owned by its verifier and aggregate.
SUITES = (
    Suite("actor_message"),
    Suite("sound_banks", report_stem="sound-bank"),
    Suite("sound_device", audits=("audit_audio_global_startup",)),
    Suite("sound_playback"),
    Suite("sound_stopping", report_stem="sound-stop"),
    Suite("sound_allocation"),
    Suite("sound_samples", report_stem="sound-sample"),
    Suite("sound_ambient"),
    Suite("actor_voice"),
    Suite("sound_fades", report_stem="sound-fade"),
    Suite("message_category"),
    Suite("building_access_search"),
    Suite("building_destruction"),
    Suite("labor_allocation"),
    Suite("building_access", audits=("audit_building_access_returns",)),
    Suite("building_removal"),
    Suite("map_surface"),
    Suite("access_repair_flood"),
    Suite("map_region"),
    Suite("building_occupancy"),
    Suite("ring_growth"),
    Suite("building_processing", audits=("audit_processing_receiver",)),
    Suite("sound_streams", report_stem="sound-stream"),
    Suite("byte_string_compare"),
    Suite("sound_file_lifecycle"),
    Suite("actor_reserved", audits=("audit_actor_reserved_callers",)),
    Suite("actor_scenario_spawn"),
    Suite("actor_command_transition", audits=("audit_actor_command_transition",)),
    Suite("actor_spawn_hippos", audits=("audit_actor_spawn_hippos",)),
    Suite("music_configuration"),
    Suite("actor_animal_target", audits=("audit_actor_animal_target",)),
    Suite("actor_animal_parent", audits=("audit_actor_animal_parent",)),
    Suite("actor_animal_following", audits=("audit_actor_animal_parent", "audit_actor_animal_following")),
    Suite("sound_allocation_regressions", "supplemental", ("sound_allocation",)),
    Suite("music_configuration_regressions", "supplemental", ("music_configuration",)),
    Suite("actor_ring_queue", "supplemental", report_overrides=(("game", "metadata/actor-ring-queue-reconstruction.json"),)),
    Suite("formation_control", "supplemental"),
    Suite("town_record_queries", "supplemental"),
    Suite("actor_scenario_spawn_composition", "composition", ("actor_scenario_spawn",)),
    Suite("actor_command_transition_composition", "composition", ("actor_command_transition",)),
    Suite("actor_command_parent", "composition", ("actor_command_transition", "actor_spawn_hippos")),
)
FINALIZERS = (
    ("verify_music_cmake_scope", "metadata/music-cmake-scope-audit.json"),
    ("verify_music_configuration_host", "metadata/music-configuration-host.json"),
    ("verify_labor_access_current", "metadata/labor-and-access-current.json"),
)


@dataclass(frozen=True)
class Job:
    identifier: str
    stage: str
    script: str
    configuration: str | None = None
    report: str | None = None

    def command(self, root: Path) -> list[str]:
        command = [sys.executable, str(root / self.script)]
        if self.configuration:
            command.append(str(root / "build/c-only/Release" / CONFIGURATIONS[self.configuration]))
        return command


def digest(path: Path) -> str:
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(block)
    return value.hexdigest()


def utc_now() -> str:
    return datetime.now(timezone.utc).isoformat()


def select_suites(names: list[str] | None) -> tuple[list[Suite], list[str]]:
    registry = {suite.name: suite for suite in SUITES}
    if len(registry) != len(SUITES):
        raise ValueError("Duplicate family in SUITES registry")
    requested = set(names or registry)
    unknown = requested - registry.keys()
    if unknown:
        raise ValueError("Unknown suite(s): " + ", ".join(sorted(unknown)))
    selected = set(requested)
    visiting: set[str] = set()

    def visit(name: str) -> None:
        if name in visiting:
            raise ValueError("Cyclic suite prerequisites: " + name)
        visiting.add(name)
        suite = registry[name]
        for dependency in suite.prerequisites:
            if dependency not in registry:
                raise ValueError(f"Missing prerequisite {dependency} for {name}")
            if STAGES.index(registry[dependency].stage) >= STAGES.index(suite.stage):
                raise ValueError(f"Prerequisite {dependency} for {name} must be in an earlier stage")
            selected.add(dependency)
            visit(dependency)
        visiting.remove(name)

    for name in requested:
        visit(name)
    return [suite for suite in SUITES if suite.name in selected], sorted(selected - requested)


def build_plan(suites: list[Suite], configurations: list[str]) -> tuple[list[Job], bool]:
    jobs: list[Job] = []
    audits = dict.fromkeys(audit for suite in suites for audit in suite.audits)
    for audit in audits:
        jobs.append(Job(audit, "audit", f"direct_c/{audit}.py"))
    for stage in STAGES[1:-1]:
        for suite in suites:
            if suite.stage == stage:
                for configuration in configurations:
                    jobs.append(Job(f"{suite.name}-{configuration}", stage,
                                    f"direct_c/verify_{suite.name}.py", configuration,
                                    suite.report(configuration)))
    full = {suite.name for suite in suites} == {suite.name for suite in SUITES} and set(configurations) == set(CONFIGURATIONS)
    if full:
        for script, report in FINALIZERS:
            jobs.append(Job(script, "final", f"direct_c/{script}.py", report=report))
    return jobs, full


def source_snapshot(root: Path, jobs: list[Job]) -> dict[str, str]:
    paths = {root / "CMakeLists.txt", Path(__file__).resolve()}
    paths.update(root / job.script for job in jobs)
    for directory in ("src", "runtime", "include", "generated"):
        paths.update(path for path in (root / directory).rglob("*")
                     if path.is_file() and path.suffix.lower() in (".c", ".h", ".asm", ".inc"))
    # Include imported verification helpers, not their generated bytecode.
    paths.update((root / "direct_c").rglob("*.py"))
    return {str(path.resolve()): digest(path) for path in sorted(paths) if path.is_file()}


def binary_snapshot(root: Path, configurations: list[str]) -> dict[str, str]:
    paths = [root / "build/smoke-game/Pharaoh.exe"]
    for configuration in configurations:
        binary = root / "build/c-only/Release" / CONFIGURATIONS[configuration]
        paths.extend((binary, binary.with_suffix(".map")))
    return {str(path.resolve()): digest(path) for path in paths}


def atomic_json(path: Path, value: dict) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    os.replace(temporary, path)


class CheckpointRunner:
    """Run staged jobs; a failed stage drains its children and starts no later stage."""

    def __init__(self, root: Path, directory: Path, prefix: str, jobs: list[Job],
                 workers: int, full: bool, configurations: list[str]):
        self.root, self.directory, self.prefix = root, directory, prefix
        self.jobs, self.workers, self.full = jobs, workers, full
        self.configurations = configurations
        self.interrupted = False
        self.receipt: dict = {}
        self.receipt_path = directory / "receipt.json"

    def save(self) -> None:
        atomic_json(self.receipt_path, self.receipt)

    def run_job(self, number: int, job: Job) -> dict:
        log = self.directory / f"{self.prefix}-{number:03d}-{job.identifier}.log"
        command = job.command(self.root)
        result = dict(identifier=job.identifier, stage=job.stage, command=command,
                      configuration=job.configuration,
                      started_at=utc_now(), log=str(log.resolve()))
        start = time.monotonic()
        try:
            result["script_sha256"] = digest(self.root / job.script)
            # Isolate child console signals on Windows. Ctrl+C stops scheduling
            # here and drains actual child exits, including a finalizer's build.
            flags = subprocess.CREATE_NEW_PROCESS_GROUP if os.name == "nt" else 0
            with log.open("xb") as output:
                process = subprocess.Popen(command, cwd=self.root.parent, stdout=output,
                                           stderr=subprocess.STDOUT, creationflags=flags,
                                           start_new_session=os.name != "nt")
                result["pid"] = process.pid
                result["exit_code"] = process.wait()
            result["status"] = "passed" if result["exit_code"] == 0 else "failed"
            result["log_sha256"] = digest(log)
            if job.report and result["exit_code"] == 0:
                report = self.root / job.report
                document = json.loads(report.read_text(encoding="utf-8-sig"))
                if job.stage != "final" and document.get("passed") is not True:
                    raise ValueError(f"Verifier output is not passed: {job.report}")
                if job.configuration:
                    binary = self.root / "build/c-only/Release" / CONFIGURATIONS[job.configuration]
                    if Path(document.get("compiled_C_path", "")).resolve() != binary.resolve():
                        raise ValueError(f"Verifier output names another binary path: {job.report}")
                    if document.get("compiled_C_sha256") != digest(binary):
                        raise ValueError(f"Verifier output is bound to another binary: {job.report}")
                    if document.get("native_sha256") != NATIVE_SHA256:
                        raise ValueError(f"Verifier output is bound to another original: {job.report}")
                result["report"] = str(report.resolve())
                result["report_sha256"] = digest(report)
        except Exception as error:
            result.update(status="failed", error=f"{type(error).__name__}: {error}")
            if log.exists():
                result["log_sha256"] = digest(log)
        result.update(finished_at=utc_now(), elapsed_seconds=round(time.monotonic() - start, 3))
        return result

    def run_stage(self, stage: str) -> bool:
        stage_jobs = [(number, job) for number, job in enumerate(self.jobs, 1) if job.stage == stage]
        workers = 1 if stage in ("audit", "final") else self.workers
        failed = False
        waiting = iter(stage_jobs)
        active = {}
        interrupt_announced = False
        with ThreadPoolExecutor(max_workers=workers) as pool:
            def submit() -> None:
                for _ in range(workers - len(active)):
                    if failed or self.interrupted:
                        break
                    try:
                        number, job = next(waiting)
                    except StopIteration:
                        break
                    active[pool.submit(self.run_job, number, job)] = job

            submit()
            while active:
                if self.interrupted and not interrupt_announced:
                    print("Interrupted: no new jobs; waiting for running verifiers to finish.", flush=True)
                    interrupt_announced = True
                try:
                    completed, _ = wait(active, timeout=0.25, return_when=FIRST_COMPLETED)
                except KeyboardInterrupt:
                    self.interrupted = True
                    print("Interrupted: no new jobs; waiting for running verifiers to finish.", flush=True)
                    continue
                for future in completed:
                    job = active.pop(future)
                    result = future.result()
                    self.receipt["results"].append(result)
                    failed |= result["status"] != "passed"
                    print(f"{job.identifier}: {result['status']} (exit {result.get('exit_code', 'not started')}, "
                          f"{result['elapsed_seconds']:.1f}s)", flush=True)
                    self.save()
                submit()
        return not failed and not self.interrupted

    def run(self) -> int:
        self.receipt = dict(schema_version=1, started_at=utc_now(), status="running",
                            root=str(self.root.resolve()), log_directory=str(self.directory.resolve()),
                            full_checkpoint_requested=self.full, complete_checkpoint=False,
                            jobs=self.workers, configurations=self.configurations,
                            plan=[dict(identifier=j.identifier, stage=j.stage, command=j.command(self.root)) for j in self.jobs],
                            results=[], limitations=["Finite native/C verification, not complete gameplay equivalence.",
                            "No stale result is skipped or rebound. Script exits and output digests are recorded.",
                            "The lock coordinates this runner only; manually launched verifiers/builds must already be stopped."])
        code = 1
        previous_interrupt_handler = signal.getsignal(signal.SIGINT)

        def request_stop(signum, frame):
            self.interrupted = True

        signal.signal(signal.SIGINT, request_stop)
        try:
            before = source_snapshot(self.root, self.jobs)
            binaries = binary_snapshot(self.root, self.configurations)
            if binaries[str((self.root / "build/smoke-game/Pharaoh.exe").resolve())] != NATIVE_SHA256:
                raise ValueError("Original executable does not match pinned SHA-256")
            self.receipt.update(source_sha256_before=before, binaries_sha256_before=binaries)
            self.save()
            for stage in STAGES:
                if self.interrupted:
                    code = 130
                    break
                if not self.run_stage(stage):
                    code = 130 if self.interrupted else 1
                    break
                if source_snapshot(self.root, self.jobs) != before:
                    raise ValueError(f"Source files changed during {stage}; later stages were not started")
                if binary_snapshot(self.root, self.configurations) != binaries:
                    raise ValueError(f"Original/main EXE or MAP changed during {stage}")
            else:
                if self.full:
                    aggregate = json.loads((self.root / "metadata/labor-and-access-current.json").read_text(encoding="utf-8-sig"))
                    expected = {suite.report(c) for suite in SUITES if suite.stage == "primary" for c in self.configurations}
                    if not expected.issubset(aggregate.get("proofs", {})):
                        raise ValueError("Aggregate does not include all registered primary report paths")
                code = 0
                self.receipt["complete_checkpoint"] = self.full
        except KeyboardInterrupt:
            self.interrupted = True
            code = 130
        except Exception as error:
            self.receipt["error"] = f"{type(error).__name__}: {error}"
        finally:
            try:
                after = source_snapshot(self.root, self.jobs)
                artifacts = binary_snapshot(self.root, self.configurations)
                self.receipt.update(source_sha256_after=after, binaries_sha256_after=artifacts,
                                    sources_unchanged=after == self.receipt.get("source_sha256_before"),
                                    binaries_unchanged=artifacts == self.receipt.get("binaries_sha256_before"))
                if code == 0 and (not self.receipt["sources_unchanged"] or not self.receipt["binaries_unchanged"]):
                    code = 1
                    self.receipt.update(error="Sources or original/main artifacts changed before receipt completion", complete_checkpoint=False)
            except OSError as error:
                code = 1
                self.receipt.update(snapshot_error=f"{type(error).__name__}: {error}", complete_checkpoint=False)
            if self.interrupted:
                code = 130
            if code != 0:
                self.receipt["complete_checkpoint"] = False
            self.receipt["status"] = "interrupted" if self.interrupted else "passed" if code == 0 else "failed"
            self.receipt["finished_at"] = utc_now()
            completed = {item["identifier"] for item in self.receipt["results"]}
            self.receipt["not_run"] = [job.identifier for job in self.jobs if job.identifier not in completed]
            self.save()
            signal.signal(signal.SIGINT, previous_interrupt_handler)
        return code


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(description=__doc__)
    mode = result.add_mutually_exclusive_group()
    mode.add_argument("--plan", action="store_true", help="print the plan without writing or executing anything (default)")
    mode.add_argument("--run", action="store_true", help="run a new checkpoint and write new logs/receipt")
    result.add_argument("--phase", default="checkpoint", help="new directory name under build/checkpoints")
    result.add_argument("--log-prefix", help="filename prefix; defaults to --phase")
    result.add_argument("--jobs", type=int, default=4, help="maximum parallel oracle jobs; audit/final stages are serial")
    result.add_argument("--suite", action="append", help="family name; repeat or separate with commas; default all")
    result.add_argument("--configuration", action="append", choices=tuple(CONFIGURATIONS), help="main Release variant; repeat; default both")
    result.add_argument("--json", action="store_true", help="print a machine-readable plan")
    result.add_argument("--list-suites", action="store_true", help="print the family registry and exit")
    return result


def main(argv: list[str] | None = None) -> int:
    arguments = parser().parse_args(argv)
    if arguments.list_suites:
        for suite in SUITES:
            print(f"{suite.name:39} {suite.stage:13} prerequisites={','.join(suite.prerequisites) or '-'}")
        return 0
    try:
        for value in (arguments.phase, arguments.log_prefix or arguments.phase):
            if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]{0,79}", value):
                raise ValueError("Phase/log prefix must be 1-80 filename characters, starting with a letter or digit")
        if not 1 <= arguments.jobs <= 32:
            raise ValueError("--jobs must be between 1 and 32")
        names = [name for group in arguments.suite or [] for name in group.split(",") if name]
        if arguments.suite and not names:
            raise ValueError("--suite selection is empty")
        suites, added = select_suites(names or None)
        configurations = list(dict.fromkeys(arguments.configuration or CONFIGURATIONS))
        jobs, full = build_plan(suites, configurations)
        plan = dict(phase=arguments.phase, root=str(ROOT), configurations=configurations,
                    dependencies_added=added, full_checkpoint=full, finalization_included=full,
                    stages=[dict(stage=stage, parallelism=1 if stage in ("audit", "final") else arguments.jobs,
                                 jobs=[dict(identifier=j.identifier, command=j.command(ROOT), report=j.report) for j in jobs if j.stage == stage])
                            for stage in STAGES])
        if not arguments.run:
            if arguments.json:
                print(json.dumps(plan, indent=2))
            else:
                print(f"READ-ONLY PLAN: {len(suites)} families, {len(jobs)} jobs, configurations={','.join(configurations)}")
                print("Full finalization included." if full else "Partial selection: no CMake audit, host binding or final aggregate.")
                if added:
                    print("Added prerequisites: " + ", ".join(added))
                for stage in plan["stages"]:
                    print(f"{stage['stage']}: {len(stage['jobs'])} jobs (parallelism {stage['parallelism']})")
                    for job in stage["jobs"]:
                        print("  " + job["identifier"])
            return 0
        if arguments.json:
            raise ValueError("--json is for read-only plans; execution writes receipt.json")
        for job in jobs:
            if not (ROOT / job.script).is_file():
                raise ValueError("Missing verifier: " + job.script)
        binary_snapshot(ROOT, configurations)
        output = ROOT / "build/checkpoints" / arguments.phase
        if output.exists():
            raise ValueError(f"Phase already exists; choose a new --phase: {output}")
        lock = ROOT / "build/reconstruction-checkpoint.lock"
        lock.parent.mkdir(parents=True, exist_ok=True)
        try:
            with lock.open("x", encoding="utf-8") as stream:
                json.dump(dict(pid=os.getpid(), phase=arguments.phase, started_at=utc_now()), stream)
        except FileExistsError as error:
            raise ValueError(f"Another runner or retained lock exists: {lock}; inspect it before a new run") from error
        try:
            output.mkdir(parents=True, exist_ok=False)
            runner = CheckpointRunner(ROOT, output, arguments.log_prefix or arguments.phase,
                                      jobs, arguments.jobs, full, configurations)
            code = runner.run()
            print(f"Receipt: {runner.receipt_path}")
            return code
        finally:
            # Only this invocation creates/removes this one literal lock file.
            if lock.exists() and json.loads(lock.read_text(encoding="utf-8")).get("pid") == os.getpid():
                lock.unlink()
    except (ValueError, OSError) as error:
        print(f"checkpoint: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
