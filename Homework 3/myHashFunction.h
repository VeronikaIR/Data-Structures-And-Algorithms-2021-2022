#pragma once
#include <string>

size_t stringHashFunction(const std::string& word) {
	size_t hashValue = 0;
	for (int i = 0; i < word.size(); i++) {
		hashValue += int(word[i]);
	}
	return hashValue;
}