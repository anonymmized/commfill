# commfill

A lightweight C utility that formats text files by padding each line with spaces to match the longest line in the file, then appends a C-style comment (`//`) at the end of every line.

Perfect for adding clean, aligned inline comments to configuration files, code snippets, or any plain text!

📝 Example

**Before**:
```c
int x = 1;
hello
a
```
**After running** `commfill`:
```C
int x = 1; //
hello      //
a          //
```
## 🛠️ How It Works

1) Reads the input file and finds the length of the longest line (excluding newline characters).
2) Rewrites the file so that every line:
    - Keeps its original content,
    - Is padded with trailing spaces to match the maximum line length,
    - Ends with `//` followed by a newline.
3) Uses a temporary file to safely update the original without data loss.
## ▶️ Usage

1) Compile the program:
```bash
gcc commfill.c -o commfill
```
2) Run it and enter the filename when prompted:
```bash
./comment-align
Enter the filename (with extension): example.txt
```
3) The original file will be updated in place.

>💡 The program works with any plain text file. It checks for a file extension (e.g., .c, .txt) and warns if missing, but will process the file regardless.

## ⚠️ Notes

- Lines longer than 999 characters are handled correctly (partial line reading with overflow counting).
- The tool preserves the original content and only modifies line endings for alignment.
- On Unix-like systems (macOS, Linux), temporary files are used safely via rename().

## 📄 License

This project is free and open-source. Feel free to use, modify, and distribute it for personal or educational purposes.

> Made with love for clean code.

> Part of a growing collection of C algorithms and utilities.

## 🔮 Future Plans

### Here’s what’s coming next:

- 🧹 ***Remove existing comments***
    Add an option to automatically strip all trailing `// ...` comments from the file before reformatting — perfect for cleaning up and re-aligning.

- ⚙️ ***Command-line options***
    Support intuitive flags like:
    ```bash
    ./comment-align -i input.txt -r    # -r: remove old comments
    ./comment-align --no-comment       # skip adding "//" entirely
    ./comment-align -w 80              # enforce fixed width (ignore max line)
    ```

- 🔄 ***Non-destructive mode***
    Optionally write output to a new file (e.g., input.txt.aligned) instead of overwriting the original.

- 🌐 ***Multi-language support***
    Detect or specify comment style: // (C/C++/Java), # (Python/Shell), -- (SQL), etc.

- 📂 ***Batch processing***
    Apply formatting to multiple files or entire directories.
    Your ideas are welcome! Feel free to open an issue or pull request on GitHub.

## ⚠️ Note
> ***This tool aligns lines by byte count, not visual length. Strings containing escape sequences (like \n, \t) will appear misaligned in editors because they are counted as multiple bytes, but displayed as single characters.***