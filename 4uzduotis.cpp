#include <bits/stdc++.h>
#include <fstream>
#include <random>

using namespace std;

struct Transaction {
    string transaction_id;
    string sender;
    string receiver;
    int amount;
};

struct Block {
    string previous_block_hash;
    vector<Transaction> transactions;
    string block_hash;
    int nonce;

    Block(string previousHash, vector<Transaction> trans) 
        : previous_block_hash(previousHash), transactions(trans), nonce(0) {}
};

// Optimized Hex function
string Hex(unsigned long long num) {
    const string hexChars = "0123456789abcdef";
    string result(16, '0');
    for (int i = 15; i >= 0; --i) {
        result[i] = hexChars[num & 0xf];
        num >>= 4;
    }
    return result;
}

// Optimized hash function
string hashFunkcija(const string& data) {
    unsigned long long hash = 0xcbf29ce484222325; // FNV offset basis
    const unsigned long long prime = 0x100000001b3; // FNV prime
    for (char c : data) {
        hash ^= (c + 7);  // Slightly modified hash operations
        hash *= prime;
    }
    string resultHex = Hex(hash);
    
    // Expand or trim to ensure 64 characters
    while (resultHex.size() < 64) resultHex += resultHex;
    return resultHex.substr(0, 64);
}

// Function to mine a block with custom hash function
string mineBlock(Block& block, int difficulty) {
    string target(difficulty, '0');
    string baseData = block.previous_block_hash;
    for (const auto& transaction : block.transactions) {
        baseData += transaction.transaction_id + transaction.sender + transaction.receiver + to_string(transaction.amount);
    }

    while (true) {
        string data = baseData + to_string(block.nonce);
        block.block_hash = hashFunkcija(data);
        
        if (block.block_hash.substr(0, difficulty) == target) {
            cout << "Block mined! Nonce: " << block.nonce << ", Hash: " << block.block_hash << endl;
            return block.block_hash;
        }
        
        block.nonce++;
    }
}

// Function to load transactions from a file
vector<Transaction> loadTransactions(const string& filename) {
    vector<Transaction> transactions;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return transactions;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Transaction tx;
        string temp;

        getline(iss, temp, ':');
        getline(iss, tx.transaction_id, ',');

        getline(iss, temp, ':');
        getline(iss, tx.sender, ',');

        getline(iss, temp, ':');
        getline(iss, tx.receiver, ',');

        getline(iss, temp, ':');
        iss >> tx.amount;

        transactions.push_back(tx);
    }
    file.close();
    return transactions;
}

int main() {
    int difficulty = 1;
    string previous_block_hash = "0";

    vector<Transaction> transactions = loadTransactions("failai/transakcijos.txt");

    if (transactions.empty()) {
        cerr << "No transactions loaded. Exiting program." << endl;
        return 1;
    }

    cout << "Loaded " << transactions.size() << " transactions." << endl;

    Block newBlock(previous_block_hash, transactions);

    mineBlock(newBlock, difficulty);

    return 0;
}
