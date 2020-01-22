#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <list>
#include <map>
#include <set>
#include <vector>

typedef struct Parameter {
    std::string path;
    float minSup;
    int itemsetSort;
    bool dCharm;
    bool stats;
    bool printSets;
    bool printNames;
    bool twoSetsCheck;
} Parameter;

typedef struct Stats {
    long long frequentMiningTime;
    long long algorithmTreeTime;
    long long algorithmTime;
    long long closedItemsetsCount;
    std::array<unsigned int, 4> propertyCalls{};
    Stats() {
        frequentMiningTime = 0;
        algorithmTreeTime = 0;
        algorithmTime = 0;
        closedItemsetsCount = 0;
        propertyCalls = std::array<unsigned int, 4> {0, 0, 0, 0};
    }
} Stats;

std::istream& getLine(std::istream& is, std::string& t);

std::vector<std::list<int>> readTransactions(const std::string& filepath);

std::vector<std::string> readNames(const std::string& filepath);

void getFrequentItemsets(const std::vector<std::list<int>>& transactions, std::map<std::list<int>, std::list<int>>& oneFrequentItemsets, std::set<std::list<int>>& twoFrequentItemsets, const int minSup, const bool getDiffsets, const bool findTwoSets);

void displayStats(Stats& stats);

bool parseArgs(const std::vector<std::string>& args, Parameter& params);

void displayHelp(const bool& wrongArg);

#endif
