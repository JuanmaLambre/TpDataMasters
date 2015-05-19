class ProbabilisticMethod(object):
	
	"""The initialize take the words from the documents into four hashes that contains 1, 2, 3
	   words or sentiment words"""
	def __init__(self):
		self.hash_one_word = { }
		self.hash_two_word = { }
		
		self.total_freq_pos_one_word = 0
		self.total_freq_neg_one_word = 0
		
		self.total_freq_pos_two_word = 0
		self.total_freq_neg_two_word = 0
		
		
		#### Proccesing one word ####
		one_word_doc = open("OneWordsSentiment.txt", "r")

		word_line = one_word_doc.readline()
		word_line = one_word_doc.readline()
		while word_line:
			one_word_and_sentiment = word_line.split('\t')
			self.hash_one_word[one_word_and_sentiment[0]] = (float(one_word_and_sentiment[1]), float(one_word_and_sentiment[2]))
					
			self.total_freq_pos_one_word += float(one_word_and_sentiment[1]) # Quantity of good words
			self.total_freq_neg_one_word += float(one_word_and_sentiment[2]) # Quantity of bad words

			word_line = one_word_doc.readline()	
		one_word_doc.close()
		
		
		#### Proccesing two words ####
		two_word_doc = open("TwoWordsSentiment.txt", "r")

		word_line = two_word_doc.readline()
		word_line = two_word_doc.readline()
		while word_line:
			two_word_and_sentiment = word_line.split('\t')
			self.hash_two_word[(two_word_and_sentiment[0], two_word_and_sentiment[1])] = (float(two_word_and_sentiment[2]), float(two_word_and_sentiment[3]))
			
			self.total_freq_pos_two_word += float(two_word_and_sentiment[2]) # Quantity of good words
			self.total_freq_neg_two_word += float(two_word_and_sentiment[3]) # Quantity of bad words
			
			word_line = two_word_doc.readline()	
		two_word_doc.close()	
		
		
		
	""" This function calculate the sentiment of a review. It receive a word list of the review and
		calculates the probability using 5 methods:
		*The standard method
		*The Bayes method
		You can choose one or all this method
		You can choose to work with one, two, three words or sentiments words"""
	def calculate_sentiment(self, word_list):
		
		#### Variables one and two word methods ####
		prob_standard_method = 0
		divisor_standard_method = 0

		prob_bayesian_method_pos = 1
		prob_bayesian_method_neg = 1


		division_de_los_metodos = len(self.hash_one_word) + len(self.hash_two_word) + self.total_freq_pos_one_word + self.total_freq_pos_two_word
		for x in range(0, len(word_list) - 1):
				
			inabilitar_siguiente = 0
			
			#### Proccesing two words ####
			if x < len(word_list) - 2:
				two_words = (word_list[x], word_list[x + 1])
				if self.hash_two_word.has_key(two_words):
					inabilitar_siguiente = 1
					
					quant_reviews = self.hash_two_word[two_words][0] + self.hash_two_word[two_words][1]
					
					"""Standar Method"""
					prob_standard_method += self.hash_two_word[two_words][0] / quant_reviews
					divisor_standard_method += 1
					
					"""Bayesian Method"""
					prob_bayesian_method_pos *= (self.hash_two_word[two_words][0] + 1) / division_de_los_metodos
					prob_bayesian_method_neg *= (self.hash_two_word[two_words][1] + 1) / division_de_los_metodos
					if prob_bayesian_method_pos < 10 ** (-100) or prob_bayesian_method_neg < 10 ** (-100):
						prob_bayesian_method_pos *= 10 ** (100)
						prob_bayesian_method_neg *= 10 ** (100)
						
						
				#### Proccesing one word ####
				elif inabilitar_siguiente == 0:
					if self.hash_one_word.has_key(word_list[x]):
						quant_reviews = self.hash_one_word[word_list[x]][0] + self.hash_one_word[word_list[x]][1]
					
						"""Standar Method"""
						prob_standard_method += self.hash_one_word[word_list[x]][0] / quant_reviews
						divisor_standard_method += 1
						
						"""Bayesian Method"""
						prob_bayesian_method_pos *= (self.hash_one_word[word_list[x]][0] + 1) / division_de_los_metodos
						prob_bayesian_method_neg *= (self.hash_one_word[word_list[x]][1] + 1) / division_de_los_metodos
						if prob_bayesian_method_pos < 10 ** (-100) or prob_bayesian_method_neg < 10 ** (-100):
							prob_bayesian_method_pos *= 10 ** (100)
							prob_bayesian_method_neg *= 10 ** (100)
				
				else:
					inabilitar_siguiente = 0
					
			if x == len(word_list) - 1:
				if inabilitar_siguiente == 0:
					if self.hash_one_word.has_key(word_list[x]):
						quant_reviews = self.hash_one_word[word_list[x]][0] + self.hash_one_word[word_list[x]][1]
					
						"""Standar Method"""
						prob_standard_method += self.hash_one_word[word_list[x]][0] / quant_reviews
						divisor_standard_method += 1
						
						"""Bayesian Method"""
						prob_bayesian_method_pos *= (self.hash_one_word[word_list[x]][0] + 1) / division_de_los_metodos
						prob_bayesian_method_neg *= (self.hash_one_word[word_list[x]][1] + 1) / division_de_los_metodos
						if prob_bayesian_method_pos < 10 ** (-100) or prob_bayesian_method_neg < 10 ** (-100):
							prob_bayesian_method_pos *= 10 ** (100)
							prob_bayesian_method_neg *= 10 ** (100)
		
				
				
				
		#### Find out probabilities of the methods ####
		prob_total = 0
		quant_methods = 0				
		
		
		#### One and two word methods ####
		
		"""
		if divisor_standard_method <> 0:
			prob_standard_method = prob_standard_method / divisor_standard_method
			prob_total += prob_standard_method 
			quant_methods += 1 
		
		
		if prob_bayesian_method_pos <> 0 or prob_bayesian_method_neg <> 0:
			prob_bayesian_method = prob_bayesian_method_pos / (prob_bayesian_method_pos + prob_bayesian_method_neg)
			prob_total += prob_bayesian_method
			quant_methods += 1 
		"""
			
		return prob_total / quant_methods
