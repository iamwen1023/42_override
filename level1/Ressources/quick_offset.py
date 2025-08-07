#!/usr/bin/env python3
import sys
pattern = "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4"
hex_val = sys.argv[1].replace('0x', '') if len(sys.argv) > 1 else "37634136"
ascii_le = bytes.fromhex(hex_val).decode('ascii', errors='ignore')
ascii_be = bytes.fromhex(hex_val)[::-1].decode('ascii', errors='ignore')
print(f"Hex: 0x{hex_val}")
print(f"Little-endian: '{ascii_le}' -> offset: {pattern.find(ascii_le) if pattern.find(ascii_le) != -1 else 'Not found'}")
print(f"Big-endian: '{ascii_be}' -> offset: {pattern.find(ascii_be) if pattern.find(ascii_be) != -1 else 'Not found'}") 