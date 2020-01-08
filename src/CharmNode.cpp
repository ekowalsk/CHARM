#include "CharmNode.h"

CharmNode::CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList): Node(parent, itemSet){
    this->tidList = copyTidList(tidList);
}

CharmNode::tid_list * CharmNode::copyTidList(tid_list * tidList){
    auto tidListCopy = new tid_list();
    for (auto &tid : *tidList)
        tidListCopy->push_back(tid);
    return tidListCopy;
}

void CharmNode::intersectTidList(tid_list * intersectSource){
    tidList->sort();
    intersectSource->sort();
    auto intersectionTidList = new tid_list();
    auto destIterator = tidList->begin();
    for (auto &sourceTid : *intersectSource){
        while (destIterator != tidList->end() && *destIterator < sourceTid)
            destIterator++;
        if (*destIterator == sourceTid)
            intersectionTidList->push_back(sourceTid);
    }
    delete tidList;
    tidList = intersectionTidList;
}

CharmNode::~CharmNode() {
    delete tidList;
}