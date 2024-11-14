#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "licensemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LicenseManager* lm = LicenseManager::instance();

    qDebug() << "activating";
    lm->activate(lm->activationKey());

    if (!lm->checkLicense()) {
        qDebug() << "prompt key input";
        return 1;
    }

    qDebug() << "license valid";

    return 0;
}
