#ifndef PROJEKT_BASECHARM_H
#define PROJEKT_BASECHARM_H

#include <unordered_map>

#include "Node.h"

class BaseCharm {
private:
    Node * root;
    std::unordered_map<int, std::list<Node::item_set>> closedItemsets;
public:
    BaseCharm(Node * root);
};

#endif //PROJEKT_BASECHARM_H
