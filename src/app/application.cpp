#include "application.h"
#include "productmodel.h"
#include "productcategorymodel.h"
#include "usermodel.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , _currentUser(nullptr)
    , _userModel(nullptr)
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

ProductModel* Application::productModel()
{
    if (_productModel == nullptr) {
        _productModel = new ProductModel(this);
    }

    return _productModel;
}

ProductCategoryModel* Application::productCategoryModel()
{
    if (_productCategoryModel == nullptr) {
        _productCategoryModel = new ProductCategoryModel(this);
    }

    return _productCategoryModel;
}

UserModel* Application::userModel()
{
    if (_userModel == nullptr) {
        _userModel = new UserModel(this);
    }

    return _userModel;
}
