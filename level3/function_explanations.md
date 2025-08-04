# Level3 - Function Call Explanations

## Overview
This program implements a **simple XOR-based decryption challenge** where the user must provide the correct input to decrypt a hidden message and get a shell.

## Main Function Analysis

### 1. time() Function
```assembly
0x0804886c <+18>:    movl   $0x0,(%esp)
0x08048873 <+25>:    call   0x80484b0 <time@plt>
```
**Purpose**: Get current timestamp
**C equivalent**: `time(NULL)`
**Why**: Used as seed for random number generator

### 2. srand() Function
```assembly
0x08048878 <+30>:    mov    %eax,(%esp)
0x0804887b <+33>:    call   0x8048500 <srand@plt>
```
**Purpose**: Seed random number generator
**C equivalent**: `srand(time(NULL))`
**Why**: Ensures different random numbers on each run

### 3. puts() Function (Banner)
```assembly
0x08048880 <+38>:    movl   $0x8048a48,(%esp)
0x08048887 <+45>:    call   0x80484d0 <puts@plt>
```
**Purpose**: Print banner message
**C equivalent**: `puts("***********************************")`

### 4. puts() Function (Welcome)
```assembly
0x0804888c <+50>:    movl   $0x8048a6c,(%esp)
0x08048893 <+57>:    call   0x80484d0 <puts@plt>
```
**Purpose**: Print welcome message
**C equivalent**: `puts("*\t\tlevel03\t\t**")`

### 5. puts() Function (Banner again)
```assembly
0x08048898 <+62>:    movl   $0x8048a48,(%esp)
0x0804889f <+69>:    call   0x80484d0 <puts@plt>
```
**Purpose**: Print banner again
**C equivalent**: `puts("***********************************")`

### 6. printf() Function
```assembly
0x080488a4 <+74>:    mov    $0x8048a7b,%eax
0x080488a9 <+79>:    mov    %eax,(%esp)
0x080488ac <+82>:    call   0x8048480 <printf@plt>
```
**Purpose**: Print input prompt
**C equivalent**: `printf("Password:")`

### 7. scanf() Function
```assembly
0x080488b1 <+87>:    mov    $0x8048a85,%eax
0x080488b6 <+92>:    lea    0x1c(%esp),%edx
0x080488ba <+96>:    mov    %edx,0x4(%esp)
0x080488be <+100>:   mov    %eax,(%esp)
0x080488c1 <+103>:   call   0x8048530 <__isoc99_scanf@plt>
```
**Purpose**: Read integer input from user
**C equivalent**: `scanf("%d", &user_input)`

### 8. test() Function Call
```assembly
0x080488c6 <+108>:   mov    0x1c(%esp),%eax
0x080488ca <+112>:   movl   $0x1337d00d,0x4(%esp)
0x080488d2 <+120>:   mov    %eax,(%esp)
0x080488d5 <+123>:   call   0x8048747 <test>
```
**Purpose**: Call test function with user input and magic number
**C equivalent**: `test(user_input, 0x1337d00d)`

## Test Function Analysis

### 9. rand() Function (in default case)
```assembly
0x0804884a <+259>:   call   0x8048520 <rand@plt>
0x0804884f <+264>:   mov    %eax,(%esp)
0x08048852 <+267>:   call   0x8048660 <decrypt>
```
**Purpose**: Generate random number for decryption key
**C equivalent**: `decrypt(rand())`
**When**: Called when difference > 21 (default case)

### 10. decrypt() Function Calls (in switch cases)
```assembly
0x0804877b <+52>:    call   0x8048660 <decrypt>
0x0804878b <+68>:    call   0x8048660 <decrypt>
# ... (repeated for all 22 cases)
```
**Purpose**: Decrypt message with specific key
**C equivalent**: `decrypt(difference)` where difference is 0-21

## Decrypt Function Analysis

