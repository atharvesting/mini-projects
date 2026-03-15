#include <string>
#include "utils.h"
#include <vector>
#include <sstream>
#include <variant>
#include <iostream>
using namespace std;

using MapResult = variant<
	map<string, map<string, int>>,
	map<vector<string>, map<string, int>>
>;

class Model
{
protected:
	string file_contents;
	vector<string> word_list;

public:
	MapResult model_data;
	// Constructor
	Model(string file_name) {
		this->file_contents = readFile(file_name);
		this->word_list = string2Array(this->file_contents);
	}

	virtual ~Model() = default;

	static vector<string> string2Array(string& s)
	{
		vector<string> words;
		int pos = 0;
		preprocessString(s);
		stringstream ss(s);
		string temp;

		while (ss >> temp) {
			words.push_back(temp);
		}
		return words;
	}

	// Abstract Base Class to ensure the modelling functions are implemented
	virtual MapResult array2Map(const vector<string>& v) = 0;
	//virtual void display() = 0;
	virtual void predict(string start, unsigned int word_count) = 0;

	void buildModel() {
		this->model_data = array2Map(this->word_list);
	}

};

class Bigram : public Model
{
public:

	Bigram(string file_name) : Model(file_name) {
		buildModel();
	}

	MapResult array2Map(const vector<string>& v) override
	{
		map<string, map<string, int>> fMap;
		for (size_t i = 0; i < v.size() - 1; i++)
		{
			fMap[v[i]][v[i + 1]]++;
		}
		return fMap;
	}

	void predict(string start, unsigned int word_count) override
	{
		auto& fMap = get <map<string, map<string, int>>>(model_data);
		unsigned int i = 0;
		string current = start;
		cout << start << " ";
		while (i < word_count)
		{
			if (fMap.find(current) == fMap.end()) break;

			double r = randomNumGenerator();
			double cumulative_prob = 0.0f;
			map<string, int> temp = fMap.at(current);

			int total = nextWordsfSum(temp);

			for (const auto& [word, count] : temp)
			{
				// Inverse transform sampling to choose next word (Thought of this approach myself!)
				cumulative_prob += (double)count / total;
				if (cumulative_prob >= r)
				{
					cout << word << " ";
					i++;
					current = word;
					break;
				}
			}
		}
		cout << "\n\nPrediction terminated!" << endl;
	}
};

class Trigram : public Model {
public:
	Trigram(string file_name) : Model(file_name) {
		buildModel();
	}

	MapResult array2Map(const vector<string>& v) override {
		map<vector<string>, map<string, int>> fMap;
		for (size_t i = 0; i < v.size() - 2; i++) {
			vector<string> temp = { v[i], v[i + 1] };
			fMap[temp][v[i + 2]]++;
		}
		return fMap;
	}

	void predict(string start, unsigned int word_count) override {
		cout << "Here is the prediction" << endl;
	}
};