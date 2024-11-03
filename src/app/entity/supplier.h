#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "party.h"

class Supplier : public Party
{
public:
    Supplier();
};

typedef QList<Supplier> Suppliers;

#endif // SUPPLIER_H
