#pragma once

#include "../Homework 2 -Tests/interface.h"
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;

vector<string> split(const string& input, const char& symbol);

struct HierarchyRecord {

	Hierarchy* hierarchy;
	bool hasChanged = false;
	bool isSaved = false;

	~HierarchyRecord() {
		delete hierarchy;
	}
};

class ConsoleApp {
private:
	vector<HierarchyRecord*> hierarchies;

public:

	void startUp();
	~ConsoleApp();
};

ConsoleApp::~ConsoleApp() {
	for (int i = 0; i < hierarchies.size(); i++) {
		delete hierarchies[i];
	}
}

void ConsoleApp::startUp() {

	while (true) {

		std::cin.clear();
		std::cout << "Type function (type 'help' for a list of functions):" << std::endl;
		string input;
		std::getline(std::cin, input);

		vector<string> arguments;
		arguments = split(input, ' ');

		string command = arguments[0];

		if (command == "help") {

			if (arguments.size() == 1) {

				std::cout << "This project supports the following commands:" << std::endl;
				std::cout << "1: load <object_name> <file_name> - loads hierarchy data from a file with the provided name and creates a tree associated with object_name" << std::endl;
				std::cout << "2: save <object_name> <file_name> - saves the hierarchy information of the specified object in a file with the specified name" << std::endl;
				std::cout << "3: find <object_name> <subordinate_name> - checks whether there is an employee with the specified name in the specified object" << std::endl;
				std::cout << "4: num_subordinates <object_name> <subordinate_name> - returns the number of direct subordinates of the employee in the specified object" << std::endl;
				std::cout << "5: manager <object_name> <subordinate_name> - returns the name of the manager of the employee in the specified object" << std::endl;
				std::cout << "6: num_employees <object_name> - returns the number of employees in the specified object" << std::endl;
				std::cout << "7: print <object_name> - returns the content of tree" << std::endl;
				std::cout << "8: longest_chain <object_name> - returns the longest chain in tree" << std::endl;
				std::cout << "9: num_overloaded <object_name> - returns the number of employees in the specified site for which the number of subordinates (direct or not) is greater than 20" << std::endl;
				std::cout << "10: join <object_name_1> <object_name_2> <object_name_result> - merges the two submitted objects into a new object named object_name_result" << std::endl;
				std::cout << "11: fire <object_name> <subordinate_name> - removes the employee from the specified object" << std::endl;
				std::cout << "12: hire <object_name> <subordinate_name> - <manager_name> appoints the employee in the specified object as subordinate to the submitted manager" << std::endl;
				std::cout << "13: salary <object_name> <subordinate_name> - returns the employee's salary" << std::endl;
				std::cout << "14: incorporate <object_name> - incorporates the company; the operation is applied to the object object_name" << std::endl;
				std::cout << "15: modernize <object_name> - modernizes the company; the operation is applied to the object object_name" << std::endl;
				std::cout << "16: exit - terminates the program" << std::endl;
			}
			else { std::cout << "Invalid arguments!" << std::endl; }
		}
		else if (command == "load") {

			if (arguments.size() == 2 || arguments.size() == 3) {
				string objectName = arguments[1];

				if (arguments.size() == 2) {
					string str;
					string result;

					while (std::getline(std::cin, str)) {
						if(str[0] == EOF){
							break;
						}
						result += str + '\n';
					}
				
					Hierarchy* currentHierarchy;
					try {
						currentHierarchy = new Hierarchy(result);
					}
					catch (std::invalid_argument ex) {
						std::cerr << ex.what() << std::endl;
						continue;
					}
					currentHierarchy->setNameOfTheHierarchy(objectName);
					HierarchyRecord* current = new HierarchyRecord{ currentHierarchy, false, false };
					hierarchies.push_back(current);

					std::cout << objectName << " is loaded successfully!" << std::endl;
				}

				if (arguments.size() == 3) {

					string fileName = arguments[2];
					string result, input;
					std::ifstream myfile(fileName);
					if (myfile.is_open())
					{
						while (std::getline(myfile, input))
						{
							bool isWhiteSpaceLine = true;
							for (char s : input) {
								if (s != ' ') { isWhiteSpaceLine = false; break; }
							 }
							if (isWhiteSpaceLine) { continue; }

							result += input + "\n";
						}
						myfile.close();
					}
					else { std::cout << "File is not found!" << std::endl; continue; }

					if (result != "") {
						Hierarchy* currentHierarchy;
						try {
							currentHierarchy = new Hierarchy(result);
						}
						catch (std::invalid_argument ex) {
							std::cerr << ex.what() << std::endl;
							continue;
						}
						currentHierarchy->setNameOfTheHierarchy(objectName);
						HierarchyRecord* current = new HierarchyRecord{ currentHierarchy, false,true };
						hierarchies.push_back(current);

						std::cout << objectName << " is loaded successfully!" << std::endl;
					}
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "save") {

			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string fileName = arguments[2];

				string hierarchyStr;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						hierarchies[i]->hasChanged = false;
						hierarchies[i]->isSaved = true;
						hierarchyStr = hierarchies[i]->hierarchy->print();
					}
				}

				std::ofstream myFile(fileName);
				if (myFile.is_open()) {
					myFile << hierarchyStr;

					myFile.close();
					std::cout << objectName << " is saved successfully!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "find") {

			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName) {
						std::cout << subordinateName << " is employed in " << objectName << " ." << std::endl;
						break;
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "num_subordinates") {

			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName) {
						if (hierarchies[i]->hierarchy->num_subordinates(subordinateName) == 0) {
							std::cout << subordinateName << " has no subordinates." << std::endl;
						}
						else {
							std::cout << subordinateName << " has " << hierarchies[i]->hierarchy->num_subordinates(subordinateName) << " subordinates." << std::endl;
						}
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }

			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "manager") {

			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName) {
						std::cout << "The manager of " << subordinateName << " is" << hierarchies[i]->hierarchy->manager(subordinateName) << std::endl;
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}

		else if (command == "num_employees") {
			if (arguments.size() == 2) {
				string objectName = arguments[1];

				bool isExistObjectWithThatName = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					if (isExistObjectWithThatName) {
						std::cout << "There are " << hierarchies[i]->hierarchy->num_employees() << " employees in Lozenec." << std::endl;
						break;
					}
				}
				if (isExistObjectWithThatName == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }

		}
		else if (command == "longest_chain") {

			if (arguments.size() == 2) {
				string objectName = arguments[1];
				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						std::cout << "The longest chain in " << objectName << " is " << hierarchies[i]->hierarchy->longest_chain() << " ." << std::endl;
						correctArguments = true;
						break;
					}
				}
				if (correctArguments == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "num_overloaded") {

			if (arguments.size() == 2) {
				string objectName = arguments[1];
				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						if (hierarchies[i]->hierarchy->num_overloaded() == 0) {
							std::cout << "No overloaded employees in " << objectName << " ." << std::endl;
						}
						else {
							std::cout << "Overloaded employees in " << objectName << " are " << hierarchies[i]->hierarchy->num_overloaded() << std::endl;
						}
						correctArguments = true;
						break;
					}
				}
				if (correctArguments == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "join") {

			if (arguments.size() == 4) {
				string objectName1 = arguments[1];
				string objectName2 = arguments[2];
				string newObjectName = arguments[3];

				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName1 || hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName2) {
						for (int j = i+1; j < hierarchies.size(); ++j) {
							if (hierarchies[j]->hierarchy->getNameOfTheHierarchy() == objectName1 || hierarchies[j]->hierarchy->getNameOfTheHierarchy() == objectName2) {
							
								Hierarchy* newHierarchy;

								try {

									newHierarchy =  new Hierarchy(hierarchies[i]->hierarchy->join(*hierarchies[j]->hierarchy).print());
								}
								catch (std::invalid_argument ex) {
									std::cerr << ex.what() << std::endl;
									continue;
								}
								newHierarchy->setNameOfTheHierarchy(newObjectName);
								HierarchyRecord* hierarchyToAdd = new HierarchyRecord{ newHierarchy, false, false };
								hierarchies.push_back(hierarchyToAdd);

							}
							correctArguments = true;
							break;
						}
						if (correctArguments) { break; }
					}
				}
				if (correctArguments == false) {
					std::cout << "Unknown offices!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "fire") {

			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName) {
						hierarchies[i]->hierarchy->fire(subordinateName);
						hierarchies[i]->hasChanged = true;
						hierarchies[i]->isSaved = false;
						std::cout << subordinateName << " was fired." << std::endl;
						break;
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}

		else if (command == "hire") {

			if (arguments.size() == 4) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];
				string managerName = arguments[3];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				bool isExistManagerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					isExistManagerWithThatName = hierarchies[i]->hierarchy->find(managerName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName && isExistManagerWithThatName) {
						hierarchies[i]->hierarchy->hire(subordinateName, managerName);
						hierarchies[i]->hasChanged = true;
						hierarchies[i]->isSaved = false;
						std::cout << subordinateName << " was hired." << std::endl;
						break;
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << managerName << " in " << objectName << "." << std::endl; }
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "salary") {
			if (arguments.size() == 3) {
				string objectName = arguments[1];
				string subordinateName = arguments[2];

				bool isExistObjectWithThatName = false;
				bool isExistWorkerWithThatName = false;
				for (int i = 0; i < hierarchies.size(); ++i) {
					isExistObjectWithThatName = hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName;
					isExistWorkerWithThatName = hierarchies[i]->hierarchy->find(subordinateName);
					if (isExistObjectWithThatName && isExistWorkerWithThatName) {
						std::cout << "The salary is " << hierarchies[i]->hierarchy->getSalary(subordinateName) << " ." << std::endl;
					}
				}
				if (isExistObjectWithThatName == false) { std::cout << objectName << " does not exist!" << std::endl; }
				if (isExistWorkerWithThatName == false) { std::cout << "There is no " << subordinateName << " in " << objectName << "." << std::endl; }
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }
		}
		else if (command == "incorporate") {

			if (arguments.size() == 2) {
				string objectName = arguments[1];
				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						hierarchies[i]->hierarchy->incorporate();
						hierarchies[i]->hasChanged = true;
						hierarchies[i]->isSaved = false;
						std::cout << objectName << " incorporated." << std::endl;
						correctArguments = true;
						break;
					}
				}
				if (correctArguments == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments!" << std::endl; }


		}
		else if (command == "modernize") {

			if (arguments.size() == 2) {
				string objectName = arguments[1];
				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						hierarchies[i]->hierarchy->modernize();
						hierarchies[i]->hasChanged = true;
						hierarchies[i]->isSaved = false;
						std::cout << objectName << " modernized." << std::endl;
						correctArguments = true;
						break;
					}
				}
				if (correctArguments == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Invalid number of arguments;" << std::endl; }

		}
		else if (command == "print") {
			string objectName = arguments[1];

			if (arguments.size() == 2) {
				bool correctArguments = false;

				for (int i = 0; i < hierarchies.size(); ++i) {
					if (hierarchies[i]->hierarchy->getNameOfTheHierarchy() == objectName) {
						std::cout << hierarchies[i]->hierarchy->print();
						correctArguments = true;
					}
				}
				if (correctArguments == false) {
					std::cout << objectName << " is an unknown office!" << std::endl;
				}
			}
			else { std::cout << "Inalid number of arguments!" << std::endl; }
		}
		else if (command == "exit") {

			for (int i = 0; i < hierarchies.size(); ++i) {
				if (!hierarchies[i]->isSaved) {
					string hierarchyStr = hierarchies[i]->hierarchy->print();
					std::cout << this->hierarchies[i]->hierarchy->getNameOfTheHierarchy() << " is modified, but not saved." << std::endl;
					std::cout << "Enter file name to save it:" << std::endl;
					std::string fileName;
					std::cin >> fileName;

					std::ofstream myFile(fileName);
					if (myFile.is_open()) {
						myFile << hierarchyStr;

						myFile.close();
					}
					hierarchies[i]->isSaved = true;
					hierarchies[i]->hasChanged = false;
					std::cout << this->hierarchies[i]->hierarchy->getNameOfTheHierarchy() << " saved." << std::endl;
				}
			}
			std::cout << "Good bye!" << std::endl;
			break;
		}
		else {
			std::cerr << "Invalid function!" << std::endl;
		}
	
	}
}

vector<string> split(const string& input, const char& symbol) {

	vector<string> arguments;

	int i = 0;
	int countSpaces = 0;

	for (; i < input.size(); ++i) {
		if (input[i] == ' ') {
			while (input[i] == ' ')
			{
				++i;
			}
			countSpaces++;
		}
	}

	int j = 0;
	for (int p = 0; p < countSpaces + 1; p++) {
		string currString;

		while (input[j] != symbol && j < input.size()) {

			currString.push_back(input[j]);
			j++;
		}
		arguments.push_back(currString);

		while (input[j] ==' ')
		{
			j++;

		}
	}
	return arguments;
}

