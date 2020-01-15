#include "utils.h"
#include <sstream>

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

std::vector<std::set<int>> readTransactions(const std::string& filepath)
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
            transaction.insert(std::stoi(number));
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

bool parseArgs(const std::vector<std::string>& args, std::string& dataset, int& startSort, bool& example, bool& dCharm, bool& measure)
{
    for (std::vector<std::string>::size_type i = 0; i < args.size(); ++i)
    {
        if (args[i] == "--help")
        {
            displayHelp(false);
            return false;
        }
        else if (args[i] == "-d")
            dCharm = true;
        else if (args[i] == "-e")
            example = true;
        else if (args[i] == "-m")
            measure = true;
        else if (args[i] == "-n")
        {
            if (++i == args.size())
            {
                displayHelp(true);
                return false;
            }
            dataset = args[i];
        }
        else if (args[i] == "-s")
        {
            if (++i == args.size())
            {
                displayHelp(true);
                return false;
            }
            if (args[i] == "asc")
                startSort = 1;
            else if (args[i] == "desc")
                startSort = -1;
            else if (args[i] == "lex")
                startSort = 0;
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

}
