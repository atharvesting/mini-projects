#pragma once
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
using namespace std;

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