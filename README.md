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
