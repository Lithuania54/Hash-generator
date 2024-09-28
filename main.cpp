#include <bits/stdc++.h>
#include <string>
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

int main() {
    string input;
    int choice;

    cout << "Choose input method:\n";
    cout << "1. Ranka\n";
    cout << "2. Is failo\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << "Input: ";
        getline(cin, input);
    } else if (choice == 2) {
        string filename;
        cout << "Enter filename (in test_files folder): ";
        cin >> filename;
        ifstream file("test_files/" + filename);
        if (file.is_open()) {
            stringstream buffer;
            buffer << file.rdbuf();
            input = buffer.str();
            file.close();
        } else {
            cerr << "Error: Could not open file." << endl;
            return 1;
        }
    } else {
        cerr << "Invalid choice." << endl;
        return 1;
    }

    cout << "Hash: " << hashFunkcija(input) << endl;

    return 0;
}