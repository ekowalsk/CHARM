#include <algorithm>
#include <iostream>
#include <numeric>

#include "DCharmNode.h"

DCharmNode::DCharmNode(DCharmNode * parent, item_set * itemSet, diff_set * diffSet, int support, int sortMode) {
    this->itemSet = itemSet;
    this->diffSet = diffSet;
    this->parent = parent;
    this->support = support == -1 && parent != nullptr ? calculateSupport(parent) : support;

    hashValue = calculateHash();
    this->sortMode = sortMode;
    std::function<bool(const std::list<int>&, const std::list<int>&)> sortFun =
            [sortMode](const std::list<int>& one, const std::list<int>& two) {
                if (sortMode > 0)
                    return one.front() < two.front();
                else if (sortMode < 0)
                    return one.front() > two.front();
                else {
                    for (auto it1 = one.begin(), it2 = two.begin(); ;++it1, ++it2) {
                        if (it1 == one.end())
                            return true;
                        else if (it2 == two.end())
                            return false;
                        else if (*it1 != *it2)
                            return *it1 < *it2;
                    }
                }
            };
    children = new std::multimap<std::list<int>, DCharmNode*, decltype(sortFun)>(sortFun);
}

int DCharmNode::calculateSupport(DCharmNode * parentNode){
    return parentNode->support - static_cast<int>(diffSet->size());
}

int DCharmNode::calculateHash(){
    if (parent == nullptr) {
        std::list<int> sum(support);
        std::iota(sum.begin(), sum.end(), 0);
        return std::accumulate(sum.begin(), sum.end(), 0);
    }
    int returnValue = parent->getHash() - getHashOfDiffset();
    return returnValue;
}

int DCharmNode::getHashOfDiffset(){
    if (diffSet == nullptr)
        return 0;
    return std::accumulate(diffSet->begin(), diffSet->end(), 0);
}

DCharmNode::item_set * DCharmNode::copyItemSet(const item_set * source){
    auto itemSetCopy = new item_set();
    std::copy(source->begin(), source->end(), std::inserter(*itemSetCopy, itemSetCopy->begin()));
    return itemSetCopy;
}

DCharmNode::item_set * DCharmNode::unionItemSet(item_set * itemSet1, item_set * itemSet2){
    auto * itemSetUnion = new item_set();
    std::set_union(itemSet1->begin(), itemSet1->end(), itemSet2->begin(), itemSet2->end(),
            std::inserter(*itemSetUnion, itemSetUnion->begin()));
    return itemSetUnion;
}

DCharmNode::diff_set * DCharmNode::copyDiffSet(diff_set * diffSet){
    auto diffSetCopy = new diff_set();
    std::copy(diffSet->begin(), diffSet->end(), std::inserter(*diffSetCopy, diffSetCopy->begin()));
    return diffSetCopy;
}

DCharmNode::diff_set * DCharmNode::differenceDiffSet(diff_set * diffSet1, diff_set * diffSet2){
    auto returnedDiffSet = new diff_set();
    std::set_difference(diffSet1->begin(), diffSet1->end(), diffSet2->begin(), diffSet2->end(),
            std::inserter(*returnedDiffSet, returnedDiffSet->begin()));
    return returnedDiffSet;
}


DCharmNode::item_set * DCharmNode::unionItemSet(const DCharmNode * node1, const DCharmNode * node2){
    return unionItemSet(node1->itemSet, node2->itemSet);
}

DCharmNode::diff_set * DCharmNode::getDiffSet(const DCharmNode * node1, const DCharmNode * node2){
    return differenceDiffSet(node1->diffSet, node2->diffSet);
}

DCharmNode::childIterator DCharmNode::getChildrenBegin(){
    return children->begin();
}

DCharmNode::childIterator DCharmNode::getChildrenEnd(){
    return  children->end();
}

DCharmNode::item_set * DCharmNode::getItemSet(){
    return itemSet;
}

int DCharmNode::getSortMode() {
    return sortMode;
}

void DCharmNode::setItemSet(item_set * itSet){
    delete this->itemSet;
    this->itemSet = itSet;
}

void DCharmNode::removeChild(childIterator childIt){
    children->erase(childIt);
}

void DCharmNode::removeChildren(){
    children->clear();
}

void DCharmNode::updateItemSet(item_set * updateItemSet) {
    setItemSet(unionItemSet(this->itemSet, updateItemSet));
    for (auto & child : *children)
        child.second->updateItemSet(updateItemSet);
}

int DCharmNode::getHash(){
    return hashValue;
}

void DCharmNode::insertChild(DCharmNode * child){
    if (sortMode != 0)
        children->insert({{child->getSupport()}, child});
    else
        children->insert({*child->itemSet, child});
}


int DCharmNode::getSupport(){
    return support;
}

bool DCharmNode::isItemSetContained(item_set * contains, item_set * contained){
    return std::includes(contains->begin(), contains->end(), contained->begin(), contained->end());
}

bool DCharmNode::equalsDiffSet(DCharmNode *node){
    return std::equal(diffSet->begin(), diffSet->end(), node->diffSet->begin(), node->diffSet->end());
}

bool DCharmNode::containsDiffSet(DCharmNode *node){
    return std::includes(diffSet->begin(), diffSet->end(), node->diffSet->begin(), node->diffSet->end());
}

bool DCharmNode::hasChildren(){
    return !children->empty();
}

void DCharmNode::printItemSet(item_set * itemSet){
    for (auto &item : *itemSet)
        std::cout << item << " ";
    std::cout << std::endl;
}

DCharmNode::~DCharmNode(){
    delete itemSet;
    delete diffSet;
    for (auto &child : *children)
        delete child.second;
    delete children;
}
