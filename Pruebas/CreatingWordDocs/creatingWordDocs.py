import cleanText as cl

hash_one_word = { } 
hash_two_word = { }
hash_three_word = { }
hash_four_word = { }
hash_five_word = { }


# Getting all the tokenized words into the hash. This value contain the
# many times that the word appear

# Proccesing unTrainData
unTrainData = open("../unlabeledTrainData.tsv", 'r')

wholeReview = unTrainData.readline()
while wholeReview:
	rawReview = wholeReview.split('\t')[1]
	list_tokenized_words = cl.cleanText(rawReview) 
	for x in range(0, len(list_tokenized_words) - 1):
		
		# Proccesing one word
		hash_one_word[list_tokenized_words[x]] = hash_one_word.get(list_tokenized_words[x], 0) + 1
		
		# Proccesing two words
		if x < len(list_tokenized_words) - 1:
			two_word = (list_tokenized_words[x], list_tokenized_words[x + 1])
			hash_two_word[two_word] = hash_two_word.get(two_word, 0) + 1
			
		# Proccesing three words
		if x < len(list_tokenized_words) - 2:
			three_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2])
			hash_three_word[three_word] = hash_three_word.get(three_word, 0) + 1
		
		# Proccesing four words
		if x < len(list_tokenized_words) - 3:
			four_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2], list_tokenized_words[x + 3])
			hash_four_word[four_word] = hash_four_word.get(four_word, 0) + 1
		
		# Proccesing five words
		if x < len(list_tokenized_words) - 4:
			five_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2], list_tokenized_words[x + 3], list_tokenized_words[x + 4])
			hash_five_word[five_word] = hash_five_word.get(five_word, 0) + 1

	wholeReview = unTrainData.readline()

unTrainData.close()
print "Terminamos el 1"

# Proccesing trainData
trainData = open("../labeledTrainData.tsv", 'r')

wholeReview = trainData.readline()
while wholeReview:
	rawReview = wholeReview.split('\t')[2]
	list_tokenized_words = cl.cleanText(rawReview) 
	for x in range(0, len(list_tokenized_words) - 1):
		
		# Proccesing one word
		hash_one_word[list_tokenized_words[x]] = hash_one_word.get(list_tokenized_words[x], 0) + 1
		
		# Proccesing two words
		if x < len(list_tokenized_words) - 1:
			two_word = (list_tokenized_words[x], list_tokenized_words[x + 1])
			hash_two_word[two_word] = hash_two_word.get(two_word, 0) + 1
			
		# Proccesing three words
		if x < len(list_tokenized_words) - 2:
			three_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2])
			hash_three_word[three_word] = hash_three_word.get(three_word, 0) + 1
		
		# Proccesing four words
		if x < len(list_tokenized_words) - 3:
			four_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2], list_tokenized_words[x + 3])
			hash_four_word[four_word] = hash_four_word.get(four_word, 0) + 1
		
		# Proccesing five words
		if x < len(list_tokenized_words) - 4:
			five_word = (list_tokenized_words[x], list_tokenized_words[x + 1], list_tokenized_words[x + 2], list_tokenized_words[x + 3], list_tokenized_words[x + 4])
			hash_five_word[five_word] = hash_five_word.get(five_word, 0) + 1

	wholeReview = trainData.readline()

trainData.close()
print "Terminamos el 2"


# Then we generate the words doc with each word frequency

# Proccesing one word
word_doc = open("OneWordsFrequency.txt", "w")
word_doc.write("Palabra" + "\t" + "Frecuencia" + "\n")
for word in sorted(hash_one_word, key = hash_one_word.get, reverse=True):
	word_doc.write(str(word) + "\t" + str(hash_one_word[word]) + "\n")	
word_doc.close()

# Proccesing two words
word_doc = open("TwoWordsFrequency.txt", "w")
word_doc.write("Palabra1" + "\t" + "Palabra2" + "\t" + "Frecuencia" + "\n")
for two_words in sorted(hash_two_word, key = hash_two_word.get, reverse=True):
	word_doc.write(str(two_words[0]) + "\t" + str(two_words[1]) + "\t" + str(hash_two_word[two_words]) + "\n")
word_doc.close()

# Proccesing three words
word_doc = open("ThreeWordsFrequency.txt", "w")
word_doc.write("Palabra1" + "\t" + "Palabra2" + "\t" + "Palabra3" + "\t" + "Frecuencia" + "\n")
for three_words in sorted(hash_three_word, key = hash_three_word.get, reverse=True):
	word_doc.write(str(three_words[0]) + "\t" + str(three_words[1]) + "\t" + str(three_words[2]) + "\t" + str(hash_three_word[three_words]) + "\n")
word_doc.close()

# Proccesing four words
word_doc = open("FourWordsFrequency.txt", "w")
word_doc.write("Palabra1" + "\t" + "Palabra2" + "\t" + "Palabra3" + "\t" + "Palabra4" + "\t" + "Frecuencia" + "\n")
for four_words in sorted(hash_four_word, key = hash_four_word.get, reverse=True):
	word_doc.write(str(four_words[0]) + "\t" + str(four_words[1]) + "\t" + str(four_words[2]) + "\t" + str(four_words[3]) + "\t" + str(hash_four_word[four_word]) + "\n")
word_doc.close()

# Proccesing five words
word_doc = open("FiveWordsFrequency.txt", "w")
word_doc.write("Palabra1" + "\t" + "Palabra2" + "\t" + "Palabra3" + "\t" + "Palabra4" + "\t" + "Palabra5" + "\t" + "Frecuencia" + "\n")
for five_words in sorted(hash_five_word, key = hash_five_word.get, reverse=True):
	word_doc.write(str(five_words[0]) + "\t" + str(five_words[1]) + "\t" + str(five_words[2]) + "\t" + str(five_words[3]) + "\t" + str(five_words[4]) + "\t" + str(hash_five_word[five_word]) + "\n")
word_doc.close()

