#ifndef PROJEKT_DCHARMNODE_H
#define PROJEKT_DCHARMNODE_H

#include <list>
#include <map>
#include <vector>
#include <functional>

class DCharmNode {
public:
    typedef std::list<int> diff_set;
    typedef std::list<int> item_set;
    typedef std::multimap<std::list<int>, DCharmNode *, std::function<bool(const std::list<int>&, const std::list<int>&)>>::iterator childIterator;

private:
    item_set * itemSet;
    diff_set * diffSet;
    int sortMode;

    DCharmNode * parent;
    std::multimap<std::list<int>, DCharmNode *, std::function<bool(const std::list<int>&, const std::list<int>&)>> * children; // map of pairs <support, Node>

    int hashValue;
    int support;

    int calculateSupport(DCharmNode * parentNode);
    int getHashOfDiffset();
    int calculateHash();

    static item_set * copyItemSet(const item_set * source);
    static item_set * unionItemSet(item_set * itemSet1, item_set * itemSet2);

    static diff_set * copyDiffSet(diff_set * diffSet);
    static diff_set * differenceDiffSet(diff_set * diffSet1, diff_set * diffSet2);

public:
    DCharmNode(DCharmNode * parent, item_set * itemSet, diff_set * diffSet, int support=-1, int sortMode = 1);
    ~DCharmNode();

    static item_set * unionItemSet(const DCharmNode * node1, const DCharmNode * node2);
    static diff_set * getDiffSet(const DCharmNode * node1, const DCharmNode * node2);

    childIterator getChildrenBegin();
    childIterator getChildrenEnd();

    item_set * getItemSet();
    int getSortMode();

    void setItemSet(item_set * itemSet);

    void insertChild(DCharmNode * child, int weight = -1);
    void removeChild(childIterator childIt);
    void removeChildren();

    void updateItemSet(item_set * itemSet);

    int getHash();
    int getSupport();

    static bool isItemSetContained(item_set * contains, item_set * contained);
    bool equalsDiffSet(DCharmNode *node);
    bool containsDiffSet(DCharmNode *node);
    bool hasChildren();

    void static printItemSet(item_set * itemSet, std::vector<std::string>);
};
#endif //PROJEKT_DCHARMNODE_H
