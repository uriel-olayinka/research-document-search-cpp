#include "index.h"
#include <cmath>

// Turns a word into large number used to pick its bucket
unsigned long hashWord(const std::string& word) {
    unsigned long h = 5381;
    for (char c : word) {
        h = h * 33 + static_cast<unsigned char>(c);
    }
    return h;
}

// Sets up an empty table with the given number of buckets
void initTable(HashTable& table, int bucketCount) {
    table.bucketCount = bucketCount;
    table.size = 0;
    table.buckets = new TermEntry*[bucketCount];
    for (int i = 0; i < bucketCount; i++) {
        table.buckets[i] = nullptr;
    }
}

// Doubles the number of buckets and moves every entry to its new bucket
void resizeTable(HashTable& table) {
    int newCount = table.bucketCount * 2;
    TermEntry** newBuckets = new TermEntry*[newCount];
    for (int i = 0; i < newCount; i++) {
        newBuckets[i] = nullptr;
    }
    for (int i = 0; i < table.bucketCount; i++) {
        TermEntry* entry = table.buckets[i];
        while (entry != nullptr) {
            TermEntry* next = entry->next;
            int b = static_cast<int>(hashWord(entry->word) % newCount);
            entry->next = newBuckets[b];
            newBuckets[b] = entry;
            entry = next;
        }
    }
    delete[] table.buckets;
    table.buckets = newBuckets;
    table.bucketCount = newCount;
}

// Adds one occurrence of a word in a paper, creating or updating its posting list
void insertTerm(HashTable& table, const std::string& word, int paper) {
    if (static_cast<double>(table.size) / table.bucketCount > 0.75) {
        resizeTable(table);
    }
    int b = static_cast<int>(hashWord(word) % table.bucketCount);
    TermEntry* entry = table.buckets[b];
    while (entry != nullptr && entry->word != word) {
        entry = entry->next;
    }
    if (entry == nullptr) {
        entry = new TermEntry;
        entry->word = word;
        entry->next = table.buckets[b];
        table.buckets[b] = entry;
        table.size++;
    }
    if (!entry->postings.empty() && entry->postings.back().paper == paper) {
        entry->postings.back().count++;
    } else {
        entry->postings.push_back({paper, 1});
    }
}

// Returns the word's posting list, or nullptr if the word is not in the table
const std::vector<Posting>* lookupTerm(const HashTable& table, const std::string& word) {
    int b = static_cast<int>(hashWord(word) % table.bucketCount);
    TermEntry* entry = table.buckets[b];
    while (entry != nullptr && entry->word != word) {
        entry = entry->next;
    }
    if (entry == nullptr) {
        return nullptr;
    }
    return &entry->postings;
}

// Frees all memory used by the table
void freeTable(HashTable& table) {
    for (int i = 0; i < table.bucketCount; i++) {
        TermEntry* entry = table.buckets[i];
        while (entry != nullptr) {
            TermEntry* next = entry->next;
            delete entry;
            entry = next;
        }
    }
    delete[] table.buckets;
    table.buckets = nullptr;
    table.bucketCount = 0;
    table.size = 0;
}

// Splits every paper's title and abstract into words and adds each word to the table
void buildIndex(const std::vector<Paper>& papers, HashTable& table) {
    for (int i = 0; i < (int)papers.size(); i++) {
        std::vector<std::string> words = tokenize(papers[i].title + " " + papers[i].abstractText);
        for (const std::string& w : words) {
            insertTerm(table, w, i);
        }
    }
}

// Returns log(N / number of papers containing the word), or 0 if the word is not in the table
double computeIDF(const HashTable& table, const std::string& word, int totalPapers) {
    const std::vector<Posting>* postings = lookupTerm(table, word);
    if (postings == nullptr || postings->empty()) {
        return 0.0;
    }
    return std::log(static_cast<double>(totalPapers) / postings->size());
}

// Adds up the TF-IDF score of every paper that contains at least one query word
std::vector<Result> scorePapers(const HashTable& table, const std::vector<std::string>& words, int totalPapers) {
    std::vector<double> scores(totalPapers, 0.0);
    std::vector<bool> matched(totalPapers, false);

    for (int w = 0; w < (int)words.size(); w++) {
        bool repeated = false;
        for (int k = 0; k < w; k++) {
            if (words[k] == words[w]) {
                repeated = true;
            }
        }
        if (repeated) {
            continue;
        }
        const std::vector<Posting>* postings = lookupTerm(table, words[w]);
        if (postings == nullptr) {
            continue;
        }
        double idf = computeIDF(table, words[w], totalPapers);
        for (const Posting& p : *postings) {
            scores[p.paper] += p.count * idf;
            matched[p.paper] = true;
        }
    }
    std::vector<Result> results;
    for (int i = 0; i < totalPapers; i++) {
        if (matched[i]) {
            results.push_back({i, scores[i]});
        }
    }
    return results;
}

// Merges the sorted halves results[left..mid] and results[mid+1..right], highest score first
void mergeHalves(std::vector<Result>& results, int left, int mid, int right) {
    std::vector<Result> merged;
    int i = left;
    int j = mid + 1;
    while (i <= mid && j <= right) {
        if (results[i].score >= results[j].score) {
            merged.push_back(results[i]);
            i++;
        } else {
            merged.push_back(results[j]);
            j++;
        }
    }
    while (i <= mid) {
        merged.push_back(results[i]);
        i++;
    }
    while (j <= right) {
        merged.push_back(results[j]);
        j++;
    }
    for (int k = 0; k < (int)merged.size(); k++) {
        results[left + k] = merged[k];
    }
}

// Recursively sorts results[left..right] by score
void mergeSortRange(std::vector<Result>& results, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortRange(results, left, mid);
    mergeSortRange(results, mid + 1, right);
    mergeHalves(results, left, mid, right);
}

// Sorts results by score, highest first; equal scores keep their original order
void mergeSortResults(std::vector<Result>& results) {
    if (results.size() > 1) {
        mergeSortRange(results, 0, (int)results.size() - 1);
    }
}

// Tokenizes the query, scores matching papers, and returns them ranked
std::vector<Result> search(const std::string& query, const HashTable& table, int totalPapers) {
    std::vector<std::string> words = tokenize(query);
    std::vector<Result> results = scorePapers(table, words, totalPapers);
    mergeSortResults(results);
    return results;
}