#ifndef LINEAR_H
#define LINEAR_H

#include <string>
#include <vector>
#include "common.h"
#include "index.h"

// Scores every paper by scanning its text (no index) and returns matches ranked by TF-IDF
std::vector<Result> linearSearch(const std::string& query, const std::vector<Paper>& papers);

#endif