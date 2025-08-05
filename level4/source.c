#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>

/*
 * LEVEL4 - Anti-Debugging Challenge
 * 
 * This program implements anti-debugging techniques:
 * 1. Fork() to create a child process
 * 2. Child process sets up ptrace protection
 * 3. Parent process monitors child for debugging attempts
 * 4. Uses gets() for vulnerable input (buffer overflow potential)
 * 
 * Assembly Analysis:
 * - Function prologue: push %ebp, mov %esp,%ebp, push %edi, push %ebx, and $0xfffffff0,%esp, sub $0xb0,%esp
 * - Stack allocation: 176 bytes (0xb0) with 16-byte alignment
 * - Uses fork(), prctl(), ptrace(), wait(), gets(), kill() system calls
 */

int main() {
    int child_pid;      // Stored at 0xac(%esp) - 172 bytes from esp
    int buffer[8];      // Stored at 0x20(%esp) - 32 bytes from esp (32 bytes = 8 ints)
    int status;         // Stored at 0xa8(%esp) - 168 bytes from esp
    int wait_status;    // Stored at 0x1c(%esp) - 28 bytes from esp
    int ptrace_result;  // Stored at 0xa0(%esp) - 160 bytes from esp
    
    // Assembly: call fork@plt
    // fork() - Create a child process, returns PID in parent, 0 in child
    child_pid = fork();
    
    // Assembly: lea 0x20(%esp),%ebx; mov $0x0,%eax; mov $0x20,%edx; mov %ebx,%edi; mov %edx,%ecx; rep stos %eax,%es:(%edi)
    // Initialize buffer with zeros using rep stos (repeat store string)
    // This clears the 32-byte buffer (8 integers)
    for (int i = 0; i < 8; i++) {
        buffer[i] = 0;
    }
    
    // Assembly: cmpl $0x0,0xac(%esp); jne 0x8048769
    // Check if we're in child process (fork returns 0 in child)
    if (child_pid == 0) {
        // CHILD PROCESS - Anti-debugging setup
        
        // Assembly: movl $0x1,0x4(%esp); movl $0x1,(%esp); call prctl@plt
        // prctl(PR_SET_DUMPABLE, 1) - Allow core dumps
        prctl(PR_SET_DUMPABLE, 1);
        
        // Assembly: movl $0x0,0xc(%esp); movl $0x0,0x8(%esp); movl $0x0,0x4(%esp); movl $0x0,(%esp); call ptrace@plt
        // ptrace(PTRACE_TRACEME, 0, 0, 0) - Request to be traced by parent
        // This is the key anti-debugging mechanism!
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        
        // Assembly: movl $0x8048903,(%esp); call puts@plt
        // Print message (likely "Give me some shellcode, k")
        puts("Give me some shellcode, k");
        
        // Assembly: lea 0x20(%esp),%eax; mov %eax,(%esp); call gets@plt
        // gets(buffer) - VULNERABLE! No bounds checking, can overflow buffer
        gets((char*)buffer);
        
        // Assembly: jmp 0x804881a
        // Jump to function epilogue
        return 0;
        
    } else {
        // PARENT PROCESS - Monitor child for debugging attempts
        
        // Assembly: lea 0x1c(%esp),%eax; mov %eax,(%esp); call wait@plt
        // wait(&wait_status) - Wait for child to finish or be stopped
        wait(&wait_status);
        
        // Assembly: mov 0x1c(%esp),%eax; mov %eax,0xa0(%esp); mov 0xa0(%esp),%eax; and $0x7f,%eax; test %eax,%eax; je 0x80487ac
        // Check if child exited normally (WIFEXITED macro equivalent)
        if (WIFEXITED(wait_status)) {
            // Child exited normally, continue to next check
        } else {
            // Assembly: mov 0x1c(%esp),%eax; mov %eax,0xa4(%esp); mov 0xa4(%esp),%eax; and $0x7f,%eax; add $0x1,%eax; sar %al; test %al,%al; jle 0x80487ba
            // Check if child was stopped by signal (WIFSIGNALED macro equivalent)
            if (WIFSIGNALED(wait_status)) {
                // Child was killed by signal, print error and exit
                puts("child is exiting...");
                return 0;
            }
        }
        
        // Assembly: movl $0x0,0xc(%esp); movl $0x2c,0x8(%esp); mov 0xac(%esp),%eax; mov %eax,0x4(%esp); movl $0x3,(%esp); call ptrace@plt
        // ptrace(PTRACE_PEEKDATA, child_pid, 0x2c, 0) - Read data from child's memory
        // This checks if a debugger is attached by trying to read child's memory
        ptrace_result = ptrace(PTRACE_PEEKDATA, child_pid, 0x2c, 0);
        
        // Assembly: cmpl $0xb,0xa8(%esp); jne 0x8048768
        // Check if ptrace returned 0xb (11 in decimal)
        // If ptrace fails (returns -1), it means a debugger is attached
        if (ptrace_result == 0xb) {
            // No debugger detected, print success message
            puts("no exec() for you");
            
            // Assembly: movl $0x9,0x4(%esp); mov 0xac(%esp),%eax; mov %eax,(%esp); call kill@plt
            // kill(child_pid, SIGKILL) - Kill the child process
            kill(child_pid, SIGKILL);
        } else {
            // Debugger detected, loop back to wait for child
            // This creates an infinite loop if debugger is detected
            goto wait_loop;
        }
    }
    
    // Assembly: mov $0x0,%eax; lea -0x8(%ebp),%esp; pop %ebx; pop %edi; pop %ebp; ret
    // Function epilogue: return 0
    return 0;
}

/*
 * ANTI-DEBUGGING MECHANISMS EXPLAINED:
 * 
 * 1. FORK() PROTECTION:
 *    - Creates child process to handle user input
 *    - Parent monitors child for debugging attempts
 *    - If debugger detected, child is killed
 * 
 * 2. PTRACE PROTECTION:
 *    - Child calls ptrace(PTRACE_TRACEME, 0, 0, 0)
 *    - This allows only ONE debugger to attach
 *    - If another debugger tries to attach, it fails
 * 
 * 3. PTRACE DETECTION:
 *    - Parent tries to read child's memory with ptrace(PTRACE_PEEKDATA)
 *    - If debugger is attached, this operation fails
 *    - Parent checks return value to detect debugger
 * 
 * 4. VULNERABILITY:
 *    - gets() function has no bounds checking
 *    - Buffer overflow can overwrite return address
 *    - Shellcode can be injected to bypass protection
 * 
 * 5. EXPLOITATION STRATEGY:
 *    - Inject shellcode into buffer
 *    - Overflow buffer to overwrite return address
 *    - Point return address to shellcode in buffer
 *    - Execute shellcode to get shell
 * 
 * 6. BYPASSING ANTI-DEBUGGING:
 *    - Use shellcode instead of debugging
 *    - Buffer overflow bypasses ptrace protection
 *    - Direct code execution in child process
 */

