#include <bits/stdc++.h>
#include <fstream>
#include <random>

using namespace std;

string Hex(unsigned long long num)
{
    const string hexChars = "0123456789abcdef";
    string result;
    for (int i = 0; i < 16; ++i, num >>= 4)
        result.insert(result.begin(), hexChars[num & 0xf]);
    return result;
}

string hashFunction(const string &data)
{
    unsigned long long hash = 0xcbf29ce484222325;
    const unsigned long long prime = 0x100000001b3;
    for (char c : data)
        hash = (hash ^ (c + 7)) * prime;
    string resultHex = Hex(hash);
    return resultHex.size() < 64 ? resultHex + resultHex.substr(0, 64 - resultHex.size()) : resultHex;
}

struct User
{
    string publicKey;
    unsigned long long balance;
};

struct Transaction
{
    string transactionID, sender, receiver;
    unsigned int amount;
};

unordered_map<string, User> loadUsers(const string &filename)
{
    unordered_map<string, User> users;
    ifstream file(filename);
    if (!file)
        return users;

    string line, temp;
    while (getline(file, line))
    {
        istringstream iss(line);
        User user;
        getline(iss, temp, ':');
        getline(iss, temp, ':');
        iss >> user.publicKey;
        getline(iss, temp, ':');
        iss >> user.balance;
        users[user.publicKey] = user;
    }
    return users;
}

vector<Transaction> loadTransactions(const string &filename)
{
    vector<Transaction> transactions;
    ifstream file(filename);
    if (!file)
        return transactions;

    string line, temp;
    while (getline(file, line))
    {
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

vector<Transaction> selectRandomTransactions(vector<Transaction> &transactions, int num = 100)
{
    vector<Transaction> selected;
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < num && !transactions.empty(); i++)
    {
        int index = rand() % transactions.size();
        selected.push_back(transactions[index]);
        transactions.erase(transactions.begin() + index);
    }
    return selected;
}

struct Block
{
    string prevBlockHash, timestamp, version = "v0.1", merkleRootHash, nonce = "0", difficultyTarget = "0";
    vector<Transaction> transactions;
};

string getCurrentTimestamp()
{
    return to_string(time(nullptr));
}

string calculateMerkleRootHash(const vector<Transaction> &transactions)
{
    if (transactions.empty()) return "";

    vector<string> merkleLayer;
    for (const auto &tx : transactions)
        merkleLayer.push_back(hashFunction(tx.transactionID));

    while (merkleLayer.size() > 1)
    {
        if (merkleLayer.size() % 2 != 0)
            merkleLayer.push_back(merkleLayer.back());

        vector<string> nextLayer;
        for (size_t i = 0; i < merkleLayer.size(); i += 2)
        {
            string combinedHash = hashFunction(merkleLayer[i] + merkleLayer[i + 1]);
            nextLayer.push_back(combinedHash);
        }
        merkleLayer = nextLayer;
    }
    return merkleLayer[0];
}

Block createBlock(const string &prevBlockHash, const vector<Transaction> &transactions)
{
    Block newBlock{prevBlockHash, getCurrentTimestamp()};
    newBlock.merkleRootHash = calculateMerkleRootHash(transactions);
    newBlock.transactions = transactions;
    return newBlock;
}

string mineBlock(Block &block)
{
    unsigned long long nonce = 0;
    string hash;
    do
    {
        block.nonce = to_string(nonce++);
        hash = hashFunction(block.prevBlockHash + block.timestamp + block.version +
                            block.merkleRootHash + block.nonce + block.difficultyTarget);
    } while (hash.substr(0, block.difficultyTarget.length()) != block.difficultyTarget);
    return hash;
}

void applyTransactions(const vector<Transaction> &transactions, unordered_map<string, User> &users)
{
    for (const auto &tx : transactions)
    {
        if (hashFunction(tx.sender + tx.receiver + to_string(tx.amount)) != tx.transactionID)
        {
            cerr << "Invalid transaction ID hash for transaction: " << tx.transactionID << endl;
            continue;
        }

        // Balance check
        if (users[tx.sender].balance >= tx.amount)
        {
            users[tx.sender].balance -= tx.amount;
            users[tx.receiver].balance += tx.amount;
        }
        else
        {
            cerr << "Insufficient balance for transaction: " << tx.transactionID << endl;
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

vector<Block> createBlockchain(const vector<Transaction> &transactions, unordered_map<string, User> &users)
{
    vector<Block> blockchain;
    string prevBlockHash = "0000000000000000000";
    vector<Transaction> remainingTransactions = transactions;
    int blockCount = 0;

    while (!remainingTransactions.empty())
    {
        auto selectedTransactions = selectRandomTransactions(remainingTransactions);
        Block newBlock = createBlock(prevBlockHash, selectedTransactions);
        string blockHash = mineBlock(newBlock);
        prevBlockHash = blockHash;

        applyTransactions(selectedTransactions, users);
        blockchain.push_back(newBlock);
        saveBlockDetails(newBlock, blockHash, ++blockCount);
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