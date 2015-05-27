# *************************************************************************** #
#																			  #
#	This file contains a function that recieves a completly raw text and	  #
#	returns a tuple score (positive, negative) based on the file			  #
#	AFINN-111.txt from this same directory.									  #
#	Notices that it uses cleanText.py. The first lines prepare variables	  #
#	used in the function textScore(text)									  #
#																			  #
# *************************************************************************** #

import cleanText as cl

scoresFile = open('AFINN-111.txt', 'r')
scores = {}

# Prepare the 'scores' dictionary from scoresFile
fileLine = scoresFile.readline()
while fileLine:
	if len( fileLine.split() ) < 3:
		word = fileLine.split()[0]
		score = int( fileLine.split()[1] )
		scores[word] = score
	fileLine = scoresFile.readline()


# Recieves a whole paragraph (as string) as returns a score tuple
#	(positive, negative) based on AFINN-111 file. The value 'negative from
#	the tuple is allways above 0
def textScore(text):
	negativeScore, positiveScore = 0, 0
	wordsVec = cl.cleanText(text)
	for word in wordsVec:
		wordScore = scores.get(word, 0)
		if wordScore > 0:
			positiveScore += wordScore
		else:
			negativeScore -= wordScore
		
	return (positiveScore, negativeScore)
	
	
	
	
	
	
