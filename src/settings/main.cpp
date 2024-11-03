#include "initialsettingsdialog.h"

#include <QApplication>
#include <QFile>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("EzPoS Settings");

    InitialSettingsDialog widget;
    widget.show();

    return app.exec();
}
