#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <vector>

struct Posting {
    int paper;
    int count;
};

struct TermEntry {
    std::string word;
    std::vector<Posting> postings;
    TermEntry* next;
};

struct HashTable {
    TermEntry** buckets;
    int bucketCount;
    int size;
};

// Turns a word into large number used to pick its bucket
unsigned long hashWord(const std::string& word);

// Sets up an empty table with the given number of buckets
void initTable(HashTable& table, int bucketCount);

// Adds one occurrence of a word in a paper, creating or updating its posting list
void insertTerm(HashTable& table, const std::string& word, int paper);

// Returns the word's posting list, or nullptr if the word is not in the table
const std::vector<Posting>* lookupTerm(const HashTable& table, const std::string& word);

// Doubles the number of buckets and moves every entry to its new bucket
void resizeTable(HashTable& table);

// Frees all memory used by the table
void freeTable(HashTable& table);

#endif