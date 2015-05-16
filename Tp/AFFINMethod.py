class AFIINMethod(object):
	""" """
	def __init__(self):
		self.hash_sentiment_word = { }
		
		# Proccesing sentiment_words
		sentiment_word_doc = open("AFINN-111.txt", "r")

		word_line = sentiment_word_doc.readline()
		while word_line:
			word_and_sentiment = word_line.split('\t')
			self.hash_sentiment_word[word_and_sentiment[0]] = int(word_and_sentiment[1])
			word_line = sentiment_word_doc.readline()	
		sentiment_word_doc.close()	
		
	""" This function calculate the sentiment of a review according to the sentiments of each word """
	def calculate_sentiment(self, word_list):
		review_score = 0
		
		for x in range(0, len(word_list) - 1):
			if self.hash_sentiment_word.has_key(word_list[x]):
				review_score += self.hash_sentiment_word[word_list[x]]	
			
		if review_score < 0:
			return 0
		else:
			return 1
		
