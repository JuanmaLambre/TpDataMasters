import cleanText as cl
import probabilistic_method as proba
import probabilistic_methods_two_words as proba_two
import probabilistic_method_one_two_words as proba_one_two
import AFFINMethod as afin
#import perceptron
#import loggisticRegression as lg

review_file = "labeledTrainData.tsv"
POS_ID_REVIEW = 0
POS_REVIEW = 2

#proba_method = proba.ProbabilisticMethod()			 	ACA HAY 3 TIPOS DE METODOS DISTINTOS, PROBA ES PARA 1 PALABRA
proba_method = proba_two.ProbabilisticMethod()			#PROBA_TWO INCLUYE 1, 2 Y 3 PALABRAS PERO POR SEPARADO
#proba_method = proba_one_two.ProbabilisticMethod()		#PROBA_ONE_TWO INCLUTE 1 Y 2 PALABRAS COMBINADAS.
afin_method = afin.AFIINMethod()



"""Proccesing Reviews"""

file = open(review_file, 'r')

well_qualified_reviews = 0
qualified_reviews = 0

wholeReview = file.readline()
wholeReview = file.readline()
while wholeReview:
	wholeReviewSplit = wholeReview.split('\t')
	rawReview = wholeReviewSplit[POS_REVIEW]
	idReview = wholeReviewSplit[POS_ID_REVIEW]
	
	list_tokenized_words = cl.cleanText(rawReview) 							#List of words in review
	
	sentiment = proba_method.calculate_sentiment(list_tokenized_words) 		# Sentiment from Probabilistic Methods
	#sentiment_afin = afin_method.calculate_sentiment(list_tokenized_words)
	print sentiment

	if sentiment <> 0:
		if (sentiment > 0.5):
			if (wholeReviewSplit[1] == "1"):
				well_qualified_reviews += 1
		else:
			if (wholeReviewSplit[1] == "0"):
				well_qualified_reviews += 1
		qualified_reviews += 1

					
	wholeReview = file.readline()

print "Reviews bien calificadas: " + str(well_qualified_reviews)
print "Reviews calificadas: " + str(qualified_reviews)



				     #### Metodos Invidualmente ####

#  Palabras 	|	Bayes   |   Standar	  |	 Weight	 |  Weight abs  |

#	   1		|	0.904   |	 0.904	  |	 0.698   |	  0.763  	|
#	   2		|	0.968	|	 0.979	  |			 |				|
#	   3		|	0.892   |	 0.891	  |			 |				|
#  sent word	|	0.901   |	 0.834	  |	 0.740	 |	  0.769	    |
# 	 1 y 2   	|	0.952   |	 0.967	  |	    	 |	    	    |


# Aclaracion: Con 3 Palabras solo llega a procesar 19.000 reviews nomas y con 2 palabras 24.986



				#### Rangos de probabilidades de los metodos de 1 palabra ####

# Bayes:
#			0 < sentiment < 0.1 or 0.9 < sentiment < 1          ==>  0.9197  y califica 20.000 rev
#			0 < sentiment < 0.01 or 0.99 < sentiment < 1        ==>  0.943   y califica 19.000 rev
#			0 < sentiment < 0.001 or 0.999 < sentiment < 1      ==>  0.9564  y califica 17.000 rev
#			0 < sentiment < 0.0001 or 0.9999 < sentiment < 1    ==>  0.9650  y califica 16.000 rev
#			0 < sentiment < 0.00001 or 0.99999 < sentiment < 1  ==>  0.9701  y califica 13.000 rev
#
#			0 < sentiment < 0.001								 ==>  0.955  y califica 10.000 rev		
#			0 < sentiment < 0.0001								 ==>  0.9659 y califica  9000  rev	
#			0 < sentiment < 0.00001								 ==>  0.9731 y califica  8000  rev	
#
#			0.999 < sentiment < 1								 ==>  0.9580 y califica  7000  rev		
#			0.9999 < sentiment < 1								 ==>  0.9635 y califica  6000  rev	
#			0.99999 < sentiment < 1								 ==>  0.9683 y califica  5000  rev

# Standar:
#			0 < sentiment < 0.4 or 0.6 < sentiment < 1      ==>  0.9985  y califica 700 rev
#			0 < sentiment < 0.45 or 0.55 < sentiment < 1    ==>  0.9929  y califica 10.000 rev
#			0 < sentiment < 0.475 or 0.525 < sentiment < 1  ==>  0.9747  y califica 18.000 rev
#			0 < sentiment < 0.48 or 0.52 < sentiment < 1    ==>  0.9660  y califica 20.000 rev
#			0 < sentiment < 0.49 or 0.51 < sentiment < 1    ==>  0.942   y califica 22.000 rev
#
#			0 < sentiment < 0.45 						     ==>  0.9948  y califica 4000 rev
#			0 < sentiment < 0.475 						     ==>  0.9783  y califica 8000 rev
#			0 < sentiment < 0.48 						     ==>  0.9702  y califica 9000 rev
#			0 < sentiment < 0.49 						     ==>  0.952   y califica 11.000 rev
#
#			0.55 < sentiment < 1 						     ==>  0.9917  y califica 6000 rev
#			0.525 < sentiment < 1 						     ==>  0.9716  y califica 10.000 rev
#			0.52 < sentiment < 1 						     ==>  0.9624  y califica 10.000 rev
#			0.51 < sentiment < 1 						     ==>  0.9325  y califica 11.000 rev

#			0 < sentiment < 0.48 or 0.525 < sentiment < 1	 ==>  0.9709  y califica 19.000 rev   Probable a usar!
 
