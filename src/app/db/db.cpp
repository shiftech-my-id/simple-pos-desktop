#include "db.h"
#include "global.h"
#include "common.h"
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSettings>

#include <random>

namespace db {

QSqlDatabase connection()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
        if (db.driverName() == "QMYSQL") {
            bool success = false;
            {
                QSqlQuery q(db);
                success = q.exec("select 1");
            }

            if (!success) {
                db.close();
                db = QSqlDatabase();
                QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            }
        }
    }

    if (!db.isValid()) {
        QSettings s(APP_SETTINGS_PATH, QSettings::IniFormat);
        db = QSqlDatabase::addDatabase(s.value(SK_DATABASE_DRIVER, APP_DEFAULT_DB_DRIVER).toString(), QSqlDatabase::defaultConnection);
        db.setDatabaseName(s.value(SK_DATABASE_DATABASENAME, APP_DEFAULT_DB_FILENAME).toString());
        if (db.driverName() != "QSQLITE") {
            db.setHostName(s.value(SK_DATABASE_HOSTNAME).toString());
            db.setPort(s.value(SK_DATABASE_PORT).toInt());
            db.setUserName(s.value(SK_DATABASE_USERNAME).toString());
            db.setPassword(s.value(SK_DATABASE_PASSWORD).toString());
        }
    }

    db.open();

    return db;
}

QSqlDatabase database()
{
    return connection();
}

void cleanup()
{
    DB_EXEC("DROP TABLE IF EXISTS settings");
    DB_EXEC("DROP TABLE IF EXISTS users");
    DB_EXEC("DROP TABLE IF EXISTS products");
    DB_EXEC("DROP TABLE IF EXISTS product_categories");
    DB_EXEC("DROP TABLE IF EXISTS parties");
    DB_EXEC("DROP TABLE IF EXISTS stock_updates");
    DB_EXEC("DROP TABLE IF EXISTS stock_update_details");
}

void seed()
{
    QSqlDatabase db = connection();

    //  Kode untuk generate dummy products
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 generator(rd()); // seed the generator
    std::uniform_int_distribution<> stockdist(1, 1000); // define the range
    std::uniform_int_distribution<> costdist(1, 3000); // define the range
    std::uniform_int_distribution<> categoryDist(1, 100); // define the range
    std::uniform_int_distribution<> activeDist(0, 1); // define the range

    QSqlQuery q(db);
    db.transaction();

    q.prepare("insert into users ( username, fullname, role, password, active) "
              "values(:username,:fullname,:role,:password,:active)");
    q.bindValue(":username", "admin");
    q.bindValue(":fullname", "Administrator");
    q.bindValue(":password", encryptPassword("12345"));
    q.bindValue(":active", true);
    q.bindValue(":role", 1);
    DB_EXEC(q);

    for (int i = 1; i <= 10; i++) {
        q.prepare("insert into users ( username, fullname, role, password, active) "
                  "values(:username,:fullname,:role,:password,:active)");
        q.bindValue(":username", "user" + QString::number(i));
        q.bindValue(":fullname", "User " + QString::number(i));
        q.bindValue(":password", encryptPassword("12345"));
        q.bindValue(":active", true);
        q.bindValue(":role", 2);
        DB_EXEC(q);
    }

    for (int i = 1; i <= 25; i++) {
        QString name = QString("Kategori %1").arg(i);
        q.prepare("insert into product_categories"
                  " (name)"
                  " values"
                  " (:name)");
        q.bindValue(":name", name);
        DB_EXEC(q);
    }

    for (int i = 1; i <= 10000; i++) {
        int cost = costdist(generator) * 100;
        q.prepare("insert into products"
                  " ( name, description, category_id, barcode, uom, type, stock, cost, price, active)"
                  " values"
                  " (:name,:description,:category_id,:barcode,:uom,:type,:stock,:cost,:price,:active)");
        q.bindValue(":name", QString("Produk %1").arg(i));
        q.bindValue(":description", QString("Deskripsi Produk %1").arg(i));
        q.bindValue(":barcode", QString::number(i));
        q.bindValue(":category_id", categoryDist(generator));
        q.bindValue(":uom", "bh");
        q.bindValue(":type", 1);
        q.bindValue(":stock", stockdist(generator));
        q.bindValue(":cost", cost);
        q.bindValue(":price", cost + ((20. / 100.) * cost));
        q.bindValue(":active", activeDist(generator));
        DB_EXEC(q);
    }

    for (int i = 1; i <= 100; i++) {
        q.prepare("insert into parties"
                  " (type, name, address, phone, active)"
                  " values"
                  " (1,:name,:address,:phone,1)");
        q.bindValue(":name", QString("Supplier %1").arg(i));
        q.bindValue(":address", QString("Alamat Supplier %1").arg(i));
        q.bindValue(":phone", QString("0812-1000-0%1").arg(QString::number(i)));
        DB_EXEC(q);
    }

    for (int i = 1; i <= 1000; i++) {
        q.prepare("insert into parties"
                  " (type, name, address, phone, active)"
                  " values"
                  " (2,:name,:address,:phone,1)");
        q.bindValue(":name", QString("Customer %1").arg(i));
        q.bindValue(":address", QString("Alamat Customer %1").arg(i));
        q.bindValue(":phone", QString("0812-2000-0%1").arg(QString::number(i)));
        DB_EXEC(q);
    }

    db.commit();
}

