#include "CharmNode.h"

CharmNode::CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList){
    this->itemSet = copyItemSet(itemSet);
    this->tidList = copyTidList(tidList);
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
    auto * itemSetUnion = new item_set();
    auto itemIterator = itemSet1->begin();
    for (auto &item : *itemSet2){
        while (itemIterator != itemSet1->end() && *itemIterator < item)
            itemIterator++;
        if (*itemIterator == item)
            continue;
        else
            itemSetUnion->insert(itemIterator, item);
    }
    return itemSetUnion;
}

CharmNode::item_set * CharmNode::unionItemSet(CharmNode * node1, CharmNode * node2){
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

CharmNode::tid_list * CharmNode::intersectedTidList(const CharmNode& node1, const CharmNode& node2){
    return intersectTidList(node1.tidList, node2.tidList);
}

void CharmNode::setItemSet(item_set * itSet){
    delete this->itemSet;
    this->itemSet = copyItemSet(itSet);
}

void CharmNode::setTidList(tid_list * tList){
    delete this->tidList;
    this->tidList = copyTidList(tList);
}

void CharmNode::insertChild(CharmNode * child){
    int childSupport = child->getSupport();
    children->insert({childSupport, child});
}

void CharmNode::removeChild(childIterator childIt){
    children->erase(childIt);
}

int CharmNode::getHash(){
    int tidSum = 0;
    for (auto &tid : *tidList)
        tidSum += tid;
    return tidSum;
}

int CharmNode::getSupport(){
    int support = 0;
    for (auto &tid : *tidList)
        support += 1;
    return support;
}

CharmNode::~CharmNode() {
    delete itemSet;
    delete tidList;
    delete parent;
    delete children;
}