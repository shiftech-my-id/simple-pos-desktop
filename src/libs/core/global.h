#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>

#define APP_NAME            "simple-pos"
#define APP_DISPLAY_NAME    "Simple POS"
#define APP_VERSION         0x010100
#define APP_VERSION_STR     "1.1.0"
#define APP_SETTINGS_PATH   APP_NAME ".ini"
#define APP_STYLESHEET_PATH APP_NAME ".qss"
#define APP_LICENSE_PATH    APP_NAME "-license"
#define APP_DEFAULT_DB_MIGRATION_PATH "database/migration"

#define APP_DEFAULT_DB_DRIVER "QSQLITE"
#define APP_DEFAULT_DB_FILENAME APP_NAME ".sqlite3"

#define SK_APPLOCKER_ADDRESS    "AppLocker/address"
#define SK_APPLOCKER_PORT       "AppLocker/port"
#define SK_APPLOCKER_TIMESTAMP  "AppLocker/timestamp"

#define SK_DATABASE_DRIVER       "Database/driver"
#define SK_DATABASE_USERNAME     "Database/user"
#define SK_DATABASE_PASSWORD     "Database/pass"
#define SK_DATABASE_HOSTNAME     "Database/host"
#define SK_DATABASE_PORT         "Database/port"
#define SK_DATABASE_DATABASENAME "Database/name"

#define SK_SmallPrinterName    "Printer/small"
#define SK_MediumPrinterName   "Printer/medium"
#define SK_GeneralPrinterName  "Printer/general"
#define SK_BarcodePrinterName  "Printer/barcode"

#define SK_BarcodeLabel_PaperWidth        "BarcodeLabel/paper-width"
#define SK_BarcodeLabel_PaperHeight       "BarcodeLabel/paper-height"
#define SK_BarcodeLabel_MarginTop         "BarcodeLabel/margin-top"
#define SK_BarcodeLabel_MarginLeft        "BarcodeLabel/margin-left"
#define SK_BarcodeLabel_ColumnCount       "BarcodeLabel/column-count"
#define SK_BarcodeLabel_RowCount          "BarcodeLabel/row-count"
#define SK_BarcodeLabel_LabelWidth        "BarcodeLabel/label-width"
#define SK_BarcodeLabel_LabelHeight       "BarcodeLabel/label-height"
#define SK_BarcodeLabel_HorizontalSpacing "BarcodeLabel/horizontal-spacing"
#define SK_BarcodeLabel_VerticalSpacing   "BarcodeLabel/vertical-spacing"
#define SK_BarcodeLabel_FontSIze          "BarcodeLabel/font-size"

#define SK_Sales_DefaultPriceId         "Sales/default-price-id"
#define SK_Sales_DefaultReceiptSize     "Sales/default-receipt-size"

#define SK_Print_BalanceSheet_Margins   "Print/balance-sheet.margins"

#define APP_TOOLBAR_ICON_SIZE 12

#if defined(CORE_LIBRARY)
#define CORE_EXPORT Q_DECL_EXPORT
#else
#define CORE_EXPORT Q_DECL_IMPORT
#endif

#if defined(WIDGETS_LIBRARY)
#define WIDGETS_EXPORT Q_DECL_EXPORT
#else
#define WIDGETS_EXPORT Q_DECL_IMPORT
#endif
#endif // GLOBAL_H
