#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <sstream>

std::string Hex(unsigned long long num) {
    const std::string hexChars = "0123456789abcdef";
    std::string result(16, '0');
    for (int i = 15; i >= 0; --i) {
        result[i] = hexChars[num & 0xf];
        num >>= 4;
    }
    return result;
}

std::string hashFunkcija(const std::string& data) {
    unsigned long long hash = 0xcbf29ce484222325;
    const unsigned long long prime = 0x100000001b3;
    for (char c : data) {
        hash ^= (c + 7);
        hash *= prime;
    }
    std::string resultHex = Hex(hash);
    while (resultHex.size() < 64) resultHex += resultHex;
    return resultHex.substr(0, 64);
}

struct User {
    std::string publicKey;
    unsigned long long balance;
};

struct Transaction {
    std::string transactionID;
    std::string sender;
    std::string receiver;
    unsigned int amount;
};

std::unordered_map<std::string, User> loadUsers(const std::string& filename) {
    std::unordered_map<std::string, User> users;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return users;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        User user;
        std::istringstream iss(line);
        
        std::string temp;
        std::getline(iss, temp, ':');
        std::getline(iss, temp, ':');
        iss >> user.publicKey;
        std::getline(iss, temp, ':');
        iss >> user.balance;

        users[user.publicKey] = user;
    }
    file.close();
    return users;
}

std::vector<Transaction> loadTransactions(const std::string& filename) {
    std::vector<Transaction> transactions;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return transactions;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        Transaction tx;
        std::istringstream iss(line);
        
        std::string temp;
        std::getline(iss, temp, ':');
        iss >> tx.transactionID;
        std::getline(iss, temp, ':');
        iss >> tx.sender;
        std::getline(iss, temp, ':');
        iss >> tx.receiver;
        std::getline(iss, temp, ':');
        iss >> tx.amount;

        transactions.push_back(tx);
    }
    file.close();
    return transactions;
}

std::vector<Transaction> selectRandomTransactions(std::vector<Transaction>& transactions, int num = 100) {
    std::vector<Transaction> selected;
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < num && !transactions.empty(); ++i) {
        int index = std::rand() % transactions.size();
        selected.push_back(transactions[index]);
        transactions.erase(transactions.begin() + index);
    }
    return selected;
}

struct Block {
    std::string prevBlockHash;
    std::string timestamp;
    std::string version;
    std::string merkleRootHash;
    std::string nonce;
    std::string difficultyTarget;
    std::vector<Transaction> transactions;
};

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    return std::to_string(now);
}

std::string calculateMerkleRootHash(const std::vector<Transaction>& transactions) {
    std::string concatenatedHashes;
    for (const auto& tx : transactions) {
        concatenatedHashes += tx.transactionID;
    }
    return hashFunkcija(concatenatedHashes);
}

Block createBlock(const std::string& prevBlockHash, const std::vector<Transaction>& transactions) {
    Block newBlock;
    newBlock.prevBlockHash = prevBlockHash;
    newBlock.timestamp = getCurrentTimestamp();
    newBlock.version = "v0.1";
    newBlock.merkleRootHash = calculateMerkleRootHash(transactions);
    newBlock.difficultyTarget = "0";
    newBlock.nonce = "0";
    newBlock.transactions = transactions;
    
    return newBlock;
}

std::string mineBlock(Block& block) {
    unsigned long long nonce = 0;
    std::string hash;
    do {
        block.nonce = std::to_string(nonce);
        std::string headerData = block.prevBlockHash + block.timestamp + block.version + block.merkleRootHash + block.nonce + block.difficultyTarget;
        hash = hashFunkcija(headerData);
        nonce++;
    } while (hash.substr(0, block.difficultyTarget.length()) != block.difficultyTarget);

    return hash;
}

void applyTransactions(const std::vector<Transaction>& transactions, std::unordered_map<std::string, User>& users) {
    for (const auto& tx : transactions) {
        if (users[tx.sender].balance >= tx.amount) {
            users[tx.sender].balance -= tx.amount;
            users[tx.receiver].balance += tx.amount;
        } else {
            std::cerr << "Insufficient balance for transaction: " << tx.transactionID << std::endl;
        }
    }
}

int main() {
    std::unordered_map<std::string, User> users = loadUsers("failai/vartotojai.txt");
    if (users.empty()) {
        std::cerr << "No users loaded." << std::endl;
        return 1;
    }

    std::vector<Transaction> transactions = loadTransactions("failai/transakcijos.txt");
    if (transactions.empty()) {
        std::cerr << "No transactions loaded." << std::endl;
        return 1;
    }

    std::string prevBlockHash = "0000000000000000000";
    while (!transactions.empty()) {
        std::vector<Transaction> selectedTransactions = selectRandomTransactions(transactions);

        Block newBlock = createBlock(prevBlockHash, selectedTransactions);

        std::string blockHash = mineBlock(newBlock);
        prevBlockHash = blockHash;

        applyTransactions(selectedTransactions, users);

        std::cout << "New Block Mined Successfully!" << std::endl;
        std::cout << "Block Hash: " << blockHash << std::endl;
        std::cout << "Prev Block Hash: " << newBlock.prevBlockHash << std::endl;
        std::cout << "Timestamp: " << newBlock.timestamp << std::endl;
        std::cout << "Merkle Root Hash: " << newBlock.merkleRootHash << std::endl;
        std::cout << "Nonce: " << newBlock.nonce << std::endl;
        std::cout << "Difficulty Target: " << newBlock.difficultyTarget << std::endl;
        std::cout << "Transactions in Block: " << newBlock.transactions.size() << std::endl;
    }

    return 0;
}
