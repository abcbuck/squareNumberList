# Usage

`squareNumberList [number] [file]`

# What it does

Writes a list of the first `[number]` square numbers to `[file]`. The numbers are written in binary and without padding. The last byte is filled with zeroes if unwritten bits remain.

## Example

Command: `squareNumberList 10 squares.txt`

Output: `11001001 10000110 01100100 11000110 00000101 00011100 10000000` (7 bytes) is written to 'squares.txt'

Explanation: In binary, the first 10 square numbers are 1, 100, 1001, 10000, 11001, 100100, 110001, 1000000, 1010001, 1100100. 5 zeroes are added to fill the remaining bits of the last byte.

# Why

Originally, this was intended to generate structured data for compression testing. You may find other uses.

The files in the following image store square numbers from 1 to 100 million squared.

![image](https://github.com/user-attachments/assets/c13a1213-69e1-4db0-bd7d-240e6460b0fb)

Results may vary with different files. This image is not indicative of the compression quality of the shown formats in general.
