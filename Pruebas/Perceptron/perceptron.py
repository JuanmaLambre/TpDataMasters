import textScore as ts
import matplotlib.pyplot as plt
import sys


trainData = open("../labeledTrainData.tsv", 'r')
# Dismissing the header:
trainData.readline()

# scores[0] is a list for negative reviews (according to sentiment)
# scores[1] is a list for positive reviews
scores = [[], []]

# Getting the scores for each review:
wholeReview = trainData.readline()
iteration, porcentaje = 0, 0
MAX_ITERATION = 1000.0
while wholeReview and iteration < MAX_ITERATION:
	rawReview = wholeReview.split('\t')[2]
	reviewScores = ts.textScore(rawReview)
	reviewScoresWithID = (reviewScores[0], reviewScores[1], wholeReview.split('\t')[0])
	sentiment = int( wholeReview.split('\t')[1] )
	scores[sentiment].append(reviewScoresWithID)
	
	wholeReview = trainData.readline()
	iteration += 1
	
	porcentaje = int(iteration*100.0/MAX_ITERATION)
	sys.stdout.write("\r%d%%" % int(porcentaje))
	sys.stdout.flush()

	

# Preparo las variables necesarias para graficar:
positiveX, positiveY = [], []
negativeX, negativeY = [], []
for score in scores[1]:
	positiveX.append(score[0])
	positiveY.append(score[1])
for score in scores[0]:
	negativeX.append(score[0])
	negativeY.append(score[1])
	
	
# Detecting repeated spots:
positivePoints = zip(positiveX, positiveY)
negativePoints = zip(negativeX, negativeY)
repeatedPoints = filter(lambda x: x in positivePoints, negativePoints)
bothX, bothY = [], []
for point in repeatedPoints:
	bothX.append(point[0])
	bothY.append(point[1])
	
# Y ahora grafico los resultados. En azul estan los reviews positivos, en rojo
# los negativos y en violeta los puntajes que aparecen en ambos
plt.plot(positiveX, positiveY, 'bo')
plt.show()
plt.plot(negativeX, negativeY, 'ro')
plt.show()
plt.plot(bothX, bothY, 'mo')
plt.show()
plt.plot(positiveX, positiveY, 'bo', negativeX, negativeY, 'ro', bothX, bothY, 'mo')
plt.show()
