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
};

string Hex(unsigned long long num) {
    const string hexGalimi = "0123456789abcdef";
    string result;
    do {
        result = hexGalimi[num & 15] + result;
        num >>= 4;
    } while (num > 0);
    return result;
}

string hashFunkcija(const string& data) {
    unsigned long long hash = 0;
    const unsigned long long prime = 31;
    for (char c : data) {
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

vector<Transaction> selectRandomTransactions(const vector<Transaction>& transactions, int count) {
    vector<Transaction> temp_transactions = transactions;
    shuffle(temp_transactions.begin(), temp_transactions.end(), std::mt19937(std::random_device()()));
    return vector<Transaction>(temp_transactions.begin(), temp_transactions.begin() + min(count, static_cast<int>(temp_transactions.size())));
}

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
    return transactions;
}

Block createBlock(const vector<Transaction>& transactions, const string& previous_block_hash) {
    Block block;
    block.previous_block_hash = previous_block_hash;
    block.transactions = selectRandomTransactions(transactions, 100);

    string combined_data = previous_block_hash;
    for (const auto& tx : block.transactions) {
        combined_data += tx.transaction_id;
    }
    block.block_hash = hashFunkcija(combined_data);
    return block;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<Transaction> transactions = loadTransactions("failai/transakcijos.txt");

    if (transactions.size() < 100) {
        cerr << "Not enough transactions to create a block." << endl;
        return 1;
    }

    string previous_block_hash = "00000000000000000000000000000000";
    Block new_block = createBlock(transactions, previous_block_hash);

    cout << "New Block Created:\n";
    cout << "Previous Block Hash: " << new_block.previous_block_hash << "\n";
    cout << "Block Hash: " << new_block.block_hash << "\n";
    cout << "Transactions:\n";
    for (const auto& tx : new_block.transactions) {
        cout << "    Transaction ID: " << tx.transaction_id
             << ", Sender: " << tx.sender
             << ", Receiver: " << tx.receiver
             << ", Amount: " << tx.amount << "\n";
    }

    return 0;
}
