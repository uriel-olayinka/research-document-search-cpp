#include "linear.h"
#include <cmath>

// Removes repeated words, keeping the first time each word appears
static std::vector<std::string> uniqueWords(const std::vector<std::string>& words) {
    std::vector<std::string> unique;
    for (const std::string& w : words) {
        bool seen = false;
        for (const std::string& u : unique) {
            if (u == w) {
                seen = true;
            }
        }
        if (!seen) {
            unique.push_back(w);
        }
    }
    return unique;
}

// Scores every paper by scanning its text (no index) and returns matches ranked by TF-IDF
std::vector<Result> linearSearch(const std::string& query, const std::vector<Paper>& papers) {
    std::vector<std::string> words = uniqueWords(tokenize(query));
    int n = (int)papers.size();
    int q = (int)words.size();
    std::vector<Result> results;
    if (q == 0 || n == 0) {
        return results;
    }

    std::vector<int> df(q, 0);
    std::vector<int> matchedPapers;
    std::vector<std::vector<int>> matchedCounts;

    for (int i = 0; i < n; i++) {
        std::vector<std::string> tokens = tokenize(papers[i].title + " " + papers[i].abstractText);
        std::vector<int> counts(q, 0);
        bool found = false;
        for (const std::string& t : tokens) {
            for (int w = 0; w < q; w++) {
                if (t == words[w]) {
                    counts[w]++;
                    found = true;
                }
            }
        }
        if (found) {
            for (int w = 0; w < q; w++) {
                if (counts[w] > 0) {
                    df[w]++;
                }
            }
            matchedPapers.push_back(i);
            matchedCounts.push_back(counts);
        }
    }

    std::vector<double> idf(q, 0.0);
    for (int w = 0; w < q; w++) {
        if (df[w] > 0) {
            idf[w] = std::log(static_cast<double>(n) / df[w]);
        }
    }

    for (int m = 0; m < (int)matchedPapers.size(); m++) {
        double score = 0.0;
        for (int w = 0; w < q; w++) {
            if (matchedCounts[m][w] > 0) {
                score += matchedCounts[m][w] * idf[w];
            }
        }
        results.push_back({matchedPapers[m], score});
    }

    mergeSortResults(results);
    return results;
}