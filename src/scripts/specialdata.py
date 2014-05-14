#!/usr/bin/python
import sys, re
sizestr = re.search(r'(\d*)', sys.argv[1]).group(1)
size = int(sizestr)
print size

if sys.argv[1].find("k") > 0 or sys.argv[1].find("K") > 0 :
    size=size*1024
if sys.argv[1].find("m") > 0 or sys.argv[1].find("M") > 0 :
    size=size*1024
    size=size*1024
print size


for i in range(size/16):
    ii = i * 16
    sys.stdout.write('%015x\n' % ii)
   #sys.stdout.write('.'*8)
    
    
