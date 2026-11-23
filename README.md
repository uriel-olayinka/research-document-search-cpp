# Research Document Search Engine (C++)

A menu-driven C++ search engine that finds and ranks research-paper records by keyword using a hand-built hash-table inverted index and TF-IDF scoring.

**Team:** Uriel Olayinka, Shang-Yun Hsieh

## Data format
One paper per line, fields separated by `|`:
```
id|title|abstract|category
```
A line is skipped and counted as a bad line if it does not have exactly 4 fields or has an empty id or title. A trailing `|` with an empty category (for example `id|title|abstract|`) also counts as a bad line. Empty lines are ignored.

The course dataset has not been provided yet, so `data/` contains synthetic sample files.

## Build
```
g++ -std=c++17 -O2 src/*.cpp -o searchengine
```

## Run
Run from the project root. It loads `data/sample_5000.txt` on start.
```
./searchengine
```
Use menu option 1 to load a different file, for example `data/sample_small.txt`. Search shows the top 10 ranked results.