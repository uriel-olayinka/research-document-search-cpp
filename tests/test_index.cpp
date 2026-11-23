#include <iostream>
#include <cmath>
#include "index.h"

// Prints PASS or FAIL for one check
void check(bool ok, const std::string& name) {
    std::cout << (ok ? "PASS: " : "FAIL: ") << name << "\n";
}

// Returns true if two decimal numbers are equal to 4 decimal places
bool closeTo(double a, double b) {
    return std::fabs(a - b) < 0.0001;
}

// Makes one paper record
Paper makePaper(const std::string& id, const std::string& title, const std::string& abstractText) {
    Paper p;
    p.id = id;
    p.title = title;
    p.abstractText = abstractText;
    p.category = "test";
    return p;
}

int main() {
    std::vector<Paper> papers;
    papers.push_back(makePaper("P001", "Graph Search", "Graph algorithms find shortest paths."));
    papers.push_back(makePaper("P002", "Tree Basics", "A tree is a graph with no cycles."));
    papers.push_back(makePaper("P003", "Sorting", "Merge sort and quick sort can order a graph."));

    HashTable table;
    initTable(table, 8);
    buildIndex(papers, table);

    const std::vector<Posting>* g = lookupTerm(table, "graph");
    check(g != nullptr && g->size() == 3, "graph is in all 3 papers");
    check(g != nullptr && (*g)[0].paper == 0 && (*g)[0].count == 2, "Graph in title and abstract of paper 0 counted as 2");

    const std::vector<Posting>* t = lookupTerm(table, "tree");
    check(t != nullptr && t->size() == 1 && (*t)[0].paper == 1 && (*t)[0].count == 2, "tree appears twice in paper 1 only");

    const std::vector<Posting>* s = lookupTerm(table, "sort");
    check(s != nullptr && (*s)[0].paper == 2 && (*s)[0].count == 2, "sort appears twice in paper 2");
    check(lookupTerm(table, "sorting") != nullptr, "sorting is stored as a separate word from sort");
    check(lookupTerm(table, "paths") != nullptr, "punctuation removed from paths.");

    check(closeTo(computeIDF(table, "graph", 3), 0.0), "IDF of graph (in every paper) is 0");
    check(closeTo(computeIDF(table, "tree", 3), 1.0986), "IDF of tree = ln(3/1) = 1.0986");
    check(closeTo(computeIDF(table, "a", 3), 0.4055), "IDF of a = ln(3/2) = 0.4055");
    check(closeTo(computeIDF(table, "banana", 3), 0.0), "IDF of missing word is 0");

    freeTable(table);
    return 0;
}