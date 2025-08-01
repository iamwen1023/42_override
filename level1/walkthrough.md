# Level1 Walkthrough

## Assembly Analysis

### Main Function Structure

#### Function Prologue (Lines 0-8)
```
0x080484d0 <+0>:	push   %ebp                    # Save old base pointer
0x080484d1 <+1>:	mov    %esp,%ebp               # Set new base pointer
0x080484d3 <+3>:	push   %edi                    # Save %edi register
0x080484d4 <+4>:	push   %ebx                    # Save %ebx register
0x080484d5 <+5>:	and    $0xfffffff0,%esp        # Align stack to 16-byte boundary
0x080484d8 <+8>:	sub    $0x60,%esp              # Allocate 96 bytes on stack
```
**Explanation**: Standard function prologue that sets up the stack frame, saves registers that will be used, aligns the stack for optimal performance, and allocates local variables.

#### Buffer Initialization (Lines 11-31)
```
0x080484db <+11>:	lea    0x1c(%esp),%ebx         # Load address of password buffer into %ebx
0x080484df <+15>:	mov    $0x0,%eax               # Set %eax to 0 (value to fill buffer with)
0x080484e4 <+20>:	mov    $0x10,%edx              # Set %edx to 16 (number of bytes to fill)
0x080484e9 <+25>:	mov    %ebx,%edi               # Copy buffer address to %edi
0x080484eb <+27>:	mov    %edx,%ecx               # Copy count to %ecx
0x080484ed <+29>:	rep stos %eax,%es:(%edi)       # Fill buffer with zeros (rep stos = repeat store)
0x080484ef <+31>:	movl   $0x0,0x5c(%esp)        # Initialize result variable to 0
```
**Explanation**: This section initializes the password buffer with zeros using the `rep stos` instruction, which is equivalent to `memset(buffer, 0, 16)`. The buffer is located at `%esp + 0x1c` (28 bytes from stack pointer).

#### Banner and Username Prompt (Lines 39-59)
```
0x080484f7 <+39>:	movl   $0x80486b8,(%esp)      # Load banner message address
0x080484fe <+46>:	call   0x8048380 <puts@plt>     # Call puts() to display banner
0x08048503 <+51>:	mov    $0x80486df,%eax         # Load username prompt address
0x08048508 <+56>:	mov    %eax,(%esp)             # Set up argument for printf
0x0804850b <+59>:	call   0x8048360 <printf@plt>   # Call printf() to display prompt
```
**Explanation**: Displays the banner and username prompt. The addresses `0x80486b8` and `0x80486df` point to the banner and prompt strings respectively.

#### Username Input (Lines 64-88)
```
0x08048510 <+64>:	mov    0x804a020,%eax          # Load stdin file pointer
0x08048515 <+69>:	mov    %eax,0x8(%esp)          # Set 3rd argument (FILE*)
0x08048519 <+73>:	movl   $0x100,0x4(%esp)       # Set 2nd argument (size = 256)
0x08048521 <+81>:	movl   $0x804a040,(%esp)      # Set 1st argument (buffer address)
0x08048528 <+88>:	call   0x8048370 <fgets@plt>   # Call fgets(username_buffer, 256, stdin)
```
**Explanation**: Reads username input using `fgets()`. The function call is equivalent to `fgets(username_buffer, 256, stdin)` where `0x804a040` is the global username buffer.

#### Username Verification (Lines 93-107)
```
0x0804852d <+93>:	call   0x8048464 <verify_user_name>  # Call username verification function
0x08048532 <+98>:	mov    %eax,0x5c(%esp)               # Store return value
0x08048536 <+102>:	cmpl   $0x0,0x5c(%esp)              # Compare return value with 0
0x0804853b <+107>:	je     0x8048550 <main+128>          # If equal (success), jump to password prompt
```
**Explanation**: Calls the username verification function and checks its return value. If the return value is 0 (success), it continues to the password prompt. Otherwise, it jumps to error handling.

#### Error Path for Username (Lines 109-126)
```
0x0804853d <+109>:	movl   $0x80486f0,(%esp)     # Load error message address
0x08048544 <+116>:	call   0x8048380 <puts@plt>    # Call puts() to display error
0x08048549 <+121>:	mov    $0x1,%eax              # Set return value to 1 (error)
0x0804854e <+126>:	jmp    0x80485af <main+223>   # Jump to function epilogue
```
**Explanation**: Displays error message and returns 1 to indicate failure. The address `0x80486f0` points to the error message string.

#### Password Prompt (Lines 128-135)
```
0x08048550 <+128>:	movl   $0x804870d,(%esp)     # Load password prompt address
0x08048557 <+135>:	call   0x8048380 <puts@plt>    # Call puts() to display password prompt
```
**Explanation**: Displays the password prompt. The address `0x804870d` points to the password prompt string.

