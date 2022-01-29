#pragma once

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include "../Homework 3/myHashFunction.h"
#include "../Homework 3/hashTable.h"


const int HASH_TABLE_SIZE_FACTOR = 4;
using std::pair;
using std::string;



class WordsMultiset {
private:
	HashTable<string,size_t> table = HashTable<string, size_t>(&stringHashFunction);

public:
	WordsMultiset() {}

	WordsMultiset(const size_t& tableSize) {
		this->table = HashTable<string, size_t>(tableSize, &stringHashFunction);
	}

	WordsMultiset(const size_t& tableSize, size_t(*hashFunc)(const string&)) {
		this->table = HashTable<string, size_t>(tableSize, hashFunc);
	}


	void add(const std::string& word, size_t times = 1) {
		size_t count;
		times += this->table.get(word, count) ? count : 0;
		this->table.put(word, times);
	}

	void put(const std::string& word, size_t times = 1) {
		this->table.put(word, times);
	}

	void remove(const std::string& word) {
		this->table.remove(word);
	}

	bool contains(const std::string& word) const {
		size_t result;
		return this->table.get(word,result);
	}
	size_t countOf(const std::string& word) const {
		size_t result;
		return this->table.get(word,result) ? result :0;
	}
	size_t countOfUniqueWords() const {
		return this->table.getSize();
	}
	std::multiset<std::string> words() const {
		vector<pair<string, size_t>> keyValues = this->table.getAllKeyValues();
		std::multiset<string> set;

		for (pair<string, size_t> pair : keyValues) {
			for (size_t i = 0; i < pair.second; i++) {
				set.insert(pair.first);
			}
		}
		return set;
	}
};


class ComparisonReport {
public:
	/// A multiset of all words that exist in both streams
	WordsMultiset commonWords;

	/// Multisets of words unique to the two streams
	/// The first element of the array contains the words that are unique
	/// to the first stream (a) and the second one -- to the second stream (b)
	WordsMultiset uniqueWords[2];
};

class Comparator {
public:
	ComparisonReport compare(std::istream& a, std::istream& b) {

		a.seekg(0, std::ios::end);
		size_t aStreamSize = a.tellg();
		a.seekg(0, std::ios::beg);

		b.seekg(0, std::ios::end);
		size_t bStreamSize = b.tellg();
		b.seekg(0, std::ios::beg);

		WordsMultiset file1UniqueSet(aStreamSize < HASH_TABLE_SIZE_FACTOR ? HASH_TABLE_SIZE_FACTOR : aStreamSize / HASH_TABLE_SIZE_FACTOR);
		WordsMultiset file2UniqueSet(bStreamSize < HASH_TABLE_SIZE_FACTOR ? HASH_TABLE_SIZE_FACTOR : bStreamSize / HASH_TABLE_SIZE_FACTOR);
		WordsMultiset commonWords((aStreamSize + bStreamSize) < HASH_TABLE_SIZE_FACTOR ? HASH_TABLE_SIZE_FACTOR : (aStreamSize + bStreamSize) / HASH_TABLE_SIZE_FACTOR);

		string word1;
		while (a >> word1)
		{
			file1UniqueSet.add(word1);
		}

		string word2;
		while (b >> word2)
		{
			size_t file1Times = file1UniqueSet.countOf(word2);
			if (file1Times > 0) {
				commonWords.add(word2);

				if (file1Times == 1) {
					file1UniqueSet.remove(word2);
				}
				else {
					file1UniqueSet.put(word2, file1Times - 1);
				}
			}
			else {
				file2UniqueSet.add(word2);
			}
		}

		ComparisonReport report;
		report.commonWords = commonWords;

		report.uniqueWords[0] = file1UniqueSet;
		report.uniqueWords[1] = file2UniqueSet;

		return report;
	}
};
