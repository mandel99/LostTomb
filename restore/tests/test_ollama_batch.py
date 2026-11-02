"""Batch state and fragment boundaries; no model/server/compiler required."""
import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location('ollama_batch', Path(__file__).resolve().parents[1] / 'tools/ollama_batch.py')
batch = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(batch)


class FragmentTests(unittest.TestCase):
    def test_small_fragments_reassemble_every_original_character(self):
        source = '#include "x.h"\nint f(void)\n{\n' + ''.join(f'  value += {i};\n' for i in range(300)) + 'x' * 7000 + ';\n  return value;\n}\n'
        chunks = batch.split_body(source, 2500, 40)
        self.assertEqual(batch.assemble(source, chunks, {}), source)
        self.assertGreater(len(chunks), 10)
        for chunk in chunks:
            fragment = source[chunk['start']:chunk['end']]
            self.assertLessEqual(len(fragment), 2500)
            self.assertLessEqual(len(fragment.splitlines()), 40)
            self.assertEqual(batch.sha(fragment.encode()), chunk['sha256'])
        for a, b in zip(chunks, chunks[1:]):
            self.assertEqual(a['end'], b['start'])

    def test_replacements_preserve_preamble_and_unselected_fragments(self):
        source = 'int f(int x)\n{\n  x += 1;\n  return x;\n}\n'
        chunks = batch.split_body(source, 100, 1)
        actual = batch.assemble(source, chunks, {'0': '  x += 2;\n'})
        self.assertEqual(actual, source.replace('x += 1', 'x += 2'))

    def test_reject_new_calls_directives_and_oversized_answers(self):
        for replacement in ['return system("x");', '#include <stdlib.h>\nreturn 1;', '__asm { int 3 }', 'x' * 5001]:
            with self.assertRaises(ValueError):
                batch.response_check('return 1;', replacement, 2500)
        batch.response_check('return x ? 1 : 0;', 'return x != 0;', 2500)
        with self.assertRaises(ValueError):
            batch.response_check('x = (f(), restore_unresolved_return(0x401000u));', 'x = f();', 2500)

    def test_compile_failure_is_retained_and_original_is_not_changed(self):
        source = 'int f(int x)\n{\n  return x;\n}\n'
        class Compiler:
            def compile(self, *args):
                return {'ok': 'bad syntax' not in args[0].read_text()}
            def verify(self, *args):
                return {'status': 'unavailable'}
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp); directory = root / 'units/FUN_00000001'; directory.mkdir(parents=True)
            (root / 'original-unit.c').write_bytes(source.encode())
            (directory / 'original.c').write_text(source)
            unit = dict(name='FUN_00000001', file='original-unit.c', source_sha256=batch.sha(source.encode()),
                snapshot_sha256=batch.sha(source.encode()), signature='int f(int x)', unresolved={},
                chunks=batch.split_body(source, 2500, 40), behavior_oracle=None)
            plan = dict(model='test', endpoint='http://127.0.0.1:11434', num_ctx=8192, num_predict=2048, max_chars=2500)
            answer = {'done': True, 'done_reason': 'stop', 'message': {'content': '{"replacement":"bad syntax;","notes":"test"}'}}
            with patch.object(batch, 'ROOT', root), patch.object(batch, 'post', return_value=answer) as post:
                batch.process_unit(root, plan, unit, Compiler())
                result = batch.read_json(directory / 'result.json')
                self.assertEqual(result['status'], 'needs_codex')
                self.assertEqual(result['chunks']['0']['status'], 'compile_failed')
                self.assertEqual((directory / 'candidate.c').read_text(), source)
                self.assertEqual((root / 'original-unit.c').read_text(), source)
                batch.process_unit(root, plan, unit, Compiler())
                self.assertEqual(post.call_count, 1)  # No automatic model repair loop.

    def test_behavior_failure_and_transport_retry_never_become_verified(self):
        source = 'int f(int x)\n{\n  return x;\n}\n'
        class Compiler:
            def compile(self, *args): return {'ok': True}
            def verify(self, *args): return {'status': 'failed'}
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp); directory = root / 'units/FUN_00000001'; directory.mkdir(parents=True)
            (root / 'original-unit.c').write_bytes(source.encode())
            (directory / 'original.c').write_text(source)
            unit = dict(name='FUN_00000001', file='original-unit.c', source_sha256=batch.sha(source.encode()),
                snapshot_sha256=batch.sha(source.encode()), signature='int f(int x)', unresolved={},
                chunks=batch.split_body(source, 2500, 40), behavior_oracle='test')
            plan = dict(model='test', endpoint='http://127.0.0.1:11434', num_ctx=8192, num_predict=2048, max_chars=2500)
            answer = {'done': True, 'done_reason': 'stop', 'message': {'content': '{"replacement":"  return 0;","notes":"test"}'}}
            with patch.object(batch, 'ROOT', root), patch.object(batch, 'post', side_effect=[OSError('disconnected'), answer]) as post:
                with self.assertRaises(RuntimeError):
                    batch.process_unit(root, plan, unit, Compiler())
                self.assertEqual(batch.read_json(directory / 'result.json')['chunks']['0']['status'], 'request_failed')
                batch.process_unit(root, plan, unit, Compiler())
                result = batch.read_json(directory / 'result.json')
                self.assertEqual(result['status'], 'needs_codex')
                self.assertEqual(result['verification']['status'], 'failed')
                self.assertEqual(post.call_count, 2)
                self.assertEqual((root / 'original-unit.c').read_text(), source)


if __name__ == '__main__':
    unittest.main()
