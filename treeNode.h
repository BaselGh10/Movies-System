//
// Created by Basel Gh on 03/01/2023.
//

#ifndef WORLDCUP23A2_CPP_TREENODE_H
#define WORLDCUP23A2_CPP_TREENODE_H
#include "customer.h"
#include "record.h"
using namespace std;

class UnionTreeNode {
public:
    UnionTreeNode* father;
    shared_ptr<record> rec;
    shared_ptr<record> MainRec;
    int valueExtra;
    int total_extra;
    int size;





    class CompareNodeByID {
    public:
        bool operator()(std::shared_ptr<UnionTreeNode> p1, std::shared_ptr<UnionTreeNode> p2) const {
            return p1->rec->r_id < p2->rec->r_id;
        }
    };

    UnionTreeNode() = default;
    UnionTreeNode(UnionTreeNode* father, shared_ptr<record> rec, shared_ptr<record> MainRec, int extraValue,int total_extra, int size):father(father), rec(rec), MainRec(MainRec), valueExtra(extraValue),total_extra(total_extra), size(size){};
    ~UnionTreeNode() = default;

    bool operator==(const UnionTreeNode& p2) {
        return this->rec->r_id == p2.rec->r_id;
    }



};

#endif //WORLDCUP23A2_CPP_TREENODE_H
