# Level3 - XOR Decryption Challenge Walkthrough

## Overview
Level3 contains a **XOR-based decryption challenge** where the user must provide the correct input to decrypt a hidden message and get a shell. The program uses a switch statement with a jump table to handle different decryption keys.

## Key Assembly Analysis

### Main Function Flow
```assembly
0x0804886c <+18>:    movl   $0x0,(%esp)
0x08048873 <+25>:    call   0x80484b0 <time@plt>          # time(NULL)
0x08048878 <+30>:    mov    %eax,(%esp)
0x0804887b <+33>:    call   0x8048500 <srand@plt>         # srand(time(NULL))
0x080488c1 <+103>:   call   0x8048530 <__isoc99_scanf@plt> # scanf("%d", &user_input)
0x080488d5 <+123>:   call   0x8048747 <test>              # test(user_input, 0x1337d00d)
```

### Test Function Logic
```assembly
0x0804874d <+6>:     mov    0x8(%ebp),%eax               # user_input
0x08048750 <+9>:     mov    0xc(%ebp),%edx               # 0x1337d00d
0x08048755 <+14>:    sub    %eax,%ecx                    # difference = expected - input
0x0804875c <+21>:    cmpl   $0x15,-0xc(%ebp)             # if (difference > 21)
0x08048760 <+25>:    ja     0x804884a <test+259>         # goto default case
```

### Jump Table Implementation
```assembly
0x08048766 <+31>:    mov    -0xc(%ebp),%eax              # load difference
0x08048769 <+34>:    shl    $0x2,%eax                    # multiply by 4 (for jump table)
0x0804876c <+37>:    add    $0x80489f0,%eax              # add jump table base address
0x08048771 <+42>:    mov    (%eax),%eax                  # load jump target
0x08048773 <+44>:    jmp    *%eax                        # jump to case
```

## Vulnerability Analysis

### 1. Predictable Random Numbers
- **Issue**: `srand(time(NULL))` uses current time as seed
- **Impact**: Random numbers are predictable if you know the time
- **Exploit**: Not the primary attack vector

### 2. XOR Decryption with Known Target
- **Issue**: Encrypted string and expected result are hardcoded
- **Impact**: Can reverse engineer the correct key
- **Exploit**: Find key that decrypts to "Congratulations!"

### 3. Limited Key Range
- **Issue**: Only keys 0-21 are valid (difference must be ≤ 21)
- **Impact**: Brute force is feasible
- **Exploit**: Test all 22 possible keys

## Exploitation Strategy

### Step 1: Understand the Logic
```c
difference = 0x1337d00d - user_input;
if (difference <= 21) {
    decrypt(difference);  // Use difference as XOR key
}
```

### Step 2: Find the Correct Key
The encrypted string is "Q}|u`sfg~sf{}|a3" and should decrypt to "Congratulations!".

```python
def find_key():
    encrypted = "Q}|u`sfg~sf{}|a3"
    expected = "Congratulations!"
    
    for key in range(22):  # Test keys 0-21
        decrypted = "".join(chr(ord(c) ^ key) for c in encrypted)
        if decrypted == expected:
            return key
    return None
```

### Step 3: Calculate Input Value
```python
magic_number = 0x1337d00d  # 322424845
correct_key = 18
input_value = magic_number - correct_key
# input_value = 322424845 - 18 = 322424827
```

## Exploit Development

### Method 1: Brute Force Approach
```python
#!/usr/bin/env python3

def decrypt_xor(encrypted, key):
    return "".join(chr(ord(c) ^ key) for c in encrypted)

def find_solution():
    encrypted = "Q}|u`sfg~sf{}|a3"
    expected = "Congratulations!"
    magic = 0x1337d00d
    
    for key in range(22):
        decrypted = decrypt_xor(encrypted, key)
        if decrypted == expected:
            input_value = magic - key
            return input_value, key
    return None, None

input_value, key = find_solution()
print(f"Input: {input_value}")
print(f"Key: {key}")
```

### Method 2: Manual Calculation
1. **Find key 18** decrypts "Q}|u`sfg~sf{}|a3" to "Congratulations!"
2. **Calculate input**: `0x1337d00d - 18 = 322424827`
3. **Verify**: `322424845 - 322424827 = 18`

## Testing Commands

```bash
# Run the program
./level03

# Enter the calculated input
322424827

# Expected output
***********************************
*		level03		**
***********************************
Password: 322424827
$  # Shell prompt
```

## Expected Results

1. **Correct input** will decrypt the string to "Congratulations!"
2. **System call** will execute `/bin/sh`
3. **Shell access** will be granted

## Key Points

- **Magic number**: `0x1337d00d` (322424845)
- **Valid difference range**: 0-21
- **Encrypted string**: "Q}|u`sfg~sf{}|a3"
- **Expected result**: "Congratulations!"
- **Correct key**: 18
- **Input value**: 322424827
- **Decryption method**: XOR with key
- **Jump table**: 22 cases (0-21) plus default

## Exploitation Steps

1. **Analyze the assembly** to understand the logic
2. **Identify the encrypted string** and expected result
3. **Find the correct XOR key** (18) through brute force
4. **Calculate the input value** (322424827)
5. **Test the solution** by running the program
6. **Get shell access** through the system call

The challenge is a **reverse engineering exercise** that requires understanding the XOR decryption logic and finding the correct input value through analysis of the hardcoded strings.
