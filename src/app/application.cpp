#include "application.h"
#include "productmodel.h"
#include "productcategorymodel.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , _productModel(nullptr)
    , _productCategoryModel(nullptr)
{
    _qtAwesome = new fa::QtAwesome(qApp);
    _qtAwesome->initFontAwesome();
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
