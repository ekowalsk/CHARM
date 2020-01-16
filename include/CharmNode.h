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
    CharmNode(CharmNode * parent, item_set * itemSet, tid_list * tidList, int sortMode = 1);
    ~CharmNode();

    static item_set * unionItemSet(const CharmNode * node1, const CharmNode * node2);
    static tid_list * intersectedTidList(const CharmNode * node1, const CharmNode * node2);

    childIterator getChildrenBegin();
    childIterator getChildrenEnd();

    item_set * getItemSet();
    tid_list * getTidList();

    void setItemSet(item_set * itemSet);
    void setTidList(tid_list * tidList);

    // children <*list<int>, Node> - if na mode
    void insertChild(CharmNode * child);
    void removeChild(childIterator childIt);
    void removeChildren();

    void updateItemSet(item_set * itemSet);

    int static getHash(tid_list * tidList);
    int getSupport();

    static bool isItemSetContained(item_set * contains, item_set * contained);
    bool equalsTidList(CharmNode *node);
    bool containsTidList(CharmNode *node);
    bool hasChildren();

    void static printItemSet(item_set * itemSet);
};
#endif //PROJEKT_CHARMNODE_H
