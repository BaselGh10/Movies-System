

#ifndef WET1_RankTree_H
#define WET1_RankTree_H

#include <iostream>
#include <memory>
using std::shared_ptr;
using std::weak_ptr;

template<class T, class Pred>
class RankTree {
public:
    struct TreeNode {
        shared_ptr<TreeNode> right;
        shared_ptr<TreeNode> left;
        shared_ptr<TreeNode> father;
        shared_ptr<T> data;
        long height;
        long numberOfSons;
        long gradeOfSubtree;
        double prize;

    };

    Pred predicate;
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> max;
    shared_ptr<TreeNode> min;
    long size; // initialize size

    void empty() {
        if(size > 0){
            empty_aux(root);
            root = nullptr;
            size = 0;
            min = nullptr;
            max = nullptr;
        }
    }

    void empty_aux(shared_ptr<TreeNode>& toDelete){
        if(!toDelete){
            return;
        }
        empty_aux(toDelete->left);
        empty_aux(toDelete->right);
        if (toDelete->father) {
            toDelete->father.reset();
        }
        if (toDelete->left) {
            toDelete->left.reset();
        }
        if (toDelete->right) {
            toDelete->right.reset();
        }
        toDelete->data.reset();
        toDelete.reset();
    }
    // private functions
    shared_ptr<TreeNode> initNode(shared_ptr<T> data, int id) const {
        shared_ptr<TreeNode> node = shared_ptr<TreeNode>(new TreeNode());

        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        node->father = nullptr;
        node->height = 0;
        node->numberOfSons = 0;

        //node->gradeOfSubtree = data->grade;
        return node;
    }


    shared_ptr<TreeNode> findNode(shared_ptr<TreeNode> treeNode, shared_ptr<T> toFind) const {
        if (treeNode == nullptr) {
            return nullptr;
        }
        if (treeNode->data == nullptr) {
            return nullptr;
        }
        if (*treeNode->data == *toFind) {
            return treeNode;
        }

        if (predicate(toFind, treeNode->data)) {
            return findNode(treeNode->left, toFind);
        }
        return findNode(treeNode->right, toFind);
    }

    double findNode1(shared_ptr<TreeNode> treeNode, shared_ptr<T> toFind) const {
        double prize = 0;
        while(!(*treeNode->data == *toFind)){
            if (predicate(toFind, treeNode->data)) {
                prize += treeNode->prize;
                treeNode = treeNode->left;
            }
            else{
                prize += treeNode->prize;
                treeNode = treeNode->right;
            }
        }
        return prize;
    }

    long BF(shared_ptr<TreeNode> treeNode) const {
        long hl = 0, hr = 0;
        if (treeNode->height == 0) {
            return hl -hr;
        }

        if (treeNode->left != nullptr) {
            hl = treeNode->left->height;
        }

        if (treeNode->right != nullptr) {
            hr = treeNode->right->height;
        }

        return hl - hr;
    }

    long getHeight(shared_ptr<TreeNode> node) const {
        return node == nullptr ? -1 : node->height;
    }

    long getNumOfSons(shared_ptr<TreeNode> node) const {
        return node == nullptr ? 0 : node->numberOfSons;
    }

    long getGradeOfSubTree(shared_ptr<TreeNode> node) const {
        return node == nullptr ? 0 : node->gradeOfSubtree;
    }

    shared_ptr<TreeNode> insertNode(shared_ptr<TreeNode> toAdd, shared_ptr<TreeNode> target,
                                    shared_ptr<TreeNode> targetFather) {
        if (target == nullptr) {
            toAdd->father = targetFather;
            toAdd->numberOfSons = 1;
            //toAdd->gradeOfSubtree = toAdd->data->grade;
            return toAdd;
        }
        // toAdd < target
        if (predicate(toAdd->data, target->data)) {
            target->left = insertNode(toAdd, target->left, target);
        } else { // target >= toAdd
            target->right = insertNode(toAdd, target->right, target);
        }
        target->numberOfSons = getNumOfSons(target->left) + getNumOfSons(target->right) + 1;

        target->height = maxInt(getHeight(target->left), getHeight(target->right)) + 1;


        target = balanceTree(target);


        target->height = maxInt(getHeight(target->left), getHeight(target->right)) + 1;
        return target;
    }

