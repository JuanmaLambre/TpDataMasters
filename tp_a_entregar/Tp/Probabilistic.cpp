#include "Probabilistic.h"
#include "Misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

Probabilistic::Probabilistic(int quantWordByShingle, std::vector<std::string> fileFrequencyDocumentNames, std::string trainFile) {
	std::cout << "INICIALIZANDO BAYES..." << std::endl;
	this->quantWordByShingle = quantWordByShingle;
	this->labeledFile = trainFile;
	std::string line;

	if (quantWordByShingle >= 1) {					// Shingle de 1 palabra
		std::string nameFile = fileFrequencyDocumentNames[0];
		std::ifstream wordFile(nameFile.c_str());
		if (!wordFile.good()) {
			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			std::cout << "    " + nameFile + " is not good\n";
			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		}

		for(int i = 0; i < 75000; ++i) {
			std::getline(wordFile, line);
			std::string words = Misc::split(line, '\t')[0];
			if ((int)Misc::split(words, ' ').size() == 1) {
				std::string word = Misc::split(words, ' ')[0];
				oneWordSentiment[word] = std::pair<int, int> (0, 0);
			}
		}
	}
	if (quantWordByShingle >= 2) {					// Shingle de 2 palabra
		std::string nameFile = fileFrequencyDocumentNames[1];
		std::ifstream wordFile(nameFile.c_str());

		for(int i = 0; i < 315000; ++i) {

			std::getline(wordFile, line);
			std::string words = Misc::split(line, '\t')[0];

			if ((int)Misc::split(words, ' ').size() == 2) {
				std::string word1 = Misc::split(words, ' ')[0];
				std::string word2 = Misc::split(words, ' ')[1];
				twoWordSentiment[word1 + " " + word2] = std::pair<int, int> (0, 0);
			}
		}
	}
	if (quantWordByShingle >= 3) {					// Shingle de 3 palabra
		std::string nameFile = fileFrequencyDocumentNames[2];
		std::ifstream wordFile(nameFile.c_str());

		for(int i = 0; i < 85000; ++i) {
			std::getline(wordFile, line);
			std::string words = Misc::split(line, '\t')[0];
			if ((int)Misc::split(words, ' ').size() == 3) {
				std::string word1 = Misc::split(words, ' ')[0];
				std::string word2 = Misc::split(words, ' ')[1];
				std::string word3 = Misc::split(words, ' ')[2];
				threeWordSentiment[word1 + " " + word2 + " " + word3] = std::pair<int, int> (0, 0);
			}
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
	std::cout << "ENTRENANDO BAYES..." << std::endl;
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

		std::vector<std::string>::iterator itS;
		for (itS = simpleShingles.begin(); itS != simpleShingles.end(); ++itS) {

			if ((itS + 1) != simpleShingles.end() and (itS + 2) != simpleShingles.end()) {
				std::string threeWords = (*itS) + " " + (*(itS + 1)) + " " + (*(itS + 2));
				if (threeWordSentiment.count(threeWords) != 0) {
					std::pair<int, int> actualSentiment = threeWordSentiment[threeWords];
					if (sentiment == 1) {
						totalPositiveThreeWord += 1;
						threeWordSentiment[threeWords] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
					} else {
						totalNegativeThreeWord += 1;
						threeWordSentiment[threeWords] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
					}
				}
			}

			if ((itS + 1) != simpleShingles.end()) {
				std::string twoWords = (*itS) + " " + (*(itS + 1));
				if (twoWordSentiment.count(twoWords) != 0) {
					std::pair<int, int> actualSentiment = twoWordSentiment[twoWords];
					if (sentiment == 1) {
						totalPositiveTwoWord += 1;
						twoWordSentiment[twoWords] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
					} else {
						totalNegativeTwoWord += 1;
						twoWordSentiment[twoWords] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
					}
				}
			}
			std::string word = (*itS);
			if (oneWordSentiment.count(word) != 0) {
				std::pair<int, int> actualSentiment = oneWordSentiment[word];
				if (sentiment == 1) {
					totalPositiveOneWord += 1;
					oneWordSentiment[word] = std::pair<int, int> (1 + actualSentiment.first, actualSentiment.second);
				} else {
					totalNegativeOneWord += 1;
					oneWordSentiment[word] = std::pair<int, int> (actualSentiment.first, actualSentiment.second + 1);
				}
			}
		}
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
	std::cout << "\n";
}

void Probabilistic::calificar(const std::string& inputFilename) {
	std::cout << "CALIFICANDO BAYES..." << std::endl;
	std::ifstream testFile(inputFilename.c_str());
	std::string line;
	std::string review;
	std::vector<std::string> simpleShingles;
	int iteracion = 0;

	std::getline(testFile, line);
	while(std::getline(testFile, line)) {
		float probStandarMethod = 0;
		int divisorStandarMethod = 0;
		double probBayesianMethodPos = 0;
		double probBayesianMethodNeg = 0;

		review = line.substr(line.find('\t') + 1);
		Misc::processText(review);
		Misc::removeStopwords(review);
		simpleShingles =  Misc::split(review);

		float positive = 0;
		float negative = 0;
		std::vector<std::string>::iterator itS;
		for (itS = simpleShingles.begin(); itS != simpleShingles.end(); ++itS) {
			if ((itS + 1) != simpleShingles.end() and (itS + 2) != simpleShingles.end()) {
				std::string threeWords = (*itS) + " " + (*(itS + 1)) + " " + (*(itS + 2));
				if (threeWordSentiment.count(threeWords) != 0) {
					positive = threeWordSentiment[threeWords].first;
					negative = threeWordSentiment[threeWords].second;
					int divisorBayesianoPos = threeWordSentiment.size() + totalPositiveThreeWord;
					int divisorBayesianoNeg = threeWordSentiment.size() + totalNegativeThreeWord;
					if (positive != 0 or negative != 0) {
						// Calculamos por el Metodo Standar
						probStandarMethod += (positive / (positive + negative));
						divisorStandarMethod += 1;

						// Calculamos por el Metodo Bayesiano
						probBayesianMethodPos += log((positive + 1) / divisorBayesianoPos);
						probBayesianMethodNeg += log((negative + 1) / divisorBayesianoNeg);

					}
				}
			}

			if ((itS + 1) != simpleShingles.end()) {
				std::string twoWords = (*itS) + " " + (*(itS + 1));

				if (twoWordSentiment.count(twoWords) != 0) {
					positive = twoWordSentiment[twoWords].first;
					negative = twoWordSentiment[twoWords].second;
					int divisorBayesianoPos = twoWordSentiment.size() + totalPositiveTwoWord;
					int divisorBayesianoNeg = twoWordSentiment.size() + totalNegativeTwoWord;

					if (positive != 0 or negative != 0) {
						// Calculamos por el Metodo Standar
						probStandarMethod += (positive / (positive + negative));
						divisorStandarMethod += 1;

						// Calculamos por el Metodo Bayesiano
						probBayesianMethodPos += log((positive + 1) / divisorBayesianoPos);
						probBayesianMethodNeg += log((negative + 1) / divisorBayesianoNeg);

					}
				}
			}

			std::string word = (*itS);
			if (oneWordSentiment.count(word) != 0) {
				positive = oneWordSentiment[word].first;
				negative = oneWordSentiment[word].second;
				int divisorBayesianoPos = oneWordSentiment.size() + totalPositiveOneWord;
				int divisorBayesianoNeg = oneWordSentiment.size() + totalNegativeOneWord;

				if (positive != 0 or negative != 0) {
					// Calculamos por el Metodo Standar
					probStandarMethod += (positive / (positive + negative));
					divisorStandarMethod += 1;

					// Calculamos por el Metodo Bayesiano
					probBayesianMethodPos += log((positive + 1) / divisorBayesianoPos);
					probBayesianMethodNeg += log((negative + 1) / divisorBayesianoNeg);
				}
			}
		}
		std::string id = line.substr(0, line.find('\t'));


		probBayesianMethodPos += log(0.5);
		probBayesianMethodNeg += log(0.5);


		/*if (probBayesianMethodPos != 0 and probBayesianMethodNeg != 0) {
			double diference = probBayesianMethodPos - probBayesianMethodNeg;
			double probPos = (exp (diference) / (1 + exp(diference)));
			std::string sentimentText = line.substr(line.find('\t') + 1, 1);
			int sentiment = atoi(sentimentText.c_str());

			if ((0.9999999999999999 < probPos and probPos < 1) or (0 < probPos and probPos < 0.0000000000000001)) {
				cantidadReviewsCalificadas++;

				if ((sentiment == 1 and probPos > 0.5) or (sentiment == 0 and probPos < 0.5 )) {
					cantidadReviewsCalificadasBien++;
				}
			}

			//results << probPos << '\n';
		}*/

		if (divisorStandarMethod != 0) {
			float probPos = probStandarMethod / divisorStandarMethod;
			califications[id] = probPos;
		}

		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
	std::cout << "\n";
	testFile.close();
}

float Probabilistic::getReviewProba(const std::string& id) {
	if (califications.count(id) != 0) {
		return califications[id];
	} else {
		return -1;
	}
}

// STANDARD

// (0.55 < probPos and probPos < 1) or (0 < probPos and probPos < 0.45) ==>	2700 con 97.2%. 60k, 650k, 250k
// (0.59 < probPos and probPos < 1) or (0 < probPos and probPos < 0.41) ==> 1100 con 98.8%, 60k, 650k, 250k
// (0.575 < probPos and probPos < 1) or (0 < probPos and probPos < 0.425) ==> 1500 con 98.7, 80k, 650k, 250k

// (0.55 < probPos and probPos < 1) or (0 < probPos and probPos < 0.45) ==>	2550 con 97.88%. 75k, 320k, 82k
// (0.59 < probPos and probPos < 1) or (0 < probPos and probPos < 0.41) ==> 950 con 99%, 75k, 320k, 82k
// (0.575 < probPos and probPos < 1) or (0 < probPos and probPos < 0.425) ==> 1450 con 98.95, 75k, 320k, 82k
