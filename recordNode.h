//
// Created by Basel Gh on 07/06/2023.
//

#ifndef WET2_DATA_RECORDNODE_H
#define WET2_DATA_RECORDNODE_H
#include "record.h"
//#include "treeNode.h"
using namespace std;


class recordNode {
public:
    UnionTreeNode* node;
    /*
    class CompareRecordNodeByID {
    public:
        bool operator()(shared_ptr<recordNode> c1, shared_ptr<recordNode> c2) const {
            return c1->groupOfRec->groupId < c2->groupOfRec->groupId;
        }
    };*/

    recordNode(UnionTreeNode* node ):node(node){};
    ~recordNode(){
        delete this->node;
    }
    /*
    bool operator==(const recordNode& c2) {
        return this->groupOfRec->groupId == c2.groupOfRec->groupId;
    }*/

};
#endif //WET2_DATA_RECORDNODE_H