void reset()
{

}

void init() {
    cleanup();
    DB_EXEC("CREATE TABLE settings ("
            " id INTEGER NOT NULL DEFAULT 0,"
            " data TEXT NOT NULL DEFAULT '',"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE users ("
            " id INTEGER NOT NULL DEFAULT 0,"
            " username TEXT NOT NULL DEFAULT '',"
            " fullname TEXT NOT NULL DEFAULT '',"
            " password TEXT NOT NULL DEFAULT '',"
            " active INT NOT NULL DEFAULT 0,"
            " role INT NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE parties ("
            " id INTEGER NOT NULL DEFAULT 0,"
            " type TEXT NOT NULL DEFAULT 0,"
            " name TEXT NOT NULL DEFAULT '',"
            " active INT NOT NULL DEFAULT 0,"
            " address TEXT NOT NULL DEFAULT '',"
            " email TEXT NOT NULL DEFAULT '',"
            " phone TEXT NOT NULL DEFAULT '',"
            " balance REAL NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE product_categories ("
            " id     INTEGER NOT NULL DEFAULT 0,"
            " name   TEXT NOT NULL DEFAULT '',"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE products ("
            " id INTEGER NOT NULL DEFAULT 0,"
            " name TEXT NOT NULL DEFAULT '',"
            " description TEXT NOT NULL DEFAULT '',"
            " uom TEXT NOT NULL DEFAULT '',"
            " barcode TEXT NOT NULL DEFAULT '',"
            " cost REAL NOT NULL DEFAULT 0,"
            " price REAL NOT NULL DEFAULT 0,"
            " active INTEGER NOT NULL DEFAULT 0,"
            " stock INTEGER NOT NULL DEFAULT 0,"
            " type INTEGER NOT NULL DEFAULT 0,"
            " uom2 TEXT NOT NULL DEFAULT '',"
            " uom3 TEXT NOT NULL DEFAULT '',"
            " uom_qty2 INTEGER NOT NULL DEFAULT 0,"
            " uom_qty3 INTEGER NOT NULL DEFAULT 0,"
            " markup REAL NOT NULL DEFAULT 0,"
            " fixed_markup REAL NOT NULL DEFAULT 0,"
            " price2 REAL NOT NULL DEFAULT 0,"
            " markup2 REAL NOT NULL DEFAULT 0,"
            " fixed_markup2 REAL NOT NULL DEFAULT 0,"
            " price3 REAL NOT NULL DEFAULT 0,"
            " markup3 REAL NOT NULL DEFAULT 0,"
            " fixed_markup3 REAL NOT NULL DEFAULT 0,"
            " price_editable INTEGER NOT NULL DEFAULT 0,"
            " category_id INTEGER DEFAULT NULL,"
            " supplier_id INTEGER DEFAULT NULL,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE stock_updates("
            " id            INTEGER NOT NULL DEFAULT 0,"
            " type          INTEGER NOT NULL DEFAULT 0,"
            " datetime      TEXT NOT NULL DEFAULT '',"
            " party_id      INT NULL DEFAULT NULL,"
            " party_name    TEXT NOT NULL DEFAULT '',"
            " party_address TEXT NOT NULL DEFAULT '',"
            " total         REAL NOT NULL DEFAULT 0,"
            " discount      REAL NOT NULL DEFAULT 0,"
            " total_payment REAL NOT NULL DEFAULT 0,"
            " total_paid    REAL NOT NULL DEFAULT 0,"
            " total_cost    REAL NOT NULL DEFAULT 0,"
            " total_profit  REAL NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE stock_update_details("
            " id             INTEGER NOT NULL DEFAULT 0,"
            " update_id      INTEGER NOT NULL DEFAULT 0,"
            " product_id     INTEGER NOT NULL DEFAULT 0,"
            " product_name   TEXT NOT NULL DEFAULT '',"
            " product_uom    TEXT NOT NULL DEFAULT '',"
            " quantity       INTEGER NOT NULL DEFAULT 0,"
            " cost           REAL NOT NULL DEFAULT 0,"
            " price          REAL NOT NULL DEFAULT 0,"
            " subtotal_cost  REAL NOT NULL DEFAULT 0,"
            " subtotal_price REAL NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    db::seed();
}

bool exec(const QString &str, const char* file, int line, const char *fn)
{
    QSqlQuery q(connection());
    q.prepare(str);
    return exec(q, file, line, fn);
}

bool exec(QSqlQuery &q, const char* file, int line, const char */*fn*/)
{
    if (!q.exec()) {
        const QString title("SQL Error");
        const QString text("Terdapat kesalahan saat mengeksekusi query.");
        const QString infoText(QString("file: %2:%3").arg(file, QString("%1").arg(line)));
        const QString detailedText(QString("SQL: '%1'. Database error text: %2").arg(q.lastQuery(), q.lastError().text()));

        qCritical() << title << text << "\nInfo:" << infoText << "\nDetail: " << detailedText;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle(title);
        msgBox.setText(text);
        msgBox.setInformativeText(infoText);
        msgBox.setDetailedText(detailedText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return false;
    }

    return true;
}



}
