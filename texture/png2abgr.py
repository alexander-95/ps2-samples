import numpy as np
from PIL import Image
import sys

# python png2abgr.py foo.png

#parse args. remember, the file name is the first argument, second should be a png file
if len(sys.argv) != 2:
    print  'ERROR:',sys.argv[0],'takes exactly 1 argument,', len(sys.argv)-1,'given'
    quit()

filename = sys.argv[1]
if filename[-4:] != '.png':
    print 'ERROR: file does not have .png extension'
    quit()

# create a numpy array from the png file
img = Image.open(filename).convert('RGBA')
arr = np.array(img)

# create a list from the multi-dimensional numpy array
data=[]
for i in range(img.height):
    for j in range(img.width):
        x = 0
        x += arr[i,j,0]
        x += arr[i,j,1]<<8
        x += arr[i,j,2]<<16
        x += (255-arr[i,j,3])<<24
        data.append(x)

#produce a header file
filename = filename[:-4]

h = open(filename+'.h', 'w')

h.write('#include <stdio.h>\n')
h.write('#ifndef '+filename.upper()+'_H\n')
h.write('#define '+filename.upper()+'_H\n')
h.write('\n')
h.write('extern const u32 '+filename+'[];\n')
h.write('\n')
h.write('#endif\n')
h.close()

#produce a source file
c = open(filename+'.c', 'w')
c.write('#include "'+filename+'.h"\n')
abgr = '{'
for i in data:
    abgr+=str(i)
    abgr+=', '
abgr += '}'
c.write('extern const u32 '+filename+'[] = '+abgr+';\n')
c.close()
