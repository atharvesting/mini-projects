#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>

using namespace std;

void preprocessString(string& s) 
{
    // Removing Punctuation, Lowercase
    s.erase(remove_if(s.begin(), s.end(), [](unsigned char c) { return ispunct(c); }), s.end());
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
}

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

void testMapper(map<string, map<string, int>>& a) 
{
    for (const auto& m : a) 
    {
        cout << "------" << m.first << "------" << "\n";
        for (auto n : m.second) 
        {
            cout << n.first << ": " << n.second << "\n";
        }
        cout << "\n";
    }
}

void testProbabilities(auto m, string w)
{
    if (m.find(w) != m.end())
    {
        double total = nextWordsfSum(m[w]);
        for (auto a : m[w]) {
            cout << a.first << ": P -> " << setprecision(5) << a.second / total << "\n";
        }
    }
}

string readFile(string file_name) {

    ifstream file(file_name);
    string s, file_contents;

    ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

map<string, map<string, int>> string2Map(string& s) 
{
    vector<string> a = string2Array(s);
    return array2Map(a);
}

int nextWordsfSum(map<string, int>& a) 
{
    int total = 0;
    for (auto n : a) {
        total += n.second;
    }
    return total;
}

double randomNumGenerator(void) {
    static random_device rd;  // Seed: Hardware entropy source
    static mt19937 gen(rd()); // Engine: Mersenne Twister
    static uniform_real_distribution<double> dis(0.0f, 1.0f);

    return dis(gen);
}


void startPredictionLoop(map<string, map<string, int>>& fMap, string start)
{
    int i = 0;
    string current = start;
    cout << start << " ";
    while (i < 10) 
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
    auto file_contents = readFile("test.txt");
    auto m = string2Map(file_contents);
    startPredictionLoop(m, "with");

    return 0;
}