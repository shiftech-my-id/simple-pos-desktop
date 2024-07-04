#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTimer>

#include "global.h"
#include "db.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationDisplayName(APP_DISPLAY_NAME);
    app.setApplicationVersion(APP_VERSION_STR);
    app.setProperty("settings-path", qApp->applicationDirPath().append("/" APP_SETTINGS_FILENAME));

    QFile f(LICENSE_PATH);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(nullptr, "ERROR", "Lisensi tidak ditemukan, silahkan hubungi tim Shift!");
        return 1;
    }

    QByteArray data = f.readAll();
    if (data != QCryptographicHash::hash("SIMPLE_POS", QCryptographicHash::Sha1).toHex()) {
        QMessageBox::critical(nullptr, "ERROR", "Lisensi tidak valid, silahkan hubungi tim Shift!");
        return 1;
    }

    QFile file(STYLESHEET_PATH);
    file.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(file.readAll());

    QLocale::setDefault(QLocale(QLocale::Indonesian, QLocale::Indonesia));

    db::init();
    // db::resetData();

    MainWindow mw;
    //QTimer::singleShot(100, &mw, SLOT(showLoginDialog()));
    mw.show();
    mw.showCalculatorDialog();

    return app.exec();
}
