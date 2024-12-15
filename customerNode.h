//
// Created by Basel Gh on 31/12/2022.
//

#ifndef WORLDCUP23A2_CPP_customerNODE_H
#define WORLDCUP23A2_CPP_customerNODE_H
#include "treeNode.h"

using namespace std;


class customerNode {
public:
    shared_ptr<Customer> customer;
    shared_ptr<UnionTreeNode> head;

    class CompareCustomerNodeByID {
    public:
        bool operator()(shared_ptr<customerNode> c1, shared_ptr<customerNode> c2) const {
            return c1->customer->c_id < c2->customer->c_id;
        }
    };

    customerNode(shared_ptr<Customer> customer):customer(customer){};
    //customerNode(shared_ptr<Customer> customer, shared_ptr<UnionTreeNode> head):customer(customer),isActive(isActive), head(head){};
    ~customerNode() = default;

    bool operator==(const customerNode& c2) {
        return this->customer->c_id == c2.customer->c_id;
    }

};

#endif //WORLDCUP23A2_CPP_customerNODE_H
