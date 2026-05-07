#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>


using namespace std;

namespace fs = filesystem;

//            1. Create an empty dictionary:
//               punctuationDict (done)
//
//            2. For each txt file in books folder:
//               a. Read the whole file into one string (done)
//               b. Go through the text character by character (working)
//               c. Build one sentence at a time
//               d. When you find '.', '?', or '!', treat it as the end of a sentence
//
//            3. For each sentence:
//               a. Count word positions
//               b. Find punctuation marks
//               c. Save punctuation as:
//                  wordPosition + punctuation
//
//            4. Add the punctuation pattern into the dictionary:
//               punctuationDict[pattern]++
//
//            5. After all books are processed:
//               Save the dictionary to an output file


vector<string> splitSentences(const string& text) {

    vector<string> sentences;
    string currentSentence;

    for (char ch : text) {

        currentSentence += ch;

        // end of sentence
        if (ch == '.' || ch == '?' || ch == '!') {

            sentences.push_back(currentSentence);

            currentSentence.clear();
        }
    }

    return sentences;
}

int main() {
    // empty dictionary
    unordered_map<string, int> punctuationDict;
    
    // folderPath = demo or folderPath = books
    string folderPath = "demo";
    // the function for each txt file (access each txt file)
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        //1. Open and Read txt file and print it out
        // reading path of txt
        if (entry.path().extension() == ".txt") { // check the format of the file
            // confirm to print out the path of the txt file
            cout << "Reading file: "
                 << entry.path().filename()
                 << endl;
            // open the file
            ifstream file(entry.path());

            if (!file) {
                cout << "Cannot open file." << endl;
                continue;
            }
            // reading the txt file by using stringstream
            stringstream buffer;
            buffer << file.rdbuf();
            
            // get the string
            string text = buffer.str();
            
            cout << "Characters read: "
                 << text.size()
                 << endl;

            // print out the book
//            cout << "-------------------" << endl;
//            cout << "context: "
//                 << text
//                 << endl;
            
            
            // Go through the text character by character
            // arrays of string
            vector<string> sentences = splitSentences(text);
                for (string s : sentences) {
                    cout << s << endl;
                }

            
        }
    }

    return 0;
}

