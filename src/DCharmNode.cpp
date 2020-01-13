#include <iostream>
#include "DCharmNode.h"

DCharmNode::DCharmNode(DCharmNode * parent, item_set * itemSet, diff_set * diffSet, int hash, int support) {
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
    else if (parent == nullptr)
        this->support = 0;
    else
        this->support = support;

    this->hashValue = hash;

    children = new std::multimap<int, DCharmNode *>();
}

int DCharmNode::calculateSupport(DCharmNode * parentNode){
    return parentNode->support - diffSet->size();
}

int DCharmNode::calculateHash(DCharmNode * nodeX, DCharmNode * nodeY){
    diff_set * XYdiffSet = differenceDiffSet(nodeX->diffSet, nodeY->diffSet);
    int returnValue = nodeX->getHashOfDiffset();
    for (auto &tid : * XYdiffSet)
        returnValue -= tid;
    return returnValue;
}

int DCharmNode::getHashOfDiffset(){
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

DCharmNode::diff_set * DCharmNode::copyDiffSet(diff_set * diffSet){
    auto diffSetCopy = new diff_set();
    for (auto &tid : *diffSet)
        diffSetCopy->push_back(tid);
    return diffSetCopy;
}

DCharmNode::diff_set * DCharmNode::differenceDiffSet(diff_set * diffSet1, diff_set * diffSet2){
    diffSet1->sort();
    diffSet2->sort();
    auto returnedDiffSet = new diff_set();
    auto diffSetIterator1 = diffSet1->begin();
    for (auto &tid : *diffSet2){
        while (diffSetIterator1 != diffSet1->end() && *diffSetIterator1  < tid)
            diffSetIterator1++;
        if (*diffSetIterator1 == tid)
            continue;
        else
            returnedDiffSet->push_back(tid);
    }
    return returnedDiffSet;
}


DCharmNode::item_set * DCharmNode::unionItemSet(const DCharmNode * node1, const DCharmNode * node2){
    return unionItemSet(node1->itemSet, node2->itemSet);
}

DCharmNode::diff_set * DCharmNode::getDiffSet(const DCharmNode * node1, const DCharmNode * node2){
    return differenceDiffSet(node1->itemSet, node2->itemSet);
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

DCharmNode::diff_set * DCharmNode::getDiffSet(){
    return diffSet;
}

void DCharmNode::setItemSet(item_set * itSet){
    delete this->itemSet;
    this->itemSet = copyItemSet(itSet);
    this->itemSet->sort();
}

void DCharmNode::setSupport (int support){
    this->support = support;
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
        if (*containsIt != *containedIt)
            return false;
    }
    return true;
}

bool DCharmNode::hasChildren(){
    return !children->empty();
}

void DCharmNode::printItemSet(item_set * itemSet){
    for (auto &item : *itemSet)
        std::cout << item;
    std::cout << std::endl;
}

DCharmNode::~DCharmNode(){
    if (itemSet != nullptr)
        delete itemSet;
    if (diffSet != nullptr)
        delete diffSet;
    if (parent != nullptr)
        delete parent;
    delete children;
}
