#ifndef PROJEKT_DCHARM_H
#define PROJEKT_DCHARM_H

#include <unordered_map>
#include "DCharmNode.h"

class DCharm{
public:
    typedef std::unordered_map<int, std::list<std::pair<DCharmNode::item_set *, int>>> closedItemsetsmap;
private:
    closedItemsetsmap closedItemsets; //unordered_multimap<hashValue,list<closedSet, support>>

    void charmExtend(DCharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats);
    static void charmProperty(DCharmNode ** rootNode, DCharmNode::item_set * X, DCharmNode::diff_set * Y, DCharmNode::childIterator * nodeI, DCharmNode::childIterator * nodeJ, std::array<unsigned int, 4> * propertyStats);

    void insertClosedSet(DCharmNode * node);
    bool isSubsumed(DCharmNode * node);
public:
    DCharm();
    ~DCharm();
    closedItemsetsmap dcharm(DCharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats);
    void printClosedItemsets(std::vector<std::string>);
};

#endif //PROJEKT_DCHARM_H
