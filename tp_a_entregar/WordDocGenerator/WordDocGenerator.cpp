#include "WordDocGenerator.h"

void WordDocGenerator::generateFrequencyDocument(int cantShingles, std::string label, std::string test) {
	std::string line;
	std::ifstream trainFile;
	trainFile.open(label.c_str());

    std::string reviewId, reviewText, sentimentText;
    std::vector<std::string> simpleShingles;

    // CALCULAMOS LAS FRECUENCIAS DEL LABEL
    int iteracion = 0;
    while (std::getline(trainFile, line)) {

  	   // Shinglize el texto:
    	reviewText = line.substr( line.find('\t', line.find('\t')+1) + 1 );

		Misc::processText(reviewText);
		Misc::removeStopwords(reviewText);
		simpleShingles =  Misc::split(reviewText);		// simpleShingles considera solo 1 palabra.

		// Calculo el producto de W x Shingles
		std::vector<std::string>::iterator itShingles;

		for (itShingles = simpleShingles.begin(); itShingles != simpleShingles.end(); ++itShingles) {

			std::string word = (*itShingles);
			if (oneWordFrequency.count(word) != 0) {
				oneWordFrequency[word] += 1;
			} else {
				oneWordFrequency[word] = 1;
			}

			if (itShingles + 1 != simpleShingles.end()) {
				if ((*itShingles).size() != 0 and (*(itShingles + 1)).size() != 0) {
					std::string twoWord = (*itShingles) + " " + (*(itShingles + 1));
					if (twoWordFrequency.count(twoWord) != 0) {
						twoWordFrequency[twoWord] +=1;
					} else {
						twoWordFrequency[twoWord] =1;
					}
				}
			}
			if ((itShingles + 2 != simpleShingles.end()) and (itShingles + 1 != simpleShingles.end())) {
				if ((*itShingles).size() != 0 and (*(itShingles + 1)).size() != 0 and (*(itShingles + 2)).size() != 0) {
					std::string threeWord = (*itShingles) + " " + (*(itShingles + 1)) + " " + (*(itShingles + 2));
					if (threeWordFrequency.count(threeWord) != 0) {
						threeWordFrequency[threeWord] += 1;
					} else {
						threeWordFrequency[threeWord] = 1;
					}
				}
			}


		}
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
    trainFile.close();


	std::ifstream testFile;
	testFile.open(test.c_str());
	iteracion = 0;
    while (std::getline(testFile, line)) {

  	   // Shinglize el texto:
    	reviewText = line.substr(line.find('\t') + 1);

		Misc::processText(reviewText);
		Misc::removeStopwords(reviewText);
		simpleShingles =  Misc::split(reviewText);		// simpleShingles considera solo 1 palabra.

		// Calculo el producto de W x Shingles
		std::vector<std::string>::iterator itShingles;

		for (itShingles = simpleShingles.begin(); itShingles != simpleShingles.end(); ++itShingles) {

			std::string word = (*itShingles);
			if (oneWordFrequency.count(word) != 0) {
				oneWordFrequency[word] += 1;
			} else {
				oneWordFrequency[word] = 1;
			}

			if (itShingles + 1 != simpleShingles.end()) {
				std::string twoWord = (*itShingles) + " " + (*(itShingles + 1));
				if (twoWordFrequency.count(twoWord) != 0) {
					twoWordFrequency[twoWord] +=1;
				} else {
					twoWordFrequency[twoWord] =1;
				}
			}
			if ((itShingles + 2 != simpleShingles.end()) and (itShingles + 1 != simpleShingles.end())) {
				std::string threeWord = (*itShingles) + " " + (*(itShingles + 1)) + " " + (*(itShingles + 2));
				if (threeWordFrequency.count(threeWord) != 0) {
					threeWordFrequency[threeWord] += 1;
				} else {
					threeWordFrequency[threeWord] = 1;
				}
			}

		}
		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
    testFile.close();

    std::ofstream oneDoc("OneWordsFrequency.tsv");
    std::ofstream twoDoc("TwoWordsFrequency.tsv");
    std::ofstream threeDoc("ThreeWordsFrequency.tsv");

    std::set< std::pair<int, std::string> > threeInverse;
    std::set< std::pair<int, std::string> > twoInverse;
    std::set< std::pair<int, std::string> > oneInverse;

    std::map<std::string, int >::iterator it;
    for (it = oneWordFrequency.begin(); it != oneWordFrequency.end(); ++it) {
    	std::pair<int, std::string> par ((*it).second, (*it).first);
    	oneInverse.insert(par);
    }

    std::set< std::pair<int, std::string> >::iterator itPar;
    itPar = oneInverse.end();
    itPar--;
    for (; itPar != oneInverse.begin(); --itPar) {
    	if ((int)Misc::split((*itPar).second, ' ').size() == 1) {
    		oneDoc << (*itPar).second << '\t' << (*itPar).first << "\n";
    	}
    }
    oneDoc << (*itPar).second << '\t' << (*itPar).first << "\n";


    std::map<std::string, int >::iterator it2;
    for (it2 = twoWordFrequency.begin(); it2 != twoWordFrequency.end(); ++it2) {
    	std::pair<int, std::string> par ((*it2).second, (*it2).first);
    	twoInverse.insert(par);
    }
    std::set< std::pair<int, std::string> >::iterator itPar2;
    itPar2 = twoInverse.end();
    itPar2--;
    printf("Total Frec: %d \n",(int)twoWordFrequency.size());
    printf("Total: %d \n",(int)twoInverse.size());
    for (; itPar2 != twoInverse.begin(); --itPar2) {
    	if ((int)Misc::split((*itPar2).second, ' ').size() == 2) {
    		std::vector<std::string> words = Misc::split((*itPar2).second, ' ');
			if (words[0].size() != 0 and words[1].size() != 0) {
				twoDoc << (*itPar2).second << '\t' << (*itPar2).first << "\n";
			}
    	}
    }


    std::map<std::string, int >::iterator it3;
    for (it3 = threeWordFrequency.begin(); it3 != threeWordFrequency.end(); ++it3) {
    	std::pair<int, std::string> par ((*it3).second, (*it3).first);
    	threeInverse.insert(par);
    }

    std::set< std::pair<int, std::string> >::iterator itPar3;
    itPar3 = threeInverse.end();
    itPar3--;
    for (; itPar3 != threeInverse.begin(); --itPar3) {
    	if ((int)Misc::split((*itPar3).second, ' ').size() == 3) {
    		std::vector<std::string> words = Misc::split((*itPar3).second, ' ');
			if (words[0].size() != 0 and words[1].size() != 0 and words[2].size() != 0) {
				threeDoc << (*itPar3).second << '\t' << (*itPar3).first << "\n";
			}
    	}
    }

}
