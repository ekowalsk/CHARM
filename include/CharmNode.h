#ifndef PROJEKT_CHARMNODE_H
#define PROJEKT_CHARMNODE_H

#include <list>
#include <map>

class CharmNode {
public:
    typedef std::list<int> tid_list;
    typedef std::list<int> item_set;
    typedef std::multimap<int, CharmNode *>::iterator childIterator;
private:
    item_set * itemSet;
    tid_list * tidList;

    CharmNode * parent;
    std::multimap<int, CharmNode *> * children; // map of pairs <support, Node>

    static item_set * copyItemSet(const item_set * source);
    static item_set * unionItemSet(item_set * itemSet1, item_set * itemSet2);

    static tid_list * copyTidList(tid_list * tidList);
    static tid_list * intersectTidList(tid_list * tidList1, tid_list * tidList2);

public:
    CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList);
    ~CharmNode();

    static item_set * unionItemSet(CharmNode * node1, CharmNode * node2);
    static tid_list * intersectedTidList(const CharmNode& node1, const CharmNode& node2);

    void setItemSet(item_set * itemSet);
    void setTidList(tid_list * tidList);

    void insertChild(CharmNode * child);
    void removeChild(childIterator childIt);

    int getHash();
    int getSupport();
};
#endif //PROJEKT_CHARMNODE_H
