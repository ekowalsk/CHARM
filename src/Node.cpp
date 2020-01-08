#include "Node.h"

Node::Node(Node * parent, const item_set& itemSet){
    this->items = copyItemSet(itemSet);
    this->parent = parent;
    children = new std::map<int, Node>();
}

Node::~Node(){
    delete parent;
    delete items;
    delete children;
}

Node::item_set * Node::copyItemSet(const item_set& source){
    auto itemSetCopy = new item_set();
    for (auto item : source)
        itemSetCopy->push_back(item);
    return itemSetCopy;
}

void Node::unionItemSet(item_set * mergeSource){
    mergeSource->sort();
    items->sort();
    auto destIterator = items->begin();
    for (auto &sourceItem : *mergeSource){
        while (destIterator != items->end() && *destIterator < sourceItem)
            destIterator++;
        if (*destIterator == sourceItem)
            continue;
        else
            items->insert(destIterator, sourceItem);
    }
}