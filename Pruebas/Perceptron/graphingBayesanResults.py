import matplotlib.pyplot as plt 


bayesianResults = open("../../Tp/OneWordsSentiment.txt", 'r')
xCoords = []
yCoords = []

# Dismissing header:
bayesianResults.readline()

totalWords = 44853.0 # Totalmente hardcodeado
currentWord = 1.0
currentLine = bayesianResults.readline()
while currentLine:
	xCoords.append( int(currentLine.split('\t')[1]) )
	yCoords.append( int(currentLine.split('\t')[2]) )
	print int( currentWord*100.0/totalWords )
	currentWord += 1
	currentLine = bayesianResults.readline()
	

plt.plot(xCoords, yCoords, 'mo')
plt.show()
