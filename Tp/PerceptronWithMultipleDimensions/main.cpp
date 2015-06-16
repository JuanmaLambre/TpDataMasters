#include <iostream>
#include <cstdlib>
#include "Misc.h"
#include "Perceptron.h"
using namespace std;


int main(int argc, char** argv)
{
	//Misc::limpiarLabeled("../labeledTrainData.tsv", "../cleanLabeled.tsv");
	int iteraciones = 1;
	if (argc == 2) {
		iteraciones = atoi(argv[1]);
	}

	Perceptron perceptron("cleanLabeled.tsv", "OneWordsFrequency.tsv","TwoWordsFrequency.tsv");
	perceptron.entrenar(iteraciones);
	perceptron.calificar("testData.tsv", "results_perceptron.csv");

	return 0;
}
