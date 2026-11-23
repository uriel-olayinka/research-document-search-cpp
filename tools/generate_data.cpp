#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

// Picks a random word from the list
std::string pickWord(const std::vector<std::string>& list, std::mt19937& rng) {
    return list[rng() % list.size()];
}

// Writes the requested number of synthetic paper records, using a fixed seed so the file is the same every run
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./generate_data <number of papers> <output file>\n";
        return 1;
    }
    int count = std::atoi(argv[1]);
    if (count <= 0) {
        std::cerr << "Number of papers must be a positive whole number.\n";
        return 1;
    }
    std::ofstream out(argv[2]);
    if (!out) {
        std::cerr << "Could not open " << argv[2] << " for writing.\n";
        return 1;
    }

    std::vector<std::string> vocab = {
        "graph", "tree", "hash", "table", "search", "sort", "merge", "quick", "heap", "queue",
        "stack", "array", "list", "node", "edge", "path", "shortest", "spanning", "network", "neural",
        "learning", "gradient", "descent", "model", "data", "index", "query", "ranking", "document", "text",
        "prefix", "trie", "binary", "balanced", "rotation", "collision", "chaining", "probe", "cache", "memory",
        "parallel", "distributed", "complexity", "analysis", "dynamic", "programming", "greedy", "recursion", "matrix", "vector",
        "cluster", "classification", "regression", "optimization", "compression", "encryption", "protocol", "database", "transaction", "scheduling"
    };
    std::vector<std::string> fillers = {"the", "a", "of", "for", "with", "and", "in", "on", "using", "we", "this", "method", "study", "approach"};
    std::vector<std::string> categories = {"algorithms", "data structures", "machine learning", "information retrieval", "systems"};

    std::mt19937 rng(42);
    for (int i = 1; i <= count; i++) {
        std::string title;
        int titleLength = 3 + rng() % 3;
        for (int w = 0; w < titleLength; w++) {
            if (w > 0) {
                title += " ";
            }
            title += pickWord(vocab, rng);
        }

        std::string abstractText;
        int abstractLength = 20 + rng() % 21;
        for (int w = 0; w < abstractLength; w++) {
            if (w > 0) {
                abstractText += " ";
            }
            if (rng() % 3 == 0) {
                abstractText += pickWord(fillers, rng);
            } else {
                abstractText += pickWord(vocab, rng);
            }
        }
        abstractText += ".";

        out << "G" << i << "|" << title << "|" << abstractText << "|" << pickWord(categories, rng) << "\n";
    }

    std::cout << "Wrote " << count << " papers to " << argv[2] << "\n";
    return 0;
}