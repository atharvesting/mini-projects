#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;

void preprocessString(string& s) 
{
    // Removing Punctuation, Lowercase, Adding a Whitespace at End
    s.erase(remove_if(s.begin(), s.end(), [](unsigned char c) { return ispunct(c); }), s.end());
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    s.push_back(' ');  // (char)" " does not work!
}

vector<string> string2Array(string& s) 
{
    vector<string> words;
    int pos = 0;
    preprocessString(s);
    
    for (int i = 0; i < s.size(); i++)
    {
        if (isspace(s[i]))
        {
            words.push_back(s.substr(pos, i - pos));
            pos = i + 1;
        }
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
    for (const auto& m : a) {
        cout << "------" << m.first << "------" << "\n";
        for (auto n : m.second) {
            cout << n.first << ": " << n.second << "\n";
        }
        cout << "\n";
    }
}

string readFile(string file_name) {
    ifstream file(file_name);
    string s, file_contents;
    while (getline(file, s))
    {
        file_contents += s;
        file_contents.push_back(' ');
    }
    file.close();

    return file_contents;
}

map<string, map<string, int>> string2Map(string& s) {
    vector<string> a = string2Array(s);
    return array2Map(a);
}

int nextWordsfSum(map<string, int>& a) {
    int total = 0;
    for (auto n : a) {
        total += n.second;
    }
    return total;
}


int main()
{
    auto file_contents = readFile("test.txt");
    auto m = string2Map(file_contents);
    testMapper(m);
    
    string w;
    cout << "Enter a word: ";
    cin >> w;

    if (m.find(w) != m.end())
    {
        double total = nextWordsfSum(m[w]);
        for (auto a : m[w]) {
            cout << a.first << ": P -> " << setprecision(5) << a.second / total << "\n";
        }
    }
    
    //string test = "this is a example of a sentence where sentence is a group of word of example and a group this is";
    return 0;
}