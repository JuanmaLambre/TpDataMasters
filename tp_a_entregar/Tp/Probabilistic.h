#include <map>
#include <string>
#include <vector>

class Probabilistic {
private:
	int quantWordByShingle;
	int totalPositiveOneWord;
	int totalNegativeOneWord;
	int totalPositiveTwoWord;
	int totalNegativeTwoWord;
	int totalPositiveThreeWord;
	int totalNegativeThreeWord;
	std::string labeledFile;
	std::map<std::string, std::pair<int, int> > oneWordSentiment;
	std::map<std::string, std::pair<int, int> > twoWordSentiment;
	std::map<std::string, std::pair<int, int> > threeWordSentiment;
	std::map<std::string, float> califications;
public:
	Probabilistic(int quantWordByShingle, std::vector<std::string> fileFrequencyDocumentNames, std::string trainFile);

	void entrenar();

	void calificar(const std::string& inputFilename);

	float getReviewProba(const std::string& id);
};
