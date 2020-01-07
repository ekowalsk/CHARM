#include <utility>

#include "Node.h"
//TODO delete tree

Node::Node(Node * parent, const item_set& itemSet){
    this->items = copyItemSet(itemSet);
    this->parent = parent;
    children = new std::map<int, Node>();
}

Node::~Node(){
    delete items;
    delete children;
}

Node::item_set * Node::copyItemSet(const item_set& source){
    auto itemSetCopy = new item_set();
    for (auto item : source)
        itemSetCopy->push_back(item);
    return itemSetCopy;
}

void Node::unionItemSet(item_set * mergeDestination, item_set * mergeSource){
    mergeSource->sort();
    mergeDestination->sort();
    auto destIterator = mergeDestination->begin();
    for (auto &sourceItem : *mergeSource){
        while (destIterator != mergeDestination->end() && *destIterator < sourceItem)
            destIterator++;
        if (*destIterator == sourceItem)
            continue;
        else
            mergeDestination->insert(destIterator, sourceItem);
    }
}