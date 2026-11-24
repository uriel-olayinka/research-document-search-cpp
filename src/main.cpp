#include "common.h"
#include "index.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

constexpr int kInitialBuckets = 1024;
constexpr size_t kMaxResults = 10;

void printMenu() {
    std::cout << "\n=== Research Document Search Engine ===\n"
              << "1. Load papers from file\n"
              << "2. Search papers by keyword\n"
              << "3. List loaded papers\n"
              << "0. Exit\n"
              << "Select an option: ";
}

// Throws away the old index and builds a new one from the loaded papers
void rebuildIndex(HashTable& table, const std::vector<Paper>& papers) {
    freeTable(table);
    initTable(table, kInitialBuckets);
    buildIndex(papers, table);
}

}  // namespace

int main() {
    std::vector<Paper> papers;
    int badLines = 0;

    HashTable table{};
    initTable(table, kInitialBuckets);

    const std::string defaultFile = "data/sample_5000.txt";
    if (loadPapers(defaultFile, papers, badLines)) {
        rebuildIndex(table, papers);
        std::cout << "Loaded " << papers.size() << " papers from " << defaultFile
                  << " (skipped bad lines: " << badLines << ").\n";
    } else {
        std::cout << "Error: could not load default file '" << defaultFile << "'.\n";
    }

    while (true) {
        printMenu();
        int choice = -1;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            std::cout << "Exiting...\n";
            break;
        } else if (choice == 1) {
            std::string path;
            std::cout << "Enter file path (e.g., data/sample_5000.txt): ";
            if (!std::getline(std::cin, path)) break;

            int currentBad = 0;
            if (loadPapers(path, papers, currentBad)) {
                badLines = currentBad;
                rebuildIndex(table, papers);
                std::cout << "Loaded " << papers.size() << " papers "
                          << "(skipped bad lines: " << badLines << ").\n";
            } else {
                std::cout << "Error: Could not open file '" << path << "'.\n";
            }
        } else if (choice == 2) {
            std::string query;
            std::cout << "Enter search query: ";
            if (!std::getline(std::cin, query)) break;

            const auto results = search(query, table, static_cast<int>(papers.size()));
            std::cout << "Found " << results.size() << " result(s)";
            if (results.size() > kMaxResults) {
                std::cout << " (showing top " << kMaxResults << ")";
            }
            std::cout << ":\n";
            for (size_t i = 0; i < results.size() && i < kMaxResults; ++i) {
                const Paper& paper = papers[results[i].paper];
                std::cout << "Score: " << std::fixed << std::setprecision(4) << results[i].score
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

    freeTable(table);
    return 0;
}
