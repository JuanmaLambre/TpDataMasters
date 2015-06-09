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
		Perceptron(const std::string& filename, int n=2, bool preprocesado = true);
		virtual ~Perceptron();

		/** Entrena a Perceptron con shingles de multiplicidad n (ver ctor) **/
		void entrenar();

		void calificar(const std::string& input, const std::string& output);

	private:
		bool textoPreprocesado;
		int multiplicity;
		std::ifstream labeledFile;
		std::map< std::string, std::pair<int,int> > frecuencias;

		/** Dado un texto lo convierte en un vector de shingles.
			PRE: El texto ya esta preprocesado**/
		std::list<std::string> shinglize(const std::string& rawText) const;
};

#endif // PERCEPTRON_H
