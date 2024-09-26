#include <bits/stdc++.h>
#include <string>
using namespace std;

unsigned int funkcija(const string& input) {
    unsigned int hash = 0;
    unsigned int prime = 31;

    for (char c : input) {
        hash = (hash * prime) + (c * (c + 7));
        hash += 12345;
    }

    return hash;
}

int main() {
    string input;
    cout << "Stringas: ";
    getline(cin, input);

    unsigned int reiksme = funkcija(input);
    cout << "Hash: " << reiksme << endl;

    return 0;
}