#include "toptoolbar.h"

TopToolBar::TopToolBar(QToolBar *parent): QToolBar(parent)
{

}

TopToolBar::TopToolBar(const QString &title, QWidget *parent):QToolBar(title, parent)
{
    cars = new QComboBox();
    cars->addItem(tr("car1"));
    cars->addItem(tr("car2"));

    products = new QComboBox();
    products->addItem(tr("product1"));
    products->addItem(tr("product1"));

    addWidget(cars);
    addSeparator();
    addWidget(products);
}

TopToolBar::~TopToolBar()
{

}
