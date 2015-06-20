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

#define INF_LIMIT 0.435
#define SUP_LIMIT 0.565

int main(int argc, char** argv)
{
	int iteraciones = 100;
	if (argc >= 2) {
		iteraciones = atoi(argv[1]);
	} else {
		std::cout << "!! Perceptron: " << iteraciones
				<< " iteraciones por default !!\n";
	}

	// Probabilistic:
	std::vector<std::string> fileFrequencyDocumentNames;
	fileFrequencyDocumentNames.push_back("./extra/OneWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("./extra/TwoWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("./extra/ThreeWordsFrequency.tsv");
	Probabilistic proba(3, fileFrequencyDocumentNames, "./extra/cleanLabeled.tsv");
	proba.entrenar();
	proba.calificar("extra/testData.tsv");

	// Perceptron:
	Perceptron perceptron("extra/cleanLabeled.tsv", "extra/OneWordsFrequency.tsv","extra/TwoWordsFrequency.tsv","extra/ThreeWordsFrequency.tsv");
	perceptron.entrenar(iteraciones);
	perceptron.calificar("extra/testData.tsv");

	// Este es el archivo de salida
	ofstream results("results_merged.csv");
	results << "\"id\",\"sentiment\"\n";

	// Este es la lista que se va a pasar por los métodos
	ifstream test("extra/testData.tsv");
	string line;
	int CantidadCalificadaPorProba = 0;
	getline(test, line);
	while (getline(test, line)) {
		std::string id = line.substr(0, line.find('\t'));
		float probRev;
		probRev = proba.getReviewProba(id);


		if (probRev != -1 and ((SUP_LIMIT < probRev) or (probRev < INF_LIMIT))) {
			results << id << "," << probRev << "\n";
			 CantidadCalificadaPorProba++;
		} else {
			probRev = perceptron.getReviewProba(id);
			results << id << "," << probRev << "\n";
			if (probRev == -1) {
				cout << "ERROR!";
			}
		}
	}
	printf("Cantidad de reviews calificadas por Proba: %d \n", CantidadCalificadaPorProba);
	return 0;
}
