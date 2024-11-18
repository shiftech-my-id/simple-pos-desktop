#include "application.h"
#include "gui/productmodel.h"
#include "gui/productcategorymodel.h"
#include "gui/usermodel.h"
#include "gui/suppliermodel.h"
#include "gui/customermodel.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , _currentUser(nullptr)
    , _userModel(nullptr)
    , _customerModel(nullptr)
    , _supplierModel(nullptr)
    , _productModel(nullptr)
    , _productCategoryModel(nullptr)
{
    _qtAwesome = new fa::QtAwesome(qApp);
    _qtAwesome->initFontAwesome();
}

void Application::setCurrentUser(User* user)
{
    if (_currentUser) {
        delete _currentUser;
    }

    _currentUser = user;
}

CustomerModel* Application::customerModel()
{
    if (_customerModel == nullptr) {
        _customerModel = new CustomerModel(this);
    }

    return _customerModel;
}

SupplierModel* Application::supplierModel()
{
    if (_supplierModel == nullptr) {
        _supplierModel = new SupplierModel(this);
        _supplierModel->refresh();
    }

    return _supplierModel;
}

ProductModel* Application::productModel()
{
    if (_productModel == nullptr) {
        _productModel = ProductModel::instance();
        _productModel->refresh();
    }

    return _productModel;
}

ProductCategoryModel* Application::productCategoryModel()
{
    if (_productCategoryModel == nullptr) {
        _productCategoryModel = ProductCategoryModel::instance();
        _productCategoryModel->refresh();
    }

    return _productCategoryModel;
}

UserModel* Application::userModel()
{
    if (_userModel == nullptr) {
        _userModel = UserModel::instance();
        _userModel->refresh();
    }

    return _userModel;
}
