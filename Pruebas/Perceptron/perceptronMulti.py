#
#	ACLARACION: En realidad aca no estoy haciendo Perceptron, simplemente estoy
#	categorizando con la recta y=x a fin de simplificar las cosas.
#
#	A diferencia de perceptron.py este codigo grafica
#	(cant_reviews_pos; cant_revies_neg) por cada par de palabra que exista
#	IMPORTANTE: A difrenecia de perceptron simple (con una palabra), aca no hay
#	un archivo con todos los puntajes de cada palabra. Esto lo creamos nosotros
#	con el archivo de entrenamiento mismo.
#

import cleanText as ct
import matplotlib.pyplot as plt
import sys
print "----------"

# Primero que todo, creo un hash con los puntajes de cada shingle

#	Dado una lista de palabras devuelve una lista de shingles de 'multi' palabras
def joinMultiShingles(multi, wordsList):
	shingles = []	
	for i in range(len(wordsList)-multi+1):
		# Uno las palabras a shingle:
		shingle = ""
		for j in range(multi):
			shingle += wordsList[i+j] + " "
		# Remuevo el ultimo espacio
		shingle = shingle[:len(shingle)-1]
		shingles.append( shingle )
		
	return shingles
	
	
puntajes = {}
labeled = open("../labeledTrainData.tsv", 'r')
labeled.readline()

#	Obtengo una lista de tuplas ([shingles], sentiment), una por cada review
MULTI = 5 # PARAMETRO
print "MULTI =", MULTI
reviewsProcesadas = []
MAX_ITERACIONES = 20000.0
iteracion = 0.0
line = labeled.readline()
print "ENTRENANDO..."
while line and iteracion < MAX_ITERACIONES:
	reviewVec = line.split('\t')
	rawReview = reviewVec[2]
	cleanReview = ct.cleanText(rawReview)
	shingles = joinMultiShingles(MULTI, cleanReview)
	sentiment = int( reviewVec[1] )
	
	reviewsProcesadas.append( (shingles,sentiment) )
	line = labeled.readline()
	
	iteracion += 1
	sys.stdout.write("\r%d%%" % int( iteracion*100/MAX_ITERACIONES ))
	sys.stdout.flush()
print

print iteracion, "reviews entrenados"

#	Ahora lleno el hash puntajes
for review in reviewsProcesadas:
	sumaPositivo = review[1]
	sumaNegativo = not sumaPositivo
	for shingle in review[0]:
		frecuencias = puntajes[shingle] = puntajes.get(shingle, (0,0))
		frecActualizada = (frecuencias[0]+sumaNegativo, frecuencias[1]+sumaPositivo)
		puntajes[shingle] = frecActualizada

# Obtenidos los puntajes ahora voy a calificar los siguientes reviews y ver
# que tan bien o mal califico

correctos, incorrectos = 0.0, 0.0
iteracion = 0.0
MAX_PRUEBAS = 4900.0
line = labeled.readline()
print "PREDICIENDO..."
while line and iteracion < MAX_PRUEBAS:
	rawReview = line.split('\t')[2]
	cleanReview = ct.cleanText(rawReview)
	shingles = joinMultiShingles(MULTI, cleanReview)
	puntajePos, puntajeNeg = 0, 0
	for sh in shingles:
		puntajeShingle = puntajes.get(sh, (0,0))
		puntajePos += puntajeShingle[1]
		puntajeNeg += puntajeShingle[0]
	if (puntajePos != puntajeNeg):
		sentimientoPosta = int( line.split('\t')[1] )
		prediccion = (puntajePos > puntajeNeg)
		correctos += (prediccion == sentimientoPosta)
		incorrectos += (prediccion != sentimientoPosta)
		
	line = labeled.readline()
	iteracion += 1
	sys.stdout.write("\r%d%%" % int(iteracion*100/MAX_PRUEBAS))
print

print "\nCORRECTOS:", correctos*100/(correctos+incorrectos), "%"
print "EVALUADOS:", correctos+incorrectos, "out of", iteracion, " (", (correctos+incorrectos)*100/iteracion, " % )\n"


# Aca voy a graficar los reviews que quedaron sin entrenar para ver si hay una
# separacion lineal interesante
'''
iteracion, MAX_GRAFICOS = 0.0, 4900.0
missed = 0
reviewsPosX, reviewsPosY, reviewsNegX, reviewsNegY = [], [], [], []
line = labeled.readline()
print "\nGRAFICANDO..."
while line and iteracion<MAX_GRAFICOS:
	reviewVec = line.split('\t')
	rawReview = reviewVec[2]
	cleanReview = ct.cleanText(rawReview)
	shingles = joinMultiShingles(MULTI, cleanReview)
	puntajePos, puntajeNeg = 0, 0
	for sh in shingles:
		puntajeShingle = puntajes.get(sh, (0,0))
		puntajePos += puntajeShingle[1]
		puntajeNeg += puntajeShingle[0]
	if (puntajePos == puntajeNeg):
		missed += 1
	if (int(reviewVec[1])):
		reviewsPosX.append(puntajePos)
		reviewsPosY.append(puntajeNeg)
	else:
		reviewsNegX.append(puntajePos)
		reviewsNegY.append(puntajeNeg)
	
	line = labeled.readline()
	iteracion += 1
	sys.stdout.write( "\r%d%%" % int(iteracion*100/MAX_GRAFICOS) )
	sys.stdout.flush()
print

# Detecto puntos repetidos:
positivePoints = zip(reviewsPosX, reviewsPosY)
negativePoints = zip(reviewsNegX, reviewsNegY)
repeatedPoints = filter(lambda x: x in positivePoints, negativePoints)
bothX, bothY = [], []
for point in repeatedPoints:
	bothX.append(point[0])
	bothY.append(point[1])

print
print "MISSED REVIEWS:", missed, "out of", iteracion, " (", missed*100/iteracion, "% )"
plt.plot(reviewsPosX, reviewsPosY, 'bo', reviewsNegX, reviewsNegY, 'ro', bothX, bothY, 'mo')
plt.show()

		
'''
print "----------"
