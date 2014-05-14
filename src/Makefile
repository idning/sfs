#usage :
# alias m='make'
# m
# m c
# m t 
# m cs
#
#
#
ARG=--window-with-profile=tmp --working-directory=/home/ning/idning-paper/src --hide-menubar

.PHONY: all cs g c t r k gen clean test  u m v run killall indent
all: cs 
	scons -Q -f scons.py

g : gen
c : clean
t : test
r : run
k : killall
u:
	-mkdir -p /tmp/a
	-sudo umount /tmp/a -l
	-sudo umount /tmp/b -l
	-sudo umount /tmp/c -l
	-sudo umount /tmp/d -l
m: u
	#./client/mount.out /tmp/a -d
	./client/mount.out /tmp/a 
v: u
	valgrind --tool=memcheck --leak-check=full -v ./client/mount.out /tmp/a -d 
#i:
	#gnome-terminal $(ARG) -e "cd /tmp/a && iozone -a"                      -t "cmgr"   --geometry=60x60+0+0

data:
	python ./scripts/specialdata.py 65k > /tmp/65k
	python ./scripts/specialdata.py 1M > /tmp/1M


gen: 
	#./scripts/protocol_gen.py
	./scripts/event_rpcgen.py common/protocol.rpc

clean:
	scons -f scons.py -c
	find . -name '*.gcov' | xargs rm -f
	find . -name '*.gcda' | xargs rm -f
	find . -name '*.gcno' | xargs rm -f
	#find . -name '*.out' | xargs rm -f
	find . -name '*.o' | xargs rm -f
	find . -name '*~' | xargs rm -f

test: all
	scons -Q -f scons.py ning_target=test

#cscope
cs:
	echo "" > /tmp/cscope.files
	-find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.h" >> /tmp/cscope.files
	-find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.c" >> /tmp/cscope.files
	-find /home/ning/idning-source/c/fuse-2.8.5 -name "*.c" >> /tmp/cscope.files
	-find /home/ning/idning-source/c/fuse-2.8.5 -name "*.h" >> /tmp/cscope.files
	#
	-find . -name "*.c" >> /tmp/cscope.files
	-find . -name "*.h" >> /tmp/cscope.files
	-cscope  -i /tmp/cscope.files -I/usr/include/ -R -b
	#
	-ctags -L /tmp/cscope.files -R
	#######################################################################################

gcov:
	# -o 指的是path. 垃圾.
	gcov client/mount.c -o client
	gcov client/osd_conn.c -o client
	gcov client/mds_conn.c -o client

run: all
	gnome-terminal $(ARG) -e "./cmgr/cmgr.out" 						-t "cmgr" 	--geometry=60x20-0+0 
	gnome-terminal $(ARG) -e "./mds/mds.out" 						-t "mds0" 	--geometry=120x20+0+000	
	gnome-terminal $(ARG) -e "./mds/mds.out -c etc/mds.cfg.1" 		-t "mds1" 	--geometry=120x20+0+300	
	gnome-terminal $(ARG) -e "./mds/mds.out -c etc/mds.cfg.2" 		-t "mds2" 	--geometry=120x20+0+900	&
	gnome-terminal $(ARG) -e "./mds/mds.out -c etc/mds.cfg.3" 		-t "mds3" 	--geometry=120x20+0+300	
	gnome-terminal $(ARG) -e "./mds/mds.out -c etc/mds.cfg.4" 		-t "mds4" 	--geometry=120x20+0+900	&
	#
	gnome-terminal $(ARG) -e "./osd/osd.out" 						-t "osd" 	--geometry=80x10-0+400 &
	gnome-terminal $(ARG) -e "./osd/osd.out -c etc/osd.cfg.2" 		-t "osd2" 	--geometry=80x10-0+400 &

rb: run-back
run-back:
	./cmgr/cmgr.out 2>/dev/null &
	./mds/mds.out 2>/dev/null &
	./mds/mds.out -c etc/mds.cfg.1 2>/dev/null &
	./mds/mds.out -c etc/mds.cfg.2 2>/dev/null &
	
	./osd/osd.out 2>/dev/null &
	./osd/osd.out -c etc/osd.cfg.2 2>/dev/null &

show-log:

#killall
killall: 
	-killall -q cmgr.out osd.out mds.out mount.out

kk:killall
	rm /mnt/hd1/* -rf 
	rm /mnt/hd2/* -rf  
	rm ./log/* -rf  
	rm ./data/* -rf  

indent:
	find . -name "*.c" | xargs indent -npro -kr -i4 -ts4 -sob -l120 -ss -ncs -cp1 --no-tabs
	find . -name "*.h" | xargs indent -npro -kr -i4 -ts4 -sob -l120 -ss -ncs -cp1 --no-tabs



