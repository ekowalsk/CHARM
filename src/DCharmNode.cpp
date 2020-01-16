#include <iostream>
#include "DCharmNode.h"

DCharmNode::DCharmNode(DCharmNode * parent, item_set * itemSet, diff_set * diffSet, int support) {
    if (itemSet != nullptr) {
        this->itemSet = copyItemSet(itemSet);
        this->itemSet->sort();
    }
    else
        this->itemSet = nullptr;

    if (diffSet != nullptr) {
        this->diffSet = copyDiffSet(diffSet);
        this->diffSet->sort();
    }
    else
        this->diffSet = nullptr;

    this->parent = parent;

    if (support == -1 && parent != nullptr)
        this->support = calculateSupport(parent);
    else if (support == -2)
        this->support = diffSet->size();
    else if (parent == nullptr)
        this->support = 0;

    hashValue = calculateHash();
    children = new std::multimap<int, DCharmNode *>();
}

int DCharmNode::calculateSupport(DCharmNode * parentNode){
    return parentNode->support - diffSet->size();
}

int DCharmNode::calculateHash(){
    if (parent == nullptr)
        return 0;
    int returnValue = parent->getHashOfDiffset();
    for (auto &tid : *diffSet)
        returnValue -= tid;
    return returnValue;
}

int DCharmNode::getHashOfDiffset(){
    if (diffSet == nullptr)
        return 0;
    int hash = 0;
    for (auto &tid : *diffSet)
        hash += tid;
    return hash;
}

DCharmNode::item_set * DCharmNode::copyItemSet(const item_set * source){
    auto itemSetCopy = new item_set();
    for (auto &item : *source)
        itemSetCopy->push_back(item);
    return itemSetCopy;
}

DCharmNode::item_set * DCharmNode::unionItemSet(item_set * itemSet1, item_set * itemSet2){
    auto * itemSetUnion = copyItemSet(itemSet1);
    auto itemIterator = itemSetUnion->begin();
    item_set::iterator savedIterator;
    for (auto &item : *itemSet2){
        while (itemIterator != itemSetUnion->end() && *itemIterator < item){
            savedIterator = itemIterator;
            itemIterator++;
        }
        if (itemIterator == itemSetUnion->end()){
            if (*savedIterator != item)
                itemSetUnion->insert(itemIterator, item);
        }
        else if (*itemIterator != item)
            itemSetUnion->insert(itemIterator, item);
    }
    return itemSetUnion;
}

DCharmNode::diff_set * DCharmNode::copyDiffSet(diff_set * diffSet){
    auto diffSetCopy = new diff_set();
    for (auto &tid : *diffSet)
        diffSetCopy->push_back(tid);
    return diffSetCopy;
}

DCharmNode::diff_set * DCharmNode::differenceDiffSet(diff_set * diffSet1, diff_set * diffSet2){
    auto returnedDiffSet = new diff_set();
    auto diffSetIterator1 = diffSet1->begin();
    diff_set::iterator savedIterator;
    for (auto &tid : *diffSet2){
        while (diffSetIterator1 != diffSet1->end() && *diffSetIterator1 < tid) {
            savedIterator = diffSetIterator1;
            diffSetIterator1++;
        }
        if (diffSetIterator1 == diffSet1->end()) {
            if (*savedIterator != tid)
                returnedDiffSet->push_back(tid);
        }
        else if (*diffSetIterator1 != tid)
            returnedDiffSet->push_back(tid);
    }
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

void DCharmNode::setItemSet(item_set * itSet){
    delete this->itemSet;
    this->itemSet = copyItemSet(itSet);
    this->itemSet->sort();
}

void DCharmNode::removeChild(childIterator childIt){
    children->erase(childIt);
}

void DCharmNode::removeChildren(){
    children->clear();
}

void DCharmNode::updateItemSet(item_set * updateItemSet){
    setItemSet(unionItemSet(this->itemSet, updateItemSet));
    for (auto & child : *children)
        child.second->updateItemSet(updateItemSet);
}

int DCharmNode::getHash(){
    return hashValue;
}

void DCharmNode::insertChild(DCharmNode * child){
    int childSupport = child->getSupport();
    children->insert({childSupport, child});
}


int DCharmNode::getSupport(){
    return support;
}

bool DCharmNode::isItemSetContained(item_set * contains, item_set * contained){
    for (auto containsIt = contains->begin(), containedIt = contained->begin(); containedIt != contained->end(); containedIt++){
        while (containsIt != contains->end() && *containsIt < *containedIt)
            containsIt++;
        if (containsIt == contains->end() || *containsIt != *containedIt)
            return false;
    }
    return true;
}

bool DCharmNode::equalsDiffSet(DCharmNode *node){
    if (diffSet->size() != node->diffSet->size())
        return false;
    for (auto tidIt1 = diffSet->begin(), tidIt2 = node->diffSet->begin(); tidIt1 != diffSet->end(); tidIt1++, tidIt2++){
        if (*tidIt1 != *tidIt2)
            return false;
    }
    return true;
}

bool DCharmNode::containsDiffSet(DCharmNode *node){
    for (auto tidIt1 = node->diffSet->begin(), tidIt2 = diffSet->begin(); tidIt1 != node->diffSet->end(); tidIt1++){
        while (tidIt2 != diffSet->end() && *tidIt2 < *tidIt1)
            tidIt2++;
        if (tidIt2 == diffSet->end() || *tidIt1 != *tidIt2)
            return false;
    }
    return true;
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
    delete parent;
    for (auto &child : *children)
        delete child.second;
    delete children;
}
