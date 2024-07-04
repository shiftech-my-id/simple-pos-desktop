#include "mainwindow.h"
#include "db.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationDisplayName(APP_DISPLAY_NAME);
    app.setApplicationVersion(APP_VERSION_STR);
    app.setProperty("settings-path", qApp->applicationDirPath().append("/" APP_SETTINGS_FILENAME));

    QFile f("C:/simple-pos-license.txt");
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(nullptr, "ERROR", "Lisensi tidak ditemukan, silahkan hubungi tim Shift!");
        return 1;
    }

    QByteArray data = f.readAll();
    if (data != QCryptographicHash::hash("SIMPLE_POS", QCryptographicHash::Sha1).toHex()) {
        QMessageBox::critical(nullptr, "ERROR", "Lisensi tidak valid, silahkan hubungi tim Shift!");
        return 1;
    }

    QFile file("style.css");
    file.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(file.readAll());

    QLocale::setDefault(QLocale(QLocale::Indonesian, QLocale::Indonesia));

    db::init();

    MainWindow mw;
    mw.showMaximized();

    return app.exec();
}
