// Baseline linear-scan search, kept for the Iteration 4 comparison.
// Not part of the main build (lives in tests/, not src/).
#include "../src/common.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

std::vector<std::pair<int, Paper>> searchPapers(
    const std::vector<Paper>& papers,
    const std::string& query) {
    const auto queryTokens = tokenize(query);
    std::vector<std::pair<int, Paper>> results;

    for (const Paper& paper : papers) {
        const auto titleTokens = tokenize(paper.title);
        const auto abstractTokens = tokenize(paper.abstractText);
        int score = 0;

        for (const std::string& qToken : queryTokens) {
            score += static_cast<int>(
                std::count(titleTokens.begin(), titleTokens.end(), qToken));
            score += static_cast<int>(
                std::count(abstractTokens.begin(), abstractTokens.end(), qToken));
        }

        if (score > 0) {
            results.push_back({score, paper});
        }
    }

    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) {
                  if (a.first != b.first) {
                      return a.first > b.first;
                  }
                  return a.second.id < b.second.id;
              });

    return results;
}