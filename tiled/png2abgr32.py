import numpy as np
from PIL import Image
import sys
import argparse

# python png2abgr.py -i foo.png

parser = argparse.ArgumentParser(description='convert png image to abgr array')
parser.add_argument("-i", "--input", type=str, help="input PNG file")
parser.add_argument("-o", "--output", type=str, help="directory for storing output files")

args = parser.parse_args()

path, filename = "", args.input

if filename[-4:] != '.png':
    print 'ERROR: file does not have .png extension'
    quit()
    
# create a numpy array from the png file
img = Image.open(filename).convert('RGBA')
arr = np.array(img)
#img.show()

# create a list from the multi-dimensional numpy array
data=[]
for i in range(img.height):
    for j in range(img.width):
        if arr[i,j,3] == 0:
            arr[i,j,0] = 0
            arr[i,j,1] = 0
            arr[i,j,2] = 0
        r = arr[i,j,0]
        #r = int("{0:b}".format(r)[::-1], 2)
        g = arr[i,j,1]
        #g = int("{0:b}".format(g)[::-1], 2)
        b = arr[i,j,2]
        #b = int("{0:b}".format(b)[::-1], 2)
        a = (128-(arr[i,j,3]*128/255))
        x = (a<<24) + (b<<16) + (g<<8) + r
        # interestingly, the second approach causes transparency issues on a physical system,
        # that aren't seen on the emulator.
        #x += (255-arr[i,j,3])<<24 # A
        data.append(x)

if '/' in args.input:
    path, filename = args.input.split('/', -1)
        
#produce a header file
filename = filename[:-4]

h = open(args.output+filename+'.h', 'w') if args.output else open(filename+'.h', 'w')

h.write('#include <stdio.h>\n')
h.write('#ifndef '+filename.upper()+'_H\n')
h.write('#define '+filename.upper()+'_H\n')
h.write('\n')
h.write('extern u32 '+filename+'_array[];\n')
h.write('\n')
h.write('#endif\n')
h.close()

#produce a source file
c = open(args.output+filename+'.c', 'w') if args.output else open(filename+'.c', 'w')
c.write('#include "'+filename+'.h"\n')
abgr = '{'

for i in range(img.height):
    for j in data[i*img.width:(i+1)*img.width]:
        abgr+="0x%08x" % (j)
        abgr+=', '
    abgr+='\n'
    abgr+=' '*(29+len(filename))
        
    
abgr += '}'
c.write('u32 '+filename+'_array[] __attribute__((aligned(16))) = '+abgr+';\n')
c.close()
