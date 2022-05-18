#!/usr/bin/env python

import cgi, cgitb

print("hello")

cgitb.enable()

input_data = cgi.FieldStorage()

try:
	val1 = input_data["value1"].value
	val2 = input_data["value2"].value
	val3 = input_data["value3"].value
except:
	print('<output>Sorry, the script cannot turn your inputs into numbers (integers).</output>')
	raise SystemExit(1)
print('<output>{0} {1} {2}</output>'.format(val1, val2, val3))