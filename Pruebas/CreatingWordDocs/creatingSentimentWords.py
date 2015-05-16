import cleanText as cl

hash_one_word = { } 
hash_two_word = { }
hash_three_word = { }

hash_posible_one_word = { }
hash_posible_two_word = { }
hash_posible_three_word = { }

# Generate a hash that contain the words we will use

# Proccesing one word
words_selected = open("OneWordsfrequency.txt", "r")
	
word_line = words_selected.readline()
iteration = 0
while word_line and iteration < 50000:
	word = word_line.split('\t')[0]
	hash_posible_one_word[word] = 1
	word_line = words_selected.readline()
	iteration += 1

words_selected.close()

# Proccesing two words
words_selected = open("TwoWordsFrequency.txt", "r")
	
word_line = words_selected.readline()
iteration = 0
while word_line and iteration < 320000:
	words = word_line.split('\t')
	hash_posible_two_word[(words[0], words[1])] = 1
	word_line = words_selected.readline()
	iteration += 1

words_selected.close()

# Proccesing three words
words_selected = open("ThreeWordsFrequency.txt", "r")
	
word_line = words_selected.readline()
iteration = 0
while word_line and iteration < 100000:
	words = word_line.split('\t')
	hash_posible_three_word[(words[0], words[1], words[2])] = 1
	word_line = words_selected.readline()
	iteration += 1

words_selected.close()


# Generate a hash that contain the words with the sentiment 
trainData = open("../labeledTrainData.tsv", 'r')

wholeReview = trainData.readline()
while wholeReview:
	wholeReviewSplit = wholeReview.split('\t')
	rawReview = wholeReviewSplit[2]
	sentiment = wholeReviewSplit[1]
	if sentiment == "1":
		sentiment_review = (1, 0)
	else:
		sentiment_review = (0, 1)
	
	list_tokenized_words = cl.cleanText(rawReview) 
	for x in range(0, len(list_tokenized_words) - 1):

		# Proccesing one word
		if hash_posible_one_word.has_key(list_tokenized_words[x]):
			if hash_one_word.has_key(list_tokenized_words[x]):
				sentiment_tuple = hash_one_word[list_tokenized_words[x]]
				hash_one_word[list_tokenized_words[x]] = (sentiment_tuple[0] + sentiment_review[0], sentiment_tuple[1] + sentiment_review[1])
			else:
				hash_one_word[list_tokenized_words[x]] = sentiment_review
			
		# Proccesing two word
		if x < len(list_tokenized_words) - 1:
			two_word = (list_tokenized_words[x], list_tokenized_words[x + 1])
			if hash_posible_two_word.has_key(two_word):
				if hash_two_word.has_key(two_word):
					sentiment_tuple = hash_two_word[two_word]
					hash_two_word[two_word] = (sentiment_tuple[0] + sentiment_review[0], sentiment_tuple[1] + sentiment_review[1])
				else:
					hash_two_word[two_word] = sentiment_review
		
		# Proccesing three word
		if x < len(list_tokenized_words) - 2:
			three_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2])
			if hash_posible_three_word.has_key(three_word):
				if hash_three_word.has_key(three_word):
					sentiment_tuple = hash_three_word[three_word]
					hash_three_word[three_word] = (sentiment_tuple[0] + sentiment_review[0], sentiment_tuple[1] + sentiment_review[1])
				else:
					hash_three_word[three_word] = sentiment_review
					
	wholeReview = trainData.readline()


# Proccesing one word
sentiment_word_doc = open("OneWordsSentiment.txt", "w")
sentiment_word_doc.write("Palabra" + "\t" + "Cantidad Review Pos" + "\t" + "Cantidad Review Neg" + "\n")
for word, sentiment in hash_one_word.items():
	sentiment_word_doc.write(str(word) + "\t" + str(sentiment[0]) + "\t" + str(sentiment[1]) + "\n")
	
sentiment_word_doc.close()

# Proccesing two words
sentiment_word_doc = open("TwoWordsSentiment.txt", "w")
sentiment_word_doc.write("Palabra" + "\t" + "Cantidad Review Pos" + "\t" + "Cantidad Review Neg" + "\n")
for word, sentiment in hash_two_word.items():
	sentiment_word_doc.write(str(word[0]) + "\t" + str(word[1]) + "\t" + str(sentiment[0]) + "\t" + str(sentiment[1]) + "\n")
	
sentiment_word_doc.close()

# Proccesing three words
sentiment_word_doc = open("ThreeWordsSentiment.txt", "w")
sentiment_word_doc.write("Palabra" + "\t" + "Cantidad Review Pos" + "\t" + "Cantidad Review Neg" + "\n")
for word, sentiment in hash_three_word.items():
	sentiment_word_doc.write(str(word[0]) + "\t" + str(word[1]) + "\t" + str(word[2]) + "\t" + str(sentiment[0]) + "\t" + str(sentiment[1]) + "\n")
	
sentiment_word_doc.close()



