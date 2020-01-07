#ifndef PROJEKT_NODE_H
#define PROJEKT_NODE_H

#include <list>
#include <map>

#include "Item.h"

class Node {
private:
    typedef std::list<Item> item_set;

    item_set * items;
    Node * parent;
    std::map<int, Node> * children; // map of pairs <order_number, Node>

    static item_set * copyItemSet(const item_set& source);
    static item_set * intersectItemSets(const item_set& itemSet1, const item_set& itemSet2);

public:
    Node(Node * parent, const item_set& itemSet);
    ~Node();
};

#endif //PROJEKT_NODE_H
