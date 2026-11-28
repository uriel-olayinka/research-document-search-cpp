#include <iostream>
#include <cmath>
#include "index.h"
#include "linear.h"

// Prints PASS or FAIL for one check
void check(bool ok, const std::string& name) {
    std::cout << (ok ? "PASS: " : "FAIL: ") << name << "\n";
}

// Returns true if both result lists have the same papers, order, and exact scores
bool sameResults(const std::vector<Result>& a, const std::vector<Result>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i].paper != b[i].paper || a[i].score != b[i].score) {
            return false;
        }
    }
    return true;
}

// Loads a file, builds the index, and checks both searches agree on every query
void compareOnFile(const std::string& filename, const std::vector<std::string>& queries) {
    std::vector<Paper> papers;
    int badLines = 0;
    if (!loadPapers(filename, papers, badLines)) {
        std::cout << "FAIL: could not open " << filename << "\n";
        return;
    }
    HashTable table;
    initTable(table, 16);
    buildIndex(papers, table);
    int n = (int)papers.size();

    for (const std::string& query : queries) {
        std::vector<Result> indexed = search(query, table, n);
        std::vector<Result> linear = linearSearch(query, papers);
        check(sameResults(indexed, linear),
              filename + ": \"" + query + "\" (" + std::to_string(indexed.size()) + " results)");
    }
    freeTable(table);
}

int main() {
    std::vector<Paper> papers;
    int badLines = 0;
    loadPapers("data/sample_small.txt", papers, badLines);
    std::vector<Result> g = linearSearch("graph", papers);
    check(g.size() == 3 && papers[g[0].paper].id == "P002" && std::fabs(g[0].score - 3.6119) < 0.0001,
          "linear: graph gives P002 first with score 3.6119");
    check(linearSearch("graph", std::vector<Paper>()).empty(), "linear: no papers gives no results");

    compareOnFile("data/sample_small.txt", {
        "graph", "autocomplete", "method", "Graph,", "graph tree", "a sort",
        "tree tree tree", "search", "method graph", "", "!!!", "zzznomatch"
    });

    compareOnFile("data/sample_5000.txt", {
        "graph", "graph tree", "neural network learning", "hash table collision",
        "the", "method study approach", "sort merge quick heap", "quantum"
    });

    return 0;
}