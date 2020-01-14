#include <iostream>

#include "CharmNode.h"

CharmNode::CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList) {
    if (itemSet != nullptr) {
        this->itemSet = copyItemSet(itemSet);
        this->itemSet->sort();
    }
    else
        this->itemSet = nullptr;
    if (tidList != nullptr) {
        this->tidList = copyTidList(tidList);
        this->tidList->sort();
    }
    else
        this->tidList = nullptr;
    this->parent = parent;
    children = new std::multimap<int, CharmNode *>();
}

CharmNode::item_set * CharmNode::copyItemSet(const item_set * source){
    auto itemSetCopy = new item_set();
    for (auto &item : *source)
        itemSetCopy->push_back(item);
    return itemSetCopy;
}

CharmNode::item_set * CharmNode::unionItemSet(item_set * itemSet1, item_set * itemSet2){
    itemSet1->sort();
    itemSet2->sort();
    auto * itemSetUnion = copyItemSet(itemSet1);
    auto itemIterator = itemSet1->begin();
    for (auto &item : *itemSet2){
        while (itemIterator != itemSet1->end() && *itemIterator < item)
            itemIterator++;
        if (*itemIterator == item)
            continue;
        else
            itemSetUnion->push_back(item);
    }
    return itemSetUnion;
}

CharmNode::item_set * CharmNode::unionItemSet(const CharmNode * node1, const CharmNode * node2){
    return unionItemSet(node1->itemSet, node2->itemSet);
}

CharmNode::tid_list * CharmNode::copyTidList(tid_list * tidList){
    auto tidListCopy = new tid_list();
    for (auto &tid : *tidList)
        tidListCopy->push_back(tid);
    return tidListCopy;
}

CharmNode::tid_list * CharmNode::intersectTidList(tid_list * tidList1, tid_list * tidList2){
    tidList1->sort();
    tidList2->sort();
    auto intersectionList = new tid_list();
    auto tidIterator1 = tidList1->begin();
    for (auto &sourceTid : *tidList2){
        while (tidIterator1 != tidList1->end() && *tidIterator1 < sourceTid)
            tidIterator1++;
        if (*tidIterator1 == sourceTid)
            intersectionList->push_back(sourceTid);
    }
    return intersectionList;
}

CharmNode::tid_list * CharmNode::intersectedTidList(const CharmNode * node1, const CharmNode * node2){
    return intersectTidList(node1->tidList, node2->tidList);
}

CharmNode::childIterator CharmNode::getChildrenBegin(){
    return children->begin();
}
CharmNode::childIterator CharmNode::getChildrenEnd(){
    return  children->end();
}

CharmNode::item_set * CharmNode::getItemSet(){
    return itemSet;
}

CharmNode::tid_list * CharmNode::getTidList(){
    return tidList;
}

void CharmNode::setItemSet(item_set * itSet){
    delete this->itemSet;
    this->itemSet = copyItemSet(itSet);
    this->itemSet->sort();
}

void CharmNode::setTidList(tid_list * tList){
    delete this->tidList;
    this->tidList = copyTidList(tList);
    this->tidList->sort();
}

void CharmNode::insertChild(CharmNode * child){
    int childSupport = child->getSupport();
    children->insert({childSupport, child});
}

void CharmNode::removeChild(childIterator childIt){
    children->erase(childIt);
}

void CharmNode::removeChildren(){
    children->clear();
}

void CharmNode::updateItemSet(item_set * updateItemSet){
    setItemSet(unionItemSet(this->itemSet, updateItemSet));
    for (auto & child : *children)
        child.second->updateItemSet(updateItemSet);
}

int CharmNode::getHash(tid_list * tidList){
    int tidSum = 0;
    for (auto &tid : *tidList)
        tidSum += tid;
    return tidSum;
}

int CharmNode::getSupport(){
    return tidList->size();
}

bool CharmNode::isItemSetContained(item_set * contains, item_set * contained){
    for (auto containsIt = contains->begin(), containedIt = contained->begin(); containedIt != contained->end(); containedIt++){
        while (containsIt != contains->end() && *containsIt < *containedIt)
            containsIt++;
        if (*containsIt != *containedIt)
            return false;
    }
    return true;
}

bool CharmNode::equalsTidList(CharmNode *node){
    if (tidList->size() != node->tidList->size())
        return false;
    for (auto tidIt1 = tidList->begin(), tidIt2 = node->tidList->begin(); tidIt1 != tidList->end(); tidIt1++, tidIt2++){
        if (*tidIt1 != *tidIt2)
            return false;
    }
    return true;
}

bool CharmNode::containsTidList(CharmNode *node){
    for (auto tidIt1 = node->tidList->begin(), tidIt2 = tidList->begin(); tidIt1 != node->tidList->end(); tidIt1++){
        while (tidIt2 != tidList->end() && *tidIt2 < *tidIt1)
            tidIt2++;
        if (*tidIt1 != *tidIt2)
            return false;
    }
    return true;
}

bool CharmNode::hasChildren(){
    return !children->empty();
}

void CharmNode::printItemSet(item_set * itemSet){
    for (auto &item : *itemSet)
        std::cout << item;
    std::cout << std::endl;
}

CharmNode::~CharmNode() {
    if (itemSet != nullptr)
        delete itemSet;
    if (tidList != nullptr)
        delete tidList;
    if (parent != nullptr)
        delete parent;
    if (children != nullptr)
        delete children;
}