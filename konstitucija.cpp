#include <bits/stdc++.h>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;

string Hex(unsigned long long num) {
    const string hexGalimi = "0123456789abcdef";
    string result;

    if (num == 0) return "0";

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

string readLines(const string& filePath, int numLines) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return "";
    }

    string line, content;
    int lineCount = 0;
    while (getline(file, line) && lineCount < numLines) {
        content += line + "\n";
        lineCount++;
    }

    file.close();
    return content;
}

void Performance(const string& filePath) {
    vector<int> lineCounts = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    const int numExperiments = 5;

    for (int numLines : lineCounts) {
        double totalTime = 0.0;

        for (int i = 0; i < numExperiments; ++i) {
            string inputData = readLines(filePath, numLines);
            
            auto start = chrono::high_resolution_clock::now();
            string hashValue = hashFunkcija(inputData);
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double, milli> elapsedTime = end - start;
            totalTime += elapsedTime.count();
        }

        double averageTime = totalTime / numExperiments;
        cout << "Average time for " << numLines << " lines: " << averageTime << " ms" << endl;
    }
}

int main() {
    string konstitucijaPath = "test_files/konstitucija.txt";

    Performance(konstitucijaPath);

    return 0;
}
