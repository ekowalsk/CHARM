#ifndef PROJEKT_CHARM_H
#define PROJEKT_CHARM_H

#include <unordered_map>


#include "CharmNode.h"

class Charm {
private:
    CharmNode * root;
    std::unordered_map<int, std::list<std::pair<CharmNode::item_set *, int>>> closedItemsets; //unordered_multimap<hashValue,list<closedSet, support>>

    void charmExtend(CharmNode * rootNode, int minSupport);
    static void charmProperty(CharmNode * rootNode, CharmNode::item_set * X, CharmNode::tid_list * Y, CharmNode::childIterator * nodeI, CharmNode::childIterator * nodeJ);

    void insertClosedSet(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList);
    bool isSubsumed(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList);
public:
    Charm();
    ~Charm();
};

#endif //PROJEKT_CHARM_H
