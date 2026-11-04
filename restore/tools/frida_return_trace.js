'use strict';

// Keep batches deliberately small: tracing hundreds of hot functions at once can
// perturb timing enough to hide the original behaviour we are trying to observe.
const targets = [
  { name: 'FUN_004ded50', va: 0x004ded50 },
  { name: 'FUN_004e0c50', va: 0x004e0c50 },
  { name: 'FUN_004e0cc0', va: 0x004e0cc0 },
  { name: 'FUN_004e1600', va: 0x004e1600 },
  { name: 'FUN_004e1750', va: 0x004e1750 },
  { name: 'FUN_004e1850', va: 0x004e1850 },
  { name: 'FUN_004e7db0', va: 0x004e7db0 },
  { name: 'FUN_004e7e10', va: 0x004e7e10 },
  { name: 'FUN_004e7e20', va: 0x004e7e20 },
  { name: 'FUN_004e85d0', va: 0x004e85d0 },
  { name: 'FUN_004e87a0', va: 0x004e87a0 },
  { name: 'FUN_004e8e80', va: 0x004e8e80 },
  { name: 'FUN_004e92c0', va: 0x004e92c0 },
  { name: 'FUN_004e92f0', va: 0x004e92f0 },
  { name: 'FUN_004e94a0', va: 0x004e94a0 },
  { name: 'FUN_004e94d0', va: 0x004e94d0 },
  { name: 'FUN_004e9500', va: 0x004e9500 },
  { name: 'FUN_004e96c0', va: 0x004e96c0 },
  { name: 'FUN_004e9700', va: 0x004e9700 },
  { name: 'FUN_004e9760', va: 0x004e9760 },
  { name: 'FUN_004e9ee0', va: 0x004e9ee0 },
  { name: 'FUN_004ea280', va: 0x004ea280 },
  { name: 'FUN_004ea740', va: 0x004ea740 },
  { name: 'FUN_004ea930', va: 0x004ea930 },
  { name: 'FUN_004ea9a0', va: 0x004ea9a0 },
  { name: 'FUN_004ea9c0', va: 0x004ea9c0 },
  { name: 'FUN_004eada0', va: 0x004eada0 },
  { name: 'FUN_004f1cb0', va: 0x004f1cb0 },
  { name: 'FUN_004f2190', va: 0x004f2190 },
  { name: 'FUN_004f26a0', va: 0x004f26a0 },
  { name: 'FUN_004f3360', va: 0x004f3360 },
  { name: 'FUN_004f4a60', va: 0x004f4a60 },
  { name: 'FUN_004f4a80', va: 0x004f4a80 },
  { name: 'FUN_004f4aa0', va: 0x004f4aa0 },
  { name: 'FUN_004f5020', va: 0x004f5020 },
  { name: 'FUN_004f56a0', va: 0x004f56a0 },
  { name: 'FUN_004f5770', va: 0x004f5770 },
  { name: 'FUN_004f6000', va: 0x004f6000 },
  { name: 'FUN_005013a0', va: 0x005013a0 },
  { name: 'FUN_0050fc70', va: 0x0050fc70 },
  { name: 'FUN_005150a0', va: 0x005150a0 },
  { name: 'FUN_005165b0', va: 0x005165b0 },
  { name: 'FUN_005169c0', va: 0x005169c0 },
  { name: 'FUN_00516a20', va: 0x00516a20 },
  { name: 'FUN_00517a20', va: 0x00517a20 },
  { name: 'FUN_0051afa0', va: 0x0051afa0 },
  { name: 'FUN_0051aff0', va: 0x0051aff0 },
  { name: 'FUN_0051c5f0', va: 0x0051c5f0 },
  { name: 'FUN_0051c620', va: 0x0051c620 },
  { name: 'FUN_0051cba0', va: 0x0051cba0 },
  { name: 'FUN_0051f780', va: 0x0051f780 },
  { name: 'FUN_00527ad0', va: 0x00527ad0 },
  { name: 'FUN_0053b830', va: 0x0053b830 },
  { name: 'FUN_00542110', va: 0x00542110 },
  { name: 'FUN_00542f40', va: 0x00542f40 },
  { name: 'FUN_00545f60', va: 0x00545f60 },
  { name: 'FUN_0054fed0', va: 0x0054fed0 },
  { name: 'FUN_005567b0', va: 0x005567b0 },
  { name: 'FUN_00559460', va: 0x00559460 },
  { name: 'FUN_00559c60', va: 0x00559c60 },
  { name: 'FUN_0055c0f0', va: 0x0055c0f0 },
  { name: 'FUN_0055c650', va: 0x0055c650 },
  { name: 'FUN_0055ca20', va: 0x0055ca20 },
  { name: 'FUN_0055cb10', va: 0x0055cb10 },
  { name: 'FUN_0055d110', va: 0x0055d110 },
];

