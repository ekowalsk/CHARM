#include "utils.h"
#include <sstream>

std::istream& getLine(std::istream &is, std::string &t)
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

std::vector<std::set<int>> readTransactions(std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::set<int>> transactions;
    while (!getLine(file, line).eof())
    {
        std::set<int> transaction;
        std::istringstream stream(line);
        std::string number;
        while (std::getline(stream, number, ','))
            transaction.emplace(std::stoi(number));
        transactions.push_back(transaction);
    }
    file.close();
    return transactions;
}

std::vector<std::string> readNames(std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::string> names;
    while (!getLine(file, line).eof())
        names.emplace_back(line);
    file.close();
    return names;
}
