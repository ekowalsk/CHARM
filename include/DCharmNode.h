#ifndef DCHARMNODE_H
#define DCHARMNODE_H

#include <functional>
#include <list>
#include <map>
#include <vector>

class DCharmNode {
public:
    typedef std::list<int> diff_set;
    typedef std::list<int> item_set;
    typedef std::multimap<std::list<int>, DCharmNode*, std::function<bool(const std::list<int>&, const std::list<int>&)>>::iterator child_iterator;

private:
    item_set* itemSet;
    diff_set* diffSet;
    int sortMode;

    DCharmNode* parent;
    std::multimap<std::list<int>, DCharmNode*, std::function<bool(const std::list<int>&, const std::list<int>&)>>* children;

    int hashValue;
    int support;

    int calculateSupport(DCharmNode* parentNode);
    int getHashOfDiffset();
    int calculateHash();

    static item_set* unionItemSet(item_set* itemSet1, item_set* itemSet2);

    static diff_set* differenceDiffSet(diff_set* diffSet1, diff_set* diffSet2);

public:
    DCharmNode(DCharmNode* parent, item_set* itemSet, diff_set* diffSet, int support = -1, int sortMode = 0);
    ~DCharmNode();

    static item_set* unionItemSet(const DCharmNode* node1, const DCharmNode* node2);
    static diff_set* getDiffSet(const DCharmNode* node1, const DCharmNode* node2);

    child_iterator getChildrenBegin();
    child_iterator getChildrenEnd();

    item_set* getItemSet();
    int getSortMode();

    void setItemSet(item_set* itemSet);

    void insertChild(DCharmNode* child, int childSupport = -1);
    void removeChild(child_iterator childIt);

    void updateItemSet(item_set* itemSet);

    int getHash();
    int getSupport();

    static bool isItemSetContained(item_set* contains, item_set* contained);
    bool equalsDiffSet(DCharmNode *node);
    bool containsDiffSet(DCharmNode *node);
    bool hasChildren();

    void static printItemSet(item_set* itemSet, std::vector<std::string>& names);
};
#endif
