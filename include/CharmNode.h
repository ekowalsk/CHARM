#ifndef PROJEKT_CHARMNODE_H
#define PROJEKT_CHARMNODE_H

#include "Node.h"

class CharmNode : public Node {
public:
    typedef std::list<int> tid_list;
private:
    tid_list * tidList;

    static tid_list * copyTidList(tid_list * tidList);
    static tid_list * intersectTidList(tid_list * tidList1, tid_list * tidList2);

public:
    CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList);
    ~CharmNode();
    static tid_list * intersectedTidList(CharmNode node1, CharmNode node2);
    void setTidList(tid_list * tidList);
};
#endif //PROJEKT_CHARMNODE_H
