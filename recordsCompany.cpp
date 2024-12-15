//
// Created by Basel Gh on 07/06/2023.
#include "recordsCompany.h"
#include "recordNode.h"

shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_prev(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node);
shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_next(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node);


shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_max(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node, int id){
    auto max = node;
    while(max) {
        if (max->data->c_id == id) {
            break;
        } else  if(max->data->c_id < id){
            if(max->right == nullptr){
                break;
            }
            if(max->right->data->c_id > id && max->right->left == nullptr){
                return max;
            }
            max = max->right;
        }
        else{
            if(max->left == nullptr){
                break;
            }
            if(max->left->data->c_id < id && max->left->right == nullptr){
                return max->left;
            }
            max = max->left;
        }
    }
    if(max->data->c_id > id){
        return find_prev(max);
    }
    return max;
}

shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_min(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node, int id){
    auto min = node;
    while(min) {
        if (min->data->c_id == id) {
            break;
        } else  if(min->data->c_id < id){
            if(min->right == nullptr){
                break;
            }
            if(min->right->data->c_id > id && min->right->left == nullptr){
                return min->right;
            }
            min = min->right;
        }
        else{
            if(min->left == nullptr){
                break;
            }
            if(min->left->data->c_id < id && min->left->right == nullptr){
                return min;
            }
            min = min->left;
        }
    }
    if(min->data->c_id < id){
        return find_next(min);
    }
    return min;
}

shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_next(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node) {
    if(node->father == nullptr){
        if(node->right != nullptr){
            return node->right;
        }
        return nullptr;
    }
    if(node->right != nullptr){
        return node->right;
    }
    else if(node->father->left == node){
        return node->father;
    }
    else{
        while(node->father->left != node){
            node = node->father;
            if(node->father == nullptr){
                return nullptr;
            }
        }
        return node->father;
    }
}

shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> find_prev(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> node) {
    if(node->father == nullptr){
        if(node->left != nullptr){
            if(node->left->right == nullptr) {
                return node->left;
            }
            else{
                node = node->left;
                while(node->right != nullptr){
                    node = node->right;
                }
                return node;
            }
        }
        return nullptr;
    }
    if(node->left != nullptr){
        if(node->left->right == nullptr) {
            return node->left;
        }
        else{
            node = node->left;
            while(node->right != nullptr){
                node = node->right;
            }
            return node;
        }
    }
    else if(node->father->right == node){
        return node->father;
    }
    else{
        while(node->father->right != node){
            node = node->father;
            if(node->father == nullptr){
                return nullptr;
            }
        }
        return node->father;
    }
}

void hash_inorder(shared_ptr<AVLTree<customerNode, customerNode::CompareCustomerNodeByID>::TreeNode> T){
    if(T == nullptr) return;
    hash_inorder(T->left);
    T->data->customer->amount = 0;
    hash_inorder(T->right);
}

void tree_inorder(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> T){
    if(T == nullptr) return;
    tree_inorder(T->left);
    T->prize = 0;
    tree_inorder(T->right);
}


RecordsCompany::RecordsCompany() {
    this->theCustomers = shared_ptr<CustomerHash>(new CustomerHash());
    this->uniRecords = shared_ptr<UnionFindRecords>(new UnionFindRecords(0));
    this->vipCustomers = shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>>(new RankTree<Customer,Customer::CompareCustomerByID>());
    this->size = 0;
}

RecordsCompany::~RecordsCompany(){
    for(int i=0; i<this->uniRecords->size;i++) {
        delete this->uniRecords->theRecords[i];
    }

}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records) {
    if(number_of_records < 0){
        return INVALID_INPUT;
    }
    for(int i=0; i<this->uniRecords->size;i++) {
        delete this->uniRecords->theRecords[i];
    }

    shared_ptr<UnionFindRecords> x = shared_ptr<UnionFindRecords>(new UnionFindRecords(number_of_records));
    for(int i = 0; i < number_of_records; i++){
        shared_ptr<record> y = shared_ptr<record>(new record(i,records_stocks[i],0));
        UnionTreeNode* node = new UnionTreeNode(nullptr, y, y,0,records_stocks[i],1);
        x->theRecords[i] = new recordNode(node);
    }
    this->uniRecords = x;
    this->size = this->uniRecords->size;

    for(int i = 0; i < this->theCustomers->arraySize; i++){
        hash_inorder(this->theCustomers->array[i].get_root());
    }

    for(int i = 0; i < this->vipCustomers->size; i++){
        tree_inorder(this->vipCustomers->getRoot());
    }
    return SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone) {
    if(c_id<0 || phone<0){
        return INVALID_INPUT;
    }
    if(this->theCustomers->find(c_id)){
        return ALREADY_EXISTS;
    }
    shared_ptr<Customer> x = shared_ptr<Customer>(new Customer(c_id,phone,0, false,0,0));
    shared_ptr<customerNode> cus = shared_ptr<customerNode>(new customerNode(x));
    this->theCustomers->insert(cus);
    return SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id) {
    if(c_id<0){
        return INVALID_INPUT;
    }
    if(!this->theCustomers->find(c_id)){
        return DOESNT_EXISTS;
    }
    int result = this->theCustomers->findCustomerById(c_id)->customer->phone;
    Output_t<int> out = Output_t<int>(result);
    return out;

}

