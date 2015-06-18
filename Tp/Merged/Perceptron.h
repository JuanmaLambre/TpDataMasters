#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <fstream>


class Perceptron
{
	public:
		Perceptron(const std::string &filename,
				   const std::string &oneWordsDoc,
				   const std::string &twoWordsDoc,
				   const std::string &threeWordsDoc,
				   int n=3,
				   bool preprocesado = true);
		virtual ~Perceptron();

		/** Entrena a Perceptron con shingles de multiplicidad n (ver ctor) **/
		void entrenar(int iterations);

		void calificar(const std::string& input);

		float getReviewProba(const std::string& id);

	private:
		bool textoPreprocesado;
		int multiplicity;
		std::string labeledFile;
		std::ifstream oneWordFile;
		std::ifstream twoWordFile;
		std::ifstream threeWordFile;
		std::map<std::string, int > W;
		std::map<std::string, float> califications;

		/** Dado un texto lo convierte en un vector de shingles.
			PRE: El texto ya esta preprocesado**/
		std::list<std::string> shinglize(const std::string& rawText) const;
};

#endif // PERCEPTRON_H
