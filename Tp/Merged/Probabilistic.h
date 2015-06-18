#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "definitions.h"

class Probabilistic {
private:
	int quantWordByShingle;
	int totalPositiveOneWord;
	int totalNegativeOneWord;
	int totalPositiveTwoWord;
	int totalNegativeTwoWord;
	int totalPositiveThreeWord;
	int totalNegativeThreeWord;
	std::string labeledFile;
	std::map<std::string, std::pair<int, int> > oneWordSentiment;
	std::map<std::string, std::pair<int, int> > twoWordSentiment;
	std::map<std::string, std::pair<int, int> > threeWordSentiment;
public:
	Probabilistic(int quantWordByShingle, std::vector<std::string> fileFrequencyDocumentNames, std::string trainFile);
	~Probabilistic() { std::cout << std::endl; };

	void entrenar();

	void calificar(ReviewsList& reviews, std::ofstream& output);
};
