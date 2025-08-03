#!/usr/bin/env python2

import sys

def hex_to_ascii(hex_string):
    """Convert hex string to ASCII"""
    # Remove '0x' prefix if present
    if hex_string.startswith('0x'):
        hex_string = hex_string[2:]
    
    try:
        # Convert hex to bytes
        bytes_data = hex_string.decode('hex')
        
        # Try to decode as ASCII (little-endian)
        ascii_le = bytes_data
        
        # Try to decode as ASCII (big-endian)
        ascii_be = bytes_data[::-1]
        
        return ascii_le, ascii_be, bytes_data
    except Exception as e:
        print("Error converting hex to ASCII: %s" % e)
        return None, None, None

def find_offset(pattern, target):
    """Find the offset of target string in pattern"""
    try:
        offset = pattern.find(target)
        return offset if offset != -1 else None
    except:
        return None

def main():
    if len(sys.argv) != 2:
        print("Usage: python2 find_offset.py <hex_string>")
        print("Example: python2 find_offset.py 0x37634136")
        sys.exit(1)
    
    hex_string = sys.argv[1]
    print("Input hex: %s" % hex_string)
    
    # Static pattern
    pattern = "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4"
    
    # Convert hex to ASCII
    ascii_le, ascii_be, hex_bytes = hex_to_ascii(hex_string)
    
    if ascii_le is None:
        print("Failed to convert hex to ASCII")
        sys.exit(1)
    
    print("Hex bytes: %s" % repr(hex_bytes))
    print("ASCII (little-endian): '%s'" % ascii_le)
    print("ASCII (big-endian): '%s'" % ascii_be)
    print("Pattern: %s" % pattern)
    print("Pattern length: %d" % len(pattern))
    
    # Find offsets
    print("\nSearching for offsets...")
    
    offset_le = find_offset(pattern, ascii_le)
    if offset_le is not None:
        print("Found '%s' at offset: %d" % (ascii_le, offset_le))
    else:
        print("'%s' not found in pattern" % ascii_le)
    
    offset_be = find_offset(pattern, ascii_be)
    if offset_be is not None:
        print("Found '%s' at offset: %d" % (ascii_be, offset_be))
    else:
        print("'%s' not found in pattern" % ascii_be)
    
    # Also try to find the raw hex bytes
    if len(hex_bytes) == 4:
        hex_offset = find_offset(pattern, hex_bytes)
        if hex_offset is not None:
            print("Found hex bytes %s at offset: %d" % (repr(hex_bytes), hex_offset))
        else:
            print("Hex bytes %s not found in pattern" % repr(hex_bytes))

if __name__ == "__main__":
    main() 