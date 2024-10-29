#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QtAwesome.h>

class ProductModel;
class ProductCategoryModel;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);

    void initProductModel();

    ProductModel* productModel();
    ProductCategoryModel* productCategoryModel();

    inline fa::QtAwesome* qtAwesome() const { return _qtAwesome; }

private:
    fa::QtAwesome* _qtAwesome;
    ProductModel *_productModel;
    ProductCategoryModel *_productCategoryModel;
};

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

#if defined(FA_ICON)
#undef FA_ICON
#endif
#define FA_ICON(A) qApp->qtAwesome()->icon(A)

#endif // APPLICATION_H
