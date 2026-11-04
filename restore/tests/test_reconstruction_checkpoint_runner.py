"""Isolated review of the checkpoint runner; never execute real game verifiers.

Run from any working directory with Python3.10+. Every run retains a fresh
sandbox under build/checkpoint-runner-review/. No file or cache cleanup is performed.
"""
from __future__ import annotations

import contextlib
from datetime import datetime, timezone
import importlib.util
import io
import json
from pathlib import Path
import signal
import subprocess
import sys
import threading
import time
import unittest
import uuid

sys.dont_write_bytecode = True
RESTORE_ROOT = Path(__file__).resolve().parents[1]
REVIEW_ROOT = RESTORE_ROOT / "build/checkpoint-runner-review"
RUNNER_PATH = RESTORE_ROOT / "tools/run_reconstruction_checkpoint.py"
SPEC = importlib.util.spec_from_file_location("checkpoint_runner_under_review", RUNNER_PATH)
RUNNER = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = RUNNER
SPEC.loader.exec_module(RUNNER)
ORIGINAL_REGISTRY = RUNNER.SUITES
ORIGINAL_NATIVE_SHA = RUNNER.NATIVE_SHA256
RUN_ROOT = REVIEW_ROOT / (datetime.now(timezone.utc).strftime("run-%Y%m%dT%H%M%SZ-") + uuid.uuid4().hex[:8])


