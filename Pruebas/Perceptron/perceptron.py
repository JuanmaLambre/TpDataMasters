import textScore as ts
import matplotlib.pyplot as plt


trainData = open("../labeledTrainData.tsv", 'r')
# Dismissing the header:
trainData.readline()

# scores[0] is a list for negative reviews (according to sentiment)
# scores[1] is a list for positive reviews
scores = [[], []]

# Getting the scores for each review:
wholeReview = trainData.readline()
iteration = 0
while wholeReview and iteration < 2000:
	rawReview = wholeReview.split('\t')[2]
	reviewScores = ts.textScore(rawReview)
	sentiment = int( wholeReview.split('\t')[1] )
	scores[sentiment].append(reviewScores)
	
	wholeReview = trainData.readline()
	iteration += 1
	

positiveX, positiveY = [], []
negativeX, negativeY = [], []

for score in scores[0]:
	positiveX.append(score[0])
	positiveY.append(score[1])
	
for score in scores[1]:
	negativeX.append(score[0])
	negativeY.append(score[1])
	
plt.plot(positiveX, positiveY, 'bo', negativeX, negativeY, 'ro')
plt.show()
