#!/usr/bin/python
#coding:utf-8
#----------------------------------
#只支持两种数据类型 uint32_t, uint8_t*

class StringWriter:
    def __init__(self):
        self.buf = ''
    def append(self, s):
        self.buf = self.buf + s + '\n'
    def append1(self, s):
        self.buf = self.buf + '\t' + s + '\n'
    def append2(self, s):
        self.buf = self.buf + '\t\t' + s + '\n'
    def __str__(self):
        return self.buf
# end class StringWriter 
import re, random

# gen pack method
def pack_method(name, fields):
    header = 'int %s_pack(struct %s * s, uint8_t * data, uint32_t len)' % (name, name)
    content = StringWriter()
    content.append('{')
    has_msglength = [i[1] for i in fields] .count('msglength') >=1


    content.append1('uint8_t * p = data;')
    if has_msglength:
        content.append1("s->msglength = 88888888;") # big enouth
    for t, n in fields:
        if t == 'uint32_t':
            content.append1('put32bit(&p, s -> %s);' % n )
        elif t == 'uint8_t*':
            content.append1('if (s->%slength <= 0)' % (n) )
            content.append1('   s->%slength = strlen((char *)s->%s);' %(n, n) )
            content.append1('putstr(&p, s -> %slength, s -> %s);' % (n, n) )
        elif t == '/*arr*/': #TODO
            (cls, arr) = n.split('*'); #class, arr_name= ...
            cls = cls.strip()
            arr = arr.strip()
            content.append1('int i;')
            content.append1('for(i=0; i<s->%slength; i++){' % (arr))
            content.append1('   int plen = %s_pack(s->%s+i, p, 0);' % (cls, arr))
            content.append1('   p += plen;' )
            content.append1('}')

    if has_msglength:
        content.append1("s->msglength = p-data;")
        content.append1("uint8_t *p2 = data ;")
        content.append1("put32bit_width(&p2, s->msglength, 8);")

    content.append1("return p-data;")
    content.append("}")
    return (header, str(content))

# gen unpack method
def unpack_method(name, fields):
    header = 'int %s_unpack(struct %s * s, const uint8_t * data, uint32_t len)' % (name, name)
    content = StringWriter()
    content.append('{')
    content.append1('const uint8_t* orig = data;')

    for t, n in fields:
        if t == 'uint32_t':
            content.append1('s -> %s = get32bit(&data);' % n)
        elif t == 'uint8_t*':
            content.append1('s -> %s = getstr(&data, s -> %slength);' % (n, n))

        elif t == '/*arr*/': #TODO
            (cls, arr) = n.split('*'); #class, arr_name= ...
            cls = cls.strip()
            arr = arr.strip()

            content.append1('int i;')
            content.append1('s->%s = (%s *) malloc (sizeof(%s) * s->%slength);' % (arr, cls, cls, arr))
            content.append1('for(i=0; i<s->%slength; i++){' % (arr))
            content.append1('   int plen = %s_unpack(s->%s+i, data, 0);' % (cls, arr))
            content.append1('   data += plen;' )
            content.append1('}')
    content.append1("return data-orig;")
    content.append("}")
    return (header, str(content))

def tostring_method(name, fields):
    header = 'char* %s_tostring(struct %s * s)' % (name, name)
    content = StringWriter()
    content.append('{')
    content.append1('static char str[10240];')
    content.append1('char * ptr = str;')
    for t, n in fields:
        if t == 'uint32_t':
            content.append1(r'sprintf(ptr, "\t%s = %d\n",'+' "%s", s->%s);' % (n, n) )
            content.append1('while (*ptr) ptr++;' )
        elif t == 'uint8_t*':
            content.append1(r'sprintf(ptr, "\t%s = %s\n",'+' "%s", s->%s);' % (n, n) )
            content.append1('while (*ptr) ptr++;' )

    content.append1("return str;")
    content.append("}")
    return (header, str(content))

def new_method(name, fields):
    header = '%s * %s_new()' % (name, name)
    content = StringWriter()
    content.append('{')
    content.append1('%s * p = (%s * )malloc (sizeof(%s));' % (name, name, name))
    content.append('''
    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    ''')
    msg_name = 'MSG_' + name.upper()

    names = [i[1] for i in fields]
    if names.count('operation') >=1 :
        content.append1("p->operation = %s ;" % msg_name)
    content.append1("return p;")
    content.append("}")
    return (header, str(content))

def free_method(name, fields):
    header = 'void %s_free(%s * s)' % (name, name)
    content = StringWriter()
    content.append('{')

    for t, n in fields:
        if t == 'uint8_t*':
            content.append1('free( s -> %s);' % (n) )
        elif t == '/*arr*/': #TODO
            (cls, arr) = n.split('*'); #class, arr_name= ...
            cls = cls.strip()
            arr = arr.strip()
            content.append1('free( s -> %s);' % (arr) )
    content.append1('free(s);')

    content.append("}")
    return (header, str(content))

def random_int():
    return random.randint(2, 500) 