class Review(unittest.TestCase):
    def setUp(self):
        RUNNER.SUITES = ORIGINAL_REGISTRY
        RUNNER.NATIVE_SHA256 = ORIGINAL_NATIVE_SHA
        self.root = RUN_ROOT / self._testMethodName
        self.root.mkdir(parents=True, exist_ok=False)

    def tearDown(self):
        RUNNER.SUITES = ORIGINAL_REGISTRY
        RUNNER.NATIVE_SHA256 = ORIGINAL_NATIVE_SHA

    def fixture(self):
        for directory in ("direct_c", "metadata", "runtime", "include", "src", "generated",
                          "build/smoke-game", "build/c-only/Release", "logs"):
            (self.root / directory).mkdir(parents=True, exist_ok=True)
        (self.root / "CMakeLists.txt").write_text("# synthetic fixture, no build\n", encoding="utf-8")
        (self.root / "runtime/state.c").write_text("int state;\n", encoding="utf-8")
        native = self.root / "build/smoke-game/Pharaoh.exe"
        native.write_bytes(b"synthetic native input; not an executable")
        RUNNER.NATIVE_SHA256 = RUNNER.digest(native)
        for name in RUNNER.CONFIGURATIONS.values():
            binary = self.root / "build/c-only/Release" / name
            binary.write_bytes(b"synthetic C input; not an executable")
            binary.with_suffix(".map").write_text("synthetic MAP\n", encoding="utf-8")

    def script(self, name, body):
        path = self.root / "direct_c" / (name + ".py")
        path.write_text("from pathlib import Path\nimport hashlib,json,sys,time\n"
                        "r=Path(__file__).resolve().parents[1]\n" + body, encoding="utf-8")
        return "direct_c/" + name + ".py"

    def report_body(self, stem, wrong_sha=False, wrong_path=False):
        return ("exe=Path(sys.argv[1])\n"
                "c='diagnostic' if exe.stem=='restore_link_check' else 'game'\n"
                f"out=r/'metadata'/('{stem}-'+c+'.json')\n"
                "out.write_text(json.dumps(dict(passed=True,"
                "native_sha256=hashlib.sha256((r/'build/smoke-game/Pharaoh.exe').read_bytes()).hexdigest(),"
                "compiled_C_path=" + ("str(r/'wrong.exe')" if wrong_path else "str(exe)") + ","
                "compiled_C_sha256=" + ("'0'*64" if wrong_sha else "hashlib.sha256(exe.read_bytes()).hexdigest()") + ")))\n")

    def execute(self, jobs, full=False, workers=2, configurations=None):
        runner = RUNNER.CheckpointRunner(self.root, self.root / "logs", "fake", jobs,
                                         workers, full, configurations or ["diagnostic", "game"])
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            code = runner.run()
        (self.root / "runner-stdout.log").write_text(output.getvalue(), encoding="utf-8")
        receipt = json.loads(runner.receipt_path.read_text(encoding="utf-8"))
        return code, receipt

    def test_registry_and_plan(self):
        suites, _ = RUNNER.select_suites(None)
        jobs, full = RUNNER.build_plan(suites, list(RUNNER.CONFIGURATIONS))
        self.assertTrue(full)
        self.assertEqual(len(suites), 39)
        self.assertEqual(len(jobs), 88)
        self.assertEqual(len({job.identifier for job in jobs}), len(jobs))
        self.assertEqual(sum(job.stage == "primary" for job in jobs), 62)
        self.assertEqual(sum(job.stage == "supplemental" for job in jobs), 10)
        self.assertEqual(sum(job.stage == "composition" for job in jobs), 6)
        self.assertEqual([job.identifier for job in jobs if job.stage == "final"],
                         ["verify_music_cmake_scope", "verify_music_configuration_host", "verify_labor_access_current"])
        self.assertTrue(all((RESTORE_ROOT / job.script).is_file() for job in jobs))
        queue = next(suite for suite in suites if suite.name == "actor_ring_queue")
        self.assertEqual(queue.report("game"), "metadata/actor-ring-queue-reconstruction.json")
        selected, added = RUNNER.select_suites(["actor_command_parent"])
        self.assertEqual({suite.name for suite in selected},
                         {"actor_command_parent", "actor_command_transition", "actor_spawn_hippos"})
        self.assertEqual(set(added), {"actor_command_transition", "actor_spawn_hippos"})
        jobs, full = RUNNER.build_plan(selected, ["game"])
        self.assertFalse(full)
        self.assertFalse(any(job.stage == "final" for job in jobs))

    def test_same_stage_prerequisite_rejected(self):
        RUNNER.SUITES = (RUNNER.Suite("first"),
                         RUNNER.Suite("second", prerequisites=("first",)))
        with self.assertRaisesRegex(ValueError, "must be in an earlier stage"):
            RUNNER.select_suites(["second"])

    def test_later_stage_prerequisite_rejected(self):
        RUNNER.SUITES = (RUNNER.Suite("first", prerequisites=("later",)),
                         RUNNER.Suite("later", "supplemental"))
        with self.assertRaisesRegex(ValueError, "must be in an earlier stage"):
            RUNNER.select_suites(["first"])

    def test_cli_plans_and_rejections(self):
        phase = "review-read-only-" + uuid.uuid4().hex[:10]
        output = RESTORE_ROOT / "build/checkpoints" / phase
        self.assertFalse(output.exists())
        for arguments, expected in ((["--phase", phase, "--plan", "--json"], 0),
                                    (["--suite", "not_a_family", "--plan"], 2),
                                    (["--phase", "../escape", "--plan"], 2),
                                    (["--suite", ",", "--plan"], 2),
                                    (["--jobs", "0", "--plan"], 2)):
            completed = subprocess.run([sys.executable, str(RUNNER_PATH), *arguments],
                                       capture_output=True, text=True)
            self.assertEqual(completed.returncode, expected, (arguments, completed.stderr))
            if expected == 0:
                plan = json.loads(completed.stdout)
                self.assertTrue(plan["full_checkpoint"])
                self.assertEqual(len(plan["stages"][1]["jobs"]), 62)
        self.assertFalse(output.exists())

    def test_full_fake_pipeline_and_hash_receipt(self):
        self.fixture()
        RUNNER.SUITES = (RUNNER.Suite("sample"),)
        jobs = [RUNNER.Job("audit", "audit", self.script("audit", "(r/'build/audit.done').write_text('yes')\n"))]
        for configuration in ("diagnostic", "game"):
            body = ("assert (r/'build/audit.done').exists()\ntime.sleep(.03)\n" + self.report_body("sample") +
                    f"(r/'build/primary-{configuration}.done').write_text('yes')\n")
            jobs.append(RUNNER.Job("sample-" + configuration, "primary", self.script("primary_" + configuration, body),
                                   configuration, "metadata/sample-" + configuration + ".json"))
        body = ("assert (r/'build/primary-game.done').exists() and (r/'build/primary-diagnostic.done').exists()\n" +
                self.report_body("supp") + "(r/'build/supp.done').write_text('yes')\n")
        jobs.append(RUNNER.Job("supp-game", "supplemental", self.script("supp", body), "game", "metadata/supp-game.json"))
        body = ("assert (r/'build/supp.done').exists()\n" + self.report_body("comp") +
                "(r/'build/comp.done').write_text('yes')\n")
        jobs.append(RUNNER.Job("comp-game", "composition", self.script("comp", body), "game", "metadata/comp-game.json"))
        for current, previous in (("scope", "comp"), ("host", "scope"), ("aggregate", "host")):
            body = f"assert (r/'build/{previous}.done').exists()\n(r/'build/{current}.done').write_text('yes')\n"
            if current == "aggregate":
                body += ("(r/'metadata/labor-and-access-current.json').write_text(json.dumps(dict(proofs={"
                         "'metadata/sample-game.json':{},'metadata/sample-diagnostic.json':{}})))\n")
            jobs.append(RUNNER.Job(current, "final", self.script(current, body)))
        code, receipt = self.execute(jobs, full=True)
        self.assertEqual(code, 0)
        self.assertEqual(receipt["status"], "passed")
        self.assertTrue(receipt["complete_checkpoint"])
        self.assertTrue(receipt["sources_unchanged"])
        self.assertTrue(receipt["binaries_unchanged"])
        self.assertFalse(receipt["not_run"])
        self.assertEqual([item["stage"] for item in receipt["results"]],
                         ["audit", "primary", "primary", "supplemental", "composition", "final", "final", "final"])
        for item in receipt["results"]:
            self.assertEqual(item["exit_code"], 0)
            self.assertEqual(item["log_sha256"], RUNNER.digest(Path(item["log"])))
            self.assertEqual(item["script_sha256"], RUNNER.digest(Path(item["command"][1])))
            if "report" in item:
                self.assertEqual(item["report_sha256"], RUNNER.digest(Path(item["report"])))

    def test_failure_drains_and_skips_queued_jobs(self):
        self.fixture()
        fail = self.script("fail", "deadline=time.monotonic()+5\n"
                           "while not (r/'build/slow.started').exists() and time.monotonic()<deadline:time.sleep(.005)\n"
                           "print('synthetic failure',file=sys.stderr)\nraise SystemExit(7)\n")
        slow = self.script("slow", "(r/'build/slow.started').write_text('yes')\ntime.sleep(.15)\n"
                           "(r/'build/slow.done').write_text('yes')\n")
        never = self.script("never", "(r/'build/never.done').write_text('bad')\n")
        jobs = [RUNNER.Job("fail", "primary", fail), RUNNER.Job("slow", "primary", slow),
                RUNNER.Job("queued", "primary", never), RUNNER.Job("final", "final", never)]
        code, receipt = self.execute(jobs)
        self.assertEqual(code, 1)
        self.assertEqual(receipt["status"], "failed")
        self.assertFalse(receipt["complete_checkpoint"])
        self.assertTrue((self.root / "build/slow.done").exists())
        self.assertFalse((self.root / "build/never.done").exists())
        self.assertEqual(set(receipt["not_run"]), {"queued", "final"})
        self.assertEqual(next(item for item in receipt["results"] if item["identifier"] == "fail")["exit_code"], 7)

    def check_bad_report(self, wrong_sha=False, wrong_path=False):
        self.fixture()
        bad = self.script("bad", self.report_body("bad", wrong_sha, wrong_path))
        never = self.script("never", "(r/'build/never.done').write_text('bad')\n")
        code, receipt = self.execute([RUNNER.Job("bad-game", "primary", bad, "game", "metadata/bad-game.json"),
                                      RUNNER.Job("final", "final", never)])
        self.assertEqual(code, 1)
        self.assertEqual(receipt["results"][0]["exit_code"], 0)
        self.assertEqual(receipt["results"][0]["status"], "failed")
        self.assertIn("another binary", receipt["results"][0]["error"])
        self.assertFalse((self.root / "build/never.done").exists())

    def test_incorrect_binary_sha_rejected(self):
        self.check_bad_report(wrong_sha=True)

    def test_incorrect_binary_path_rejected(self):
        self.check_bad_report(wrong_path=True)

    def test_changed_source_stops_later_stages(self):
        self.fixture()
        mutate = self.script("mutate", "(r/'runtime/state.c').write_text('changed source\\n')\n")
        never = self.script("never", "(r/'build/never.done').write_text('bad')\n")
        code, receipt = self.execute([RUNNER.Job("mutate", "primary", mutate), RUNNER.Job("final", "final", never)])
        self.assertEqual(code, 1)
        self.assertFalse(receipt["sources_unchanged"])
        self.assertTrue(receipt["binaries_unchanged"])
        self.assertIn("Source files changed", receipt["error"])
        self.assertFalse((self.root / "build/never.done").exists())

    def test_interrupt_drains_and_returns130(self):
        self.fixture()
        slow = self.script("slow", "(r/'build/started').write_text('yes')\ntime.sleep(.2)\n"
                           "(r/'build/drained').write_text('yes')\n")
        never = self.script("never", "(r/'build/never.done').write_text('bad')\n")
        signalled = []

        def request_interrupt():
            deadline = time.monotonic() + 5
            while not (self.root / "build/started").exists() and time.monotonic() < deadline:
                time.sleep(.005)
            signal.raise_signal(signal.SIGINT)
            signalled.append(True)

        timer = threading.Thread(target=request_interrupt, daemon=True)
        timer.start()
        code, receipt = self.execute([RUNNER.Job("slow", "primary", slow), RUNNER.Job("queued", "primary", never),
                                      RUNNER.Job("final", "final", never)], workers=1)
        timer.join(1)
        self.assertTrue(signalled)
        self.assertEqual(code, 130)
        self.assertEqual(receipt["status"], "interrupted")
        self.assertFalse(receipt["complete_checkpoint"])
        self.assertTrue((self.root / "build/drained").exists())
        self.assertFalse((self.root / "build/never.done").exists())
        self.assertEqual(set(receipt["not_run"]), {"queued", "final"})


if __name__ == "__main__":
    RUN_ROOT.mkdir(parents=True, exist_ok=False)
    print("Retained isolated review artifacts: " + str(RUN_ROOT), flush=True)
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(Review)
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    outcome = dict(passed=result.wasSuccessful(), tests_run=result.testsRun,
                   errors=len(result.errors), failures=len(result.failures),
                   runner_path=str(RUNNER_PATH), runner_sha256=RUNNER.digest(RUNNER_PATH),
                   real_verifiers_executed=False, real_build_executed=False, host_game_launched=False,
                   cleanup_performed=False)
    (RUN_ROOT / "harness-result.json").write_text(json.dumps(outcome, indent=2) + "\n", encoding="utf-8")
    raise SystemExit(0 if result.wasSuccessful() else 1)
