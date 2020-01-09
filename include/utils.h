#ifndef CHARM_UTILS_H
#define CHARM_UTILS_H

#include <fstream>
#include <set>
#include <vector>

std::istream& getLine(std::istream &is, std::string &t);

std::vector<std::set<int>> readTransactions(std::string& filepath);

std::vector<std::string> readNames(std::string& filepath);

#endif //CHARM_UTILS_H