StatusType RecordsCompany::makeMember(int c_id) {
    if(c_id<0){
        return INVALID_INPUT;
    }
    if(!this->theCustomers->find(c_id)){
        return DOESNT_EXISTS;
    }

    shared_ptr<Customer> toCheck = shared_ptr<Customer>(new Customer(c_id));
    if(this->vipCustomers->find(toCheck)){
            return ALREADY_EXISTS;
    }
    this->vipCustomers->insert(this->theCustomers->findCustomerById(c_id)->customer, c_id);
    shared_ptr<Customer> toCheck1 = this->vipCustomers->find(toCheck);
    this->vipCustomers->updatePrize(toCheck1);
    this->theCustomers->findCustomerById(c_id)->customer->isVip = true;
    return SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id) {
    if(c_id<0){
        return INVALID_INPUT;
    }
    if(!this->theCustomers->find(c_id)){
        return DOESNT_EXISTS;
    }

    bool result = this->theCustomers->findCustomerById(c_id)->customer->isVip;
    Output_t<bool> out = Output_t<bool>(result);
    return out;

}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    if(c_id < 0 || r_id < 0){
        return INVALID_INPUT;
    }
    if(!this->theCustomers->find(c_id)){
        return DOESNT_EXISTS;
    }
    if(r_id >= this->size){
        return DOESNT_EXISTS;
    }
    shared_ptr<Customer> toCheck = shared_ptr<Customer>(new Customer(c_id));
    if(this->vipCustomers->find(toCheck)){
        this->theCustomers->findCustomerById(c_id)->customer->amount += (100 + this->uniRecords->theRecords[r_id]->node->rec->buys);
    }
    this->uniRecords->theRecords[r_id]->node->rec->amount--;
    this->uniRecords->theRecords[r_id]->node->rec->buys++;

    return SUCCESS;
}

static void Add(shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> root, int id, double toAdd){
    bool flag1 = false, flag2 = false;
    if(root == nullptr){
        return;
    }
    while(root->data->c_id > id){
        root = root->left;
    }
    while(root->data->c_id != id){
        if(root->data->c_id < id){
            if(!flag1){
                root->prize += toAdd;
                flag1 = true;
                flag2 = false;
            }
            root = root->right;
        }
        else if(root->data->c_id > id){
            if(!flag2){
                root->prize -= toAdd;
                flag2 = true;
                //root->left->prize += toAdd;
                flag1 = false;
            }
            root = root->left;
        }
    }
    if(root->father == nullptr){
        root->prize += toAdd;
        if(root->right != nullptr){
            root->right->prize -= toAdd;
        }
        return;
    }
    if(root->father->left == root){
        root->prize += toAdd;
    }
    if(root->right != nullptr){
        root->right->prize -= toAdd;
    }
}



StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {
    if(c_id1 < 0 || c_id2 < c_id1 || amount <= 0){
        return INVALID_INPUT;
    }
    if(this->vipCustomers->size == 0){
        return SUCCESS;
    }
    auto c1 = find_min(this->vipCustomers->getRoot(), c_id1);
    auto c2 = find_max(this->vipCustomers->getRoot(), c_id2);
    if(c1 == nullptr || c2 == nullptr){
        return SUCCESS;
    }

    if(c2->data->c_id == c_id2){
        c2 = find_prev(c2);
        if(c2 == nullptr){
            return SUCCESS;
        }
    }

    auto c3 = find_prev(c1);
    /////////////////////////////////////

    if(c1->data->c_id > c_id2){
        return SUCCESS;
    }
    if(c2->data->c_id < c_id1){
        return SUCCESS;
    }
    /////////////////////////////////////
    if(this->vipCustomers->getRoot() == nullptr){
        return SUCCESS;
    }
    Add(this->vipCustomers->getRoot(), c2->data->c_id,amount);
    if(c3 != nullptr) {
        Add(this->vipCustomers->getRoot(), c3->data->c_id, -amount);
    }

    return SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if(c_id<0){
        return INVALID_INPUT;
    }
    shared_ptr<Customer> toCheck = shared_ptr<Customer>(new Customer(c_id));
    if(!this->vipCustomers->find(toCheck)){
        return DOESNT_EXISTS;
    }
    double prize = 0;
    shared_ptr<RankTree<Customer,Customer::CompareCustomerByID>::TreeNode> c = this->vipCustomers->findTheNode(toCheck);

    while(c->father != nullptr){
        prize += c->prize;
        c = c->father;
    }
    prize += c->prize;
    shared_ptr<Customer> v =this->vipCustomers->find(toCheck);
    double result = v->amount - prize;
    Output_t<double> out = Output_t<double>(result);
    return out;
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if(r_id1 < 0 || r_id2 < 0){
        return INVALID_INPUT;
    }
    if(r_id2 >= this->uniRecords->size || r_id1 >= this->uniRecords->size){
        return DOESNT_EXISTS;
    }
    UnionTreeNode* x1 = this->uniRecords->theRecords[r_id1]->node;
    UnionTreeNode* x2 = this->uniRecords->theRecords[r_id2]->node;
    if(this->uniRecords->find(x1) == this->uniRecords->find(x2)){
        return FAILURE;
    }
    UnionTreeNode* y1 = this->uniRecords->find(x1);
    UnionTreeNode* y2 = this->uniRecords->find(x2);
    shared_ptr<record> newRecord = this->uniRecords->unionRecords(y1->rec->r_id, y2->rec->r_id);
    return SUCCESS;


}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight) {
    if(r_id < 0 || column == nullptr || hight == nullptr){
        return INVALID_INPUT;
    }
    if(r_id >= this->uniRecords->size){
        return DOESNT_EXISTS;
    }

    UnionTreeNode* x = this->uniRecords->theRecords[r_id]->node;
    UnionTreeNode* y= this->uniRecords->find(x);
    if(x->father != nullptr) {
        *hight = x->valueExtra + x->father->valueExtra;
    }
    else{
        *hight = x->valueExtra;
    }
    *column = y->MainRec->r_id;

    return SUCCESS;
}

