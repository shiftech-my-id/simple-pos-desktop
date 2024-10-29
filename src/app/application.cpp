#include "application.h"
#include "productmodel.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , _productModel(nullptr)
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
