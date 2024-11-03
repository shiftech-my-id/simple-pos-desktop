#ifndef LEFTTABWIDGET_H
#define LEFTTABWIDGET_H

#include <QWidget>

class QButtonGroup;
class QAbstractButton;

namespace Ui {
class LeftTabWidget;
}

class LeftTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeftTabWidget(QWidget *parent = nullptr);
    ~LeftTabWidget();

    int addWidget(const QString& label, const QIcon& icon, QWidget* widget);

    void setCurrentIndex(int i);

private slots:
    void onButtonToggled(bool);

private:
    Ui::LeftTabWidget *ui;
    QButtonGroup *buttonGroup;
};

#endif // LEFTTABWIDGET_H
