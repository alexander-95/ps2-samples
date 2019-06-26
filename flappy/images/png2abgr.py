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
        if arr[i,j,3] == 0:
            arr[i,j,0] = 0
            arr[i,j,1] = 0
            arr[i,j,2] = 0
        x = 0
        x += arr[i,j,0]                     # B
        x += arr[i,j,1]<<8                  # G
        x += arr[i,j,2]<<16                 # R
        x += (128-(arr[i,j,3]*128/255))<<24 # A
        # interestingly, the second approach causes transparency issues on a physical system,
        # that aren't seen on the emulator.
        #x += (255-arr[i,j,3])<<24 # A
        data.append(x)

#produce a header file
filename = filename[:-4]

h = open(filename+'.h', 'w')

h.write('#include <stdio.h>\n')
h.write('#ifndef '+filename.upper()+'_H\n')
h.write('#define '+filename.upper()+'_H\n')
h.write('\n')
h.write('extern const u32 '+filename+'_array[];\n')
h.write('\n')
h.write('#endif\n')
h.close()

#produce a source file
c = open(filename+'.c', 'w')
c.write('#include "'+filename+'.h"\n')
abgr = '{'

for i in range(img.height):
    for j in data[i*img.width:(i+1)*img.width]:
        abgr+="0x%08x" % (j)
        abgr+=', '
    abgr+='\n'
    abgr+=' '*(29+len(filename))
        
    
abgr += '}'
c.write('extern const u32 '+filename+'_array[] = '+abgr+';\n')
c.close()
