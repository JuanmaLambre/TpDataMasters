#ifndef MISC_H
#define MISC_H

#include <string>
#include <vector>


class Misc
{
	public:
		/** Saca los tags de HTML y borra puntuacion (no reemplaza por espacio).
			La idea es que todo texto que se deba procesar con una pasada
			se haga en una pasada, y no hacer una funcion para borrar los tags,
			y otra para sacar los apostrofes, etc **/
		static void processText(std::string& text);

		/** PRE: el texto deberia estar procesado por processText **/
		static void removeStopwords(std::string& text);

		static char& lowercase(char& unChar);

		static bool esPuntuacion(char& unChar);

		/** Dada la ruta del labeledTrainData crea un archivo nuevo con los
			reviews limpios: sin header y con los reviews procesados por
			processText(1) y por removeStopwords(1) **/
		static void limpiarLabeled(const std::string& inputFile,
				const std::string& outputFile);

		static std::vector<std::string> split(const std::string& text,
				const char& delimiter = ' ');

	private:
		static std::vector<std::string> stopwords;

		static void loadStopwords();
};

#endif // MISC_H
