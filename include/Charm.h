#ifndef PROJEKT_CHARM_H
#define PROJEKT_CHARM_H

#include <unordered_map>


#include "CharmNode.h"

class Charm {
public:
    typedef std::unordered_map<int, std::list<std::pair<CharmNode::item_set *, int>>> closed_itemsets_map;
private:
    closed_itemsets_map closedItemsets; //unordered_multimap<hashValue,list<closedSet, support>>
    std::map<std::list<int>, std::list<int>> frequentTwoItemsets;

    void charmExtend(CharmNode ** rootNode, int minSupport,  std::array<unsigned int, 4> * propertyStats, bool checkTwoItemsets);
    static void charmProperty(CharmNode ** rootNode, CharmNode::item_set * X, CharmNode::tid_list * Y, CharmNode::childIterator * nodeI, CharmNode::childIterator * nodeJ, std::array<unsigned int, 4> * propertyStats);

    void insertClosedSet(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList);
    bool isSubsumed(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList);
    bool isFrequentItemset(CharmNode::item_set * twoItemSet);
public:
    Charm();
    ~Charm();
    closed_itemsets_map charm(CharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats, bool checkTwoItemsets);
    void printClosedItemsets(const std::vector<std::string>& namesVector);
    void setFrequentTwoItemsets(std::map<std::list<int>, std::list<int>> frequentTwoItemsets);
};

#endif //PROJEKT_CHARM_H
