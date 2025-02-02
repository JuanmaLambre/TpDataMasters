#
#	Dado una lista de palabras devuelve una lista de shingles de 'multi' palabras
#	Por ejemplo: Dado el ["Wish", "you", "were", "here"], si multi=2 el metodo
#	devolvera ["Wish you", "you were", "were here"]. Si multi=3, devolvera
#	["Wish you were", "you were here"]


def join(multi, wordsList):
	shingles = []	
	for i in range(len(wordsList)-multi+1):
		# Uno las palabras a shingle:
		shingle = ""
		for j in range(multi):
			shingle += wordsList[i+j] + " "
		# Remuevo el ultimo espacio
		shingle = shingle[:len(shingle)-1]
		shingles.append( shingle )
	return shingles	
