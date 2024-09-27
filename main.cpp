#include <bits/stdc++.h>
#include <string>
using namespace std;

string Hex(unsigned long long num) {
    string galimiHex = "0123456789abcdef";
    string result = "";

    while (num > 0) {
        int liekana = num & 15;
        result = galimiHex[liekana] + result;
        num = num >> 4;
    }

    return result;
}

string funkcija(const string& input) {
    unsigned long long hash = 0;
    unsigned long long prime = 31;

    for (char c : input) {
        hash = (hash * prime) + (c * (c + 7));
        hash += 12345;
    }

    string rezultHex = Hex(hash);

    while (rezultHex.length() < 64) {
        rezultHex = "0" + rezultHex;
    }

    return rezultHex;
}

int main() {
    string input;
    cout << "Stringas: ";
    getline(cin, input);

    string reiksme = funkcija(input);
    cout << "Hash (64 symboliu hex): " << reiksme << endl;

    return 0;
}
