#include "BaseCharm.h"

BaseCharm::BaseCharm(Node *root) {
    this->root = root;
    closedItemsets = std::unordered_map<int, std::list<Node::item_set>>();
}