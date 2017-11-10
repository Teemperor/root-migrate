#!/usr/bin/env python

from __future__ import print_function

import sys

print("Comparing expected " + sys.argv[1] + " against " + sys.argv[2])

with open(sys.argv[1]) as f:
    expected = f.readlines()
expected = [x.strip() for x in expected]

with open(sys.argv[2]) as f:
    output = f.readlines()
output = [x.strip() for x in output]

got_error = False

if len(output) != len(expected):
  print("Different numbers of lines! Expected output has " +
        str(len(output)) + " lines  but output has " + str(len(expected)))
  got_error = True

min_line_num = min(len(output), len(expected))

for line_index in range(0, min_line_num):
  line_output = output[line_index]
  line_expected = expected[line_index]
  line_num = line_index + 1
  if line_output != line_expected:
    print("#############################################")
    print("Different line " + str(line_num) + ":")
    print("Expected: " + line_expected)
    print("     Got: " + line_output)
    got_error = True

if got_error:
  exit(1)
