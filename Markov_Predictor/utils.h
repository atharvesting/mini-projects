#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

void preprocessString(string& s);
string readFile(string file_name);
void writeFile(string file_name, string content);
double randomNumGenerator(void);
int nextWordsfSum(map<string, int>& a);
void export2JSON(const string& file_name, map<string, map<string, int>> fMap);
void export2JSON(const string& file_name, map<vector<string>, map<string, int>> fMap);
map<string, map<string, int>> JSON2Map(string file_name);