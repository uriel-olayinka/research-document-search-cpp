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
    std::vector<Result> r = {{0, 1.0}, {1, 3.0}, {2, 2.0}, {3, 3.0}};
    mergeSortResults(r);
    check(r[0].paper == 1 && r[1].paper == 3 && r[2].paper == 2 && r[3].paper == 0, "merge sort: highest first, ties keep order");

    std::vector<Paper> papers;
    papers.push_back(makePaper("P001", "Graph Search", "Graph algorithms find shortest paths."));
    papers.push_back(makePaper("P002", "Tree Basics", "A tree is a graph with no cycles."));
    papers.push_back(makePaper("P003", "Sorting", "Merge sort and quick sort can order a graph."));

    HashTable table;
    initTable(table, 8);
    buildIndex(papers, table);
    int n = (int)papers.size();

    std::vector<Result> s1 = search("sort", table, n);
    check(s1.size() == 1 && s1[0].paper == 2 && closeTo(s1[0].score, 2.1972), "sort: only paper 2, score 2 x 1.0986");

    std::vector<Result> s2 = search("a sort", table, n);
    check(s2.size() == 2 && s2[0].paper == 2 && s2[1].paper == 1, "a sort: paper 2 ranked above paper 1");
    check(s2.size() == 2 && closeTo(s2[0].score, 2.6027) && closeTo(s2[1].score, 0.8109), "a sort: scores 2.6027 and 0.8109");

    std::vector<Result> s3 = search("graph tree", table, n);
    check(s3.size() == 3 && s3[0].paper == 1 && s3[1].paper == 0 && s3[2].paper == 2, "graph tree: paper 1 first, then tied papers 0 and 2 in order");

    std::vector<Result> s4 = search("Tree TREE tree!", table, n);
    check(s4.size() == 1 && closeTo(s4[0].score, 2.1972), "repeated query word is not counted twice");

    check(search("banana", table, n).empty(), "word in no paper gives no results");
    check(search("", table, n).empty(), "empty query gives no results");
    check(search("!!! ...", table, n).empty(), "punctuation-only query gives no results");

    freeTable(table);
    return 0;
}