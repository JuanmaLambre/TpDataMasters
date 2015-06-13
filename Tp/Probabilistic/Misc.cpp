#include "Misc.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#define STOPWORDS_FILENAME "stopWords"

std::vector<std::string> Misc::stopwords;


void Misc::processText(std::string& text) {
    std::string::iterator it = text.begin();

    while (it != text.end()) {
		lowercase(*it);

		if (*it == '<') {
			// Tag de HTML encontrado
			int itPos = std::distance(text.begin(), it);
			int closeTagPos = text.find('>', itPos);
			text.erase(itPos, closeTagPos - itPos + 1);
		} else if (esPuntuacion(*it)) {
			text.erase(it);
		} else {
			++it;
		}
    }
}


void Misc::removeStopwords(std::string& text) {
	if (stopwords.empty())
		loadStopwords();
	// Voy a recorrer palabra por palabra del string text
	std::string word;
	std::string::iterator it = text.begin();
	while (it != text.end()) {
		int itPos = std::distance(text.begin(), it);
		int nextSpacePos = text.find(' ', itPos);
		word = text.substr(itPos, nextSpacePos - itPos);
		std::vector<std::string>::iterator stopwordIt =
				std::find(stopwords.begin(), stopwords.end(), word);
		if (stopwordIt != stopwords.end()) {
			// Se encontro la stopword
			text.erase(itPos, nextSpacePos - itPos + 1);
		} else {
			it += word.size();
			if (it != text.end())
				++it; // Me salteo el espacio (si hay)
		}
	}

}


char& Misc::lowercase(char& unChar) {
	if (unChar <= 'Z' and unChar >= 'A')
		unChar += 'a' - 'A';
	return unChar;
}


bool Misc::esPuntuacion(char& unChar) {
	lowercase(unChar);
	return ! ( ((unChar >= 'a') and (unChar <= 'z'))
			or ( (unChar >= '0') and (unChar <= '9') )
			or (unChar == '/')
			or (unChar == ' ') );
}


void Misc::limpiarLabeled(const std::string& input, const std::string& output) {
	std::ifstream labeled(input.c_str());
	std::ofstream cleanLabeled(output.c_str());
	std::string header, line;
	std::vector<std::string> lineVec;
	int bytesLeidos = 0;
	int bytesTotales = 33556378; // Hardcodeadisimo

	std::cout << "LIMPIANDO LABELED...\n";
	// Header:
	labeled >> header >> header >> header;
	labeled.ignore(1,'\n');

	while (labeled.peek() != EOF) {
		std::getline(labeled, line);
		lineVec = split(line, '\t');
		processText(lineVec[2]);
		removeStopwords(lineVec[2]);
		cleanLabeled << lineVec[0] + '\t' << lineVec[1] << '\t' << lineVec[2];

		labeled.ignore(1, '\n');
		bytesLeidos += line.size() + 1;
		std::cout << "\r" << (int)(bytesLeidos*100.0/bytesTotales) << "%";
		if (labeled.peek() != EOF)
			cleanLabeled << '\n';
	}
	std::cout << "\r100%\n";
}


std::vector<std::string> Misc::split(const std::string& text,
		const char& delimiter) {
	std::vector<std::string> splited;
	std::string word;
	int i=0;

	while (i < (int)text.size()) {
		int nextDelimiterPos = text.find(delimiter, i);
		word = text.substr(i, nextDelimiterPos - i);
		splited.push_back(word);
		i += word.size() + 1;
	}
	return splited;
}



/******************************* PRIVATE *************************************/

void Misc::loadStopwords() {
    std::ifstream stopwordsFile(STOPWORDS_FILENAME);
	std::string stopword;

	for ( ; !stopwordsFile.eof() ; stopwordsFile.ignore(1,'\n')) {
		stopwordsFile >> stopword;
		stopwords.push_back(stopword);
	}
}
