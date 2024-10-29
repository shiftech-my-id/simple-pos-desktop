#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QtAwesome.h>

class User;
class UserModel;
class SupplierModel;
class ProductModel;
class ProductCategoryModel;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);

    void initProductModel();

    void setCurrentUser(User* user);
    inline User* currentUser() const { return _currentUser; }
    SupplierModel* supplierModel();
    ProductModel* productModel();
    ProductCategoryModel* productCategoryModel();
    UserModel* userModel();

    inline fa::QtAwesome* qtAwesome() const { return _qtAwesome; }

private:
    fa::QtAwesome* _qtAwesome;
    User* _currentUser;
    UserModel* _userModel;
    SupplierModel* _supplierModel;
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
