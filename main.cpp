#include <chrono>

#include "Charm.h"
#include "DCharm.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    Parameter params {
        .path = "/home/ewa/Dokumenty/EDAMI/projekt/data/processed/example",
        .minSup = 2,
        .startSort = 1,
        .dCharm = false,
        .stats = false
    };
    Stats stats;
    if (!parseArgs(std::vector<std::string>(argv + 1, argv + argc), params))
        return 1;

    bool findTwoSets = true;
    std::vector<std::list<int>> transactions = readTransactions(params.path + ".data");
    std::vector<std::string> names = readNames(params.path + ".names");
    auto frequentMiningStart = std::chrono::steady_clock::now();
    std::map<std::list<int>, std::list<int>> freqSets = getFrequentItemsets(transactions, params.minSup, params.dCharm, findTwoSets);
    auto frequentOneItemSets = std::map<std::list<int>, std::list<int>>(), frequentTwoItemSets = std::map<std::list<int>, std::list<int>>();
    separateFrequentItemsets(freqSets, &frequentOneItemSets, &frequentTwoItemSets);
    stats.frequentMiningTime = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::steady_clock::now() - frequentMiningStart).count();
    auto algorithmStart = std::chrono::steady_clock::now();
    if (!params.dCharm) {
        Charm charm;
        charm.setFrequentTwoItemsets(frequentTwoItemSets);
        auto root = new CharmNode(nullptr, nullptr, nullptr, params.startSort);
        for (auto& itemTid : frequentOneItemSets)
            root->insertChild(new CharmNode(root, new CharmNode::item_set(itemTid.first), new CharmNode::tid_list(itemTid.second), params.startSort));
        charm.charm(&root, params.minSup, &(stats.propertyCalls), findTwoSets);
        delete root;
        stats.algorithmTime = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - algorithmStart).count();
        charm.printClosedItemsets(names);
    }
    else {
        DCharm dCharm;
        dCharm.setFrequentTwoItemsets(frequentTwoItemSets);
        auto root = new DCharmNode(nullptr, nullptr, nullptr, transactions.size(), params.startSort);
        for (auto& itemTid : frequentOneItemSets)
            root->insertChild(new DCharmNode(root, new DCharmNode::item_set(itemTid.first), new DCharmNode::diff_set(itemTid.second), -1, params.startSort));
        dCharm.dcharm(&root, params.minSup, &(stats.propertyCalls), findTwoSets);
        delete root;
        stats.algorithmTime = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - algorithmStart).count();
        dCharm.printClosedItemsets(names);
    }
    if (params.stats)
        displayStats(stats);
    return 0;
}