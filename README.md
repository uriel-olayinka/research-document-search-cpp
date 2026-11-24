# Research Document Search Engine (C++)

A menu-driven C++ search engine that finds and ranks research-paper records by keyword using a hand-built hash-table inverted index and TF-IDF scoring.

**Team:** Uriel Olayinka, Shang-Yun Hsieh

## Data format

One paper per line, with fields separated by `|`:

```text
id|title|abstract|category
```

A line is skipped and counted as a bad line if it does not have exactly four fields or has an empty ID or title. A trailing `|` with an empty category (for example, `id|title|abstract|`) also counts as a bad line. Empty lines are ignored.

The course dataset has not been provided yet, so `data/` contains synthetic sample files.

## Build

Run from the project root. On macOS or Linux:

```bash
g++ -std=c++17 -O2 src/*.cpp -o searchengine
./searchengine
```

On Windows PowerShell:

```powershell
g++ -std=c++17 -O2 .\src\*.cpp -o .\searchengine.exe
.\searchengine.exe
```

The program loads `data/sample_5000.txt` on start. Use menu option 1 to load another file, such as `data/sample_small.txt`. Search displays up to the top 10 ranked results.

## Current Progress

The program loads pipe-delimited paper records, builds an inverted index, and searches and ranks matches using TF-IDF. The command-line menu supports loading a file, listing loaded papers, and searching. The index is rebuilt at startup and whenever a new file is loaded. Sample data and tests for the loader, hash table, index, and search are included.

## Known Limitations

- Search does not use stemming, so words such as `graph` and `graphs` are treated as different terms.
- Multi-word queries use OR matching: a paper is returned if it matches at least one query term.
- Stop words are not removed from documents or queries.
- The included paper records are synthetic because the course dataset has not been provided.
- The index is rebuilt each time the program runs; it is not saved to disk.
- Input fields cannot contain an escaped `|` character.

## Testing

### Unit tests

Three test programs in `tests/` check the core functions directly. Each prints `PASS` or `FAIL` for every check. Run from the project root.

On macOS or Linux:

```bash
g++ -std=c++17 -Wall -Isrc src/index.cpp tests/test_hash.cpp -o test_hash
./test_hash
g++ -std=c++17 -Wall -Isrc src/index.cpp src/loader.cpp tests/test_index.cpp -o test_index
./test_index
g++ -std=c++17 -Wall -Isrc src/index.cpp src/loader.cpp tests/test_search.cpp -o test_search
./test_search
```

On Windows PowerShell, use the same commands with `.exe` output names (for example `-o test_hash.exe`, then `.\test_hash.exe`).

Expected: 7 checks pass in `test_hash`, 10 in `test_index`, and 9 in `test_search`.

### Menu tests

The following tests were run through the program's menu. Actual results matched the expected results in each case.

| Test | Input or scenario | Expected result | Actual result | Pass/Fail |
| ---- | ----------------- | --------------- | ------------- | --------- |
| Normal search | Load `data/sample_small.txt`; search for `graph` | P002 first (score ≈ 3.6119), followed by P001 and P009 tied (score ≈ 1.2040 each) | P002 first; P001 and P009 tied at the expected scores | Pass |
| Empty query (boundary) | Load `data/sample_small.txt`; submit an empty search query | Return 0 results | 0 results | Pass |
| No-match search | Load `data/sample_small.txt`; search for `zzznomatch` | Return 0 results | 0 results | Pass |
| Invalid records | Load `data/malformed.txt` | Load 3 valid papers and report 5 skipped bad lines | 3 papers loaded; 5 bad lines skipped | Pass |
| Empty input | Load `data/empty.txt`; search for `anything` | Load 0 papers and return 0 results | 0 papers loaded; 0 results | Pass |
| Larger input | Load `data/sample_5000.txt`; search for `graph` | Load 5,000 papers and display no more than 10 matching results | 5,000 papers loaded; up to 10 matching results displayed | Pass |

Expected results were determined by inspecting the test files and applying the documented loading and search behavior. For the `graph` query, the term appears three times in P002 and once each in P001 and P009. It occurs in three papers total, so its IDF for this ten-paper dataset is `ln(10/3) ≈ 1.2040`; multiplying by term frequency gives P002 ≈ 3.6119 and P001/P009 ≈ 1.2040 each. `data/malformed.txt` contains three valid records and five invalid nonempty lines, while `data/empty.txt` contains no records. Other search expectations were based on whether query terms occur in the loaded records and the program's 10-result display limit.

One design issue checked was how an empty search query behaves: it returns zero results without a separate explanation in the interface. The test confirmed this behavior, and it is documented in the boundary case above. No unresolved failures were observed in the cases tested.
