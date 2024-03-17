#ifndef UNLOADDIALOG_H
#define UNLOADDIALOG_H

#include <QDialog>

namespace Ui {
    class UnloadDialog;
}

//!
//! \brief The UnloadDialog class
//!
class UnloadDialog : public QDialog
{
    Q_OBJECT

public:
    //!
    //! \brief UnloadDialog
    //! \param parent
    //!
    explicit UnloadDialog (QWidget *parent = nullptr);

    //!
    ~UnloadDialog ();

public slots:
    //!
    //! \brief slotTerminate
    //!
    void slotTerminate ();

private:
    //!
    Ui::UnloadDialog *ui;
};

#endif // UNLOADDIALOG_H
