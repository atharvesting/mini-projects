#pragma once
#include <string>
#include <map>
using namespace std;

void preprocessString(string& s);
string readFile(string file_name);
void writeFile(string file_name, string content);
double randomNumGenerator(void);
int nextWordsfSum(map<string, int>& a);
void export2JSON(const string& file_name, const map<string, map<string, int>>& fMap);
map<string, map<string, int>> JSON2Map(string file_name);