import cleanText as ct
import removeStopwords as rm
import sys


def vectorizeReview(rawText):
	rawText = rawText.replace("'", '')
	clean = ct.cleanText(rawText)
	return rm.removeStopwords(clean)
		
	
def productoInterno(x, y):
	result = 0
	for i in range(len(x)):
		result += x[i]*y[i]
	return result
	
	
def contarMalCalificados(w, reviewsPos, reviewsNeg):
	malCalificados = 0
	for review in reviewsPos:
		result = productoInterno(w, [1]+reviewsVecs[review])
		if result <= 0:
			malCalificados += 1
	for review in reviewsNeg:
		result = productoInterno(w, [1]+reviewsVecs[review])
		if result > 0:
			malCalificados += 1
	return malCalificados
	

#if __name__ == "__main__":
labeled = open("../labeledTrainData.tsv", 'r')
labeled.readline()

# Hash: reviewsWordsFrec[word] -> [ (review_id,frec_word) ]
reviewsWordsFrec = {}
# Hash: reviews[id] -> [ frec_word0 , frec_word1 , ... , frec_wordn ]
#		siendo 'n' el tamanio de reviewsWordsFrec
reviewsPos = {}
reviewsNeg = {}

# Proceso todo el archivo:
print "PRE PROCESANDO..."
line = labeled.readline()
iteracion = 0.0
MAX_REVIEWS = 50
while line:
	wordsFrec = {}
	reviewText = vectorizeReview(line.split('\t')[2])
	reviewId = line.split('\t')[0]
	if line.split('\t')[1] == '1':
		reviewsPos[reviewId] = None
	else:
		reviewsNeg[reviewId] = None
	
	# Cuento las frecuencias de cada palabra DEL REVIEW:
	for word in reviewText:
		wordsFrec[word] = 1 + wordsFrec.get(word, 0)
	# Y agrego las frecuencias al "contador universal" (hash reviewsWordsFrec)
	for word in wordsFrec:
		reviewsWordsFrec[word] = reviewsWordsFrec.get(word,[])
		reviewsWordsFrec[word].append( (reviewId,wordsFrec[word]) )
	
	line = labeled.readline()
	iteracion += 1
	sys.stdout.write( "\r%d%%" % int(iteracion*100/MAX_REVIEWS) )
	if iteracion == MAX_REVIEWS:
		break
print


# Ahora tengo los datos preprocesados.
# Vamos a hacer una lista de vectores. Un vector es un review, y una coordenada es la frecuencia de la palabra
for review in reviews:
	reviews[review] = [ 0 for x in range(len(reviewsWordsFrec)) ]

print "PROCESANDO..."
palabraNumero = 0
for word in reviewsWordsFrec:
	for freqTuple in reviewsWordsFrec[word]:
		reviews [freqTuple[0]] [palabraNumero] = freqTuple[1]
	palabraNumero += 1
	sys.stdout.write("\r%d%%" % int( palabraNumero*100.0/len(reviewsWordsFrec) ))
print
		

# Ahora tengo todos los puntos. Perceptron time!
print "ENTRENANDO PERCEPTRON... (puede volver a cero)"
w = [ 1 for x in range(len(reviewsWordsFrec)+1) ]
iteracion += 0.0
minimoMalCalificados = contarMalCalificados(w, reviewsPos, reviewsNeg)
while iteracion < len(reviewsWordsFrec):
	actualizarW(w, 
	
	malCalificados(posibleW, reviewsPos, reviewsNeg)
	if posibleW != w and minimoMalCalificados > malCalificados:
		posibleW = w
		iteracion = 0.0
	else:
		iteracion += 1
	
	sys.stdout.write( "\r%d%%" % int(iteracion*100/len(reviewsWordsFrec)) )



	
	
	
	
	
	
	
	
	
	

