#include "Perceptron.h"
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>
#include "Misc.h"

#define FREQUENCY_FILENAME "shingles_frequencies.txt"
#define SCORES_FILENAME "scores.txt"
#define QUANT_ONE_SINGLES   167418
#define QUANT_TWO_SINGLES   3161248
#define QUANT_THREE_SINGLES 5389045
#define QUANT_REVIEWS 25000

Perceptron::Perceptron(const std::string &filename,
					   const std::string &oneWordsDoc,
					   const std::string &twoWordsDoc,
					   const std::string &threeWordsDoc,
					   int n,
					   bool preprocesado)
		:oneWordFile(oneWordsDoc.c_str()), twoWordFile(twoWordsDoc.c_str()), threeWordFile(threeWordsDoc.c_str()){
	std::cout << "INICIALIZANDO PERCEPTRON..." << std::endl;
	Misc misc;
	labeledFile = filename;
	textoPreprocesado = preprocesado;
	multiplicity = n;

	if (preprocesado) {
		std::string line;

		// Considerando 1 palabra
		for(int i = 0; i < QUANT_ONE_SINGLES; ++i) {
			std::getline(oneWordFile, line);
			std::string word = misc.split(line, '\t')[0];
			W[word] = 0;
		}

		// Considerando 2 palabras
		if (n >= 2) {
			for(int i = 0; i < QUANT_TWO_SINGLES; ++i) {
				getline(twoWordFile, line);
				std::string words = misc.split(line, '\t')[0];
				if ((int)misc.split(words, ' ').size() == 2) {
					std::string word1 = misc.split(words, ' ')[0];
					std::string word2 = misc.split(words, ' ')[1];
					W[word1 + " " + word2] = 0;
				}
			}
		}

		// Considerando 3 palabras
		if (n >= 3) {
			printf("3 \n");
			for(int i = 0; i < QUANT_THREE_SINGLES; ++i) {
				getline(threeWordFile, line);
				std::string words = misc.split(line, '\t')[0];
				if ((int)misc.split(words, ' ').size() == 3) {
					std::string word1 = misc.split(words, ' ')[0];
					std::string word2 = misc.split(words, ' ')[1];
					std::string word3 = misc.split(words, ' ')[2];
					W[word1 + " " + word2 + " " + word3] = 0;
				}
			}
		}
	}
	oneWordFile.close();
	twoWordFile.close();
	threeWordFile.close();
}

void Perceptron::entrenar(int iterations) {
	std::cout << "ENTRENANDO PERCEPTRON..." << std::endl;
	std::string line, header;
	float iteracion = 0;
	int errors = QUANT_REVIEWS;

	while(errors > 0 and iteracion < iterations) {
		errors = 0;
		iteracion++;

		std::ifstream trainFile;
		trainFile.open(labeledFile.c_str());

		if (!textoPreprocesado) {
			trainFile >> header >> header >> header;
			trainFile.ignore('\n', 1);
		}
        std::string reviewId, reviewText, sentimentText;
        int sentiment;
        int nroReview = 0;
        std::vector<std::string> simpleShingles;
        while (std::getline(trainFile, line)) {
        	nroReview++;
			// SEPARO LA REVIEWS EN SHINGLES DE 1 PALABRA
			reviewText = line.substr( line.find('\t', line.find('\t')+1) + 1 );
			if (!textoPreprocesado) {
				Misc::processText(reviewText);
				Misc::removeStopwords(reviewText);
			}
			simpleShingles =  Misc::split(reviewText);

			// CALCULO EL PRODUCTO DE W x EL VECTOR DE SHINGLES
			int product = 0;
			std::vector<std::string>::iterator itShingles;
			for (itShingles = simpleShingles.begin(); itShingles != simpleShingles.end(); ++itShingles) {
				std::string word = (*itShingles);
				if (W.count(word) != 0) {
					product += W[word];
				}
				if (itShingles + 1 != simpleShingles.end()) {
					std::string twoWord = (*itShingles) + " " + (*(itShingles + 1));
					if (W.count(twoWord) != 0) {
						product += W[twoWord];
					}
				}
				if ((itShingles + 2 != simpleShingles.end()) and (itShingles + 1 != simpleShingles.end())) {
					std::string threeWord = (*itShingles) + " " + (*(itShingles + 1)) + " " + (*(itShingles + 2));
					if (W.count(threeWord) != 0) {
						product += W[threeWord];
					}
				}
			}
			sentimentText = line.substr(line.find('\t') + 1, 1);
			sentiment = atoi(sentimentText.c_str());

			// ACTUALIZO W EN CASO QUE NO HAYA CALIFICADO BIEN
			if ((sentiment == 0 and product > 0) or (sentiment == 1 and product <= 0)) {
				errors++;
				int recalculate;
				if (sentiment == 1) {
					recalculate = 1;
				} else {
					recalculate = -1;
				}

				// RECALCULO W
				for (itShingles = simpleShingles.begin(); itShingles != simpleShingles.end(); ++itShingles) {
					std::string word = (*itShingles);
					if (W.count(word) != 0) {
						W[word] += recalculate;
					}
					if (itShingles + 1 != simpleShingles.end()) {
						std::string twoWord = (*itShingles) + " " + (*(itShingles + 1));
						if (W.count(twoWord) != 0) {
							W[twoWord] += recalculate;
						}
					}
					if ((itShingles + 2 != simpleShingles.end()) and (itShingles + 1 != simpleShingles.end())) {
						std::string threeWord = (*itShingles) + " " + (*(itShingles + 1)) + " " + (*(itShingles + 2));
						if (W.count(threeWord) != 0) {
							W[threeWord] += recalculate;
						}
					}
				}
			}
        }
        trainFile.close();
        std::cout << "Iteracion: " << iteracion << " Cant Errores: " << errors << std::endl;
	}

}


