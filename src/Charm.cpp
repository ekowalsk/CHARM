#include "Charm.h"

Charm::Charm() {
    closedItemsets = std::unordered_map<int, std::list<std::pair<CharmNode::item_set *, int>>>();
}

Charm::closedItemsetsmap Charm::charm(CharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats){
    charmExtend(rootNode, minSupport, propertyStats);
    return closedItemsets;
}

void Charm::charmExtend(CharmNode ** rootNode, int minSupport, std::array<unsigned int, 4> * propertyStats){
    for (auto childIterator = (*rootNode)->getChildrenBegin(); childIterator != (*rootNode)->getChildrenEnd();) {
        auto rightHandChildren = childIterator;
        for (auto rNeighbourIterator = ++rightHandChildren; rNeighbourIterator != (*rootNode)->getChildrenEnd();){
            auto savedIterator= rNeighbourIterator;
            ++savedIterator;
            CharmNode::tid_list* tidListIntersection = CharmNode::intersectedTidList(childIterator->second, rNeighbourIterator->second);
            if (tidListIntersection->size() > minSupport) {
                CharmNode::item_set* itemSetUnion = CharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second);
                charmProperty(rootNode, itemSetUnion, tidListIntersection, &childIterator, &rNeighbourIterator, propertyStats);
            }
            rNeighbourIterator = savedIterator;
        }
        if(childIterator->second->hasChildren())
            charmExtend(&(childIterator->second), minSupport, propertyStats);

        auto toCheck = childIterator++;
        if(!isSubsumed(toCheck->second->getItemSet(), toCheck->second->getTidList()))
            insertClosedSet(toCheck->second->getItemSet(), toCheck->second->getTidList());
        else
            delete toCheck->second;
        (*rootNode)->removeChild(toCheck);
    }
}
void Charm::charmProperty(CharmNode ** rootNode, CharmNode::item_set * X, CharmNode::tid_list * Y, CharmNode::childIterator * nodeIiterator, CharmNode::childIterator * nodeJiterator, std::array<unsigned int, 4> * propertyStats){
    CharmNode * nodeI = (*nodeIiterator)->second;
    CharmNode * nodeJ = (*nodeJiterator)->second;
    if (nodeI->equalsTidList(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->updateItemSet(X);
        (*propertyStats)[0] += 1;
        delete X;
        delete Y;
    }
    else if (nodeJ->containsTidList(nodeI)) {
        (*nodeIiterator)->second->updateItemSet(X);
        (*propertyStats)[1] += 1;
        delete X;
        delete Y;
    }
    else if (nodeI->containsTidList(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->insertChild(new CharmNode((*nodeIiterator)->second, X, Y, (*nodeIiterator)->second->getSortMode()));
        (*propertyStats)[2] += 1;
    }
    else {
        (*nodeIiterator)->second->insertChild(new CharmNode((*nodeIiterator)->second, X, Y, (*nodeIiterator)->second->getSortMode()));
        (*propertyStats)[3] += 1;
    }
}

void Charm::insertClosedSet(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList){
    int hash = CharmNode::getHash(tidList);
    try {
        closedItemsets.at(hash).push_back({itemSet, tidList->size()});
    } catch (std::out_of_range& e){
        closedItemsets.insert(std::make_pair(hash, std::list<std::pair<CharmNode::item_set *, int>>()));
        closedItemsets.at(hash).push_back({itemSet, tidList->size()});
    }
}

bool Charm::isSubsumed(CharmNode::item_set * itemSet, CharmNode::tid_list * tidList){
    int hash = CharmNode::getHash(tidList);
    try {
        for (auto & iterator : closedItemsets.at(hash)){
            if (iterator.second == tidList->size()){
                if(CharmNode::isItemSetContained(iterator.first, itemSet)){
                    return true;
                }
            }
        }
        return false;

    } catch (std::out_of_range& e){
        return false;
    }
}

void Charm::printClosedItemsets(){
    for (const auto& element : closedItemsets){
        for (auto closedItemSet : element.second){
            CharmNode::printItemSet(closedItemSet.first);
        }
    }
}

Charm::~Charm(){
    for (const auto& closedItemsetsMapElement : closedItemsets){
        for (auto closedItemset: closedItemsetsMapElement.second)
            delete closedItemset.first;
    }
}