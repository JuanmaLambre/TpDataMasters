stopwordsFile = open("../stopWords", 'r')
stopwords = []
for line in stopwordsFile:
	stopwords.append( line[:len(line)-1] )
stopwords.remove('no')
stopwords.remove('not')
stopwords.remove('nor')
stopwords.remove('against')	

def removeStopwords(wordsList):
	newList = []
	for word in wordsList:
		if word not in stopwords:
			newList.append(word)
	return newList	
