#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include "test.h"
#include "utils.h"
#include "models.h"
using namespace std;

struct TrieNode {
    uint32_t wordId;
    uint32_t firstChild;
    uint32_t nextSibling;
    uint32_t frequency;
};

class Dictionary {
public:
    unordered_map<string, int> wordIdMap;
    unordered_map<int, string> idWordMap;
    vector<uint32_t> idSequence;

    uint32_t wordtoId(string s) {
        auto t = wordIdMap.find(s);
        if (t == wordIdMap.end()) {
            uint32_t id = wordIdMap.size();
            wordIdMap[s] = id;
            idWordMap[id] = s;
        }
        return wordIdMap[s];
    }

    void populateSequence(const vector<string>& word_list) {
        for (const auto& word : word_list) {
            auto id = wordtoId(word);
            idSequence.push_back(id);
        }
    }
};


class MarkovTrie {
public:
    string              file_contents;
    vector<string>      word_list;
    Dictionary          dict;
    uint32_t            n_gram;
    vector<TrieNode>    arena;

    MarkovTrie(string filename, uint32_t n) {
        n_gram = n;
        file_contents = readFile(filename);
        preprocessString(file_contents);
        word_list = stringToArray(file_contents);
        dict.populateSequence(word_list);
        arena.reserve(10000);
        arena.push_back(TrieNode{ 0, 0, 0, 0 });
    }

    uint32_t findWordInChildren(uint32_t parentLoc, uint32_t targetId) {
        uint32_t current = arena[parentLoc].firstChild;

        while (current != 0) {
            if (arena[current].wordId == targetId) {
                return current; // Found the word in the sequence
            }
            current = arena[current].nextSibling;
        }
        return 0;
    }

    uint32_t getEndOfSiblingChain(uint32_t parentLoc) {
        uint32_t current = arena[parentLoc].firstChild;
        if (current == 0) return 0;

        while (arena[current].nextSibling != 0) {
            current = arena[current].nextSibling;
        }
        return current;
    }

    void buildMarkovTrie() 
    {
        uint32_t size = dict.idSequence.size();
        
        // For every word as a starting word in the window
        for (uint32_t i = 0; i <= size - n_gram; i++)
        {
            uint32_t parentLoc = 0;

            // For all the words that fall into the window
            for (uint32_t j = 0; j < n_gram; j++)
            {
                uint32_t currentWordId = dict.idSequence[i + j];
                uint32_t currentLoc = findWordInChildren(parentLoc, currentWordId);

                if (currentLoc == 0)  // If current word is not a child
                {     
                    TrieNode newNode{ currentWordId, 0, 0, 1 };
                    arena.push_back(newNode);
                    uint32_t newNodeId = arena.size() - 1;

                    uint32_t currentLvlEndLoc = getEndOfSiblingChain(parentLoc);

                    if (currentLvlEndLoc == 0) {
                        arena[parentLoc].firstChild = newNodeId;
                    }
                    else {
                        arena[currentLvlEndLoc].nextSibling = newNodeId;
                    }

                    parentLoc = newNodeId;
                }
                else 
                {
                    arena[currentLoc].frequency++;
                    parentLoc = currentLoc;
                }
            }
        }
    }

    uint32_t findTotalChildrenNonUnique(uint32_t parentLoc) {
        uint32_t total{ 0 };
        uint32_t count{ 0 };
        uint32_t current = arena[parentLoc].firstChild;
        if (current == 0) return 0;

        while (arena[current].nextSibling != 0) {
            total += arena[current].frequency;
            current = arena[current].nextSibling;
        }
        return total;
    }

    void predict(vector<string> starting_words, uint32_t word_count) {
        if (starting_words.size() != n_gram - 1) {
            cout << "The model needs exactly " << n_gram - 1 << " starting word(s) to run" << endl;
            return;
        }

        vector<string> seq;
        seq.insert(seq.end(), starting_words.begin(), starting_words.end());

        // Print out the starting words
        for (const auto& word : seq) {
            cout << word << " ";
        }

        // Start the main prediction loop
        for (uint32_t i = 0; i < word_count; i++) 
        {
            uint32_t parent = 0;

            // Sliding window that takes the last n words of the vector into account
            //cout << "\nSW START" << endl;
            for (size_t j = i; j < seq.size(); j++) 
            {
                //cout << seq[j] << " ";
                uint32_t loc = findWordInChildren(parent, dict.wordtoId(seq[j]));
                parent = loc; // I am assuming that the starting_words are an existing sequence
            }
            //cout << "SW END" << endl;

            uint32_t totalChildren = findTotalChildrenNonUnique(parent);
            double randomNum = randomNumGenerator();
            double cummulativeProb = 0.0;
            
            auto current = arena[parent].firstChild;

            while (current != 0) {
                //cout << "\nCurrent word: " << dict.idWordMap[arena[current].wordId] << "\n";
                cummulativeProb += (double)arena[current].frequency / totalChildren;
                
                if (cummulativeProb > randomNum) {
                    string prediction = dict.idWordMap[arena[current].wordId];
                    cout << prediction << " ";

                    // This prediction will influence the next
                    seq.push_back(prediction);
                    break;
                }
                
                current = arena[current].nextSibling;
            }
        }
    }
};

int main()
{
    MarkovTrie model("train.txt", 2);
    model.buildMarkovTrie();
    vector<string> starting_words = { "if" }; // "i", "must", "not"
    model.predict(starting_words, 30); 

    return 0; 
}