import matplotlib.pyplot as plt
import numpy as np


def wPorX(w, x):
	return (w[0] + w[1]*x[1] + w[2]*x[2])

def y(w, x):
	return ( (w[0], w[1]*x[1], w[2]*x[2]) )

def algunPuntoNoCalifica(w, dataSet):
	for x in dataSet:
		if ( wPorX(w,x) * x[3] < 0 ):
			return True
	return False


outputApuntes = (-29, 10, -1)

# dataSet[i] es un punto
dataSet = []
dataSetFile = open('hardcoded_train_set.txt', 'r')
currentLine = dataSetFile.readline()

# Cargo los datos en datSet
while ( currentLine ):
	vec = currentLine.split(' ')
	dataSet.append( (1, int(vec[0]), int(vec[1]), int(vec[2])) )
	currentLine = dataSetFile.readline()
	
w = (-19, 12, 3)

while algunPuntoNoCalifica(w, dataSet):
	for x in dataSet:
		if wPorX(w,x) * x[3] < 0:
			w = ( w[0]+x[3]*x[0], w[1]+x[3]*x[1], w[2]+x[3]*x[2] )

			
print "Output apuntes:", outputApuntes
print "Output calculado:", w

# Graphing the result w=(-28, 10, -1)
domain = np.arange(0.0, 8.0, 0.1)
def f(x):
	return -28.0/1.0 + 10.0/1.0 * x

xPositive, yPositive = [], []
yNegative, xNegative = [], []
for vec in dataSet:
	if (vec[3] > 0):
		xPositive.append(vec[1])
		yPositive.append(vec[2])
	else:
		xNegative.append(vec[1])
		yNegative.append(vec[2])

plt.plot(domain, f(domain), 'k', xPositive, yPositive, 'bo', xNegative, yNegative, 'ro')
plt.show()
			
			
			
