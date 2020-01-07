#ifndef PROJEKT_NODE_H
#define PROJEKT_NODE_H

#include <list>
#include <map>

class Node {
private:
    typedef std::list<int> item_set;

    item_set * items;
    Node * parent;
    std::map<int, Node> * children; // map of pairs <order_number, Node>

    static item_set * copyItemSet(const item_set& source);
    static void unionItemSet(item_set * mergeDestination, item_set * mergeSource);

public:
    Node(Node * parent, const item_set& itemSet);
    ~Node();
};

#endif //PROJEKT_NODE_H
