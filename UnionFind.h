
#ifndef AVLTREE_H_UnionFindRecords_H
#define AVLTREE_H_UnionFindRecords_H
#include "customerHash.h"
#include "recordNode.h"

using std::shared_ptr;
using std::weak_ptr;



class UnionFindRecords
{

public:
    recordNode* *theRecords;
    long size;


    UnionFindRecords(long size) :size(size) {
        this->theRecords = new recordNode*[size];
    }
    ~UnionFindRecords(){
        delete[] this->theRecords;
    }


    UnionFindRecords& operator=(const UnionFindRecords& c){
        delete[] this->theRecords;
        try {
            this->theRecords = new recordNode*[c.size];
            this->size = c.size;
            for (int i = 0; i < this->size; i++) {
                this->theRecords[i] = c.theRecords[i];
            }
        }
        catch (...){
            delete[] this->theRecords;
        }
        return *this;

    }
    void updateExtraValue(UnionTreeNode* toUpdate, UnionTreeNode* root)
    {
        if(toUpdate->father == nullptr || toUpdate == root)
        {
            return;
        }

        //UnionTreeNode* toUpdate2 = toUpdate;
        //UnionTreeNode* toUpdate3 = toUpdate;

        double sumExtra1 = toUpdate->valueExtra;
        UnionTreeNode* firstNode = toUpdate;
        while(toUpdate->father != root){
            toUpdate = toUpdate->father;
            sumExtra1 += toUpdate->valueExtra;
        }
        toUpdate = firstNode;
        toUpdate->valueExtra = sumExtra1;


    }

    //returns the root of toFind in the uptree
    UnionTreeNode* find(UnionTreeNode* toFind) {
        if(toFind->father == nullptr)
        {
            return toFind;
        }
        UnionTreeNode* root = find(toFind->father);
        updateExtraValue(toFind, root);
        toFind->father = root;
        return root;
    }


    //returns the team of the current player
    /*shared_ptr<Customer> findCustomer(long id) {
        shared_ptr<UnionTreeNode> toFind = members.findPlayerbyId(id);    //find((*members)[id]);        // was * not shared
        //return toFind->theCustomer.lock();
        return toFind->team;
    }*/

    /*UnionTreeNode* findPlayer(long id) {
        UnionTreeNode* toFind = theRecords[id].node;
        return toFind;
    }*/




    double getTotalExtraValue(UnionTreeNode* toFind){
        double totalValue = 0;
        while (toFind != nullptr){
            totalValue += toFind->valueExtra;
            toFind = toFind->father;
        }
        return totalValue;
    }



    shared_ptr<record> unionRecords(long c1_id, long c2_id)
    {
        UnionTreeNode* sRecord = this->theRecords[c1_id]->node;
        UnionTreeNode* bRecord = this->theRecords[c2_id]->node;


        if(sRecord->size <= bRecord->size){
            sRecord->valueExtra -= bRecord->valueExtra;
            sRecord->valueExtra += bRecord->total_extra;
            bRecord->total_extra += sRecord->total_extra;
            sRecord->father = bRecord;
            bRecord->size += sRecord->size;
            //sRecord->head = bRecord->head;

        }

        else{

            sRecord->valueExtra += bRecord->total_extra;
            bRecord->valueExtra -= sRecord->valueExtra;
            sRecord->total_extra += bRecord->total_extra;
            bRecord->father = sRecord;
            //bRecord->head->size += sRecord->size;
            //sRecord->head = bRecord->head;
            sRecord->MainRec = bRecord->MainRec;
            bRecord->size += sRecord->size;



        }

        return bRecord->rec;
    }

};

#endif //AVLTREE_H_UnionFindRecords_H
