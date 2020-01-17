#ifndef CHARM_UTILS_H
#define CHARM_UTILS_H

#include <fstream>
#include <list>
#include <map>
#include <vector>

typedef struct Parameter {
    std::string path;
    int minSup;
    int startSort;
    bool dCharm;
    bool stats;
} Parameter;

typedef struct Stats {
    long long frequentMiningTime;
    long long algorithmTime;
    std::array<unsigned int, 4> propertyCalls;
} Stats;

std::istream& getLine(std::istream& is, std::string& t);

std::vector<std::list<int>> readTransactions(const std::string& filepath);

std::vector<std::string> readNames(const std::string& filepath);

std::map<std::list<int>, std::list<int>> getFrequentItemsets(const std::vector<std::list<int>>& transactions, const int& minSup, const bool& findTwoSets);

void displayStats(Stats& stats);

bool parseArgs(const std::vector<std::string>& args, Parameter& params);

void displayHelp(const bool& wrongArg);

#endif //CHARM_UTILS_H
