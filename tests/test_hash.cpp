#include <iostream>
#include "index.h"

// Prints PASS or FAIL for one check
void check(bool ok, const std::string& name) {
    std::cout << (ok ? "PASS: " : "FAIL: ") << name << "\n";
}

int main() {
    HashTable table;
    initTable(table, 2);

    insertTerm(table, "graph", 0);
    insertTerm(table, "graph", 0);
    insertTerm(table, "graph", 2);
    insertTerm(table, "tree", 1);

    const std::vector<Posting>* g = lookupTerm(table, "graph");
    check(g != nullptr && g->size() == 2, "graph appears in 2 papers");
    check(g != nullptr && (*g)[0].paper == 0 && (*g)[0].count == 2, "graph counted twice in paper 0");
    check(g != nullptr && (*g)[1].paper == 2 && (*g)[1].count == 1, "graph counted once in paper 2");
    check(lookupTerm(table, "banana") == nullptr, "missing word returns nullptr");

    for (int i = 0; i < 100; i++) {
        insertTerm(table, "w" + std::to_string(i), 3);
    }

    bool allFound = true;
    for (int i = 0; i < 100; i++) {
        const std::vector<Posting>* p = lookupTerm(table, "w" + std::to_string(i));
        if (p == nullptr || (*p)[0].paper != 3) {
            allFound = false;
        }
    }
    check(allFound, "all 100 words found after resizes");
    check(lookupTerm(table, "tree") != nullptr, "tree still found after resizes");
    check(table.size == 102, "table holds 102 unique words");
    std::cout << "Bucket count: " << table.bucketCount << "\n";

    freeTable(table);
    return 0;
}