#include <bits/stdc++.h>
#include <string>
using namespace std;

string Hex(unsigned long long num) {
    const string hexGalimi = "0123456789abcdef";
    string result;

    while (num > 0) {
        result = hexGalimi[num & 15] + result;
        num >>= 4;
    }

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

int countDifferingBits(const string& hash1, const string& hash2) {
    int differingBits = 0;
    for (size_t i = 0; i < hash1.size(); i++) {
        unsigned char xorResult = hash1[i] ^ hash2[i];
        differingBits += __builtin_popcount(xorResult);
    }
    return differingBits;
}

int countDifferingHex(const string& hash1, const string& hash2) {
    int differingHex = 0;
    for (size_t i = 0; i < hash1.size(); i++) {
        if (hash1[i] != hash2[i]) {
            differingHex++;
        }
    }
    return differingHex;
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
    string filePath = "test_files/different_by_one_symbol.txt";

    vector<pair<string, string>> stringPairs = readStringPairs(filePath);
    if (stringPairs.empty()) {
        cerr << "No string pairs found in the file!" << endl;
        return 1;
    }

    int totalPairs = stringPairs.size();
    double totalBitPercentage = 0.0;
    double totalHexPercentage = 0.0;
    double minBitPercentage = std::numeric_limits<double>::max();
    double maxBitPercentage = 0.0;
    double minHexPercentage = std::numeric_limits<double>::max();
    double maxHexPercentage = 0.0;

    const int totalBits = 256;
    const int totalHexChars = 64;

    for (const auto& pair : stringPairs) {
        string hash1 = hashFunkcija(pair.first);
        string hash2 = hashFunkcija(pair.second);

        int differingBits = countDifferingBits(hash1, hash2);
        int differingHex = countDifferingHex(hash1, hash2);

        double bitPercentage = (differingBits / (double)totalBits) * 100.0;
        double hexPercentage = (differingHex / (double)totalHexChars) * 100.0;

        totalBitPercentage += bitPercentage;
        totalHexPercentage += hexPercentage;

        if (bitPercentage > 0) {
            minBitPercentage = min(minBitPercentage, bitPercentage);
            maxBitPercentage = max(maxBitPercentage, bitPercentage);
        }
        if (hexPercentage > 0) {
            minHexPercentage = min(minHexPercentage, hexPercentage);
            maxHexPercentage = max(maxHexPercentage, hexPercentage);
        }
    }

    double avgBitPercentage = totalBitPercentage / totalPairs;
    double avgHexPercentage = totalHexPercentage / totalPairs;

    cout << "Total string pairs processed: " << totalPairs << endl;

    cout << "Bit-Level Percentage Differences:" << endl;
    cout << "  Min: " << (minBitPercentage == std::numeric_limits<double>::max() ? 0.0 : minBitPercentage) << " %" << endl; // Handle case where min is not updated
    cout << "  Max: " << maxBitPercentage << " %" << endl;
    cout << "  Avg: " << avgBitPercentage << " %" << endl;

    cout << "Hex-Level Percentage Differences:" << endl;
    cout << "  Min: " << (minHexPercentage == std::numeric_limits<double>::max() ? 0.0 : minHexPercentage) << " %" << endl; // Handle case where min is not updated
    cout << "  Max: " << maxHexPercentage << " %" << endl;
    cout << "  Avg: " << avgHexPercentage << " %" << endl;

    return 0;
}
