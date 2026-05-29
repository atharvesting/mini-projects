#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <fast-cpp-csv-parser/csv.h>
using namespace std;

// Defining a Stem-Leaf pair
struct StemLeaf {
    int stem;
    vector<int> leaf;
};

// Creating a Stem-Leaf Tree (array)
vector<struct StemLeaf> tree;
map<int, int> stems;
int last_idx = 0, pre, post, ncount;

vector<int> readCSV(string file_name) {
    vector<int> v; int value;
    io::CSVReader<1> in(file_name);
    in.read_header(io::ignore_extra_column, "value");
    while (in.read_row(value)) {
        v.push_back(value);
    }
    ncount = v.size();
    return v;
}

struct Stats {
    size_t count = 0;
    long long sum = 0;
    double mean = 0.0;
    double median = 0.0;
    double variance = 0.0;
    double q1 = 0.0;
    double q3 = 0.0;
    double iqr = 0.0;
    std::vector<int> outliers;
};

struct Stats calculate_stats(vector<int>& v) {
    struct Stats s;
    sort(v.begin(), v.end());

    s.count = v.size();
    s.sum = accumulate(v.begin(), v.end(), 0LL); // 0LL: Sum is tracked as long long
    s.mean = static_cast<double>(s.sum) / v.size();

    if (s.count % 2 == 0) {
        s.median = (v[s.count / 2 - 1] + v[s.count / 2]) / 2.0;
    } 
    else {
        s.median = v[s.count / 2];
    }

    double squared_diff_sum = 0;
    for (const int& n : v) {
        squared_diff_sum += (n - s.mean, 2) * (n - s.mean, 2); // Faster than std::pow
    }
    s.variance = squared_diff_sum / s.count;

    s.q1 = v[s.count / 4];
    s.q3 = v[3 * s.count / 4];
    s.iqr = s.q3 - s.q1;

    double lower_bound = s.q1 - (1.5 * s.iqr);
    double upper_bound = s.q3 + (1.5 * s.iqr);

    for (int n : v) {
        if ((n > upper_bound) || (n < lower_bound)) {
            s.outliers.push_back(n);
        }
    }

    return s;
}

void print_stats(struct Stats s) {
    cout << "\n++++++ Statistics ++++++" << "\n\n";
    cout << "Count: " << s.count << "\n";
    cout << "Sum: " << s.sum << "\n";
    cout << "Mean: " << s.mean << "\n";
    cout << "Median: " << s.median << "\n";
    cout << "Variance: " << s.variance << "\n";
    cout << "Outliers: ";
    for (const int& n : s.outliers) {
        cout << n << "  ";
    }
    cout << "\n";
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
        sort(tree[stems[pre]].leaf.begin(), tree[stems[pre]].leaf.end());
    }
    sort(tree.begin(), tree.end(), [](const struct StemLeaf& a, const struct StemLeaf& b) {
        return a.stem < b.stem;
    });
}

void print_tree(vector<struct StemLeaf> tree) {
    cout << "++++++ Stemleaf Chart Generator ++++++\n\n";
    cout << "Total numbers: " << ncount << "\n\n";
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
    //vector<int> test = { 16, 131, 108, 18, 121, 41, 77, 147, 88, 24, 110, 199, 32, 198, 87, 67, 44, 190, 186, 13, 31, 122, 16, 56, 48, 61, 88, 11, 42, 93, 18, 124, 108, 130, 180, 12, 87, 141, 200, 76, 49, 15, 105, 139, 22, 63, 42, 77, 86, 147, 121, 24 };
    //vector<int> test = { 2, 3, 2, 1, 3, 2, 2, 10, 2 };
    auto v = readCSV("test_single_col.csv");
    create_tree(v);
    print_tree(tree);
    auto s = calculate_stats(v);
    print_stats(s);

    return 0;
}