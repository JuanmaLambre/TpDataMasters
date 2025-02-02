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
iteration = 0.0
acertados = 0.0
MAX_ITERATION = 25000.0
print "PREDICIENDO CON AFINN-111..."
while wholeReview and iteration < MAX_ITERATION:
	rawReview = wholeReview.split('\t')[2]
	reviewScores = ts.textScore(rawReview)
	reviewScoresWithID = (reviewScores[0], reviewScores[1], wholeReview.split('\t')[0])
	sentiment = bool( int(wholeReview.split('\t')[1]) )
	prediccion = (reviewScores[0] - reviewScores[1]) > 0
	acertados += prediccion == sentiment
	
	wholeReview = trainData.readline()
	iteration += 1
	
	sys.stdout.write("\r%d%%" % int(iteration*100/MAX_ITERATION))
print
	
print "ACERTADOS:", acertados*100/iteration, "%"
'''
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
	
'''
	
	
