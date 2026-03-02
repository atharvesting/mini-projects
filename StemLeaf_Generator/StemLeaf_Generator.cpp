#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

// Defining a Stem-Leaf pair
struct StemLeaf {
    int stem;
    vector<int> leaf;
};
// Creating a Stem-Leaf Tree (array)
vector<struct StemLeaf> tree;
map<int, int> stems;
int last_idx = 0, pre, post;

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
        sort(tree[stems[pre]].leaf.begin(), tree[stems[pre]].leaf.end());
    }
    sort(tree.begin(), tree.end(), [](const struct StemLeaf& a, const struct StemLeaf& b) {
        return a.stem < b.stem;
    });
}

void print_tree(vector<struct StemLeaf> tree) {
    for (auto branch : tree) {
        for (int i = 0; i < 3 - to_string(branch.stem).length(); i++)
            cout << " ";
        cout << branch.stem << " | ";
        for (int bit : branch.leaf) {
            cout << bit << ",  ";
        }
        cout << endl;
    }
}

int main()
{
    vector<int> test = { 16, 131, 108, 18, 121, 41, 77, 147, 88, 24, 110, 199, 32, 198, 87, 67, 44, 190, 186, 13, 31, 122, 16, 56, 48, 61, 88, 11, 42, 93, 18, 124, 108, 130, 180, 12, 87, 141, 200, 76, 49, 15, 105, 139, 22, 63, 42, 77, 86, 147, 121, 24 };
    create_tree(test);
    print_tree(tree);

    return 0;
}