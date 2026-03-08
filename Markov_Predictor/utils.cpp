#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include "nlohmann/json.hpp"
#include "models.h"
using json = nlohmann::json;
using namespace std;

double randomNumGenerator(void) 
{
    static random_device rd;  // Seed: Hardware entropy source
    static mt19937 gen(rd()); // Engine: Mersenne Twister
    static uniform_real_distribution<double> dis(0.0f, 1.0f);

    return dis(gen);
}

string readFile(string file_name) 
{
    ifstream file(file_name);
    string s, file_contents;

    ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

void writeFile(string file_name, string content)
{
    ofstream file(file_name);
    file << content;
    file.close();
}

void preprocessString(string& s)
{
    // Removing Punctuation, Lowercase
    s.erase(remove_if(s.begin(), s.end(), [](unsigned char c) { return ispunct(c); }), s.end());
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
}

int nextWordsfSum(map<string, int>& a)
{
    int total = 0;
    for (auto n : a) {
        total += n.second;
    }
    return total;
}

void export2JSON(const string& file_name, const map<string, map<string, int>>& fMap) {
    json j(fMap);
    writeFile(file_name, j.dump(4));
}

map<string, map<string, int>> JSON2Map(string file_name) {
    string s = readFile(file_name);
    json j = json::parse(s);

    return j.get<map<string, map<string, int>>>();
}