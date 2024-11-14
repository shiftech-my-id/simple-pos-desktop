#include "licensemanager.h"
#include "global.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <string>
#include <windows.h>
#include <comutil.h>

bool LicenseManager::checkLicense()
{
    QFile f(APP_LICENSE_PATH);
    if (!QFile::exists(APP_LICENSE_PATH)) {
        const char * message = "Lisensi tidak ditemukan.";
        qCritical() << message;
        QMessageBox::critical(nullptr, "ERROR", message);
        return false;
    }

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        const char * message = "File lisensi tidak bisa dibaca.";
        qCritical() << message;
        QMessageBox::critical(nullptr, "ERROR", message);
        return false;
    }

    QByteArray storedKey = f.readAll();

    if (storedKey != serialNumber() + activationKey()) {
        const char * message = "Kode lisensi tidak valid.";
        qCritical() << message;
        QMessageBox::critical(nullptr, "ERROR", message);
        return false;
    }

    return true;
}

LicenseManager::LicenseManager()
{
    _uniqueKey = _getUniqueKey();
}

bool LicenseManager::activate(const QString& key) {
    qDebug() << key << serialNumber() << activationKey();
    if (key != activationKey()) {
        const char * message = "Kode lisensi tidak valid.";
        qCritical() << message;
        return false;
    }

    // save to license file
    QFile f(APP_LICENSE_PATH);
    if (!f.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        const char * message = "Gagal membuat file lisensi.";
        qCritical() << message;
        return false;
    }

    QTextStream out(&f);
    out << serialNumber() << activationKey();
    f.close();

    return true;
}

QString LicenseManager::_getUniqueKey()
{
    DWORD serialNumber = 0;
    GetVolumeInformationA("C:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0);
    QString sn = QString::fromStdString(std::to_string(serialNumber));
    sn.prepend("SHX-");
    sn.append("-2024");
    return QCryptographicHash::hash(sn.toLocal8Bit(), QCryptographicHash::Sha1).toHex().toUpper();
}

QString LicenseManager::serialNumber() const {
    return _uniqueKey.first(_uniqueKey.size() / 2);
}

QString LicenseManager::activationKey() const {
    return generateActivationKey(serialNumber());
}

QString LicenseManager::generateActivationKey(const QString& sn)
{
    QString key = QCryptographicHash::hash(sn.toLocal8Bit(), QCryptographicHash::Sha1).toHex().toUpper();
    return key.last(key.size() / 2);
}
