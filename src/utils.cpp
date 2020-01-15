#include "utils.h"
#include <sstream>
#include <iostream>

std::istream& getLine(std::istream& is, std::string& t)
{
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}

std::vector<std::list<int>> readTransactions(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::list<int>> transactions;
    while (!getLine(file, line).eof())
    {
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

std::vector<std::string> readNames(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::string> names;
    while (!getLine(file, line).eof())
        names.push_back(line);
    file.close();
    return names;
}

std::map<std::list<int>, std::list<int>> getFrequentItemsets(const std::vector<std::list<int>>& transactions, const int& minSup, const bool& findTwoSets)
{
    std::map<std::list<int>, std::list<int>> result;
    int transactionNumber = 0;
    for (auto& transaction : transactions)
    {
        for (auto& item : transaction)
        {
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
    for (auto& it : result)
    {
        if (it.second.size() <= minSup)
            toDelete.push_back(it.first);
        else if (findTwoSets)
        {
            for (auto& tid : it.second)
            {
                if (reducedTransactions.find(tid) == reducedTransactions.end())
                    reducedTransactions[tid] = it.first;
                else
                    reducedTransactions[tid].push_back(it.first.front());
            }
        }
    }
    for (auto& it : toDelete)
        result.erase(it);
    if (findTwoSets)
    {
        std::map<std::list<int>, std::list<int>> reducedResult;
        transactionNumber = 0;
        for (auto& transaction : reducedTransactions)
        {
            std::list<int> items = transaction.second;
            for (auto itemOne = items.begin(); itemOne != items.end(); ++itemOne)
                for (auto itemTwo = std::next(itemOne, 1); itemTwo != items.end(); ++itemTwo)
                {
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
            if (it.second.size() > minSup)
                result[it.first] = it.second;
        }
    }
    return result;
}

bool parseArgs(const std::vector<std::string>& args, Parameter& params)
{
    for (std::vector<std::string>::size_type i = 0; i < args.size(); ++i)
    {
        if (args[i] == "-h" || args[i] == "-help")
        {
            displayHelp(false);
            return false;
        }
        else if (args[i] == "-d" || args[i] == "-dcharm")
            params.dCharm = true;
        else if (args[i] == "-e" || args[i] == "-example")
            params.example = true;
        else if (args[i] == "-m" || args[i] == "-measure")
            params.measure = true;
        else if (args[i] == "-ms" || args[i] == "-minSup")
        {
            if (++i == args.size())
            {
                displayHelp(true);
                return false;
            }
            params.minSup = stoi(args[i]);
        }
        else if (args[i] == "-p" || args[i] == "-path")
        {
            if (++i == args.size())
            {
                displayHelp(true);
                return false;
            }
            params.path = args[i];
        }
        else if (args[i] == "-s" || args[i] == "-sort")
        {
            if (++i == args.size())
            {
                displayHelp(true);
                return false;
            }
            if (args[i] == "asc")
                params.startSort = 1;
            else if (args[i] == "desc")
                params.startSort = -1;
            else if (args[i] == "lex")
                params.startSort = 0;
            else
            {
                displayHelp(true);
                return false;
            }
        }
        else
        {
            displayHelp(true);
            return false;
        }
    }
    return true;
}

void displayHelp(const bool& wrongArg)
{
    if (wrongArg)
        std::cerr << "Unrecognized argument provided!" << std::endl;
    std::cout << "Available command line arguments:" << std::endl;
    std::cout << "-h,-help - print help" << std::endl;
    std::cout << "-d,-dcharm - use dCharm instead of Charm. Defaults to false" << std::endl;
    std::cout << "-e,-example - run example small dataset on Charm (run with -d to use dCharm). Defaults to false" << std::endl;
    std::cout << "-m,-measure - print statistics (runtime, (d)Charm properties count). Defaults to false" << std::endl;
    std::cout << "-ms,-minSup <support> - minimum support value for frequent itemsets. Defaults to 10." << std::endl;
    std::cout << "-p <name>,-path <name> - path to dataset (.data and .names files) without extensions"<< std::endl;
    std::cout << "files must have the same first part name, i.e. mushroom.data and mushroom.names. Defaults to data/processed/mushroom" << std::endl;
    std::cout << "-s <type>,-sort <type> - type of ordering used in algorithms, possible types: asc (ascending), desc (descending), lex (lexographical). Defaults to lex" << std::endl;
}
