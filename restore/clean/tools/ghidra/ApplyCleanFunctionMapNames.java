// Apply verified clean-C names to still-anonymous native functions.
// Run against the pinned Pharaoh.exe Ghidra project with the function-map.json
// path and a report path as the two script arguments.
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Iterator;

public class ApplyCleanFunctionMapNames extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2 || args.length > 3) {
            throw new IllegalArgumentException(
                "Expected function-map.json, report path, and optional source prefix");
        }
        if (!"fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
                .equalsIgnoreCase(currentProgram.getExecutableSHA256())) {
            throw new IllegalStateException("Refusing to name functions in an unpinned executable");
        }

        JsonObject root = JsonParser.parseString(Files.readString(Paths.get(args[0])))
            .getAsJsonObject();
        if (!"fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
                .equalsIgnoreCase(root.get("original_sha256").getAsString())) {
            throw new IllegalStateException("Function map is for a different executable");
        }

        StringBuilder report = new StringBuilder();
        String sourcePrefix = args.length == 3 ? args[2] : "game/runtime/;game/assets/";
        String[] sourcePrefixes = sourcePrefix.split(";");
        int selected = 0;
        int renamed = 0;
        int alreadyNamed = 0;
        int missing = 0;
        int conflicts = 0;
        JsonArray entries = root.getAsJsonArray("functions");
        for (int i = 0; i < entries.size(); i++) {
            JsonObject item = entries.get(i).getAsJsonObject();
            String source = item.get("source").getAsString().replace('\\', '/');
            boolean included = false;
            for (String prefix : sourcePrefixes) {
                if (!prefix.isEmpty() && source.startsWith(prefix)) {
                    included = true;
                    break;
                }
            }
            if (!included) {
                continue;
            }
            selected++;
            String address = item.get("address").getAsString();
            String desired = item.get("name").getAsString();
            Function function = getFunctionAt(toAddr(address));
            if (function == null) {
                report.append("MISSING ").append(address).append(' ').append(desired).append('\n');
                missing++;
                continue;
            }
            String current = function.getName();
            if (current.equals(desired)) {
                report.append("ALREADY ").append(address).append(' ').append(desired).append('\n');
                alreadyNamed++;
                continue;
            }
            if (!(current.startsWith("FUN_") || current.startsWith("sub_") ||
                    current.startsWith("thunk_FUN_"))) {
                report.append("PRESERVED ").append(address).append(' ').append(current)
                    .append(" => ").append(desired).append('\n');
                alreadyNamed++;
                continue;
            }

            boolean collision = false;
            Iterator<Symbol> symbols = currentProgram.getSymbolTable().getSymbols(desired);
            while (symbols.hasNext()) {
                Symbol symbol = symbols.next();
                if (!symbol.getAddress().equals(function.getEntryPoint())) {
                    collision = true;
                    break;
                }
            }
            if (collision) {
                report.append("CONFLICT ").append(address).append(' ').append(desired).append('\n');
                conflicts++;
                continue;
            }
            function.setName(desired, SourceType.USER_DEFINED);
            report.append("RENAMED ").append(address).append(' ').append(current)
                .append(" => ").append(desired).append('\n');
            renamed++;
        }

        report.append("SUMMARY selected=").append(selected)
            .append(" renamed=").append(renamed)
            .append(" already_named=").append(alreadyNamed)
            .append(" missing=").append(missing)
            .append(" conflicts=").append(conflicts).append('\n');
        Path reportPath = Paths.get(args[1]).toAbsolutePath();
        Files.createDirectories(reportPath.getParent());
        Files.writeString(reportPath, report.toString(), StandardCharsets.UTF_8);
        println(report.substring(report.lastIndexOf("SUMMARY ")).trim());
    }
}
