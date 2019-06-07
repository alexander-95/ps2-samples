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
        if j % 2 == 0:
            arr[i,j,0] = arr[i,j,0]>>3
            arr[i,j,1] = arr[i,j,1]>>3
            arr[i,j,2] = arr[i,j,2]>>3
            arr[i,j,3] = arr[i,j,3]>>7

            arr[i,j+1,0] = arr[i,j+1,0]>>3
            arr[i,j+1,1] = arr[i,j+1,1]>>3
            arr[i,j+1,2] = arr[i,j+1,2]>>3
            arr[i,j+1,3] = arr[i,j+1,3]>>7

            x = 0
            r = arr[i,j,0]
            g = arr[i,j,1]
            b = arr[i,j,2]
            a = arr[i,j,3]
            pixel1 = r + (g<<5) + (b<<10) + (a<<15)
            if arr[i,j,3] == 0:
                pixel1 = 0

            r = arr[i,j+1,0]
            g = arr[i,j+1,1]
            b = arr[i,j+1,2]
            a = arr[i,j+1,3]
            pixel2 = r + (g<<5) + (b<<10) + (a<<15)
            if arr[i,j+1,3] == 0:
                pixel2 = 0
                        
            x = pixel1 + (pixel2<<16)
            data.append(x)
        else:
            continue


#produce a header file
filename = filename[:-4]

h = open(filename+'.h', 'w')

h.write('#include <stdio.h>\n')
h.write('#ifndef '+filename.upper()+'_H\n')
h.write('#define '+filename.upper()+'_H\n')
h.write('\n')
h.write('extern u32 '+filename+'_array[];\n')
h.write('\n')
h.write('#endif\n')
h.close()

#produce a source file
c = open(filename+'.c', 'w')
c.write('#include "'+filename+'.h"\n')
abgr = '{'

for i in range(img.height):
    for j in data[i*img.width/2:(i+1)*img.width/2]:
        abgr+="0x%08x" % (j)
        abgr+=', '
    abgr+='\n'
    abgr+=' '*(45+len(filename))
        
    
abgr += '}'
c.write('u32 '+filename+'_array[] __attribute__((aligned(16))) = '+abgr+';\n')
c.close()
