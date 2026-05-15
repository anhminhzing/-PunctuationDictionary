#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cctype>


using namespace std;

namespace fs = filesystem;

struct SummaryStats {
    int totalFiles = 0;
    int totalSentences = 0;
    int totalPatterns = 0;
};

void loadDictionaryFromFile(unordered_map<string, int>& punctuationDict,
                            const string& filename) {

    ifstream inFile(filename);

    if (!inFile) {
        cout << "No existing dictionary file found." << endl;
        return;
    }

    string pattern;
    string arrow;
    int count;

    while (inFile >> pattern >> arrow >> count) {
        punctuationDict[pattern] = count;
    }

    inFile.close();

    cout << "Dictionary loaded from "
         << filename
         << endl;
}

bool isSentenceEnd(char ch) {
    return ch == '.' || ch == '?' || ch == '!';
}

bool isAbbreviation(const string& sentence) {
    string lower = "";

    for (char ch : sentence) {
        lower += tolower(ch);
    }

    vector<string> abbreviations = {
        "mr.", "mrs.", "ms.", "dr.", "prof.", "sr.", "jr.",
        "st.", "vs.", "etc.", "e.g.", "i.e."
    };

    for (string abbr : abbreviations) {
        if (lower.size() >= abbr.size()) {
            if (lower.substr(lower.size() - abbr.size()) == abbr) {
                return true;
            }
        }
    }

    return false;
}

vector<string> splitSentences(const string& text) {
    vector<string> sentences;
    string currentSentence;

    for (int i = 0; i < text.size(); i++) {
        char ch = text[i];
        currentSentence += ch;

        if (isSentenceEnd(ch)) {

            if (ch == '.' && isAbbreviation(currentSentence)) {
                continue;
            }

            while (i + 1 < text.size() &&
                  (text[i + 1] == '"' || text[i + 1] == '\'' ||
                   text[i + 1] == ')' || text[i + 1] == ']')) {
                i++;
                currentSentence += text[i];
            }

            sentences.push_back(currentSentence);
            currentSentence.clear();
        }
    }

    return sentences;
}

bool isSupportedPunctuation(char ch) {
    string punctuations = ".,?!;:\"'()[]{}-";

    return punctuations.find(ch) != string::npos;
}

string getPunctuationPattern(const string& sentence) {
    string pattern = "";
    int wordPosition = 0;
    bool insideWord = false;

    for (char ch : sentence) {

        if (isalnum(ch)) {
            if (!insideWord) {
                wordPosition++;
                insideWord = true;
            }
        }
        else {
            insideWord = false;

            if (isSupportedPunctuation(ch)) {
                pattern += to_string(wordPosition);
                pattern += ch;
            }
        }
    }

    return pattern;
}

void printDictionary(const unordered_map<string, int>& punctuationDict) {

    cout << "\n===== Punctuation Dictionary =====\n";

    for (const auto& item : punctuationDict) {

        cout << item.first
             << " -> "
             << item.second
             << endl;
    }

    cout << "==================================\n";
}

void saveDictionaryToFile(const unordered_map<string, int>& punctuationDict,
                          const string& filename) {

    ofstream outFile(filename);

    if (!outFile) {
        cout << "Cannot create output file." << endl;
        return;
    }

    for (const auto& item : punctuationDict) {

        outFile << item.first
                << " -> "
                << item.second
                << endl;
    }

    outFile.close();

    cout << "Dictionary saved to "
         << filename
         << endl;
}

void saveSummaryStats(const SummaryStats& stats, const unordered_map<string, int>& punctuationDict, const string& filename) {

    ofstream outFile(filename);

    if (!outFile) {
        cout << "Cannot create summary file." << endl;
        return;
    }

    outFile << "===== Summary Statistics =====" << endl;
    outFile << "Total files read: " << stats.totalFiles << endl;
    outFile << "Total sentences processed: " << stats.totalSentences << endl;
    outFile << "Total punctuation patterns found: " << stats.totalPatterns << endl;
    outFile << "Unique punctuation patterns: " << punctuationDict.size() << endl;

    string mostCommonPattern = "";
    int highestCount = 0;

    for (const auto& item : punctuationDict) {
        if (item.second > highestCount) {
            mostCommonPattern = item.first;
            highestCount = item.second;
        }
    }

    outFile << "Most common pattern: "
            << mostCommonPattern
            << " -> "
            << highestCount
            << endl;

    outFile.close();

    cout << "Summary saved to " << filename << endl;
}


int main() {
    // empty dictionary
    unordered_map<string, int> punctuationDict;
    SummaryStats stats;
    // load the existing file and save it to punctuationDict
    loadDictionaryFromFile(punctuationDict,
                           "punctuation_dictionary.txt");
    
    // folderPath = demo or folderPath = books
    string folderPath = "books";
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
            stats.totalFiles++;
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
            
            
            // Get arrays of string (list of sentences)
            vector<string> sentences = splitSentences(text);
            stats.totalSentences += sentences.size();
            
//            3. split sentenses into words, count words possition and detect punctuation
            
            for (string sentence : sentences) {
//                cout << sentence << endl;
                string pattern = getPunctuationPattern(sentence);

                if (!pattern.empty()) {
                    punctuationDict[pattern]++;
                    stats.totalPatterns++;
                }
            }
            
//            printDictionary(punctuationDict);
            saveDictionaryToFile(punctuationDict, "punctuation_dictionary.txt");
            saveSummaryStats(stats, punctuationDict, "summary.txt");
                
        }
    }

    return 0;
}

