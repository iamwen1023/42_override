The main idea of the script is to pass through the following condition:
```c
        if (serial == local_char)
            return (0);
```

int the `auth` function.

Since we control the `serial` value, and the input which is used to create the `local_char` one,
we can create a script that'll generate the good serial from a specific string.

The `local_char` variable is generated with that code:
```c
int local_char = (int)(buff[3] ^ 4919) + 0x5eeded; // ebp-0x10
int i = 0;                                         // ebp-0x14
while (i < size)
{
	int old_c = buff[i];
	if (old_c <= 31)
		return (1);

	local_char += (old_c ^ local_char) % 1337;
	++i;
}
```

What we can do is from a given buffer of at least 3 characters, make this algorithm again in python for example,
and see what is the result of that code.

If we run the script, we can have the following output:
```sh
$ python exploit.py coucou
For the 'coucou' buffer, you can try the '6232801' serial
```

And so, if we try these values into the terminal:
```sh
level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: coucou
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232801
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```