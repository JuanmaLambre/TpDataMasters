#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <fstream>
#include "definitions.h"


class Perceptron
{
	public:
		Perceptron(const std::string &filename,
				   const std::string &oneWordsDoc,
				   const std::string &twoWordsDoc,
				   int n=3,
				   bool preprocesado=true);

		/** Entrena a Perceptron con shingles de multiplicidad n (ver ctor) **/
		void entrenar(int iterations);

		void calificar(ReviewsList& reviews, std::ofstream& output);

	private:
		bool textoPreprocesado;
		int multiplicity;
		std::string labeledFile;
		std::ifstream oneWordFile;
		std::ifstream twoWordFile;
		std::ifstream threeWordFile;
		std::map<std::string, int > W;

		/** Dado un texto lo convierte en un vector de shingles.
			PRE: El texto ya esta preprocesado**/
		std::list<std::string> shinglize(const std::string& rawText) const;
};

#endif // PERCEPTRON_H