    shared_ptr<TreeNode> balanceTree(shared_ptr<TreeNode> target) {
        if (BF(target) >= -1 && BF(target) <= 1) {
            return target;
        }
        if (BF(target) < -1) {
            if (BF(target->right) == 1) {
                rotateRL(target);
            } else {
                rotateRR(target);
            }
        } else if (BF(target) > 1) {
            if (BF(target->left) == -1) {
                rotateLR(target);
            } else {
                rotateLL(target);
            }
        }
        return target->father;
    }

    void removeNode(shared_ptr<TreeNode> toRemove) {
        if (toRemove->right && toRemove->left) {
            shared_ptr<TreeNode> temp = toRemove->right;
            while (temp->left) {
                temp = temp->left;
            }
            shared_ptr<T> tempData = temp->data;
            temp->data = toRemove->data;
            toRemove->data = tempData;

            toRemove = temp;
        }

        shared_ptr<TreeNode> toBalance = toRemove->father;
        removeNodeWithLessThanTwoSons(toRemove);



        while (toBalance) {
            toBalance->height = maxInt(getHeight(toBalance->left), getHeight(toBalance->right)) + 1;
            toBalance->numberOfSons = getNumOfSons(toBalance->right) + getNumOfSons(toBalance->left) + 1;

            balanceTree(toBalance);
            toBalance = toBalance->father;
        }
    }

    void removeNodeWithLessThanTwoSons(shared_ptr<TreeNode> toRemove) {
        if (!toRemove->left && !toRemove->right) { // toRemove is a leaf
            if (!toRemove->father) { // root is a leaf
                this->root = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = nullptr;
            } else {
                toRemove->father->right = nullptr;
            }
            return;
        }
        if (!toRemove->left) { // toRemove has right son only
            if (!toRemove->father) { // root is a
                this->root = toRemove->right;
                toRemove->right->father = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = toRemove->right;
                toRemove->right->father = toRemove->father;
            } else {
                toRemove->father->right = toRemove->right;
                toRemove->right->father = toRemove->father;
            }
            return;
        }

        if (!toRemove->right) { // toRemove has left son only
            if (!toRemove->father) { // root is a
                this->root = toRemove->left;
                toRemove->left->father = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = toRemove->left;
                toRemove->left->father = toRemove->father;
            } else {
                toRemove->father->right = toRemove->left;
                toRemove->left->father = toRemove->father;
            }
        }
    }

    void fixMax() {
        this->max = root;
        if (root) {
            while (this->max->right) {
                this->max = this->max->right;
            }
        }
    }

    void fixMin() {
        this->min = root;
        if (root) {
            while (this->min->left) {
                this->min = this->min->left;
            }
        }
    }

