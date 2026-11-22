#include "common.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string token;

    for (unsigned char ch : text) {
        if (std::isalnum(ch)) {
            token += static_cast<char>(std::tolower(ch));
        } else if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

bool loadPapers(const std::string& filename, std::vector<Paper>& papers, int& badLines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    papers.clear();
    badLines = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 4 || fields[0].empty() || fields[1].empty()) {
            ++badLines;
            continue;
        }

        Paper paper;
        paper.id = fields[0];
        paper.title = fields[1];
        paper.abstractText = fields[2];
        paper.category = fields[3];
        papers.push_back(paper);
    }

    return true;
}