CREATE TABLE settings (
    key TEXT UNIQUE NOT NULL DEFAULT '',
    value TEXT NOT NULL DEFAULT '',
    PRIMARY KEY(key)
);

CREATE TABLE users (
    id INTEGER NOT NULL DEFAULT 0,
    username TEXT NOT NULL DEFAULT '',
    fullname TEXT NOT NULL DEFAULT '',
    password TEXT NOT NULL DEFAULT '',
    active INT NOT NULL DEFAULT 0,
    role INT NOT NULL DEFAULT 0,
    PRIMARY KEY(id AUTOINCREMENT)
);

CREATE TABLE parties (
    id INTEGER NOT NULL DEFAULT 0,
    type TEXT NOT NULL DEFAULT 0,
    name TEXT NOT NULL DEFAULT '',
    active INT NOT NULL DEFAULT 0,
    address TEXT NOT NULL DEFAULT '',
    email TEXT NOT NULL DEFAULT '',
    phone TEXT NOT NULL DEFAULT '',
    balance REAL NOT NULL DEFAULT 0,
    PRIMARY KEY(id AUTOINCREMENT)
);

CREATE TABLE product_categories (
    id     INTEGER NOT NULL DEFAULT 0,
    name   TEXT NOT NULL DEFAULT '',
    PRIMARY KEY(id AUTOINCREMENT)
);
CREATE TABLE products (
    id INTEGER NOT NULL DEFAULT 0,
    name TEXT NOT NULL DEFAULT '',
    description TEXT NOT NULL DEFAULT '',
    uom TEXT NOT NULL DEFAULT '',
    barcode TEXT NOT NULL DEFAULT '',
    cost REAL NOT NULL DEFAULT 0,
    price REAL NOT NULL DEFAULT 0,
    active INTEGER NOT NULL DEFAULT 0,
    stock INTEGER NOT NULL DEFAULT 0,
    type INTEGER NOT NULL DEFAULT 0,
    uom2 TEXT NOT NULL DEFAULT '',
    uom3 TEXT NOT NULL DEFAULT '',
    uom_qty2 INTEGER NOT NULL DEFAULT 0,
    uom_qty3 INTEGER NOT NULL DEFAULT 0,
    markup REAL NOT NULL DEFAULT 0,
    fixed_markup REAL NOT NULL DEFAULT 0,
    price2 REAL NOT NULL DEFAULT 0,
    markup2 REAL NOT NULL DEFAULT 0,
    fixed_markup2 REAL NOT NULL DEFAULT 0,
    price3 REAL NOT NULL DEFAULT 0,
    markup3 REAL NOT NULL DEFAULT 0,
    fixed_markup3 REAL NOT NULL DEFAULT 0,
    price_editable INTEGER NOT NULL DEFAULT 0,
    category_id INTEGER DEFAULT NULL,
    supplier_id INTEGER DEFAULT NULL,
    PRIMARY KEY(id AUTOINCREMENT)
);
CREATE TABLE stock_updates(
    id            INTEGER NOT NULL DEFAULT 0,
    type          INTEGER NOT NULL DEFAULT 0,
    datetime      TEXT NOT NULL DEFAULT '',
    party_id      INT NULL DEFAULT NULL,
    party_name    TEXT NOT NULL DEFAULT '',
    party_address TEXT NOT NULL DEFAULT '',
    total         REAL NOT NULL DEFAULT 0,
    discount      REAL NOT NULL DEFAULT 0,
    total_payment REAL NOT NULL DEFAULT 0,
    total_paid    REAL NOT NULL DEFAULT 0,
    total_cost    REAL NOT NULL DEFAULT 0,
    total_profit  REAL NOT NULL DEFAULT 0,
    PRIMARY KEY(id AUTOINCREMENT)
);
CREATE TABLE stock_update_details(
    id             INTEGER NOT NULL DEFAULT 0,
    update_id      INTEGER NOT NULL DEFAULT 0,
    product_id     INTEGER NOT NULL DEFAULT 0,
    product_name   TEXT NOT NULL DEFAULT '',
    product_uom    TEXT NOT NULL DEFAULT '',
    quantity       INTEGER NOT NULL DEFAULT 0,
    cost           REAL NOT NULL DEFAULT 0,
    price          REAL NOT NULL DEFAULT 0,
    subtotal_cost  REAL NOT NULL DEFAULT 0,
    subtotal_price REAL NOT NULL DEFAULT 0,
    PRIMARY KEY(id AUTOINCREMENT)
);
