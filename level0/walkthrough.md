Level0 is a simple password validation program that compares user input against a hardcoded value. The goal is to find the correct password to gain shell access.

The key comparison is at line 83:
```
cmp    $0x149c,%eax
```
- Converting to decimal: `0x149c = 5276`

The password is **5276**

```bash
cat /home/users/level01/.pass
```