### 11. strlen() Function (implemented with repnz scasb)
```assembly
0x0804869c <+60>:    lea    -0x1d(%ebp),%eax
0x0804869f <+63>:    movl   $0xffffffff,-0x2c(%ebp)
0x080486a6 <+70>:    mov    %eax,%edx
0x080486a8 <+72>:    mov    $0x0,%eax
0x080486ad <+77>:    mov    -0x2c(%ebp),%ecx
0x080486b0 <+80>:    mov    %edx,%edi
0x080486b2 <+82>:    repnz scas %es:(%edi),%al
0x080486b4 <+84>:    mov    %ecx,%eax
0x080486b6 <+86>:    not    %eax
0x080486b8 <+88>:    sub    $0x1,%eax
```
**Purpose**: Calculate string length
**C equivalent**: `strlen(encrypted_string)`
**How**: Uses `repnz scasb` to scan for null terminator

### 12. strcmp() Function (implemented with repz cmpsb)
```assembly
0x080486ed <+141>:   lea    -0x1d(%ebp),%eax
0x080486f0 <+144>:   mov    %eax,%edx
0x080486f2 <+146>:   mov    $0x80489c3,%eax
0x080486f7 <+151>:   mov    $0x11,%ecx
0x080486fc <+156>:   mov    %edx,%esi
0x080486fe <+158>:   mov    %eax,%edi
0x08048700 <+160>:   repz cmpsb %es:(%edi),%ds:(%esi)
0x08048702 <+162>:   seta   %dl
0x08048705 <+165>:   setb   %al
0x08048708 <+168>:   mov    %edx,%ecx
0x0804870a <+170>:   sub    %al,%cl
0x0804870c <+172>:   mov    %ecx,%eax
0x0804870e <+174>:   movsbl %al,%eax
0x08048711 <+177>:   test   %eax,%eax
```
**Purpose**: Compare decrypted string with expected result
**C equivalent**: `strcmp(decrypted_string, "Congratulations!")`
**How**: Uses `repz cmpsb` to compare strings byte by byte

### 13. system() Function (success case)
```assembly
0x08048715 <+181>:   movl   $0x80489d4,(%esp)
0x0804871c <+188>:   call   0x80484e0 <system@plt>
```
**Purpose**: Execute shell on successful decryption
**C equivalent**: `system("/bin/sh")`
**When**: Called when decrypted string equals "Congratulations!"

### 14. puts() Function (failure case)
```assembly
0x08048723 <+195>:   movl   $0x80489dc,(%esp)
0x0804872a <+202>:   call   0x80484d0 <puts@plt>
```
**Purpose**: Print error message on failed decryption
**C equivalent**: `puts("Invalid Password")`
**When**: Called when decrypted string doesn't match

## Security Features

### 15. Stack Canary (__stack_chk_fail)
```assembly
0x08048668 <+8>:     mov    %gs:0x14,%eax
0x0804866e <+14>:    mov    %eax,-0xc(%ebp)
# ... (at end of function)
0x0804872f <+207>:   mov    -0xc(%ebp),%esi
0x08048732 <+210>:   xor    %gs:0x14,%esi
0x08048739 <+217>:   je     0x8048740 <decrypt+224>
0x0804873b <+219>:   call   0x80484c0 <__stack_chk_fail@plt>
```
**Purpose**: Detect stack buffer overflow attacks
**How**: Compares stored canary value with original

## Key Constants and Values

- **Magic Number**: `0x1337d00d` (322424845 in decimal)
- **Expected Difference Range**: 0-21
- **Encrypted String**: "Q}|u`sfg~sf{}|a3"
- **Expected Result**: "Congratulations!"
- **Decryption Method**: XOR with key

## Exploitation Strategy

To get the shell, you need to:
1. **Calculate the correct input**: `322424845 - key = difference` (where difference is 0-21)
2. **Find the correct key**: The key that decrypts "Q}|u`sfg~sf{}|a3" to "Congratulations!"
3. **Input the calculated value**: `322424845 - correct_key`

The program uses a **jump table** (switch statement) to handle different decryption keys, making it a reverse engineering challenge to find the correct input value. 