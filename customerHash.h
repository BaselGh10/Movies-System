//
// Created by Basel Gh on 31/12/2022.
//

#ifndef WORLDCUP23A2_CPP_CustomerHASH_H
#define WORLDCUP23A2_CPP_CustomerHASH_H
#include "customerNode.h"
#include "AVLTree.h"
#include "DynamicArray.h"

class CustomerHash {
public:
    DynamicArray<AVLTree<customerNode, customerNode::CompareCustomerNodeByID>> array;
    long arraySize;
    long numberOfElements;

    CustomerHash() : array(), arraySize(INIT_SIZE), numberOfElements(0) {}
    ~CustomerHash() = default;

    long getSize() const {
        return numberOfElements;
    }
    unsigned long hash(shared_ptr<customerNode> p) {
        return p->customer->c_id % arraySize;
    }

    void insert(shared_ptr<customerNode> to_add) {
        numberOfElements++;
        array[hash(to_add)].insert(to_add);

        if (numberOfElements >= arraySize) {
            rehash();
        }
    }

    void remove(shared_ptr<customerNode> to_remove) {
        numberOfElements--;
        array[hash(to_remove)].remove(to_remove);
//        if((double)numberOfElements <= (double)((arraySize) / 4)){
//            rehashDown();
//        }
    }

    bool find(long id) {
        shared_ptr<Customer> t = shared_ptr<Customer>(new Customer(id));
        shared_ptr<customerNode> toFind = shared_ptr<customerNode>(new customerNode(t));

        return array[hash(toFind)].find(toFind) != nullptr;
    }

    shared_ptr<customerNode> findCustomerById(long id) {
        shared_ptr<Customer> t = shared_ptr<Customer>(new Customer(id));
        shared_ptr<customerNode> toFind = shared_ptr<customerNode>(new customerNode(t));

        return array[hash(toFind)].find(toFind);
    }

    bool find(shared_ptr<customerNode> to_find) {
        return array[hash(to_find)].find(to_find) != nullptr;
    }

    shared_ptr<customerNode> findcustomerNode(shared_ptr<customerNode> to_find){
        return array[hash(to_find)].find(to_find);
    }

    void rehash() {
        DynamicArray<AVLTree<customerNode, customerNode::CompareCustomerNodeByID>> newArray(arraySize * 2);
        long oldSize = arraySize;
        arraySize = 2 * arraySize;
        try{
            for (long i = 0; i < oldSize; ++i) {
                for (AVLTree<customerNode, customerNode::CompareCustomerNodeByID>::AVLIter it = array[i].begin(); it != array[i].end() ; ++it){
                    newArray[hash(*it)].insert(*it);
                }
            }
        } catch (...){
            arraySize = oldSize;
            throw;
        }
        this->array.~DynamicArray();
        this->array = newArray;
        newArray.array = nullptr;
    }

    void rehashDown() {
        DynamicArray<AVLTree<customerNode, customerNode::CompareCustomerNodeByID>> newArray(arraySize / 2);
        long oldSize = arraySize;
        arraySize = arraySize / 2;
        try{
            for (long i = 0; i < oldSize; ++i) {
                for (AVLTree<customerNode, customerNode::CompareCustomerNodeByID>::AVLIter it = array[i].begin(); it != array[i].end() ; ++it){
                    newArray[hash(*it)].insert(*it);
                }
            }
        } catch (...){
            arraySize = oldSize;
            throw;
        }
        (this->array).~DynamicArray();
        this->array = newArray;
        newArray.array = nullptr;

    }

    void merge(const CustomerHash& toMergeFrom) {
        DynamicArray<AVLTree<customerNode, customerNode::CompareCustomerNodeByID>> newArray((arraySize + toMergeFrom.arraySize) * 2);
        long oldSize = arraySize;
        long toMergeSize = toMergeFrom.arraySize;
        arraySize = (this->numberOfElements + toMergeFrom.numberOfElements) * 2;
        try{
            for (long i = 0; i < oldSize; ++i) {
                for (AVLTree<customerNode, customerNode::CompareCustomerNodeByID>::AVLIter it = array[i].begin(); it != array[i].end() ; ++it){
                    newArray[hash(*it)].insert(*it);
                }
            }
            for (long i = 0; i < toMergeSize; ++i) {
                for (AVLTree<customerNode, customerNode::CompareCustomerNodeByID>::AVLIter it = toMergeFrom.array[i].begin(); it != toMergeFrom.array[i].end() ; ++it){
                    newArray[hash(*it)].insert(*it);
                }
            }
        } catch (...){
            arraySize = oldSize;
            throw;
        }
        (this->array).~DynamicArray();
        this->array = newArray;
        this->numberOfElements += toMergeFrom.numberOfElements;
        newArray.array = nullptr;
    }

};
#endif //WORLDCUP23A2_CPP_CustomerHASH_H
