#!/usr/bin/python3

"""
Read a log file created by interceptty and print it in a slightly
more readable format.
"""

import sys
import re

import hexdump

last_direction = None
data = []

def print_data():
    if not len(data):
        return

    print()
    hexdump.hexprint(data, title=(last_direction * 5))

for line in sys.stdin:
    direction, byte = re.match('^([><])\s+0x([0-9a-fA-F]{2}).*', line).groups()

    byte = int(byte, 16)

    if direction == last_direction:
        data.append(byte)
        continue

    print_data()
    
    last_direction = direction
    data = [byte]

print_data()
