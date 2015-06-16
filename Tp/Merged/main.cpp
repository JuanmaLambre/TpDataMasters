#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include "Misc.h"
#include "Probabilistic.h"
using namespace std;


int main()
{
	list< vector<string> > reviews;
	ifstream test("cleanTest.tsv");
	string line;
	while (test.peek() != EOF) {
		getline(test, line);
		reviews.push_back(Misc::split(line, '\t'));
	}

	/*
	0 < p < 0.4  or  0.6 < p < 1   ==>   0.99 y califica  400 rev
	0 < p < 0.45  or  0.55 < p < 1  ==>  0.966  y califica  1500 rev
	0 < p < 0.475  or  0.525 < p  < 1   ==>  0.935  y califica  2300 rev
	*/

	std::vector<std::string> fileFrequencyDocumentNames;
	fileFrequencyDocumentNames.push_back("OneWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("TwoWordsFrequency.tsv");
	fileFrequencyDocumentNames.push_back("ThreeWordsFrequency.tsv");
	Probabilistic proba(2, fileFrequencyDocumentNames, "cleanLabeled.tsv");
	proba.entrenar();
	ReviewsList_t out = proba.calificar(reviews, "results_proba.csv");
	
	std::cout << out.size() << '\n';

	return 0;
}
