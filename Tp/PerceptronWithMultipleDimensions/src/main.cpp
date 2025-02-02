#include <iostream>
#include <cstdlib>
#include "Misc.h"
#include "Perceptron.h"
using namespace std;


int main(int argc, char** argv)
{
	int iteraciones = 1;
	if (argc == 2) {
		iteraciones = atoi(argv[1]);
		std::cout << "Con " << iteraciones << " iteraciones\n";
	} else {
		std::cout << "Iteraciones por default: 1\n";
	}
	//Misc::limpiarLabeled("labeledTrainData.tsv", "cleanLabeled.tsv");
	Perceptron perceptron("cleanLabeled.tsv", "OneWordsFrequency.tsv","TwoWordsFrequency.tsv", "ThreeWordsFrequency.tsv");
	perceptron.entrenar(iteraciones);
	perceptron.calificar("testData.tsv", "results_perceptron.csv");

	return 0;
}
