#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QSettings>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QSqlError>

#include <iostream>

#include "global.h"
#include "db/db.h"
#include "widgets/mainwindow.h"
#include "widgets/application.h"
#include "licensemanager.h"
#include <QCryptographicHash>

static QTextStream sLogStream;
static int sCounter = 0;

void MessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString formatted = qFormatLogMessage(type, context, msg);
    std::cout << formatted.toStdString() << "\n";
    std::cout.flush();
    sLogStream << formatted << "\n";
    if (sCounter++ % 10 == 0)
        sLogStream.flush();
}

void init_logger()
{
#ifndef QT_DEBUG
    QDir dir = QDir(qApp->applicationDirPath());
    dir.mkdir("logs");
    dir.cd("logs");
    qSetMessagePattern("%{time yyyy-MM-dd HH:mm:ss} %{type} %{message}");
    qInstallMessageHandler(MessageHandler);
#ifdef Q_OS_WIN32
    const QString logFileName(APP_NAME ".log");
#else
    const QString logFileName(dir.absoluteFilePath("log.log"));
#endif
    static QFile file(dir.absoluteFilePath(logFileName));
    QFileInfo fi(dir.absoluteFilePath(logFileName));
    if (fi.size() >= (2 * 1024 * 1024)) {
        QFile::rename(logFileName, logFileName + "." + QDateTime::currentDateTime().toString("yyyyMMddhhMMss"));
    }
    file.open(QFile::Append);
    sLogStream.setDevice(&file);
#endif
}

void init_locale()
{
    QLocale::setDefault(QLocale(QLocale::Indonesian, QLocale::Indonesia));
}

void init_stylesheet()
{
    QFile cssFile(APP_STYLESHEET_PATH);
    if (cssFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(cssFile.readAll()));
    }
}

void init_applocker(QSettings &settings)
{
    QTcpServer server;
    {
        QDateTime now = QDateTime::currentDateTime();
        QDateTime lastAccessTimestamp = settings.value(SK_APPLOCKER_TIMESTAMP, now).toDateTime();
        if (QDateTime::currentDateTime() < lastAccessTimestamp) {
            qCritical() << "Periksa jam dan tanggal pada sistem!";
            QMessageBox::critical(nullptr, "Kesalahan", "Periksa jam dan tanggal pada sistem!");
            exit(2);
        }
        settings.setValue(SK_APPLOCKER_TIMESTAMP, now);

        QHostAddress addr(settings.value(SK_APPLOCKER_ADDRESS, "127.0.0.1").toString());
        quint16 port = settings.value(SK_APPLOCKER_PORT, 22122).value<quint16>();
        if (!server.listen(addr, port)) {
            qWarning() << "Aplikasi telah dijalankan!";
            QMessageBox::warning(nullptr, QString(), "Aplikasi telah dijalankan.");
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationDisplayName(APP_DISPLAY_NAME);
    app.setApplicationVersion(APP_VERSION_STR);

    QSettings settings(APP_SETTINGS_PATH, QSettings::IniFormat);
    init_logger();
    init_locale();
    init_applocker(settings);
    init_stylesheet();

    QFile file(APP_STYLESHEET_PATH);
    file.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(file.readAll());

    db::connection(); // trigger database initialization

    MainWindow mw;
    mw.updateDatabaseInfoLabel();
    if (!LicenseManager::instance()->checkLicense()) {
        QTimer::singleShot(100, &mw, SLOT(showActivationDialog()));
    }
    else {
    // QTimer::singleShot(100, &mw, SLOT(showLoginDialog()));
        mw.autoLogin();
    }

    return app.exec();
}
