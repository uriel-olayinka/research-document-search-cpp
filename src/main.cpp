#include "common.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

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

void printMenu() {
    std::cout << "\n=== Research Document Search Engine ===\n"
              << "1. Load papers from file\n"
              << "2. Search papers by keyword\n"
              << "3. List loaded papers\n"
              << "0. Exit\n"
              << "Select an option: ";
}

}  // namespace

int main() {
    std::vector<Paper> papers;
    int badLines = 0;

    const std::string defaultFile = "data/sample_papers.txt";
    if (loadPapers(defaultFile, papers, badLines)) {
        std::cout << "Loaded " << papers.size() << " papers from " << defaultFile
                  << " (skipped bad lines: " << badLines << ").\n";
    }

    while (true) {
        printMenu();
        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');  

        if (choice == 0) {
            std::cout << "Exiting...\n";
            break;
        } else if (choice == 1) {
            std::string path;
            std::cout << "Enter file path (e.g., data/sample_paper.txt): ";
            std::getline(std::cin, path);

            int currentBad = 0;
            if (loadPapers(path, papers, currentBad)) {
                badLines = currentBad;
                std::cout << "Loaded " << papers.size() << " papers "
                          << "(skipped bad lines: " << badLines << ").\n";
            } else {
                std::cout << "Error: Could not open file '" << path << "'.\n";
            }
        } else if (choice == 2) {
            std::string query;
            std::cout << "Enter search query: ";
            std::getline(std::cin, query);

            const auto results = searchPapers(papers, query);
            std::cout << "Found " << results.size() << " result(s):\n";
            for (const auto& result : results) {
                const Paper& paper = result.second;
                std::cout << "Score: " << result.first
                          << " | ID: " << paper.id
                          << " | Title: " << paper.title
                          << " | Category: " << paper.category << '\n';
            }
        } else if (choice == 3) {
            std::cout << "Total loaded papers: " << papers.size() << '\n';
            for (const Paper& paper : papers) {
                std::cout << "[" << paper.id << "] " << paper.title
                          << " (" << paper.category << ")\n";
            }
        } else {
            std::cout << "Unknown option. Try again.\n";
        }
    }

    return 0;
}