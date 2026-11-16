// Materialize clean-project functions absent from Ghidra only when their
// verified entry address is executable and outside an existing function.
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class MaterializeCleanFunctionMapEntries extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 3) {
            throw new IllegalArgumentException(
                "Expected function-map.json, report path, and source prefix");
        }
        String pinnedSha = "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d";
        if (!pinnedSha.equalsIgnoreCase(currentProgram.getExecutableSHA256())) {
            throw new IllegalStateException("Refusing to modify an unpinned executable");
        }
        JsonObject root = JsonParser.parseString(Files.readString(Paths.get(args[0])))
            .getAsJsonObject();
        if (!pinnedSha.equalsIgnoreCase(root.get("original_sha256").getAsString())) {
            throw new IllegalStateException("Function map is for a different executable");
        }

        StringBuilder report = new StringBuilder();
        String prefix = args[2];
        int selected = 0;
        int created = 0;
        int existing = 0;
        int overlapping = 0;
        int nonExecutable = 0;
        int failures = 0;
        JsonArray entries = root.getAsJsonArray("functions");
        for (int i = 0; i < entries.size(); i++) {
            JsonObject item = entries.get(i).getAsJsonObject();
            String source = item.get("source").getAsString().replace('\\', '/');
            if (!source.startsWith(prefix)) {
                continue;
            }
            selected++;
            String addressText = item.get("address").getAsString();
            String desired = item.get("name").getAsString();
            Address address = toAddr(addressText);
            Function function = getFunctionAt(address);
            if (function != null) {
                report.append("EXISTING ").append(addressText).append(' ')
                    .append(function.getName()).append('\n');
                existing++;
                continue;
            }
            function = getFunctionContaining(address);
            if (function != null) {
                report.append("OVERLAP ").append(addressText).append(" in ")
                    .append(function.getEntryPoint()).append(' ')
                    .append(function.getName()).append('\n');
                overlapping++;
                continue;
            }
            var block = currentProgram.getMemory().getBlock(address);
            if (block == null || !block.isExecute()) {
                report.append("NON_EXECUTABLE ").append(addressText).append('\n');
                nonExecutable++;
                continue;
            }
            try {
                if (getInstructionAt(address) == null && !disassemble(address)) {
                    throw new IllegalStateException("Disassembly failed");
                }
                if (getInstructionAt(address) == null) {
                    throw new IllegalStateException("No instruction at entry after disassembly");
                }
                function = createFunction(address, desired);
                if (function == null) {
                    throw new IllegalStateException("Ghidra did not create a function");
                }
                function.setName(desired, SourceType.USER_DEFINED);
                report.append("CREATED ").append(addressText).append(' ')
                    .append(desired).append(" body=")
                    .append(function.getBody().getMinAddress()).append("..")
                    .append(function.getBody().getMaxAddress()).append('\n');
                created++;
            } catch (Exception error) {
                report.append("FAILED ").append(addressText).append(' ')
                    .append(desired).append(" reason=")
                    .append(error.toString().replace('\n', ' ')).append('\n');
                failures++;
            }
        }
        report.append("SUMMARY selected=").append(selected)
            .append(" created=").append(created)
            .append(" existing=").append(existing)
            .append(" overlapping=").append(overlapping)
            .append(" non_executable=").append(nonExecutable)
            .append(" failed=").append(failures).append('\n');
        Path reportPath = Paths.get(args[1]).toAbsolutePath();
        Files.createDirectories(reportPath.getParent());
        Files.writeString(reportPath, report.toString(), StandardCharsets.UTF_8);
        println(report.substring(report.lastIndexOf("SUMMARY ")).trim());
    }
}
