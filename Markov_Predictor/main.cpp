#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "test.h"
#include "utils.h"
using namespace std;

vector<string> string2Array(string& s) 
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

map<string, map<string, int>> array2Map(const vector<string>& v) 
{
    map<string, map<string, int>> fMap;

    for (int i = 0; i < v.size(); i++) {
        if (fMap.find(v[i]) == fMap.end()) fMap[v[i]] = { };

        if (i != v.size() - 1) 
        {
            // Adding 1 to the Value of the inner map for each next word
            // The next word is auto added and initialized to 0 if it doesn't exist
            fMap[v[i]][v[i + 1]]++;
        }
    }
    return fMap;
}

map<string, map<string, int>> string2Map(string& s) 
{
    vector<string> a = string2Array(s);
    return array2Map(a);
}

void startPredictionLoop(map<string, map<string, int>>& fMap, string start, unsigned int word_count)
{
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

int main()
{
    auto file_contents = readFile("school.txt");
    auto m = string2Map(file_contents);
    startPredictionLoop(m, "the", 15);

    return 0;
}