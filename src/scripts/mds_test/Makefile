ARG=--window-with-profile=tmp --working-directory=/home/ning/idning-paper/src --hide-menubar
all:
	cd /tmp/1/ && mkdir -p 1 2 3 4 5 6 7 8 a b c d e f g h
	#
	ssh localhost "cd /tmp/1/1 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.1" &
	ssh localhost "cd /tmp/2/2 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.2" &
	ssh localhost "cd /tmp/3/3 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.3" &
	ssh localhost "cd /tmp/4/4 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.4" &
	ssh localhost "cd /tmp/5/5 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.5" &
	ssh localhost "cd /tmp/6/6 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.6" &
	ssh localhost "cd /tmp/7/7 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.7" &
	ssh localhost "cd /tmp/8/8 && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.8" &
	#
	ssh localhost "cd /tmp/a/a && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.a" &
	ssh localhost "cd /tmp/b/b && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.b" &
	ssh localhost "cd /tmp/c/c && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.c" &
	ssh localhost "cd /tmp/d/d && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.d" &
	ssh localhost "cd /tmp/e/e && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.e" &
	ssh localhost "cd /tmp/f/f && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.f" &
	ssh localhost "cd /tmp/g/g && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.g" &
	ssh localhost "cd /tmp/h/h && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.h" &

create:
	cd /tmp/ && mkdir -p 1 2 3 4 5 6 7 8 a b c d e f g h

m:

	gnome-terminal $(ARG) -e "./client/mount.out /tmp/1" 	-t "c1" 	--geometry=80x5+0+0&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/2" 	-t "c2" 	--geometry=80x5+0+100&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/3" 	-t "c3" 	--geometry=80x5+0+200&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/4" 	-t "c4" 	--geometry=80x5+0+300&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/5" 	-t "c5" 	--geometry=80x5+0+400&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/6" 	-t "c6" 	--geometry=80x5+0+500&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/7" 	-t "c7" 	--geometry=80x5+0+600&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/8" 	-t "c8" 	--geometry=80x5+0+700&
	#
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/a" 	-t "ca" 	--geometry=80x5+300+0&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/b" 	-t "cb" 	--geometry=80x5+300+100&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/c" 	-t "cc" 	--geometry=80x5+300+200&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/d" 	-t "cd" 	--geometry=80x5+300+300&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/e" 	-t "ce" 	--geometry=80x5+300+400&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/f" 	-t "cf" 	--geometry=80x5+300+500&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/g" 	-t "cg" 	--geometry=80x5+300+600&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/h" 	-t "ch" 	--geometry=80x5+300+700&


u:
	-sudo umount /tmp/* -l


	

