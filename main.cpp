#include <chrono>
#include <cmath>

#include "Charm.h"
#include "DCharm.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    Parameter params {
        .path = "data/processed/example",
        .minSup = 0.1f,
        .itemsetSort = 0,
        .dCharm = false,
        .stats = false,
        .printSets = true,
        .printNames = false,
        .twoSetsCheck = false,
    };

    Stats stats;
    if (!parseArgs(std::vector<std::string>(argv + 1, argv + argc), params))
        return 1;

    std::vector<std::list<int>> transactions = readTransactions(params.path + ".data");
    std::vector<std::string> names = params.printNames ? readNames(params.path + ".names") : std::vector<std::string>();
    int rootSupport = transactions.size();
    bool findTwoSets = params.itemsetSort || params.twoSetsCheck;
    auto frequentMiningStart = std::chrono::steady_clock::now();
    int minSup = static_cast<int>(std::floor(params.minSup * static_cast<float>(rootSupport)));
    std::map<std::list<int>, std::list<int>> freqSets = getFrequentItemsets(transactions, minSup, params.dCharm, findTwoSets);
    std::map<std::list<int>, std::list<int>> frequentOneItemSets, frequentTwoItemSets;
    separateFrequentItemsets(freqSets, frequentOneItemSets, frequentTwoItemSets);
    stats.frequentMiningTime = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - frequentMiningStart).count();
    auto algorithmStart = std::chrono::steady_clock::now();
    if (!params.dCharm) {
        Charm charm;
        charm.setFrequentTwoItemsets(frequentTwoItemSets);
        auto root = new CharmNode(nullptr, nullptr, nullptr, params.itemsetSort);
        for (auto& itemTid : frequentOneItemSets) {
            int weight = -1;
            if (params.itemsetSort) {
                weight = 0;
                for (auto& twoSets : frequentTwoItemSets) {
                    if (twoSets.first.front() == itemTid.first.front() || twoSets.first.back() == itemTid.first.front())
                        weight += twoSets.second.size();
                }
            }
            root->insertChild(new CharmNode(root, new CharmNode::item_set(itemTid.first), new CharmNode::tid_list(itemTid.second), params.itemsetSort), weight);
        }
        auto closedItemsets = charm.charm(&root, minSup, &(stats.propertyCalls), params.twoSetsCheck);
        delete root;
        stats.algorithmTime = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - algorithmStart).count();
        if (params.printSets)
            charm.printClosedItemsets(names, rootSupport);
        stats.closedItemsetsCount = closedItemsets.size();
    }
    else {
        DCharm dCharm;
        dCharm.setFrequentTwoItemsets(frequentTwoItemSets);
        auto root = new DCharmNode(nullptr, nullptr, nullptr, rootSupport, params.itemsetSort);
        for (auto& itemTid : frequentOneItemSets) {
            int weight = -1;
            if (params.itemsetSort) {
                weight = 0;
                for (auto& twoSets : frequentTwoItemSets) {
                    if (twoSets.first.front() == itemTid.first.front() || twoSets.first.back() == itemTid.first.front())
                        weight += rootSupport - itemTid.second.size() - twoSets.second.size();
                }
            }
            root->insertChild(new DCharmNode(root, new DCharmNode::item_set(itemTid.first), new DCharmNode::diff_set(itemTid.second), -1, params.itemsetSort), weight);
        }
        auto closedItemsets = dCharm.dcharm(&root, minSup, &(stats.propertyCalls), params.twoSetsCheck);
        delete root;
        stats.algorithmTime = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - algorithmStart).count();
        if (params.printSets)
            dCharm.printClosedItemsets(names, rootSupport);
        stats.closedItemsetsCount = closedItemsets.size();
    }
    if (params.stats)
        displayStats(stats);
    return 0;
}
