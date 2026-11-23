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

Run from the project root:

```powershell
g++ -std=c++17 -O2 .\src\*.cpp -o .\searchengine.exe
```

## Run

Run from the project root. The program loads `data/sample_5000.txt` on start.

```powershell
.\searchengine.exe
```

Use menu option 1 to load another file, such as `data/sample_small.txt`. Search displays up to the top 10 ranked results.

## Testing

The following tests were run through the program's menu. Actual results matched the expected results in each case.

| Test | Input or scenario | Expected result | Actual result | Pass/Fail |
| ---- | ----------------- | --------------- | ------------- | --------- |
| Normal search | Load `data/sample_small.txt`; search for `graph` | Return papers matching the query | Matching papers were returned | Pass |
| Empty query (boundary) | Load `data/sample_small.txt`; submit an empty search query | Return 0 results | 0 results | Pass |
| No-match search | Load `data/sample_small.txt`; search for `zzznomatch` | Return 0 results | 0 results | Pass |
| Invalid records | Load `data/malformed.txt` | Load 3 valid papers and report 5 skipped bad lines | 3 papers loaded; 5 bad lines skipped | Pass |
| Empty input | Load `data/empty.txt`; search for `anything` | Load 0 papers and return 0 results | 0 papers loaded; 0 results | Pass |
| Larger input | Load `data/sample_5000.txt`; search for `graph` | Load 5,000 papers and display no more than 10 matching results | 5,000 papers loaded; up to 10 matching results displayed | Pass |

Expected results were determined by inspecting the test files and applying the documented loading and search behavior. For example, `data/malformed.txt` contains three valid records and five invalid nonempty lines, while `data/empty.txt` contains no records. The search expectations were based on whether the query occurs in the loaded records and the program's 10-result display limit.

One design issue checked was how an empty search query behaves: it returns zero results without a separate explanation in the interface. The test confirmed this behavior, and it is documented in the boundary case above. No unresolved failures were observed in the cases tested.
