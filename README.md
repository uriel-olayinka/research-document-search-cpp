# Research Document Search Engine (C++)

A menu-driven C++ search engine that finds and ranks research-paper records by keyword using a hand-built hash-table inverted index and TF-IDF scoring.

**Team:** Uriel Olayinka, Shang-Yun Hsieh

## Data format
One paper per line, fields separated by `|`:
```
id|title|abstract|category
```
The course dataset has not been provided yet, so `data/` contains synthetic sample files.

## Build
```
g++ -std=c++17 -O2 src/*.cpp -o searchengine
```

## Run
```
./searchengine data/sample_small.txt
```

## Current progress
In progress (Iteration 3).