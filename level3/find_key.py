#!/usr/bin/env python3

def decrypt_xor(encrypted_string, key):
    """Decrypt string using XOR with given key"""
    decrypted = ""
    for char in encrypted_string:
        decrypted += chr(ord(char) ^ key)
    return decrypted

def find_correct_key():
    """Find the key that decrypts to 'Congratulations!'"""
    
    # The encrypted string from the assembly
    encrypted = "Q}|u`sfg~sf{}|a3"
    
    # The expected result
    expected = "Congratulations!"
    
    print("=== Level3 Key Finder ===")
    print(f"Encrypted string: {encrypted}")
    print(f"Expected result: {expected}")
    print(f"Encrypted length: {len(encrypted)}")
    print(f"Expected length: {len(expected)}")
    print()
    
    # Try all possible keys (0-21)
    print("Testing all keys from 0 to 21:")
    print("-" * 50)
    
    for key in range(22):  # 0 to 21
        decrypted = decrypt_xor(encrypted, key)
        print(f"Key {key:2d}: {decrypted}")
        
        if decrypted == expected:
            print(f"\n🎉 FOUND THE CORRECT KEY: {key}")
            return key
    
    print("\n❌ No key found in range 0-21")
    return None

def calculate_input_value(key):
    """Calculate the input value that leads to the correct key"""
    magic_number = 0x1337d00d  # 322424845
    input_value = magic_number - key
    return input_value

def main():
    print("=== Level3 Exploit Calculator ===")
    
    # Find the correct key
    correct_key = find_correct_key()
    
    if correct_key is not None:
        # Calculate the input value
        input_value = calculate_input_value(correct_key)
        
        print(f"\n=== SOLUTION ===")
        print(f"Correct key: {correct_key}")
        print(f"Magic number: 0x1337d00d ({0x1337d00d})")
        print(f"Input value: {input_value}")
        print(f"Verification: {0x1337d00d} - {input_value} = {correct_key}")
        
        print(f"\n=== EXPLOIT ===")
        print(f"Run the program and enter: {input_value}")
        print(f"This will call: decrypt({correct_key})")
        print(f"Which will decrypt to: 'Congratulations!'")
        print(f"And then execute: system('/bin/sh')")
        
        # Test the decryption
        encrypted = "Q}|u`sfg~sf{}|a3"
        decrypted = decrypt_xor(encrypted, correct_key)
        print(f"\n=== VERIFICATION ===")
        print(f"Encrypted: {encrypted}")
        print(f"Key: {correct_key}")
        print(f"Decrypted: {decrypted}")
        print(f"Expected: Congratulations!")
        print(f"Match: {decrypted == 'Congratulations!'}")
        
    else:
        print("\n❌ Could not find the correct key!")
        print("Try expanding the key range or check the encrypted string.")

if __name__ == "__main__":
    main() 