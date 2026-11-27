import com.google.gson.*;
import com.sun.net.httpserver.*;
import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.util.importer.*;
import ghidra.app.util.opinion.LoadResults;
import ghidra.program.model.address.*;
import ghidra.program.model.lang.*;
import ghidra.program.model.listing.Program;
import ghidra.util.task.TaskMonitor;
import java.io.*;
import java.lang.instrument.Instrumentation;
import java.math.BigInteger;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;
import java.util.concurrent.*;

/** Temporary cohort diagnostic hook. Executes only Ghidra p-code in isolated
 * emulator state, never writes the live program or closes/restarts Ghidra. */
public final class LostTombNativeComparisonAgent {
    static final Gson G = new GsonBuilder().serializeNulls().create();
    static final String PIN="fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d";
    static final Object CONSUMER=new Object();
    static LoadResults<Program> loaded;
    static Program program;
    static HttpServer server;
    static Path folder;
    static String token;
    static String hash(byte[] b)throws Exception{return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(b));}
    static long number(JsonElement e){return e.isJsonPrimitive()&&e.getAsJsonPrimitive().isString()?Long.parseUnsignedLong(e.getAsString().replaceFirst("^0[xX]",""),16):e.getAsLong();}
    static Address addr(long n){return program.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
    static JsonObject error(Throwable t){JsonObject o=new JsonObject();o.addProperty("ok",false);o.addProperty("error",t.toString());StringWriter w=new StringWriter();t.printStackTrace(new PrintWriter(w));o.addProperty("stack",w.toString());return o;}
    public static void agentmain(String directory,Instrumentation ignored){
        folder=Path.of(directory);
        Thread t=new Thread(()->{
            try{
                Path path=Path.of("C:/Program Files (x86)/GOG.com/Pharaoh/Pharaoh.exe");
                String observed=hash(Files.readAllBytes(path));if(!PIN.equals(observed))throw new IllegalStateException("Original SHA-256 differs");
                loaded=AutoImporter.importByUsingBestGuess(path.toFile(),null,null,CONSUMER,new MessageLog(),TaskMonitor.DUMMY);
                program=loaded.getPrimaryDomainObject();
                if(program.getDefaultPointerSize()!=4||!program.getLanguageID().toString().equals("x86:LE:32:default"))throw new IllegalStateException("Original language differs");
                token=UUID.randomUUID().toString();server=HttpServer.create(new InetSocketAddress("127.0.0.1",0),0);
                server.setExecutor(Executors.newSingleThreadExecutor(r->{Thread v=new Thread(r,"LostTomb-pcode");v.setDaemon(true);return v;}));
                server.createContext("/",ex->{
                    JsonObject out;
                    try{
                        if(!token.equals(ex.getRequestHeaders().getFirst("X-LostTomb-Token")))throw new SecurityException("Wrong local diagnostic token");
                        if(ex.getRequestURI().getPath().equals("/info")){
                            out=new JsonObject();out.addProperty("ok",true);out.addProperty("sha256",PIN);out.addProperty("language",program.getLanguageID().toString());out.addProperty("isolated_emulator",true);
                        }else if(ex.getRequestURI().getPath().equals("/emulate")){
                            String request=new String(ex.getRequestBody().readAllBytes(),StandardCharsets.UTF_8);out=emulate(JsonParser.parseString(request).getAsJsonObject());
                        }else if(ex.getRequestURI().getPath().equals("/stop")){
                            out=new JsonObject();out.addProperty("ok",true);new Thread(()->{try{Thread.sleep(200);server.stop(0);loaded.release(CONSUMER);}catch(Exception e){}}).start();
                        }else throw new IllegalArgumentException("Unknown diagnostic operation");
                    }catch(Throwable e){out=error(e);}
                    byte[] bytes=G.toJson(out).getBytes(StandardCharsets.UTF_8);ex.getResponseHeaders().set("Content-Type","application/json");ex.sendResponseHeaders(200,bytes.length);try(OutputStream s=ex.getResponseBody()){s.write(bytes);}
                });
                server.start();JsonObject ready=new JsonObject();ready.addProperty("ok",true);ready.addProperty("port",server.getAddress().getPort());ready.addProperty("token",token);ready.addProperty("sha256",PIN);ready.addProperty("source","Ghidra p-code in existing JVM; transient pinned import; live session untouched");Files.writeString(folder.resolve("ready.json"),G.toJson(ready));
            }catch(Throwable e){try{Files.writeString(folder.resolve("ready.json"),G.toJson(error(e)));}catch(Exception ignored2){}}
        },"LostTomb-Ghidra-comparison-start");t.setDaemon(true);t.start();
    }
    static synchronized JsonObject emulate(JsonObject request)throws Exception{
        EmulatorHelper h=new EmulatorHelper(program);
        try{
            long total=0;
            if(request.has("writes"))for(JsonElement v:request.getAsJsonArray("writes")){
                JsonObject w=v.getAsJsonObject();byte[] b=w.has("hex")?HexFormat.of().parseHex(w.get("hex").getAsString()):Base64.getDecoder().decode(w.get("base64").getAsString());total+=b.length;if(total>32L*1024*1024)throw new IllegalArgumentException("Fixture exceeds memory budget");h.writeMemory(addr(number(w.get("address"))),b);
            }
            if(request.has("registers"))for(Map.Entry<String,JsonElement> r:request.getAsJsonObject("registers").entrySet())h.writeRegister(r.getKey(),new BigInteger(Long.toUnsignedString(number(r.getValue()))));
            long entry=number(request.get("entry")),stop=number(request.get("stop"));
            Register base=program.getProgramContext().getBaseContextRegister();
            if(base!=null){RegisterValue context=program.getProgramContext().getRegisterValue(base,addr(entry));if(context!=null)h.setContextRegister(context);}
            h.getEmulator().setExecuteAddress(entry);
            int max=request.has("max_steps")?Math.min(request.get("max_steps").getAsInt(),1000000):200000;
            long until=System.nanoTime()+Math.min(request.has("timeout_ms")?request.get("timeout_ms").getAsLong():30000,60000)*1000000L;
            JsonArray trace=new JsonArray();int steps=0;boolean stopped=false;
            for(;steps<max;++steps){
                long pc=h.getExecutionAddress().getOffset();if(pc==stop){stopped=true;break;}if(System.nanoTime()>until)throw new IllegalStateException("Emulation time budget reached");
                if(request.has("trace"))for(JsonElement v:request.getAsJsonArray("trace")){
                    JsonObject point=v.getAsJsonObject();if(number(point.get("address"))!=pc)continue;if(trace.size()>=512)throw new IllegalStateException("Trace budget reached");
                    JsonObject hit=new JsonObject();hit.addProperty("address",Long.toHexString(pc));hit.addProperty("tag",point.has("tag")?point.get("tag").getAsString():"");JsonArray args=new JsonArray();long esp=h.readRegister("ESP").longValue();
                    if(point.has("stack_offsets"))for(JsonElement off:point.getAsJsonArray("stack_offsets")){byte[] b=h.readMemory(addr(esp+off.getAsInt()),4);long n=0;for(int i=0;i<4;i++)n|=(b[i]&255L)<<(8*i);args.add(n);}hit.add("args",args);trace.add(hit);
                }
                if(!h.step(TaskMonitor.DUMMY))throw new IllegalStateException("Ghidra emulator: "+h.getLastError()+" at "+h.getExecutionAddress());
            }
            JsonObject out=new JsonObject();out.addProperty("ok",stopped);out.addProperty("steps",steps);out.addProperty("pc",h.getExecutionAddress().toString());out.add("trace",trace);JsonObject regs=new JsonObject();
            for(String n:new String[]{"EAX","ECX","EDX","EBX","ESI","EDI","EBP","ESP"})regs.addProperty(n,h.readRegister(n).toString(16));out.add("registers",regs);JsonArray watched=new JsonArray();
            if(request.has("watch"))for(JsonElement v:request.getAsJsonArray("watch")){JsonObject w=v.getAsJsonObject();int size=w.get("size").getAsInt();if(size<0||size>8*1024*1024)throw new IllegalArgumentException("Watch exceeds budget");byte[] b=h.readMemory(addr(number(w.get("address"))),size);JsonObject value=new JsonObject();value.add("address",w.get("address"));value.addProperty("size",size);value.addProperty("sha256",hash(b));if(size<=512)value.addProperty("hex",HexFormat.of().formatHex(b));watched.add(value);}out.add("memory",watched);return out;
        }finally{h.dispose();}
    }
}
