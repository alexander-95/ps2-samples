import binascii

# read icn into a hex-dump string
with open('icon.icn', 'rb') as f:
    content = f.read()
content = binascii.hexlify(content)
print len(content)
icn_size = len(content)/2

# generate icon.c
output = '#include \"icon.h\"\n'

# generate byte array from icn hex-dump
icn = 'extern const u8 icn_array[] = {'
for i in range(0, len(content),2):
    icn += '0x'+content[i:i+2].upper()+', '
icn = icn[:-2]
icn += "};\n"
output += icn

# read sys into a hex-dump string
with open('icon.sys', 'rb') as f:
    content = f.read()
content = binascii.hexlify(content)
print len(content)
sys_size = len(content)/2

# generate byte array from sys hex-dump
sys = 'extern const u8 sys_array[] = {'
for i in range(0, len(content),2):
    sys += '0x'+content[i:i+2].upper()+', '
sys = sys[:-2]
sys += "};"
output += sys

output += 'extern const u32 icn_size = '+str(icn_size)+';\n'
output += 'extern const u32 sys_size = '+str(sys_size)+';\n\n'

c = open('icon.c', 'w')
c.write(output)
c.close()

output = '#include <stdio.h>\n\n'
output += '#ifndef ICON_H\n'
output += '#define ICON_H\n\n'
output += 'extern const u8 icn_array[];\n'
output += 'extern const u8 sys_array[];\n\n'
output += 'extern const u32 icn_size;\n'
output += 'extern const u32 sys_size;\n\n'
output += '#endif'

h = open('icon.h', 'w')
h.write(output)
h.close()
