#include <iostream>

#include "Charm.h"

int main() {
CharmNode * node = new CharmNode(nullptr, nullptr, nullptr);
node->insertChild(new CharmNode(node, new CharmNode::item_set ({1}), new CharmNode::tid_list ({2,4,5,6})));
node->insertChild(new CharmNode(node, new CharmNode::item_set ({2}), new CharmNode::tid_list ({1,3,5,6})));
node->insertChild(new CharmNode(node, new CharmNode::item_set ({3}), new CharmNode::tid_list ({1,3,4,5})));
node->insertChild(new CharmNode(node, new CharmNode::item_set ({4}), new CharmNode::tid_list ({1,2,3,4,5})));
node->insertChild(new CharmNode(node, new CharmNode::item_set ({5}), new CharmNode::tid_list ({1,2,3,4,5,6})));

Charm charm = Charm();
charm.charm(&node, 3);
charm.printClosedItemsets();
return 0;
}