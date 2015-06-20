#ifndef WDG_H
#define WDG_H

#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Misc.h"

class WordDocGenerator {
private:
	std::map<std::string, int > oneWordFrequency;
	std::map<std::string, int > twoWordFrequency;
	std::map<std::string, int > threeWordFrequency;
public:

	/* Cuenta la cantidad de ocurrencias por palabra de ciertos documentos
	 * y genera un archivo con las mismas pero ordenadas por frecuencia.
	 * Si se condidera mas de un Shingle genera un archivo por cada
	 * cantidad de Shingles que se consideren con sus respectivas frecuencias.
	 */
	void generateFrequencyDocument(int cantShingles, std::string label, std::string test);

};

#endif // WDG_H
