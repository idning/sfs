#!/bin/bash
ls -l
cat hello_
echo "abcdefghijk" > hello_
cat helllo_


iozone -i 0 -i 1 -g 1G -Rab out.wks| tee iozone.log

iozone -i 0 -i 1 -g 1G -y 4k -q 1M -Rab out.wks| tee iozone.log

iozone -i 0 -i 1 -g 10M  -n 1M -r 1M   -Rab out.wks| tee iozone.log