#### Password Input (Lines 140-164)
```
0x0804855c <+140>:	mov    0x804a020,%eax         # Load stdin file pointer
0x08048561 <+145>:	mov    %eax,0x8(%esp)         # Set 3rd argument (FILE*)
0x08048565 <+149>:	movl   $0x64,0x4(%esp)       # Set 2nd argument (size = 100)
0x0804856d <+157>:	lea    0x1c(%esp),%eax        # Calculate password buffer address
0x08048571 <+161>:	mov    %eax,(%esp)            # Set 1st argument (buffer address)
0x08048574 <+164>:	call   0x8048370 <fgets@plt>  # Call fgets(password_buffer, 100, stdin)
```
**Explanation**: Reads password input using `fgets()`. The function call is equivalent to `fgets(password_buffer, 100, stdin)` where the password buffer is located at `%esp + 0x1c`.

#### Password Verification (Lines 169-190)
```
0x08048579 <+169>:	lea    0x1c(%esp),%eax        # Calculate password buffer address
0x0804857d <+173>:	mov    %eax,(%esp)            # Set 1st argument for verify_user_pass
0x08048580 <+176>:	call   0x80484a3 <verify_user_pass>  # Call password verification function
0x08048585 <+181>:	mov    %eax,0x5c(%esp)       # Store return value
0x08048589 <+185>:	cmpl   $0x0,0x5c(%esp)       # Compare return value with 0
0x0804858e <+190>:	je     0x8048597 <main+199>   # If equal (success), jump to success path
```
**Explanation**: Calls the password verification function and checks its return value. If the return value is 0 (success), it continues to the success path. Otherwise, it jumps to error handling.

#### Success Path (Lines 199-216)
```
0x08048597 <+199>:	movl   $0x804871e,(%esp)     # Load success message address
0x0804859e <+206>:	call   0x8048380 <puts@plt>    # Call puts() to display success message
0x080485a3 <+211>:	mov    $0x1,%eax              # Set return value to 1 (success)
0x080485a8 <+216>:	jmp    0x80485af <main+223>   # Jump to function epilogue
```
**Explanation**: Displays success message and returns 1 to indicate success. The address `0x804871e` points to the success message string.

### verify_user_name Function

#### Function Prologue (Lines 0-8)
```
0x08048464 <+0>:	push   %ebp                    # Save old base pointer
0x08048465 <+1>:	mov    %esp,%ebp               # Set new base pointer
0x08048467 <+3>:	push   %edi                    # Save %edi register
0x08048468 <+4>:	push   %esi                    # Save %esi register
0x08048469 <+5>:	sub    $0x10,%esp              # Allocate 16 bytes on stack
```
**Explanation**: Standard function prologue that sets up the stack frame and saves registers that will be used for string operations.

#### String Comparison (Lines 15-51)
```
0x0804846c <+8>:	movl   $0x8048690,(%esp)      # Load "verif_user: " message address
0x08048473 <+15>:	call   0x8048380 <puts@plt>     # Call puts() to display message
0x08048478 <+20>:	mov    $0x804a040,%edx        # Load username input buffer address
0x0804847d <+25>:	mov    $0x80486a8,%eax        # Load expected username address
0x08048482 <+30>:	mov    $0x7,%ecx              # Set counter to 7 (length of username)
0x08048487 <+35>:	mov    %edx,%esi              # Set %esi to input string
0x08048489 <+37>:	mov    %eax,%edi              # Set %edi to expected string
0x0804848b <+39>:	repz cmpsb %es:(%edi),%ds:(%esi)  # Compare strings byte by byte
```
**Explanation**: This is the core string comparison logic. The `repz cmpsb` instruction compares two strings byte by byte:
- `repz`: Repeat while zero flag is set (continue until difference found or count reaches 0)
- `cmpsb`: Compare string bytes
- `%es:(%edi)`: Expected username string
- `%ds:(%esi)`: User input string
- `%ecx`: Counter (7 bytes to compare)

The comparison continues until either:
1. A difference is found (zero flag cleared)
2. All 7 bytes match (counter reaches 0)

#### Return Value Calculation (Lines 41-51)
```
0x0804848d <+41>:	seta   %dl                     # Set %dl if above (input > expected)
0x08048490 <+44>:	setb   %al                     # Set %al if below (input < expected)
0x08048493 <+47>:	mov    %dl,%cl                 # Copy above flag to %cl
0x08048495 <+49>:	sub    %al,%cl                 # Subtract below flag from above flag
0x08048497 <+51>:	mov    %cl,%al                 # Copy result to %al
0x08048499 <+53>:	movsbl %al,%eax               # Sign extend byte to 32-bit
```
**Explanation**: This calculates the return value based on the string comparison:
- If strings are equal: both flags are 0, result = 0 (success)
- If input > expected: above flag = 1, below flag = 0, result = 1 (failure)
- If input < expected: above flag = 0, below flag = 1, result = -1 (failure)

