#include <algorithm>
#include <iostream>
#include <numeric>

#include "CharmNode.h"

CharmNode::CharmNode(CharmNode* parent, item_set* itemSet, tid_list* tidList, int sortMode) {
    this->itemSet = itemSet;
    this->tidList = tidList;
    this->parent = parent;

    this->sortMode = sortMode;
    std::function<bool(const std::list<int>&, const std::list<int>&)> sortFun =
            [sortMode](const std::list<int>& one, const std::list<int>& two) {
        if (sortMode > 0)
            return one.front() < two.front();
        else if (sortMode < 0)
            return one.front() > two.front();
        else
            return one < two;
    };
    children = new std::multimap<std::list<int>, CharmNode*, decltype(sortFun)>(sortFun);
}

CharmNode::item_set* CharmNode::copyItemSet(const item_set* source) {
    auto itemSetCopy = new item_set();
    std::copy(source->begin(), source->end(), std::inserter(*itemSetCopy, itemSetCopy->begin()));
    return itemSetCopy;
}

CharmNode::item_set* CharmNode::unionItemSet(item_set* itemSet1, item_set* itemSet2) {
    auto* itemSetUnion = new item_set();
    std::set_union(itemSet1->begin(), itemSet1->end(), itemSet2->begin(), itemSet2->end(),
                   std::inserter(*itemSetUnion, itemSetUnion->begin()));
    return itemSetUnion;
}

CharmNode::item_set* CharmNode::unionItemSet(const CharmNode* node1, const CharmNode* node2) {
    return unionItemSet(node1->itemSet, node2->itemSet);
}

CharmNode::tid_list* CharmNode::copyTidList(tid_list* tidList) {
    auto tidListCopy = new tid_list();
    std::copy(tidList->begin(), tidList->end(), std::inserter(*tidListCopy, tidListCopy->begin()));
    return tidListCopy;
}

CharmNode::tid_list* CharmNode::intersectTidList(tid_list* tidList1, tid_list* tidList2) {
    auto* intersectedTidList = new tid_list();
    std::set_intersection(tidList1->begin(), tidList1->end(), tidList2->begin(), tidList2->end(),
                                 std::inserter(*intersectedTidList, intersectedTidList->begin()));
    return intersectedTidList;
}

CharmNode::tid_list* CharmNode::intersectedTidList(const CharmNode* node1, const CharmNode* node2) {
    return intersectTidList(node1->tidList, node2->tidList);
}

CharmNode::child_iterator CharmNode::getChildrenBegin() {
    return children->begin();
}
CharmNode::child_iterator CharmNode::getChildrenEnd() {
    return  children->end();
}

CharmNode::item_set* CharmNode::getItemSet() {
    return itemSet;
}

CharmNode::tid_list* CharmNode::getTidList() {
    return tidList;
}

int CharmNode::getSortMode() {
    return sortMode;
}

void CharmNode::setItemSet(item_set* itSet) {
    delete this->itemSet;
    this->itemSet = itSet;
}

void CharmNode::insertChild(CharmNode* child, int weight) {
    if (sortMode != 0)
        children->insert({{weight == -1 ? child->getSupport() : weight}, child});
    else
        children->insert({*child->itemSet, child});
}

void CharmNode::removeChild(child_iterator childIt) {
    children->erase(childIt);
}

void CharmNode::updateItemSet(item_set* updateItemSet) {
    setItemSet(unionItemSet(this->itemSet, updateItemSet));
    for (auto & child : *children)
        child.second->updateItemSet(updateItemSet);
}

int CharmNode::getHash(tid_list* tidList) {
    return std::accumulate(tidList->begin(), tidList->end(), 0);
}

int CharmNode::getSupport() {
    return tidList->size();
}

bool CharmNode::isItemSetContained(item_set* contains, item_set* contained) {
    return std::includes(contains->begin(), contains->end(), contained->begin(), contained->end());
}

bool CharmNode::equalsTidList(CharmNode *node) {
    return std::equal(tidList->begin(), tidList->end(), node->tidList->begin(), node->tidList->end());
}

bool CharmNode::containsTidList(CharmNode *node) {
    return std::includes(tidList->begin(), tidList->end(), node->tidList->begin(), node->tidList->end());
}

bool CharmNode::hasChildren() {
    return !children->empty();
}

void CharmNode::printItemSet(item_set* itemSet, std::vector<std::string>& namesVector) {
    for (auto &item : *itemSet)
        std::cout << (!namesVector.empty() ? namesVector[item] : std::to_string(item)) << " ";
}

CharmNode::~CharmNode() {
    delete itemSet;
    delete tidList;
    for (auto &child : *children)
        delete child.second;
    delete children;
}