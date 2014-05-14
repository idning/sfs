#!/bin/bash

#for i in {1..2}
#do
#    ssh localhost "cd /tmp/a/ && mkdir -p g$i "
#done
#
#for i in {1..2}
#do
#    ssh localhost "cd /tmp/a/g$i && ~/idning-paper/iozone3_373/fileop -f 20 -bw -s1K" &
#done

#ssh localhost "cd /mnt/mfs && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &




#ssh localhost "cd /mnt/mfs/ && mkdir -p g1 && mkdir -p g2 && mkdir -p g3 && mkdir -p g4 && mkdir -p g5 && mkdir -p g6" 
#
#ssh localhost "cd /mnt/mfs/g1 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &
#ssh localhost "cd /mnt/mfs/g2 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &
#ssh localhost "cd /mnt/mfs/g3 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &
#ssh localhost "cd /mnt/mfs/g4 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &
#ssh localhost "cd /mnt/mfs/g5 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &
#ssh localhost "cd /mnt/mfs/g6 && ~/idning-paper/iozone3_373/fileop -f 10 -bw -s1K" &



ssh localhost "cd /tmp/a/a && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.a" &
ssh localhost "cd /tmp/b/b && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.b" &
ssh localhost "cd /tmp/c/c && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.c" &
ssh localhost "cd /tmp/d/d && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.d" &

ssh localhost "cd /tmp/1/1 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.a" &
ssh localhost "cd /tmp/2/2 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.b" &
ssh localhost "cd /tmp/3/3 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.c" &
ssh localhost "cd /tmp/4/4 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.d" &

