#!/usr/bin/env python3

# Quick verification
encrypted = "Q}|u`sfg~sf{}|a3"
key = 18
magic = 0x1337d00d

# Decrypt
decrypted = "".join(chr(ord(c) ^ key) for c in encrypted)
print(f"Decrypted: {decrypted}")

# Calculate input
input_value = magic - key
print(f"Input value: {input_value}")
print(f"Verification: {magic} - {input_value} = {key}")

# Test the logic
user_input = input_value
difference = magic - user_input
print(f"Program logic: {magic} - {user_input} = {difference}")
print(f"Will call: decrypt({difference})")
print(f"Result: {decrypted == 'Congratulations!'}") 