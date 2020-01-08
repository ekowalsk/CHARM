#ifndef PROJEKT_CHARMNODE_H
#define PROJEKT_CHARMNODE_H

#include "Node.h"

class CharmNode : public Node {
public:
    typedef std::list<int> tid_list;
private:
    tid_list * tidList;

    static tid_list * copyTidList(tid_list * tidList);
    void intersectTidList(tid_list * intersectSource);

public:
    CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList);
    ~CharmNode();

};
#endif //PROJEKT_CHARMNODE_H
