#ifndef PROJEKT_DCHARMNODE_H
#define PROJEKT_DCHARMNODE_H

#include <list>
#include <map>

class DCharmNode {
public:
    typedef std::list<int> diff_set;
    typedef std::list<int> item_set;
    typedef std::multimap<int, DCharmNode *>::iterator childIterator;
private:
    item_set * itemSet;
    diff_set * diffSet;

    DCharmNode * parent;
    std::multimap<int, DCharmNode *> * children; // map of pairs <support, Node>

    int hashValue;
    int support;

    int calculateSupport(DCharmNode * parentNode);
    int getHashOfDiffset();

    static item_set * copyItemSet(const item_set * source);
    static item_set * unionItemSet(item_set * itemSet1, item_set * itemSet2);

    static diff_set * copyDiffSet(diff_set * diffSet);
    static diff_set * differenceDiffSet(diff_set * diffSet1, diff_set * diffSet2);

public:
    DCharmNode(DCharmNode * parent, item_set * itemSet, diff_set * diffSet, int hash, int support=-1);
    ~DCharmNode();

    static item_set * unionItemSet(const DCharmNode * node1, const DCharmNode * node2);
    static diff_set * getDiffSet(const DCharmNode * nod1, const DCharmNode * node2);

    childIterator getChildrenBegin();
    childIterator getChildrenEnd();

    item_set * getItemSet();
    diff_set * getDiffSet();

    void setItemSet(item_set * itemSet);
    void setSupport (int support);

    void insertChild(DCharmNode * child);
    void removeChild(childIterator childIt);
    void removeChildren();

    void updateItemSet(item_set * itemSet);

    int getHash();
    int getSupport();

    static bool isItemSetContained(item_set * contains, item_set * contained);
    bool equalsTidList(DCharmNode *node);
    bool containsTidList(DCharmNode *node);
    bool hasChildren();

    int static calculateHash(DCharmNode * nodeX, DCharmNode * nodeY);

    void static printItemSet(item_set * itemSet);
};
#endif //PROJEKT_DCHARMNODE_H
