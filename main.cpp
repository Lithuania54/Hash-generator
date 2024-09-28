#include <bits/stdc++.h>
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

string readFileContent(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

int main() {
    string folderPath = "test_files";

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        string filePath = entry.path().string();

        cout << "Processing file: " << filePath << endl;

        string fileContent = readFileContent(filePath);

        string hashValue = hashFunkcija(fileContent);

        cout << "Hash: " << hashValue << endl;
    }

    return 0;
}
