#include "Charm.h"
//TODO destruktor - usuwanie itemset
Charm::Charm() {
    this->root = new CharmNode(nullptr, nullptr, nullptr);
    closedItemsets = std::unordered_map<int, std::list<std::pair<CharmNode::item_set *, int>>>();
}

void Charm::charmExtend(CharmNode * rootNode, int minSupport){
    for (auto childIterator = rootNode->getChildrenBegin(); childIterator != rootNode->getChildrenEnd(); childIterator++) {
        auto rightHandChildren = childIterator;
        CharmNode::item_set * itemSetUnion = nullptr;
        CharmNode::tid_list * tidListIntersection = nullptr;
        for (auto rNeighbourIterator = ++rightHandChildren; rNeighbourIterator != rootNode->getChildrenEnd(); rNeighbourIterator++){
            itemSetUnion = CharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second);
            if (itemSetUnion->size() >= minSupport){
                tidListIntersection = CharmNode::intersectedTidList(childIterator->second, rNeighbourIterator->second);
                charmProperty(&rootNode, itemSetUnion, tidListIntersection, &childIterator, &rNeighbourIterator);
            }
        }
        if(childIterator->second->hasChildren())
            charmExtend(childIterator->second, minSupport);
        if(!isSubsumed(itemSetUnion, tidListIntersection))
            insertClosedSet(itemSetUnion, tidListIntersection);
    }
}
void Charm::charmProperty(CharmNode ** rootNode, CharmNode::item_set * X, CharmNode::tid_list * Y, CharmNode::childIterator * nodeIiterator, CharmNode::childIterator * nodeJiterator){
    CharmNode * nodeI = (*nodeIiterator)->second;
    CharmNode * nodeJ = (*nodeJiterator)->second;
    if (nodeI->equalsTidList(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->updateItemSet(X);
    }
    else if (nodeJ->containsTidList(nodeI))
        (*nodeIiterator)->second->updateItemSet(X);
    else if (nodeI->containsTidList(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->insertChild(new CharmNode((*nodeIiterator)->second, X, Y));
    }
    else
        (*nodeIiterator)->second->insertChild(new CharmNode((*nodeIiterator)->second, X, Y));
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

Charm::~Charm(){
    delete root;
}