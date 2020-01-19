#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>

#include "utils.h"

std::istream& getLine(std::istream& is, std::string& t) {
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for (;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if (sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                if (t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}

std::vector<std::list<int>> readTransactions(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::list<int>> transactions;
    while (!getLine(file, line).eof()) {
        std::list<int> transaction;
        std::istringstream stream(line);
        std::string number;
        while (std::getline(stream, number, ','))
            transaction.push_back(std::stoi(number));
        transactions.push_back(transaction);
    }
    file.close();
    return transactions;
}

std::vector<std::string> readNames(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::string> names;
    while (!getLine(file, line).eof())
        names.push_back(line);
    file.close();
    return names;
}

std::map<std::list<int>, std::list<int>> getFrequentItemsets(const std::vector<std::list<int>>& transactions, const int& minSup, const bool& getDiffsets, const bool& findTwoSets) {
    std::map<std::list<int>, std::list<int>> result;
    int transactionNumber = 0;
    for (auto& transaction : transactions) {
        for (auto& item : transaction) {
            std::list<int> itemset { item };
            if (result.find(itemset) == result.end())
                result[itemset] = std::list<int> { transactionNumber };
            else
                result[itemset].push_back(transactionNumber);
        }
        ++transactionNumber;
    }
    std::vector<std::list<int>> toDelete;
    std::map<int, std::list<int>> reducedTransactions;
    for (auto& it : result) {
        if (it.second.size() <= minSup)
            toDelete.push_back(it.first);
        else {
            if (findTwoSets) {
                for (auto &tid : it.second) {
                    if (reducedTransactions.find(tid) == reducedTransactions.end())
                        reducedTransactions[tid] = it.first;
                    else
                        reducedTransactions[tid].push_back(it.first.front());
                }
            }
            if (getDiffsets) {
                std::list<int> transactionList(transactions.size());
                std::list<int> diffSet;
                std::iota(transactionList.begin(), transactionList.end(), 0);
                std::set_difference(transactionList.begin(), transactionList.end(), it.second.begin(), it.second.end(),
                                    std::inserter(diffSet, diffSet.begin()));
                it.second = diffSet;
            }
        }
    }
    for (auto& it : toDelete)
        result.erase(it);
    if (findTwoSets) {
        std::map<std::list<int>, std::list<int>> reducedResult;
        transactionNumber = 0;
        for (auto& transaction : reducedTransactions) {
            std::list<int> items = transaction.second;
            for (auto itemOne = items.begin(); itemOne != items.end(); ++itemOne)
                for (auto itemTwo = std::next(itemOne, 1); itemTwo != items.end(); ++itemTwo) {
                    std::list<int> itemset { *itemOne, *itemTwo };
                    if (reducedResult.find(itemset) == reducedResult.end())
                        reducedResult[itemset] = std::list<int> { transactionNumber };
                    else
                        reducedResult[itemset].push_back(transactionNumber);
                }
            ++transactionNumber;
        }
        for (auto& it : reducedResult)
        {
            if (it.second.size() > minSup) {
                if (getDiffsets) {
                    std::list<int> tidX = result[std::list<int>{it.first.front()}];
                    std::list<int> tidY = result[std::list<int>{it.first.back()}];
                    std::list<int> diffSet;
                    std::set_difference(tidY.begin(), tidY.end(), tidX.begin(), tidX.end(),
                                        std::inserter(diffSet, diffSet.begin()));
                    result[it.first] = diffSet;
                }
                else
                    result[it.first] = it.second;
            }
        }
    }
    return result;
}

void separateFrequentItemsets(const std::map<std::list<int>, std::list<int>>& frequentItemsets, std::map<std::list<int>, std::list<int>>& frequentOneItemsets, std::map<std::list<int>, std::list<int>>& frequentTwoItemsets) {
    for (auto& itemSet : frequentItemsets) {
        if (itemSet.first.size() == 2)
            frequentTwoItemsets.insert(itemSet);
        else if (itemSet.first.size() == 1)
            frequentOneItemsets.insert(itemSet);
    }
}

void displayStats(Stats& stats) {
    std::cout << "Frequent itemsets mining time (ms): " << stats.frequentMiningTime << std::endl;
    std::cout << "Algorithm time (ms): " << stats.algorithmTime << std::endl;
    std::cout << "Number of closed itemsets: " << stats.closedItemsetsCount << std::endl;
    for (int i = 0; i < 4; ++i)
        std::cout << "Property " << i + 1 << " calls: " << stats.propertyCalls[i] << std::endl;
}

bool parseArgs(const std::vector<std::string>& args, Parameter& params) {
    for (std::vector<std::string>::size_type i = 0; i < args.size(); ++i) {
        if (args[i] == "-h" || args[i] == "-help") {
            displayHelp(false);
            return false;
        }
        else if (args[i] == "-d" || args[i] == "-dcharm")
            params.dCharm = true;
        else if (args[i] == "-dps" || args[i] == "-disablePrintSets")
            params.printSets = false;
        else if (args[i] == "-ms" || args[i] == "-minSup") {
            if (++i == args.size()) {
                displayHelp(true);
                return false;
            }
            params.minSup = std::stof(args[i]);
        }
        else if (args[i] == "-p" || args[i] == "-path") {
            if (++i == args.size()) {
                displayHelp(true);
                return false;
            }
            params.path = args[i];
        }
        else if (args[i] == "-pn" || args[i] == "-printNames")
            params.printNames = true;
        else if (args[i] == "-ps" || args[i] == "-printStats")
            params.stats = true;
        else if (args[i] == "-s" || args[i] == "-sort") {
            if (++i == args.size()) {
                displayHelp(true);
                return false;
            }
            if (args[i] == "asc")
                params.itemsetSort = 1;
            else if (args[i] == "desc")
                params.itemsetSort = -1;
            else if (args[i] == "lex")
                params.itemsetSort = 0;
            else {
                displayHelp(true);
                return false;
            }
        }
        else if (args[i] == "-uts" || args[i] == "-useTwoSets")
            params.twoSetsCheck = true;
        else {
            displayHelp(true);
            return false;
        }
    }
    return true;
}

void displayHelp(const bool& wrongArg) {
    if (wrongArg)
        std::cout << "Unrecognized argument provided!" << std::endl;
    std::cout << "Available command line arguments:" << std::endl;
    std::cout << "-h,-help - print help" << std::endl;
    std::cout << "-d,-dcharm - use dCharm instead of Charm." << std::endl;
    std::cout << "-dps,-disablePrintSets - disables printing of closed itemsets." << std::endl;
    std::cout << "-ms,-minSup <support> - minimum relative support value for frequent itemsets. Defaults value is 0.1." << std::endl;
    std::cout << "-p <path>,-path <path> - path to dataset (.data and .names files) without extensions. Available names: example, mushroom, nursery, car."<< std::endl;
    std::cout << "Files must have the same first part name, i.e. mushroom.data and mushroom.names. Default is data/processed/example (example small dataset)." << std::endl;
    std::cout << "-pn,-printNames - print closed itemsets items' names instead of their integer mappings." << std::endl;
    std::cout << "-ps,-printStats - print statistics." << std::endl;
    std::cout << "-s <type>,-sort <type> - type of ordering used in algorithms, possible types: asc (ascending), desc (descending), lex (lexographical). Default is lex." << std::endl;
    std::cout << "-uts, -useTwoSets - use frequent two itemsets for checking in algorithms." << std::endl;
}
