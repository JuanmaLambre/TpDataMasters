# *************************************************************************** #
#																			  #
#	This file contains a function that recieves a raw text and returns a	  #
#	vector of lowercased unicode words, without HTML tags or punctuation	  #
#																			  #
# *************************************************************************** #

from bs4 import BeautifulSoup
import re


def cleanText(text):
	withoutHtmlTags = BeautifulSoup(text).get_text()
	onlyLetters = re.sub("[^a-zA-Z]", " ", withoutHtmlTags)
	return onlyLetters.lower().split()
