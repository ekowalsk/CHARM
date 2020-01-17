#include <iostream>

#include "DCharm.h"

DCharm::DCharm(){
    closedItemsets = std::unordered_map<int, std::list<std::pair<DCharmNode::item_set *, int>>>();
}

DCharm::closedItemsetsmap DCharm::dcharm(DCharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats, int checkTwoItemsets){
    charmExtend(rootNode, minSupport, propertyStats, checkTwoItemsets);
    return closedItemsets;
}

void DCharm::charmExtend(DCharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats, int checkTwoItemsets){
    for (auto childIterator = (*rootNode)->getChildrenBegin(); childIterator != (*rootNode)->getChildrenEnd();) {
        auto rightHandChildren = childIterator;
        for (auto rNeighbourIterator = ++rightHandChildren; rNeighbourIterator != (*rootNode)->getChildrenEnd();){
            auto savedIterator= rNeighbourIterator;
            ++savedIterator;
            if (checkTwoItemsets){
                if (childIterator->second->getItemSet()->size() == 1 && rNeighbourIterator->second->getItemSet()->size() == 1) {
                    if (!isFrequentItemset(DCharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second))) {
                        rNeighbourIterator = savedIterator;
                        continue;
                    }
                }
            }
            DCharmNode::diff_set * diffSet = DCharmNode::getDiffSet(rNeighbourIterator->second, childIterator->second);
            if (childIterator->second->getSupport() - diffSet->size() > minSupport) {
                DCharmNode::item_set * itemSetUnion = DCharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second);
                charmProperty(rootNode, itemSetUnion, diffSet, &childIterator, &rNeighbourIterator, propertyStats);
            }
            rNeighbourIterator = savedIterator;
        }
        if(childIterator->second->hasChildren())
            charmExtend(&(childIterator->second), minSupport, propertyStats, checkTwoItemsets);

        auto toCheck = childIterator++;
        if(!isSubsumed(toCheck->second))
            insertClosedSet(toCheck->second);
        else
            delete toCheck->second;
        (*rootNode)->removeChild(toCheck);
    }
}

void DCharm::charmProperty(DCharmNode ** rootNode, DCharmNode::item_set * X, DCharmNode::diff_set * Y, DCharmNode::childIterator * nodeIiterator, DCharmNode::childIterator * nodeJiterator, std::array<unsigned int, 4> * propertyStats){
    DCharmNode * nodeI = (*nodeIiterator)->second;
    DCharmNode * nodeJ = (*nodeJiterator)->second;
    if (nodeI->equalsDiffSet(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        nodeI->updateItemSet(X);
        (*propertyStats)[0] += 1;
        delete X;
        delete Y;
    }
    else if (nodeI->containsDiffSet(nodeJ)) {
        nodeI->updateItemSet(X);
        (*propertyStats)[1] += 1;
        delete X;
        delete Y;
    }
    else if (nodeJ->containsDiffSet(nodeI)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->insertChild(new DCharmNode(nodeI, X, Y, -1, nodeI->getSortMode()));
        (*propertyStats)[2] += 1;
    }
    else {
        (*nodeIiterator)->second->insertChild(new DCharmNode(nodeI, X, Y, -1, nodeI->getSortMode()));
        (*propertyStats)[3] += 1;
    }
}

void DCharm::insertClosedSet(DCharmNode * node){
    try {
        closedItemsets.at(node->getHash()).push_back({node->getItemSet(), node->getSupport()});
    } catch (std::out_of_range& e){
        closedItemsets.insert(std::make_pair(node->getHash(), std::list<std::pair<DCharmNode::item_set *, int>>()));
        closedItemsets.at(node->getHash()).push_back({node->getItemSet(), node->getSupport()});
    }
}

bool DCharm::isSubsumed(DCharmNode * node){
    try {
        for (auto & iterator : closedItemsets.at(node->getHash())){
            if (iterator.second == node->getSupport()){
                if(DCharmNode::isItemSetContained(iterator.first, node->getItemSet())){
                    return true;
                }
            }
        }
        return false;
    } catch (std::out_of_range& e){
        return false;
    }
}

void DCharm::printClosedItemsets(std::vector<std::string> namesVector){
    for (const auto& element : closedItemsets){
        for (auto closedItemSet : element.second){
            DCharmNode::printItemSet(closedItemSet.first, namesVector);
            std::cout << "   support: " << closedItemSet.second << std::endl;
        }
    }
}

bool DCharm::isFrequentItemset(DCharmNode::item_set * twoItemSet){
    return !(frequentTwoItemsets.find(*twoItemSet) == frequentTwoItemsets.end());
}

void DCharm::setFrequentTwoItemsets(std::map<std::list<int>, std::list<int>> freqTwoItemsets){
    this->frequentTwoItemsets = std::move(freqTwoItemsets);
}

DCharm::~DCharm(){
    for (const auto& closedItemsetsMapElement : closedItemsets){
        for (auto closedItemset: closedItemsetsMapElement.second)
           delete closedItemset.first;
    }
}
