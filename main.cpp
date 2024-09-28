#include <bits/stdc++.h>
#include <string>
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
    cout << "Input: ";
    getline(cin, input);

    cout << "Hash: " << hashFunkcija(input) << endl;

    return 0;
}