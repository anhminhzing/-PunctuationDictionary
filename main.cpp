#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cctype>


using namespace std;

namespace fs = std::filesystem;

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

bool isAbbreviationWord(const string& word) {
    string lower = "";

    for (char ch : word) {
        lower += tolower(ch);
    }

    return lower == "mr" ||
           lower == "mrs" ||
           lower == "ms" ||
           lower == "dr" ||
           lower == "prof" ||
           lower == "sr" ||
           lower == "jr" ||
           lower == "st";
}

bool isSupportedPunctuation(char ch) {
    string punctuations = ".,?!;:()[]{}-";
    return punctuations.find(ch) != string::npos;
}


vector<string> splitSentences(const string& text) {
    vector<string> sentences;
    string currentSentence;

    for (int i = 0; i < text.size(); i++) {
        char ch = text[i];
        currentSentence += ch;

        if (isSentenceEnd(ch)) {

            if (ch == '.' && isAbbreviationWord(currentSentence)) {
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


string getPunctuationPattern(const string& sentence) {
    string pattern = "";
    string currentWord = "";

    int wordPosition = 0;
    bool insideWord = false;

    for (int i = 0; i < sentence.size(); i++) {
        char ch = sentence[i];

        if (isalnum(ch)) {
            if (!insideWord) {
                wordPosition++;
                insideWord = true;
                currentWord = "";
            }

            currentWord += ch;
        }
        else {
            insideWord = false;

            if (isSupportedPunctuation(ch)) {

                // Ignore period after abbreviation
                if (ch == '.' && isAbbreviationWord(currentWord)) {
                    continue;
                }

                // Ignore punctuation before any word
                if (wordPosition == 0) {
                    continue;
                }

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
    unordered_map<string, int> punctuationDict;
    SummaryStats stats;

    string folderPath = "demo"; // change path to "books" for real books

    for (const auto& entry : fs::directory_iterator(folderPath)) {

        if (entry.path().extension() == ".txt") {

            stats.totalFiles++;

            cout << "Reading file: "
                 << entry.path().filename()
                 << endl;

            ifstream file(entry.path());

            if (!file) {
                cout << "Cannot open file." << endl;
                continue;
            }

            stringstream buffer;
            buffer << file.rdbuf();

            string text = buffer.str();

            vector<string> sentences = splitSentences(text);

            stats.totalSentences += sentences.size();

            for (string sentence : sentences) {
                string pattern = getPunctuationPattern(sentence);

                if (!pattern.empty()) {
                    punctuationDict[pattern]++;
                    stats.totalPatterns++;
                }
            }
        }
    }

    printDictionary(punctuationDict);

    saveDictionaryToFile(punctuationDict, "punctuation_dictionary.txt");

    saveSummaryStats(stats, punctuationDict, "summary.txt");

    return 0;
}

