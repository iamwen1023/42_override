#!/usr/bin/env python2
import sys

pattern = "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4"
hex_val = sys.argv[1].replace('0x', '') if len(sys.argv) > 1 else "37634136"

# Convert hex to ASCII (little-endian and big-endian)
ascii_le = hex_val.decode('hex')
ascii_be = hex_val.decode('hex')[::-1]

print "Hex: 0x{}".format(hex_val)
print "Little-endian: '{}' -> offset: {}".format(ascii_le, pattern.find(ascii_le) if pattern.find(ascii_le) != -1 else 'Not found')
print "Big-endian: '{}' -> offset: {}".format(ascii_be, pattern.find(ascii_be) if pattern.find(ascii_be) != -1 else 'Not found')