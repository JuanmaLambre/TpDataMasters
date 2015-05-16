import cleanText as cl
import probabilistic_method as proba
import AFFINMethod as afin
#import perceptron
#import loggisticRegression as lg

review_file = "labeledTrainData.tsv"
POS_ID_REVIEW = 0
POS_REVIEW = 2
proba_method = proba.ProbabilisticMethod()
afin_method = afin.AFIINMethod()

"""Proccesing Reviews"""

file = open(review_file, 'r')

well_qualified_reviews = 0
wholeReview = file.readline()
wholeReview = file.readline()
while wholeReview:
	wholeReviewSplit = wholeReview.split('\t')
	rawReview = wholeReviewSplit[POS_REVIEW]
	idReview = wholeReviewSplit[POS_ID_REVIEW]
	
	list_tokenized_words = cl.cleanText(rawReview) #List of words in review
	
	sentiment = proba_method.calculate_sentiment(list_tokenized_words) # Sentiment from Probabilistic Methods
	#sentiment_afin = afin_method.calculate_sentiment(list_tokenized_words)
	print sentiment
	if sentiment <> 0:
		if (sentiment > 0.5):
			if (wholeReviewSplit[1] == "1"):
				well_qualified_reviews += 1
		else:
			if (wholeReviewSplit[1] == "0"):
				well_qualified_reviews += 1
					
	wholeReview = file.readline()

print well_qualified_reviews
