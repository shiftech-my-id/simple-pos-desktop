#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

#include "global.h"
#include "singleton.h"
#include <QString>

class CORE_EXPORT LicenseManager : public Singleton<LicenseManager>
{
public:
    inline QString uniqueKey() const { return _uniqueKey; }

    bool checkLicense();
    bool activate(const QString& key);

    QString serialNumber() const;
    QString activationKey() const;

    static QString generateActivationKey(const QString& sn);

    SINGLETON_INSTANCE(LicenseManager)

private:
    LicenseManager();

    QString _getUniqueKey();

    QString _uniqueKey;
};

#endif // LICENSEMANAGER_H
