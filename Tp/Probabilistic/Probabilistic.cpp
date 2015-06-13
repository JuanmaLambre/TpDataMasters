#include "Probabilistic.h"
#include "Misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

Probabilistic::Probabilistic(int quantWordByShingle, std::vector<std::string> fileFrequencyDocumentNames, std::string trainFile) {
	std::cout << "INICIALIZANDO..." << std::endl;
	this->quantWordByShingle = quantWordByShingle;
	this->labeledFile = trainFile;
	std::string line;

	if (quantWordByShingle >= 1) {					// Shingle de 1 palabra
		std::string nameFile = fileFrequencyDocumentNames[0];
		std::ifstream wordFile(nameFile.c_str());

		std::getline(wordFile, line);
		for(int i = 0; i < 50000; ++i) {
			std::getline(wordFile, line);
			std::string word = Misc::split(line, '\t')[0];
			oneWordSentiment[word] = std::pair<int, int> (0, 0);
		}
	}
	if (quantWordByShingle >= 2) {					// Shingle de 2 palabra
		std::string nameFile = fileFrequencyDocumentNames[1];
		std::ifstream wordFile(nameFile.c_str());

		std::getline(wordFile, line);
		for(int i = 0; i < 150000; ++i) {
			std::getline(wordFile, line);
			std::string words = Misc::split(line, '\t')[0] + " " + Misc::split(line, '\t')[1];
			twoWordSentiment[words] = std::pair<int, int> (0, 0);
		}
	}
	if (quantWordByShingle >= 3) {					// Shingle de 3 palabra
		std::string nameFile = fileFrequencyDocumentNames[2];
		std::ifstream wordFile(nameFile.c_str());

		std::getline(wordFile, line);
		for(int i = 0; i < 120000; ++i) {
			std::getline(wordFile, line);
			std::string words = Misc::split(line, '\t')[0] + " " + Misc::split(line, '\t')[1] + " " + Misc::split(line, '\t')[2];
			threeWordSentiment[words] = std::pair<int, int> (0, 0);
		}
	}
	totalPositiveOneWord = 0;
	totalNegativeOneWord = 0;
	totalPositiveTwoWord = 0;
	totalNegativeTwoWord = 0;
	totalPositiveThreeWord = 0;
	totalNegativeThreeWord = 0;
}