const preferredImageBase = 0x00400000;
const maxSamplesPerFunction = 4;

// A Frida-spawned process inherits the analysis service's working directory.
// Set the game's directory before its first instruction so relative data-file
// lookups behave exactly like a normal launch from the smoke-game folder.
const gameDirectory = 'C:\\Users\\Daniel\\Documents\\ChatGPT\\game decompile and rewrite\\restore\\build\\smoke-game';
const kernel32 = Process.getModuleByName('kernel32.dll');
const setCurrentDirectoryW = new NativeFunction(
  kernel32.getExportByName('SetCurrentDirectoryW'),
  'bool',
  ['pointer']
);
const directoryBuffer = Memory.allocUtf16String(gameDirectory);
if (!setCurrentDirectoryW(directoryBuffer)) {
  throw new Error('SetCurrentDirectoryW failed for ' + gameDirectory);
}

const modules = Process.enumerateModules();
const game = modules.find(m => m.name.toLowerCase() === 'pharaoh.exe') || {
  name: 'mapped-original-image',
  base: ptr(preferredImageBase),
};

function hex32(value) {
  return '0x' + (value >>> 0).toString(16).padStart(8, '0');
}

function readStackWords(context, count) {
  const words = [];
  try {
    const stack = context.esp.add(4);
    for (let i = 0; i < count; ++i) {
      words.push(hex32(stack.add(i * 4).readU32()));
    }
  } catch (error) {
    words.push('<unreadable>');
  }
  return words;
}

for (const target of targets) {
  const address = game.base.add(target.va - preferredImageBase);
  let samples = 0;

  try {
    Interceptor.attach(address, {
    onEnter() {
      this.traceEnabled = samples < maxSamplesPerFunction;
      if (!this.traceEnabled) {
        return;
      }

      ++samples;
      this.sequence = samples;
      this.startedAt = Date.now();
      this.stackWords = readStackWords(this.context, 8);
      this.caller = this.returnAddress.toString();
      this.entryEax = hex32(this.context.eax.toUInt32());
      this.entryEcx = hex32(this.context.ecx.toUInt32());
      this.entryEdx = hex32(this.context.edx.toUInt32());
    },

    onLeave(retval) {
      if (!this.traceEnabled) {
        return;
      }

      send({
        kind: 'function-return',
        function: target.name,
        address: address.toString(),
        sequence: this.sequence,
        threadId: this.threadId,
        caller: this.caller,
        elapsedMs: Date.now() - this.startedAt,
        entry: {
          eax: this.entryEax,
          ecx: this.entryEcx,
          edx: this.entryEdx,
          stack: this.stackWords,
        },
        result: {
          eax: hex32(retval.toUInt32()),
          edx: hex32(this.context.edx.toUInt32()),
        },
      });
    },
    });

    send({
      kind: 'hook-installed',
      function: target.name,
      preferredAddress: hex32(target.va),
      runtimeAddress: address.toString(),
    });
  } catch (error) {
    send({
      kind: 'hook-failed',
      function: target.name,
      preferredAddress: hex32(target.va),
      error: String(error),
    });
  }
}

send({
  kind: 'trace-ready',
  module: game.name,
  base: game.base.toString(),
  functions: targets.length,
  maxSamplesPerFunction,
});
