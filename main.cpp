#include <iostream>

#include "Charm.h"
#include "DCharm.h"

int main() {
CharmNode * node = new CharmNode(nullptr, nullptr, nullptr, 0);
    node->insertChild(new CharmNode(node, new CharmNode::item_set({1}), new CharmNode::tid_list({2, 4, 5, 6}), -1));
    node->insertChild(new CharmNode(node, new CharmNode::item_set({5}), new CharmNode::tid_list({1, 2, 3, 4, 5, 6}), -1));
    node->insertChild(new CharmNode(node, new CharmNode::item_set({2}), new CharmNode::tid_list({1, 3, 5, 6}), -1));
    node->insertChild(new CharmNode(node, new CharmNode::item_set({4}), new CharmNode::tid_list({1, 2, 3, 4, 5}), -1));
    node->insertChild(new CharmNode(node, new CharmNode::item_set({3}), new CharmNode::tid_list({1, 3, 4, 5}), -1));

Charm charm = Charm();
charm.charm(&node, 2);
charm.printClosedItemsets();

DCharmNode * dNode = new DCharmNode(nullptr, nullptr, nullptr, -1, 0);
dNode->insertChild(new DCharmNode(dNode, new DCharmNode::item_set ({1}), new DCharmNode::diff_set ({2,4,5,6}), -2, 0));
dNode->insertChild(new DCharmNode(dNode, new DCharmNode::item_set ({5}), new DCharmNode::diff_set ({1,2,3,4,5,6}), -2, 0));
dNode->insertChild(new DCharmNode(dNode, new DCharmNode::item_set ({2}), new DCharmNode::diff_set ({1,3,5,6}), -2, 0));
dNode->insertChild(new DCharmNode(dNode, new DCharmNode::item_set ({4}), new DCharmNode::diff_set ({1,2,3,4,5}), -2, 0));
dNode->insertChild(new DCharmNode(dNode, new DCharmNode::item_set ({3}), new DCharmNode::diff_set ({1,3,4,5}), -2, 0));

DCharm dcharm = DCharm();
dcharm.dcharm(&dNode, 2);
dcharm.printClosedItemsets();
return 0;
}