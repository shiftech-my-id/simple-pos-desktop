#include "stockupdate.h"

StockUpdate::StockUpdate() :
    code(0),
    total(0.),
    discount(0.),
    totalPayment(0.),
    totalPaid(0.),
    exchange(0.),
    totalCost(0.),
    totalProfit(0.)
{
}

QVariantMap StockUpdate::toMap() const
{
    QVariantMap data;
    data["id"] = id == 0 ? QVariant() : id;
    data["code"] = code;
    data["type"] = type;
    data["total"] = total;
    data["discount"] = discount;
    data["total_payment"] = totalPayment;
    data["total_paid"] = totalPaid;
    data["exchange"] = exchange;
    data["total_cost"] = totalCost;
    data["total_profit"] = totalProfit;
    data["datetime"] = dateTime;
    data["party_name"] = partyName;
    data["party_address"] = partyAddress;
    return data;
}

void StockUpdate::fromMap(const QVariantMap &o)
{
    id = o.value("id").toInt();
    code = o.value("code").toInt();
    type = o.value("type").toInt();
    total = o.value("total").toDouble();
    discount = o.value("discount").toDouble();
    totalPayment = o.value("total_payment").toDouble();
    totalPaid = o.value("total_paid").toDouble();
    exchange = o.value("exchange").toDouble();
    totalCost = o.value("total_cost").toDouble();
    totalProfit = o.value("total_profit").toDouble();
    dateTime = o.value("datetime").toDateTime();
    partyName = o.value("party_name").toString();
    partyAddress = o.value("party_address").toString();
}