# gen test method
def test_method(name, fields):
    header = 'void %s_test()' % (name)
    content = StringWriter()
    content.append('{')

    content.append1('%s s1;' % name) # mc_mkdir_request s1
    content.append1('%s s2;' % name) # mc_mkdir_request s2
    content.append1('uint8_t buffer [10000];') 
    for t, n in fields:
        if t == 'uint32_t':
            content.append1('s1.%s = %d;' % (n, random_int()) )
        elif t == 'uint8_t*':
            content.append1('s1.name = (uint8_t *)randomstring;')
    msg_name = 'MSG_' + name.upper()
    content.append1('s1.operation = %s;' % msg_name)
    
    content.append1('%s_pack(&s1, buffer, 100);' % name)
    content.append1('%s_unpack(&s2, buffer, 100);' % name)

    for t, n in fields:
        if t == 'uint32_t':
            content.append1('assert(s1.%s == s2.%s);' % (n, n))
        elif t == 'uint8_t*':
            content.append1('assert(strncmp((const char *)s1.%s, (const char *)s2.%s, s1.%slength) == 0);' % (n, n, n))
    content.append("}\n")
    return (header, str(content))


def test_main_method(struct_names):
    header = 'int main()'
    content = StringWriter()
    content.append('{')
    for name in struct_names:
        content.append1('%s_test();' % name) # mc_mkdir_request s1
    content.append1("return 0;")
    content.append("}\n")
    return (header, str(content))

def constant(struct_names):
    content = StringWriter()
    content.append('')
    i = 100;
    for name in struct_names:
        content.append('#define MSG_%s %d'%(name.upper(), i))
        i = i+1
    return str(content)

protocol_h_templete_file = 'common/protocol.input.h'
protocol_h_file = 'common/protocol.gen.h'
protocol_c_file = 'common/protocol.gen.c'
protocol_test_c_file = 'test/protocol_test.gen.c'

protocol_c_file_header = """
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "protocol.gen.h"

"""

protocol_test_c_file_header = """
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "test.h"
char * randomstring="01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890"; 
"""

def file_clean(fname):
    f = file(fname, 'w') 
    f.write('') 
    f.close() 

def file_append(fname, statement):
    f = file(fname, 'a') 
    f.write(statement) 
    f.close() 

def deal_struct(name, fields):
    print 'struct', name
    #fields = re.sub('/\*.*?\*/', '', fields) #del comments
    fields = re.sub('//.*\n', '', fields) #del comments

    fields = fields.split(';')
    fields = [f.strip() for f in fields]
    fields = parse_fields(fields)

    for f in fields:
        print '\t', f 
    (head, body) = pack_method(name, fields)
    file_append(protocol_h_file, head+';\n')
    file_append(protocol_c_file, head+body)

    (head, body) = unpack_method(name, fields)
    file_append(protocol_h_file, head+';\n')
    file_append(protocol_c_file, head+body)

    (head, body) = tostring_method(name, fields)
    file_append(protocol_h_file, head+';\n')
    file_append(protocol_c_file, head+body)

    (head, body) = new_method(name, fields)
    file_append(protocol_h_file, head+';\n')
    file_append(protocol_c_file, head+body)

    (head, body) = free_method(name, fields)
    file_append(protocol_h_file, head+';\n')
    file_append(protocol_c_file, head+body)

    file_append(protocol_h_file, '\n')

    (head, body) = test_method(name, fields)
    file_append(protocol_test_c_file, head+body)

def parse_fields(fields):
    print fields
    rst = []
    for f in fields:
        if re.search('uint32_t', f):
            n = re.sub('uint32_t', '', f).strip()
            rst.append(('uint32_t', n))

        elif re.search(r'/\*arr\*/', f):
            n = re.sub(r'/\*arr\*/', '', f).strip()
            rst.append(('/*arr*/', n))

        elif re.search('uint8_t *\*', f):
            n = re.sub('uint8_t *\*', '', f).strip()
            rst.append(('uint8_t*', n))
    return rst

def init(input_file):
    file_clean(protocol_h_file)
    file_clean(protocol_c_file)
    file_clean(protocol_test_c_file)

    file_append(protocol_h_file, input_file)
    file_append(protocol_c_file, protocol_c_file_header)
    file_append(protocol_test_c_file, protocol_test_c_file_header)

def main():
    input_file = open(protocol_h_templete_file, 'rb').read()
    init(input_file)
    input_file = input_file.split("/*below_is_auto_generated*/")[0]
    structs = re.findall(r'typedef struct (.*){(.*?)}(\1)', input_file, re.DOTALL)## 
    for struct in structs:
        name = struct[0]
        fields = struct[1]
        fields = fields.split('/*--')[0]
        #print fields
        deal_struct(name, fields)

    struct_names = [s[0] for s in structs]
    (head, body) = test_main_method(struct_names)
    file_append(protocol_test_c_file, head+body)
    
    file_append(protocol_h_file, constant(struct_names))
    
    print 'generate ', protocol_h_file
    print 'generate ', protocol_c_file
    print 'generate ', protocol_test_c_file

if __name__ == '__main__':
    main()
