#pragma once

#include <fstream>
#include <iostream>
#include <exception>
#include <set>

#include "Stack.h"

class WordCollector
{
private:
	std::set<std::string>* hashSetNoun;
	std::set<std::string>* hashSetVerb;
	std::set<std::string>* hashSetOther;

	Stack<std::string>* stackNoun;
	Stack<std::string>* stackVerb;
	Stack<std::string>* stackOther;

	std::set<std::string> *getWordSet(std::string fileName) {
		std::ifstream file(fileName);
		if (!file.is_open()) {
			std::string msg = "Не удалось открыть файл: " + fileName + "\n";
			throw std::ifstream::failure(msg.data());
		}

		std::set<std::string>* res = new std::set<std::string>();
		std::string word;
		while (file >> word)
			res->insert(word);
		
		file.close();

		return res;
	}

	bool isWordInSet(std::set<std::string> *set, std::string word) {
		return set->find(word) != set->end();
	}
public:
	WordCollector() {
		try {
			hashSetNoun = getWordSet("dict_noun.txt");
			hashSetVerb = getWordSet("dict_verb.txt");
			hashSetOther = getWordSet("dict_other.txt");
		}
		catch (const std::ifstream::failure& e) {
			std::cerr << e.what();
			throw std::ofstream::failure("");
		}

		stackNoun = new Stack<std::string>();
		stackVerb = new Stack<std::string>();
		stackOther = new Stack<std::string>();
	}

	~WordCollector() {
		if (hashSetNoun != nullptr) delete hashSetNoun;
		if (hashSetVerb != nullptr) delete hashSetVerb;
		if (hashSetOther != nullptr) delete hashSetOther;

		if (stackNoun != nullptr) delete stackNoun;
		if (stackVerb != nullptr) delete stackVerb;
		if (stackOther != nullptr) delete stackOther;

	}

	void recordToFile(std::string sourceName, std::string destinationName) {
		std::ifstream source(sourceName);
		if (!source.is_open()) {
			std::string msg = "Не удалось открыть файл: " + sourceName + "\n";
			throw std::exception(msg.data());
		}

		std::ofstream destination(destinationName);
		if (!destination.is_open()) {
			std::string msg = "Не удалось создать файл: " + destinationName + "\n";
			throw std::exception(msg.data());
		}

		std::string word;
		while (source >> word)
		{
			if (std::ispunct(word[word.size() - 1]))
				word.pop_back();

			for (size_t i = 0; i < word.size(); i++)
				word[i] = std::tolower(word[i]);

			if (isWordInSet(hashSetNoun, word))
				stackNoun->add(word);
			else if (isWordInSet(hashSetVerb, word))
				stackVerb->add(word);
			else if (isWordInSet(hashSetOther, word))
				stackOther->add(word);
			else 
				throw std::runtime_error("Слова нет в справочнике!");
		}

		source.close();

		while (!stackNoun->isEmpty())
		{
			destination << stackNoun->pop() << " ";
		}

		while (!stackVerb->isEmpty())
		{
			destination << stackVerb->pop() << " ";
		}

		while (!stackOther->isEmpty())
		{
			destination << stackOther->pop() << " ";
		}

		destination.close();
	}
};