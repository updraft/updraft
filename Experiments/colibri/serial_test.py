#!/usr/bin/python3

"""
A simple test that sends hex string to serial port and prints hex dump of the reply.

Usage:
    ./serial_test.py <port> <baudrate> <hex_data>

Reads at most 1K of reply, or timeouts after 2 seconds.
"""

import serial
import sys
import hexdump
import __main__

if len(sys.argv) != 4:
    print(__main__.__doc__)
    sys.exit(1)

port = serial.Serial(sys.argv[1], baudrate=sys.argv[2], timeout=0.5)

data = bytes.fromhex(sys.argv[3])

hexdump.hexprint(data, title='Sending')

port.write(bytes.fromhex(sys.argv[3]))

reply = port.read(1024)

if reply:
    hexdump.hexprint(reply, title='Received')
else:
    print('Nothing received')
