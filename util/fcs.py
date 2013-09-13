#!/usr/bin/env python

# This tool is useful for finding common substrings in binary
# files. This will help us pick out similarities such as
# section header magic numbers that are buried in with the
# rest of the packed binary data.

# Running this on large files should be considered a horrible
# mistake. It creates a list of n*m elements, where n and m
# are the number of bytes in the input file1 and file2,
# respectively. Usually I run this on 4K byte chunks.

import argparse
import sys

# Based on longest common substring algorithm
def common_substring(input1, input2, threshold):
    count_matrix = [[0] * len(input2) for _ in xrange(len(input1))]

    substrings = []
    for i, input1char in enumerate(input1):
        for j, input2char in enumerate(input2):
            if input1char == input2char:
                if i == 0 or j == 0:
                    count_matrix[i][j] = 1
                else:
                    count_matrix[i][j] = count_matrix[i-1][j-1] + 1
                if count_matrix[i][j] >= threshold:
                    sub = input1[i-count_matrix[i][j]+1:i+1]
                    # prune the shorter substring, but preserve duplicates
                    if sub[:-1] in substrings:
                        substrings.remove(sub[:-1])
                    substrings.append(sub)
            else:
                count_matrix[i][j] = 0
    
    return substrings

parser = argparse.ArgumentParser(description="Find common substrings >= N length")
parser.add_argument('N', type=int, help="minimum length of substrings to report")
parser.add_argument('file1', type=argparse.FileType('rb'), help="binary input file")
parser.add_argument('file2', type=argparse.FileType('rb'), help="binary input file")

args = parser.parse_args()
subs = common_substring(args.file1.read(), args.file2.read(),
                        args.N)

for sub in subs:
    print(sub)
