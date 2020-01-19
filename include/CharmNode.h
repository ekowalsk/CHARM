#ifndef CHARMNODE_H
#define CHARMNODE_H

#include <functional>
#include <list>
#include <map>
#include <vector>

class CharmNode {
public:
    typedef std::list<int> tid_list;
    typedef std::list<int> item_set;
    typedef std::multimap<std::list<int>, CharmNode*, std::function<bool(const std::list<int>&, const std::list<int>&)>>::iterator child_iterator;
private:
    item_set* itemSet;
    tid_list* tidList;
    int sortMode;

    CharmNode* parent;
    std::multimap<std::list<int>, CharmNode*, std::function<bool(const std::list<int>&, const std::list<int>&)>>* children;

    static item_set* copyItemSet(const item_set* source);
    static item_set* unionItemSet(item_set* itemSet1, item_set* itemSet2);

    static tid_list* copyTidList(tid_list* tidList);
    static tid_list* intersectTidList(tid_list* tidList1, tid_list* tidList2);

public:
    CharmNode(CharmNode* parent, item_set* itemSet, tid_list* tidList, int sortMode = 0);
    ~CharmNode();

    static item_set* unionItemSet(const CharmNode* node1, const CharmNode* node2);
    static tid_list* intersectedTidList(const CharmNode* node1, const CharmNode* node2);

    child_iterator getChildrenBegin();
    child_iterator getChildrenEnd();

    item_set* getItemSet();
    tid_list* getTidList();
    int getSortMode();

    void setItemSet(item_set* itemSet);

    void insertChild(CharmNode* child, int weight = -1);
    void removeChild(child_iterator childIt);

    void updateItemSet(item_set* itemSet);

    int static getHash(tid_list* tidList);
    int getSupport();

    static bool isItemSetContained(item_set* contains, item_set* contained);
    bool equalsTidList(CharmNode* node);
    bool containsTidList(CharmNode* node);
    bool hasChildren();

    void static printItemSet(item_set* itemSet, std::vector<std::string>& namesVector);
};
#endif
