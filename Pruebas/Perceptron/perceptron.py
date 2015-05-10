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
dataSetFile = open('train_set.txt', 'r')
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
			
			
			
