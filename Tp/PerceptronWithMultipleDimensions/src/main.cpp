#include <iostream>
#include "Misc.h"
#include "Perceptron.h"
using namespace std;


int main()
{
	//Misc::limpiarLabeled("../labeledTrainData.tsv", "../cleanLabeled.tsv");
	Perceptron perceptron("cleanLabeled.tsv", "OneWordsFrequency.tsv","TwoWordsFrequency.tsv");
	perceptron.entrenar(1);
	perceptron.calificar("testData.tsv", "results_perceptron.csv");

	return 0;
}
