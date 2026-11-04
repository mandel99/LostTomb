'use strict';

// Loaded by REA into the original game. Each original call still supplies the
// gameplay result. C is checked on the same inputs, and original writes restored.
const config=validationConfig;
const original=Process.getModuleByName('Pharaoh.exe');
if (!original.base.equals(ptr(0x00400000))) throw new Error('Unexpected original image base');
const setDirectory=new NativeFunction(Process.getModuleByName('kernel32.dll').getExportByName('SetCurrentDirectoryW'),'bool',['pointer']);
if (!setDirectory(Memory.allocUtf16String(config.gameDirectory))) throw new Error('Cannot set game data directory');
const runtime=Module.load(config.runtimePath);
const counters={};
function equalBytes(left,right) {
 const a=new Uint8Array(left),b=new Uint8Array(right);
 if(a.length!==b.length) return false;
 for(let i=0;i<a.length;++i) if(a[i]!==b[i]) return false;
 return true;
}
for(const target of config.targets) {
 const cFunction=new NativeFunction(runtime.base.add(target.rva),target.mode==='scalar'||target.mode==='pointer'?'uint32':'void',Array(target.argc).fill('int'),'mscdecl');
 const stats=counters[target.name]={samples:0,passed:0,failed:0,errors:0};
 Interceptor.attach(ptr(target.address),{
  onEnter(args) {
   this.check=stats.samples<config.maxSamples;
   if(!this.check) return;
   ++stats.samples;
   this.values=Array.from({length:target.argc},(_,i)=>args[i].toInt32());
   this.caller=this.returnAddress.toString();
   this.memory=null;
   if(target.mode==='pointer') {this.memory=args[0];this.size=target.start;}
   if(target.mode==='global') {this.memory=ptr(target.start);this.size=target.size;}
   if(target.mode==='city') {this.memory=ptr(0x00ea0000).add(ptr(0x00eaac28).readU32()*0x49d8);this.size=0x49d8;}
   if(this.memory) this.before=this.memory.readByteArray(this.size);
  },
  onLeave(retval) {
   if(!this.check) return;
   let expectedMemory=null,actual,pass=false;
   const expected=retval.toUInt32();
   try {
    if(this.memory) {
     expectedMemory=this.memory.readByteArray(this.size);
     this.memory.writeByteArray(this.before);
    }
    actual=cFunction(...this.values);
    pass=target.mode==='scalar'||target.mode==='pointer' ? actual===expected : true;
    if(this.memory) pass=pass && equalBytes(this.memory.readByteArray(this.size),expectedMemory);
    ++stats[pass?'passed':'failed'];
    if(!pass || stats.samples===1 || stats.samples===4 || stats.samples===config.maxSamples) {
     send({kind:'live-c-comparison',function:target.name,pass,caller:this.caller,arguments:this.values,originalEax:expected,cEax:actual,stats:{...stats}});
    }
   } catch(error) {
    ++stats.errors;
    send({kind:'live-c-error',function:target.name,error:String(error),stats:{...stats}});
   } finally {
    if(expectedMemory) this.memory.writeByteArray(expectedMemory);
   }
  },
 });
}
send({kind:'live-c-ready',runtime:runtime.path,runtimeSha256:config.runtimeSha256,targets:config.targets.length,maxSamples:config.maxSamples});
setInterval(()=>send({kind:'live-c-totals',counters}),15000);
