###############################################################################
#
#	Cuenta cuantas reviews tienen un puntaje x/10 en el review
#
###############################################################################

reviews = open('labeledTrainData.tsv', 'r')

contador, total = 0.0, 0.0;
# Header:
reviews.readline()

line = reviews.readline()
while line:
	total += 1
	texto = line.split('\t')[2]
	contador += ("/10" in texto)
	line = reviews.readline()
	
print "CONTADOS:", contador
print "TOTAL:", total
print contador*100/total, "%"
