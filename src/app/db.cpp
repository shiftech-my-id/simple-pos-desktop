#include "db.h"

#include <QMessageBox>
#include <QSqlError>
#include <QCryptographicHash>
#include <QVariant>
#include <random>

namespace db {

QSqlDatabase database()
{
    return QSqlDatabase::database();
}

void init() {
    // Inisialisasi database
    // File database akan buat secara otomatis jika file database tidak tersedia
    QSqlDatabase db = QSqlDatabase::addDatabase(DB_DRIVER);
    db.setDatabaseName(DB_FILENAME);
    db.open();
}

void resetData()
{
    QSqlDatabase db = QSqlDatabase::database();

    DB_EXEC("DROP TABLE IF EXISTS stock_update_details");
    DB_EXEC("DROP TABLE IF EXISTS stock_updates");
    DB_EXEC("DROP TABLE IF EXISTS parties");
    DB_EXEC("DROP TABLE IF EXISTS products");
    DB_EXEC("DROP TABLE IF EXISTS product_categories");
    DB_EXEC("DROP TABLE IF EXISTS users");

    DB_EXEC("CREATE TABLE users ("
            " id     INTEGER NOT NULL DEFAULT 0,"
            " username TEXT NOT NULL DEFAULT 0,"
            " password TEXT NOT NULL DEFAULT '',"
            " active INT NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE parties ("
            " id     INTEGER NOT NULL DEFAULT 0,"
            " type   TEXT NOT NULL DEFAULT 0,"
            " name   TEXT NOT NULL DEFAULT '',"
            " active INT NOT NULL DEFAULT 0,"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE product_categories ("
            " id     INTEGER NOT NULL DEFAULT 0,"
            " name   TEXT NOT NULL DEFAULT '',"
            " PRIMARY KEY(id AUTOINCREMENT)"
            ")");
    DB_EXEC("CREATE TABLE products ("
            " id     INTEGER NOT NULL DEFAULT 0,"
            " category_id INTEGER DEFAULT NULL,"
            " name   TEXT NOT NULL DEFAULT '',"
            " barcode TEXT NOT NULL DEFAULT '',"
            " uom    TEXT NOT NULL DEFAULT '',"
            " type   INTEGER NOT NULL DEFAULT 0,"
            " stock  INTEGER NOT NULL DEFAULT 0,"
            " cost   REAL NOT NULL DEFAULT 0,"
            " price  REAL NOT NULL DEFAULT 0,"
            " active INTEGER NOT NULL DEFAULT 0,"
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

    //  Kode untuk generate dummy products
       std::random_device rd; // obtain a random number from hardware
       std::mt19937 generator(rd()); // seed the generator
       std::uniform_int_distribution<> stockdist(1, 1000); // define the range
       std::uniform_int_distribution<> costdist(1, 3000); // define the range

       QSqlQuery q(db);
       db.transaction();

       q.prepare("insert into users ( username, password, active) "
                 "values(:username,:password,:active)");
       q.bindValue(":username", "admin");
       q.bindValue(":password", QCryptographicHash::hash("12345", QCryptographicHash::Sha1).toHex());
       q.bindValue(":active", true);
       DB_EXEC(q);

       for (int i = 1; i <= 10; i++) {
           q.prepare("insert into users ( username, password, active) "
                     "values(:username,:password,:active)");
           q.bindValue(":username", "user" + QString::number(i));
           q.bindValue(":password", QCryptographicHash::hash("12345", QCryptographicHash::Sha1).toHex());
           q.bindValue(":active", true);
           DB_EXEC(q);
       }

       for (int i = 1; i <= 10000; i++) {
           QString name = QString("Produk %1").arg(i);
           int cost = costdist(generator) * 100;
           q.prepare("insert into products"
                     " (name, barcode, uom, type, stock, cost, price, active)"
                     " values"
                     " (:name,:barcode,:uom,:type,:stock,:cost,:price,1)");
           q.bindValue(":name", name);
           q.bindValue(":barcode", QString::number(i));
           q.bindValue(":uom", "bh");
           q.bindValue(":type", 1);
           q.bindValue(":stock", stockdist(generator));
           q.bindValue(":cost", cost);
           q.bindValue(":price", cost + ((20. / 100.) * cost));
           DB_EXEC(q);
       }
       db.commit();
}

bool exec(const QString &str, const QString& file, int line)
{
    QSqlQuery q(database());
    q.prepare(str);
    return exec(q, file, line);
}

bool exec(QSqlQuery &q, const QString& file, int line)
{
    if (!q.exec()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Sql Error");
        msgBox.setText("Terdapat kesalahan saat mengeksekusi query.<br>");
        msgBox.setInformativeText(QString("file: %2:%3<br>")
                                  .arg(file, QString("%1").arg(line)));
        msgBox.setDetailedText(QString("SQL: '%1'. "
                                       "Database error text: %2")
                               .arg(q.lastQuery(), q.lastError().text()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return false;
    }

    return true;
}

}
