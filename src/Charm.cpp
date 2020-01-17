#include "Charm.h"

Charm::Charm() {
    closedItemsets = std::unordered_map<int, std::list<std::pair<CharmNode::item_set *, int>>>();
}

Charm::closedItemsetsmap Charm::charm(CharmNode ** rootNode, int minSupport){
    charmExtend(rootNode, minSupport);
    return closedItemsets;
}

void Charm::charmExtend(CharmNode ** rootNode, int minSupport){
    for (auto childIterator = (*rootNode)->getChildrenBegin(); childIterator != (*rootNode)->getChildrenEnd(); childIterator++) {
        auto rightHandChildren = childIterator;
        CharmNode::item_set * itemSetUnion = childIterator->second->getItemSet();
        CharmNode::tid_list * tidListIntersection = childIterator->second->getTidList();
        for (auto rNeighbourIterator = ++rightHandChildren; rNeighbourIterator != (*rootNode)->getChildrenEnd();){
            auto savedIterator= rNeighbourIterator;
            ++savedIterator;
            tidListIntersection = CharmNode::intersectedTidList(childIterator->second, rNeighbourIterator->second);
            if (tidListIntersection->size() > minSupport) {
                itemSetUnion = CharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second);
                charmProperty(rootNode, itemSetUnion, tidListIntersection, &childIterator, &rNeighbourIterator);
            }
            rNeighbourIterator = savedIterator;
        }
        if(childIterator->second->hasChildren()) {
            charmExtend(&(childIterator->second), minSupport);
            childIterator->second->removeChildren();
        }

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
        //TODO - dodać dla opcji leksykograficznej - <itemset, Child>
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