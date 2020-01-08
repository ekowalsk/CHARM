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

CharmNode::tid_list * CharmNode::intersectedTidList(CharmNode node1, CharmNode node2){
    return intersectTidList(node1.tidList, node2.tidList);
}

CharmNode::tid_list * CharmNode::intersectTidList(tid_list * tidList1, tid_list * tidList2){
    tidList1->sort();
    tidList2->sort();
    auto intersectionList = new tid_list();
    auto tidIterator1 = tidList1->begin();
    for (auto &sourceTid : *tidList2){
        while (tidIterator1 != tidList1->end() && *tidIterator1 < sourceTid)
            tidIterator1++;
        if (*tidIterator1 == sourceTid)
            intersectionList->push_back(sourceTid);
    }
    return intersectionList;
}

void CharmNode::setTidList(tid_list * tidList){
    delete this->tidList;
    this->tidList = copyTidList(tidList);
}

CharmNode::~CharmNode() {
    delete tidList;
}