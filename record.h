//
// Created by Basel Gh on 07/06/2023.
//

#ifndef WET2_DATA_RECORD_H
#define WET2_DATA_RECORD_H
#include "treeNode.h"

class record{

public:
    int r_id;
    int amount;
    int buys;
    //UnionTreeNode* node;


    record() = default;
    record(int r_id, int amount, int buys):r_id(r_id),amount(amount), buys(buys){};
    ~record() = default;
    record(const record& r) = default;
    record& operator=(const record& r) = default;

};
#endif //WET2_DATA_RECORD_H
