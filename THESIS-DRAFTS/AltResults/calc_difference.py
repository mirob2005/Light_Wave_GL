import sys
import time

entries = []
for entry in sys.argv:
	entries.append(entry)

entries.reverse()
entries.pop()
entries.reverse()



inputFile1 = entries[0]
inputFile2 = entries[1]

print('Start Program', time.clock())
inFile1 = open(inputFile1, 'rb').read()

file1 = inFile1.decode()

file1Pixels = file1.partition('\n255\n')[2]
file1PixelList = file1Pixels.split()

inFile2 = open(inputFile2, 'rb').read()

file2 = inFile2.decode()

file2Pixels = file2.partition('\n255\n')[2]
file2PixelList = file2Pixels.split()

print('Files inputted', time.clock())

#Can add check whether the 2 files are the same resolution later...
maxPixels = 921600*3


newPixelList = []

pixel = 0
while pixel < maxPixels:
	if(int(file1PixelList[pixel]) > int(file2PixelList[pixel])):
		difference = (int(file2PixelList[pixel])+1)/(int(file1PixelList[pixel])+1)
	else:
		difference = (int(file1PixelList[pixel])+1)/(int(file2PixelList[pixel])+1)
	newPixelList.append(difference)
	pixel = pixel +1

print('Interpolation done', time.clock())

totalSimilar = 0.0;
pixel = 0
while pixel < maxPixels:
	totalSimilar += newPixelList[pixel]
	pixel = pixel +1

totalSimilar = totalSimilar/maxPixels

print('The images are: '+str(totalSimilar)+' similar')
print('The images are: '+str((1-totalSimilar))+' different')




