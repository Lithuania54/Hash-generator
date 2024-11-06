#include <bits/stdc++.h>
#include <fstream>
#include <random>

using namespace std;

string Hex(unsigned long long num) {
    const string hexChars = "0123456789abcdef";
    string result;
    for (int i = 0; i < 16; ++i, num >>= 4)
        result.insert(result.begin(), hexChars[num & 0xf]);
    return result;
}

string hashFunction(const string &data) {
    unsigned long long hash = 0xcbf29ce484222325;
    const unsigned long long prime = 0x100000001b3;
    for (char c : data)
        hash = (hash ^ (c + 7)) * prime;
    string resultHex = Hex(hash);
    return resultHex.size() < 64 ? resultHex + resultHex.substr(0, 64 - resultHex.size()) : resultHex;
}

struct UTXO {
    string transactionID;
    unsigned int outputIndex;
    unsigned int amount;

    bool operator==(const UTXO &other) const {
        return transactionID == other.transactionID && outputIndex == other.outputIndex && amount == other.amount;
    }
};

struct User {
    string publicKey;
    vector<UTXO> utxos;
};

struct Transaction {
    string transactionID;
    string sender;
    string receiver;
    unsigned int amount;
    vector<UTXO> inputs;
    vector<pair<string, unsigned int>> outputs;
};

unordered_map<string, User> loadUsers(const string &filename) {
    unordered_map<string, User> users;
    ifstream file(filename);
    if (!file)
        return users;

    string line, temp;
    while (getline(file, line)) {
        istringstream iss(line);
        User user;
        getline(iss, temp, ':');
        getline(iss, temp, ':');
        iss >> user.publicKey;
        users[user.publicKey] = user;
    }
    return users;
}

vector<Transaction> loadTransactions(const string &filename) {
    vector<Transaction> transactions;
    ifstream file(filename);
    if (!file)
        return transactions;

    string line, temp;
    while (getline(file, line)) {
        Transaction tx;
        istringstream iss(line);
        getline(iss, temp, ':');
        iss >> tx.transactionID;
        getline(iss, temp, ':');
        iss >> tx.sender;
        getline(iss, temp, ':');
        iss >> tx.receiver;
        getline(iss, temp, ':');
        iss >> tx.amount;

        transactions.push_back(tx);
    }
    return transactions;
}

vector<Transaction> selectRandomTransactions(vector<Transaction> &transactions, int num = 100) {
    vector<Transaction> selected;
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < num && !transactions.empty(); i++) {
        int index = rand() % transactions.size();
        selected.push_back(transactions[index]);
        transactions.erase(transactions.begin() + index);
    }
    return selected;
}

struct Block {
    string prevBlockHash, timestamp, version = "v0.1", merkleRootHash, nonce = "0", difficultyTarget = "0";
    vector<Transaction> transactions;
};

string getCurrentTimestamp() {
    return to_string(time(nullptr));
}

string calculateMerkleRootHash(const vector<Transaction> &transactions) {
    if (transactions.empty()) return "";

    vector<string> merkleLayer;
    for (const auto &tx : transactions)
        merkleLayer.push_back(hashFunction(tx.transactionID));

    while (merkleLayer.size() > 1) {
        if (merkleLayer.size() % 2 != 0)
            merkleLayer.push_back(merkleLayer.back());

        vector<string> nextLayer;
        for (size_t i = 0; i < merkleLayer.size(); i += 2) {
            string combinedHash = hashFunction(merkleLayer[i] + merkleLayer[i + 1]);
            nextLayer.push_back(combinedHash);
        }
        merkleLayer = nextLayer;
    }
    return merkleLayer[0];
}

Block createBlock(const string &prevBlockHash, const vector<Transaction> &transactions) {
    Block newBlock{prevBlockHash, getCurrentTimestamp()};
    newBlock.merkleRootHash = calculateMerkleRootHash(transactions);
    newBlock.transactions = transactions;
    return newBlock;
}

string mineBlock(Block &block) {
    unsigned long long nonce = 0;
    string hash;
    do {
        block.nonce = to_string(nonce++);
        hash = hashFunction(block.prevBlockHash + block.timestamp + block.version +
                            block.merkleRootHash + block.nonce + block.difficultyTarget);
    } while (hash.substr(0, block.difficultyTarget.length()) != block.difficultyTarget);
    return hash;
}

void applyTransactions(const vector<Transaction> &transactions, unordered_map<string, User> &users) {
    for (const auto &tx : transactions) {
        bool validTransaction = true;
        unsigned int inputSum = 0;

        for (const auto &input : tx.inputs) {
            if (find(users[tx.sender].utxos.begin(), users[tx.sender].utxos.end(), input) == users[tx.sender].utxos.end()) {
                cerr << "Invalid or double-spent UTXO in transaction: " << tx.transactionID << endl;
                validTransaction = false;
                break;
            }
            inputSum += input.amount;
        }

        if (!validTransaction) continue;

        unsigned int outputSum = 0;
        for (const auto &output : tx.outputs)
            outputSum += output.second;

        if (inputSum < outputSum) {
            cerr << "Insufficient input sum for transaction: " << tx.transactionID << endl;
            continue;
        }

        for (const auto &input : tx.inputs)
            users[tx.sender].utxos.erase(remove(users[tx.sender].utxos.begin(), users[tx.sender].utxos.end(), input), users[tx.sender].utxos.end());

        for (size_t i = 0; i < tx.outputs.size(); i++) {
            UTXO newUTXO{tx.transactionID, static_cast<unsigned int>(i), tx.outputs[i].second};
            users[tx.outputs[i].first].utxos.push_back(newUTXO);
        }
    }
}