#### Function Epilogue (Lines 56-62)
```
0x0804849c <+56>:	add    $0x10,%esp             # Restore stack pointer
0x0804849f <+59>:	pop    %esi                    # Restore %esi register
0x080484a0 <+60>:	pop    %edi                    # Restore %edi register
0x080484a1 <+61>:	pop    %ebp                    # Restore base pointer
0x080484a2 <+62>:	ret                            # Return from function
```
**Explanation**: Standard function epilogue that restores the stack and registers.

### verify_user_pass Function

#### Function Prologue (Lines 0-8)
```
0x080484a3 <+0>:	push   %ebp                    # Save old base pointer
0x080484a4 <+1>:	mov    %esp,%ebp               # Set new base pointer
0x080484a6 <+3>:	push   %edi                    # Save %edi register
0x080484a7 <+4>:	push   %esi                    # Save %esi register
0x080484a8 <+5>:	mov    0x8(%ebp),%eax          # Get password buffer address from stack
```
**Explanation**: Standard function prologue that sets up the stack frame and gets the password buffer address from the function parameter.

#### String Comparison (Lines 15-38)
```
0x080484ab <+8>:	mov    %eax,%edx               # Copy password buffer address to %edx
0x080484ad <+10>:	mov    $0x80486b0,%eax        # Load expected password address
0x080484b2 <+15>:	mov    $0x5,%ecx              # Set counter to 5 (length of password)
0x080484b7 <+20>:	mov    %edx,%esi              # Set %esi to input password
0x080484b9 <+22>:	mov    %eax,%edi              # Set %eax to expected password
0x080484bb <+24>:	repz cmpsb %es:(%edi),%ds:(%esi)  # Compare strings byte by byte
```
**Explanation**: Similar to username verification, but compares 5 bytes instead of 7. The `repz cmpsb` instruction compares the input password with the expected password.

#### Return Value Calculation (Lines 26-38)
```
0x080484bd <+26>:	seta   %dl                     # Set %dl if above (input > expected)
0x080484c0 <+29>:	setb   %al                     # Set %al if below (input < expected)
0x080484c3 <+32>:	mov    %dl,%cl                 # Copy above flag to %cl
0x080484c5 <+34>:	sub    %al,%cl                 # Subtract below flag from above flag
0x080484c7 <+36>:	mov    %cl,%al                 # Copy result to %al
0x080484c9 <+38>:	movsbl %al,%eax               # Sign extend byte to 32-bit
```
**Explanation**: Same return value calculation logic as `verify_user_name`:
- If passwords match: result = 0 (success)
- If passwords don't match: result = 1 or -1 (failure)

#### Function Epilogue (Lines 41-44)
```
0x080484cc <+41>:	pop    %esi                    # Restore %esi register
0x080484cd <+42>:	pop    %edi                    # Restore %edi register
0x080484ce <+43>:	pop    %ebp                    # Restore base pointer
0x080484cf <+44>:	ret                            # Return from function
```
**Explanation**: Standard function epilogue that restores registers and returns.

## Solution Steps

### Step 1: Extract Hardcoded Strings
We need to examine the memory addresses to find the hardcoded strings:

- Username comparison: `0x80486a8` (7 bytes)
- Password comparison: `0x80486b0` (5 bytes)

### Step 2: Use GDB to Extract Strings
```bash
(gdb) x/s 0x80486a8
# This will show the expected username

(gdb) x/s 0x80486b0  
# This will show the expected password
```

### Step 3: Alternative Method - Dynamic Analysis
Run the program and observe the behavior:
```bash
./level1
# Enter different usernames to see which one is accepted
# Then enter different passwords to find the correct one
```

### Step 4: String Extraction from Memory
The strings are likely stored in the data section. We can extract them by:
1. Running the program in GDB
2. Setting breakpoints at the comparison functions
3. Examining the memory addresses

## Key Insights

1. **Two-Step Authentication**: Requires both correct username and password
2. **String Comparisons**: Uses `repz cmpsb` for exact string matching
3. **Fixed Lengths**: Username is 7 bytes, password is 5 bytes
4. **No Encryption**: Plain text string comparisons
5. **Buffer Locations**: 
   - Username input: `0x804a040`
   - Password input: Local buffer at `%esp + 0x1c`
   - Expected username: `0x80486a8`
   - Expected password: `0x80486b0`

## Expected Solution
The program expects:
- **Username**: 7 characters (to be determined from memory)
- **Password**: 5 characters (to be determined from memory)

## Next Steps
1. Use GDB to extract the hardcoded strings
2. Test the extracted username and password
3. Gain shell access to proceed to the next level

## Note
This level requires dynamic analysis or memory examination to extract the hardcoded strings, as they're not immediately visible in the assembly code like the integer comparison in level0.
