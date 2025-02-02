import sys

if __name__ == "__main__":
	
	infLimit, supLimit = 0, 0
	if len(sys.argv) == 3:
		infLimit = min(float(sys.argv[1]), float(sys.argv[2]))
		supLimit = max(float(sys.argv[1]), float(sys.argv[2]))
	if len(sys.argv) == 2:
		limit = float(sys.argv[1])
		infLimit = 0.5 - abs(0.5-limit)
		supLimit = 0.5 + abs(0.5-limit)
	
	results = open("results_proba.csv", 'r')
	results.readline()
	line = results.readline()
	enRango = 0
	while line:
		calif = float( line.split(',')[1].split('\n')[0] )
		enRango += not (infLimit < calif < supLimit)
		line = results.readline()
	
	print enRango*100./25000, '%';
	
	