void Perceptron::calificar(const std::string& testFilename) {
	std::ifstream testFile(testFilename.c_str());
	std::string id, review, header, line;
	std::vector<std::string> reviewSimpleShingles;
	std::map<std::string, int> idProduct;
	int iteracion = 0, maxProduct = 0, minProduct = 0;

    std::cout << "CALIFICANDO PERCEPTRON...\n";

	std::getline(testFile, line);
	while (std::getline(testFile, line)) {
		// SEPARO LA REVIEWS EN SHINGLES DE 1 PALABRA
		id = line.substr(0, line.find('\t'));
		review = line.substr(line.find('\t') + 1);
		Misc::processText(review);
		Misc::removeStopwords(review);
		reviewSimpleShingles = Misc::split(review);

		// PROCESADO DE REVIEW:
		// (1) OBTENGO EL PRODUCTO W x VECTOR DE SHINGLES:
		int product = 0;
		std::vector<std::string>::iterator itShingles;
		for (itShingles = reviewSimpleShingles.begin(); itShingles != reviewSimpleShingles.end(); ++itShingles) {
			std::string word = (*itShingles);
			if (W.count(word) != 0) {
				product += W[word];
			}
			if (itShingles + 1 != reviewSimpleShingles.end()) {
				std::string twoWord = (*itShingles) + " " + (*(itShingles + 1));
				if (W.count(twoWord) != 0) {
					product += W[twoWord];
				}
			}
			if ((itShingles + 2 != reviewSimpleShingles.end()) and (itShingles + 1 != reviewSimpleShingles.end())) {
				std::string threeWord = (*itShingles) + " " + (*(itShingles + 1)) + " " + (*(itShingles + 2));
				if (W.count(threeWord) != 0) {
					product += W[threeWord];
				}
			}

		}
		//	(2) ACTUALIZO LOS PRODUCTOS MAXIMOS Y MINIMOS PARA NORMALIZAR LAS PROBABILIDADES
		if (minProduct > product) {
			minProduct = product;
		}
		if (maxProduct < product) {
			maxProduct = product;
		}
		//	(3) GUARDO EL PRODUCTO POR CADA ID
		idProduct[id] = product;
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
	std::map<std::string, int>::iterator itIds;
	for (itIds = idProduct.begin(); itIds != idProduct.end(); ++itIds ) {
		califications[(*itIds).first] = float(((*itIds).second - minProduct)) / float((maxProduct - minProduct));
	}

	std::cout << "\r100%\n";
	testFile.close();
}

float Perceptron::getReviewProba(const std::string& id) {
	if (califications.count(id) != 0) {
		return califications[id];
	} else {
		return -1;
	}
}

/******************************* PRIVATE *************************************/

std::list<std::string> Perceptron::shinglize(const std::string& text) const {
	std::list<std::string> shingles;
	std::vector<std::string> splitedText;

	splitedText = Misc::split(text);
	int palabraNro = -1;
	while (++palabraNro < (int)splitedText.size()-multiplicity+1) {
		std::string shingle = "";
		for (int it=palabraNro; it<multiplicity+palabraNro; ++it) {
			shingle += splitedText[it] + " ";
		}
		shingle.erase(shingle.end()-1);
		shingles.push_back(shingle);
	}

	return shingles;
}






