# Usage

squareNumberList [number] [file]

# What it does

Writes a list of square numbers to file. The numbers are written in binary and without padding. The last byte is filled with zeroes if unwritten bits remain.

## Example

Command: `squareNumberList 10 squares.txt`
Output: `11001001 10000110 01100100 11000110 00000101 00011100 10000000` (7 bytes) is written to 'squares.txt'
Explanation: In binary, the first 10 square numbers are 1, 100, 1001, 10000, 11001, 100100, 110001, 1000000, 1010001, 1100100. 5 zeroes are added to fill the remaining bits of the last byte.
