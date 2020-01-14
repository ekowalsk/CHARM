#include "DCharm.h"

DCharm::DCharm(){
    closedItemsets = std::unordered_map<int, std::list<std::pair<DCharmNode::item_set *, int>>>();
}

DCharm::closedItemsetsmap DCharm::dcharm(DCharmNode ** rootNode, int minSupport){
    charmExtend(rootNode, minSupport);
    return closedItemsets;
}

void DCharm::charmExtend(DCharmNode ** rootNode, int minSupport){
    for (auto childIterator = (*rootNode)->getChildrenBegin(); childIterator != (*rootNode)->getChildrenEnd(); childIterator++) {
        DCharmNode * node = childIterator->second;
        auto rightHandChildren = childIterator;
        for (auto rNeighbourIterator = ++rightHandChildren; rNeighbourIterator != (*rootNode)->getChildrenEnd();){
            auto savedIterator= rNeighbourIterator;
            ++savedIterator;
            DCharmNode::item_set * itemSetUnion = DCharmNode::unionItemSet(childIterator->second, rNeighbourIterator->second);
            DCharmNode::diff_set * diffSet = DCharmNode::getDiffSet(rNeighbourIterator->second, childIterator->second);
            node = new DCharmNode(childIterator->second, itemSetUnion, diffSet);
            if (node->getSupport() >= minSupport)
                charmProperty(rootNode, itemSetUnion, diffSet, &childIterator, &rNeighbourIterator);
            rNeighbourIterator = savedIterator;
        }
        if(childIterator->second->hasChildren()) {
            charmExtend(&(childIterator->second), minSupport);
            childIterator->second->removeChildren();
        }

        if(!isSubsumed(node))
            insertClosedSet(node);
        else
            if (node != childIterator->second)
                delete node;
    }
}

void DCharm::charmProperty(DCharmNode ** rootNode, DCharmNode::item_set * X, DCharmNode::diff_set * Y, DCharmNode::childIterator * nodeIiterator, DCharmNode::childIterator * nodeJiterator){
    DCharmNode * nodeI = (*nodeIiterator)->second;
    DCharmNode * nodeJ = (*nodeJiterator)->second;
    if (nodeI->equalsDiffSet(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->updateItemSet(X);
    }
    else if (nodeJ->containsDiffSet(nodeI))
        (*nodeIiterator)->second->updateItemSet(X);
    else if (nodeI->containsDiffSet(nodeJ)){
        (*rootNode)->removeChild(*nodeJiterator);
        (*nodeIiterator)->second->insertChild(new DCharmNode((*nodeIiterator)->second, X, Y));
    }
    else
        (*nodeIiterator)->second->insertChild(new DCharmNode((*nodeIiterator)->second, X, Y));
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

void DCharm::printClosedItemsets(){
    for (const auto& element : closedItemsets){
        for (auto closedItemSet : element.second){
            DCharmNode::printItemSet(closedItemSet.first);
        }
    }
}

DCharm::~DCharm(){
    for (const auto& closedItemsetsMapElement : closedItemsets){
        for (auto closedItemset: closedItemsetsMapElement.second)
           delete closedItemset.first;
    }
}
