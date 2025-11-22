#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// Danielle A.

// Build cipher alphabet from keyword
string buildCipher(const string& keyword) {
    bool used[26] = {false};
    string cipher = "";

    // Add unique letters from keyword
    for (char c : keyword) {
        c = toupper(c);
        if (c >= 'A' && c <= 'Z' && !used[c - 'A']) {
            cipher += c;
            used[c - 'A'] = true;
        }
    }

    // Add remaining letters in reverse order
    for (char c = 'Z'; c >= 'A'; --c) {
        if (!used[c - 'A']) cipher += c;
    }

    return cipher;
}

// Encrypt a single character
char encryptChar(char c, const string& cipher) {
    if (isalpha(c)) {
        bool lower = islower(c);
        char upper = toupper(c);
        int index = upper - 'A';
        char mapped = cipher[index];
        return lower ? tolower(mapped) : mapped;
    }
    return c;
}

// Decrypt a single character
char decryptChar(char c, const string& cipher) {
    if (isalpha(c)) {
        bool lower = islower(c);
        char upper = toupper(c);

        // Find index in cipher alphabet
        size_t index = cipher.find(upper);
        if (index != string::npos) {
            char mapped = 'A' + static_cast<char>(index);
            return lower ? tolower(mapped) : mapped;
        }
    }
    return c;
}

int main(int argc, char *argv[]) {
    bool decrypt = false;
    string keyword = "";
    string infile = "", outfile = "";

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-d") {
            decrypt = true;
        }
        else if (arg.rfind("-k", 0) == 0) { // starts with "-k"
            keyword = arg.substr(2);
        }
        else if (infile.empty()) {
            infile = arg;
        }
        else if (outfile.empty()) {
            outfile = arg;
        }
    }

    if (keyword.empty()) {
        cerr << "ERROR: Must supply keyword using -kKEYWORD\n";
        return 1;
    }
    if (infile.empty() || outfile.empty()) {
        cerr << "ERROR: Must supply input and output file names.\n";
        return 1;
    }

    // Build the cipher alphabet from the keyword
    string cipher = buildCipher(keyword);

    ifstream fin(infile);
    if (!fin) {
        cerr << "ERROR: Cannot open input file: " << infile << endl;
        return 1;
    }

    ofstream fout(outfile);
    if (!fout) {
        cerr << "ERROR: Cannot open output file: " << outfile << endl;
        return 1;
    }

    char c;
    while (fin.get(c)) {
        if (decrypt)
            fout << decryptChar(c, cipher);
        else
            fout << encryptChar(c, cipher);
    }

    return 0;
}