#include "utils.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    auto a = readTransactions(R"(..\data\processed\mushroom.data)");
    auto b = readNames(R"(..\data\processed\mushroom.names)");
    auto x = getFrequentItemsets(a, 0, true);
    for (const auto& i : x)
    {
        std::cout << "{ ";
        for (auto j : i.first)
            std::cout << j << " ";
        std::cout << "}";
        std::cout << ": " << i.second.size() << std::endl;
    }
    return 0;
}