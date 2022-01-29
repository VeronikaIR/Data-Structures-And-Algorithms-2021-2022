#include <iostream>
#include "../HW_3-Tests/interface.h"

int main(int argc, char* argv[])
{
	if (argc == 3) {
		for (int i = 1; i < argc; i++) {
			std::cout << argv[i] << std::endl;
		}
	}
	else {
		std::cout << "Invalid number of arguments!" << std::endl;
	}

	Comparator comparator;
	ComparisonReport report;

	std::ifstream myfile1(argv[1]);
	std::ifstream myfile2(argv[2]);

	if (myfile1.is_open()){
		if (myfile2.is_open())
		{
			report = comparator.compare(myfile1, myfile2);

			myfile1.close();
			myfile2.close();
		}
		else {
			myfile1.close();
			std::cout << "File 2 is not found!" << std::endl;
		}
	}
	else {
		std::cout << "File 1 is not found!" << std::endl;

	}


	size_t commonWords = report.commonWords.words().size();
	size_t allWordsInFile1 = commonWords + report.uniqueWords[0].words().size();
	size_t allWordsInFile2 = commonWords + report.uniqueWords[1].words().size();

	int percent1 = (allWordsInFile1 == 0) ? 0:(commonWords /double(allWordsInFile1)) * 100;
	int percent2 = (allWordsInFile2 == 0) ? 0:(commonWords /double(allWordsInFile2)) * 100;

	std::cout << "file 1 contains " << allWordsInFile1 << " words and " << commonWords << " of them contains in file2 ("<< percent1  <<"%)" << std::endl;
	std::cout << "file 2 contains " << allWordsInFile2 << " words and " << commonWords << " of them contains in file1 (" << percent2 << "%)" << std::endl;

	return 0;
}