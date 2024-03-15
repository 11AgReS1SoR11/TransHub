#include "UnloadDialog.h"
#include "ui_UnloadDialog.h"

UnloadDialog::UnloadDialog (QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::UnloadDialog)
{
    ui->setupUi (this);
}

UnloadDialog::~UnloadDialog ()
{
    delete ui;
}

void UnloadDialog::slotTerminate ()
{
    if (!this->isVisible ())
        show ();
}