void printTransaction(const Transaction &tx, ostream &out = cout)
{
    out << "Transaction ID: " << tx.transactionID << "\nSender: " << tx.sender
        << "\nReceiver: " << tx.receiver << "\nAmount: " << tx.amount << "\n-----------------------------\n";
}

void printBlock(const Block &block, const string &blockHash, int blockCount, ostream &out = cout)
{
    out << "Block " << blockCount << " Mined Successfully!\nBlock Hash: " << blockHash
        << "\nPrev Block Hash: " << block.prevBlockHash << "\nTimestamp: " << block.timestamp
        << "\nMerkle Root Hash: " << block.merkleRootHash << "\nNonce: " << block.nonce
        << "\nDifficulty Target: " << block.difficultyTarget
        << "\nTransactions in Block: " << block.transactions.size() << "\n\nTransactions:\n";
    for (const auto &tx : block.transactions)
        printTransaction(tx, out);
    out << "=============================\n";
}

void saveBlockDetails(const Block &block, const string &blockHash, int blockCount)
{
    ofstream file("failai/mined_blocks.txt", ios_base::app);
    if (file)
        printBlock(block, blockHash, blockCount, file);
    else
        cerr << "Unable to open file failai/mined_blocks.txt for saving.\n";
}

void printAllTransactions(const vector<Transaction> &transactions)
{
    for (const auto &tx : transactions)
    {
        printTransaction(tx);
    }
}

Block mineWithCandidates(const string &prevBlockHash, vector<Transaction> &transactions)
{
    const int numCandidates = 5;
    const int transactionsPerCandidate = 100;
    const int maxAttempts = 100000;
    const auto maxTime = chrono::seconds(5);

    vector<Block> candidates;
    
    for (int i = 0; i < numCandidates; ++i)
    {
        auto selectedTransactions = selectRandomTransactions(transactions, transactionsPerCandidate);
        candidates.push_back(createBlock(prevBlockHash, selectedTransactions));
    }

    unsigned long long attemptCount;
    while (true)
    {
        for (auto &candidate : candidates)
        {
            auto startTime = chrono::steady_clock::now();
            attemptCount = 0;

            while (chrono::steady_clock::now() - startTime < maxTime && attemptCount < maxAttempts)
            {
                candidate.nonce = to_string(attemptCount++);
                string hash = hashFunction(candidate.prevBlockHash + candidate.timestamp + candidate.version +
                                           candidate.merkleRootHash + candidate.nonce + candidate.difficultyTarget);

                if (hash.substr(0, candidate.difficultyTarget.length()) == candidate.difficultyTarget)
                {
                    cout << "Successfully mined a block!" << endl;
                    return candidate;
                }
            }
        }

        cout << "No block mined in this round. Extending time and attempts." << endl;
    }
}

vector<Block> createBlockchain(const vector<Transaction> &transactions, unordered_map<string, User> &users)
{
    vector<Block> blockchain;
    string prevBlockHash = "0000000000000000000";
    vector<Transaction> remainingTransactions = transactions;
    int blockCount = 0;

    while (!remainingTransactions.empty())
    {
        Block minedBlock = mineWithCandidates(prevBlockHash, remainingTransactions);
        string blockHash = hashFunction(minedBlock.prevBlockHash + minedBlock.timestamp + minedBlock.version +
                                        minedBlock.merkleRootHash + minedBlock.nonce + minedBlock.difficultyTarget);

        prevBlockHash = blockHash;
        applyTransactions(minedBlock.transactions, users);
        blockchain.push_back(minedBlock);
        saveBlockDetails(minedBlock, blockHash, ++blockCount);
    }
    return blockchain;
}

int main()
{
    auto users = loadUsers("failai/vartotojai.txt");
    if (users.empty())
        return cerr << "No users loaded.\n", 1;

    auto transactions = loadTransactions("failai/transakcijos.txt");
    if (transactions.empty())
        return cerr << "No transactions loaded.\n", 1;

    string input;
    cout << "Do you want to print transactions or blocks? (Enter 'transactions' or 'blocks'): ";
    cin >> input;

    if (input == "transactions")
    {
        cout << "Do you want to print 'all' transactions or a 'specific' one? ";
        cin >> input;

        if (input == "all")
        {
            printAllTransactions(transactions);
        }
        else if (input == "specific")
        {
            cout << "Enter the transaction ID: ";
            string txID;
            cin >> txID;
            bool found = false;
            for (const auto &tx : transactions)
            {
                if (tx.transactionID == txID)
                {
                    printTransaction(tx);
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Transaction with ID " << txID << " not found.\n";
        }
        else
        {
            cout << "Invalid input.\n";
        }
    }
    else if (input == "blocks")
    {
        cout << "Generating blockchain...\n";
        vector<Block> blockchain = createBlockchain(transactions, users);

        cout << "Do you want to print 'all' blocks or a 'specific' one? ";
        cin >> input;

        if (input == "all")
        {
            for (size_t i = 0; i < blockchain.size(); i++)
            {
                string blockHash = mineBlock(const_cast<Block &>(blockchain[i]));
                printBlock(blockchain[i], blockHash, i + 1);
            }
        }
        else if (input == "specific")
        {
            cout << "Enter the block index: ";
            int index;
            cin >> index;
            if (index < 0 || index >= blockchain.size())
            {
                cout << "Block with index " << index << " not found.\n";
            }
            else
            {
                string blockHash = mineBlock(const_cast<Block &>(blockchain[index]));
                printBlock(blockchain[index], blockHash, index + 1);
            }
        }
        else
        {
            cout << "Invalid input.\n";
        }
    }
    else
    {
        cout << "Invalid choice.\n";
    }
    return 0;
}