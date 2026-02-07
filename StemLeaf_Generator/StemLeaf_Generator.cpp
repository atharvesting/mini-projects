#include <iostream>
#include <vector>
#include <map>
using namespace std;

struct StemLeaf {
    int stem;
    vector<int> leaf;
};
vector<struct StemLeaf> tree;
map<int, int> stems;

int last_idx = 0, pre, post;

void create_tree(vector<int> data);
void print_tree(vector<struct StemLeaf> tree);

int main()
{
    vector<int> test = { 11, 12, 13, 15, 16, 16, 18, 18, 22, 24, 31, 32, 41, 42, 42, 44, 48, 49, 56, 61, 63, 67, 76, 77, 86, 87, 87, 88, 88, 93, 105, 108, 108, 110, 121, 121, 122, 124, 130, 131, 139, 141, 147, 147, 180, 186, 190, 198, 199, 200 };
    create_tree(test);
    print_tree(tree);

    return 0;
}

void create_tree(vector<int> data) {
    
    for (int num : data) {
        post = num % 10;
        pre = num / 10;
        auto it = stems.find(pre);
        struct StemLeaf st = { .stem = pre };
        if (it == stems.end()) {
            tree.push_back(st);
            stems.insert({ pre, last_idx });
            last_idx++;
        }
        tree[stems[pre]].leaf.push_back(post);
    }
}

void print_tree(vector<struct StemLeaf> tree) {
    for (auto branch : tree) {
        cout << branch.stem << " | ";
        for (int bit : branch.leaf) {
            cout << bit << ",  ";
        }
        cout << endl;
    }
}

/*
* For every number in the data:
* check if it is in the map
* if it is, continue
* if not, then insert pre into map, create new struct in tree and get its idx
* 
* access its struct using the idx tracked in the map and append post to leaf
*/