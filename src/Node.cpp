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

std::list<Item> * Node::intersectItemSets(const std::list<Item>& itemSet1, const std::list<Item>& itemSet2){
    std::list<Item> * itemSetIntersection = new  std::list<Item>();
}