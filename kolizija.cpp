#include <bits/stdc++.h>
#include <fstream>
using namespace std;

string Hex(unsigned long long num) {
    const string hexGalimi = "0123456789abcdef";
    string result;

    while (num > 0) {
        result = hexGalimi[num & 15] + result;
        num >>= 4;
    };

    return result;
}

string hashFunkcija(const string& input) {
    unsigned long long hash = 0;
    const unsigned long long prime = 31;

    for (char c : input) {
        hash = hash * prime + c * (c + 7) + 12345;
    }

    string resultHex = Hex(hash);

    while (resultHex.length() < 64) {
        for (char c : resultHex) {
            hash = hash * prime + c * (c + 5) + 54321;
        }
        resultHex += Hex(hash);
    }

    return resultHex.substr(0, 64);
}

vector<pair<string, string>> readStringPairs(const string& filePath) {
    vector<pair<string, string>> stringPairs;
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return stringPairs;
    }

    string line;
    while (getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string str1 = line.substr(0, commaPos);
            string str2 = line.substr(commaPos + 1);
            stringPairs.emplace_back(str1, str2);
        }
    }

    file.close();
    return stringPairs;
}

int main() {
    string filePath = "test_files/random_string_pairs.txt"; 

    vector<pair<string, string>> stringPairs = readStringPairs(filePath);
    if (stringPairs.empty()) {
        cerr << "No string pairs found in the file!" << endl;
        return 1;
    }

    int totalPairs = stringPairs.size();
    int collisions = 0;

    for (const auto& pair : stringPairs) {
        string hash1 = hashFunkcija(pair.first);
        string hash2 = hashFunkcija(pair.second);

        if (hash1 == hash2) {
            collisions++;
        }
    }

    cout << "Total string pairs checked: " << totalPairs << endl;
    cout << "Total collisions found: " << collisions << endl;
    cout << "Collision rate: " << (collisions / (double)totalPairs) * 100 << "%" << endl;

    return 0;
}
