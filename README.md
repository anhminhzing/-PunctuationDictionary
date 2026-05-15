# -PunctuationDictionary
1. Compile the files
g++ main.cpp -std=c++17 -o main      
2. Run the file
./main 


Todo list
1. Create an empty dictionary:
  punctuationDict (done)

2. For each txt file in books folder:
   a. Read the whole file into one string (done)
   b. Go through the text character by character (done)
   c. Build one sentence at a time (done)
   d. When you find '.', '?', or '!', treat it as the end of a sentence (done)

3. For each sentence:
   a. Count word positions (done)
   b. Find punctuation marks (done)
   c. Save punctuation as:
      wordPosition + punctuation (done)

4. Add the punctuation pattern into the dictionary:
   punctuationDict[pattern]++ (done)

5. After all books are processed:
   Save the dictionary to an output file (done)
   
-----------------------------------   

Overview

This project analyzes punctuation patterns from multiple text files (books).
The program reads .txt files, splits the text into sentences, detects punctuation positions, and stores the punctuation patterns in a dictionary.

Example:

Sentence:
This, I believe, is a test.

Pattern:
1,3,6.

The dictionary records how many times each punctuation pattern appears across all books.

Folder Structure
project/
 ├── main.cpp
 ├── run.sh
 ├── books/
 │    ├── book1.txt
 │    ├── book2.txt
 │    └── book3.txt

How to Run
Step 1 — Open Terminal

Navigate to the project folder:

cd path/to/project
Step 2 — Make Script Executable

Run this command once:

chmod +x run.sh
./run.sh

The program:

Reads all .txt files inside the books folder
Splits text into sentences
Detects punctuation positions
Builds a punctuation dictionary
Counts frequency of each punctuation pattern
Saves output files
