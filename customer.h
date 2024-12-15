//
// Created by Basel Gh on 07/06/2023.
//

#ifndef WET2_DATA_CUSTOMER_H
#define WET2_DATA_CUSTOMER_H
#include <memory>
using namespace std;
class Customer{
public:
    int c_id;
    int phone;
    double amount;
    bool isVip;
    int size;
    double prize;

    Customer() = default;
    Customer(int c_id):c_id(c_id){};
    Customer(int  c_id, int phone, double amount, bool isVip, int size, double prize): c_id(c_id),phone(phone),amount(amount),isVip(isVip), size(size), prize(prize){};
    ~Customer() = default;
    Customer(const Customer& c) = default;
    Customer& operator= (const Customer& c) = default;
    bool operator==(const Customer& c){
        return this->c_id == c.c_id;
    }

    class CompareCustomerByID {
    public:
        bool operator()(shared_ptr<Customer> c1, shared_ptr<Customer> c2) const {
            return c1->c_id < c2->c_id;
        }
    };

};

#endif //WET2_DATA_CUSTOMER_H
