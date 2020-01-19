#ifndef DCHARM_H
#define DCHARM_H

#include <unordered_map>

#include "DCharmNode.h"

class DCharm{
public:
    typedef std::unordered_map<int, std::list<std::pair<DCharmNode::item_set*, int>>> closed_itemsets_map;
private:
    closed_itemsets_map closedItemsets; //unordered_multimap<hashValue,list<closedSet, support>>
    std::map<std::list<int>, std::list<int>> frequentTwoItemsets;

    void charmExtend(DCharmNode** rootNode, int minSupport, std::array<unsigned int, 4>* propertyStats, int checkTwoItemsets);
    static void charmProperty(DCharmNode** rootNode, DCharmNode::item_set* X, DCharmNode::diff_set* Y,
                              DCharmNode::child_iterator* nodeI, DCharmNode::child_iterator* nodeJ, std::array<unsigned int, 4>* propertyStats);

    void insertClosedSet(DCharmNode* node);
    bool isSubsumed(DCharmNode* node);
    bool isFrequentItemset(DCharmNode::item_set* twoItemSet);
public:
    DCharm();
    ~DCharm();
    closed_itemsets_map dcharm(DCharmNode** rootNode, int minSupport, std::array<unsigned int, 4>* propertyStats, int checkTwoItemsets);
    void setFrequentTwoItemsets(std::map<std::list<int>, std::list<int>>& frequentTwoItemsets);
    void printClosedItemsets(std::vector<std::string>& names, int rootSupport);
};

#endif
