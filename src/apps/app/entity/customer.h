#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "party.h"

class Customer : public Party
{
public:
    Customer();
};

typedef QList<Customer> Customers;

#endif // CUSTOMER_H
