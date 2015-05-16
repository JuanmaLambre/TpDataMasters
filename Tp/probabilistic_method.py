class ProbabilisticMethod(object):
	"""The initialize take the words from the documents into four hashes that contains 1, 2, 3
	   words or sentiment words"""
	def __init__(self):
		self.hash_sentiment_word = { }
		self.hash_one_word = { }
		self.hash_two_word = { }
		self.hash_three_word = { }
		
		# Proccesing one word
		one_word_doc = open("OneWordsSentiment.txt", "r")

		word_line = one_word_doc.readline()
		word_line = one_word_doc.readline()
		while word_line:
			one_word_and_sentiment = word_line.split('\t')
			self.hash_one_word[one_word_and_sentiment[0]] = (float(one_word_and_sentiment[1]), float(one_word_and_sentiment[2]))
			word_line = one_word_doc.readline()	
		one_word_doc.close()	
		
		"""
		# Proccesing two words
		two_word_doc = open("TwoWordsSentiment.txt", "r")

		word_line = two_word_doc.readline()
		word_line = two_word_doc.readline()
		while word_line:
			two_word_and_sentiment = word_line.split('\t')
			self.hash_two_word[(two_word_and_sentiment[0], two_word_and_sentiment[1])] = (int(two_word_and_sentiment[2]), int(two_word_and_sentiment[3]))
			word_line = two_word_doc.readline()	
		two_word_doc.close()	
		
		
		# Proccesing two words
		three_word_doc = open("ThreeWordsSentiment.txt", "r")

		word_line = three_word_doc.readline()
		word_line = three_word_doc.readline()
		while word_line:
			three_word_and_sentiment = word_line.split('\t')
			self.hash_three_word[(three_word_and_sentiment[0], three_word_and_sentiment[1], three_word_and_sentiment[2])] = (int(three_word_and_sentiment[3]), int(three_word_and_sentiment[4]))
			word_line = three_word_doc.readline()	
		three_word_doc.close()
		"""
		
		# Proccesing sentiment_words
		sentiment_word_doc = open("sentimentWords.txt", "r")

		word_line = sentiment_word_doc.readline()
		word_line = sentiment_word_doc.readline()
		while word_line:
			word_and_sentiment = word_line.split('\t')
			self.hash_sentiment_word[word_and_sentiment[0]] = (float(word_and_sentiment[1]), float(word_and_sentiment[2]))
			word_line = sentiment_word_doc.readline()	
		sentiment_word_doc.close()	
		
	""" This function calculate the sentiment of a review. It receive a word list of the review and
		calculates the probability using 5 methods:
		*The standard method
		*The Bayes method
		*The weight method
		*The diference weight method
		*The module diference weight method
		You can choose one or all this method
		You can choose to work with one, two, three words or sentiments words"""
	def calculate_sentiment(self, word_list):
		
		# Variables one word methods
		prob_standard_method = 0
		divisor_standard_method = 0

		prob_bayesian_method = 0
		divisor_bayesian_method = 0
		
		prob_weight_method = 0
		divisor_weight_method = 0
		
		prob_weight_dif_method = 0
		divisor_weight_dif_method = 0
		
		prob_weight_dif_method_abs = 0
		divisor_weight_dif_method_abs = 0
		
		# Variables sentiment words methods
		s_prob_standard_method = 0
		s_divisor_standard_method = 0

		s_prob_bayesian_method = 0
		s_divisor_bayesian_method = 0
		
		s_prob_weight_method = 0
		s_divisor_weight_method = 0
		
		s_prob_weight_dif_method = 0
		s_divisor_weight_dif_method = 0
		
		s_prob_weight_dif_method_abs = 0
		s_divisor_weight_dif_method_abs = 0
		
		for x in range(0, len(word_list) - 1):
			
			# Proccesing sentiment words
			if self.hash_sentiment_word.has_key(word_list[x]):
				quant_reviews = self.hash_sentiment_word[word_list[x]][0] + self.hash_sentiment_word[word_list[x]][1]
				
				"""Standar Method"""
				s_prob_standard_method += self.hash_sentiment_word[word_list[x]][0] / quant_reviews
				s_divisor_standard_method += 1
				"""Bayesian Method"""
				
				"""Weight Method"""
				s_prob_weight_method += self.hash_sentiment_word[word_list[x]][0]
				s_divisor_weight_method += quant_reviews
				
				"""Diference weight method"""
				s_prob_weight_dif_method += self.hash_sentiment_word[word_list[x]][0] * (self.hash_sentiment_word[word_list[x]][0] - self.hash_sentiment_word[word_list[x]][1]) / quant_reviews
				s_divisor_weight_dif_method += (self.hash_sentiment_word[word_list[x]][0] - self.hash_sentiment_word[word_list[x]][1])
				
				"""Module diference weight method"""
				s_prob_weight_dif_method_abs += self.hash_sentiment_word[word_list[x]][0] * abs(self.hash_sentiment_word[word_list[x]][0] - self.hash_sentiment_word[word_list[x]][1]) / quant_reviews
				s_divisor_weight_dif_method_abs += abs(self.hash_sentiment_word[word_list[x]][0] - self.hash_sentiment_word[word_list[x]][1])
				
			#Proccesing one word
			if self.hash_one_word.has_key(word_list[x]):
				quant_reviews = self.hash_one_word[word_list[x]][0] + self.hash_one_word[word_list[x]][1]
			
				"""Standar Method"""
				prob_standard_method += self.hash_one_word[word_list[x]][0] / quant_reviews
				divisor_standard_method += 1
				"""Bayesian Method"""
				
				"""Weight Method"""
				prob_weight_method += self.hash_one_word[word_list[x]][0]
				divisor_weight_method += quant_reviews
				
				"""Diference weight method"""
				prob_weight_dif_method += self.hash_one_word[word_list[x]][0] * (self.hash_one_word[word_list[x]][0] - self.hash_one_word[word_list[x]][1]) / quant_reviews
				divisor_weight_dif_method += (self.hash_one_word[word_list[x]][0] - self.hash_one_word[word_list[x]][1])
				
				"""Module diference weight method"""
				prob_weight_dif_method_abs += self.hash_one_word[word_list[x]][0] * abs(self.hash_one_word[word_list[x]][0] - self.hash_one_word[word_list[x]][1]) / quant_reviews
				divisor_weight_dif_method_abs += abs(self.hash_one_word[word_list[x]][0] - self.hash_one_word[word_list[x]][1])
			
			"""
			# Proccesing two word
			if x < len(list_tokenized_words) - 1:
				two_word = (list_tokenized_words[x], list_tokenized_words[x + 1])
				if self.hash_two_word.has_key(two_word):

			
			# Proccesing three word
			if x < len(list_tokenized_words) - 2:
				three_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2])
				if self.hash_three_word.has_key(three_word):
			"""
		
				
		# Find out probabilities of the methods
		prob_total = 0
		quant_methods = 0
		
		# One word methods
		if divisor_standard_method <> 0:
			prob_standard_method = prob_standard_method / divisor_standard_method
			prob_total += prob_standard_method 
			quant_methods += 1
			
		if divisor_weight_method <> 0:
			prob_weight_method = prob_weight_method / divisor_weight_method
			prob_total += prob_weight_method 
			quant_methods += 1

		if divisor_weight_dif_method_abs <> 0:
			prob_weight_dif_method_abs = prob_weight_dif_method_abs / divisor_weight_dif_method_abs
			prob_total += prob_weight_dif_method_abs 
			quant_methods += 1
		"""	
		if divisor_weight_dif_method <> 0:
			prob_weight_dif_method = prob_weight_dif_method / divisor_weight_dif_method
			prob_total += prob_weight_dif_method
			quant_methods += 1"""
			
		# sentiment words methods
		if s_divisor_standard_method <> 0:
			s_prob_standard_method = s_prob_standard_method / s_divisor_standard_method
			prob_total += s_prob_standard_method
			quant_methods += 1
			
		if s_divisor_weight_method <> 0:
			s_prob_weight_method = s_prob_weight_method / s_divisor_weight_method
			prob_total += s_prob_weight_method
			quant_methods += 1
			
		if s_divisor_weight_dif_method_abs <> 0:
			s_prob_weight_dif_method_abs = s_prob_weight_dif_method_abs / s_divisor_weight_dif_method_abs
			prob_total += s_prob_weight_dif_method_abs 
			quant_methods += 1
		"""
		if s_divisor_weight_dif_method <> 0:
			s_prob_weight_dif_method = s_prob_weight_dif_method / s_divisor_weight_dif_method
			prob_total += s_prob_weight_dif_method 
			quant_methods += 1
		"""
		"""prob_bayesian_method = prob_bayesian_method / divisor_bayesian_method """
		
		return prob_total / quant_methods
		
