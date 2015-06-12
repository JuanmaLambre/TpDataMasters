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
				   int n=2,
				   bool preprocesado = true);
		virtual ~Perceptron();

		/** Entrena a Perceptron con shingles de multiplicidad n (ver ctor) **/
		void entrenar(int iterations);

		void calificar(const std::string& input, const std::string& output);

	private:
		bool textoPreprocesado;
		int multiplicity;
		std::string labeledFile;
		std::ifstream oneWordFile;
		std::ifstream twoWordFile;
		std::map<std::string, int > W;

		/** Dado un texto lo convierte en un vector de shingles.
			PRE: El texto ya esta preprocesado**/
		std::list<std::string> shinglize(const std::string& rawText) const;
};

#endif // PERCEPTRON_H
