#include <iostream>
#include "Misc.h"
#include "Probabilistic.h"
using namespace std;


int main()
{
	//Misc::limpiarLabeled("../labeledTrainData.tsv", "../cleanLabeled.tsv");
	std::vector<std::string> fileFrequencyDocumentNames;
	fileFrequencyDocumentNames.push_back("OneWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("TwoWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("ThreeWordsFrequency.tsv");
	Probabilistic proba(2, fileFrequencyDocumentNames, "cleanLabeled.tsv");
	proba.entrenar();
	proba.calificar("testData.tsv", "results_proba.csv");

	return 0;
}
