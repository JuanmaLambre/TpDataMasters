###############################################################################
#
#	Este algoritmo inventado agarra los resultados del metodo bayesiano
#	y por cada review suma los puntajes positivos y negativos de cada palabra.
#	Si el puntaje de todas las palabras es mas positivo que negativo entonces
#	predice que el review es positivo
#
###############################################################################


import matplotlib.pyplot as plt 


bayesianResults = open("../../Tp/OneWordsSentiment.txt", 'r')
wordsHash = {}

# Dismissing header:
bayesianResults.readline()

# Obtengo los resultados bayesianos:
currentLine = bayesianResults.readline()
while currentLine:
	word = currentLine.split('\t')[0]
	posCoord = int(currentLine.split('\t')[1])
	negCoord = int(currentLine.split('\t')[2])
	wordsHash[word] = (posCoord, negCoord)
	currentLine = bayesianResults.readline()
	
# Los paso a listas para poder graficarlos:
xCoords, yCoords = [], []
for word in wordsHash:
	xCoords.append(wordsHash[word][0])
	yCoords.append(wordsHash[word][1])

# Y grafico:
plt.plot(xCoords, yCoords, 'mo')
plt.show()


# Ahora voy a predecir los sentimientos de los reviews the labeledTrainData
#	y comparo el resultado con la posta
labeled = open("../labeledTrainData.tsv", 'r')
labeled.readline()

reviewLine = labeled.readline()
reviewsTotales, reviewsAcertados = 0.0, 0.0
print ">>> Some examples: <<<"
while reviewLine:
	reviewText = reviewLine.split('\t')[2]
	realSentiment = int( reviewLine.split('\t')[1] )
	
	puntajePositivo, puntajeNegativo = 0, 0
	for word in reviewText:
		contadores = wordsHash.get(word, (0,0))
		puntajePositivo += contadores[0]
		puntajeNegativo += contadores[1]
		
	predictedSentiment = puntajePositivo > puntajeNegativo
	
	reviewsAcertados += predictedSentiment == realSentiment
	reviewLine = labeled.readline()
	reviewsTotales += 1
	
	if len(reviewText.split()) < 15:
		print "---------"
		print reviewText
		print "Puntaje +:", puntajePositivo
		print "Puntaje -:", puntajeNegativo
		print "PREDICTED:", predictedSentiment
		print "REAL:", realSentiment
		print "---------"


print "ACERTADOS:", reviewsAcertados*100/reviewsTotales, "%"


