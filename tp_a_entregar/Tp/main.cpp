#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Misc.h"
#include "Probabilistic.h"
#include "Perceptron.h"
using namespace std;

#define INF_FIRST_LIMIT  0.4375
#define SUP_FIRST_LIMIT  0.5625
#define INF_SECOND_LIMIT 0.4575
#define SUP_SECOND_LIMIT 0.5425

int main(int argc, char** argv) {
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

	// Decidimos que probabilidad vamos a utilizar para calificar a la review.
	ifstream test("extra/testData.tsv");
	string line;
	getline(test, line);
	while (getline(test, line)) {
		std::string id = line.substr(0, line.find('\t'));
		float probRev;
		probRev = proba.getReviewProba(id);

		if (probRev != -1 and ((probRev > SUP_FIRST_LIMIT) or (probRev < INF_FIRST_LIMIT))) {
			results << id << "," << probRev << "\n";

		} else if (probRev != -1 and ((0.5425 < probRev) or (probRev < 0.4575))) {
			probRev = (perceptron.getReviewProba(id) + proba.getReviewProba(id)) / 2;
			results << id << "," << probRev << "\n";

		} else {
			probRev = perceptron.getReviewProba(id);
			results << id << "," << probRev << "\n";
			if (probRev == -1) {
				cout << "ERROR!\n";
				return 1;
			}
		}
	}
	return 0;
}
