# Level0 Walkthrough

## Overview
Level0 is a simple password validation program that compares user input against a hardcoded value. The goal is to find the correct password to gain shell access.

## Assembly Analysis

### Function Prologue (Lines 0-6)
```
0x08048494 <+0>:	push   %ebp
0x08048495 <+1>:	mov    %esp,%ebp
0x08048497 <+3>:	and    $0xfffffff0,%esp
0x0804849a <+6>:	sub    $0x20,%esp
```
- Standard function prologue
- Sets up stack frame
- Aligns stack to 16-byte boundary
- Allocates 32 bytes (0x20) of local variables

### Banner Display (Lines 9-40)
```
0x0804849d <+9>:	movl   $0x80485f0,(%esp)
0x080484a4 <+16>:	call   0x8048390 <puts@plt>
0x080484a9 <+21>:	movl   $0x8048614,(%esp)
0x080484b0 <+28>:	call   0x8048390 <puts@plt>
0x080484b5 <+33>:	movl   $0x80485f0,(%esp)
0x080484bc <+40>:	call   0x8048390 <puts@plt>
```
- Displays the banner with asterisks
- Calls `puts()` three times to print the decorative border

### Password Prompt (Lines 45-53)
```
0x080484c1 <+45>:	mov    $0x804862c,%eax
0x080484c6 <+50>:	mov    %eax,(%esp)
0x080484c9 <+53>:	call   0x8048380 <printf@plt>
```
- Loads address of "Password:" string into `%eax`
- Calls `printf()` to display the prompt

### Input Reading (Lines 58-74)
```
0x080484ce <+58>:	mov    $0x8048636,%eax
0x080484d3 <+63>:	lea    0x1c(%esp),%edx
0x080484d7 <+67>:	mov    %edx,0x4(%esp)
0x080484db <+71>:	mov    %eax,(%esp)
0x080484de <+74>:	call   0x80483d0 <__isoc99_scanf@plt>
```
- Loads format string for `scanf()` (likely "%d")
- Calculates address of local variable at `%esp + 0x1c` (28 bytes from stack pointer)
- Calls `scanf()` to read integer input from user

### Password Comparison (Lines 79-88)
```
0x080484e3 <+79>:	mov    0x1c(%esp),%eax
0x080484e7 <+83>:	cmp    $0x149c,%eax
0x080484ec <+88>:	jne    0x804850d <main+121>
```
- **CRITICAL**: Loads user input from stack into `%eax`
- Compares it with the hardcoded value `0x149c`
- If not equal, jumps to error handling

### Success Path (Lines 90-119)
```
0x080484ee <+90>:	movl   $0x8048639,(%esp)
0x080484f5 <+97>:	call   0x8048390 <puts@plt>
0x080484fa <+102>:	movl   $0x8048649,(%esp)
0x08048501 <+109>:	call   0x80483a0 <system@plt>
0x08048506 <+114>:	mov    $0x0,%eax
0x0804850b <+119>:	jmp    0x804851e <main+138>
```
- Prints "Authenticated!" message
- Calls `system("/bin/sh")` to spawn a shell
- Returns 0 (success)

### Error Path (Lines 121-133)
```
0x0804850d <+121>:	movl   $0x8048651,(%esp)
0x08048514 <+128>:	call   0x8048390 <puts@plt>
0x08048519 <+133>:	mov    $0x1,%eax
```
- Prints "Invalid Password!" message
- Returns 1 (error)

## Solution Steps

### Step 1: Identify the Hardcoded Value
The key comparison is at line 83:
```
cmp    $0x149c,%eax
```

### Step 2: Convert Hex to Decimal
- `0x149c` in hexadecimal
- Converting to decimal: `0x149c = 5276`

### Step 3: Verify the Solution
The password is **5276**

### Step 4: Test the Solution
```bash
./level0
# Enter: 5276
# Should output: "Authenticated!" and spawn a shell
```

## Key Insights

1. **Simple Integer Comparison**: The program uses a straightforward integer comparison
2. **No Encryption**: The password is stored as a plain integer constant
3. **Direct Shell Access**: Success grants immediate shell access via `system("/bin/sh")`
4. **Stack Layout**: User input is stored at `%esp + 0x1c` (28 bytes from stack pointer)

## Flag
The password for level0 is: **5276**

## Next Level
After gaining shell access, check for the level1 password:
```bash
cat /home/users/level01/.pass
```
