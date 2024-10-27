#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    string transaction_id;
    string sender;
    string receiver;
    int amount;
};

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
    vector<Transaction> transactions = loadTransactions("failai/transakcijos.txt");

    if (transactions.empty()) {
        cerr << "No transactions loaded. Check the file format and path." << endl;
        return 1;
    }

    for (size_t i = 0; i < min(transactions.size(), size_t(5)); i++) {
        cout << "Transaction ID: " << transactions[i].transaction_id
                  << ", Sender: " << transactions[i].sender
                  << ", Receiver: " << transactions[i].receiver
                  << ", Amount: " << transactions[i].amount << std::endl;
    }

    return 0;
}
