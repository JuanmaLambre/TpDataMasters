#include "Perceptron.h"
#include <cstdlib>
#include <vector>
#include "Misc.h"

#define FREQUENCY_FILENAME "shingles_frequencies.txt"
#define SCORES_FILENAME "scores.txt"


Perceptron::Perceptron(const std::string&filename, int n, bool preprocesado)
		: labeledFile(filename.c_str()) {
	textoPreprocesado = preprocesado;
	multiplicity = n;
	if (!textoPreprocesado) {
		std::string idHeader, sentimentHeader, reviewHeader;
		labeledFile >> idHeader >> sentimentHeader >> reviewHeader;
	}
}


Perceptron::~Perceptron() {
	labeledFile.close();
}


void Perceptron::entrenar() {
	std::cout << "ENTRENANDO..." << std::endl;
	std::string line, header;
	float iteracion = 0.0;

	if (!textoPreprocesado) {
		labeledFile >> header >> header >> header;
		labeledFile.ignore('\n', 1);
	}

	while(!labeledFile.eof()) {
        std::string reviewId, reviewText, sentimentText;
        int sentiment;
        std::list<std::string> shingles;

		// Shinglize el texto:
        std::getline(labeledFile, line);
		reviewText = line.substr( line.find('\t', line.find('\t')+1) + 1 );
        if (!textoPreprocesado) {
			Misc::processText(reviewText);
			Misc::removeStopwords(reviewText);
		}
		shingles = shinglize(reviewText);

		// Y agrego los shingles a las frecuencias
		sentimentText = line.substr(line.find('\t') + 1, 1);
		sentiment = atoi(sentimentText.c_str());
		bool isNegative = sentiment == 0, isPositive = !isNegative;
        std::list<std::string>::iterator it = shingles.begin();
        for (; it != shingles.end(); ++it) {
			std::pair<int,int> contadores = frecuencias[*it];
			frecuencias[*it] = std::make_pair(
					contadores.first + isNegative, contadores.second + isPositive);
        }

		labeledFile.ignore(1, '\n');
		std::cout << "\r" << (int)(++iteracion*100/25000) << '%';
	}
	std::cout << "\r100%\n";

	// Ahora guardo un archivo guardando los resultados:
	std::ofstream frequenciesFile(FREQUENCY_FILENAME);
	frequenciesFile << "SHINGLE\t#NEG\t#POS\n";
	std::map< std::string, std::pair<int,int> >::iterator it = frecuencias.begin();
	for (; it != frecuencias.end(); ++it) {
		frequenciesFile << it->first + "\t"
				<< it->second.first
				<< '\t'
				<< it->second.second
				<< '\n';
	}
}


void Perceptron::calificar(const std::string& testFilename,
		const std::string& outputFilename) {
	std::ifstream testFile(testFilename.c_str());
	std::ofstream results(outputFilename.c_str()), scores(SCORES_FILENAME);
	std::string id, review, header, line;
	std::list<std::string> reviewShingles;
	int iteracion = 0, cantPositivos = 0, cantNegativos = 0;

	scores << "NEG\tPOS\n";
    std::cout << "CALIFICANDO...\n";
    results << "\"id\",\"sentiment\"\n";
	testFile >> header >> header;
	testFile.ignore(1, '\n');

	while (testFile.peek() != EOF) {
		// Preprocesado del review:
		std::getline(testFile, line);
		id = line.substr(0, line.find('\t'));
		review = line.substr(line.find('\t') + 1);
		Misc::processText(review);
		Misc::removeStopwords(review);
		reviewShingles = shinglize(review);

		results << id + ',';
		// Procesado de review (calificacion):
		//	(1) Obtengo los puntajes:
		int puntajePos=0, puntajeNeg=0;
		std::list<std::string>::iterator it = reviewShingles.begin();
		for (; it != reviewShingles.end(); ++it) {
			puntajePos += frecuencias[*it].second;
			puntajeNeg += frecuencias[*it].first;
		}
		//	(2) Califico:
		bool calificadoPositivo;
		if (puntajeNeg == puntajePos)
			// Si hubo mas negativos, estadisticamente predigo que es positivo
			calificadoPositivo = cantPositivos < cantNegativos;
		else
			calificadoPositivo = puntajePos > puntajeNeg;
		results << calificadoPositivo << '\n';
		cantNegativos += !calificadoPositivo;
		cantPositivos += calificadoPositivo;
		scores << cantNegativos << '\t' << cantPositivos << std::endl;

		std::cout << "\r" << (int)(++iteracion*100.0/25000) << '%';
	}
	std::cout << "\r100%\n";
}


/******************************* PRIVATE *************************************/

std::list<std::string> Perceptron::shinglize(const std::string& text) const {
	std::list<std::string> shingles;
	std::vector<std::string> splitedText;

	splitedText = Misc::split(text);
	int palabraNro = -1;
	while (++palabraNro < splitedText.size()-multiplicity+1) {
		std::string shingle = "";
		for (int it=palabraNro; it<multiplicity+palabraNro; ++it) {
			shingle += splitedText[it] + " ";
		}
		shingle.erase(shingle.end()-1);
		shingles.push_back(shingle);
	}

	return shingles;
}






