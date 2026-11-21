#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

struct Paper {
    std::string id;
    std::string title;
    std::string abstractText;
    std::string category;
};

// Reads papers from a file into the vector; counts skipped bad lines; returns false if the file can't be opened
bool loadPapers(const std::string& filename, std::vector<Paper>& papers, int& badLines);

// Lowercases text, removes punctuation, and splits it into words
std::vector<std::string> tokenize(const std::string& text);

#endif