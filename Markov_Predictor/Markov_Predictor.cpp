#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

vector<string> string2Array(string& s) {
    vector<string> words;
    int pos = 0;
    s.push_back(' ');  // (char)" " does not work!
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

map<string, map<string, int>> array2Map(vector<string>& v) {
    map<string, map<string, int>> fMap;

    for (int i = 0; i < v.size(); i++) {
        if (fMap.find(v[i]) == fMap.end()) fMap[v[i]] = { };

        if (i != v.size() - 1) 
        {
            if (fMap[v[i]].find(v[i + 1]) == fMap[v[i]].end()) 
            {
                fMap[v[i]][v[i + 1]] = 0;
            }
            fMap[v[i]][v[i + 1]]++;
        }
    }
    return fMap;
}

void testMapper(map<string, map<string, int>>& a) {
    for (auto m : a) {
        cout << "Word: " << m.first << "\n";
        for (auto n : m.second) {
            cout << n.first << ": " << n.second << "\n";
        }
        cout << "_________\n";
    }
}

int main()
{
    string test = "this is a example of a sentence where sentence is a group of word of example and a group this is";

    auto a = string2Array(test);

    auto m = array2Map(a);

    testMapper(m);

    return 0;
}