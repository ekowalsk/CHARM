#ifndef CHARM_UTILS_H
#define CHARM_UTILS_H

#include <fstream>
#include <set>
#include <vector>

std::istream& getLine(std::istream& is, std::string& t);

std::vector<std::set<int>> readTransactions(const std::string& filepath);

std::vector<std::string> readNames(const std::string& filepath);

bool parseArgs(const std::vector<std::string>& args, std::string& dataset, int& startSort, bool& example, bool& dCharm, bool& measure);

void displayHelp(const bool& wrongArg);

#endif //CHARM_UTILS_H
