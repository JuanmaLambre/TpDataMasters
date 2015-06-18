#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Misc.h"
#include "definitions.h"
#include "Probabilistic.h"
#include "Perceptron.h"
using namespace std;


int main(int argc, char** argv)
{
	int iteraciones = 100;
	if (argc >= 2) {
		iteraciones = atoi(argv[1]);
	} else {
		std::cout << "!! Perceptron: " << iteraciones
				<< " iteraciones por default !!\n";
	}

	// Este es la lista que se va a pasar por los métodos
	list< vector<string> > reviews;
	ifstream test("extra/cleanTest.tsv");
	string line;
	while (test.peek() != EOF) {
		getline(test, line);
		reviews.push_back(Misc::split(line, '\t'));
	}

	// Este es el archivo de salida
	ofstream results("results_merged.csv");
	results << "\"id\",\"sentiment\"\n";

	// Probabilistic:
	std::vector<std::string> fileFrequencyDocumentNames;
	fileFrequencyDocumentNames.push_back("./extra/OneWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("./extra/TwoWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("./extra/ThreeWordsFrequency.tsv");
	Probabilistic proba(2, fileFrequencyDocumentNames, "./extra/cleanLabeled.tsv");
	proba.entrenar();
	proba.calificar(reviews, results);
	
	// Perceptron:
	Perceptron perceptron("extra/cleanLabeled.tsv", "extra/OneWordsFrequency.tsv","extra/TwoWordsFrequency.tsv");
	perceptron.entrenar(iteraciones);
	perceptron.calificar(reviews, results);

	return 0;
}
