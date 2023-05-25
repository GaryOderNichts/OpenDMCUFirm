# heyconv.py: convert motorola S19-style SREC to the DMCU hex format
# This file is part of OpenDMCUFirm

import sys

if len(sys.argv) != 3:
    print(f'Usage: {sys.argv[0]} in.s19 out.d.hex')
    sys.exit(1)

infile = open(sys.argv[1], "r")
outfile = open(sys.argv[2], "w")

lines = infile.readlines()
for lnum, line in enumerate(lines):
    if line[0] != 'S':
        print(f'Error ({lnum}): Invalid record start: {line[0]}')
        sys.exit(1)

    type = line[1]
    if type != '0' and type != '1' and type != '5' and type != '9':
        print(f'Error ({lnum}): Invalid record type: {type}. Not S19-style?')
        sys.exit(1)

    byte_count = int(line[2:4], 16)
    address = int(line[4:8], 16)

    data_count = byte_count - 2
    checksum_start = byte_count*2 + 2
    data = bytes.fromhex(line[8:checksum_start])
    checksum = int(line[checksum_start:checksum_start+2], 16)

    # calculate checksum
    sum = 0
    for i in range(2, checksum_start, 2):
        sum += int(line[i:i+2], 16)
    sum = 0xFF - (sum & 0xFF)

    if checksum != sum:
        print(f'Error ({lnum}): Invalid checksum')
        sys.exit(1)

    # write data records to outfile
    if type == '1':
        outfile.write(f'@{address:04X}')
        for b in data:
            outfile.write(f' {b:02X}')
        outfile.write('\n')

# append null term at the end
outfile.write('\0')

infile.close()
outfile.close()
