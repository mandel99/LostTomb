#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    DWORD pid;
    DWORD selected_tid = 0;
    HANDLE process;
    HANDLE snapshot;
    THREADENTRY32 entry;

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: capture_x86_threads PID [TID]\n");
        return 2;
    }
    pid = (DWORD)strtoul(argv[1], NULL, 10);
    if (argc == 3) selected_tid = (DWORD)strtoul(argv[2], NULL, 10);
    process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 3;
    entry.dwSize = sizeof(entry);
    if (Thread32First(snapshot, &entry)) {
        do {
            HANDLE thread;
            CONTEXT context;
            if (entry.th32OwnerProcessID != pid) continue;
            if (selected_tid != 0 && entry.th32ThreadID != selected_tid) continue;
            thread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT |
                                THREAD_QUERY_INFORMATION, FALSE, entry.th32ThreadID);
            if (thread == NULL) continue;
            if (SuspendThread(thread) != (DWORD)-1) {
                ZeroMemory(&context, sizeof(context));
                context.ContextFlags = CONTEXT_FULL;
                if (GetThreadContext(thread, &context)) {
                    printf("tid=%lu eip=%08lx esp=%08lx ebp=%08lx eax=%08lx "
                           "ebx=%08lx ecx=%08lx edx=%08lx esi=%08lx edi=%08lx\n",
                           entry.th32ThreadID, context.Eip, context.Esp, context.Ebp,
                           context.Eax, context.Ebx, context.Ecx, context.Edx,
                           context.Esi, context.Edi);
                    if (process != NULL) {
                        DWORD words[96];
                        SIZE_T read = 0;
                        unsigned int i;
                        if (ReadProcessMemory(process, (const void *)(ULONG_PTR)context.Esp,
                                              words, sizeof(words), &read)) {
                            printf("stack:");
                            for (i = 0; i < read / sizeof(words[0]); ++i) {
                                if ((words[i] >= 0x00400000 && words[i] < 0x00600000) ||
                                    (words[i] >= 0x10000000 && words[i] < 0x80000000)) {
                                    printf(" +%03x=%08lx", i * 4, words[i]);
                                }
                            }
                            putchar('\n');
                        }
                        {
                            DWORD frame = context.Ebp;
                            unsigned int depth;
                            printf("frames:");
                            for (depth = 0; depth < 64 && frame != 0; ++depth) {
                                DWORD pair[2];
                                SIZE_T frame_read = 0;
                                if (!ReadProcessMemory(process, (const void *)(ULONG_PTR)frame,
                                                       pair, sizeof(pair), &frame_read) ||
                                    frame_read != sizeof(pair)) break;
                                printf(" %08lx", pair[1]);
                                if (pair[0] <= frame || pair[0] - frame > 0x100000) break;
                                frame = pair[0];
                            }
                            putchar('\n');
                        }
                    }
                }
                ResumeThread(thread);
            }
            CloseHandle(thread);
        } while (Thread32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    if (process != NULL) CloseHandle(process);
    return 0;
}
