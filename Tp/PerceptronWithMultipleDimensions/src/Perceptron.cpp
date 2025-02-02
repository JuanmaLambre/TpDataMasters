#include "Perceptron.h"
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>
#include "Misc.h"

#define FREQUENCY_FILENAME "shingles_frequencies.txt"
#define SCORES_FILENAME "scores.txt"


Perceptron::Perceptron(const std::string &filename,
					   const std::string &oneWordsDoc,
					   const std::string &twoWordsDoc,
					   const std::string &threeWordsDoc,
					   int n,
					   bool preprocesado)
		:oneWordFile(oneWordsDoc.c_str()), twoWordFile(twoWordsDoc.c_str()), threeWordFile(threeWordsDoc.c_str()){
	std::cout << "INICIALIZANDO..." << std::endl;
	Misc misc;
	labeledFile = filename;
	textoPreprocesado = preprocesado;
	multiplicity = n;

	if (preprocesado) {
		std::string line;

		// Considerando 1 palabra
		std::getline(oneWordFile, line);
		for(int i = 0; i < 102707; ++i) {
			std::getline(oneWordFile, line);
			std::string word = misc.split(line, '\t')[0];
			float random = (rand() % 10) - 5;
			W[word] = random;
		}

		// Considerando 2 palabras
		if (n >= 2) {
			getline(twoWordFile, line);
			for(int i = 0; i < 3145919; ++i) {
				getline(twoWordFile, line);
				std::string word1 = misc.split(line, '\t')[0];
				std::string word2 = misc.split(line, '\t')[1];
				float random = (rand() % 10) - 5;
				W[word1 + " " + word2] = random;
			}
		}

		// Considerando 3 palabras
		if (n >= 3) {
			getline(threeWordFile, line);
			for(int i = 0; i < 5636985; ++i) {
				getline(threeWordFile, line);
				std::string word1 = misc.split(line, '\t')[0];
				std::string word2 = misc.split(line, '\t')[1];
				std::string word3 = misc.split(line, '\t')[2];
				float random = (rand() % 10) - 5;
				W[word1 + " " + word2 + " " + word3] = random;
			}
		}
	}
	oneWordFile.close();
	twoWordFile.close();
	threeWordFile.close();
}

Perceptron::~Perceptron(){

}

void Perceptron::entrenar(int iterations) {
	std::cout << "ENTRENANDO..." << std::endl;
	std::string line, header;
	float iteracion = 0;
	int errors = 25000;

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
        std::vector<std::string> simpleShingles;
        int nroReview = 0;

        while (std::getline(trainFile, line)) {
        	nroReview++;
			// Shinglize el texto:
			reviewText = line.substr( line.find('\t', line.find('\t')+1) + 1 );
			if (!textoPreprocesado) {
				Misc::processText(reviewText);
				Misc::removeStopwords(reviewText);
			}
			simpleShingles =  Misc::split(reviewText);		// simpleShingles considera solo 1 palabra.

			// Calculo el producto de W x Shingles
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

			// Actualizo W en caso que no haya calificado bien.
			if ((sentiment == 0 and product > 0) or (sentiment == 1 and product <= 0)) {
				errors++;
				int recalculate;
				if (sentiment == 1) {
					recalculate = 1;
				} else {
					recalculate = -1;
				}

				// Recalculo W
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


void Perceptron::calificar(const std::string& testFilename,
		const std::string& outputFilename) {
	std::ifstream testFile(testFilename.c_str());
	std::ofstream results(outputFilename.c_str());
	std::string id, review, header, line;
	std::vector<std::string> reviewSimpleShingles;
	std::map<std::string, int> idProduct;
	int iteracion = 0, maxProduct = 0, minProduct = 0;

    std::cout << "CALIFICANDO...\n";
    results << "\"id\",\"sentiment\"\n";

	std::getline(testFile, line);
	while (std::getline(testFile, line)) {
		// Preprocesado del review:
		id = line.substr(0, line.find('\t'));
		review = line.substr(line.find('\t') + 1);
		Misc::processText(review);
		Misc::removeStopwords(review);
		reviewSimpleShingles = Misc::split(review);

		// Procesado de review (multiplicacion de reviewSingles x W):
		//	(1) Obtengo el producto:

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
		//	(2) Actualizo productos maximos y minimos para luego normalizar:
		if (minProduct > product) {
			minProduct = product;
		}
		if (maxProduct < product) {
			maxProduct = product;
		}
		//	(3) Guardo el producto por cada id:
		idProduct[id] = product;
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
	std::map<std::string, int>::iterator itIds;
	for (itIds = idProduct.begin(); itIds != idProduct.end(); ++itIds ) {
		results << (*itIds).first + ',';
		results << float(((*itIds).second - minProduct)) / float((maxProduct - minProduct)) << '\n';
	}

	std::cout << "\r100%\n";
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






