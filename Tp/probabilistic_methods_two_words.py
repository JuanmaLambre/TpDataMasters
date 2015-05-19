class ProbabilisticMethod(object):
	
	"""The initialize take the words from the documents into four hashes that contains 1, 2, 3
	   words or sentiment words"""
	def __init__(self):
		self.hash_one_word = { }
		self.hash_two_word = { }
		self.hash_three_word = { }
		
		self.total_freq_pos_one_word = 0
		self.total_freq_neg_one_word = 0
		
		self.total_freq_pos_two_word = 0
		self.total_freq_neg_two_word = 0
		
		self.total_freq_pos_three_word = 0
		self.total_freq_neg_three_word = 0
		
		##### Proccesing one word #####
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
		
		
		##### Proccesing two words #####
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
		
		
		#### Proccesing three words ####
		three_word_doc = open("ThreeWordsSentiment.txt", "r")

		word_line = three_word_doc.readline()
		word_line = three_word_doc.readline()
		while word_line:
			three_word_and_sentiment = word_line.split('\t')
			self.hash_three_word[(three_word_and_sentiment[0], three_word_and_sentiment[1], three_word_and_sentiment[2])] = (float(three_word_and_sentiment[3]), float(three_word_and_sentiment[4]))
			word_line = three_word_doc.readline()	
		three_word_doc.close()
		
		
	""" This function calculate the sentiment of a review. It receive a word list of the review and
		calculates the probability using 5 methods:
		*The standard method
		*The Bayes method
		You can choose one or all this method
		You can choose to work with one, two, three words or sentiments words"""
	def calculate_sentiment(self, word_list):
		
		#### Variables one word methods ####
		prob_standard_method = 0
		divisor_standard_method = 0

		prob_bayesian_method_pos = 1
		prob_bayesian_method_neg = 1
		
		#### Variables two words methods ####
		w_prob_standard_method = 0
		w_divisor_standard_method = 0

		w_prob_bayesian_method_pos = 1
		w_prob_bayesian_method_neg = 1
		
		#### Variables sentiment three methods ####
		t_prob_standard_method = 0
		t_divisor_standard_method = 0

		t_prob_bayesian_method_pos = 1
		t_prob_bayesian_method_neg = 1
		
		
		
		for x in range(0, len(word_list) - 1):
			
			#### Proccesing one word ####
			if self.hash_one_word.has_key(word_list[x]):
				quant_reviews = self.hash_one_word[word_list[x]][0] + self.hash_one_word[word_list[x]][1]
			
				"""Standar Method"""
				prob_standard_method += self.hash_one_word[word_list[x]][0] / quant_reviews
				divisor_standard_method += 1
				
				"""Bayesian Method"""
				prob_bayesian_method_pos *= (self.hash_one_word[word_list[x]][0] + 1) / (len(self.hash_one_word) + self.total_freq_pos_one_word )
				prob_bayesian_method_neg *= (self.hash_one_word[word_list[x]][1] + 1) / (len(self.hash_one_word) + self.total_freq_neg_one_word )
				if prob_bayesian_method_pos < 10 ** (-100) or prob_bayesian_method_neg < 10 ** (-100):
					prob_bayesian_method_pos *= 10 ** (100)
					prob_bayesian_method_neg *= 10 ** (100)
				
				
			#### Proccesing two words ####
			if x < len(word_list) - 1:
				two_words = (word_list[x], word_list[x + 1])
				if self.hash_two_word.has_key(two_words):
					quant_reviews = self.hash_two_word[two_words][0] + self.hash_two_word[two_words][1]
					
					"""Standar Method"""
					w_prob_standard_method += self.hash_two_word[two_words][0] / quant_reviews
					w_divisor_standard_method += 1
					
					"""Bayesian Method"""
					w_prob_bayesian_method_pos *= (self.hash_two_word[two_words][0] + 1) / (len(self.hash_two_word) + self.total_freq_pos_two_word )
					w_prob_bayesian_method_neg *= (self.hash_two_word[two_words][1] + 1) / (len(self.hash_two_word) + self.total_freq_neg_two_word )
					if w_prob_bayesian_method_pos < 10 ** (-100) or w_prob_bayesian_method_neg < 10 ** (-100):
						w_prob_bayesian_method_pos *= 10 ** (100)
						w_prob_bayesian_method_neg *= 10 ** (100)
					
					
			#### Proccesing three words ####
			if x < len(word_list) - 2:
				three_words = (word_list[x], word_list[x + 1], word_list[x + 2])
				
				if self.hash_three_word.has_key(three_words):
					quant_reviews = self.hash_three_word[three_words][0] + self.hash_three_word[three_words][1]
					
					"""Standar Method"""
					t_prob_standard_method += self.hash_three_word[three_words][0] / quant_reviews
					t_divisor_standard_method += 1
					
					"""Bayesian Method"""
					t_prob_bayesian_method_pos *= (self.hash_three_word[three_words][0] + 1) / (len(self.hash_three_word) + self.total_freq_pos_three_word )
					t_prob_bayesian_method_neg *= (self.hash_three_word[three_words][1] + 1) / (len(self.hash_three_word) + self.total_freq_neg_three_word )
					if t_prob_bayesian_method_pos < 10 ** (-100) or t_prob_bayesian_method_neg < 10 ** (-100):
						t_prob_bayesian_method_pos *= 10 ** (100)
						t_prob_bayesian_method_neg *= 10 ** (100)
		
				
					
		##### Find out probabilities of the methods #####
		prob_total = 0
		quant_methods = 0				
		
		
		
		#### One word methods ####
		
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
		
		
		#### Two word methods ####
		
		
		if w_divisor_standard_method <> 0:
			w_prob_standard_method = w_prob_standard_method / w_divisor_standard_method
			prob_total += w_prob_standard_method 
			quant_methods += 1
		else:
			return 0
		
		"""
		if w_prob_bayesian_method_pos <> 0 or w_prob_bayesian_method_neg <> 0:
			w_prob_bayesian_method = w_prob_bayesian_method_pos / (w_prob_bayesian_method_pos + w_prob_bayesian_method_neg)
			prob_total += w_prob_bayesian_method
			quant_methods += 1 
		
		else: 
			return 0
		"""
		
		
		#### Three word methods ####
		
		"""
		if t_divisor_standard_method <> 0:
			t_prob_standard_method = t_prob_standard_method / t_divisor_standard_method
			prob_total += t_prob_standard_method 
			quant_methods += 1
		else:
			return 0


		if t_prob_bayesian_method_pos <> 0 or t_prob_bayesian_method_neg <> 0:
			t_prob_bayesian_method = t_prob_bayesian_method_pos / (t_prob_bayesian_method_pos + t_prob_bayesian_method_neg)
			prob_total += t_prob_bayesian_method
			quant_methods += 1 
		
		else: 
			return 0
		"""
		
		return prob_total / quant_methods
		
