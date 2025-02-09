#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

class AmountIndex
{
private:
	int amount;
	std::vector<int> indexes;
public:
	AmountIndex() {}
	AmountIndex(int amount, std::vector<int> indexes) {
		this->amount = amount;
		this->indexes = indexes;
	}
	int getAmount() {
		return amount;
	}
	std::vector<int> getIndexes() {
		return indexes;
	}
	void setAmount(int amount) {
		this->amount = amount;
	}
	void setIndexes(std::vector<int> indexes) {
		this->indexes = indexes;
	}
	void addAmount() {
		(this->amount)++;
	}
	void addIndex(int i) {
		indexes.push_back(i);
	}

	friend std::ostream& operator<<(std::ostream& os, const AmountIndex& ai)
	{
		os << "am: " << ai.amount << " ind: ";
			for (auto index : ai.indexes) {
				std::cout << index << ' ';
			}
			std::cout << '\b';

		return os;
	}

	~AmountIndex() {}

};

class HtmlFinder
{
private:
	
	std::map<std::string, AmountIndex> dict;
public:
	HtmlFinder() {}
	HtmlFinder(std::string html) {
		bool ignore = false;
		bool isBackSlash = false;
		std::string buf;
		for (int i = 0; i < html.length(); i++)
		{
			char c = html[i];

			if (c == '<') {
				ignore = true;
			}
			else if (c == '>') {
				ignore = false;
				continue;
			}

			if (ignore) {
				continue;
			}
			else if (!isalpha(c)) {
				if (buf.empty()) 
					continue;

				for (int j = 0; j < buf.length(); j++) {
					buf[j] = tolower(buf[j]);
				}

				std::string temp(buf);
				if (dict.find(temp) != dict.end()) { // todo maybe truble
					AmountIndex &ai = dict[temp];
					ai.addAmount();
					ai.addIndex(i);
				}
				else {
					std::vector<int> indeces;
					indeces.push_back(i);
					dict[temp] = AmountIndex(1, indeces);
				}

				buf.clear();
				continue;
			}

			buf += c;
		}
	}

	std::map<std::string, AmountIndex> getDictionary() {
		return dict;
	}

	AmountIndex find(std::string word) {
		std::map<std::string, AmountIndex>::iterator iter = dict.begin();
		while (iter != dict.end()) {
			if ((*iter).first == word) {
				return (*iter).second;
			}

			iter++;
		}

		AmountIndex res;
		return res;
	}

	~HtmlFinder() {}
};

int main() {
	std::ifstream f("text.html");
	std::stringstream ss;
	ss << f.rdbuf();

	std::string text = ss.str();

	HtmlFinder htmlf(text);
	for (auto pair : htmlf.getDictionary())
	{
		std::cout << pair.first << ' ' << pair.second << '\n';
	}

	std::cout << "\nresult of foundation:\n";
	std::cout << "while - " << htmlf.find("while") << '\n';
	std::cout << "a - "<< htmlf.find("a") << '\n';

	return 0;
}