void Probabilistic::entrenar() {
	std::cout << "ENTRENANDO..." << std::endl;
	std::ifstream trainFile(labeledFile.c_str());
	std::string line;
	std::string reviewText;
	std::string sentimentText;
	std::vector<std::string> simpleShingles;
	int sentiment;
	int iteracion = 0;
	while(std::getline(trainFile, line)) {
		reviewText = line.substr( line.find('\t', line.find('\t')+1) + 1 );
		sentimentText = line.substr(line.find('\t') + 1, 1);
		sentiment = atoi(sentimentText.c_str());

		simpleShingles =  Misc::split(reviewText);
		bool inabilitar_siguiente_one = false;
		bool inabilitar_siguiente_two = false;
		std::vector<std::string>::iterator itS;
		for (itS = simpleShingles.begin(); (itS + 2) != simpleShingles.end(); ++itS) {
			std::string threeWords = (*itS) + " " + (*(itS + 1)) + " " + (*(itS + 2));
			if (threeWordSentiment.count(threeWords) != 0) {
				inabilitar_siguiente_one = true;
				inabilitar_siguiente_two = true;
				std::pair<int, int> actualSentiment = twoWordSentiment[threeWords];
				if (sentiment == 1) {
					twoWordSentiment[threeWords] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
				} else {
					twoWordSentiment[threeWords] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
				}

			} else if (!inabilitar_siguiente_two) {
				std::string twoWords = (*itS) + " " + (*(itS + 1));
				if (twoWordSentiment.count(twoWords) != 0) {
					inabilitar_siguiente_one = true;
					std::pair<int, int> actualSentiment = twoWordSentiment[twoWords];
					if (sentiment == 1) {
						twoWordSentiment[twoWords] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
					} else {
						twoWordSentiment[twoWords] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
					}

				} else if(!inabilitar_siguiente_one) {
					std::string word = (*itS);
					if (oneWordSentiment.count(word) != 0) {
						std::pair<int, int> actualSentiment = oneWordSentiment[word];
						if (sentiment == 1) {
							oneWordSentiment[word] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
						} else {
							oneWordSentiment[word] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
						}
					}

				} else {
					inabilitar_siguiente_one = false;
				}

			} else {
				inabilitar_siguiente_two = false;
				inabilitar_siguiente_one = false;
			}
		}
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
}

void Probabilistic::calificar(const std::string& inputFilename, const std::string& outputFilename) {
	std::cout << "CALIFICANDO..." << std::endl;
	std::ifstream testFile(inputFilename.c_str());
	std::string line;
	std::string review;
	std::vector<std::string> simpleShingles;
	int iteracion = 0;
	int divisorBayesianoPos = twoWordSentiment.size() + oneWordSentiment.size() + totalPositiveOneWord + totalPositiveTwoWord;
	int divisorBayesianoNeg = twoWordSentiment.size() + oneWordSentiment.size() + totalNegativeOneWord + totalNegativeTwoWord;

	std::ofstream results(outputFilename.c_str());
	results << "\"id\",\"sentiment\"\n";

	std::getline(testFile, line);
	while(std::getline(testFile, line)) {
		float probStandarMethod = 0;
		int divisorStandarMethod = 0;
		float probBayesianMethodPos = 1;
		float probBayesianMethodNeg = 1;

		review = line.substr(line.find('\t') + 1);
		Misc::processText(review);
		Misc::removeStopwords(review);
		simpleShingles =  Misc::split(review);

		bool inabilitar_siguiente_one = false;
		bool inabilitar_siguiente_two = false;
		bool inabilitar_actual_one = false;
		bool inabilitar_actual_two = false;
		float positive = 0;
		float negative = 0;
		std::vector<std::string>::iterator itS;
		for (itS = simpleShingles.begin(); (itS + 2) != simpleShingles.end(); ++itS) {
			std::string threeWords = (*itS) + " " + (*(itS + 1)) + " " + (*(itS + 2));
			if (threeWordSentiment.count(threeWords) != 0) {
				positive = twoWordSentiment[threeWords].first;
				negative = twoWordSentiment[threeWords].second;
				if (positive != 0 or negative != 0) {
					inabilitar_siguiente_two = true;
				} else {
					inabilitar_actual_two = false;
				}

			} else {
				inabilitar_actual_two = false;
			}

			if (!inabilitar_actual_two) {
				if (!inabilitar_siguiente_two) {
					std::string twoWords = (*itS) + " " + (*(itS + 1));

					if (twoWordSentiment.count(twoWords) != 0) {
						positive = twoWordSentiment[twoWords].first;
						negative = twoWordSentiment[twoWords].second;

						if (positive != 0 or negative != 0) {
							inabilitar_siguiente_one = true;
						} else {
							inabilitar_actual_one = false;
						}

					} else {
						inabilitar_actual_one = false;
					}

					if(!inabilitar_actual_one) {
						if (!inabilitar_siguiente_one) {
							std::string word = (*itS);
							if (oneWordSentiment.count(word) != 0) {
								positive = oneWordSentiment[word].first;
								negative = oneWordSentiment[word].second;
							}
						} else {
							inabilitar_siguiente_one = false;
						}
					}

				} else {
					inabilitar_siguiente_two = false;
				}
			}

			if (positive != 0 or negative != 0) {
				// Calculamos por el Metodo Standar
				probStandarMethod += (positive / (positive + negative));
				divisorStandarMethod += 1;

				// Calculamos por el Metodo Bayesiano
				probBayesianMethodPos *= ((positive + 1) / divisorBayesianoPos);
				probBayesianMethodNeg *= ((negative + 1) / divisorBayesianoNeg);

				if (probBayesianMethodPos < pow(10,-100) or probBayesianMethodNeg < pow(10,-100)){
					probBayesianMethodPos *= pow(10,100);
					probBayesianMethodNeg *= pow(10,100);
				}
			}
		}
		std::string id = line.substr(0, line.find('\t'));
		results << id + ',';
		if (divisorStandarMethod != 0) {
			results << float(probStandarMethod / divisorStandarMethod) << '\n';
		} else {
			results << float(probBayesianMethodPos / (probBayesianMethodPos + probBayesianMethodNeg)) << '\n';
		}
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
}
