// Run from analyzeHeadless with one argument: the output .c path.
// This writes Ghidra's pseudocode from the opened program and its current type database.

import ghidra.app.decompiler.DecompileResults;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

import java.io.BufferedWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ExportTypedPseudocode extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] arguments = getScriptArgs();
        if (arguments.length != 1) {
            throw new IllegalArgumentException(
                "Expected one output path for the pseudocode export");
        }

        Path outputPath = Paths.get(arguments[0]).toAbsolutePath();
        Path outputDirectory = outputPath.getParent();
        if (outputDirectory != null) {
            Files.createDirectories(outputDirectory);
        }

        DecompInterface decompiler = new DecompInterface();
        DecompileOptions options = new DecompileOptions();
        options.grabFromProgram(currentProgram);
        decompiler.setOptions(options);
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(false);
        decompiler.setSimplificationStyle("decompile");
        if (!decompiler.openProgram(currentProgram)) {
            throw new IllegalStateException(
                "Could not open the current program in the decompiler: " +
                decompiler.getLastMessage());
        }

        int total = 0;
        int succeeded = 0;
        int failed = 0;
        try (BufferedWriter writer = Files.newBufferedWriter(outputPath,
                 StandardCharsets.UTF_8)) {
            writer.write("/* Ghidra pseudocode for ");
            writer.write(currentProgram.getName().replace("*/", "* /"));
            writer.write(". Uses the opened program's current type database. ");
            writer.write("For analysis only; this file is not a compiled source unit. */\n\n");

            FunctionIterator functions =
                currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext() && !monitor.isCancelled()) {
                Function function = functions.next();
                total++;
                monitor.setMessage("Decompiling " + function.getEntryPoint() +
                    " (" + total + ")");

                writer.write("/* ");
                writer.write(function.getEntryPoint().toString());
                writer.write(" ");
                writer.write(function.getName().replace("*/", "* /"));
                writer.write(" */\n");

                DecompileResults result =
                    decompiler.decompileFunction(function, 60, monitor);
                if (result.decompileCompleted() &&
                    result.getDecompiledFunction() != null) {
                    writer.write(result.getDecompiledFunction().getC());
                    writer.write("\n\n");
                    succeeded++;
                } else {
                    String message = result.getErrorMessage();
                    writer.write("/* DECOMPILATION_FAILED: ");
                    writer.write((message == null ? "unknown error" : message)
                        .replace("*/", "* /").replace('\n', ' '));
                    writer.write(" */\n\n");
                    failed++;
                }
                writer.flush();
            }

            writer.write("/* EXPORT_SUMMARY total=");
            writer.write(Integer.toString(total));
            writer.write(" succeeded=");
            writer.write(Integer.toString(succeeded));
            writer.write(" failed=");
            writer.write(Integer.toString(failed));
            writer.write(" cancelled=");
            writer.write(Boolean.toString(monitor.isCancelled()));
            writer.write(" */\n");
        } finally {
            decompiler.dispose();
        }

        println("Ghidra pseudocode export: " + outputPath);
        println("Functions: " + total + ", succeeded: " + succeeded +
            ", failed: " + failed + ", cancelled: " + monitor.isCancelled());
    }
}
