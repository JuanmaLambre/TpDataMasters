#include "WordDocGenerator.h"
#include "Misc.h"

int main() {
	std::string label = "labeledTrainData.tsv";
	std::string test = "testData.tsv";
	WordDocGenerator wdg;
	wdg.generateFrequencyDocument(3, label, test);
	return 0;
}
