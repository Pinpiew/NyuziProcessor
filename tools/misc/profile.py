# 
# Copyright (C) 2011-2014 Jeff Bush
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
# Boston, MA  02110-1301, USA.
# 


#
# USAGE: profile <objdump> <pc dump>
# Prints a breakdown of time spent per function 
#

import sys, re

symbolre = re.compile('(?P<addr>[A-Fa-f0-9]+) g\s+F\s+\.text\s+[A-Fa-f0-9]+\s+(?P<symbol>\w+)')

# Read symbols
functions = []
counts = {}
f = open(sys.argv[1], 'r')
for line in f.readlines():
	got = symbolre.search(line)
	if got:
		sym = got.group('symbol')
		functions += [(int(got.group('addr'), 16), sym)]
		counts[sym] = 0

f.close()

def findFunction(pc):
	for address, name in reversed(functions):
		if pc >= address:
			return name

	return None

# Read profile trace
linesProcessed = 0
f = open(sys.argv[2], 'r')
for line in f.readlines():
	pc = int(line, 16)
	func = findFunction(pc)
	if func:
		counts[func] += 1

f.close()

totalCycles = 0
sortedTab = []
for name in counts:
	sortedTab += [ (counts[name], name) ]
	totalCycles += counts[name]

for count, name in sorted(sortedTab, key=lambda func: func[0], reverse=True):
	if count == 0:
		break
		
	print count, str(float(count * 10000 / totalCycles) / 100) + '%', name

