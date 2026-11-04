// Private typed-fact pilot. Import project is disposable; no existing project is opened.
// @category PharaohRestore
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;
import com.google.gson.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.pcode.*;

public class ExportTypedPilot extends GhidraScript {
    private static final String PIN="fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d";
    private String address(Address a){return a==null?null:a.getAddressSpace().getName()+":"+Long.toUnsignedString(a.getOffset(),16);}
    private JsonElement type(DataType t){if(t==null)return JsonNull.INSTANCE;JsonObject j=new JsonObject();j.addProperty("name",t.getName());j.addProperty("display",t.getDisplayName());j.addProperty("path",t.getPathName());j.addProperty("size_bytes",t.getLength());j.addProperty("java_class",t.getClass().getName());if(t instanceof Pointer p)j.add("pointee",type(p.getDataType()));if(t instanceof Array a){j.addProperty("elements",a.getNumElements());j.add("element",type(a.getDataType()));}return j;}
    private JsonElement high(HighVariable h){if(h==null)return JsonNull.INSTANCE;JsonObject j=new JsonObject();j.addProperty("class",h.getClass().getSimpleName());j.addProperty("name",h.getName());j.addProperty("size_bytes",h.getSize());j.add("datatype",type(h.getDataType()));Varnode r=h.getRepresentative();j.addProperty("representative",r==null?null:address(r.getAddress()));if(h instanceof HighGlobal g)j.add("symbol",symbol(g.getSymbol()));return j;}
    private JsonElement symbol(HighSymbol s){if(s==null)return JsonNull.INSTANCE;JsonObject j=new JsonObject();j.addProperty("id",s.getId());j.addProperty("name",s.getName());j.addProperty("size_bytes",s.getSize());j.add("datatype",type(s.getDataType()));JsonArray storage=new JsonArray();if(s.getStorage()!=null)for(Varnode v:s.getStorage().getVarnodes())storage.add(node(v,false));j.add("storage",storage);return j;}
    private JsonElement node(Varnode v,boolean includeHigh){if(v==null)return JsonNull.INSTANCE;JsonObject j=new JsonObject();j.addProperty("address",address(v.getAddress()));j.addProperty("size_bytes",v.getSize());j.addProperty("constant",v.isConstant());j.addProperty("register",v.isRegister());j.addProperty("memory",v.isAddress());j.addProperty("unique",v.isUnique());if(includeHigh&&v instanceof VarnodeAST a)j.add("high",high(a.getHigh()));return j;}
    private JsonObject op(PcodeOp p){JsonObject j=new JsonObject();j.addProperty("site",address(p.getSeqnum().getTarget()));j.addProperty("sequence",p.getSeqnum().getTime());j.addProperty("opcode",p.getMnemonic());JsonArray in=new JsonArray();for(int i=0;i<p.getNumInputs();i++)in.add(node(p.getInput(i),true));j.add("inputs",in);j.add("output",node(p.getOutput(),true));return j;}
    public void run() throws Exception {
        String[] args=getScriptArgs();if(args.length!=1)throw new IllegalArgumentException("Exactly one private output path required");
        Path out=Path.of(args[0]).toAbsolutePath().normalize();String expected="\\restore\\build\\bulk-type-pilot\\";if(!out.toString().toLowerCase().contains(expected))throw new IllegalArgumentException("Output outside private pilot");
        String projectRaw=state.getProject().getProjectLocator().getLocation();
        if(projectRaw.matches("^/[A-Za-z]:/.*"))projectRaw=projectRaw.substring(1);
        Path projectLocation=Path.of(projectRaw).toAbsolutePath().normalize();
        if(!projectLocation.toString().toLowerCase().contains(expected))throw new IllegalArgumentException("Refusing nonprivate Ghidraproject");
        if(!PIN.equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalArgumentException("Wrong imported executableSHA256");
        String[] entries={"0040fa40","0040fa70","0040fae0","00425f20","0053db60"};
        // Only the new private import is changed. Seed verified entries to make no-analysis import useful.
        for(String a:new String[]{"0040fa40","0040fa70","0040fae0","00425f20","00426090","004260d0","00426100","00426150","004d5f60","0053db60","0053f790"}){Address at=toAddr(a);if(getFunctionAt(at)==null){disassemble(at);if(createFunction(at,null)==null)throw new IllegalStateException("Cannot create private entry "+a);}}
        DecompInterface dec=new DecompInterface();dec.toggleCCode(true);dec.toggleSyntaxTree(true);if(!dec.openProgram(currentProgram))throw new IllegalStateException("Cannot open privateprogram");
        JsonObject root=new JsonObject();root.addProperty("native_sha256",PIN);root.addProperty("program",currentProgram.getName());root.addProperty("private_project_location",projectLocation.toString());root.addProperty("ghidra_version",ghidra.framework.Application.getApplicationVersion());root.addProperty("language",currentProgram.getLanguageID().toString());root.addProperty("private_noanalysis_import",true);JsonObject functions=new JsonObject();
        try{for(String entry:entries){Function f=getFunctionAt(toAddr(entry));DecompileResults r=dec.decompileFunction(f,120,monitor);if(!r.decompileCompleted()||r.getHighFunction()==null||r.getDecompiledFunction()==null)throw new IllegalStateException(entry+":"+r.getErrorMessage());HighFunction hf=r.getHighFunction();JsonObject j=new JsonObject();j.addProperty("entry",entry);j.addProperty("listing_signature",f.getPrototypeString(false,true));j.addProperty("raw_pseudocode",r.getDecompiledFunction().getC());j.addProperty("decompile_error",r.getErrorMessage());
            JsonArray nativeCode=new JsonArray();InstructionIterator it=currentProgram.getListing().getInstructions(f.getBody(),true);while(it.hasNext()){Instruction ins=it.next();JsonObject n=new JsonObject();n.addProperty("address",address(ins.getAddress()));n.addProperty("instruction",ins.toString());n.addProperty("bytes",HexFormat.of().formatHex(ins.getBytes()));JsonArray pc=new JsonArray();for(PcodeOp po:ins.getPcode())pc.add(op(po));n.add("raw_pcode",pc);nativeCode.add(n);}j.add("native_instructions",nativeCode);
            FunctionPrototype fp=hf.getFunctionPrototype();JsonObject proto=new JsonObject();proto.addProperty("calling_model",fp.getModelName());proto.add("return_datatype",type(fp.getReturnType()));proto.addProperty("return_storage",fp.getReturnStorage().toString());JsonArray params=new JsonArray();for(int i=0;i<fp.getNumParams();i++)params.add(symbol(fp.getParam(i)));proto.add("parameters",params);j.add("high_prototype",proto);
            JsonArray globals=new JsonArray();Iterator<HighSymbol> gi=hf.getGlobalSymbolMap().getSymbols();while(gi.hasNext()){HighSymbol s=gi.next();JsonObject g=symbol(s).getAsJsonObject();Address a=s.getStorage()==null?null:s.getStorage().getMinAddress();Data data=a==null?null:currentProgram.getListing().getDataAt(a);g.add("listing_datatype_advisory",data==null?JsonNull.INSTANCE:type(data.getDataType()));globals.add(g);}j.add("global_symbols",globals);
            JsonArray tokens=new JsonArray();List<ClangNode> flat=new ArrayList<>();r.getCCodeMarkup().flatten(flat);int ordinal=0;for(ClangNode cn:flat){if(cn instanceof ClangToken t){JsonObject q=new JsonObject();q.addProperty("ordinal",ordinal++);q.addProperty("text",t.getText());q.addProperty("class",t.getClass().getSimpleName());q.addProperty("min_address",address(t.getMinAddress()));q.addProperty("max_address",address(t.getMaxAddress()));q.add("varnode",node(t.getVarnode(),true));q.add("high_variable",high(t.getHighVariable()));PcodeOp po=t.getPcodeOp();q.add("pcode",po==null?JsonNull.INSTANCE:op(po));tokens.add(q);}}j.add("clang_tokens",tokens);
            JsonArray operations=new JsonArray();Iterator<PcodeOpAST> pi=hf.getPcodeOps();while(pi.hasNext())operations.add(op(pi.next()));j.add("high_pcode",operations);functions.add(entry,j);println("Typed facts complete "+entry+" tokens="+tokens.size()+" instructions="+nativeCode.size());
        }}finally{dec.dispose();}root.add("functions",functions);Files.createDirectories(out.getParent());Files.writeString(out,new GsonBuilder().setPrettyPrinting().disableHtmlEscaping().create().toJson(root)+"\n");println("TYPED_PILOT_COMPLETE "+out);
    }
}