    // rotations
    // **** adjust height of changed nodes.
    void rotateLL(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> leftSon = node->left;


        leftSon->father = node->father;
        //////////////////////////////////
        double temp = leftSon->prize;
        leftSon->prize += node->prize;
        node->prize -= leftSon->prize;
        //////////////////////////////////
        if (node->father == nullptr) {
            this->root = leftSon;
        } else if (node->father->left == node){
            node->father->left = leftSon;
        } else {
            node->father->right = leftSon;
        }

        node->left = leftSon->right;
        if (node->left != nullptr) {
            node->left->father = node;
            ////////////////////////////
            node->left->prize += temp;
            ////////////////////////////
        }

        leftSon->right = node;
        node->father = leftSon;

        node->numberOfSons = getNumOfSons(node->right) + getNumOfSons(node->left) + 1;

        leftSon->numberOfSons = getNumOfSons(leftSon->right) + getNumOfSons(leftSon->left) + 1;

        node->height = maxInt(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void rotateRR(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> rightSon = node->right;

        rightSon->father = node->father;
        //////////////////////////////////////
        double temp = rightSon->prize;
        rightSon->prize += node->prize;
        node->prize -= rightSon->prize;
        /////////////////////////////////////

        if (node->father == nullptr) {
            this->root = rightSon;
        } else if (node->father->left == node){
            node->father->left = rightSon;
        } else {
            node->father->right = rightSon;
        }

        node->right = rightSon->left;
        if (node->right != nullptr) {
            node->right->father = node;
            ////////////////////////////
            node->right->prize += temp;
            ////////////////////////////
        }

        rightSon->left = node;
        node->father = rightSon;



        node->numberOfSons = getNumOfSons(node->right) + getNumOfSons(node->left) + 1;

        rightSon->numberOfSons = getNumOfSons(rightSon->right) + getNumOfSons(rightSon->left) + 1;


        node->height = maxInt(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void rotateLR(shared_ptr<TreeNode> node) {
        rotateRR(node->left);
        rotateLL(node);
    }

    void rotateRL(shared_ptr<TreeNode> node) {
        rotateLL(node->right);
        rotateRR(node);
    }


    // aux
    long maxInt(long x1, long x2) const {
        return x1 > x2 ? x1 : x2;
    }

    RankTree() : size(0) {}

    ~RankTree() {
        empty();
    }

    class AVLIter;
    // exceptions
    class AlreadyExists : public std::exception {};
    class DoesntExist : public std::exception {};

    // throws exception AlreadyExists if already exists
    void insert(shared_ptr<T> data, int id) {
        if (!root) {
            root = initNode(data, id);
        } else {
            if (findNode(root, data)) {
                throw AlreadyExists();
            }
            insertNode(initNode(data, id), root, nullptr);
        }
        ++size;
        fixMax();
        fixMin();
    }



    // returns nullptr if not found
    shared_ptr<T> find(shared_ptr<T> data) const {
        shared_ptr<TreeNode> toFind = findNode(root, data);
        return !toFind ? nullptr : toFind->data;
    }
    shared_ptr<TreeNode> findTheNode(shared_ptr<T> data) const {
        shared_ptr<TreeNode> toFind = findNode(root, data);
        return !toFind ? nullptr : toFind;
    }

    void updatePrize(shared_ptr<T> data) const {
        double prize = findNode1(root, data);
        shared_ptr<TreeNode> toFind = findNode(root, data);
        toFind->prize = -prize;
    }

    // throws DoesntExist
    void remove(shared_ptr<T> data) {
        shared_ptr<TreeNode> toRemove = findNode(root, data);
        if (!toRemove) {
            throw DoesntExist();
        }
        removeNode(toRemove);
        size--;
        fixMax();
        fixMin();
    }

    shared_ptr<T> getMax() const {
        return this->max->data;
    }

    long getSize() const {
        return this->size;
    }

    bool isEmpty() const {
        return this->size == 0;
    }

    shared_ptr<TreeNode> getRoot(){
        return root;
    }

    // iterator and merge
    class AVLIter {
        shared_ptr<TreeNode> current;
        shared_ptr<TreeNode> finish;
    public:
        AVLIter(shared_ptr<TreeNode> current, shared_ptr<TreeNode> finish) : current(current), finish(finish) {}
        AVLIter() {
            current = nullptr;
            finish = nullptr;
        }

        bool operator==(const AVLIter& iter) {
            return this->current == iter.current;
        }

        bool operator!=(const AVLIter& iter) {
            return this->current != iter.current;
        }

        AVLIter& operator++() {
            if (current == finish) {
                current = nullptr;
            } else {
                if (current->right) {
                    current = current->right;
                    while (current->left) {
                        current = current->left;
                    }
                } else if (!current->father) {
                    current = nullptr;
                } else if (current->father->left == current) {
                        current = current->father;
                    } else {
                        while (current->father->right == current) {
                            current = current->father;
                        }
                        current = current->father;
                    }
                }
            return *this;
        }

        shared_ptr<T> operator*(){
            if (!current) {
                return nullptr;
            }
            return current->data;
        }

        friend class RankTree<T, Pred>;
    };

    AVLIter begin() const {
        return AVLIter(this->min, this->max);
    }

    AVLIter end() const {
        return AVLIter(nullptr , nullptr);
    }
};




#endif //WET1_RankTree_H
