#!/usr/bin/scons -f
#coding:utf-8
import glob, os, subprocess, sys

common_src = glob.glob('common/*.c')
osd_src = glob.glob('osd/*.c')
mds_src = glob.glob('mds/*.c')
cmgr_src = glob.glob('cmgr/*.c')

client_src = glob.glob('client/*.c')
#client_src.append('/usr/lib/gcrt1.o')

test_c = glob.glob('test/*test.c')
tests = [s.replace('.c', '') for s in test_c]
#tests = ['network_test', 'dlist_test', 'protocol_test']
print tests

def system(cmd):
    from subprocess import Popen, PIPE
    p = Popen(cmd, shell=True, bufsize = 102400, stdout=PIPE)
    return p.wait()

def test():
    test_out = glob.glob('test/*.out')
    for i in test_out:
        #print '#####', i
        if 0 == system(i):
            sys.stdout.write('.')
        else:
            print 'Test error on : ', i
            return -1
    print '\nThank God, All Tests Are Passed!!!!!!'


LIBS = ['common', 'event', 'fuse', 'gcov']
LIBPATH = ['common',  '/usr/local/lib', '/usr/lib'] #顺序很重要

CPPPATH = ['common', '/usr/local/include/', '/usr/include/fuse']

CCFLAGS='-D_DEBUG -Wall -g -Wno-pointer-sign ' # -pg is for gprof  osd 不能用 -D_FILE_OFFSET_BITS=64
LINKFLAGS=' '

CCFLAGS='-D_DEBUG -Wall -g -Wno-pointer-sign -pg -fprofile-arcs -ftest-coverage' # -pg is for gprof
LINKFLAGS=' -pg '


def compile():
    Library('common/common', common_src, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS)

    for t in tests:
        target = '%s.out'%t
        src = '%s.c'%t
        #print target, src
        #usr/local/include/ for libevent2
        Program(target, [src], LIBS = LIBS, LIBPATH = LIBPATH, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS)
        #

    Program( 'osd/osd.out', osd_src, LIBS = LIBS, LIBPATH = LIBPATH, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS)
    Program( 'mds/mds.out', mds_src, LIBS = LIBS, LIBPATH = LIBPATH, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS)
    Program( 'cmgr/cmgr.out', cmgr_src, LIBS = LIBS, LIBPATH = LIBPATH, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS)

    Program( 'client/mount.out', client_src, LIBS = LIBS, LIBPATH = LIBPATH, CPPPATH = CPPPATH, CCFLAGS = CCFLAGS +' -D_FILE_OFFSET_BITS=64 ', LINKFLAGS=LINKFLAGS)


target = ARGUMENTS.get('ning_target', 'compile') #default target is 'compile'!!! haha

eval(target+'()')

