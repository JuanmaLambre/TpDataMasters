# *************************************************************************** #
#																			  #
#	This file contains a function that recieves a raw text and returns a	  #
#	vector of lowercased unicode words, without HTML tags or punctuation	  #
#																			  #
# *************************************************************************** #

from bs4 import BeautifulSoup
from nltk.corpus import stopwords
import re

def cleanText(text):
	withoutHtmlTags = BeautifulSoup(text).get_text()
	onlyLetters = re.sub('[^A-Za-z0-9]+', " ", withoutHtmlTags)
	onlyLettersSplit = onlyLetters.lower().split()
	stops = set(stopwords.words("english"))	                     
	onlyLetters_without_stops = [word for word in onlyLettersSplit if not word in stops]
	return onlyLetters_without_stops
